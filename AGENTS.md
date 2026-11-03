# AGENTS.md

Qt 6 / C++20 GUI around the hashcat CLI. `main.cpp` -> `MainWindow` (`src/mainwindow.ui`) ->
`collectHashcatOptions()` -> `HashcatOptions` -> `CommandBuilder::build()` -> hashcat argv.
Keep argv order stable: preview, clipboard and tests depend on it.

## Verify

Run from the repo root, in order:

1. `cmake -B build` — only if `build/` is not configured.
2. `cmake --build build --target clang-format` — rewrites in place, so before building/tests.
3. `cmake --build build -j$(nproc)` — must compile after formatting.
4. `ctest --test-dir build --output-on-failure --no-tests=error` — full suite.
5. `cmake --build build --target clang-tidy -j$(nproc)` — `src/` and `tests/`.
6. `cmake --build build --target clazy -j$(nproc)` — `src/` and `tests/`.

Analyzer targets run one job per file (hence `-j`) and are warning-only: fix or justify every
finding. Use `desktop-file-validate hashcat-gui.desktop` when the desktop entry or icons change.

**Prerequisites:** Qt 6.8+ (Core, Gui, Widgets, Concurrent, Network), C++20, GCC or Clang (no
MSVC), `clang-format`, `clang-tidy`, `clazy-standalone`, and hashcat 8.0.0+ at runtime. Qt6
LinguistTools is optional (no `.qm`/`update_translations` without it), as is Qt6 Test (no tests
built, so `--no-tests=error` fails). A missing target means a missing tool: install it, then
re-run `cmake -B build`.

## Test and run

- One suite: `ctest --test-dir build -R '^tst_commandbuilder$' --output-on-failure`.
- One case: `QT_QPA_PLATFORM=offscreen ./build/tst_mainwindow freshWindowMatchesTheDefaultOptions`
  (CTest sets the env var and a 60 s timeout; direct runs need it).
- `./build/hashcat-gui`; `--help` and `--version` exit before the window exists.
- A real `MainWindow` freezes offscreen (an empty `hashcatPath` queues the modal Settings dialog,
  non-JSON output opens a modal warning), so `processEvents()` blocks. For screenshots redirect
  XDG dirs and point `hashcatPath` at a stub with valid machine-readable JSON
  (`TestEnvironment::redirectBaseDirectories()`, `tst_helperutils.cpp`); for layout only,
  instantiate `Ui::MainWindow`. Harnesses stay in `/tmp`.

## Structure

- `hashcat-gui-core` is an OBJECT library with every app source except `main.cpp`; executable
  and tests link it. Keep it OBJECT: a static library drops qrc objects and static initializers.
- Register new production sources in `HASHCAT_GUI_SOURCES`, new `tests/tst_*.cpp` in
  `HASHCAT_GUI_TESTS`. No globbing.
- UI-free logic (CommandBuilder, HashcatInfoParser, HelperUtils, StateFile,
  WidgetStateSerializer, SettingsManager, UpdateChecker, PathMonitor) stays testable;
  `HelperUtils` runs hashcat/terminal.
- Attack-mode gating predicates live in `CommandBuilder`; `modeRequirements()` derives mode hints
  and launch validation. Launch is `HelperUtils::validateOptions()` then `validateLaunch()`;
  hashcat starts detached (`CREATE_NEW_CONSOLE` on Windows).
- Example sessions in `src/examples.h`: one `Entry` per attack mode in combo order; paths use
  `Examples::DataPrefix` and unpack into `Examples::DataDirectory`.
- `PathMonitor`: wordlist and PCFG rulesets may be folders (wordlist needs a file, ruleset needs
  `Grammar/grammar.txt`), tables are file-only, and a bare name like `default-passwords` is
  resolved by hashcat; unusable paths get a warning icon.
- Unchecked wordlist/ruleset/table/feed rows keep their place and session entry but are omitted
  from the command and launch validation.
- `UpdateChecker` is async; tests use a loopback `QTcpServer` and a `TestAboutDialog`-injected
  checker, never GitHub.
- Hash types load asynchronously from `hashcat --example-hashes --machine-readable`;
  `m_pendingHashTypeId` applies a session's type later. Never block the GUI thread with a hashcat
  query; read `QSettings` on the calling thread before `QtConcurrent` (it is only reentrant).

## Adding a hashcat option

Four places, then preview and tests:
- a field in `HashcatOptions` (`src/hashcatoptions.h`), defaulting to the widget's start value;
- a `CommandBuilder::Parameter`, `parameterName()` entry and `build()` case; omit it at its
  default so the preview stays quiet;
- the widget in `src/mainwindow.ui` and its read in `MainWindow::collectHashcatOptions()`;
- a case in `tests/tst_commandbuilder.cpp`.

Connect to `commandChanged()` once; the attack-mode combo uses `attackIndexChanged()` instead.

## Persistence

- `WidgetStateSerializer` keys on widget object names, which are the session format. Combo values
  are item data (attack/hash id), never display text; a combo without item data
  falls back to position, an unknown identifier is left untouched.
- `StateFile` owns only the frozen envelope. Section versions bump on a widget/setting *rename*,
  never on an addition; section names and `session.json` are constants (renaming one orphans saved
  files). A removed widget goes to `AppConstants::Sessions::RetiredWidgetNames` (never reuse it)
  without a version bump. Refuse newer versions, apply older ones naming unknown/wrong-typed
  values, never destroy a file this version cannot understand; writes replace the whole file.
- `SettingsManager` wraps `QSettings::IniFormat`; keys, types and defaults are
  `AppConstants::SettingsKeys::All` (the export/import read). Never set `QCoreApplication`'s
  organization name: it moves `QStandardPaths::AppDataLocation` and the session file.

## Conventions

- SPDX `GPL-3.0-or-later` header in every source, header and test.
- Production defines `QT_NO_CAST_FROM_ASCII`/`QT_NO_CAST_TO_ASCII`: use `QStringLiteral`,
  `QLatin1StringView` or `QString::fromUtf8` (tests exempt).
- Protocol tokens, settings keys, defaults and file names go in `src/appconstants.h`; fixed
  hashcat syntax/help tables stay non-translatable. Non-QObject classes needing `tr()` use
  `Q_DECLARE_TR_FUNCTIONS`.
- Style is `.clang-format`; the target globs `src/` and `tests/`, so new files are formatted
  automatically but still need registration. Comments carry the *why*, not the what, and stay
  short.
- Wrap user-visible text in `tr()`. Catalogs use `lupdate -no-obsolete -locations none`. Don't run
  `update_translations` during development - it rewrites every `.ts` and CI/tests don't check
  freshness; refresh once before a release. A new language updates `HASHCAT_GUI_TRANSLATIONS` and
  `AppConstants::Languages::Available` (English has no catalog); `tst_translations` enforces the
  match but skips without embedded `.qm`.
- Tests use `TestEnvironment::run`, not `QTEST_MAIN` (it redirects XDG dirs). Private access goes
  through a friend class declared in the header under test; add a friend instead of widening the
  API. Shell-script hashcat stubs cannot run on Windows: `QSKIP` those cases.
- Commit subject ≤72 chars, capitalized, imperative, no period; body only for what and why.
- `FAQ.md` is generated: edit `FAQ.yaml`, then `mdfaqgen FAQ.yaml FAQ.md`.

## CI and packaging

- CI: GCC and Clang on Linux and Windows, both run `ctest --no-tests=error`.
- OBS and the Windows release use `-DBUILD_TESTING=OFF -DHASHCAT_GUI_REQUIRE_TRANSLATIONS=ON`:
  missing LinguistTools is a configure error and a failing `lrelease` fails the build (tests are
  CI's job). Windows binary: `build-release.yml`; Linux packages: OBS from `obs/`.
