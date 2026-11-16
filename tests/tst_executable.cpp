/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "executable.h"
#include "testenvironment.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTemporaryDir>

namespace
{
// A file name Windows treats as executable (Qt checks the known program
// suffixes there); on Unix the executable bit is what counts.
QString programFileName(const QString &base)
{
#ifdef Q_OS_WIN
    return base + QStringLiteral(".exe");
#else
    return base;
#endif
}
} // namespace

// The one place that turns a configured program into what a process is
// started from: a bare name follows PATH, a path with a directory part is
// made absolute, and anything that cannot run resolves to nothing.
class TestExecutable : public QObject
{
    Q_OBJECT

private slots:
    void emptyInputResolvesToNothing();
    void bareNameOnPathResolves();
    void bareNameOffPathResolvesToNothing();
    void absolutePathResolves();
    void relativePathWithDirectoryResolvesToAbsolute();
    void directoryIsNotAnExecutable();
    void nonExecutableFileIsRefused();
    void pathWithSpacesAndASmartQuoteResolves();
};

void TestExecutable::emptyInputResolvesToNothing()
{
    QCOMPARE(Executable::resolve(QString()), QString());
}

void TestExecutable::bareNameOnPathResolves()
{
    const QString name = programFileName(QStringLiteral("hashcat-gui-resolve-test"));
    const QString stub = TestEnvironment::writeSearchableExecutable(name);
    QVERIFY2(!stub.isEmpty(), "the executable stub could not be written");

    TestEnvironment::PathGuard guard(QFileInfo(stub).absolutePath().toUtf8());

    const QString found = Executable::resolve(name);
    QVERIFY2(!found.isEmpty(), "the bare name on PATH was not found");
    QVERIFY2(QDir::isAbsolutePath(found), "a PATH lookup did not return an absolute path");
    QCOMPARE(QFileInfo(found).canonicalFilePath(), QFileInfo(stub).canonicalFilePath());
}

void TestExecutable::bareNameOffPathResolvesToNothing()
{
    QTemporaryDir empty;
    QVERIFY(empty.isValid());

    TestEnvironment::PathGuard guard(empty.path().toUtf8());

    QCOMPARE(Executable::resolve(QStringLiteral("hashcat-gui-no-such-program")), QString());
}

void TestExecutable::absolutePathResolves()
{
    const QString stub = TestEnvironment::writeSearchableExecutable(
        programFileName(QStringLiteral("hashcat-gui-absolute-test")));
    QVERIFY2(!stub.isEmpty(), "the executable stub could not be written");

    const QString resolved = Executable::resolve(stub);
    QVERIFY2(!resolved.isEmpty(), "an absolute executable path was refused");
    QCOMPARE(QFileInfo(resolved).canonicalFilePath(), QFileInfo(stub).canonicalFilePath());
}

// A relative path is made absolute against the working directory before it
// is checked; the working directory is the one hashcat then runs in.
void TestExecutable::relativePathWithDirectoryResolvesToAbsolute()
{
    // Under the working directory, so the relative form is valid on every
    // platform - Windows returns an absolute path across drives.
    QTemporaryDir directory(QDir::current().filePath(QStringLiteral("resolve-test-XXXXXX")));
    QVERIFY(directory.isValid());
    const QString stub = TestEnvironment::writeSearchableExecutable(
        programFileName(QStringLiteral("hashcat-gui-relative-test")), directory.path());
    QVERIFY2(!stub.isEmpty(), "the executable stub could not be written");

    const QString relative = QDir::current().relativeFilePath(stub);
    QVERIFY2(relative.contains(QLatin1Char('/')) || relative.contains(QLatin1Char('\\')),
             "the test needs a path with a directory part");

    const QString resolved = Executable::resolve(relative);
    QVERIFY2(!resolved.isEmpty(), "a relative executable path was refused");
    QVERIFY2(QDir::isAbsolutePath(resolved), "a relative path was not made absolute");
    QCOMPARE(QFileInfo(resolved).canonicalFilePath(), QFileInfo(stub).canonicalFilePath());
}

void TestExecutable::directoryIsNotAnExecutable()
{
    QCOMPARE(Executable::resolve(QDir::tempPath()), QString());
}

void TestExecutable::nonExecutableFileIsRefused()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString path = directory.filePath(QStringLiteral("not-executable.txt"));
    QFile file(path);
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write("data");
    file.close();
    QVERIFY(QFile::setPermissions(path, QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup
                                            | QFile::ReadOther));

    QCOMPARE(Executable::resolve(path), QString());
}

void TestExecutable::pathWithSpacesAndASmartQuoteResolves()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString stub = TestEnvironment::writeSearchableExecutable(
        programFileName(QStringLiteral("hashcat’s tool with spaces")), directory.path());
    QVERIFY2(!stub.isEmpty(), "the executable stub could not be written");

    const QString resolved = Executable::resolve(stub);
    QVERIFY2(!resolved.isEmpty(), "a path with spaces and a smart quote was refused");
    QCOMPARE(QFileInfo(resolved).canonicalFilePath(), QFileInfo(stub).canonicalFilePath());
}

int main(int argc, char *argv[])
{
    TestExecutable test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_executable.moc"
