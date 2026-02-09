# Changelog

## [0.7.0] (2026-02-07)
- Add openSUSE, Fedora, Debian and Ubuntu packages to the release pipeline (#8)
- Migrated the build system from **qmake** to **CMake** to simplify cross‑platform builds and enable CMake‑based tooling
- Added a new **Export/Import Profile** feature for saving and loading configuration profiles, which stores all settings in a file that can be shared or reloaded later
- Introduced a new setting that generates short command‑line parameters when available (e.g. `-a` instead of `--attack-mode`)
- Cleaned up and refactored a significant amount of code, resolving all warnings reported by the [KDE/clazy](https://github.com/KDE/clazy) analyzer

## [0.6.1] (2026-01-30)

- Windows packages are now built automatically with every release (#8)
- Copy‑to‑Clipboard Button: A new button allows you to copy the generated command to your clipboard
- Refined wordlist and rule support for association mode

## [0.6.0] (2026-01-19)

- Ported to Qt 6
- Added support for hashcat v7.1.2
- Removed all legacy references related to (ocl|cuda)Hashcat-(plus|lite)
- New GitHub Actions CI pipeline: linux-g++, linux-clang, win32-g++ (#4)
- New **Settings** dialog
  - The path to the hashcat binary is now configurable (previously hard-coded) (#6)
  - Added more terminals for launching: gnome-terminal, ptyxis, konsole, xfce4-terminal
- Supported hash types are now generated dynamically instead of being hard‑coded (#9)
