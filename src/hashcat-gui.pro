QT += core gui widgets
CONFIG += c++17 warn_on

TARGET = hashcat-gui
TEMPLATE = app

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    euladialog.cpp \
    aboutdialog.cpp

HEADERS += \
    mainwindow.h \
    euladialog.h \
    aboutdialog.h

FORMS += \
    mainwindow.ui \
    euladialog.ui \
    aboutdialog.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    hashcat-gui.rc

RC_FILE += \
    hashcat-gui.rc
