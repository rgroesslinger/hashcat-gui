/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfilecheck.h"
#include "potfileresultsdialog.h"
#include "potfilestatisticsdialog.h"
#include "potfiletesthelpers.h"
#include "testenvironment.h"

#include <QApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedLayout>
#include <QTemporaryDir>
#include <QTest>
#include <QTimer>
#include <QTreeWidget>

using namespace PotfileDialogTestAccess;

namespace
{
// Picks the file, clicks Export on the confirmation and closes the info box.
void driveExport(PotfileResultsDialog &dialog, const QString &path)
{
    QTimer timer;
    timer.setInterval(20);
    QObject::connect(&timer, &QTimer::timeout, &timer, [&] {
        QWidget *modal = QApplication::activeModalWidget();
        if (!modal) {
            return;
        }
        if (auto *fileDialog = qobject_cast<QFileDialog *>(modal)) {
            if (QWidget *focused = fileDialog->focusWidget()) {
                focused->clearFocus();
            }
            if (!fileDialog->selectedFiles().contains(path)) {
                fileDialog->selectFile(path);
                return;
            }
            static_cast<QDialog *>(fileDialog)->accept();
            return;
        }
        if (auto *box = qobject_cast<QMessageBox *>(modal)) {
            const auto buttons = box->buttons();
            for (QAbstractButton *b : buttons) {
                if (box->buttonRole(b) == QMessageBox::AcceptRole) {
                    b->click();
                    return;
                }
            }
            box->close();
        }
    });
    timer.start();
    button(dialog, QStringLiteral("pushButton_export"))->click();
    timer.stop();
}
} // namespace

class TestPotfileResultsDialog : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void showsMaskedRowsAndTheQueriedMode();
    void noteNamesThePotfileAndTheTitleTheMode();
    void headerSizingUsesTheSharedDefaults();
    void usernameColumnFollowsTheEntries();
    void revealShowsRealPlaintexts();
    void copyIsDisabledWhileMasked();
    void revealedCopyIsTabSeparated();
    void copyAllUsesOnlyVisibleRows();
    void searchMatchesHashAndUsernameWhileMasked();
    void encryptedEntriesStayMaskedAndCopyTheToken();
    void binaryHashFileEntriesAreLabeled();
    void leftUnknownUsesSetResult();
    void leftUnknownAppendsTheDiagnostic();
    void malformedLinesAreReportedInTheStatus();
    void showFailedUsesSetResult();
    void zeroCrackedShowsTheEmptyStateHint();
    void filterHidesRowsFromExportAndStatistics();
    void totalsUseLocaleGrouping();
};

void TestPotfileResultsDialog::init()
{
    TestEnvironment::resetSettings();
    QApplication::clipboard()->clear();
}

void TestPotfileResultsDialog::showsMaskedRowsAndTheQueriedMode()
{
    const PotfileCheckResult result = okCheckResult(
        {
            {QStringLiteral("hash-one"), QStringLiteral("secret"), QString(), false, false},
            {QStringLiteral("hash-two"), QStringLiteral("hunter2"), QStringLiteral("alice"), false,
             false},
        },
        1);

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("MD5"));

    QVERIFY(dialog.windowTitle().contains(QStringLiteral("MD5")));
    QVERIFY(
        label(dialog, QStringLiteral("label_status"))->text().contains(QStringLiteral("2 of 3")));
    QCOMPARE(label(dialog, QStringLiteral("label_status"))->textFormat(), Qt::PlainText);
    QCOMPARE(tree(dialog)->topLevelItemCount(), 2);
    // Export would write the real plaintexts while the table still masks them.
    QVERIFY(!button(dialog, QStringLiteral("pushButton_export"))->isEnabled());
    QVERIFY(button(dialog, QStringLiteral("pushButton_export"))
                ->toolTip()
                .contains(QStringLiteral("Reveal")));
    QVERIFY(button(dialog, QStringLiteral("pushButton_statistics"))->isEnabled());
    QVERIFY(tree(dialog)->topLevelItem(0)->text(1) != QStringLiteral("secret"));
    QVERIFY(tree(dialog)->topLevelItem(0)->text(1).contains(QStringLiteral("•")));
    QVERIFY(!tree(dialog)->isColumnHidden(2));
    QVERIFY(stack(dialog)->currentWidget() != label(dialog, QStringLiteral("label_empty_state")));
}

void TestPotfileResultsDialog::noteNamesThePotfileAndTheTitleTheMode()
{
    const PotfileCheckResult result = okCheckResult({
        {QStringLiteral("hash-one"), QStringLiteral("secret"), QString(), false, false},
    });

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("MD5"), QStringLiteral("/tmp/my.potfile"));
    const QString note = label(dialog, QStringLiteral("label_note"))->text();
    QVERIFY(note.contains(QStringLiteral("Potfile: /tmp/my.potfile")));
    QVERIFY(note.contains(QStringLiteral("narrows the search")));
    QVERIFY(!note.contains(QStringLiteral("-m is advisory")));
    QCOMPARE(dialog.windowTitle(), QStringLiteral("Cracked hashes — mode MD5"));

    dialog.setResult(result, QString());
    QVERIFY(label(dialog, QStringLiteral("label_note"))
                ->text()
                .contains(QStringLiteral("hashcat's default potfile")));
    QCOMPARE(dialog.windowTitle(), QStringLiteral("Cracked hashes"));
}

void TestPotfileResultsDialog::headerSizingUsesTheSharedDefaults()
{
    PotfileResultsDialog dialog;
    QHeaderView *header = tree(dialog)->header();

    QCOMPARE(header->sectionResizeMode(0), QHeaderView::Interactive);
    QCOMPARE(header->sectionSize(0), 320);
    QCOMPARE(header->sectionResizeMode(1), QHeaderView::Interactive);
    QCOMPARE(header->sectionSize(1), 360);
    QCOMPARE(header->sectionResizeMode(2), QHeaderView::Interactive);
}

void TestPotfileResultsDialog::usernameColumnFollowsTheEntries()
{
    const PotfileCheckResult result = okCheckResult({
        {QStringLiteral("hash-one"), QStringLiteral("secret"), QString(), false, false},
    });

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("MD5"));
    QVERIFY(tree(dialog)->isColumnHidden(2));
}

void TestPotfileResultsDialog::revealShowsRealPlaintexts()
{
    const PotfileCheckResult result = okCheckResult({
        {QStringLiteral("hash-one"), QStringLiteral("secret"), QString(), false, false},
    });

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("MD5"));

    button(dialog, QStringLiteral("pushButton_reveal"))->click();
    QCOMPARE(tree(dialog)->topLevelItem(0)->text(1), QStringLiteral("secret"));
    button(dialog, QStringLiteral("pushButton_reveal"))->click();
    QVERIFY(tree(dialog)->topLevelItem(0)->text(1) != QStringLiteral("secret"));
}

// Masked plaintexts must not leak through the clipboard.
void TestPotfileResultsDialog::copyIsDisabledWhileMasked()
{
    const PotfileCheckResult result = okCheckResult({
        {QStringLiteral("hash-one"), QStringLiteral("secret"), QString(), false, false},
    });

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("MD5"));

    QVERIFY(!button(dialog, QStringLiteral("pushButton_copy_all"))->isEnabled());
    tree(dialog)->topLevelItem(0)->setSelected(true);
    QVERIFY(!button(dialog, QStringLiteral("pushButton_copy_selected"))->isEnabled());
    QVERIFY(button(dialog, QStringLiteral("pushButton_copy_all"))
                ->toolTip()
                .contains(QStringLiteral("Reveal plaintexts")));

    button(dialog, QStringLiteral("pushButton_reveal"))->click();
    QVERIFY(button(dialog, QStringLiteral("pushButton_copy_all"))->isEnabled());
    QCOMPARE(button(dialog, QStringLiteral("pushButton_reveal"))->text(),
             QStringLiteral("Hide plaintexts"));
}

void TestPotfileResultsDialog::revealedCopyIsTabSeparated()
{
    const PotfileCheckResult result = okCheckResult({
        {QStringLiteral("hash-one"), QStringLiteral("secret"), QStringLiteral("alice"), false,
         false},
        {QStringLiteral("hash-two"), QStringLiteral("hunter2"), QString(), false, false},
    });

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("MD5"));
    button(dialog, QStringLiteral("pushButton_reveal"))->click();

    tree(dialog)->topLevelItem(0)->setSelected(true);
    button(dialog, QStringLiteral("pushButton_copy_selected"))->click();
    QCOMPARE(QApplication::clipboard()->text(), QStringLiteral("hash-one\tsecret\talice"));

    QApplication::clipboard()->clear();
    button(dialog, QStringLiteral("pushButton_copy_all"))->click();
    QCOMPARE(QApplication::clipboard()->text(),
             QStringLiteral("hash-one\tsecret\talice\nhash-two\thunter2\t"));
}

void TestPotfileResultsDialog::copyAllUsesOnlyVisibleRows()
{
    const PotfileCheckResult result = okCheckResult({
        {QStringLiteral("hash-one"), QStringLiteral("secret"), QString(), false, false},
        {QStringLiteral("hash-two"), QStringLiteral("hunter2"), QString(), false, false},
    });

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("MD5"));
    button(dialog, QStringLiteral("pushButton_reveal"))->click();

    search(dialog)->setText(QStringLiteral("hash-two"));
    QVERIFY(tree(dialog)->topLevelItem(0)->isHidden());
    QVERIFY(!tree(dialog)->topLevelItem(1)->isHidden());

    button(dialog, QStringLiteral("pushButton_copy_all"))->click();
    QCOMPARE(QApplication::clipboard()->text(), QStringLiteral("hash-two\thunter2\t"));
}

void TestPotfileResultsDialog::searchMatchesHashAndUsernameWhileMasked()
{
    const PotfileCheckResult result = okCheckResult({
        {QStringLiteral("hash-one"), QStringLiteral("secret"), QStringLiteral("alice"), false,
         false},
        {QStringLiteral("hash-two"), QStringLiteral("hunter2"), QStringLiteral("bob"), false,
         false},
    });

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("MD5"));

    search(dialog)->setText(QStringLiteral("secret"));
    QVERIFY(tree(dialog)->topLevelItem(0)->isHidden());
    QVERIFY(tree(dialog)->topLevelItem(1)->isHidden());

    search(dialog)->setText(QStringLiteral("alice"));
    QVERIFY(!tree(dialog)->topLevelItem(0)->isHidden());
    QVERIFY(tree(dialog)->topLevelItem(1)->isHidden());

    search(dialog)->setText(QStringLiteral("hash-two"));
    QVERIFY(tree(dialog)->topLevelItem(0)->isHidden());
    QVERIFY(!tree(dialog)->topLevelItem(1)->isHidden());

    button(dialog, QStringLiteral("pushButton_reveal"))->click();
    search(dialog)->setText(QStringLiteral("secret"));
    QVERIFY(!tree(dialog)->topLevelItem(0)->isHidden());
    QVERIFY(tree(dialog)->topLevelItem(1)->isHidden());
}

void TestPotfileResultsDialog::encryptedEntriesStayMaskedAndCopyTheToken()
{
    const PotfileCheckResult result = okCheckResult({
        {QStringLiteral("hash-one"), QStringLiteral("$HCENC$1$key$cipher"), QString(), false, true},
    });

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("MD5"));

    QCOMPARE(tree(dialog)->topLevelItem(0)->text(1), QStringLiteral("(encrypted)"));
    // Nothing revealable: the token may be copied without Reveal.
    QVERIFY(!button(dialog, QStringLiteral("pushButton_reveal"))->isEnabled());

    tree(dialog)->topLevelItem(0)->setSelected(true);
    QVERIFY(button(dialog, QStringLiteral("pushButton_copy_selected"))->isEnabled());
    button(dialog, QStringLiteral("pushButton_copy_selected"))->click();
    QCOMPARE(QApplication::clipboard()->text(), QStringLiteral("hash-one\t$HCENC$1$key$cipher\t"));
    QVERIFY(label(dialog, QStringLiteral("label_status"))
                ->text()
                .contains(QStringLiteral("encrypted")));
}

void TestPotfileResultsDialog::binaryHashFileEntriesAreLabeled()
{
    const PotfileCheckResult result = okCheckResult({
        {QStringLiteral("hashes.hc22000"), QStringLiteral("secret"), QString(), false, false, true},
    });

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("2500"));

    QVERIFY(label(dialog, QStringLiteral("label_status"))
                ->text()
                .contains(QStringLiteral("binary hash file")));
    QCOMPARE(tree(dialog)->topLevelItemCount(), 1);
    QCOMPARE(tree(dialog)->topLevelItem(0)->text(0), QStringLiteral("(file) hashes.hc22000"));
    QVERIFY(tree(dialog)->topLevelItem(0)->toolTip(0).contains(QStringLiteral("Binary hash file")));
}

void TestPotfileResultsDialog::leftUnknownUsesSetResult()
{
    PotfileCheckResult result;
    result.status = PotfileCheckResult::Status::LeftUnknown;
    result.cracked = 1;
    result.left = -1;
    result.totalCount = 1;
    result.entries = {
        {QStringLiteral("hash-one"), QStringLiteral("secret"), QString(), false, false},
    };

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("MD5"));

    QVERIFY(label(dialog, QStringLiteral("label_status"))
                ->text()
                .contains(QStringLiteral("remaining unknown")));
    QCOMPARE(tree(dialog)->topLevelItemCount(), 1);
}

void TestPotfileResultsDialog::showFailedUsesSetResult()
{
    PotfileCheckResult result;
    result.status = PotfileCheckResult::Status::ShowFailed;
    result.diagnostic = QStringLiteral("parse error");

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("MD5"));

    QCOMPARE(label(dialog, QStringLiteral("label_status"))->text(), QStringLiteral("parse error"));
    QCOMPARE(tree(dialog)->topLevelItemCount(), 0);
    QVERIFY(stack(dialog)->currentWidget() != label(dialog, QStringLiteral("label_empty_state")));
}

void TestPotfileResultsDialog::zeroCrackedShowsTheEmptyStateHint()
{
    const PotfileCheckResult result = okCheckResult({}, 2);

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("MD5"));

    QLabel *hint = label(dialog, QStringLiteral("label_empty_state"));
    QVERIFY(
        label(dialog, QStringLiteral("label_status"))->text().contains(QStringLiteral("0 of 2")));
    QCOMPARE(tree(dialog)->topLevelItemCount(), 0);
    QCOMPARE(stack(dialog)->currentWidget(), static_cast<QWidget *>(hint));
    QVERIFY(hint->text().contains(QStringLiteral("No cracked hashes")));
}

void TestPotfileResultsDialog::leftUnknownAppendsTheDiagnostic()
{
    PotfileCheckResult result;
    result.status = PotfileCheckResult::Status::LeftUnknown;
    result.cracked = 1;
    result.left = -1;
    result.totalCount = 1;
    result.diagnostic = QStringLiteral("left exploded");
    result.entries = {
        {QStringLiteral("hash-one"), QStringLiteral("secret"), QString(), false, false},
    };

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("MD5"));

    const QString text = label(dialog, QStringLiteral("label_status"))->text();
    QVERIFY(text.contains(QStringLiteral("remaining unknown")));
    QVERIFY(text.contains(QStringLiteral("left exploded")));
}

void TestPotfileResultsDialog::malformedLinesAreReportedInTheStatus()
{
    PotfileCheckResult result = okCheckResult({
        {QStringLiteral("hash-one"), QStringLiteral("secret"), QString(), false, false},
    });
    result.malformedCount = 1;

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("MD5"));

    QVERIFY(label(dialog, QStringLiteral("label_status"))
                ->text()
                .contains(QStringLiteral("1 unreadable result line skipped.")));
}

void TestPotfileResultsDialog::filterHidesRowsFromExportAndStatistics()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const PotfileCheckResult result = okCheckResult({
        {QStringLiteral("hash-one"), QStringLiteral("secret"), QString(), false, false},
        {QStringLiteral("hash-two"), QStringLiteral("hunter2"), QString(), false, false},
    });

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("MD5"));
    search(dialog)->setText(QStringLiteral("hash-two"));

    QVERIFY(label(dialog, QStringLiteral("label_status"))
                ->text()
                .contains(QStringLiteral("1 hidden by filter")));

    const QString path = dir.filePath(QStringLiteral("results.csv"));
    button(dialog, QStringLiteral("pushButton_reveal"))->click();
    driveExport(dialog, path);
    QFile file(path);
    QVERIFY(file.open(QIODevice::ReadOnly));
    const QString content = QString::fromUtf8(file.readAll());
    QVERIFY(content.contains(QStringLiteral("hash-two")));
    QVERIFY(!content.contains(QStringLiteral("hash-one")));

    // Statistics sees the same visible subset (calculated asynchronously).
    QString summary;
    QTimer timer;
    timer.setInterval(20);
    QObject::connect(&timer, &QTimer::timeout, &timer, [&] {
        if (auto *stats =
                qobject_cast<PotfileStatisticsDialog *>(QApplication::activeModalWidget())) {
            summary = stats->findChild<QLabel *>(QStringLiteral("label_summary"))->text();
            stats->close();
        }
    });
    timer.start();
    button(dialog, QStringLiteral("pushButton_statistics"))->click();
    QTRY_VERIFY_WITH_TIMEOUT(!summary.isEmpty(), 5000);
    timer.stop();
    QVERIFY(summary.contains(QStringLiteral("1 entry")));
    QVERIFY(summary.contains(QStringLiteral("duplicate lines: unknown")));

    // Nothing visible: both actions disable and the empty state says why.
    search(dialog)->setText(QStringLiteral("no-such-hash"));
    QVERIFY(!button(dialog, QStringLiteral("pushButton_export"))->isEnabled());
    QVERIFY(!button(dialog, QStringLiteral("pushButton_statistics"))->isEnabled());
    QCOMPARE(stack(dialog)->currentWidget(),
             static_cast<QWidget *>(label(dialog, QStringLiteral("label_empty_state"))));
    QVERIFY(label(dialog, QStringLiteral("label_empty_state"))
                ->text()
                .contains(QStringLiteral("No rows match the filter.")));
}

void TestPotfileResultsDialog::totalsUseLocaleGrouping()
{
    PotfileCheckResult result = okCheckResult({
        {QStringLiteral("hash-one"), QStringLiteral("secret"), QString(), false, false},
    });
    result.cracked = 1234567;
    result.totalCount = 1234567;

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("MD5"));

    QVERIFY(label(dialog, QStringLiteral("label_status"))
                ->text()
                .contains(QLocale().toString(1234567)));
}

int main(int argc, char *argv[])
{
    TestPotfileResultsDialog test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_potfileresultsdialog.moc"