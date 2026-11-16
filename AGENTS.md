# AGENTS.md

Qt 6 / C++20 GUI around the hashcat CLI. `main.cpp` -> `MainWindow` (`src/mainwindow.ui`) ->
`collectHashcatOptions()` (`OptionCollector`) -> `HashcatOptions` -> `CommandBuilder::build()` ->
hashcat argv. Keep argv order stable: preview, clipboard and tests depend on it.

## Verify

Run from the repo root, in order:

1. `cmake -B build` — only if `build/` is not configured.
2. `cmake --build build --target clang-format` — in place, before building/tests.
3. `cmake --build build -j$(nproc)`.
4. `ctest --test-dir build --output-on-failure --no-tests=error`.

Run `desktop-file-validate hashcat-gui.desktop` when the desktop entry or icons change.

Requires Qt 6.8+ (Core, Gui, Widgets, Concurrent, Network), C++20, GCC/Clang (no MSVC),
`clang-format`, and hashcat 8.0.0+ at runtime. LinguistTools, Qt6 Test, `clang-tidy` and
`clazy-standalone` are optional; a missing target means a missing tool, so install it and re-run
`cmake -B build`.

## Test and run

- One suite: `ctest --test-dir build -R '^tst_commandbuilder$' --output-on-failure`.
- One case: `QT_QPA_PLATFORM=offscreen ./build/tst_mainwindow freshWindowMatchesTheDefaultOptions`
  (CTest sets the env var and a 60 s timeout; direct runs need it).
- `./build/hashcat-gui`; `--help` and `--version` exit before the window exists.
- A real `MainWindow` blocks offscreen; use `MainWindowFixture`/`hashcatStub`
  (`tests/mainwindowtesthelpers.h`) for UI tests or `Ui::MainWindow` for layout only. Harnesses stay
  in `/tmp`. Main-window tests are split across `tst_mainwindow`, `tst_launch`, `tst_layout`,
  `tst_session` and `tst_settingsdialog`; use the narrow suite.

## Structure

- `hashcat-gui-core` is an OBJECT library with every app source except `main.cpp`; executable and
  tests link it. Keep it OBJECT: a static library drops qrc objects and static initializers.
- Register app sources in `HASHCAT_GUI_SOURCES` and tests in `HASHCAT_GUI_TESTS`. No globbing.
- Keep UI-free logic testable; `OptionCollector`/`AttackModeView` taking `Ui::MainWindow` is the
  documented exception (the session format is keyed on widget names).
- `CommandBuilder` owns attack-mode gating (`modeRequirements()`); `LaunchValidator::validate()`
  checks options then launch; `TerminalCatalog::launchDetached()` starts hashcat from resolved paths.
- Example sessions in `src/examples.h`: one `Entry` per mode, paths using `Examples::DataPrefix`; new
  data files go in `src/resources.qrc` and `Examples::DataFiles`.
- New `QGroupBox`es need `alignment=AlignLeft|AlignVCenter`; General/Attack tab boxes and the
  PCFG/table grids need `AlignTop` on their layout. `tst_layout` enforces both.
- `PathMonitor`: wordlists/PCFG rulesets may be folders (wordlist needs a file; ruleset needs
  `Grammar/grammar.txt`), tables are file-only, and bare names like `default-passwords` are resolved
  by hashcat.
- Unchecked wordlist/ruleset/table/feed rows stay in the session but are omitted from command and
  launch validation.
- Hash types load asynchronously: never block the GUI thread on hashcat, and read `QSettings` on the
  calling thread (it is only reentrant). `m_pendingHashTypeId` applies a session's type later.
- Hash type `-2` is the GUI-only Auto-detect sentinel (`AppConstants::Hashcat::AutoDetectHashType`);
  any negative `hashType` omits `-m`, the only autodetect trigger. Unknown session ids fall back to
  Auto-detect with a warning.

## Launch

- `Executable::resolve()` is the only path from a configured program to `QProcess`; empty values,
  bare PATH names, missing files and non-executable paths resolve to empty. Settings text is
  display-only.
- `build()` inserts `--` before the positional tail only when a token starts with `-`; ordinary argv
  stays byte-identical. `LaunchValidator` refuses NUL and every C0/C1 control except tab, and
  distinguishes `HashcatNotExecutable` from a missing file.
- Windows runs hashcat via `TerminalCatalog::powerShellScript()` as a `-EncodedCommand` script (also
  Tools → Copy as PowerShell Script); user data stays inside it, never `-Command`. Overlong commands
  are `CommandTooLong` refusals.
- Unix runs every terminal through one fixed `sh -c` wrapper, with hashcat and args as positional
  parameters, never spliced text; the wrapper owns working directory, exit code and hold-open prompt.
- Preview/clipboard follow the configured terminal's shell (`CommandPreview::Dialect`), not the build
  platform. `pwsh` is offered only on Windows; Unix offers the terminal table, Windows only
  `powershell.exe`/`pwsh`.
- Working directory is the canonical directory of the hashcat program; the program path stays
  resolved.

## Adding a hashcat option

Four places, then preview and tests:
- a field in `HashcatOptions` (`src/hashcatoptions.h`), defaulting to the widget's start value;
- a `CommandBuilder::Parameter`, `parameterName()` entry and `build()` case, omitted at its default;
- the widget in `src/mainwindow.ui` and its read in `OptionCollector::collect()`;
- a case in `tests/tst_commandbuilder.cpp`.

Connect to `commandChanged()` once; the attack-mode combo uses `attackIndexChanged()` instead.

## Persistence

- `WidgetStateSerializer` keys on widget object names (the session format); combo values are item
  data, never display text. Missing item data falls back to position; unknown ids stay untouched.
- `StateFile` owns the frozen envelope: versions bump on renames, never additions; section names and
  `session.json` are constants; removed widgets go to `RetiredWidgetNames`; refuse newer versions,
  apply older ones with notes, and never destroy a file this version cannot understand.
- `tests/data/golden_session.json` pins a canonical session and argv; update it plus
  `tst_session::theSerializedKeySetIsFrozen` when serialization changes.
- Restored sessions bypass validators, so `LaunchValidator` rechecks every non-empty field and
  `SettingsManager::getKey` refuses wrong-typed or non-canonical values.
- `restoreSession()` runs once between construction and `show()`; `saveSession()` refuses before it,
  so a blank form cannot overwrite the stored session.
- `SettingsManager` uses `QSettings::IniFormat`; keys/defaults are `AppConstants::SettingsKeys::All`.
  Never set the organization name: it moves `AppDataLocation` and the session file.

## Conventions

- SPDX `GPL-3.0-or-later` header in every source, header and test.
- Production defines `QT_NO_CAST_FROM_ASCII`/`QT_NO_CAST_TO_ASCII`; use `QStringLiteral`,
  `QLatin1StringView` or `QString::fromUtf8` (tests exempt).
- Tokens, keys, defaults and file names go in `src/appconstants.h`; fixed hashcat help stays
  untranslatable. Non-QObject `tr()` uses `Q_DECLARE_TR_FUNCTIONS`.
- Catalogs use `lupdate -no-obsolete -locations none`; don't run `update_translations` in
  development. New languages update `HASHCAT_GUI_TRANSLATIONS` and
  `AppConstants::Languages::Available`; `tst_translations` enforces the match.
- Tests use `TestEnvironment::run` (redirects XDG), header-declared friends
  (`MainWindowTestAccess`), and `QSKIP` shell stubs on Windows.
- Commit subject ≤72 chars, capitalized, imperative, no period; body only for what and why.
- `FAQ.md` is generated from `FAQ.yaml`: `mdfaqgen FAQ.yaml FAQ.md`.

## CI and packaging

- CI: GCC/Clang on Linux and Windows, plus Debug ASan/UBSan (leak detection off). Windows sets
  `HASHCAT_GUI_REQUIRE_SHELL_TESTS=1`, so skipped shell round-trips in `tst_terminalcatalog` fail.
- OBS and the Windows release use `-DBUILD_TESTING=OFF -DHASHCAT_GUI_REQUIRE_TRANSLATIONS=ON`;
  missing LinguistTools is a configure error. Windows: `build-release.yml`; Linux: `obs/`.
