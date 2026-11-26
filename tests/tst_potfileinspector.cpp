/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appconstants.h"
#include "hashcatquerytesthelpers.h"
#include "potfileinspector.h"
#include "potfiletesthelpers.h"
#include "programdirectory.h"
#include "settingsmanager.h"
#include "testenvironment.h"

#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QStackedLayout>
#include <QTemporaryDir>
#include <QTest>
#include <QTimer>
#include <QTreeWidget>

using TestEnvironment::appendFile;
using TestEnvironment::writeFile;
using namespace PotfileDialogTestAccess;

class TestPotfileInspector : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void loadsAndMasksThePotfile();
    void emptyPotfileShowsItsEmptyStateHint();
    void marksExactDuplicateLines();
    void searchRespectsTheMask();
    void revealedCopyIsTabSeparated();
    void copyIsDisabledWhileMasked();
    void wrongPathIsReportedWithoutCreatingAFile();
    void heuristicPathIsLabelled();
    void heuristicSurvivesFocusLossAndReloadAndDisappearsOnEdit();
    void focusLossDoesNotLoad();
    void enterLoadsThePath();
    void progressAndStopWhileReading();
    void failedReloadClearsTheRows();
    void relativePathUsesTheHashcatDirectory();
    void reloadPicksUpAChangedFile();
    void reloadPreservesFilterAndReveal();
    void exportSuggestionIsUnderHome();
    void filterWithNoMatchesShowsItsEmptyState();
};

void TestPotfileInspector::init()
{
    TestEnvironment::resetSettings();
    QApplication::clipboard()->clear();
}

void TestPotfileInspector::loadsAndMasksThePotfile()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashcat.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("hash-one:secret\n"
                                              "hash-two:hunter2\n")));

    PotfileInspectorDialog dialog;
    dialog.setPotfilePath(path);

    QTRY_VERIFY_WITH_TIMEOUT(
        label(dialog, QStringLiteral("label_status"))->text().contains(QStringLiteral("2 entries")),
        5000);
    QCOMPARE(tree(dialog)->topLevelItemCount(), 2);
    // Export would write the real plaintexts while the table still masks them.
    QVERIFY(!button(dialog, QStringLiteral("pushButton_export"))->isEnabled());
    QVERIFY(button(dialog, QStringLiteral("pushButton_export"))
                ->toolTip()
                .contains(QStringLiteral("Reveal")));
    QVERIFY(button(dialog, QStringLiteral("pushButton_statistics"))->isEnabled());
    QVERIFY(tree(dialog)->topLevelItem(0)->text(1) != QStringLiteral("secret"));
    QVERIFY(tree(dialog)->topLevelItem(0)->text(1).contains(QStringLiteral("•")));
    QVERIFY(label(dialog, QStringLiteral("label_note"))
                ->text()
                .contains(QStringLiteral("Parsed directly")));
    QCOMPARE(label(dialog, QStringLiteral("label_status"))->textFormat(), Qt::PlainText);

    button(dialog, QStringLiteral("pushButton_reveal"))->click();
    QCOMPARE(tree(dialog)->topLevelItem(0)->text(1), QStringLiteral("secret"));
    QVERIFY(button(dialog, QStringLiteral("pushButton_export"))->isEnabled());
}

void TestPotfileInspector::emptyPotfileShowsItsEmptyStateHint()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("empty.potfile"));
    QVERIFY(writeFile(path, QByteArray()));

    PotfileInspectorDialog dialog;
    dialog.setPotfilePath(path);

    QTRY_VERIFY_WITH_TIMEOUT(
        label(dialog, QStringLiteral("label_status"))->text().contains(QStringLiteral("0 entries")),
        5000);
    QLabel *hint = label(dialog, QStringLiteral("label_empty_state"));
    QCOMPARE(tree(dialog)->topLevelItemCount(), 0);
    QCOMPARE(stack(dialog)->currentWidget(), static_cast<QWidget *>(hint));
    QVERIFY(hint->text().contains(QStringLiteral("no entries")));
}

void TestPotfileInspector::marksExactDuplicateLines()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashcat.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("hash:plain\n"
                                              "hash:plain\n"
                                              "other:plain\n")));

    PotfileInspectorDialog dialog;
    dialog.setPotfilePath(path);

    QTRY_VERIFY_WITH_TIMEOUT(label(dialog, QStringLiteral("label_status"))
                                 ->text()
                                 .contains(QStringLiteral("1 duplicate")),
                             5000);
    QCOMPARE(tree(dialog)->topLevelItemCount(), 3);
    QCOMPARE(tree(dialog)->topLevelItem(1)->text(2), QStringLiteral("dup"));
    QVERIFY(tree(dialog)->topLevelItem(0)->text(2).isEmpty());
    QVERIFY(tree(dialog)->topLevelItem(2)->text(2).isEmpty());
}

void TestPotfileInspector::searchRespectsTheMask()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashcat.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("hash-one:secret\n"
                                              "hash-two:hunter2\n")));

    PotfileInspectorDialog dialog;
    dialog.setPotfilePath(path);
    QTRY_COMPARE_WITH_TIMEOUT(tree(dialog)->topLevelItemCount(), 2, 5000);

    search(dialog)->setText(QStringLiteral("secret"));
    QVERIFY(tree(dialog)->topLevelItem(0)->isHidden());
    QVERIFY(tree(dialog)->topLevelItem(1)->isHidden());

    search(dialog)->setText(QStringLiteral("hash-two"));
    QVERIFY(tree(dialog)->topLevelItem(0)->isHidden());
    QVERIFY(!tree(dialog)->topLevelItem(1)->isHidden());

    button(dialog, QStringLiteral("pushButton_reveal"))->click();
    search(dialog)->setText(QStringLiteral("secret"));
    QVERIFY(!tree(dialog)->topLevelItem(0)->isHidden());
    QVERIFY(tree(dialog)->topLevelItem(1)->isHidden());
}

void TestPotfileInspector::revealedCopyIsTabSeparated()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashcat.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("hash-one:secret\n"
                                              "hash-two:hunter2\n")));

    PotfileInspectorDialog dialog;
    dialog.setPotfilePath(path);
    QTRY_COMPARE_WITH_TIMEOUT(tree(dialog)->topLevelItemCount(), 2, 5000);
    button(dialog, QStringLiteral("pushButton_reveal"))->click();

    tree(dialog)->topLevelItem(0)->setSelected(true);
    button(dialog, QStringLiteral("pushButton_copy_selected"))->click();
    QCOMPARE(QApplication::clipboard()->text(), QStringLiteral("hash-one\tsecret\t\t1"));

    QApplication::clipboard()->clear();
    button(dialog, QStringLiteral("pushButton_copy_all"))->click();
    QCOMPARE(QApplication::clipboard()->text(),
             QStringLiteral("hash-one\tsecret\t\t1\nhash-two\thunter2\t\t2"));
}

void TestPotfileInspector::copyIsDisabledWhileMasked()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashcat.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("hash-one:secret\n")));

    PotfileInspectorDialog dialog;
    dialog.setPotfilePath(path);
    QTRY_COMPARE_WITH_TIMEOUT(tree(dialog)->topLevelItemCount(), 1, 5000);

    QVERIFY(!button(dialog, QStringLiteral("pushButton_copy_all"))->isEnabled());
    button(dialog, QStringLiteral("pushButton_reveal"))->click();
    QVERIFY(button(dialog, QStringLiteral("pushButton_copy_all"))->isEnabled());
}

void TestPotfileInspector::wrongPathIsReportedWithoutCreatingAFile()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("missing.potfile"));

    PotfileInspectorDialog dialog;
    dialog.setPotfilePath(path);

    QTRY_VERIFY_WITH_TIMEOUT(
        label(dialog, QStringLiteral("label_status"))->text().contains(QStringLiteral("not found")),
        5000);
    QVERIFY(!QFile::exists(path));
}

void TestPotfileInspector::heuristicPathIsLabelled()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString program = fakeProgram(dir);
    QVERIFY(!program.isEmpty());
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, program);
    QVERIFY(writeFile(dir.filePath(QStringLiteral("hashcat.potfile")),
                      QByteArrayLiteral("hash:plain\n")));

    PotfileInspectorDialog dialog;
    dialog.setPotfilePath(QString());

    QTRY_VERIFY_WITH_TIMEOUT(
        label(dialog, QStringLiteral("label_status"))->text().contains(QStringLiteral("1 entry")),
        5000);
    QVERIFY(!label(dialog, QStringLiteral("label_heuristic"))->isHidden());
    QCOMPARE(tree(dialog)->topLevelItemCount(), 1);
}

void TestPotfileInspector::heuristicSurvivesFocusLossAndReloadAndDisappearsOnEdit()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString program = fakeProgram(dir);
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, program);
    QVERIFY(writeFile(dir.filePath(QStringLiteral("hashcat.potfile")),
                      QByteArrayLiteral("hash:plain\n")));

    PotfileInspectorDialog dialog;
    dialog.setPotfilePath(QString());
    QTRY_COMPARE_WITH_TIMEOUT(tree(dialog)->topLevelItemCount(), 1, 5000);
    auto *lineEdit = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_potfile"));
    auto *heuristic = label(dialog, QStringLiteral("label_heuristic"));
    QVERIFY(!heuristic->isHidden());

    lineEdit->setFocus();
    lineEdit->clearFocus();
    QVERIFY(!heuristic->isHidden());

    button(dialog, QStringLiteral("pushButton_reload"))->click();
    QTRY_VERIFY_WITH_TIMEOUT(button(dialog, QStringLiteral("pushButton_reload"))->isEnabled(),
                             5000);
    QVERIFY(!heuristic->isHidden());

    QTest::keyClicks(lineEdit, "x");
    QVERIFY(heuristic->isHidden());
}

void TestPotfileInspector::focusLossDoesNotLoad()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashcat.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("hash:plain\n")));

    PotfileInspectorDialog dialog;
    auto *lineEdit = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_potfile"));
    lineEdit->setText(path);
    lineEdit->setFocus();
    lineEdit->clearFocus();
    QTest::qWait(100);

    QCOMPARE(tree(dialog)->topLevelItemCount(), 0);
    QVERIFY(label(dialog, QStringLiteral("label_status"))
                ->text()
                .contains(QStringLiteral("Choose a potfile")));
}

void TestPotfileInspector::enterLoadsThePath()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashcat.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("hash:plain\n")));

    PotfileInspectorDialog dialog;
    dialog.show();
    auto *lineEdit = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_potfile"));
    lineEdit->setText(path);
    QTest::keyClick(lineEdit, Qt::Key_Return);

    QTRY_COMPARE_WITH_TIMEOUT(tree(dialog)->topLevelItemCount(), 1, 5000);
}

void TestPotfileInspector::progressAndStopWhileReading()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("big.potfile"));
    QByteArray big;
    for (int i = 0; i < 300000; ++i) {
        big += QByteArrayLiteral("hash:plain\n");
    }
    QVERIFY(writeFile(path, big));

    PotfileInspectorDialog dialog;
    auto *progress = dialog.findChild<QProgressBar *>(QStringLiteral("progressBar_busy"));
    auto *stop = button(dialog, QStringLiteral("pushButton_stop"));
    QVERIFY(progress->isHidden());
    QVERIFY(!stop->isEnabled());

    // The finished signal is queued: it cannot be delivered before the
    // synchronous stop click below, and a 300k-line parse dwarfs that window.
    dialog.setPotfilePath(path);
    QVERIFY(!progress->isHidden());
    QVERIFY(stop->isEnabled());
    QVERIFY(!button(dialog, QStringLiteral("pushButton_reload"))->isEnabled());

    stop->click();
    QVERIFY(!stop->isEnabled());
    QVERIFY(label(dialog, QStringLiteral("label_status"))
                ->text()
                .contains(QStringLiteral("Cancelling")));
    QTRY_VERIFY_WITH_TIMEOUT(
        label(dialog, QStringLiteral("label_status"))->text().contains(QStringLiteral("cancelled")),
        10000);
    QVERIFY(progress->isHidden());
    QVERIFY(button(dialog, QStringLiteral("pushButton_reload"))->isEnabled());

    // A successful reload, then a cancelled one: the rows of the first must
    // not survive the second with their sourceIndex naming empty entries.
    button(dialog, QStringLiteral("pushButton_reload"))->click();
    QTRY_VERIFY_WITH_TIMEOUT(tree(dialog)->topLevelItemCount() > 0, 10000);
    button(dialog, QStringLiteral("pushButton_reload"))->click();
    stop->click();
    QTRY_VERIFY_WITH_TIMEOUT(
        label(dialog, QStringLiteral("label_status"))->text().contains(QStringLiteral("cancelled")),
        10000);
    QCOMPARE(tree(dialog)->topLevelItemCount(), 0);
    QVERIFY(!button(dialog, QStringLiteral("pushButton_export"))->isEnabled());
    QVERIFY(!button(dialog, QStringLiteral("pushButton_statistics"))->isEnabled());
}

void TestPotfileInspector::failedReloadClearsTheRows()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashcat.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("hash-one:secret\n"
                                              "hash-two:hunter2\n")));

    PotfileInspectorDialog dialog;
    dialog.setPotfilePath(path);
    QTRY_COMPARE_WITH_TIMEOUT(tree(dialog)->topLevelItemCount(), 2, 5000);
    button(dialog, QStringLiteral("pushButton_reveal"))->click();
    QVERIFY(button(dialog, QStringLiteral("pushButton_export"))->isEnabled());

    QVERIFY(QFile::remove(path));
    button(dialog, QStringLiteral("pushButton_reload"))->click();
    QTRY_VERIFY_WITH_TIMEOUT(
        label(dialog, QStringLiteral("label_status"))->text().contains(QStringLiteral("not found")),
        5000);

    // Stale rows would let Export index an empty entry list.
    QCOMPARE(tree(dialog)->topLevelItemCount(), 0);
    QVERIFY(!button(dialog, QStringLiteral("pushButton_export"))->isEnabled());
    QVERIFY(!button(dialog, QStringLiteral("pushButton_statistics"))->isEnabled());
    // "Not found" must not be contradicted by the empty-file hint.
    QCOMPARE(stack(dialog)->currentWidget(), static_cast<QWidget *>(tree(dialog)));
}

void TestPotfileInspector::relativePathUsesTheHashcatDirectory()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString program = fakeProgram(dir);
    QVERIFY(!program.isEmpty());
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, program);
    const QString path = dir.filePath(QStringLiteral("hashcat.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("hash:plain\n")));

    PotfileInspectorDialog dialog;
    dialog.setPotfilePath(QStringLiteral("hashcat.potfile"));

    QTRY_VERIFY_WITH_TIMEOUT(
        label(dialog, QStringLiteral("label_status"))->text().contains(QStringLiteral("1 entry")),
        5000);
    auto *lineEdit = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_potfile"));
    QVERIFY(lineEdit);
    QCOMPARE(
        lineEdit->text(),
        QDir(ProgramDirectory::forProgram(program)).filePath(QStringLiteral("hashcat.potfile")));
}

void TestPotfileInspector::reloadPicksUpAChangedFile()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashcat.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("hash-one:secret\n")));

    PotfileInspectorDialog dialog;
    dialog.setPotfilePath(path);
    QTRY_COMPARE_WITH_TIMEOUT(tree(dialog)->topLevelItemCount(), 1, 5000);

    QVERIFY(appendFile(path, QByteArrayLiteral("hash-two:hunter2\n")));
    button(dialog, QStringLiteral("pushButton_reload"))->click();
    QTRY_COMPARE_WITH_TIMEOUT(tree(dialog)->topLevelItemCount(), 2, 5000);
    QVERIFY(label(dialog, QStringLiteral("label_status"))
                ->text()
                .contains(QStringLiteral("2 entries")));
}

void TestPotfileInspector::reloadPreservesFilterAndReveal()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashcat.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("hash-one:secret\n"
                                              "hash-two:hunter2\n")));

    PotfileInspectorDialog dialog;
    dialog.setPotfilePath(path);
    QTRY_COMPARE_WITH_TIMEOUT(tree(dialog)->topLevelItemCount(), 2, 5000);

    button(dialog, QStringLiteral("pushButton_reveal"))->click();
    search(dialog)->setText(QStringLiteral("hash-two"));
    QVERIFY(tree(dialog)->topLevelItem(0)->isHidden());

    QVERIFY(appendFile(path, QByteArrayLiteral("hash-two:extra\n")));
    button(dialog, QStringLiteral("pushButton_reload"))->click();
    QTRY_COMPARE_WITH_TIMEOUT(tree(dialog)->topLevelItemCount(), 3, 5000);

    // Search text, reveal state and the filtered view survive the reload.
    QCOMPARE(search(dialog)->text(), QStringLiteral("hash-two"));
    QVERIFY(tree(dialog)->topLevelItem(0)->isHidden());
    QCOMPARE(tree(dialog)->topLevelItem(1)->text(1), QStringLiteral("hunter2"));
    QCOMPARE(button(dialog, QStringLiteral("pushButton_reveal"))->text(),
             QStringLiteral("Hide plaintexts"));
    QVERIFY(label(dialog, QStringLiteral("label_status"))
                ->text()
                .contains(QStringLiteral("1 hidden by filter")));
}

void TestPotfileInspector::exportSuggestionIsUnderHome()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("my.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("hash:plain\n")));

    PotfileInspectorDialog dialog;
    dialog.setPotfilePath(path);
    QTRY_COMPARE_WITH_TIMEOUT(tree(dialog)->topLevelItemCount(), 1, 5000);
    button(dialog, QStringLiteral("pushButton_reveal"))->click();

    QString initial;
    QTimer timer;
    timer.setInterval(20);
    QObject::connect(&timer, &QTimer::timeout, &timer, [&] {
        if (auto *fd = qobject_cast<QFileDialog *>(QApplication::activeModalWidget())) {
            if (initial.isEmpty()) {
                initial = fd->selectedFiles().value(0);
            }
            fd->reject();
        }
    });
    timer.start();
    button(dialog, QStringLiteral("pushButton_export"))->click();
    timer.stop();

    QVERIFY(initial.startsWith(QDir::homePath()));
    QVERIFY(initial.contains(QStringLiteral("my-entries")));
}

void TestPotfileInspector::filterWithNoMatchesShowsItsEmptyState()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashcat.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("hash-one:secret\n")));

    PotfileInspectorDialog dialog;
    dialog.setPotfilePath(path);
    QTRY_COMPARE_WITH_TIMEOUT(tree(dialog)->topLevelItemCount(), 1, 5000);

    search(dialog)->setText(QStringLiteral("no-such-hash"));
    QCOMPARE(stack(dialog)->currentWidget(),
             static_cast<QWidget *>(label(dialog, QStringLiteral("label_empty_state"))));
    QVERIFY(label(dialog, QStringLiteral("label_empty_state"))
                ->text()
                .contains(QStringLiteral("No rows match the filter.")));
    QVERIFY(!button(dialog, QStringLiteral("pushButton_export"))->isEnabled());
}

int main(int argc, char *argv[])
{
    TestPotfileInspector test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_potfileinspector.moc"