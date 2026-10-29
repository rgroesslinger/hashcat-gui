# AGENTS.md

Qt 6 / C++20 GUI around the hashcat CLI. `main.cpp` -> `MainWindow` -> widgets in
`src/mainwindow.ui` -> `HashcatOptions` -> `CommandBuilder::build()` -> hashcat argv.

## Verification

Run from the repo root, in order; the change is not complete until all pass:

1. `cmake -B build` — only if `build/` is not configured.
2. `cmake --build build --target clang-format` — rewrites in place, so before building/tests.
3. `cmake --build build -j$(nproc)` — must compile after formatting.
4. `ctest --test-dir build --output-on-failure --no-tests=error` — full suite.
5. `cmake --build build --target clang-tidy -j$(nproc)` — `src/` and `tests/`.
6. `cmake --build build --target clazy -j$(nproc)` — `src/` and `tests/`.

Both analyzer targets are an aggregate of one target per source file, so `-j` analyzes the
files in parallel. They are warning-only (`WarningsAsErrors` empty): read the output and fix
or justify every finding. CI runs none of the `clang-format`, `clang-tidy` or `clazy` targets;
this list is the only enforcement.

**Prerequisites:** Qt 6.8+ (Core, Gui, Widgets, Concurrent, Network), C++20, `clang-format`,
`clang-tidy`, `clazy-standalone`. Optional: Qt6 LinguistTools (otherwise no `.qm` files and no
`update_translations` target) and Qt6 Test (no tests are built, so `--no-tests=error` fails). A
missing target means a missing tool: install it or tell the user, then re-run `cmake -B build`.

**Conditional checks:** release prep refreshes catalogs (see **Translations**); desktop
entry/icon changes get `desktop-file-validate hashcat-gui.desktop`; structural changes (source
lists, build system, translations, packaging) also check `build-release.yml`, CI and `obs/`.

## Build and test

- Defaults to Release for single-config generators.
  `cmake -B build && cmake --build build -j$(nproc)`.
- One suite: `ctest --test-dir build -R '^tst_commandbuilder$' --output-on-failure`.
- One case:
  `QT_QPA_PLATFORM=offscreen ./build/tst_mainwindow freshWindowMatchesTheDefaultOptions`
  (CTest sets `QT_QPA_PLATFORM=offscreen` and a 60 s timeout; direct runs need it).
- Run `./build/hashcat-gui`; `--help` and `--version` exit before the window is created.

### Looking at the UI

- Offscreen + `QWidget::grab()` works, but a real `MainWindow` freezes: an empty
  `hashcatPath` queues the modal Settings dialog, and non-JSON hashcat output opens a modal
  warning; `processEvents()` then blocks in the modal loop. Set the application name, redirect
  XDG dirs and point `hashcatPath` at a stub with valid machine-readable JSON (see
  `TestEnvironment::redirectBaseDirectories()`, stubs in `tst_helperutils.cpp`).
- For layout only, skip `MainWindow`: include
  `build/hashcat-gui-core_autogen/include/ui_mainwindow.h`, instantiate `Ui::MainWindow`, call
  `setupUi()` on a plain `QMainWindow`, and grab. Screenshot harnesses stay in `/tmp`.

## Structure

- `hashcat-gui-core` is an OBJECT library with every app source except `main.cpp`; executable
  and tests link it. Keep it OBJECT: static drops qrc objects and static initializers.
- New production `.cpp`/`.h`/`.ui`/`.qrc` files go in `HASHCAT_GUI_SOURCES` (no globbing);
  new tests (`tests/tst_*.cpp`, linked against core) go in `HASHCAT_GUI_TESTS`.
- Flow: `main.cpp` -> `MainWindow` (`mainwindow.ui`) -> `collectHashcatOptions()` ->
  `HashcatOptions` -> `CommandBuilder::build()` -> argv. `HelperUtils` runs hashcat and the
  terminal; `WidgetStateSerializer` + `StateFile` persist sessions; settings go through
  `SettingsManager::instance()`. Keep argv order stable: preview, clipboard and tests depend
  on it. UI-free logic (CommandBuilder, HashcatInfoParser, HelperUtils, StateFile,
  WidgetStateSerializer, SettingsManager, UpdateChecker, PathMonitor) stays testable.
- Attack-mode gating predicates live in `CommandBuilder` and are reused for group-box
  enablement; `modeRequirements()` derives mode hint and launch validation from them.
- Launch: `HelperUtils::validateOptions()` (mode needs), then `validateLaunch()` (path,
  binary, terminal). hashcat starts detached (`CREATE_NEW_CONSOLE` on Windows).
- `UpdateChecker` (About dialog) is async over `QNetworkAccessManager` to GitHub. Tests never
  contact GitHub: `evaluateResponse()` parses canned JSON, `tst_updatechecker` uses a loopback
  `QTcpServer`, and `AboutDialog` injects a checker via its private constructor
  (`friend class TestAboutDialog`).
- `AboutDialog` sizes to `sizeHint()` in `showEvent`, not the constructor: wrapped labels get
  their height after polish; once per open, so user resizes survive hide/show.
- `PathMonitor` watches each input's parent directory (`QFileSystemWatcher` cannot watch a
  missing path), plus the directory itself for wordlists and PCFG rulesets: an empty wordlist
  folder and a ruleset folder without `Grammar/grammar.txt` are unusable, and both can gain
  the missing entry later. Its `Kind` decides what is usable - wordlists and PCFG take a
  folder, tables never do, and a bare ruleset name such as `default-passwords` is resolved by
  hashcat and never flagged. A missing or unusable path gets a warning icon/tooltip;
  available rows stay untouched.

### Adding a hashcat option

The option touches four places; preview and tests follow:
- a field in `HashcatOptions` (`src/hashcatoptions.h`), defaulting to what the widget in
  `mainwindow.ui` starts with;
- a `CommandBuilder::Parameter`, its `parameterName()` entry and its `build()` case
  (`src/commandbuilder.*`); leave it out at its default so the preview stays quiet;
- the widget in `src/mainwindow.ui` and its read in `MainWindow::collectHashcatOptions()`;
- a case in `tests/tst_commandbuilder.cpp`.

Connect the widget to `commandChanged()` once; the attack-mode combo is the only exception,
it goes through `attackIndexChanged()`.

### Threading and async

- Hash types load asynchronously from `hashcat --example-hashes --machine-readable`; the combo
  stays empty until then. `m_pendingHashTypeId` selects a session's hash type later.
- Never block the GUI thread with a hashcat query. `QSettings` is only reentrant: read it on
  the calling thread before `QtConcurrent`.

### WidgetStateSerializer

- Converts a widget tree to/from JSON. Known children (`QLineEdit`, `QCheckBox`, `QComboBox`,
  `QRadioButton`, `QSpinBox`, `QDoubleSpinBox`, `QListWidget`) are keyed by `objectName`; those
  names are the session format. Qt-created internals and unnamed widgets are skipped.
- `applyToWidgets()` returns names no widget claimed (foreign/old file, hand edit) and names
  the widget cannot take; the caller reports both.
- Combo values are item data (attack id, hash id, workload level), never display text. Only a
  combo without item data falls back to position; an unknown identifier leaves it untouched.
- `AppConstants::Files::PreviewWidget` is excluded; the file itself is `StateFile`'s business.

### StateFile

- One envelope for session and settings: `sections`, each with `version` and `values`. Frozen:
  changing it makes older files unreadable; section versions live inside and cannot protect it.
- Writes replace the whole file, never merge: the caller decides every section (one per file
  today). A combined file would write all explicitly, not merge, which keeps stale/foreign
  sections alive.
- Section names are constants (`AppConstants::FileSections`), never class names; the session
  section was once the class name, so a rename would orphan every saved session.
- `session.json` is a constant named after its section; its version cannot protect the name —
  a rename would leave every existing session unread.
- Per-section versions (`Sessions::Version`, `Settings::Version`): widget names and setting
  keys change independently. Bump on a rename, not an addition (an added key is reported by
  name); this is not the application version.
- A newer version is refused before applying: a bump signals a rename, so a surviving name may
  mean something else. An older file is applied and unapplicable values are named; unknown or
  wrong-typed values in the same version are skipped and named, not refused.
- Never destroy an unknown file: leave it, report it at start, overwrite only when this version
  understands it.
- The class owns only the envelope (no widgets/settings); the caller passes and compares the
  version. Another section's file is refused by name; a multi-section file is read for the
  wanted one, and the others are named.

### Settings

- `SettingsManager::instance()` wraps `QSettings::IniFormat` under the application name. Keys
  and value types are in `AppConstants::SettingsKeys`; `All` is the table export/import read, so
  a new setting is one entry.
- `SettingsDialog::savedSettings()` is static and reads `SettingsManager`: the export is a
  snapshot of stored values and needs no open dialog.
- Don't set `QCoreApplication`'s organization name: it changes
  `QStandardPaths::AppDataLocation` and the session path.

## Conventions

**Code**
- SPDX `GPL-3.0-or-later` header in every source, header and test.
- Production uses `QT_NO_CAST_FROM_ASCII`/`QT_NO_CAST_TO_ASCII`: `QStringLiteral`,
  `QLatin1StringView` or `QString::fromUtf8` (tests exempt).
- Protocol tokens, settings keys, defaults and file names go in `src/appconstants.h`. Fixed
  hashcat syntax/help tables stay non-translatable.
- Non-QObject classes needing `tr()` use `Q_DECLARE_TR_FUNCTIONS(...)`.
- Style: 4-space indent, 100 columns, Allman functions, wrapped operators at line start, no
  include sorting (`clang-format` enforces it).
- Comments carry the *why* (constraint, platform quirk, trade-off), not what the code says.

**Translations**
- Wrap user-visible text in `tr()`. Catalogs use `lupdate -no-obsolete -locations none`: no
  file/line refs (moving code leaves them alone) and no obsolete source strings.
- Don't run `update_translations` during development: it rewrites every `translations/*.ts`;
  catalogs may lag and CI/tests don't check freshness or translation completeness. Before a
  release refresh once by hand and commit them; new strings arrive untranslated.
- Adding a language: update `HASHCAT_GUI_TRANSLATIONS` and
  `AppConstants::Languages::Available` (English has no catalog); `tst_translations` enforces
  the match.

**Tests**
- Use `TestEnvironment::run`, not `QTEST_MAIN`: it redirects XDG config/data/cache dirs so
  tests never touch real settings.
- Private access goes through a friend class declared in the header under test
  (`TestMainWindow`, `TestSettingsDialog`); declare a new friend instead of widening the API.
- Shell-script hashcat stubs cannot run on Windows: `QSKIP` those cases.

**Repo hygiene**
- `FAQ.md` is generated: edit `FAQ.yaml`, then `mdfaqgen FAQ.yaml FAQ.md` (mdfaqgen asks before
  overwriting).
- A user-visible change gets a `CHANGELOG.md` entry under "Unreleased".
- `build/` is gitignored; never commit `ui_*.h`, `.qm` or binaries.

## Commit messages

- Subject ≤72 chars, capitalized, imperative, no period. Body only if needed: what and why,
  wrapped at 72; don't restate the diff.

## CI and packaging

- CI: GCC and Clang on Linux and Windows; both run `ctest --no-tests=error`; the `linux-gcc`
  job also runs `desktop-file-validate`. CI does not check that catalogs are up to date or
  fully translated.
- OBS and the Windows release both build with
  `-DBUILD_TESTING=OFF -DHASHCAT_GUI_REQUIRE_TRANSLATIONS=ON`; tests are CI's job. The option
  makes missing LinguistTools a configure error, and a failing `lrelease` fails the build. It
  does not size-check the resulting `.qm` files.
- Windows binary: `build-release.yml` on release publish; Linux packages: OBS from `obs/`.
