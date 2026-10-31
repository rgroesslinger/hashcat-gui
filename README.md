# hashcat-gui

[![CI Build](https://github.com/rgroesslinger/hashcat-gui/actions/workflows/ci.yml/badge.svg)](https://github.com/rgroesslinger/hashcat-gui/actions/workflows/ci.yml)
[![build result](https://build.opensuse.org/projects/home:rgroesslinger/packages/hashcat-gui/badge.svg?type=default)](https://build.opensuse.org/package/show/home:rgroesslinger/hashcat-gui)

A graphical user interface for [hashcat](https://github.com/hashcat/hashcat/), the password recovery utility.

![hashcat-gui](/screenshot.png)

## Features

Get a native desktop interface around hashcat's attack toolbox: choose the mode, fill in the options that apply, and see the exact command it will run.

- **Every hashcat 8 attack mode** — all ten, from straight and combination to PCFG, table, generic feed, association and the `?w`/`?q` hybrid, each revealing only the options it uses.
- **No black box** — a live preview shows the exact `hashcat` command and copies it to the clipboard, with optional short parameter names.
- **A working example for every attack mode** — Tools → Load Example Session runs a bundled session with its own wordlist, rules or table, so you can verify the setup before using your own data.
- **Sessions that survive a restart** — the last session is restored automatically; sessions and settings can be exported and imported.
- **Real list management** — several wordlists, rule slots, PCFG rulesets, tables and feed arguments with add/move/remove, per-row include checkboxes and folder support.
- **Runs where you already work** — hashcat starts detached in your terminal of choice: GNOME Terminal, Konsole, Xfce, Ptyxis, Alacritty, kitty, foot, WezTerm, PowerShell or pwsh.
- **Fits the desktop** — 15 UI languages, system/light/dark color scheme, and per-option tooltips with hashcat's reference tables.
- **Native packaging** — packages for openSUSE, Fedora, Debian and Ubuntu, a Windows build, and a built-in update check.

## Installation

hashcat 8.0.0 or newer is required.

### Linux

Packages for openSUSE, Fedora, Debian and Ubuntu are hosted on the [openSUSE Build Service (OBS)](https://build.opensuse.org/package/show/home:rgroesslinger/hashcat-gui). You have the option to download the package directly or configure your package manager to automatically receive updates for future releases by adding the OBS repository. Instructions on how to add the repository can be found on the [download page](https://software.opensuse.org/download.html?project=home:rgroesslinger&package=hashcat-gui). You can verify the fingerprint of the OpenPGP key used for signing these packages on the [Signing keys page](https://build.opensuse.org/projects/home:rgroesslinger/signing_keys).

### Windows

Windows packages are available from the [release page](https://github.com/rgroesslinger/hashcat-gui/releases/).

## Build from source

Clone the repository:

```
git clone https://github.com/rgroesslinger/hashcat-gui
```

Alternatively, download the latest [source release](https://github.com/rgroesslinger/hashcat-gui/releases/). If you have a working Qt Creator setup you can open `CMakeLists.txt` and choose `Build ➔ Run`.

### Linux

- Install dependencies

| Distribution | Package installation command |
| - | ----- |
| Debian/Ubuntu | `apt install build-essential cmake qt6-base-dev qt6-tools-dev qt6-l10n-tools` |
| Fedora | `dnf install gcc-c++ cmake qt6-qtbase-devel qt6-qttools-devel qt6-linguist` |
| openSUSE | `zypper install gcc-c++ cmake qt6-base-devel qt6-linguist-devel` |
| Arch | `pacman -S --needed gcc cmake qt6-base qt6-tools` |

The Qt translation tools are optional - without them hashcat-gui still builds, just without the translated `.qm` files.

### Windows

- Install [MSYS2](https://www.msys2.org/) and launch the `MSYS2 UCRT64` terminal

- Install dependencies
```
pacman -S mingw-w64-ucrt-x86_64-{gcc,cmake,qt6-base,qt6-tools}
```

To launch `hashcat-gui.exe` from outside the MSYS2 terminal you need to add `C:\msys64\ucrt64\bin` to your PATH.

### Build

With the dependencies installed, the build is identical on both platforms:

```
cd hashcat-gui/
cmake -B build
cmake --build build -j$(nproc)
```

### Build targets

The custom targets below are invoked with `cmake --build build --target <name>`. Each is only defined when the tool it needs is installed, so a build without them is unaffected.

| Target | Effect | Requires |
| - | - | - |
| `update_translations` | Refresh the `translations/*.ts` catalogs | Qt6 LinguistTools |
| `clang-format` | Reformat all sources in place | clang-format |
| `clang-tidy` | Run the clang-tidy checks over the sources | clang-tidy |
| `clazy` | Run the clazy Qt checks over the sources | clazy-standalone |

Each target is described in more detail where it is used: [Translations](#translations) covers `update_translations`, [Code style](#code-style) covers `clang-format`, `clang-tidy` and `clazy`.

### Tests

Run the tests after building:

```
ctest --test-dir build --output-on-failure
```

The tests need the Qt6 Test module. If it is missing the build says so and simply has no tests to run. They need no display: they run with `QT_QPA_PLATFORM=offscreen`. To leave the tests out of the build entirely, configure with `-DBUILD_TESTING=OFF`.

### Translations

The catalogs live in `translations/` and are compiled into the executable, so there is nothing to install at run time.

The UI follows the environment's language by default; **Settings → Language** offers an explicit choice instead.

The catalogs are refreshed with:

```
cmake --build build --target update_translations
```

A normal build compiles the committed catalogs to `.qm`.

### Code style

`.clang-format` encodes the project's style conventions. To reformat the sources in place:

```
cmake --build build --target clang-format
```

`.clang-tidy` runs over the sources in `src/` and the tests in `tests/` from a target:

```
cmake --build build --target clang-tidy -j$(nproc)
```

Its findings are warnings rather than errors, so the build still succeeds. Editor tooling that understands a compile database is served by the `compile_commands.json` CMake already writes into the build directory.

`clazy` analyzes the same sources for Qt-specific issues:

```
cmake --build build --target clazy -j$(nproc)
```

The `clazy` target checks `level0` and `level1` by default.
