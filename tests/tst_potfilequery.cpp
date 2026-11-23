/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appconstants.h"
#include "potfilequery.h"
#include "testenvironment.h"

#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QTemporaryDir>
#include <QTest>

class TestPotfileQuery : public QObject
{
    Q_OBJECT

private slots:
    void showArgsHaveAStableOrder();
    void leftArgsOmitJsonAndTheFormat();
    void emptyPotfilePathIsOmitted();
    void autoDetectOmitsTheHashType();
    void aLeadingDashIsProtected();
    void sessionsAreUnique();
    void resolveOnlyTreatsExistingPathsAsPaths();
    void relativePathsResolveAgainstTheProgramDirectory();
    void driveRelativePathIsNotJoinedOntoTheProgramDirectory();
};

void TestPotfileQuery::showArgsHaveAStableOrder()
{
    PotfileQueryInput input;
    input.hashInput = QStringLiteral("5d41402abc4b2a76b9719d911017c592");
    input.hashType = 0;
    input.potfilePath = QStringLiteral("/tmp/my.potfile");
    input.username = true;

    const QStringList args = PotfileQuery::showArgs(input);
    int i = 0;
    QCOMPARE(args.at(i++), QStringLiteral("--show"));
    QCOMPARE(args.at(i++), QStringLiteral("--logfile-disable"));
    QVERIFY(args.at(i++).startsWith(QStringLiteral("--session=hcgui-pot-")));
    QCOMPARE(args.at(i++), QStringLiteral("--json"));
    QCOMPARE(args.at(i++), QStringLiteral("-m"));
    QCOMPARE(args.at(i++), QStringLiteral("0"));
    QCOMPARE(args.at(i++), QStringLiteral("--potfile-path"));
    QCOMPARE(args.at(i++), QStringLiteral("/tmp/my.potfile"));
    QCOMPARE(args.at(i++), QStringLiteral("--username"));
    QCOMPARE(args.at(i++), input.hashInput);
    QCOMPARE(i, args.size());
}

void TestPotfileQuery::leftArgsOmitJsonAndTheFormat()
{
    PotfileQueryInput input;
    input.hashInput = QStringLiteral("hash");
    input.hashType = 1000;

    const QStringList args = PotfileQuery::leftArgs(input);
    QCOMPARE(args.first(), QStringLiteral("--left"));
    QVERIFY(args.contains(QStringLiteral("--logfile-disable")));
    QVERIFY(!args.contains(QStringLiteral("--json")));
    QVERIFY(!args.contains(QStringLiteral("--outfile-format")));
    QCOMPARE(args.last(), QStringLiteral("hash"));
}

void TestPotfileQuery::emptyPotfilePathIsOmitted()
{
    PotfileQueryInput input;
    input.hashInput = QStringLiteral("hash");

    QVERIFY(!PotfileQuery::showArgs(input).contains(QStringLiteral("--potfile-path")));
    QVERIFY(!PotfileQuery::leftArgs(input).contains(QStringLiteral("--potfile-path")));
}

void TestPotfileQuery::autoDetectOmitsTheHashType()
{
    PotfileQueryInput input;
    input.hashInput = QStringLiteral("hash");
    input.hashType = AppConstants::Hashcat::AutoDetectHashType;

    QVERIFY(!PotfileQuery::showArgs(input).contains(QStringLiteral("-m")));

    input.hashType = -1;
    QVERIFY(!PotfileQuery::showArgs(input).contains(QStringLiteral("-m")));
}

void TestPotfileQuery::aLeadingDashIsProtected()
{
    PotfileQueryInput input;
    input.hashInput = QStringLiteral("-not-a-hash");

    const QStringList args = PotfileQuery::showArgs(input);
    QCOMPARE(args.at(args.size() - 2), QStringLiteral("--"));
    QCOMPARE(args.last(), input.hashInput);
}

void TestPotfileQuery::sessionsAreUnique()
{
    PotfileQueryInput input;
    input.hashInput = QStringLiteral("hash");

    const QStringList show = PotfileQuery::showArgs(input);
    QString first;
    for (const QString &arg : show) {
        if (arg.startsWith(QStringLiteral("--session="))) {
            first = arg;
            break;
        }
    }
    const QStringList left = PotfileQuery::leftArgs(input);
    QString second;
    for (const QString &arg : left) {
        if (arg.startsWith(QStringLiteral("--session="))) {
            second = arg;
            break;
        }
    }

    QVERIFY(!first.isEmpty());
    QVERIFY(!second.isEmpty());
    QVERIFY(first != second);
}

void TestPotfileQuery::resolveOnlyTreatsExistingPathsAsPaths()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashes.txt"));
    QFile file(path);
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.close();

    QCOMPARE(PotfileQuery::resolveHashInput(path), QFileInfo(path).absoluteFilePath());
    QCOMPARE(PotfileQuery::resolveHashInput(QStringLiteral("missing-hashes.txt")),
             QStringLiteral("missing-hashes.txt"));
    QCOMPARE(PotfileQuery::resolveHashInput(QStringLiteral("$2b$12$abcdefghijklmnopqrstuv/xyz")),
             QStringLiteral("$2b$12$abcdefghijklmnopqrstuv/xyz"));
}

void TestPotfileQuery::relativePathsResolveAgainstTheProgramDirectory()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString program = dir.filePath(QStringLiteral("hashcat"));
    const QString hashFile = dir.filePath(QStringLiteral("hashes.txt"));
    QFile file(hashFile);
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.close();

    QCOMPARE(PotfileQuery::resolvePath(QStringLiteral("hashcat.potfile"), program),
             QFileInfo(dir.filePath(QStringLiteral("hashcat.potfile"))).absoluteFilePath());
    QCOMPARE(PotfileQuery::resolveHashInput(QStringLiteral("hashes.txt"), program),
             QFileInfo(hashFile).absoluteFilePath());
    // An absolute path is never rebased.
    QCOMPARE(PotfileQuery::resolvePath(hashFile, program), hashFile);
}

void TestPotfileQuery::driveRelativePathIsNotJoinedOntoTheProgramDirectory()
{
#ifdef Q_OS_WIN
    const QString program = QStringLiteral("C:/Program Files/hashcat/hashcat.exe");
    const QString resolved = PotfileQuery::resolvePath(QStringLiteral("C:hashes.txt"), program);

    QVERIFY(QFileInfo(resolved).isAbsolute());
    QVERIFY(!resolved.contains(QStringLiteral("hashcat/C:")));
    QVERIFY(!resolved.contains(QStringLiteral("hashcat\\C:")));
#else
    QSKIP("Drive-relative paths are Windows-only.");
#endif
}

int main(int argc, char *argv[])
{
    TestPotfileQuery test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_potfilequery.moc"