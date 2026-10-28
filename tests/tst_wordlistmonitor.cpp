/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QDir>
#include <QFile>
#include <QSaveFile>
#include <QSignalSpy>
#include <QTemporaryDir>

#include "wordlistmonitor.h"

namespace
{

bool writeFile(const QString &path, const QByteArray &content = QByteArrayLiteral("password\n"))
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }
    return file.write(content) == content.size();
}

} // namespace

class TestWordlistMonitor : public QObject
{
    Q_OBJECT

private slots:
    void reportsTheStateOfEveryPath();
    void removingAndRestoringAFileChangesTheState();
    void removingAndRecreatingADirectoryChangesTheState();
    void replacingAFileKeepsItWatched();
    void restoringTheDirectoryChangesTheStateBack();
};

// The caller reports the state of the rows it just built, so even a path that
// was watched before has to be reported again - and only once, even if it is
// in the list twice.
void TestWordlistMonitor::reportsTheStateOfEveryPath()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString presentPath = directory.filePath(QStringLiteral("present.txt"));
    QVERIFY(writeFile(presentPath));

    const QString missingPath = directory.filePath(QStringLiteral("missing.txt"));
    const QString directoryPath = directory.filePath(QStringLiteral("subdirectory"));
    QVERIFY(QDir().mkpath(directoryPath));

    WordlistMonitor monitor;
    QSignalSpy spy(&monitor, &WordlistMonitor::availabilityChanged);

    monitor.setPaths({presentPath, missingPath, directoryPath, presentPath, QString()});

    QCOMPARE(spy.count(), 3);
    QCOMPARE(spy.at(0).at(0).toString(), presentPath);
    QCOMPARE(spy.at(0).at(1).toBool(), true);
    QCOMPARE(spy.at(1).at(0).toString(), missingPath);
    QCOMPARE(spy.at(1).at(1).toBool(), false);
    QCOMPARE(spy.at(2).at(0).toString(), directoryPath);
    QCOMPARE(spy.at(2).at(1).toBool(), true);

    QVERIFY(monitor.isAvailable(presentPath));
    QVERIFY(!monitor.isAvailable(missingPath));
    QVERIFY(monitor.isAvailable(directoryPath));
}

void TestWordlistMonitor::removingAndRestoringAFileChangesTheState()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString path = directory.filePath(QStringLiteral("wordlist.txt"));
    QVERIFY(writeFile(path));

    WordlistMonitor monitor;
    monitor.setPaths({path});
    QVERIFY(monitor.isAvailable(path));

    QSignalSpy spy(&monitor, &WordlistMonitor::availabilityChanged);

    QVERIFY(QFile::remove(path));
    QTRY_COMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), path);
    QCOMPARE(spy.at(0).at(1).toBool(), false);
    QVERIFY(!monitor.isAvailable(path));

    QVERIFY(writeFile(path));
    QTRY_COMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toString(), path);
    QCOMPARE(spy.at(1).at(1).toBool(), true);
    QVERIFY(monitor.isAvailable(path));
}

// A directory is a wordlist input since hashcat 8.0.0, so a row naming one
// is available - and its removal and recreation have to be noticed just like
// a file's.
void TestWordlistMonitor::removingAndRecreatingADirectoryChangesTheState()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString path = directory.filePath(QStringLiteral("wordlists"));
    QVERIFY(QDir().mkpath(path));

    WordlistMonitor monitor;
    monitor.setPaths({path});
    QVERIFY(monitor.isAvailable(path));

    QSignalSpy spy(&monitor, &WordlistMonitor::availabilityChanged);

    QVERIFY(QDir(path).removeRecursively());
    QTRY_COMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), path);
    QCOMPARE(spy.at(0).at(1).toBool(), false);
    QVERIFY(!monitor.isAvailable(path));

    QVERIFY(QDir().mkpath(path));
    QTRY_COMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toString(), path);
    QCOMPARE(spy.at(1).at(1).toBool(), true);
    QVERIFY(monitor.isAvailable(path));
}

// Editors save by writing a temporary file and renaming it over the old one.
// The watch has to survive that and still notice a later removal.
void TestWordlistMonitor::replacingAFileKeepsItWatched()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString path = directory.filePath(QStringLiteral("wordlist.txt"));
    QVERIFY(writeFile(path));

    WordlistMonitor monitor;
    monitor.setPaths({path});
    QVERIFY(monitor.isAvailable(path));

    {
        QSaveFile replacement(path);
        QVERIFY(replacement.open(QIODevice::WriteOnly));
        QCOMPARE(replacement.write(QByteArrayLiteral("replacement\n")), qint64(12));
        QVERIFY(replacement.commit());
    }
    QTRY_VERIFY(monitor.isAvailable(path));

    QSignalSpy spy(&monitor, &WordlistMonitor::availabilityChanged);
    QVERIFY(QFile::remove(path));
    QTRY_COMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(1).toBool(), false);
}

// A parent directory that is removed as a whole loses its watch. The monitor
// has to fall back to the nearest existing ancestor, or the file coming back
// with the directory would never be noticed.
void TestWordlistMonitor::restoringTheDirectoryChangesTheStateBack()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString subdirectory = directory.filePath(QStringLiteral("sub"));
    QVERIFY(QDir().mkpath(subdirectory));
    const QString path = QDir(subdirectory).filePath(QStringLiteral("wordlist.txt"));
    QVERIFY(writeFile(path));

    WordlistMonitor monitor;
    QSignalSpy spy(&monitor, &WordlistMonitor::availabilityChanged);
    monitor.setPaths({path});
    QCOMPARE(spy.count(), 1);
    QVERIFY(monitor.isAvailable(path));

    QVERIFY(QDir(subdirectory).removeRecursively());
    QTRY_COMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toString(), path);
    QCOMPARE(spy.at(1).at(1).toBool(), false);
    QVERIFY(!monitor.isAvailable(path));

    QVERIFY(QDir().mkpath(subdirectory));
    QVERIFY(writeFile(path));
    QTRY_COMPARE(spy.count(), 3);
    QCOMPARE(spy.at(2).at(0).toString(), path);
    QCOMPARE(spy.at(2).at(1).toBool(), true);
    QVERIFY(monitor.isAvailable(path));
}

int main(int argc, char *argv[])
{
    TestWordlistMonitor test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_wordlistmonitor.moc"
