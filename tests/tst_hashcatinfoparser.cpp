/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include "hashcatinfoparser.h"

// The parser decides what reaches the hash type list on screen. A reply that
// cannot be used must leave that list alone and say why; a usable one must
// land in full, whatever whitespace and extra fields hashcat wrapped it in.
class TestHashcatInfoParser : public QObject
{
    Q_OBJECT

private slots:
    void prettyPrintedReplyParses();
    void errorIsClearedOnSuccess();
    void nonNumericKeysAreSkipped();
    void entriesWithoutANameAreSkipped();
    void unusableReplyIsRejected();
    void invalidReplyIsRejected();
    void failedParseKeepsThePreviousList();
};

// What hashcat prints: multi-line and indented, with fields the parser never
// reads. fromJson skips the whitespace itself - no squashing required.
void TestHashcatInfoParser::prettyPrintedReplyParses()
{
    const QString raw = QStringLiteral(R"({
    "0": {
        "name": "MD5",
        "extension": "hash",
        "category": "Raw Hash"
    },
    "1000": {
        "name": "NTLM",
        "extension": "ntlm",
        "category": "Raw Hash"
    }
})");

    QMap<quint32, QString> hashModes;
    QVERIFY(HashcatInfoParser::parseExampleHashes(raw, hashModes));
    QCOMPARE(hashModes.size(), 2);
    QCOMPARE(hashModes.value(0), QStringLiteral("0 | MD5"));
    QCOMPARE(hashModes.value(1000), QStringLiteral("1000 | NTLM"));
}

// The same QString travels through several refreshes; a reason left behind
// by a failed one must not show up as the error of the next successful run.
void TestHashcatInfoParser::errorIsClearedOnSuccess()
{
    QString errorMessage = QStringLiteral("left over from an earlier failure");
    QMap<quint32, QString> hashModes;

    QVERIFY(HashcatInfoParser::parseExampleHashes(
        QStringLiteral(R"({"0":{"name":"MD5"}})"), hashModes, &errorMessage));
    QVERIFY(errorMessage.isEmpty());
}

// Only keys hashcat could hand to -m are worth keeping: numeric, and inside
// quint32. "4294967295" is the last id that fits, "4294967296" already
// overflows - if it ever wrapped to 0 the value check below would catch it.
void TestHashcatInfoParser::nonNumericKeysAreSkipped()
{
    const QString raw = QStringLiteral(
        R"({"md5":{"name":"MD5"},"0":{"name":"MD5"},)"
        R"("4294967295":{"name":"Top"},"4294967296":{"name":"Overflow"}})");

    QMap<quint32, QString> hashModes;
    QVERIFY(HashcatInfoParser::parseExampleHashes(raw, hashModes));
    QCOMPARE(hashModes.size(), 2);
    QCOMPARE(hashModes.value(0), QStringLiteral("0 | MD5"));
    QCOMPARE(hashModes.value(4294967295u), QStringLiteral("4294967295 | Top"));
}

// An id without a usable name is dead weight: empty name, no name at all,
// or a whole entry that is not an object.
void TestHashcatInfoParser::entriesWithoutANameAreSkipped()
{
    const QString raw = QStringLiteral(
        R"({"0":{"name":"MD5"},"1":{"name":""},"2":{},"3":"MD5"})");

    QMap<quint32, QString> hashModes;
    QVERIFY(HashcatInfoParser::parseExampleHashes(raw, hashModes));
    QCOMPARE(hashModes.size(), 1);
    QCOMPARE(hashModes.value(0), QStringLiteral("0 | MD5"));
}

// Everything skippable at once: parsing succeeds per entry, but the reply
// as a whole carries no hash type - an empty list would silently blank the
// combo box, so it has to be an error instead.
void TestHashcatInfoParser::unusableReplyIsRejected()
{
    const QString raw = QStringLiteral(
        R"({"abc":{"name":"MD5"},"1":{"name":""},"2":{}})");

    QMap<quint32, QString> hashModes;
    QString errorMessage;
    QVERIFY(!HashcatInfoParser::parseExampleHashes(raw, hashModes, &errorMessage));
    QVERIFY(!errorMessage.isEmpty());
    QVERIFY(hashModes.isEmpty());
}

// Not JSON at all, and JSON of the wrong shape - both are "the reply cannot
// be used", reported in a translated sentence rather than left to the caller
// to guess. A nullptr errorMessage is the documented default and must be
// just as valid on the failing path.
void TestHashcatInfoParser::invalidReplyIsRejected()
{
    QMap<quint32, QString> hashModes;
    QString errorMessage;

    QVERIFY(!HashcatInfoParser::parseExampleHashes(
        QStringLiteral("Segmentation fault"), hashModes, &errorMessage));
    QVERIFY(!errorMessage.isEmpty());

    QVERIFY(!HashcatInfoParser::parseExampleHashes(
        QStringLiteral(R"([{"0":{"name":"MD5"}}])"), hashModes, &errorMessage));

    QVERIFY(!HashcatInfoParser::parseExampleHashes(QStringLiteral("nope"), hashModes));
}

// The list on screen is still worth showing after a failed refresh - the
// contract the class header makes, and the reason a failed query is not a
// reason to empty the combo box.
void TestHashcatInfoParser::failedParseKeepsThePreviousList()
{
    QMap<quint32, QString> hashModes{{quint32(0), QStringLiteral("0 | MD5")}};
    QString errorMessage;

    QVERIFY(!HashcatInfoParser::parseExampleHashes(
        QStringLiteral("garbage"), hashModes, &errorMessage));
    QCOMPARE(hashModes.size(), 1);
    QCOMPARE(hashModes.value(0), QStringLiteral("0 | MD5"));
}

int main(int argc, char *argv[])
{
    TestHashcatInfoParser test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_hashcatinfoparser.moc"
