/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "hashcatinfoparser.h"
#include "testenvironment.h"

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
    void hashcat8ModesParseInOrder();
    void bannerPrefixedReplyIsRejected();
    void unusableReplyIsRejected();
    void invalidReplyIsRejected();
    void failedParseKeepsThePreviousList();
    void utf8NamesArePreserved();
    void identifyTableParses();
    void identifySkipsPreambleAndSeparator();
    void identifyNoMatchIsItsOwnStatus();
    void identifyNeedsATable();
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

    QVERIFY(HashcatInfoParser::parseExampleHashes(QStringLiteral(R"({"0":{"name":"MD5"}})"),
                                                  hashModes, &errorMessage));
    QVERIFY(errorMessage.isEmpty());
}

// Only keys hashcat could hand to -m are worth keeping: numeric, and inside
// quint32. "4294967295" is the last id that fits, "4294967296" already
// overflows - if it ever wrapped to 0 the value check below would catch it.
void TestHashcatInfoParser::nonNumericKeysAreSkipped()
{
    const QString raw =
        QStringLiteral(R"({"md5":{"name":"MD5"},"0":{"name":"MD5"},)"
                       R"("4294967295":{"name":"Top"},"4294967296":{"name":"Overflow"}})");

    QMap<quint32, QString> hashModes;
    QVERIFY(HashcatInfoParser::parseExampleHashes(raw, hashModes));
    QCOMPARE(hashModes.size(), 2);
    QCOMPARE(hashModes.value(0), QStringLiteral("0 | MD5"));
    QCOMPARE(hashModes.value(4294967295U), QStringLiteral("4294967295 | Top"));
}

// An id without a usable name is dead weight: empty name, no name at all,
// or a whole entry that is not an object.
void TestHashcatInfoParser::entriesWithoutANameAreSkipped()
{
    const QString raw = QStringLiteral(R"({"0":{"name":"MD5"},"1":{"name":""},"2":{},"3":"MD5"})");

    QMap<quint32, QString> hashModes;
    QVERIFY(HashcatInfoParser::parseExampleHashes(raw, hashModes));
    QCOMPARE(hashModes.size(), 1);
    QCOMPARE(hashModes.value(0), QStringLiteral("0 | MD5"));
}

// The 8.0.0 numbering: the renumbered modes arrive in the 40000-44000 range
// and the old numbers are gone from --example-hashes. The list is keyed by
// quint32 and has to come out in numeric order, whatever order the JSON used.
void TestHashcatInfoParser::hashcat8ModesParseInOrder()
{
    const QString raw = QStringLiteral(R"({
    "99999": {"name": "Vendor"},
    "40032": {"name": "PBKDF2-HMAC-SHA1"},
    "40009": {"name": "sha224"},
    "73000": {"name": "Second vendor"},
    "40000": {"name": "SHA2-256"}
})");

    QMap<quint32, QString> hashModes;
    QVERIFY(HashcatInfoParser::parseExampleHashes(raw, hashModes));

    const QList<quint32> expectedKeys{40000, 40009, 40032, 73000, 99999};
    QCOMPARE(hashModes.keys(), expectedKeys);
    QCOMPARE(hashModes.value(40000), QStringLiteral("40000 | SHA2-256"));
    QCOMPARE(hashModes.value(40009), QStringLiteral("40009 | sha224"));
    QCOMPARE(hashModes.value(40032), QStringLiteral("40032 | PBKDF2-HMAC-SHA1"));

    // The pre-8.0.0 number is not part of the reply, so nothing invents it.
    QVERIFY(!hashModes.contains(12000));
}

// HashcatRunner::execute() appends --quiet for a reason: fromJson skips
// leading whitespace only, so an unquiet banner would fail the parse at char
// zero. This pins the contract that makes the flag load-bearing.
void TestHashcatInfoParser::bannerPrefixedReplyIsRejected()
{
    const QString raw = QStringLiteral("hashcat (v8.0.0) starting in autodetect mode\n\n"
                                       R"({"40009":{"name":"sha224"}})");

    QMap<quint32, QString> hashModes;
    QString errorMessage;
    QVERIFY(!HashcatInfoParser::parseExampleHashes(raw, hashModes, &errorMessage));
    QVERIFY(!errorMessage.isEmpty());
    QVERIFY(hashModes.isEmpty());
}

// Everything skippable at once: parsing succeeds per entry, but the reply
// as a whole carries no hash type - an empty list would silently blank the
// combo box, so it has to be an error instead.
void TestHashcatInfoParser::unusableReplyIsRejected()
{
    const QString raw = QStringLiteral(R"({"abc":{"name":"MD5"},"1":{"name":""},"2":{}})");

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

    QVERIFY(!HashcatInfoParser::parseExampleHashes(QStringLiteral("Segmentation fault"), hashModes,
                                                   &errorMessage));
    QVERIFY(!errorMessage.isEmpty());

    QVERIFY(!HashcatInfoParser::parseExampleHashes(QStringLiteral(R"([{"0":{"name":"MD5"}}])"),
                                                   hashModes, &errorMessage));

    QVERIFY(!HashcatInfoParser::parseExampleHashes(QStringLiteral("nope"), hashModes));
}

// The list on screen is still worth showing after a failed refresh - the
// contract the class header makes, and the reason a failed query is not a
// reason to empty the combo box.
void TestHashcatInfoParser::failedParseKeepsThePreviousList()
{
    QMap<quint32, QString> hashModes{{quint32(0), QStringLiteral("0 | MD5")}};
    QString errorMessage;

    QVERIFY(!HashcatInfoParser::parseExampleHashes(QStringLiteral("garbage"), hashModes,
                                                   &errorMessage));
    QCOMPARE(hashModes.size(), 1);
    QCOMPARE(hashModes.value(0), QStringLiteral("0 | MD5"));
}

// A hash type's display name is UTF-8 text and may hold characters outside
// ASCII. It travels from the JSON bytes to the combo box and has to arrive
// there unchanged; the literals are \u escapes so the test does not depend on
// the encoding of its own source file.
void TestHashcatInfoParser::utf8NamesArePreserved()
{
    const QString raw = QStringLiteral(
        "{\"0\":{\"name\":\"MD5 \u2013 \u00e4\u00f6\u00fc\u00df \u65e5\u672c\u8a9e\"}}");

    QMap<quint32, QString> hashModes;
    QVERIFY(HashcatInfoParser::parseExampleHashes(raw, hashModes));
    QCOMPARE(hashModes.value(0),
             QStringLiteral("0 | MD5 \u2013 \u00e4\u00f6\u00fc\u00df \u65e5\u672c\u8a9e"));
    // every character is still whole: no replacement character crept in
    QVERIFY(!hashModes.value(0).contains(QChar(0xFFFD)));
}

void TestHashcatInfoParser::identifyTableParses()
{
    const QString raw =
        QStringLiteral(R"(The following 3 hash-modes match the structure of your input hash:

      # | Name                                                       | Category
  ======+============================================================+======================================
  40102 | HAVAL-128 (3 passes)                                       | Raw Hash
      0 | MD5                                                        | Raw Hash
   1000 | NTLM                                                       | Operating System

)");

    const HashcatInfoParser::IdentifyResult result = HashcatInfoParser::parseIdentify(raw);
    QCOMPARE(result.status, HashcatInfoParser::IdentifyResult::Status::Matched);
    QCOMPARE(result.matches.size(), 3);
    QCOMPARE(result.matches.at(0).id, quint32(40102));
    QCOMPARE(result.matches.at(0).name, QStringLiteral("HAVAL-128 (3 passes)"));
    QCOMPARE(result.matches.at(0).category, QStringLiteral("Raw Hash"));
    QCOMPARE(result.matches.at(1).id, quint32(0));
    QCOMPARE(result.matches.at(2).id, quint32(1000));
    QCOMPARE(result.matches.at(2).category, QStringLiteral("Operating System"));
}

void TestHashcatInfoParser::identifySkipsPreambleAndSeparator()
{
    const QString raw = QStringLiteral(R"(a warning from a plugin
The following 1 hash-modes match the structure of your input hash:

      # | Name                                                       | Category
  ======+============================================================+======================================
      0 | MD5                                                        | Raw Hash

Started: Sun Nov 22 17:54:08 2026
)");

    const HashcatInfoParser::IdentifyResult result = HashcatInfoParser::parseIdentify(raw);
    QCOMPARE(result.status, HashcatInfoParser::IdentifyResult::Status::Matched);
    QCOMPARE(result.matches.size(), 1);
    QCOMPARE(result.matches.at(0).name, QStringLiteral("MD5"));
}

// The explicit "no match" sentence is an answer of its own, distinct from
// output that merely failed to parse.
void TestHashcatInfoParser::identifyNoMatchIsItsOwnStatus()
{
    const HashcatInfoParser::IdentifyResult result = HashcatInfoParser::parseIdentify(
        QStringLiteral("No hash-mode matches the structure of the input hash."));
    QCOMPARE(result.status, HashcatInfoParser::IdentifyResult::Status::NoMatch);
    QVERIFY(result.matches.isEmpty());
}

void TestHashcatInfoParser::identifyNeedsATable()
{
    const HashcatInfoParser::IdentifyResult garbage =
        HashcatInfoParser::parseIdentify(QStringLiteral("Segmentation fault"));
    QCOMPARE(garbage.status, HashcatInfoParser::IdentifyResult::Status::ParseError);
    QVERIFY(garbage.matches.isEmpty());

    const QString headerOnly = QStringLiteral(R"(      # | Name | Category
  ======+======+==========
)");
    const HashcatInfoParser::IdentifyResult empty = HashcatInfoParser::parseIdentify(headerOnly);
    QCOMPARE(empty.status, HashcatInfoParser::IdentifyResult::Status::ParseError);
    QVERIFY(empty.matches.isEmpty());
}

int main(int argc, char *argv[])
{
    TestHashcatInfoParser test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_hashcatinfoparser.moc"
