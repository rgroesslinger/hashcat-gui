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
or download latest [source release](https://github.com/rgroesslinger/hashcat-gui/releases/). If you have a working development environment with QtCreator you can open `hashcat-gui.pro` in QtCreator and `Build ➔ Run`.

### Linux
- Install dependencies

| Distribution | Package installation command |
| - | ----- |
| Debian/Ubuntu | `apt install build-essential qt6-base-dev qmake6` |
| Fedora | `dnf install @c-development qt6-qtbase-devel` |
| openSUSE | `zypper install -t pattern devel_basis && zypper install qt6-base-devel` |

- Build
```
cd hashcat-gui/src/
qmake6 -config release
make
```

### Windows
- open cmd.exe
- install Qt via [aqt](https://github.com/miurahr/aqtinstall/)
```
winget install -e --id miurahr.aqtinstall
aqt.exe install-qt windows desktop 6.10.1 win64_msvc2022_64 -O C:\Qt
```
- Install MSVC Build Tools and Windows SDK
```
winget install -e --id Microsoft.VisualStudio.Community --custom "--add Microsoft.VisualStudio.Component.Windows11SDK.26100 --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64"
```
- Add Qt to PATH and run *vcvars64.bat* to prepare development environment
```
set PATH=%PATH%;C:\Qt\6.10.1\msvc2022_64\bin
"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
```
- Build
```
cd hashcat-gui\src
qmake -config release
nmake
cd release
hashcat-gui.exe
```
