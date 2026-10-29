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

#include "pathmonitor.h"

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

class TestPathMonitor : public QObject
{
    Q_OBJECT

private slots:
    void reportsTheStateOfEveryPath();
    void removingAndRestoringAFileChangesTheState();
    void removingAndRecreatingADirectoryChangesTheState();
    void replacingAFileKeepsItWatched();
    void restoringTheDirectoryChangesTheStateBack();
    void emptyWordlistFolderNeedsAFile();
    void subfoldersAndDotfilesAreNotWordlists();
    void tableFolderIsRefused();
    void pcfgFolderNeedsItsGrammar();
    void pcfgArchiveAndBundledNamesAreAvailable();
};

// The caller reports the state of the rows it just built, so even a path that
// was watched before has to be reported again - and only once, even if it is
// in the list twice.
void TestPathMonitor::reportsTheStateOfEveryPath()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString presentPath = directory.filePath(QStringLiteral("present.txt"));
    QVERIFY(writeFile(presentPath));

    const QString missingPath = directory.filePath(QStringLiteral("missing.txt"));
    const QString directoryPath = directory.filePath(QStringLiteral("subdirectory"));
    QVERIFY(QDir().mkpath(directoryPath));
    QVERIFY(writeFile(QDir(directoryPath).filePath(QStringLiteral("words.txt"))));

    PathMonitor monitor(PathMonitor::Kind::Wordlist);
    QSignalSpy spy(&monitor, &PathMonitor::availabilityChanged);

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

void TestPathMonitor::removingAndRestoringAFileChangesTheState()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString path = directory.filePath(QStringLiteral("wordlist.txt"));
    QVERIFY(writeFile(path));

    PathMonitor monitor(PathMonitor::Kind::Wordlist);
    monitor.setPaths({path});
    QVERIFY(monitor.isAvailable(path));

    QSignalSpy spy(&monitor, &PathMonitor::availabilityChanged);

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
void TestPathMonitor::removingAndRecreatingADirectoryChangesTheState()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString path = directory.filePath(QStringLiteral("wordlists"));
    QVERIFY(QDir().mkpath(path));
    QVERIFY(writeFile(QDir(path).filePath(QStringLiteral("words.txt"))));

    PathMonitor monitor(PathMonitor::Kind::Wordlist);
    monitor.setPaths({path});
    QVERIFY(monitor.isAvailable(path));

    QSignalSpy spy(&monitor, &PathMonitor::availabilityChanged);

    QVERIFY(QDir(path).removeRecursively());
    QTRY_COMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), path);
    QCOMPARE(spy.at(0).at(1).toBool(), false);
    QVERIFY(!monitor.isAvailable(path));

    QVERIFY(QDir().mkpath(path));
    QVERIFY(writeFile(QDir(path).filePath(QStringLiteral("words.txt"))));
    QTRY_COMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toString(), path);
    QCOMPARE(spy.at(1).at(1).toBool(), true);
    QVERIFY(monitor.isAvailable(path));
}

// Editors save by writing a temporary file and renaming it over the old one.
// The watch has to survive that and still notice a later removal.
void TestPathMonitor::replacingAFileKeepsItWatched()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString path = directory.filePath(QStringLiteral("wordlist.txt"));
    QVERIFY(writeFile(path));

    PathMonitor monitor(PathMonitor::Kind::Wordlist);
    monitor.setPaths({path});
    QVERIFY(monitor.isAvailable(path));

    {
        QSaveFile replacement(path);
        QVERIFY(replacement.open(QIODevice::WriteOnly));
        QCOMPARE(replacement.write(QByteArrayLiteral("replacement\n")), qint64(12));
        QVERIFY(replacement.commit());
    }
    QTRY_VERIFY(monitor.isAvailable(path));

    QSignalSpy spy(&monitor, &PathMonitor::availabilityChanged);
    QVERIFY(QFile::remove(path));
    QTRY_COMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(1).toBool(), false);
}

// A parent directory that is removed as a whole loses its watch. The monitor
// has to fall back to the nearest existing ancestor, or the file coming back
// with the directory would never be noticed.
void TestPathMonitor::restoringTheDirectoryChangesTheStateBack()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString subdirectory = directory.filePath(QStringLiteral("sub"));
    QVERIFY(QDir().mkpath(subdirectory));
    const QString path = QDir(subdirectory).filePath(QStringLiteral("wordlist.txt"));
    QVERIFY(writeFile(path));

    PathMonitor monitor(PathMonitor::Kind::Wordlist);
    QSignalSpy spy(&monitor, &PathMonitor::availabilityChanged);
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

// hashcat's scan_directory() skips subdirectories and dotfiles, so a folder
// that holds only those leaves the feed with nothing and the run stops with
// "No usable dictionary file found." - the row has to say so, and has to
// recover when a wordlist arrives.
void TestPathMonitor::emptyWordlistFolderNeedsAFile()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString path = directory.filePath(QStringLiteral("wordlists"));
    QVERIFY(QDir().mkpath(path));

    PathMonitor monitor(PathMonitor::Kind::Wordlist);
    QSignalSpy spy(&monitor, &PathMonitor::availabilityChanged);
    monitor.setPaths({path});
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(1).toBool(), false);
    QVERIFY(!monitor.isAvailable(path));

    const QString wordlist = QDir(path).filePath(QStringLiteral("words.txt"));
    QVERIFY(writeFile(wordlist));
    QTRY_COMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toString(), path);
    QCOMPARE(spy.at(1).at(1).toBool(), true);
    QVERIFY(monitor.isAvailable(path));

    // A folder that loses its last wordlist is unusable again.
    QVERIFY(QFile::remove(wordlist));
    QTRY_COMPARE(spy.count(), 3);
    QCOMPARE(spy.at(2).at(1).toBool(), false);
    QVERIFY(!monitor.isAvailable(path));
}

void TestPathMonitor::subfoldersAndDotfilesAreNotWordlists()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString path = directory.filePath(QStringLiteral("wordlists"));
    QVERIFY(QDir().mkpath(path));
    QVERIFY(QDir().mkpath(QDir(path).filePath(QStringLiteral("nested"))));
    QVERIFY(writeFile(QDir(path).filePath(QStringLiteral("nested/inside.txt"))));
    QVERIFY(writeFile(QDir(path).filePath(QStringLiteral(".hidden"))));

    PathMonitor monitor(PathMonitor::Kind::Wordlist);
    monitor.setPaths({path});
    QVERIFY(!monitor.isAvailable(path));

    // A regular file next to them is what the folder needs.
    QVERIFY(writeFile(QDir(path).filePath(QStringLiteral("words.txt"))));
    QTRY_VERIFY(monitor.isAvailable(path));
}

// The table feed opens every argument as a file and never scans a folder, so a
// table folder is unusable even when it holds table files.
void TestPathMonitor::tableFolderIsRefused()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString table = directory.filePath(QStringLiteral("table.txt"));
    QVERIFY(writeFile(table));

    const QString folder = directory.filePath(QStringLiteral("tables"));
    QVERIFY(QDir().mkpath(folder));
    QVERIFY(writeFile(QDir(folder).filePath(QStringLiteral("inside.txt"))));

    PathMonitor monitor(PathMonitor::Kind::Table);
    QSignalSpy spy(&monitor, &PathMonitor::availabilityChanged);
    monitor.setPaths({table, folder});

    QCOMPARE(spy.count(), 2);
    QVERIFY(monitor.isAvailable(table));
    QVERIFY(!monitor.isAvailable(folder));

    // Removing the folder turns it into a missing path, which is just as
    // unusable: the state stays false and reports nothing new.
    QVERIFY(QDir(folder).removeRecursively());
    QTest::qWait(100);
    QCOMPARE(spy.count(), 2);
    QVERIFY(!monitor.isAvailable(folder));
}

// A ruleset folder is named by its grammar; creating or removing the grammar
// has to change the answer without a new setPaths() call.
void TestPathMonitor::pcfgFolderNeedsItsGrammar()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString ruleset = directory.filePath(QStringLiteral("trained"));
    QVERIFY(QDir().mkpath(ruleset));

    PathMonitor monitor(PathMonitor::Kind::Pcfg);
    QSignalSpy spy(&monitor, &PathMonitor::availabilityChanged);
    monitor.setPaths({ruleset});
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(1).toBool(), false);
    QVERIFY(!monitor.isAvailable(ruleset));

    const QString grammar = QDir(ruleset).filePath(QStringLiteral("Grammar"));
    QVERIFY(QDir().mkpath(grammar));
    QVERIFY(writeFile(QDir(grammar).filePath(QStringLiteral("grammar.txt"))));
    QTRY_COMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toString(), ruleset);
    QCOMPARE(spy.at(1).at(1).toBool(), true);
    QVERIFY(monitor.isAvailable(ruleset));

    // The grammar is a file of its own: removing it leaves the folder without
    // one, even though the folder itself is still there.
    QVERIFY(QFile::remove(QDir(grammar).filePath(QStringLiteral("grammar.txt"))));
    QTRY_COMPARE(spy.count(), 3);
    QCOMPARE(spy.at(2).at(1).toBool(), false);
    QVERIFY(!monitor.isAvailable(ruleset));
}

void TestPathMonitor::pcfgArchiveAndBundledNamesAreAvailable()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString archive = directory.filePath(QStringLiteral("default-passwords.tar.xz"));
    QVERIFY(writeFile(archive));

    // A name without a separator is resolved by hashcat in its own directory,
    // so the GUI cannot tell whether it exists and must not flag it.
    const QString bundled = QStringLiteral("default-passwords");

    // An absolute folder without a grammar is a broken ruleset, and an
    // absolute path that is gone is missing.
    const QString bareFolder = directory.filePath(QStringLiteral("ruleset-without-grammar"));
    QVERIFY(QDir().mkpath(bareFolder));
    const QString missing = directory.filePath(QStringLiteral("gone.tar.xz"));

    PathMonitor monitor(PathMonitor::Kind::Pcfg);
    QSignalSpy spy(&monitor, &PathMonitor::availabilityChanged);
    monitor.setPaths({archive, bundled, bareFolder, missing});

    QCOMPARE(spy.count(), 4);
    QVERIFY(monitor.isAvailable(archive));
    QVERIFY(monitor.isAvailable(bundled));
    QVERIFY(!monitor.isAvailable(bareFolder));
    QVERIFY(!monitor.isAvailable(missing));
}

int main(int argc, char *argv[])
{
    TestPathMonitor test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_pathmonitor.moc"
