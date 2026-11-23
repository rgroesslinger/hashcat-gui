/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfilecheck.h"
#include "potfileresultsdialog.h"
#include "potfiletesthelpers.h"
#include "testenvironment.h"

#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedLayout>
#include <QTest>
#include <QTreeWidget>

using namespace PotfileDialogTestAccess;

class TestPotfileResultsDialog : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void showsMaskedRowsAndTheQueriedMode();
    void headerSizingUsesTheSharedDefaults();
    void usernameColumnFollowsTheEntries();
    void revealShowsRealPlaintexts();
    void copyUsesTheRealValues();
    void copyAllUsesOnlyVisibleRows();
    void searchMatchesHashAndUsernameWhileMasked();
    void encryptedEntriesStayMaskedAndCopyTheToken();
    void binaryHashFileEntriesAreLabeled();
    void leftUnknownUsesSetResult();
    void showFailedUsesSetResult();
    void zeroCrackedShowsTheEmptyStateHint();
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

    QVERIFY(label(dialog, QStringLiteral("label_header"))->text().contains(QStringLiteral("MD5")));
    QVERIFY(
        label(dialog, QStringLiteral("label_status"))->text().contains(QStringLiteral("2 of 3")));
    QCOMPARE(label(dialog, QStringLiteral("label_status"))->textFormat(), Qt::PlainText);
    QCOMPARE(tree(dialog)->topLevelItemCount(), 2);
    QVERIFY(button(dialog, QStringLiteral("pushButton_export"))->isEnabled());
    QVERIFY(button(dialog, QStringLiteral("pushButton_statistics"))->isEnabled());
    QVERIFY(tree(dialog)->topLevelItem(0)->text(1) != QStringLiteral("secret"));
    QVERIFY(tree(dialog)->topLevelItem(0)->text(1).contains(QStringLiteral("•")));
    QVERIFY(!tree(dialog)->isColumnHidden(2));
    QVERIFY(stack(dialog)->currentWidget() != label(dialog, QStringLiteral("label_empty_state")));
}

void TestPotfileResultsDialog::headerSizingUsesTheSharedDefaults()
{
    PotfileResultsDialog dialog;
    QHeaderView *header = tree(dialog)->header();

    QCOMPARE(header->sectionResizeMode(0), QHeaderView::Interactive);
    QCOMPARE(header->sectionSize(0), 320);
    QCOMPARE(header->sectionResizeMode(1), QHeaderView::Stretch);
    QCOMPARE(header->sectionResizeMode(2), QHeaderView::ResizeToContents);
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

void TestPotfileResultsDialog::copyUsesTheRealValues()
{
    const PotfileCheckResult result = okCheckResult({
        {QStringLiteral("hash-one"), QStringLiteral("secret"), QString(), false, false},
        {QStringLiteral("hash-two"), QStringLiteral("hunter2"), QString(), false, false},
    });

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("MD5"));

    tree(dialog)->topLevelItem(0)->setSelected(true);
    button(dialog, QStringLiteral("pushButton_copy_selected"))->click();
    QCOMPARE(QApplication::clipboard()->text(), QStringLiteral("secret"));

    QApplication::clipboard()->clear();
    button(dialog, QStringLiteral("pushButton_copy_all"))->click();
    QCOMPARE(QApplication::clipboard()->text(), QStringLiteral("secret\nhunter2"));
}

void TestPotfileResultsDialog::copyAllUsesOnlyVisibleRows()
{
    const PotfileCheckResult result = okCheckResult({
        {QStringLiteral("hash-one"), QStringLiteral("secret"), QString(), false, false},
        {QStringLiteral("hash-two"), QStringLiteral("hunter2"), QString(), false, false},
    });

    PotfileResultsDialog dialog;
    dialog.setResult(result, QStringLiteral("MD5"));

    search(dialog)->setText(QStringLiteral("hash-two"));
    QVERIFY(tree(dialog)->topLevelItem(0)->isHidden());
    QVERIFY(!tree(dialog)->topLevelItem(1)->isHidden());

    button(dialog, QStringLiteral("pushButton_copy_all"))->click();
    QCOMPARE(QApplication::clipboard()->text(), QStringLiteral("hunter2"));
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
    button(dialog, QStringLiteral("pushButton_reveal"))->click();
    QCOMPARE(tree(dialog)->topLevelItem(0)->text(1), QStringLiteral("(encrypted)"));

    tree(dialog)->topLevelItem(0)->setSelected(true);
    button(dialog, QStringLiteral("pushButton_copy_selected"))->click();
    QCOMPARE(QApplication::clipboard()->text(), QStringLiteral("$HCENC$1$key$cipher"));
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

int main(int argc, char *argv[])
{
    TestPotfileResultsDialog test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_potfileresultsdialog.moc"