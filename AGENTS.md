# AGENTS.md

Qt 6.8+ (Core, Gui, Widgets, Concurrent, Network), C++20, GCC/Clang only; hashcat 8.0+ at runtime.
Flow: `main.cpp` -> `MainWindow` (`src/mainwindow.ui`) -> `OptionCollector::collect()` ->
`HashcatOptions` -> `CommandBuilder::build()` -> argv. Argv order is frozen; preview, clipboard
and tests depend on it.

## Verify

1. `cmake -B build`.
2. `cmake --build build --target clang-format` — rewrites files; before build/tests.
3. `cmake --build build -j$(nproc)`.
4. `ctest --test-dir build --output-on-failure --no-tests=error -j$(nproc)`.

- Suite: `ctest --test-dir build -R '^tst_commandbuilder$' --output-on-failure`.
- Case: `QT_QPA_PLATFORM=offscreen ./build/tst_mainwindow freshWindowMatchesTheDefaultOptions`
  (CTest supplies offscreen + 60 s timeout).
- `desktop-file-validate hashcat-gui.desktop` after desktop-entry/icon changes.
- `./build/hashcat-gui`; `--help`/`--version` exit before the window exists.
- Real `MainWindow` blocks offscreen: use `MainWindowFixture`/`hashcatStub`
  (`tests/mainwindowtesthelpers.h`) or `Ui::MainWindow` for layout only; harnesses stay in `/tmp`.
  Main-window suites: `tst_mainwindow`, `tst_launch`, `tst_layout`, `tst_session`,
  `tst_settingsdialog`.

## Structure

- `hashcat-gui-core` is an OBJECT library (all app sources except `main.cpp`); keep OBJECT — static
  drops qrc objects and static initializers. Register sources in `HASHCAT_GUI_SOURCES`, tests in
  `HASHCAT_GUI_TESTS`; no globbing.
- `OptionCollector`/`AttackModeView` take `Ui::MainWindow` (session format keys on widget names);
  all other logic stays UI-free and testable.
- `CommandBuilder` owns attack gating (`modeRequirements()`); `LaunchValidator::validate()` checks
  options then launch; `TerminalCatalog::launchDetached()` starts hashcat.
- Examples: `src/examples.h` (one `Entry` per mode, `DataPrefix` `<examples>/`), session JSONs in
  `src/sessions/`; new data files join `src/resources.qrc` + `Examples::DataFiles`.
  `src/sessions/data/**` is pinned `eol=lf` (.gitattributes): bytes embed verbatim into the binary
  and hashcat reads them — CRLF breaks examples.
- `src/potfile/` self-contained: `PotfileCheck` runs read-only `--show` (JSON) / `--left` through
  injectable `HashcatRunner::Query`, each with a unique `hcgui-pot-` session and `--logfile-disable`
  (shared pidfile).
- Potfile entries cap at `Hashcat::PotfileMaxEntries` (10000), totals stay complete; plaintexts
  masked until revealed, search never matches hidden values. Query paths resolve against hashcat's
  cwd; a hash input is a path only if it exists, else inline.
- `QGroupBox` alignment `AlignLeft|AlignVCenter`; General/Attack tab boxes and PCFG/table grids need
  `AlignTop`. `tst_layout` enforces both.
- `PathMonitor`: wordlists/PCFG rulesets may be folders (wordlist needs a file; ruleset needs
  `Grammar/grammar.txt`); tables file-only; bare names like `default-passwords` resolve via hashcat.
- Unchecked wordlist/ruleset/table/feed rows stay in the session but are omitted from command and
  launch validation.
- Hash types load asynchronously: never block the GUI thread on hashcat; read `QSettings` on the
  calling thread only (reentrant). `m_pendingHashTypeId` applies a session's type later.
- Hash type `-2` = GUI-only Auto-detect sentinel (`AppConstants::Hashcat::AutoDetectHashType`); any
  negative `hashType` omits `-m`, the only autodetect trigger. Unknown session ids fall back to
  Auto-detect with a warning.

## Launch

- `Executable::resolve()` is the only program-to-`QProcess` path; empty, PATH miss, missing or
  non-executable -> empty. Settings text is display-only.
- `build()` inserts `--` before the positional tail only when a token starts with `-`; normal argv
  stays byte-identical. `LaunchValidator` rejects NUL and C0/C1 except tab; separates
  `HashcatNotExecutable` from a missing file.
- Windows: `TerminalCatalog::powerShellScript()` via `-EncodedCommand` (also Tools -> Copy as
  PowerShell Script); user data never in `-Command`; overlong -> `CommandTooLong`.
- Unix: one fixed `sh -c` wrapper, hashcat/args as positional parameters, never spliced text; the
  wrapper owns cwd, exit code and hold-open prompt.
- Launch echo (`showLaunchCommand`) reads the real launch fields (wrapper params / CreateProcess),
  never a separately formatted string.
- New/changed terminals require `tests/terminal-probe.sh` (live argv round-trip, fresh and
  forwarded; needs a display). ctest only uses a stub — misses string-mode/forwarding bugs.
- Preview/clipboard use the configured terminal's `CommandPreview::Dialect`, not the build
  platform; `pwsh` is Windows-only.
- Working directory = hashcat program's canonical directory; program path stays resolved.

## Adding a hashcat option

1. Field in `HashcatOptions` (`src/hashcatoptions.h`), default = widget start value.
2. `CommandBuilder::Parameter` + `parameterName()` entry + `build()` case; omitted at default.
3. Widget in `src/mainwindow.ui`, read in `OptionCollector::collect()`.
4. Case in `tests/tst_commandbuilder.cpp`.

Connect once to `commandChanged()`; the attack-mode combo uses `attackIndexChanged()` instead.

## Persistence

- `WidgetStateSerializer` keys on widget object names (frozen session format); combos store item
  data, never display text; missing data falls back to position; unknown ids stay untouched.
- `StateFile` frozen envelope: versions bump only on renames; section names and `session.json` are
  constants; removed widgets go to `RetiredWidgetNames`; refuse newer versions, apply older with
  notes, never destroy files this version cannot understand.
- Serialization changes: update `tests/data/golden_session.json` and
  `tst_session::theSerializedKeySetIsFrozen`.
- Restored sessions bypass validators: `LaunchValidator` rechecks every non-empty field;
  `SettingsManager::getKey` rejects wrong-typed or non-canonical values.
- `restoreSession()` runs once between construction and `show()`; `saveSession()` refuses before
  it, so a blank form cannot overwrite the stored session.
- `SettingsManager`: `QSettings::IniFormat`; keys/defaults in `AppConstants::SettingsKeys::All`.
  Never set the application organization name — it moves `AppDataLocation` and the session file.

## Conventions

- SPDX `GPL-3.0-or-later` header in every source, header and test.
- Production defines `QT_NO_CAST_FROM_ASCII`/`QT_NO_CAST_TO_ASCII`: use `QStringLiteral`,
  `QLatin1StringView` or `QString::fromUtf8` (tests exempt).
- Tokens, keys, defaults, file names go in `src/appconstants.h`. Fixed hashcat help stays
  untranslatable; non-QObject `tr()` needs `Q_DECLARE_TR_FUNCTIONS`.
- Catalogs: `lupdate -no-obsolete -locations none`; never run `update_translations` in development.
  New languages update `HASHCAT_GUI_TRANSLATIONS` + `AppConstants::Languages::Available`;
  `tst_translations` enforces the match.
- Tests: `TestEnvironment::run` (QStandardPaths test mode), header-declared friends
  (`MainWindowTestAccess`), `QSKIP` shell stubs on Windows. Read-only hashcat dialogs inject
  `HashcatRunner::Query`; stubs in `tests/hashcatquerytesthelpers.h`.
- Commits: subject <=72 chars, capitalized, imperative, no period; body only for what and why.
- `FAQ.md` is generated: `mdfaqgen FAQ.yaml FAQ.md`.

## CI and packaging

- CI: GCC/Clang on Linux and Windows (MSYS2 UCRT64); Debug ASan/UBSan, leak detection off.
- Windows CI sets `HASHCAT_GUI_REQUIRE_SHELL_TESTS=1`: skipped shell round-trips in
  `tst_terminalcatalog` fail.
- Packaging (OBS `obs/`; Windows/AppImage `build-release.yml`) configures with
  `-DBUILD_TESTING=OFF -DHASHCAT_GUI_REQUIRE_TRANSLATIONS=ON`; missing LinguistTools is a
  configure error.
