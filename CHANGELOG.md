# Changelog

## Unreleased

### Added
- **German translation.** The catalogs in `translations/` are now compiled into the executable with `qt_add_translations()` and loaded when the program starts; Serbian is set up as an untranslated skeleton. Every user-facing string goes through `tr()`, including the attack mode names, the file dialog titles and the settings dialog file filter. Values hashcat is handed verbatim (`0`, `1,2`, the workload profiles) and product names are marked `notr`, so a translation can never change a command line.
- **More terminal emulators**: alacritty, kitty, foot, wezterm and Windows Terminal, each with the arguments its own parser expects.
- `src/appconstants.h` collects the settings keys, file names, UI defaults, outfile placeholders and hashcat tokens that used to be scattered across the sources as string literals.
- `HashcatOptions` and `CommandBuilder`: the command line is built from a UI-independent description, and the gating rules (`attackUsesMask()`, `attackUsesWordlists()`, `attackUsesRules()`) live in one place that the main window's group boxes use too, so the visible options and the generated command cannot disagree.
- `HashcatInfoParser` for the `--example-hashes` reply.
- Test suite: three `tst_*` binaries wired into `ctest`, covering the argument rules, the launch checks, the hashcat helper process and the widget state of a freshly opened window.
- FAQ entries for the launch checks and for the language.
- CI builds the translations, runs the tests and fails when the catalogs are out of step with the sources.

### Changed
- hashcat-gui **refuses to start hashcat** when the configuration is incomplete instead of failing silently: `HelperUtils::validateLaunch()` reports whether the hash file, the hashcat path, the terminal selection or the terminal program itself is missing, and the dialog names it. A configured terminal that is no longer installed is an error rather than a quiet fallback to another one.
- The application name is set explicitly, so the settings and profile locations from the FAQ hold no matter what the binary is called.
- The hashcat query reads the configured path on the calling thread instead of from the worker pool, and reaps a timed-out hashcat process instead of leaving it running.
- The command preview no longer begins with a blank when no hashcat path is configured.
- Profile errors are reported through an error message returned to the caller rather than the serializer opening its own dialog, so a failed profile load at start-up is reported as well.
- The segment size and the workload profile are read as numbers rather than as rendered text.

### Fixed
- The hash type selector is re-enabled when the background query fails or returns malformed JSON; it used to stay on "Updating..." for the rest of the session.
- `--hash-type` is left out while no hash type is selected; an empty selector used to send `-m 0`.
- Attack mode and hash type ids travel as combo box item data, so a missing entry can no longer silently become mode or type `0`.
- A reply that parses but contains nothing usable reports an error instead of emptying the list, and non-numeric keys or entries without a name are skipped.
- Sorting the word list with nothing selected no longer hands a null item to `insertItem()`.
- The suggested outfile no longer overwrites a file name the user or a loaded profile chose.
- `QProcess::startDetached()`, `QApplication::clipboard()`, `QFile::write()` and the profile save/export results are checked: a terminal that will not start, a platform without clipboard support, a full disk and a failed export all say so now.
- `getParameter()` returns an empty string for an unknown parameter instead of dereferencing `end()`, and `HashcatResult`'s status members default to "no process ran" instead of an indeterminate value, so an unfinished query can no longer look like a successful one.
- `config.h` is generated into the build directory instead of the source tree.

## 0.7.1 (2026-02-15)
- Added support for more command options
  - `-O`, `--optimized-kernel-enable`: Enables optimized kernels
  - `--speed-only`: Return expected speed of the attack
- The application now automatically saves its session state on exit and reloads it on startup (#5)
- Made querying hashcat in the background async so it doesn't block the UI anymore (#11)

## 0.7.0 (2026-02-07)
- Add openSUSE, Fedora, Debian and Ubuntu packages to the release pipeline (#8)
- Migrated the build system from **qmake** to **CMake** to simplify cross‑platform builds and enable CMake‑based tooling
- Added a new **Export/Import Profile** feature for saving and loading configuration profiles, which stores all settings in a file that can be shared or reloaded later
- Introduced a new setting that generates short command‑line parameters when available (e.g. `-a` instead of `--attack-mode`)
- Cleaned up and refactored a significant amount of code, resolving all warnings reported by the [KDE/clazy](https://github.com/KDE/clazy) analyzer

## 0.6.1 (2026-01-30)

- Windows packages are now built automatically with every release (#8)
- Copy‑to‑Clipboard Button: A new button allows you to copy the generated command to your clipboard
- Refined wordlist and rule support for association mode

## 0.6.0 (2026-01-19)

- Ported to Qt 6
- Added support for hashcat v7.1.2
- Removed all legacy references related to (ocl|cuda)Hashcat-(plus|lite)
- New GitHub Actions CI pipeline: linux-g++, linux-clang, win32-g++ (#4)
- New **Settings** dialog
  - The path to the hashcat binary is now configurable (previously hard-coded) (#6)
  - Added more terminals for launching: gnome-terminal, ptyxis, konsole, xfce4-terminal
- Supported hash types are now generated dynamically instead of being hard‑coded (#9)
