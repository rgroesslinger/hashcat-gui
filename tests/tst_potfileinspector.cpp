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
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedLayout>
#include <QTemporaryDir>
#include <QTest>
#include <QTreeWidget>

namespace
{

bool writeFile(const QString &path, const QByteArray &content)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }
    return file.write(content) == content.size();
}

bool appendFile(const QString &path, const QByteArray &content)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        return false;
    }
    return file.write(content) == content.size();
}

} // namespace

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
    void copyUsesTheRealValues();
    void wrongPathIsReportedWithoutCreatingAFile();
    void heuristicPathIsLabelled();
    void relativePathUsesTheHashcatDirectory();
    void reloadPicksUpAChangedFile();
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
    QVERIFY(button(dialog, QStringLiteral("pushButton_export"))->isEnabled());
    QVERIFY(button(dialog, QStringLiteral("pushButton_statistics"))->isEnabled());
    QVERIFY(tree(dialog)->topLevelItem(0)->text(1) != QStringLiteral("secret"));
    QVERIFY(tree(dialog)->topLevelItem(0)->text(1).contains(QStringLiteral("•")));
    QVERIFY(label(dialog, QStringLiteral("label_note"))
                ->text()
                .contains(QStringLiteral("No hash type")));
    QCOMPARE(label(dialog, QStringLiteral("label_status"))->textFormat(), Qt::PlainText);

    button(dialog, QStringLiteral("pushButton_reveal"))->click();
    QCOMPARE(tree(dialog)->topLevelItem(0)->text(1), QStringLiteral("secret"));
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

void TestPotfileInspector::copyUsesTheRealValues()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashcat.potfile"));
    QVERIFY(writeFile(path, QByteArrayLiteral("hash-one:secret\n"
                                              "hash-two:hunter2\n")));

    PotfileInspectorDialog dialog;
    dialog.setPotfilePath(path);
    QTRY_COMPARE_WITH_TIMEOUT(tree(dialog)->topLevelItemCount(), 2, 5000);

    tree(dialog)->topLevelItem(0)->setSelected(true);
    button(dialog, QStringLiteral("pushButton_copy_selected"))->click();
    QCOMPARE(QApplication::clipboard()->text(), QStringLiteral("secret"));

    QApplication::clipboard()->clear();
    button(dialog, QStringLiteral("pushButton_copy_all"))->click();
    QCOMPARE(QApplication::clipboard()->text(), QStringLiteral("secret\nhunter2"));
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
    QVERIFY(
        label(dialog, QStringLiteral("label_note"))->text().contains(QStringLiteral("heuristic")));
    QCOMPARE(tree(dialog)->topLevelItemCount(), 1);
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

int main(int argc, char *argv[])
{
    TestPotfileInspector test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_potfileinspector.moc"