/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfilestatistics.h"
#include "potfilestatisticsdialog.h"
#include "potfiletesthelpers.h"
#include "testenvironment.h"

#include <QApplication>
#include <QHeaderView>
#include <QLabel>
#include <QTest>
#include <QTimer>
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
    void unknownDuplicatesAreSaidToBeUnknown();
    void oneDuplicateLineIsSingular();
    void emptyHistogramIsHidden();
    void asyncCalculationMatchesTheDirectResult();
    void closingTheParentDiscardsTheAsyncResult();
};

void TestPotfileStatisticsDialog::showsCountsClassesAndHistogram()
{
    const PotfileStatistics::Result statistics = PotfileStatistics::calculate(mixedRows());
    PotfileStatisticsDialog dialog(nullptr, statistics, QStringLiteral("Cracked hashes — MD5"), 10,
                                   /*duplicateKnown=*/true);

    QCOMPARE(label(dialog, QStringLiteral("label_context"))->text(),
             QStringLiteral("Cracked hashes — MD5"));
    QCOMPARE(label(dialog, QStringLiteral("label_context"))->textFormat(), Qt::PlainText);
    const QString summary = label(dialog, QStringLiteral("label_summary"))->text();
    QVERIFY(summary.contains(QStringLiteral("6 entries")));
    QVERIFY(summary.contains(QStringLiteral("4 passwords analyzed")));
    QVERIFY(summary.contains(QStringLiteral("1 empty")));
    QVERIFY(summary.contains(QStringLiteral("1 encrypted")));
    QVERIFY(summary.contains(QStringLiteral("1 duplicate line")));
    QVERIFY(summary.contains(QStringLiteral("3 unique plaintexts")));
    QVERIFY(summary.contains(QStringLiteral("min 3")));
    QVERIFY(summary.contains(QStringLiteral("max 7")));
    QVERIFY(summary.contains(QStringLiteral("average 5.0")));
    QVERIFY(summary.contains(QStringLiteral("visible 6 of 10")));

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
    const PotfileStatistics::Result statistics = PotfileStatistics::calculate(noPlaintextRows());
    PotfileStatisticsDialog dialog(nullptr, statistics, QStringLiteral("Potfile"), 2,
                                   /*duplicateKnown=*/true);

    QVERIFY(label(dialog, QStringLiteral("label_classes"))
                ->text()
                .contains(QStringLiteral("No plaintext")));
    QVERIFY(label(dialog, QStringLiteral("label_summary"))
                ->text()
                .contains(QStringLiteral("Length: —")));
    QCOMPARE(tree(dialog)->topLevelItemCount(), 0);
}

void TestPotfileStatisticsDialog::unknownDuplicatesAreSaidToBeUnknown()
{
    const PotfileStatistics::Result statistics = PotfileStatistics::calculate(mixedRows());
    PotfileStatisticsDialog dialog(nullptr, statistics, QStringLiteral("Cracked hashes"), 6,
                                   /*duplicateKnown=*/false);

    QVERIFY(label(dialog, QStringLiteral("label_summary"))
                ->text()
                .contains(QStringLiteral("duplicate lines: unknown")));
}

void TestPotfileStatisticsDialog::oneDuplicateLineIsSingular()
{
    QList<PotfileExporter::Row> rows = mixedRows();
    rows[4].duplicate = false;
    rows[0].duplicate = true;
    const PotfileStatistics::Result statistics = PotfileStatistics::calculate(rows);
    PotfileStatisticsDialog dialog(nullptr, statistics, QString(), 6, /*duplicateKnown=*/true);

    const QString summary = label(dialog, QStringLiteral("label_summary"))->text();
    QVERIFY(summary.contains(QStringLiteral("1 duplicate line")));
    QVERIFY(!summary.contains(QStringLiteral("1 duplicate lines")));
}

void TestPotfileStatisticsDialog::emptyHistogramIsHidden()
{
    const PotfileStatistics::Result statistics = PotfileStatistics::calculate(noPlaintextRows());
    PotfileStatisticsDialog dialog(nullptr, statistics, QString(), 2, /*duplicateKnown=*/true);

    QVERIFY(tree(dialog)->isHidden());
    QVERIFY(!tree(dialog)->header()->stretchLastSection());

    const PotfileStatistics::Result full = PotfileStatistics::calculate(mixedRows());
    PotfileStatisticsDialog withData(nullptr, full, QString(), 6, /*duplicateKnown=*/true);
    QVERIFY(!tree(withData)->isHidden());
}

void TestPotfileStatisticsDialog::asyncCalculationMatchesTheDirectResult()
{
    const QList<PotfileExporter::Row> rows = mixedRows();
    const PotfileStatistics::Result direct = PotfileStatistics::calculate(rows);
    PotfileStatisticsDialog reference(nullptr, direct, QStringLiteral("ctx"), 10,
                                      /*duplicateKnown=*/true);
    const QString expected = label(reference, QStringLiteral("label_summary"))->text();

    QWidget parent;
    QString summary;
    QTimer timer;
    timer.setInterval(20);
    QObject::connect(&timer, &QTimer::timeout, &timer, [&] {
        if (auto *stats =
                qobject_cast<PotfileStatisticsDialog *>(QApplication::activeModalWidget())) {
            summary = label(*stats, QStringLiteral("label_summary"))->text();
            stats->close();
        }
    });
    timer.start();
    PotfileStatisticsDialog::openAsync(&parent, rows, QStringLiteral("ctx"), 10,
                                       /*duplicateKnown=*/true);
    QTRY_VERIFY_WITH_TIMEOUT(!summary.isEmpty(), 5000);
    timer.stop();
    QCOMPARE(summary, expected);
}

void TestPotfileStatisticsDialog::closingTheParentDiscardsTheAsyncResult()
{
    auto *parent = new QWidget;
    PotfileStatisticsDialog::openAsync(parent, mixedRows(), QString(), 6, /*duplicateKnown=*/true);
    delete parent;

    QTest::qWait(300);
    QVERIFY(QApplication::activeModalWidget() == nullptr);
}

int main(int argc, char *argv[])
{
    TestPotfileStatisticsDialog test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_potfilestatisticsdialog.moc"