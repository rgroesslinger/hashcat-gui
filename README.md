# hashcat-gui

A graphical user interface for the password recovery utility [hashcat](https://github.com/hashcat/hashcat/).

![hashcat-gui](/screenshot.png)

## Current status

[![CI Build](https://github.com/rgroesslinger/hashcat-gui/actions/workflows/ci.yaml/badge.svg)](https://github.com/rgroesslinger/hashcat-gui/actions/workflows/ci.yaml)
[![build result](https://build.opensuse.org/projects/home:rgroesslinger/packages/hashcat-gui/badge.svg?type=default)](https://build.opensuse.org/package/show/home:rgroesslinger/hashcat-gui)

✅ Compatible with hashcat v7.1.2

## Installation

### Linux

Packages for openSUSE, Fedora, Debian and Ubuntu are hosted on the [openSUSE Build Service (OBS)](https://build.opensuse.org/package/show/home:rgroesslinger/hashcat-gui). You have the option to download the package directly or configure your package manager to automatically receive updates for future releases by adding the OBS repository. Instructions on how to add the repository can be found on the [download page](https://software.opensuse.org/download.html?project=home:rgroesslinger&package=hashcat-gui). You can verify the fingerprint of the OpenPGP key used for signing these packages on the [Signing keys page](https://build.opensuse.org/projects/home:rgroesslinger/signing_keys).

### Windows

Windows packages are available from the [release page](https://github.com/rgroesslinger/hashcat-gui/releases/).

## Build from source

Clone the repository:

```
git clone https://github.com/rgoesslinger/hashcat-gui
```

Alternatively, download the latest [source release](https://github.com/rgoesslinger/hashcat-gui/releases/). If you have a working Qt Creator setup you can open `CMakeLists.txt` and choose `Build ➔ Run`.

### Configuration options

All options are passed at configure time (`cmake -B build -D...`). The defaults suit a normal build, so you only need to set them if you want something else.

| Option | Values | Default | Effect |
| - | - | - | - |
| `BUILD_TESTING` | `ON`, `OFF` | `ON` | Build and register the test binaries. See [Tests](#tests). |
| `ENABLE_CLANG_TIDY` | `ON`, `OFF` | `OFF` | Run clang-tidy during compilation. See [Code style](#code-style). |

Example:

```
cmake -B build -DBUILD_TESTING=OFF
```

### Linux

- Install dependencies

| Distribution | Package installation command |
| - | ----- |
| Debian/Ubuntu | `apt install build-essential cmake qt6-base-dev qt6-tools-dev qt6-l10n-tools` |
| Fedora | `dnf install gcc-c++ cmake qt6-qtbase-devel qt6-qttools-devel` |
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
cmake --build build
```

### Tests

Run the tests after building:

```
ctest --test-dir build --output-on-failure
```

The tests need the Qt6 Test module. If it is missing the build says so and simply has no tests to run. They need no display: they run with `QT_QPA_PLATFORM=offscreen`. To leave the tests out of the build entirely, configure with `-DBUILD_TESTING=OFF`.

### Translations

The catalogs live in `translations/` and are compiled into the executable, so there is nothing to install at run time.

The UI follows the environment's language by default; **Settings → Language** offers an explicit choice instead.

After adding or changing a `tr()` string, refresh them:

```
cmake --build build --target update_translations
```

A normal build already recompiles them to `.qm`. CI runs the refresh and fails if it changes anything committed, which is what keeps the catalogs in step with the sources.

### Code style

`.clang-format` encodes the project's style conventions. To reformat the sources in place:

```
cmake --build build --target format
```

The `format` target is only defined when `clang-format` is installed; without it the build is unaffected.

`.clang-tidy` runs as part of compilation but is off by default. Turn it on when configuring:

```
cmake -B build -DENABLE_CLANG_TIDY=ON
cmake --build build
```

Its findings are warnings rather than errors, so the build still succeeds. Editor tooling that understands a compile database is served by the `compile_commands.json` CMake already writes into the build directory.
