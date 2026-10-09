/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef TESTENVIRONMENT_H
#define TESTENVIRONMENT_H

#include <QApplication>
#include <QCoreApplication>
#include <QFile>
#include <QTemporaryDir>
#include <QtTest>

// Scratch base directories for a test process.
//
// QSettings and QStandardPaths resolve XDG_CONFIG_HOME / XDG_DATA_HOME when
// they are first used. Redirecting them here means no test can read, and
// above all no test can overwrite, the settings, default profile or window
// geometry of whoever runs ctest.
//
// The three accessors hold function local statics that are first touched from
// main(), before SettingsManager's singleton exists. Both are therefore
// destroyed in reverse order of that: the singleton first, the directories
// last, so nothing ever writes into a directory that is already gone.
namespace TestEnvironment
{

inline QTemporaryDir &configHome()
{
    static QTemporaryDir dir;
    return dir;
}

inline QTemporaryDir &dataHome()
{
    static QTemporaryDir dir;
    return dir;
}

inline QTemporaryDir &cacheHome()
{
    static QTemporaryDir dir;
    return dir;
}

// Has to run before the first QSettings or QStandardPaths call.
inline void redirectBaseDirectories()
{
    // A scratch directory that could not be created would send QSettings and
    // QStandardPaths back at the real ones - the exact leak this redirect
    // exists to stop. Stop the run instead of testing against somebody's own
    // configuration.
    if (!configHome().isValid() || !dataHome().isValid() || !cacheHome().isValid()) {
        qFatal("TestEnvironment: could not create the scratch base directories");
    }

    qputenv("XDG_CONFIG_HOME", configHome().path().toUtf8());
    qputenv("XDG_DATA_HOME", dataHome().path().toUtf8());
    qputenv("XDG_CACHE_HOME", cacheHome().path().toUtf8());
}

// A hashcat stand-in the tests can actually execute. Returns an empty path on
// a platform that cannot run it - no shell to interpret the #! line, or
// Windows, which starts no such file at all. Callers QSKIP on Windows and
// treat an empty path as a hard failure everywhere else.
inline QString writeShellStub([[maybe_unused]] const QString &name,
                              [[maybe_unused]] const QByteArray &content)
{
#ifdef Q_OS_WIN
    // Windows only starts real images: CreateProcess cannot run a #! script,
    // so a stub written here would never answer and the query built on it
    // would fail. Skip the stub tests instead of reporting that failure.
    return {};
#else
    static QTemporaryDir dir;
    if (!dir.isValid()) {
        return {};
    }

    QString path = dir.filePath(name);
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return {};
    }
    file.write(content);
    file.close();
    QFile::setPermissions(path, QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);
    return path;
#endif
}

// Replaces QTEST_MAIN / QTEST_GUILESS_MAIN: same behaviour, plus the redirect
// above. needsGui decides between QApplication and QCoreApplication - the
// command builder and helper utils tests never need a display.
inline int run(QObject *test, int argc, char *argv[], bool needsGui)
{
    redirectBaseDirectories();

    int result = 0;
    if (needsGui) {
        QApplication app(argc, argv);
        // The application name is the organization QSettings writes under, so
        // this is a second, independent guard on top of the XDG redirect - and
        // on Windows, where XDG plays no part, the only one.
        app.setApplicationName(QStringLiteral("hashcat-gui-tests"));
        result = QTest::qExec(test, argc, argv);
    } else {
        QCoreApplication app(argc, argv);
        app.setApplicationName(QStringLiteral("hashcat-gui-tests"));
        result = QTest::qExec(test, argc, argv);
    }

    return result;
}

} // namespace TestEnvironment

#endif // TESTENVIRONMENT_H
