# hashcat-gui
A graphical user interface for the password recovery utility [hashcat](https://github.com/hashcat/hashcat/).

![hashcat-gui](/screenshot.png)

## Current status
[![CI Build](https://github.com/rgroesslinger/hashcat-gui/actions/workflows/ci.yaml/badge.svg)](https://github.com/rgroesslinger/hashcat-gui/actions/workflows/ci.yaml)

✅ Compatible with hashcat v7.1.2

## Installation

### Linux
Packages for various Linux distributions are available from [openSUSE Build Service (OBS)](https://build.opensuse.org/project/show/home:rgroesslinger).

### Windows
Windows packages are available from the [release page](https://github.com/rgroesslinger/hashcat-gui/releases/).

## Build from source
```
git clone https://github.com/rgroesslinger/hashcat-gui
```
or download latest [source release](https://github.com/rgroesslinger/hashcat-gui/releases/). If you have a working development environment with Qt Creator you can open `CMakeLists.txt` and `Build ➔ Run`.

### Linux
- Install dependencies

| Distribution | Package installation command |
| - | ----- |
| Debian/Ubuntu | `apt install build-essential cmake qt6-base-dev` |
| Fedora | `dnf install gcc-c++ cmake qt6-qtbase-devel` |
| openSUSE | `zypper install gcc-c++ cmake qt6-base-devel` |
| Arch | `pacman -S --needed gcc cmake qt6-base` |

- Build
```
cd hashcat-gui/
cmake -B build
cmake --build build
```

### Windows
- Install [MSYS2](https://www.msys2.org/) and launch the `MSYS2 UCRT64` terminal

- Install dependencies
```
pacman -S mingw-w64-ucrt-x86_64-{gcc,cmake,qt6-base}
```

- Build
```
cd hashcat-gui/
cmake -B build
cmake --build build
```

To launch `hashcat-gui.exe` from outside the MSYS2 terminal you need to add `C:\msys64\ucrt64\bin` to your PATH.
