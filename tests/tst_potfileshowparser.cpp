/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfileshowparser.h"
#include "potfiletesthelpers.h"
#include "testenvironment.h"

#include <QTest>

#include <atomic>
#include <memory>

class TestPotfileShowParser : public QObject
{
    Q_OBJECT

private slots:
    void parsesEntriesAndFlags();
    void decodesHexEscapedPlaintext();
    void keepsNonUtf8HexEscapeLiteral();
    void keepsAnInvalidHexEscapeLiteral();
    void parsesBinaryHashFileEntries();
    void capsRetainedEntriesButKeepsTheTotal();
    void emptyOutputIsAValidEmptyAnswer();
    void malformedLinesAreCounted();
    void autoDetectPreambleIsSkipped();
    void legacyDirectoryBlockIsSkippedWhole();
    void oldPotfileBlockIsSkipped();
    void repeatedHashfileWarningsAreSkipped();
    void anUnknownLeadingLineIsStillMalformed();
    void diagnosticsOnlyOutputHasNoEntries();
    void diagnosticsWithoutTrailingNewlineStayInRange();
    void noiseAfterThePayloadIsStillMalformed();
    void aCancelledTokenStopsTheScan();
};

void TestPotfileShowParser::parsesEntriesAndFlags()
{
    const QString output = showLine("hash-one", "hello", QStringLiteral("alice"))
                           + QLatin1Char('\n') + showLine("hash-two", "") + QLatin1Char('\n')
                           + showLine("hash-three", "$HCENC$1$key$cipher");

    const PotfileShowParser::Result result = PotfileShowParser::parse(output);
    QCOMPARE(result.totalCount, 3);
    QCOMPARE(result.malformedCount, 0);
    QCOMPARE(result.entries.size(), 3);

    QCOMPARE(result.entries.at(0).hash, QStringLiteral("hash-one"));
    QCOMPARE(result.entries.at(0).plain, QStringLiteral("hello"));
    QCOMPARE(result.entries.at(0).username, QStringLiteral("alice"));
    QVERIFY(!result.entries.at(0).emptyPlain);
    QVERIFY(!result.entries.at(0).encrypted);
    QVERIFY(!result.entries.at(0).hashIsFile);

    QCOMPARE(result.entries.at(1).plain, QString());
    QVERIFY(result.entries.at(1).emptyPlain);
    QVERIFY(!result.entries.at(1).encrypted);

    QVERIFY(result.entries.at(2).encrypted);
    QVERIFY(!result.entries.at(2).emptyPlain);
}

void TestPotfileShowParser::decodesHexEscapedPlaintext()
{
    const QString output =
        showLine("hash", "$HEX[613a62]") + QLatin1Char('\n') + showLine("hash-two", "$HEX[c3a9]");
    const PotfileShowParser::Result result = PotfileShowParser::parse(output);

    QCOMPARE(result.entries.size(), 2);
    QCOMPARE(result.entries.at(0).plain, QStringLiteral("a:b"));
    QVERIFY(!result.entries.at(0).emptyPlain);
    QCOMPARE(result.entries.at(1).plain, QString::fromUtf8("\xC3\xA9"));
}

void TestPotfileShowParser::keepsNonUtf8HexEscapeLiteral()
{
    const QString output = showLine("hash", "$HEX[ff]");
    const PotfileShowParser::Result result = PotfileShowParser::parse(output);

    QCOMPARE(result.entries.size(), 1);
    QCOMPARE(result.entries.at(0).plain, QStringLiteral("$HEX[ff]"));
}

void TestPotfileShowParser::keepsAnInvalidHexEscapeLiteral()
{
    const QString output = showLine("hash", "$HEX[zz]");
    const PotfileShowParser::Result result = PotfileShowParser::parse(output);

    QCOMPARE(result.entries.size(), 1);
    QCOMPARE(result.entries.at(0).plain, QStringLiteral("$HEX[zz]"));
}

void TestPotfileShowParser::parsesBinaryHashFileEntries()
{
    const QString output = binaryHashFileShowLine("hashes.hc22000", "secret");
    const PotfileShowParser::Result result = PotfileShowParser::parse(output);

    QCOMPARE(result.totalCount, 1);
    QCOMPARE(result.malformedCount, 0);
    QCOMPARE(result.entries.size(), 1);
    QCOMPARE(result.entries.at(0).hash, QStringLiteral("hashes.hc22000"));
    QVERIFY(result.entries.at(0).hashIsFile);
    QCOMPARE(result.entries.at(0).plain, QStringLiteral("secret"));
}

void TestPotfileShowParser::capsRetainedEntriesButKeepsTheTotal()
{
    const QString output = showLine("one", "a") + QLatin1Char('\n') + showLine("two", "b")
                           + QLatin1Char('\n') + showLine("three", "c");

    const PotfileShowParser::Result result = PotfileShowParser::parse(output, 2);
    QCOMPARE(result.totalCount, 3);
    QCOMPARE(result.entries.size(), 2);
    QCOMPARE(result.entries.at(0).hash, QStringLiteral("one"));
    QCOMPARE(result.entries.at(1).hash, QStringLiteral("two"));
}

void TestPotfileShowParser::emptyOutputIsAValidEmptyAnswer()
{
    const PotfileShowParser::Result result = PotfileShowParser::parse(QString());
    QCOMPARE(result.totalCount, 0);
    QCOMPARE(result.malformedCount, 0);
    QVERIFY(result.entries.isEmpty());
}

void TestPotfileShowParser::malformedLinesAreCounted()
{
    const QString output = QStringLiteral("not json\n{}\n") + showLine("hash", "plain")
                           + QLatin1Char('\n') + QStringLiteral("\n   \n");

    const PotfileShowParser::Result result = PotfileShowParser::parse(output);
    QCOMPARE(result.totalCount, 1);
    QCOMPARE(result.malformedCount, 2);
    QCOMPARE(result.entries.size(), 1);
}

void TestPotfileShowParser::autoDetectPreambleIsSkipped()
{
    const QString output =
        QStringLiteral("Hash-mode was not specified with -m. Attempting to auto-detect hash "
                       "mode.\n"
                       "The following mode was auto-detected as the only one matching your input "
                       "hash:\n\n"
                       "      0 | MD5 | Raw Hash\n\n"
                       "NOTE: Auto-detect is best effort. The correct hash-mode is NOT "
                       "guaranteed!\n"
                       "Do NOT report auto-detect issues unless you are certain of the hash "
                       "type.\n\n")
        + showLine("hash", "plain");

    const PotfileShowParser::Result result = PotfileShowParser::parse(output);
    QCOMPARE(result.totalCount, 1);
    QCOMPARE(result.malformedCount, 0);
}

// The legacy block contains bare path lines: it must be skipped whole, not
// line by line.
void TestPotfileShowParser::legacyDirectoryBlockIsSkippedWhole()
{
    const QString output =
        QStringLiteral("Found /home/user/.hashcat, which hashcat no longer uses.\n"
                       "It is left untouched. Everything hashcat keeps per user is now in\n"
                       "  /home/user/.local/share/hashcat\n"
                       "and the files it rebuilds by itself, the compiled kernels among them, "
                       "are under\n"
                       "  /home/user/.cache/hashcat\n"
                       "That is potfiles and sessions, and anything else you were asked to put "
                       "there.\n"
                       "Move what you still want across, then delete the old directory.\n\n")
        + showLine("hash", "plain");

    const PotfileShowParser::Result result = PotfileShowParser::parse(output);
    QCOMPARE(result.totalCount, 1);
    QCOMPARE(result.malformedCount, 0);
}

void TestPotfileShowParser::oldPotfileBlockIsSkipped()
{
    const QString output = QStringLiteral("Old potfile detected: /home/user/.local/share/hashcat/"
                                          "hashcat.pot\n"
                                          "New potfile is: /home/user/.local/share/hashcat/"
                                          "hashcat.potfile \n\n")
                           + showLine("hash", "plain");

    const PotfileShowParser::Result result = PotfileShowParser::parse(output);
    QCOMPARE(result.totalCount, 1);
    QCOMPARE(result.malformedCount, 0);
}

void TestPotfileShowParser::repeatedHashfileWarningsAreSkipped()
{
    const QString output = QStringLiteral("Hashfile 'hashes.txt': Token length exception\n"
                                          "Hashfile 'hashes.txt': Token length exception\n")
                           + showLine("hash", "plain");

    const PotfileShowParser::Result result = PotfileShowParser::parse(output);
    QCOMPARE(result.totalCount, 1);
    QCOMPARE(result.malformedCount, 0);
}

void TestPotfileShowParser::anUnknownLeadingLineIsStillMalformed()
{
    const QString output = QStringLiteral("something unexpected\n") + showLine("hash", "plain");

    const PotfileShowParser::Result result = PotfileShowParser::parse(output);
    QCOMPARE(result.totalCount, 1);
    QCOMPARE(result.malformedCount, 1);
}

void TestPotfileShowParser::diagnosticsOnlyOutputHasNoEntries()
{
    const QString output = QStringLiteral("Hashfile 'hashes.txt': Token length exception\n");

    const PotfileShowParser::Result result = PotfileShowParser::parse(output);
    QCOMPARE(result.totalCount, 0);
    QCOMPARE(result.malformedCount, 0);
    QVERIFY(result.entries.isEmpty());
}

// A final line without a trailing newline must not push the payload offset
// past the end of the output.
void TestPotfileShowParser::diagnosticsWithoutTrailingNewlineStayInRange()
{
    const PotfileShowParser::Result warning =
        PotfileShowParser::parse(QStringLiteral("Hashfile 'hashes.txt': Token length exception"));
    QCOMPARE(warning.totalCount, 0);
    QCOMPARE(warning.malformedCount, 0);

    const QString block = QStringLiteral("Old potfile detected: /x/hashcat.pot\n"
                                         "New potfile is: /x/hashcat.potfile");
    const PotfileShowParser::Result blockOnly = PotfileShowParser::parse(block);
    QCOMPARE(blockOnly.totalCount, 0);
    QCOMPARE(blockOnly.malformedCount, 0);

    const PotfileShowParser::Result withPayload =
        PotfileShowParser::parse(block + QLatin1Char('\n') + showLine("hash", "plain"));
    QCOMPARE(withPayload.totalCount, 1);
    QCOMPARE(withPayload.malformedCount, 0);
}

// Only leading diagnostics are skipped; noise inside the payload stays visible.
void TestPotfileShowParser::noiseAfterThePayloadIsStillMalformed()
{
    const QString output = showLine("hash", "plain") + QLatin1Char('\n')
                           + QStringLiteral("Hashfile 'hashes.txt': late warning\n");

    const PotfileShowParser::Result result = PotfileShowParser::parse(output);
    QCOMPARE(result.totalCount, 1);
    QCOMPARE(result.malformedCount, 1);
}

void TestPotfileShowParser::aCancelledTokenStopsTheScan()
{
    const auto cancel = std::make_shared<std::atomic_bool>(true);
    const QString output = showLine("hash", "plain");

    const PotfileShowParser::Result result =
        PotfileShowParser::parse(output, AppConstants::Hashcat::PotfileMaxEntries, cancel);

    QVERIFY(result.cancelled);
    QCOMPARE(result.totalCount, 0);
    QCOMPARE(result.entries.size(), 0);
}

int main(int argc, char *argv[])
{
    TestPotfileShowParser test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_potfileshowparser.moc"