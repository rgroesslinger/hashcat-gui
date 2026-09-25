/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include <QApplication>
#include <QLocale>
#include <QTranslator>
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

    // Load the translation before the first widget exists. qt_add_translations()
    // embeds the catalogs under :/i18n; a build without LinguistTools has
    // nothing there and stays on the source strings. The locale's UI languages
    // are tried from most to least specific, so hashcat-gui_de.qm answers for
    // de, de_DE and de_AT alike.
    QTranslator translator;
    if (translator.load(QLocale(), QStringLiteral("hashcat-gui"), QStringLiteral("_"),
                        QStringLiteral(":/i18n"), QStringLiteral(".qm"))) {
        a.installTranslator(&translator);
    }

    MainWindow w;
    w.show();

    return a.exec();
}
