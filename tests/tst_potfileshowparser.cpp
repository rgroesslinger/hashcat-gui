/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfileshowparser.h"
#include "potfiletesthelpers.h"
#include "testenvironment.h"

#include <QTest>

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

int main(int argc, char *argv[])
{
    TestPotfileShowParser test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_potfileshowparser.moc"