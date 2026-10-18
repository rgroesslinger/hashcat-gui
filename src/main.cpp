/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include <QApplication>
#include <QCommandLineParser>
#include <QIcon>
#include <QLocale>
#include <QTranslator>
#include "mainwindow.h"
#include "settingsmanager.h"
#include "appconstants.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // The organization name is deliberately left unset: QSettings passes the
    // application name as its organization, and setting QCoreApplication's
    // organization in addition would change QStandardPaths::AppDataLocation
    // (and with it the location of the default profile)
    a.setApplicationName(QStringLiteral("hashcat-gui"));
    a.setApplicationVersion(QStringLiteral(HASHCAT_GUI_VERSION));

    // Standard --help / --version handling. parser.process() exits the
    // process on either option, so the main window is never created and no
    // dialog opens - handy for smoke-testing the binary.
    QCommandLineParser parser;
    parser.setApplicationDescription(a.applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(a);

    // Let the desktop match the window to the XDG desktop entry
    // to resolve the installed hicolor icon by name
    a.setDesktopFileName(QStringLiteral("hashcat-gui"));

    // Window and taskbar icon.
    //
    // Windows has no icon theme, so the full-bleed .ico is used directly: it
    // carries every size Windows asks for and matches the icon Explorer and
    // shortcuts show for the executable.
    //
    // Elsewhere the icon theme is preferred, because it resolves the installed
    // hicolor icon at the size the desktop asks for and covers the sizes no
    // bitmap exists for with the scalable SVG. QIcon::fromTheme() gives no hint
    // when that entry cannot actually be rendered - the scalable SVG needs the
    // qsvg image format plugin at runtime, and without it the icon is not null
    // and advertises a size, but yields no pixmap - so the result is probed and
    // the PNGs embedded below are used whenever it comes up empty.
#if defined(Q_OS_WIN)
    a.setWindowIcon(QIcon(QStringLiteral(":/icons/hashcat-gui.ico")));
#else
    QIcon windowIcon = QIcon::fromTheme(QStringLiteral("hashcat-gui"));
    if (windowIcon.pixmap(32, 32).isNull()) {
        static constexpr int iconSizes[] = {16, 22, 24, 32, 48, 64, 128, 256};
        for (int size : iconSizes) {
            windowIcon.addFile(
                QStringLiteral(":/icons/hicolor/%1x%1/apps/hashcat-gui.png").arg(size));
        }
    }
    a.setWindowIcon(windowIcon);
#endif

    // Load the translation before the first widget exists. qt_add_translations()
    // embeds the catalogs under :/i18n; a build without LinguistTools has
    // nothing there and stays on the source strings. The locale's UI languages
    // are tried from most to least specific, so hashcat-gui_de.qm answers for
    // de, de_DE and de_AT alike.
    const QString language = SettingsManager::instance().getKey<QString>(
        AppConstants::SettingsKeys::Language, QString::fromUtf8(AppConstants::Languages::Default));

    QTranslator translator;
    if (translator.load(language.isEmpty() ? QLocale() : QLocale(language),
                        QStringLiteral("hashcat-gui"), QStringLiteral("_"),
                        QStringLiteral(":/i18n"), QStringLiteral(".qm"))) {
        a.installTranslator(&translator);
    }

    MainWindow w;
    w.show();

    return a.exec();
}
