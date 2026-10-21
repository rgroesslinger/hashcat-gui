# AGENTS.md

Qt 6 / C++20 GUI around the hashcat CLI. `main.cpp` -> `MainWindow` -> the widgets
in `src/mainwindow.ui` -> `HashcatOptions` -> `CommandBuilder::build()` -> hashcat argv.

## Verification (required for every code change)

Run from the repo root, in order. The change is not complete until all pass:

1. `cmake -B build` — only if `build/` is not configured yet.
2. `cmake --build build --target clang-format` — rewrites sources in place, so it must precede steps 3–4.
3. `cmake --build build` — must compile after formatting.
4. `ctest --test-dir build --output-on-failure --no-tests=error` — full suite.
5. `cmake --build build --target clang-tidy` — analyzes `src/` and `tests/`.
6. `cmake --build build --target clazy` — analyzes `src/` and `tests/`.

**Prerequisites:** Qt 6.8+, C++20, `clang-format`, `clang-tidy`, `clazy-standalone`, and two Qt modules:
- Qt6 LinguistTools — needed for the `update_translations` target and for the `.qm`
  files in the binary. Without it the build still works, it just ships no catalogs.
- Qt6 Test — without it the suite silently runs nothing (hence `--no-tests=error`).

A missing target means a missing tool: install it (or tell the user), never skip the check.
After installing, re-run `cmake -B build`.

`clang-tidy` and `clazy` are warning-only (`WarningsAsErrors` is empty), so exit status 0 does
not mean clean. Read the output; fix or explicitly justify every finding. CI does not run
these three targets — this checklist is the only enforcement.

**Conditional checks:**
- Release preparation — the translation catalogs are refreshed and committed → see
  **Translations** below.
- Desktop entry or icon metadata changed → `desktop-file-validate hashcat-gui.desktop`.
- Structural/architectural change (source lists, build system, translations, packaging) →
  check that `build-release.yml`, the CI workflow, and `obs/` are still valid.

## Build and test

- Build defaults to Release. Configure/build: `cmake -B build && cmake --build build`.
- One suite: `ctest --test-dir build -R '^tst_commandbuilder$' --output-on-failure`.
- One test case: `QT_QPA_PLATFORM=offscreen ./build/tst_mainwindow freshWindowMatchesTheDefaultOptions`
  (CTest sets `QT_QPA_PLATFORM=offscreen` and a 60 s timeout; direct runs need the variable).
- Run the app: `./build/hashcat-gui`. `--help` and `--version` exit before creating the window.

### Looking at the UI
- `QT_QPA_PLATFORM=offscreen` plus `QWidget::grab()` works, but a real `MainWindow`
  freezes: an empty `hashcatPath` queues the modal Settings dialog, and a hashcat
  reply that is not valid JSON opens a modal warning — `processEvents()` then blocks
  inside the modal loop. Prepare the environment first: application name, redirected
  XDG directories, `hashcatPath` pointing at a stub that prints valid
  machine-readable JSON (see `TestEnvironment::redirectBaseDirectories()` and the
  stubs in `tst_helperutils.cpp`).
- For layout measurements skip `MainWindow` altogether: include the generated
  `build/hashcat-gui-core_autogen/include/ui_mainwindow.h`, instantiate
  `Ui::MainWindow`, call `setupUi()` on a plain `QMainWindow`, and grab that. No
  settings, hashcat or dialogs involved.
- Keep screenshot harnesses in `/tmp`; they are not part of the repo.

## Structure

- `hashcat-gui-core` is an OBJECT library with every app source except `main.cpp`; the
  executable and all tests link it. Keep it an object library: a static one would
  drop the qrc objects and static initializers as unreferenced.
- New production `.cpp`/`.h`/`.ui`/`.qrc` files go in `HASHCAT_GUI_SOURCES` (no globbing).
  New tests (one `tests/tst_*.cpp` each, linked against core) go in `HASHCAT_GUI_TESTS`.
- The wiring in one line: `main.cpp` starts `MainWindow`, which owns the widgets in
  `mainwindow.ui`; `collectHashcatOptions()` reads them into `HashcatOptions`;
  `CommandBuilder::build()` turns that into hashcat argv; `HelperUtils` talks to hashcat
  and to the terminal; `WidgetStateSerializer` maps the widgets to the values of a
  session and `StateFile` writes them into a file. Keep the argument order stable: the
  preview, the clipboard and the tests all depend on it. The logic that needs no UI
  lives in `CommandBuilder`, `HashcatInfoParser`, `HelperUtils`, `StateFile` and
  `WidgetStateSerializer`, so it stays testable.
- Attack-mode gating predicates live in `CommandBuilder` and are reused for group-box
  enablement in `MainWindow`. Do not duplicate them. `CommandBuilder::modeRequirements()`
  derives what a mode needs from those predicates, so the group boxes, the mode hint and
  the launch validation cannot disagree.
- Launch validation: `HelperUtils::validateOptions()` (what the selected mode needs) and
  then `HelperUtils::validateLaunch()` (the path, the binary and the terminal). hashcat
  starts detached in an external terminal (`CREATE_NEW_CONSOLE` on Windows).

### Adding a hashcat option
The option touches four places, and the command preview and the tests follow it:
- a field in `HashcatOptions` (`src/hashcatoptions.h`), defaulting to what the widget
  in `mainwindow.ui` starts with;
- a `CommandBuilder::Parameter` plus its entry in `parameterName()` and its case in
  `build()` (`src/commandbuilder.*`) — the builder leaves an option out when it is at
  its default, so the preview stays free of noise;
- the widget in `src/mainwindow.ui` and the read in `MainWindow::collectHashcatOptions()`;
- a case in `tests/tst_commandbuilder.cpp`.
Connect the new widget to `commandChanged()` once — the attack mode combo is the only
exception, it goes through `attackIndexChanged()`.

### Threading and async
- Hash types load asynchronously from `hashcat --example-hashes --machine-readable`; the
  hash combo is empty until the query returns. Profiles use `m_pendingHashTypeId` for this.
- Never block the GUI thread with a hashcat query.
- `QSettings` is only reentrant: read it on the calling thread before handing work to
  `QtConcurrent`.

### WidgetStateSerializer
- Turns a widget tree into a JSON object of values and back. The children it knows
  (`QLineEdit`, `QCheckBox`, `QComboBox`, `QRadioButton`, `QSpinBox`, `QDoubleSpinBox`,
  `QListWidget`) are addressed by `objectName`. The names are part of the session
  format: renaming one breaks saved profiles and tests.
- `applyToWidgets()` returns the names of the values that no widget claimed — the values
  of a file that was made by another version, or that was edited by hand. The caller
  names them for the user.
- A `QComboBox` is stored by the item data the app gave the entry (attack mode id, hash type
  id, workload level) — never by display text (translatable). Only a combo with no item data
  at all falls back to position, which is not an identifier: inserting mid-list shifts every
  later entry. An unknown identifier leaves the combo untouched.
- Transient widgets (e.g. command preview) are excluded via `AppConstants::Files::PreviewWidget`.
- The file is not the serializer's business: that is `StateFile`.

### StateFile
- The file the application writes and reads again: the session it keeps between two runs
  and the settings the user exports share one envelope (`createdWith`, `sections`, and per
  section a `version` and its `values`).
- The names of the sections are constants (`AppConstants::FileSections`), never the name
  of a class: the session file is named after its class today, so renaming that class
  would orphan every profile that was ever written.
- Each section has its own version (`AppConstants::Profiles::Version` for the session,
  `AppConstants::Settings::Version` for the settings), because the widget names and the
  setting keys change independently of each other. Bump the version of a section whenever
  one of its keys is renamed.
- The class owns the envelope and nothing else: it knows neither the widgets nor the
  settings of a section, and it does not know the version of one — the caller passes it
  in and compares what it reads.
- A file of another section is refused with a message that names it; a file that holds
  more than one section is read for the one that is wanted, and the others are named.

### Settings
- `QSettings::NativeFormat` under the application name; keys go in `AppConstants::SettingsKeys`.
- Do not set `QCoreApplication`'s organization name: it changes
  `QStandardPaths::AppDataLocation` and the default-profile path.

## Conventions

**Code**
- Every source, header, and test starts with the SPDX `GPL-3.0-or-later` header.
- Production code uses `QT_NO_CAST_FROM_ASCII`/`QT_NO_CAST_TO_ASCII`: use `QStringLiteral`,
  `QLatin1StringView`, or `QString::fromUtf8` (tests exempt).
- Protocol tokens, settings keys, defaults, and file names go in `src/appconstants.h`.
  Fixed hashcat syntax/help tables are deliberately non-translatable.
- Non-`QObject` classes needing `tr()` use `Q_DECLARE_TR_FUNCTIONS(...)`.
- Style: 4-space indent, 100 columns, Allman functions, operators wrapped at line start, no
  include sorting (`clang-format` enforces it).
- Comments carry the *why* the code doesn't state (non-obvious constraint, platform quirk,
  trade-off). Drop anything that restates the code. Prefer short.

**Translations**
- Wrap user-visible text in `tr()`. Catalogs are refreshed with
  `lupdate -no-obsolete -locations none`: no file/line refs (moving code doesn't touch
  them) and no entries left behind whose source string is gone.
- **During development the catalogs are not maintained.** Do not run
  `cmake --build build --target update_translations`: it rewrites every
  `translations/*.ts` file, so a two-line change turns into a diff of hundreds of
  lines nobody asked for. The catalogs may lag behind the sources, and nothing —
  neither CI nor the tests — complains about outdated or unfinished translations.
- **Before a release** the catalogs are refreshed once, by hand:
  `cmake --build build --target update_translations`. Commit the refreshed
  `translations/*.ts` files with the release; new strings arrive untranslated.
- Adding a language: update both `HASHCAT_GUI_TRANSLATIONS` and
  `AppConstants::Languages::Available` (English has no catalog); `tst_translations`
  enforces they match.

**Tests**
- Use `TestEnvironment::run`, not `QTEST_MAIN`; it redirects XDG config/data/cache dirs so
  tests never touch real settings.
- Private members are reached through a friend class declared in the header under test
  (`friend class TestMainWindow;`, `friend class TestSettingsDialog;`). A new test that
  needs private access declares its own friend instead of widening the public API.
- Shell-script hashcat stubs don't run on Windows: `QSKIP` those cases.

**Repo hygiene**
- `FAQ.md` is generated: edit `FAQ.yaml` only, then `mdfaqgen FAQ.yaml FAQ.md`
  ([mdfaqgen](https://github.com/rgroesslinger/mdfaqgen); it asks before overwriting).
- A user-visible change gets a `CHANGELOG.md` entry under the "Unreleased" heading.
- `build/` is gitignored. Never commit generated `ui_*.h`, `.qm`, or binaries.

## Commit messages

- Subject: one line, ≤72 chars, capitalized, imperative verb (`Add`, `Fix`, `Move`, `Use`,
  `Store`, `Keep`), no trailing period.
- Body only if the subject isn't enough: what and why, as prose or short bullets, wrapped at
  72. Don't restate the diff.

## CI and packaging

- CI: GCC and Clang on Linux and Windows; `ctest --no-tests=error`; on Linux also runs
  `desktop-file-validate hashcat-gui.desktop`. CI does not look at the translation
  catalogs at all — they are refreshed and committed by hand before a release.
- Windows binary: `build-release.yml` on GitHub release publish. Linux packages: built
  externally on openSUSE Build Service from `obs/`.
