# hashcat-gui
A graphical user interface for the password recovery utility [hashcat](https://github.com/hashcat/hashcat/).

![hashcat-gui](/screenshot.png)

## Current status
[![CI Build](https://github.com/rgroesslinger/hashcat-gui/actions/workflows/ci.yaml/badge.svg)](https://github.com/rgroesslinger/hashcat-gui/actions/workflows/ci.yaml)

✅ Compatibility with Qt6

❌ Compatibility with hashcat 7.x (WIP)

## Build from source
```
git clone https://github.com/rgroesslinger/hashcat-gui
```
or download latest [source release](https://github.com/rgroesslinger/hashcat-gui/releases/). If you have a working development environment with Qt Creator you can open `hashcat-gui.pro` and `Build ➔ Run`.

### Linux
- Install dependencies

| Distribution | Package installation command |
| - | ----- |
| Debian/Ubuntu | `apt install build-essential qt6-base-dev` |
| Fedora | `dnf install @c-development qt6-qtbase-devel` |
| openSUSE | `zypper install -t pattern devel_basis && zypper install qt6-base-devel` |
| Arch | `pacman -S --needed base-devel qt6-base` |

- Build
```
cd hashcat-gui/src/
qmake6 -config release
make
```

### Windows
- Install [MSYS2](https://www.msys2.org/) and launch the `MSYS2 UCRT64` terminal

- Install dependencies
```
pacman -S mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-qt6-base
```

- Build
```
cd hashcat-gui/src/
qmake6 -config release
mingw32-make
```

To launch `hashcat-gui.exe` from outside the MSYS2 terminal you need to add `C:\msys64\ucrt64\bin` to your PATH.
