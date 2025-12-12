QT += core gui widgets
CONFIG += c++17 warn_on

TARGET = hashcat-gui
TEMPLATE = app

SOURCES += \
    helperutils.cpp \
    main.cpp\
    mainwindow.cpp \
    aboutdialog.cpp \
    settingsdialog.cpp \
    settingsmanager.cpp

HEADERS += \
    helperutils.h \
    mainwindow.h \
    aboutdialog.h \
    settingsdialog.h \
    settingsmanager.h

FORMS += \
    mainwindow.ui \
    aboutdialog.ui \
    settingsdialog.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    hashcat-gui.rc

RC_FILE += \
    hashcat-gui.rc
