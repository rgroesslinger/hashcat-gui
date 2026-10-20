# AGENTS.md

## Verification (required for every code change)

Run from the repo root, in order. The change is not complete until all pass:

1. `cmake -B build` — only if `build/` is not configured yet.
2. `cmake --build build --target clang-format` — rewrites sources in place, so it must precede steps 3–4.
3. `cmake --build build` — must compile after formatting.
4. `ctest --test-dir build --output-on-failure --no-tests=error` — full suite.
5. `cmake --build build --target clang-tidy` — analyzes `src/` and `tests/`.
6. `cmake --build build --target clazy` — analyzes `src/` and `tests/`.

**Prerequisites:** Qt 6.8+, C++20, `clang-format`, `clang-tidy`, `clazy-standalone`, and two Qt modules:
- Qt6 LinguistTools — without it `update_translations` disappears and CI fails on stale catalogs.
- Qt6 Test — without it the suite silently runs nothing (hence `--no-tests=error`).

A missing target means a missing tool: install it (or tell the user), never skip the check.
After installing, re-run `cmake -B build`.

`clang-tidy` and `clazy` are warning-only (`WarningsAsErrors` is empty), so exit status 0 does
not mean clean. Read the output; fix or explicitly justify every finding. CI does not run
these three targets — this checklist is the only enforcement.

**Conditional checks:**
- `tr()` string added/changed/removed → `cmake --build build --target update_translations`,
  commit the refreshed `translations/*.ts` (new strings arrive untranslated).
- Desktop entry or icon metadata changed → `desktop-file-validate hashcat-gui.desktop`.
- Structural/architectural change (source lists, build system, translations, packaging) →
  check that `build-release.yml`, the CI workflow, and `obs/` are still valid.

## Build and test

- Build defaults to Release. Configure/build: `cmake -B build && cmake --build build`.
- One suite: `ctest --test-dir build -R '^tst_commandbuilder$' --output-on-failure`.
- One test case: `QT_QPA_PLATFORM=offscreen ./build/tst_mainwindow freshWindowMatchesTheDefaultOptions`
  (CTest sets `QT_QPA_PLATFORM=offscreen` and a 60 s timeout; direct runs need the variable).
- Run the app: `./build/hashcat-gui`. `--help` and `--version` exit before creating the window.

## Structure

- `hashcat-gui-core` is an OBJECT library with every app source except `main.cpp`; the
  executable and all tests link it.
- New production `.cpp`/`.h`/`.ui`/`.qrc` files go in `HASHCAT_GUI_SOURCES` (no globbing).
  New tests (one `tests/tst_*.cpp` each, linked against core) go in `HASHCAT_GUI_TESTS`.
- The wiring in one line: `main.cpp` starts `MainWindow`, which owns the widgets in
  `mainwindow.ui`; `collectHashcatOptions()` reads them into `HashcatOptions`;
  `CommandBuilder::build()` turns that into hashcat argv; `HelperUtils` talks to hashcat
  and to the terminal; `WidgetStateSerializer` persists profiles. Keep the argument order
  stable: the preview, the clipboard and the tests all depend on it.
- Attack-mode gating predicates live in `CommandBuilder` and are reused for group-box
  enablement in `MainWindow`. Do not duplicate them.
- Keep UI-independent logic in `CommandBuilder`, `HashcatInfoParser`, `HelperUtils`, and
  `WidgetStateSerializer` so it stays testable.
- Launch validation: `HelperUtils::validateLaunch`. hashcat starts detached in an external
  terminal (`CREATE_NEW_CONSOLE` on Windows).

### Threading and async
- Hash types load asynchronously from `hashcat --example-hashes --machine-readable`; the
  hash combo is empty until the query returns. Profiles use `m_pendingHashTypeId` for this.
- Never block the GUI thread with a hashcat query.
- `QSettings` is only reentrant: read it on the calling thread before handing work to
  `QtConcurrent`.

### WidgetStateSerializer
- Persists `QLineEdit`, `QCheckBox`, `QComboBox`, `QRadioButton`, `QSpinBox`,
  `QDoubleSpinBox`, `QListWidget` children by `objectName`. Renaming object names breaks
  saved profiles and tests.
- A `QComboBox` is stored by the item data the app gave the entry (attack mode id, hash type
  id, workload level) — never by display text (translatable). Only a combo with no item data
  at all falls back to position, which is not an identifier: inserting mid-list shifts every
  later entry. An unknown identifier leaves the combo untouched.
- Transient widgets (e.g. command preview) are excluded via `AppConstants::Files::PreviewWidget`.

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
  them) and no entries left behind whose source string is gone. CI fails on any drift.
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
- `build/` is gitignored. Never commit generated `ui_*.h`, `.qm`, or binaries.

## Commit messages

- Subject: one line, ≤72 chars, capitalized, imperative verb (`Add`, `Fix`, `Move`, `Use`,
  `Store`, `Keep`), no trailing period.
- Body only if the subject isn't enough: what and why, as prose or short bullets, wrapped at
  72. Don't restate the diff.

## CI and packaging

- CI: GCC and Clang on Linux and Windows; `ctest --no-tests=error`; on Linux also checks
  translation catalogs are current and runs `desktop-file-validate hashcat-gui.desktop`.
- Windows binary: `build-release.yml` on GitHub release publish. Linux packages: built
  externally on openSUSE Build Service from `obs/`.
