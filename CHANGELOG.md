# Changelog

## 0.8.0 (Unreleased)

### New Features
- **Translations**: Multi-language support with compiled translation catalogs; language can be selected in settings (system default falls back to English) (#12)
- **Terminal support**: Added alacritty, kitty, foot, wezterm, and Windows Terminal (#7)
- **CLI help**: Added `--version` and `--help` command-line parser support
- **Test suite**: Integrated `tst_*` binaries into `ctest`, covering command line rules, hash type parser, hashcat launch, profile serializer, settings dialog, main window, and translation catalogs
- **Code quality tools**: Added `clang-format`, `clang-tidy`, and `clazy` tooling

### Improvements
- Decoupled business logic from UI for better maintainability and testability
- Refactored default values and magic numbers in `src/appconstants.h` (#10)
- Hardened UTF-8 and string handling by enforcing `QT_NO_CAST_FROM_ASCII` and `QT_NO_CAST_TO_ASCII`
- `HashcatOptions` and `CommandBuilder`: command line built from UI-independent description; gating rules centralized so visible options and generated command stay in sync
- CI now builds translations, runs tests, and fails when catalogs are out of step with sources
- **Qt 6.8 & C++20 baseline**: Bumped minimum Qt version to 6.8 and moved build baseline from C++17 to C++20
- **New artwork**: Added new application artwork and improved the XDG `.desktop` file entry
- **Persistent command bar**: Moved Start, Copy, and the command preview below the tabs so they stay visible on every tab
- **More hashcat options**: Added `--increment` with bounds, `--skip`, `--limit`, `--runtime`, `--force`, `--session`, `--status`/`--status-timer`, `-D`, `--hwmon-temp-abort`, `-j`/`-k` for combination mode, `--encoding-from`/`--encoding-to`, `--hex-wordlist`, and `--potfile-disable`
- **Clearer tabs**: The Attack tab now holds the mode-specific hash input, wordlist, rules, mask and charset options; the General tab holds session, output, resource, performance and hardware options
- Combo boxes in a profile are stored by the id of their selected entry instead of its position, so a list that grows - a new hash type, a new attack mode - no longer changes what a saved profile selects

### Bug Fixes
- GUI now refuses to start hashcat when configuration is incomplete instead of failing silently
- Terminal startup failures (wrong path, permissions) now report errors instead of failing silently
- Profile errors reported via error message instead of opening a dialog, ensuring failed profile loads at start-up are reported
- Hash type saved in a profile is now correctly applied after background query completes (was dropped at start-up)
- A profile that names a hash type the installed hashcat does not know now leaves the hash type unselected and says so, instead of silently selecting another one
- File → Export only announces success when the profile was actually written
- Hash type selector re-enabled when background query fails or returns malformed JSON
- `--hash-type` omitted when no hash type is selected (empty selector no longer sends `-m 0`)
- Attack mode and hash type IDs use combo box item data, preventing silent fallback to mode/type `0`
- Sorting word list with nothing selected no longer passes a null item to `insertItem()`
- Suggested outfile no longer overwrites user-selected or profile-chosen file names
- Copy to clipboard no longer crashes on platforms without clipboard support
- Windows now size to content instead of hard-coded geometry, using the tallest tab for the height and the widest tab for the width; this fixes scrollbars, clipped content, and unused space across different screen sizes, fonts, DPI, and translations

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
