# Changelog

## Unreleased
- A Linux x86_64 AppImage is now published on the release page
- kgx (GNOME Console) is now available as a launch terminal
- The launch terminal can now print the exact command hashcat receives, so a mangled value (such as a `$` hash expanded a second time by a terminal) is immediately visible instead of going unnoticed; enable it under Settings → General

## 0.9.0 (2026-11-20)

### New Features
- **Support for hashcat 8.0.0**: the attack mode selector offers all ten modes — PCFG, table, generic feed and hybrid join the existing ones, each with its own options shown only where they apply ([#18](https://github.com/rgroesslinger/hashcat-gui/issues/18))
- Association mode now supports hashcat 8's account-aware form: without a wordlist, the words come from the hash file, and `phases=…` selects the phases to run
- The minimum hashcat version is now 8.0.0; the segment size (`-c`) and workload profile (`-w`) options were removed as part of the upgrade
- Example sessions for every attack mode are available under Tools → Load Example Session, each with a different hash type and test password
- Wordlist entries are watched while the window is open: a row whose file does not exist is marked with a warning icon ([#15](https://github.com/rgroesslinger/hashcat-gui/issues/15))
- The About dialog now checks for updates ([#16](https://github.com/rgroesslinger/hashcat-gui/issues/16))
- The settings dialog offers the hashcat found on the system PATH when no path is configured ([#17](https://github.com/rgroesslinger/hashcat-gui/issues/17))
- New options: `--hash-recipe` for hash mode 4000, `--rules-concat` to pass several `-r` files in one run, `--outfile-json` in place of the outfile format, and `--cache-path`
- A folder can be selected as a wordlist (base lists only — the multiplied `?q` list has to be a file) and as a PCFG ruleset when it contains a `Grammar/grammar.txt`; a table must still be a file
- Wordlists and PCFG rulesets offer an “Add folder…” button; empty wordlist folders, ruleset folders without a grammar and folders in the table list are marked, and a folder in the `?q` slot is refused before launch
- The hash type selector is searchable and provides autocompletion
- Ghostty is now available as a launch terminal

### Improvements
- Launch validation covers the new attack modes and settings: PCFG and table ranges and repeated keys, a mask with `omen=1`, hint accounts without hint words or file, the hybrid mode's `?w`/`?q` markers against the chosen wordlists, association settings with a wordlist, and a non-directory cache path
- The command preview and clipboard now follow the configured terminal's shell — PowerShell's call operator for `powershell.exe` and `pwsh`, POSIX otherwise — and quote only tokens the shell would split or expand, leaving flags, numbers and plain paths readable
- `cmd.exe` and `ptyxis` were dropped from the supported terminals: neither can correctly escape some shell characters that may appear in the command. See the FAQ for a more in-depth explanation; ptyxis will be re-enabled once an [upstream fix](https://gitlab.gnome.org/chergert/ptyxis/-/merge_requests/168) has landed
- The settings dialog lists every terminal the platform supports and disables the ones that are not installed, so a system without a terminal shows what is available instead of an empty dropdown
- `<unixtime>` in the outfile template is expanded from the collected options instead of being read inside the command builder, so a preview and a launch are each built from one value rather than two clock reads
- The hash type selector has a permanent “Auto-detect” entry at the top, selected when neither a session nor a reload names a hash type

### Bug Fixes
- Launches on Windows PowerShell 5.1 and pwsh before 7.3 could mangle embedded double quotes and drop empty arguments; no user value reaches a shell as text, and an encoded command beyond the 32,767-character CreateProcess limit (about 12,000 characters of arguments) is refused instead of truncated
- Positional values starting with `-` — a mask `-?d?d`, a wordlist, a session value — are protected by `--`, so they are not parsed as hashcat options. NUL and other control characters are refused before launch
- The attack mode survives a settings change that reloads the hash types; previously the mode selector was refilled and silently jumped back to Straight
- The outfile format is validated before launch (comma-separated numbers from 1 to 6; empty is refused), and an unparsable session list no longer wipes the user's rows
- Settings with the wrong type in a hand-edited INI fall back to their defaults instead of being silently converted

## 0.8.0 (2026-10-25)

### New Features
- **Translations**: Multi-language support with compiled translation catalogs; language can be selected in settings ([#12](https://github.com/rgroesslinger/hashcat-gui/issues/12))
- **Terminal support**: Added Alacritty, kitty, foot, WezTerm, Windows PowerShell, and PowerShell 7 (pwsh) ([#7](https://github.com/rgroesslinger/hashcat-gui/issues/7))
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
- Refactored default values, help tables, and magic numbers in `src/appconstants.h` ([#10](https://github.com/rgroesslinger/hashcat-gui/issues/10))
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
- The application now automatically saves its session state on exit and reloads it on startup ([#5](https://github.com/rgroesslinger/hashcat-gui/issues/5))
- Made querying hashcat in the background async so it doesn't block the UI anymore ([#11](https://github.com/rgroesslinger/hashcat-gui/issues/11))

## 0.7.0 (2026-02-07)

- Add openSUSE, Fedora, Debian and Ubuntu packages to the release pipeline ([#8](https://github.com/rgroesslinger/hashcat-gui/issues/8))
- Migrated the build system from **qmake** to **CMake** to simplify cross‑platform builds and enable CMake‑based tooling
- Added a new **Export/Import Profile** feature for saving and loading configuration profiles, which stores all settings in a file that can be shared or reloaded later
- Introduced a new setting that generates short command‑line parameters when available (e.g. `-a` instead of `--attack-mode`)
- Cleaned up and refactored a significant amount of code, resolving all warnings reported by the [KDE/clazy](https://github.com/KDE/clazy) analyzer

## 0.6.1 (2026-01-30)

- Windows packages are now built automatically with every release ([#8](https://github.com/rgroesslinger/hashcat-gui/issues/8))
- Copy‑to‑Clipboard Button: A new button allows you to copy the generated command to your clipboard
- Refined wordlist and rule support for association mode

## 0.6.0 (2026-01-19)

- Ported to Qt 6
- Added support for hashcat v7.1.2
- Removed all legacy references related to (ocl|cuda)Hashcat-(plus|lite)
- New GitHub Actions CI pipeline: linux-g++, linux-clang, win32-g++ ([#4](https://github.com/rgroesslinger/hashcat-gui/issues/4))
- New **Settings** dialog
  - The path to the hashcat binary is now configurable (previously hard-coded) ([#6](https://github.com/rgroesslinger/hashcat-gui/issues/6))
  - Added more terminals for launching: gnome-terminal, ptyxis, konsole, xfce4-terminal
- Supported hash types are now generated dynamically instead of being hard‑coded ([#9](https://github.com/rgroesslinger/hashcat-gui/issues/9))
