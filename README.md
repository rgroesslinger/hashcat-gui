![hashcat-gui](/screenshot.png)

# hashcat-gui

A graphical user interface for the password recovery utility [hashcat](https://github.com/hashcat/hashcat/).

## Current status
[![CI Build](https://github.com/rgroesslinger/hashcat-gui/actions/workflows/ci.yaml/badge.svg)](https://github.com/rgroesslinger/hashcat-gui/actions/workflows/ci.yaml)

✅ Compatibility with Qt6

❌ Compatibility with hashcat 7.x (WIP)

## Compile from source
```
git clone https://github.com/rgroesslinger/hashcat-gui
```
or download latest [source release](https://github.com/rgroesslinger/hashcat-gui/releases/). If you have a working development environment with QtCreator you can open `hashcat-gui.pro` in QtCreator and `Build ➔ Run`

### Linux
#### Debian/Ubuntu
```
apt install build-essential qt6-base-dev qmake6
cd hashcat-gui/src/
qmake6 -config release
make
```
#### Fedora
```
dnf install qt6-qtbase-devel
cd hashcat-gui/src/
qmake -config release
make
```

### Windows
#### Install Qt
- Download [Qt for Open Source](https://www.qt.io/development/download-qt-installer-oss/) or install Qt via [aqt.exe](https://github.com/miurahr/aqtinstall/releases/)
```
aqt.exe install-qt windows desktop 6.10.1 win64_msvc2022_64
```

#### Install C++ compiler and Windows SDK
- You may have to adjust version numbers in directories to match your local installation. Also make sure to run all commands in the same cmd session to keep environment variables!
- Download [Build Tools for Visual Studio](https://visualstudio.microsoft.com/downloads/) (All Downloads ➔ Tools for Visual Studio ➔ Build Tools for Visual Studio)
- Select "Desktop development with C++" or needed components only:
	- MSVC Build Tools for x64/x86 (Latest)
	- Windows 11 SDK
- Open cmd.exe
- Run *vcvars64.bat* in terminal to prepare development environment
```
"C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
```
- Set PATH, INCLUDE and LIBPATH environment
```
set PATH=%PATH%;C:\Qt\6.10.1\msvc2022_64\bin
set PATH=%PATH%;C:\Program Files (x86)\Windows Kits\10\bin\10.0.26100.0\x64
set INCLUDE=%INCLUDE%;C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\ucrt
set LIBPATH=%LIBPATH%;C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64
```
- Compile
```
cd hashcat-gui\src
qmake -config release
nmake
cd release
hashcat-gui.exe
```
