/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfileparser.h"
#include "testenvironment.h"

#include <QByteArray>
#include <QFile>
#include <QTemporaryDir>
#include <QTest>

#include <atomic>
#include <memory>

namespace
{

bool writeFile(const QString &path, const QByteArray &content)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }
    return file.write(content) == content.size();
}

QByteArray goldenPotfile()
{
    return QByteArrayLiteral("hash-one:plain-one\n"
                             "salted-hash:salt:plain-two\n"
                             "hash-three:$HEX[68653a6c6c6f]\n"
                             "hash-four:\n"
                             "hash-five:$HCENC$1$key$cipher\n"
                             "hash-one:plain-one\n"
                             "not-a-record\n"
                             "hash-six:plain-six");
}

// Two different lines with the same first fingerprint and different second
// fingerprints, fed through the real parser's duplicate decision.
PotfileParser::LineFingerprint collisionFingerprint(const QByteArray &line)
{
    if (line.startsWith(QByteArrayLiteral("collision-one"))) {
        return {41, 1};
    }
    if (line.startsWith(QByteArrayLiteral("collision-two"))) {
        return {41, 2};
    }
    return PotfileParser::LineFingerprint::of(line);
}

} // namespace

class TestPotfileParser : public QObject
{
    Q_OBJECT

private slots:
    void parsesTheGoldenPotfile();
    void keepsNonUtf8HexEscapeLiteral();
    void aCustomSeparatorSplitsAtTheLastSeparator();
    void capsRetainedEntriesButKeepsTheCounts();
    void anUnterminatedFinalLineIsSkipped();
    void aMissingFileIsAnError();
    void anEmptyFileIsAnEmptyResult();
    void aCancelledParseStopsBeforeReading();
    void aMatchingFirstFingerprintAloneIsNotADuplicate();
};

void TestPotfileParser::parsesTheGoldenPotfile()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashcat.potfile"));
    QVERIFY(writeFile(path, goldenPotfile()));

    const PotfileParser::Result result = PotfileParser::parseFile(path);
    QVERIFY(result.error.isEmpty());
    QVERIFY(!result.cancelled);
    QCOMPARE(result.totalCount, 6);
    QCOMPARE(result.malformedCount, 1);
    QCOMPARE(result.encryptedCount, 1);
    QCOMPARE(result.noPasswordCount, 1);
    QCOMPARE(result.duplicateCount, 1);
    QCOMPARE(result.truncatedCount, 1);
    QCOMPARE(result.entries.size(), 6);

    QCOMPARE(result.entries.at(0).hash, QStringLiteral("hash-one"));
    QCOMPARE(result.entries.at(0).plain, QStringLiteral("plain-one"));
    QCOMPARE(result.entries.at(0).lineNumber, 1);
    QVERIFY(!result.entries.at(0).duplicate);

    QCOMPARE(result.entries.at(1).hash, QStringLiteral("salted-hash:salt"));
    QCOMPARE(result.entries.at(1).plain, QStringLiteral("plain-two"));

    QCOMPARE(result.entries.at(2).plain, QStringLiteral("he:llo"));

    QCOMPARE(result.entries.at(3).plain, QString());
    QVERIFY(result.entries.at(3).emptyPlain);

    QCOMPARE(result.entries.at(4).plain, QStringLiteral("$HCENC$1$key$cipher"));
    QVERIFY(result.entries.at(4).encrypted);

    QCOMPARE(result.entries.at(5).lineNumber, 6);
    QVERIFY(result.entries.at(5).duplicate);
}

void TestPotfileParser::keepsNonUtf8HexEscapeLiteral()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("binary.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("hash:$HEX[ff]\n")));

    const PotfileParser::Result result = PotfileParser::parseFile(path);
    QCOMPARE(result.entries.size(), 1);
    QCOMPARE(result.entries.at(0).plain, QStringLiteral("$HEX[ff]"));
}

void TestPotfileParser::aCustomSeparatorSplitsAtTheLastSeparator()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("custom.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("hash;plain;with:colons\n")));

    const PotfileParser::Result result =
        PotfileParser::parseFile(path, AppConstants::Hashcat::PotfileMaxEntries, QLatin1Char(';'));
    QCOMPARE(result.totalCount, 1);
    QCOMPARE(result.entries.size(), 1);
    QCOMPARE(result.entries.at(0).hash, QStringLiteral("hash;plain"));
    QCOMPARE(result.entries.at(0).plain, QStringLiteral("with:colons"));
}

void TestPotfileParser::capsRetainedEntriesButKeepsTheCounts()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("capped.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("one:a\n"
                                              "two:b\n"
                                              "three:c\n")));

    const PotfileParser::Result result = PotfileParser::parseFile(path, 2);
    QCOMPARE(result.totalCount, 3);
    QCOMPARE(result.entries.size(), 2);
    QCOMPARE(result.entries.at(0).hash, QStringLiteral("one"));
    QCOMPARE(result.entries.at(1).hash, QStringLiteral("two"));
}

void TestPotfileParser::anUnterminatedFinalLineIsSkipped()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("truncated.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("one:plain\n"
                                              "two:partial")));

    const PotfileParser::Result result = PotfileParser::parseFile(path);
    QCOMPARE(result.totalCount, 1);
    QCOMPARE(result.truncatedCount, 1);
    QCOMPARE(result.entries.size(), 1);
    QCOMPARE(result.entries.at(0).hash, QStringLiteral("one"));
}

void TestPotfileParser::aMissingFileIsAnError()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("missing.potfile"));

    const PotfileParser::Result result = PotfileParser::parseFile(path);
    QVERIFY(!result.error.isEmpty());
    QVERIFY(result.entries.isEmpty());
    QVERIFY(!QFile::exists(path));
}

void TestPotfileParser::anEmptyFileIsAnEmptyResult()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("empty.potfile"));
    QVERIFY(writeFile(path, QByteArray()));

    const PotfileParser::Result result = PotfileParser::parseFile(path);
    QVERIFY(result.error.isEmpty());
    QCOMPARE(result.totalCount, 0);
    QCOMPARE(result.entries.size(), 0);
}

void TestPotfileParser::aMatchingFirstFingerprintAloneIsNotADuplicate()
{
    // Collision regression: the parser must use both fingerprints, so a shared
    // first fingerprint with a different second one is not a duplicate.
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("collision.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("collision-one:plain-one\n"
                                              "collision-two:plain-two\n")));

    const PotfileParser::Result result = PotfileParser::parseFile(
        path, AppConstants::Hashcat::PotfileMaxEntries, QLatin1Char(':'), {}, collisionFingerprint);

    QCOMPARE(result.totalCount, 2);
    QCOMPARE(result.entries.size(), 2);
    QVERIFY(!result.entries.at(0).duplicate);
    QVERIFY(!result.entries.at(1).duplicate);
    QCOMPARE(result.duplicateCount, 0);
}

void TestPotfileParser::aCancelledParseStopsBeforeReading()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashcat.potfile"));
    QVERIFY(writeFile(path, goldenPotfile()));

    auto cancel = std::make_shared<std::atomic_bool>(true);
    const PotfileParser::Result result = PotfileParser::parseFile(
        path, AppConstants::Hashcat::PotfileMaxEntries, QLatin1Char(':'), cancel);
    QVERIFY(result.cancelled);
    QVERIFY(result.entries.isEmpty());
}

int main(int argc, char *argv[])
{
    TestPotfileParser test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_potfileparser.moc"