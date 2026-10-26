# Changelog

## Unreleased

### New Features
- Wordlist entries are watched while the window is open: a row whose file does not exist is marked with a warning icon (#15)
- The About dialog now checks for updates (#16)
- The settings dialog offers the hashcat found on the system PATH when no path is configured (#17)

## 0.8.0 (2026-10-25)

### New Features
- **Translations**: Multi-language support with compiled translation catalogs; language can be selected in settings (#12)
- **Terminal support**: Added Alacritty, kitty, foot, WezTerm, Windows PowerShell, and PowerShell 7 (pwsh) (#7)
- **Test suite**: A broad test suite now covers the core logic, improving reliability and maintainability
- **Code quality tools**: Added `clang-format`, `clang-tidy`, and `clazy` tooling
- **Export and import**: Settings can be exported to a file and imported again
- **Color scheme**: The application can follow the desktop, or use a light or a dark appearance of its own where the platform supports it
- **Example session**: A bundled brute-force session to quickly check that the hashcat setup works (Tools → Load Example Session)

### Improvements
- Added new application artwork and improved the XDG `.desktop` file entry
- Bumped the minimum Qt version to 6.8 and moved the build baseline from C++17 to C++20
- Settings and sessions are now stored in a unified file format across all platforms. Note that this results in a one-time backwards-incompatible change of the storage format. Settings and session configuration are now versioned; should another incompatible change be required in the future, we can bump this version and keep support for previous formats
- The `Attack` tab holds the hash input and the mode-specific options; the `General` tab holds the options that are always available
- A hint below the attack mode selector now states what the selected mode needs: the number of word lists, a mask, or both
- Refactored default values, help tables, and magic numbers in `src/appconstants.h` (#10)
- Hardened UTF-8 and string handling by enforcing `QT_NO_CAST_FROM_ASCII` and `QT_NO_CAST_TO_ASCII`
- Decoupled the command line building from the UI in `HashcatOptions` and `CommandBuilder`, with the gating rules centralized so the visible options and generated command stay in sync
- Added `--increment` with bounds, `--skip`, `--limit`, `--runtime`, `--force`, `--session`, `--status`/`--status-timer`, `--opencl-device-types`, `--hwmon-temp-abort`, `--rule-left`/`--rule-right` single rules for the left/right wordlist, `--encoding-from`/`--encoding-to`, `--hex-wordlist`, and `--potfile-disable`

### Bug Fixes
- GUI now refuses to start hashcat when configuration is incomplete instead of failing silently
- Terminal startup failures (wrong path, permissions) now report errors instead of failing silently
- Session save/load errors now reach the main window instead of having the serializer open its own dialog; a failed session load at start-up is now reported as well
- Hash type saved in a session is now correctly applied after the background query completes (was dropped at start-up)
- A session that names a hash type the installed hashcat does not know now leaves the hash type unselected and says so, instead of silently selecting another one
- File → Export Session only announces success when the session was actually written
- `--hash-type` omitted when no hash type is selected (empty selector no longer sends `-m 0`)
- Sorting the word list with nothing selected no longer passes a null item to `insertItem()`
- The suggested outfile no longer overwrites user-selected or session-chosen file names
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
