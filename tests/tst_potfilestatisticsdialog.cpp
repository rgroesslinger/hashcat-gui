/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfilestatistics.h"
#include "potfilestatisticsdialog.h"
#include "potfiletesthelpers.h"
#include "testenvironment.h"

#include <QLabel>
#include <QTest>
#include <QTreeWidget>

namespace
{

QLabel *label(PotfileStatisticsDialog &dialog, const QString &name)
{
    return dialog.findChild<QLabel *>(name);
}

QTreeWidget *tree(PotfileStatisticsDialog &dialog)
{
    return dialog.findChild<QTreeWidget *>(QStringLiteral("treeWidget_lengths"));
}

} // namespace

class TestPotfileStatisticsDialog : public QObject
{
    Q_OBJECT

private slots:
    void showsCountsClassesAndHistogram();
    void explainsWhenThereIsNothingToAnalyze();
};

void TestPotfileStatisticsDialog::showsCountsClassesAndHistogram()
{
    const PotfileStatistics::Result statistics = PotfileStatistics::calculate(mixedRows());
    PotfileStatisticsDialog dialog(nullptr, statistics, QStringLiteral("Cracked hashes — MD5"), 10);

    QCOMPARE(label(dialog, QStringLiteral("label_context"))->text(),
             QStringLiteral("Cracked hashes — MD5"));
    QCOMPARE(label(dialog, QStringLiteral("label_context"))->textFormat(), Qt::PlainText);
    const QString summary = label(dialog, QStringLiteral("label_summary"))->text();
    QVERIFY(summary.contains(QStringLiteral("6 entries")));
    QVERIFY(summary.contains(QStringLiteral("4 passwords analyzed")));
    QVERIFY(summary.contains(QStringLiteral("1 empty")));
    QVERIFY(summary.contains(QStringLiteral("1 encrypted")));
    QVERIFY(summary.contains(QStringLiteral("1 duplicate")));
    QVERIFY(summary.contains(QStringLiteral("3 unique plaintexts")));
    QVERIFY(summary.contains(QStringLiteral("min 3")));
    QVERIFY(summary.contains(QStringLiteral("max 7")));
    QVERIFY(summary.contains(QStringLiteral("average 5.0")));
    QVERIFY(summary.contains(QStringLiteral("retained 6 of 10")));

    const QString classes = label(dialog, QStringLiteral("label_classes"))->text();
    QVERIFY(classes.contains(QStringLiteral("lowercase: 3 (75%)")));
    QVERIFY(classes.contains(QStringLiteral("non-ASCII: 1 (25%)")));

    QCOMPARE(tree(dialog)->topLevelItemCount(), 2);
    QCOMPARE(tree(dialog)->topLevelItem(0)->text(0), QStringLiteral("3"));
    QCOMPARE(tree(dialog)->topLevelItem(0)->text(1), QStringLiteral("2"));
    QCOMPARE(tree(dialog)->topLevelItem(1)->text(0), QStringLiteral("7"));
    QCOMPARE(tree(dialog)->topLevelItem(1)->text(1), QStringLiteral("2"));
}

void TestPotfileStatisticsDialog::explainsWhenThereIsNothingToAnalyze()
{
    const QList<PotfileExporter::Row> rows{
        {QStringLiteral("hash-one"), QString(), {}, 0, false, false},
        {QStringLiteral("hash-two"), QStringLiteral("$HCENC$1$key$cipher"), {}, 0, false, true},
    };
    const PotfileStatistics::Result statistics = PotfileStatistics::calculate(rows);
    PotfileStatisticsDialog dialog(nullptr, statistics, QStringLiteral("Potfile"), 2);

    QVERIFY(label(dialog, QStringLiteral("label_classes"))
                ->text()
                .contains(QStringLiteral("No plaintext")));
    QCOMPARE(tree(dialog)->topLevelItemCount(), 0);
}

int main(int argc, char *argv[])
{
    TestPotfileStatisticsDialog test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_potfilestatisticsdialog.moc"