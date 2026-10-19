# AGENTS.md

## Required verification for every code change

Run these from the repo root, in this order. The change is not complete until
all steps pass:

1. `cmake -B build` — only needed if `build/` is not configured yet.
2. `cmake --build build --target clang-format` — rewrites sources in place.
3. `cmake --build build` — must compile after formatting.
4. `ctest --test-dir build --output-on-failure --no-tests=error` — full suite.
5. `cmake --build build --target clang-tidy` — analyze `src/` and `tests/`.
6. `cmake --build build --target clazy` — analyze `src/` and `tests/`.

`clang-format` edits files, so it must run before the final build and tests.
If a target is missing, the corresponding tool is not installed
(`clang-format`, `clang-tidy`, `clazy-standalone`); install it rather than
skipping the check.

`clang-tidy` and `clazy` are configured as warning-only (`WarningsAsErrors` is
empty), so a zero exit status does not mean they found nothing. Read their
output and fix or explicitly justify every finding.

CI runs build, `ctest`, and translation checks, but not these three targets.
This local checklist is the only enforcement.

## Build and test

- Configure and build from the repo root: `cmake -B build` then `cmake --build build`.
  The build defaults to Release and requires Qt 6.8+ and C++20.
- Run the full suite: `ctest --test-dir build --output-on-failure --no-tests=error`.
  `--no-tests=error` is what CI uses; without it a missing Qt6 Test module silently runs nothing.
- Run one suite: `ctest --test-dir build -R '^tst_commandbuilder$' --output-on-failure`.
- Run one test case directly: `QT_QPA_PLATFORM=offscreen ./build/tst_mainwindow freshWindowMatchesTheDefaultOptions`.
  CTest already sets `QT_QPA_PLATFORM=offscreen` and a 60 s timeout; direct runs need the variable.
- Run the app with `./build/hashcat-gui`. `--help` and `--version` exit before creating the window.

## Structure

- `hashcat-gui-core` is an OBJECT library containing every application source except `main.cpp`;
  the executable and all tests link it. New production `.cpp`/`.h`/`.ui`/`.qrc` files must be
  added to `HASHCAT_GUI_SOURCES` in `CMakeLists.txt` — the build does not glob app sources.
- New test executables must be added to `HASHCAT_GUI_TESTS`; each is a single
  `tests/tst_*.cpp` linked against the core library.
- `MainWindow` owns the widgets and collects them into `HashcatOptions`;
  `CommandBuilder::build()` turns that into the hashcat argv. Keep argument order stable:
  tests assert exact argument lists.
- Attack-mode gating predicates live in `CommandBuilder` and are reused for group-box
  enablement in `MainWindow`. Do not duplicate those rules.
- Keep UI-independent logic in `CommandBuilder`, `HashcatInfoParser`, `HelperUtils`, and
  `WidgetStateSerializer` so it stays testable.
- Hash types are loaded asynchronously from `hashcat --example-hashes --machine-readable`.
  The hash combo is empty until the query returns; profiles use `m_pendingHashTypeIndex`
  for this. Never block the GUI thread with a hashcat query. `QSettings` is only reentrant,
  so read it on the calling thread before handing work to `QtConcurrent`.
- `WidgetStateSerializer` persists `QLineEdit`, `QCheckBox`, `QComboBox`, `QRadioButton`,
  `QSpinBox`, `QDoubleSpinBox`, and `QListWidget` children by `objectName`.
  Renaming widget object names breaks saved profiles and tests. Transient widgets such as
  the command preview are excluded via `AppConstants::Files::PreviewWidget`.
- Settings use `QSettings::NativeFormat` under the application name; keys belong in
  `AppConstants::SettingsKeys`. Do not set `QCoreApplication`'s organization name — it
  changes `QStandardPaths::AppDataLocation` and the default-profile path.
- Launch validation is in `HelperUtils::validateLaunch`; hashcat is started detached in an
  external terminal, with `CREATE_NEW_CONSOLE` on Windows.

## Conventions

- Every source, header, and test starts with the SPDX `GPL-3.0-or-later` header.
- Production code is compiled with `QT_NO_CAST_FROM_ASCII` and `QT_NO_CAST_TO_ASCII`.
  Use `QStringLiteral`, `QLatin1StringView`, or `QString::fromUtf8`; tests are exempt.
- Put protocol tokens, settings keys, defaults, and file names in `src/appconstants.h`.
  Fixed hashcat syntax/help tables are deliberately non-translatable.
- Wrap user-visible text in `tr()`. After changing a `tr()` string, run
  `cmake --build build --target update_translations` and commit the refreshed `translations/*.ts`.
  Adding a language requires updating both `HASHCAT_GUI_TRANSLATIONS` and
  `AppConstants::Languages::Available`; English has no catalog, and `tst_translations`
  enforces that the two lists stay in sync.
- Non-`QObject` classes use `Q_DECLARE_TR_FUNCTIONS(...)` when they need `tr()`.
- Style is 4-space indent, 100 columns, Allman functions, wrapped operators at line start,
  and no include sorting. `clang-format` rewrites files in place.
- Tests must use `TestEnvironment::run`, not `QTEST_MAIN`; it redirects XDG
  config/data/cache directories so tests never touch real settings. Shell-script hashcat
  stubs do not run on Windows, so those cases `QSKIP`.
- `FAQ.md` is generated from `FAQ.yaml` — edit `FAQ.yaml` only, never `FAQ.md`; nothing in
  the repo regenerates it, so a direct edit is silently lost.
- `build/` is gitignored. Do not commit generated `ui_*.h`, `.qm`, or binaries.

## CI checks

- CI builds with GCC and Clang on Linux and Windows, runs `ctest --no-tests=error`, and on
  Linux verifies the translation catalogs are current and runs
  `desktop-file-validate hashcat-gui.desktop`.
- Run `desktop-file-validate hashcat-gui.desktop` after changing the desktop entry or icon
  metadata.
- Packages, not just CI: `build-release.yml` builds the Windows binary when a GitHub release
  is published; Linux packages are built externally on the openSUSE Build Service from `obs/`.
  On structural or architectural changes (source lists, build system, translations, packaging),
  check whether both workflows and the `obs/` files are still valid.
