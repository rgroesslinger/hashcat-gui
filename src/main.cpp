/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include <QApplication>
#include "mainwindow.h"
#include "config.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // QSettings and QStandardPaths fall back to the executable name when no
    // application name has been set. For the installed binary that happens to
    // be "hashcat-gui", which is what the FAQ documents, but any other binary
    // name would silently change the settings and profile locations. Set it
    // explicitly so those paths stay valid.
    //
    // The organization name is deliberately left unset: QSettings passes the
    // application name as its organization, and setting QCoreApplication's
    // organization in addition would change QStandardPaths::AppDataLocation
    // (and with it the location of the default profile) to
    // ~/.local/share/hashcat-gui/hashcat-gui.
    a.setApplicationName(QStringLiteral("hashcat-gui"));
    a.setApplicationVersion(GUI_VERSION);

    MainWindow w;
    w.show();

    return a.exec();
}
