/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfilestatistics.h"
#include "potfiletesthelpers.h"
#include "testenvironment.h"

#include <QTest>

class TestPotfileStatistics : public QObject
{
    Q_OBJECT

private slots:
    void countsAndClasses();
    void countsUnicodeCodePoints();
    void noRowsIsAnEmptyResult();
    void emptyAndEncryptedEntriesAreCountedButNotAnalyzed();
};

void TestPotfileStatistics::countsAndClasses()
{
    const PotfileStatistics::Result result = PotfileStatistics::calculate(mixedRows());

    QCOMPARE(result.entries, 6);
    QCOMPARE(result.analyzed, 4);
    QCOMPARE(result.empty, 1);
    QCOMPARE(result.encrypted, 1);
    QCOMPARE(result.duplicate, 1);
    QCOMPARE(result.unique, 3);
    QCOMPARE(result.minLength, 3);
    QCOMPARE(result.maxLength, 7);
    QCOMPARE(result.averageLength, 5.0);
    QCOMPARE(result.lengthHistogram.value(3), 2);
    QCOMPARE(result.lengthHistogram.value(7), 2);
    QCOMPARE(result.lengthHistogram.size(), 2);

    QCOMPARE(result.classes.lowercase, 3);
    QCOMPARE(result.classes.uppercase, 1);
    QCOMPARE(result.classes.digits, 1);
    QCOMPARE(result.classes.symbols, 1);
    QCOMPARE(result.classes.whitespace, 1);
    QCOMPARE(result.classes.nonAscii, 1);
}

void TestPotfileStatistics::countsUnicodeCodePoints()
{
    const QString emoji = QString::fromUtf8("\xF0\x9F\x98\x80");
    QCOMPARE(emoji.size(), 2);

    const QList<PotfileExporter::Row> rows{
        {QStringLiteral("hash"), emoji, {}, 0, false, false},
    };
    const PotfileStatistics::Result result = PotfileStatistics::calculate(rows);
    QCOMPARE(result.analyzed, 1);
    QCOMPARE(result.minLength, 1);
    QCOMPARE(result.maxLength, 1);
    QCOMPARE(result.averageLength, 1.0);
    QCOMPARE(result.lengthHistogram.value(1), 1);
    QVERIFY(!result.lengthHistogram.contains(2));
    QCOMPARE(result.classes.symbols, 1);
    QCOMPARE(result.classes.nonAscii, 1);
}

void TestPotfileStatistics::noRowsIsAnEmptyResult()
{
    const PotfileStatistics::Result result = PotfileStatistics::calculate({});
    QCOMPARE(result.entries, 0);
    QCOMPARE(result.analyzed, 0);
    QCOMPARE(result.minLength, 0);
    QCOMPARE(result.maxLength, 0);
    QCOMPARE(result.averageLength, 0.0);
    QVERIFY(result.lengthHistogram.isEmpty());
}

void TestPotfileStatistics::emptyAndEncryptedEntriesAreCountedButNotAnalyzed()
{
    const QList<PotfileExporter::Row> rows{
        {QStringLiteral("hash-one"), QString(), {}, 0, false, false},
        {QStringLiteral("hash-two"), QStringLiteral("$HCENC$1$key$cipher"), {}, 0, false, true},
    };

    const PotfileStatistics::Result result = PotfileStatistics::calculate(rows);
    QCOMPARE(result.entries, 2);
    QCOMPARE(result.analyzed, 0);
    QCOMPARE(result.empty, 1);
    QCOMPARE(result.encrypted, 1);
    QCOMPARE(result.unique, 0);
    QCOMPARE(result.minLength, 0);
    QCOMPARE(result.maxLength, 0);
    QCOMPARE(result.averageLength, 0.0);
    QVERIFY(result.lengthHistogram.isEmpty());
}

int main(int argc, char *argv[])
{
    TestPotfileStatistics test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_potfilestatistics.moc"