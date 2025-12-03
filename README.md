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
cd src/
qmake -config release
make
```
