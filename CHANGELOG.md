# Changelog

## 0.8.0 (Unreleased)

### New Features
- **Translations**: Multi-language support with compiled translation catalogs; language can be selected in settings (system default falls back to English) (#12)
- **Terminal support**: Added Alacritty, kitty, foot, WezTerm, and Windows Terminal (#7)
- **CLI help**: Added `--version` and `--help` command-line parser support
- **Test suite**: Integrated `tst_*` binaries into `ctest`, covering the command builder, hash type parser, hashcat launch, profile serializer, settings dialog, about dialog, main window, and translation catalogs
- **Code quality tools**: Added `clang-format`, `clang-tidy`, and `clazy` tooling

### Improvements
- **One file format**: The session the application keeps between two runs and the settings the user exports share one format now; the files of the older versions are not read any more
- **Session**: The last session is restored at startup only when asked for, and the saved profile can be cleared from the settings
- **Settings**: Show where the settings and the session are stored, and add a shortcut to open the folder of the settings
- **Settings**: The settings can be exported to a file and imported again
- **Color scheme**: The application can follow the desktop, or use a light or a dark appearance of its own
- Decoupled business logic from UI for better maintainability and testability
- Refactored default values, help tables, and magic numbers in `src/appconstants.h` (#10)
- Hardened UTF-8 and string handling by enforcing `QT_NO_CAST_FROM_ASCII` and `QT_NO_CAST_TO_ASCII`
- `HashcatOptions` and `CommandBuilder`: the command line is built from a UI-independent description; gating rules centralized so visible options and generated command stay in sync
- **Qt 6.8 & C++20 baseline**: Bumped minimum Qt version to 6.8 and moved build baseline from C++17 to C++20
- **New artwork**: Added new application artwork and improved the XDG `.desktop` file entry
- **Persistent command bar**: Moved Start, Copy, and the command preview below the tabs so they stay visible on every tab
- **More hashcat options**: Added `--increment` with bounds, `--skip`, `--limit`, `--runtime`, `--force`, `--session`, `--status`/`--status-timer`, `--opencl-device-types`, `--hwmon-temp-abort`, `--rule-left`/`--rule-right` single rules for the left/right wordlist, `--encoding-from`/`--encoding-to`, `--hex-wordlist`, and `--potfile-disable`
- **Clearer tabs**: The Attack tab holds the hash input and the mode-specific word lists, rules, mask, and charset options; the General tab holds the session and limits, performance, resources, and outfile options
- **Mode hint**: A hint below the attack mode selector now states what the selected mode needs: the number of word lists, a mask, or both
- Combo boxes in a profile are stored by the identifier of their selected entry instead of its position, so a list that grows - a new hash type, a new attack mode - no longer changes what a saved profile selects
- **A profile says which version wrote it**: A file with an older format version is applied, and the values it does not know are named; a file with a newer format version is refused, because a name that survived may mean something else there

### Bug Fixes
- GUI now refuses to start hashcat when configuration is incomplete instead of failing silently
- Terminal startup failures (wrong path, permissions) now report errors instead of failing silently
- Profile save/load errors now reach the main window instead of having the serializer open its own dialog; a failed profile load at start-up is now reported as well
- Hash type saved in a profile is now correctly applied after the background query completes (was dropped at start-up)
- A profile that names a hash type the installed hashcat does not know now leaves the hash type unselected and says so, instead of silently selecting another one
- File → Export Profile only announces success when the profile was actually written
- Hash type selector re-enabled when background query fails or returns malformed JSON
- `--hash-type` omitted when no hash type is selected (empty selector no longer sends `-m 0`)
- Attack mode and hash type identifiers use combo box item data, preventing silent fallback to mode/type `0`
- Sorting the word list with nothing selected no longer passes a null item to `insertItem()`
- The suggested outfile no longer overwrites user-selected or profile-chosen file names
- Copying to the clipboard no longer crashes on platforms without clipboard support
- The main window now sizes itself to its content on first show, using the tallest tab for the height and the widest tab for the width; this avoids unnecessary scrollbars, clipped content, and unused space across screen sizes, fonts, DPI, and translations

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
