/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appconstants.h"
#include "hashcatquerytesthelpers.h"
#include "hashcatrunner.h"
#include "identifydialog.h"
#include "settingsmanager.h"
#include "testenvironment.h"

#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLabel>
#include <QProcess>
#include <QProgressBar>
#include <QPushButton>
#include <QTemporaryDir>
#include <QTest>
#include <QTreeWidget>

#include <optional>

namespace
{
constexpr auto IdentifyTable = R"(The following 2 hash-modes match the structure of your input hash:

      # | Name                                                       | Category
  ======+============================================================+======================================
      0 | MD5                                                        | Raw Hash
   1000 | NTLM                                                       | Operating System

)";

// What the injected apply handler saw, and what it answers.
struct ApplyCapture {
    int calls = 0;
    int id = -1;
    std::optional<QString> input;
    bool answer = true;
};

IdentifyDialog::ApplyHandler captureHandler(ApplyCapture &capture)
{
    return [&capture](int id, const std::optional<QString> &input) {
        ++capture.calls;
        capture.id = id;
        capture.input = input;
        return capture.answer;
    };
}

QLabel *status(IdentifyDialog &dialog)
{
    return dialog.findChild<QLabel *>(QStringLiteral("label_status"));
}

QTreeWidget *results(IdentifyDialog &dialog)
{
    return dialog.findChild<QTreeWidget *>(QStringLiteral("treeWidget_results"));
}

QPushButton *button(IdentifyDialog &dialog, const char *name)
{
    return dialog.findChild<QPushButton *>(QString::fromLatin1(name));
}

// The session argument every Identify query carries: --session=hcgui-id-…
QString sessionArg(const QStringList &args)
{
    for (const QString &arg : args) {
        if (arg.startsWith(QStringLiteral("--session="))) {
            return arg;
        }
    }
    return {};
}
} // namespace

class TestIdentifyDialog : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void fillsTheTableFromIdentifyOutput();
    void twoQueriesUseDifferentSessions();
    void singleMatchIsPreselected();
    void noMatchIsAnEmptyAnswer();
    void noMatchOnStderrIsAnEmptyAnswer();
    void applyHandlerReceivesTheIdAndNulloptForUnchangedInput();
    void editedInputIsPassedRaw();
    void handlerFalseKeepsTheDialogAndRows();
    void applyWithoutAHandlerDoesNothing();
    void leadingDashIsProtectedByTheTerminator();
    void aRelativePathResolvesAgainstTheHashcatDirectory();
    void anAbsolutePathIsQueriedAsIs();
    void aSlashContainingHashStaysLiteral();
    void editingTheInputDropsStaleResults();
    void rejectedPasteKeepsTheResults();
    void anUnconfiguredPathStartsNoQuery();
    void stopCancelsTheQuery();
    void closingWhileBusyCancelsTheQuery();
    void progressAndElapsedAreVisibleOnlyWhileBusy();
    void modesSortNumerically();
    void enterInTheInputStartsIdentify();
    void enterInTheTreeAppliesTheSelection();
    void enterDoesNotOpenBrowse();
    void doubleClickSelectsButDoesNotApply();
    void multiLinePasteIsRejectedWithAHint();
    void singleLinePasteStillInserts();
    void thePasteSlotPathIsGuarded();
    void initialStatusGuidesTheUser();
    void pidfileCollisionIsMapped();
    void noHashesLoadedIsMapped();
    void tokenEncodingIsMapped();
    void unknownStderrIsShownRaw();
    void anUnparsableReplyWithoutStderrSaysSo();
};

void TestIdentifyDialog::init()
{
    TestEnvironment::resetSettings();
}

void TestIdentifyDialog::fillsTheTableFromIdentifyOutput()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    QStringList args;
    IdentifyDialog dialog(nullptr,
                          cannedQuery(normalResult(QString::fromUtf8(IdentifyTable)), &args));
    dialog.setInput(QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
    button(dialog, "pushButton_identify")->click();

    QTRY_COMPARE(results(dialog)->topLevelItemCount(), 2);
    // --identify, --logfile-disable, a session of its own, then the hash.
    QCOMPARE(args.size(), 4);
    QCOMPARE(args.at(0), QString(AppConstants::Hashcat::Identify));
    QCOMPARE(args.at(1), QString(AppConstants::Hashcat::LogfileDisable));
    QVERIFY(sessionArg(args).startsWith(
        QStringLiteral("--session=%1").arg(QString(AppConstants::Hashcat::IdentifySessionPrefix))));
    QCOMPARE(args.at(3), QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
    QCOMPARE(results(dialog)->topLevelItem(1)->text(0), QStringLiteral("1000"));
    QCOMPARE(results(dialog)->topLevelItem(1)->text(1), QStringLiteral("NTLM"));
    QVERIFY(status(dialog)->text().contains(QStringLiteral("2 hash modes match")));
    QVERIFY(!button(dialog, "pushButton_apply")->isEnabled());

    results(dialog)->setCurrentItem(results(dialog)->topLevelItem(1));
    QVERIFY(button(dialog, "pushButton_apply")->isEnabled());
}

void TestIdentifyDialog::twoQueriesUseDifferentSessions()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    QStringList argsA;
    QStringList argsB;
    IdentifyDialog dialogA(nullptr,
                           cannedQuery(normalResult(QString::fromUtf8(IdentifyTable)), &argsA));
    IdentifyDialog dialogB(nullptr,
                           cannedQuery(normalResult(QString::fromUtf8(IdentifyTable)), &argsB));
    dialogA.setInput(QStringLiteral("hash-a"));
    dialogB.setInput(QStringLiteral("hash-b"));
    button(dialogA, "pushButton_identify")->click();
    button(dialogB, "pushButton_identify")->click();

    QTRY_VERIFY(!argsA.isEmpty() && !argsB.isEmpty());
    QVERIFY(!sessionArg(argsA).isEmpty());
    QVERIFY(sessionArg(argsA) != sessionArg(argsB));
}

void TestIdentifyDialog::singleMatchIsPreselected()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    const QString oneMatch =
        QStringLiteral("The following 1 hash-modes match the structure of your input hash:\n\n"
                       "      # | Name | Category\n"
                       "  ======+======+==========\n"
                       "      0 | MD5  | Raw Hash\n\n");
    IdentifyDialog dialog(nullptr, cannedQuery(normalResult(oneMatch)));
    dialog.setInput(QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
    button(dialog, "pushButton_identify")->click();

    QTRY_COMPARE(results(dialog)->topLevelItemCount(), 1);
    QVERIFY(status(dialog)->text().contains(QStringLiteral("1 hash mode matches")));
    QVERIFY(!status(dialog)->text().contains(QStringLiteral("1 hash modes")));
    // The only candidate is already current, so Apply is ready.
    QCOMPARE(results(dialog)->currentItem(), results(dialog)->topLevelItem(0));
    QVERIFY(button(dialog, "pushButton_apply")->isEnabled());
}

void TestIdentifyDialog::noMatchIsAnEmptyAnswer()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(
        nullptr,
        cannedQuery(normalResult(QStringLiteral("No hash-mode matches the structure of the input "
                                                "hash."),
                                 255)));
    dialog.setInput(QStringLiteral("not-a-hash"));
    button(dialog, "pushButton_identify")->click();

    QTRY_VERIFY(status(dialog)->text().contains(QStringLiteral("No hash modes match")));
    QCOMPARE(results(dialog)->topLevelItemCount(), 0);
}

// hashcat writes "no hash-mode matches" to stderr with exit 255. The dialog
// must turn that into the empty-answer message, not a generic failure.
void TestIdentifyDialog::noMatchOnStderrIsAnEmptyAnswer()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    HashcatResult result;
    result.exitStatus = QProcess::NormalExit;
    result.exitCode = 255;
    result.standardError =
        QStringLiteral("No hash-mode matches the structure of the input hash.\n");
    IdentifyDialog dialog(nullptr, cannedQuery(result));
    dialog.setInput(QStringLiteral("not-a-hash"));
    button(dialog, "pushButton_identify")->click();

    QTRY_VERIFY(status(dialog)->text().contains(QStringLiteral("No hash modes match")));
    QCOMPARE(results(dialog)->topLevelItemCount(), 0);
}

void TestIdentifyDialog::applyHandlerReceivesTheIdAndNulloptForUnchangedInput()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(nullptr, cannedQuery(normalResult(QString::fromUtf8(IdentifyTable))));
    ApplyCapture capture;
    dialog.setApplyHandler(captureHandler(capture));
    dialog.setInput(QStringLiteral("hash-to-identify"));
    button(dialog, "pushButton_identify")->click();
    QTRY_COMPARE(results(dialog)->topLevelItemCount(), 2);
    QCOMPARE(button(dialog, "pushButton_apply")->text(), QStringLiteral("Apply mode to attack"));

    results(dialog)->setCurrentItem(results(dialog)->topLevelItem(1));
    button(dialog, "pushButton_apply")->click();

    QCOMPARE(capture.calls, 1);
    QCOMPARE(capture.id, 1000);
    // Unchanged input: the form keeps its own text.
    QVERIFY(!capture.input.has_value());
    QCOMPARE(dialog.result(), int(QDialog::Accepted));
}

void TestIdentifyDialog::editedInputIsPassedRaw()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(nullptr, cannedQuery(normalResult(QString::fromUtf8(IdentifyTable))));
    ApplyCapture capture;
    dialog.setApplyHandler(captureHandler(capture));
    dialog.setInput(QStringLiteral("original-hash"));
    // The user edits before identifying: the query runs on the edited text,
    // and Apply writes that raw text back, never a resolved path.
    dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_input"))
        ->setText(QStringLiteral("  edited-hash  "));
    button(dialog, "pushButton_identify")->click();
    QTRY_COMPARE(results(dialog)->topLevelItemCount(), 2);
    QCOMPARE(button(dialog, "pushButton_apply")->text(),
             QStringLiteral("Apply mode and hash to attack"));

    results(dialog)->setCurrentItem(results(dialog)->topLevelItem(0));
    button(dialog, "pushButton_apply")->click();

    QCOMPARE(capture.calls, 1);
    QCOMPARE(capture.input, std::optional<QString>(QStringLiteral("edited-hash")));
    QCOMPARE(dialog.result(), int(QDialog::Accepted));
}

void TestIdentifyDialog::handlerFalseKeepsTheDialogAndRows()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(nullptr, cannedQuery(normalResult(QString::fromUtf8(IdentifyTable))));
    ApplyCapture capture;
    capture.answer = false;
    dialog.setApplyHandler(captureHandler(capture));
    dialog.setInput(QStringLiteral("hash-to-identify"));
    button(dialog, "pushButton_identify")->click();
    QTRY_COMPARE(results(dialog)->topLevelItemCount(), 2);

    results(dialog)->setCurrentItem(results(dialog)->topLevelItem(1));
    button(dialog, "pushButton_apply")->click();

    QCOMPARE(capture.calls, 1);
    QVERIFY(dialog.result() != int(QDialog::Accepted));
    QCOMPARE(results(dialog)->topLevelItemCount(), 2);
    QCOMPARE(results(dialog)->currentItem(), results(dialog)->topLevelItem(1));
    QVERIFY(button(dialog, "pushButton_apply")->isEnabled());
}

void TestIdentifyDialog::applyWithoutAHandlerDoesNothing()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(nullptr, cannedQuery(normalResult(QString::fromUtf8(IdentifyTable))));
    dialog.setInput(QStringLiteral("hash-to-identify"));
    button(dialog, "pushButton_identify")->click();
    QTRY_COMPARE(results(dialog)->topLevelItemCount(), 2);

    results(dialog)->setCurrentItem(results(dialog)->topLevelItem(0));
    button(dialog, "pushButton_apply")->click();

    QVERIFY(dialog.result() != int(QDialog::Accepted));
    QCOMPARE(results(dialog)->topLevelItemCount(), 2);
}

void TestIdentifyDialog::leadingDashIsProtectedByTheTerminator()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    QStringList args;
    IdentifyDialog dialog(nullptr,
                          cannedQuery(normalResult(QStringLiteral("garbage"), 255), &args));
    dialog.setInput(QStringLiteral("-suspicious"));
    button(dialog, "pushButton_identify")->click();

    QTRY_VERIFY(!args.isEmpty());
    QCOMPARE(args.size(), 5);
    QCOMPARE(args.at(3), QString(AppConstants::Hashcat::OptionTerminator));
    QCOMPARE(args.at(4), QStringLiteral("-suspicious"));
}

// A relative path is resolved against hashcat's working directory for the
// query, but Apply never rewrites the form with the resolved path.
void TestIdentifyDialog::aRelativePathResolvesAgainstTheHashcatDirectory()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));
    const QString path = dir.filePath(QStringLiteral("candidate.hash"));
    QFile file(path);
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write("hash\n");
    file.close();

    QStringList args;
    IdentifyDialog dialog(nullptr,
                          cannedQuery(normalResult(QString::fromUtf8(IdentifyTable)), &args));
    ApplyCapture capture;
    dialog.setApplyHandler(captureHandler(capture));
    dialog.setInput(QStringLiteral("candidate.hash"));
    button(dialog, "pushButton_identify")->click();

    QTRY_VERIFY(!args.isEmpty());
    QCOMPARE(args.last(), QDir::cleanPath(path));
    QTRY_COMPARE(results(dialog)->topLevelItemCount(), 2);

    results(dialog)->setCurrentItem(results(dialog)->topLevelItem(0));
    button(dialog, "pushButton_apply")->click();
    QCOMPARE(capture.calls, 1);
    QVERIFY(!capture.input.has_value());
}

void TestIdentifyDialog::anAbsolutePathIsQueriedAsIs()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));
    const QString path = dir.filePath(QStringLiteral("candidate.hash"));
    QFile file(path);
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write("hash\n");
    file.close();

    QStringList args;
    IdentifyDialog dialog(nullptr,
                          cannedQuery(normalResult(QStringLiteral("garbage"), 255), &args));
    dialog.setInput(path);
    button(dialog, "pushButton_identify")->click();

    QTRY_VERIFY(!args.isEmpty());
    QCOMPARE(args.last(), QFileInfo(path).absoluteFilePath());
}

// A bcrypt hash uses '/' in its base64 payload. Only an existing file is a
// path: rewriting this literal would make hashcat report "no match".
void TestIdentifyDialog::aSlashContainingHashStaysLiteral()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    QStringList args;
    IdentifyDialog dialog(nullptr,
                          cannedQuery(normalResult(QStringLiteral("garbage"), 255), &args));
    const QString hash =
        QStringLiteral("$2b$12$R9h/cIPz0gi.URNNX3kh2OPST9/PgBkqquzi.Ss7KIUgO2t0jWMUW");
    dialog.setInput(hash);
    button(dialog, "pushButton_identify")->click();

    QTRY_VERIFY(!args.isEmpty());
    QCOMPARE(args.last(), hash);
}

// Editing the input after a query must not leave results that Apply would
// attach to the new, unqueried hash.
void TestIdentifyDialog::editingTheInputDropsStaleResults()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(nullptr, cannedQuery(normalResult(QString::fromUtf8(IdentifyTable))));
    dialog.setInput(QStringLiteral("old-hash"));
    button(dialog, "pushButton_identify")->click();
    QTRY_COMPARE(results(dialog)->topLevelItemCount(), 2);
    results(dialog)->setCurrentItem(results(dialog)->topLevelItem(0));
    QVERIFY(button(dialog, "pushButton_apply")->isEnabled());

    dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_input"))
        ->setText(QStringLiteral("new-hash"));
    QTRY_COMPARE(results(dialog)->topLevelItemCount(), 0);
    QVERIFY(!button(dialog, "pushButton_apply")->isEnabled());
    QVERIFY(status(dialog)->text().contains(QStringLiteral("Input changed")));
}

// A rejected paste restores the text byte-for-byte; the results must survive.
void TestIdentifyDialog::rejectedPasteKeepsTheResults()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(nullptr, cannedQuery(normalResult(QString::fromUtf8(IdentifyTable))));
    dialog.setInput(QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
    button(dialog, "pushButton_identify")->click();
    QTRY_COMPARE(results(dialog)->topLevelItemCount(), 2);
    results(dialog)->setCurrentItem(results(dialog)->topLevelItem(1));

    auto *input = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_input"));
    QApplication::clipboard()->setText(QStringLiteral("first\nsecond"));
    input->paste();

    QCOMPARE(input->text(), QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
    QCOMPARE(results(dialog)->topLevelItemCount(), 2);
    QCOMPARE(results(dialog)->currentItem(), results(dialog)->topLevelItem(1));
    QVERIFY(button(dialog, "pushButton_apply")->isEnabled());
    QVERIFY(status(dialog)->text().contains(QStringLiteral("Multiple lines pasted")));
}

void TestIdentifyDialog::anUnconfiguredPathStartsNoQuery()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, QString());
    int calls = 0;
    IdentifyDialog dialog(nullptr, [&calls](const QString &, const QStringList &, int,
                                            const HashcatRunner::CancelToken &) {
        ++calls;
        return QtConcurrent::run([] { return HashcatResult{}; });
    });
    dialog.setInput(QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
    button(dialog, "pushButton_identify")->click();

    QCOMPARE(calls, 0);
    QVERIFY(status(dialog)->text().contains(QStringLiteral("is configured")));
}

void TestIdentifyDialog::stopCancelsTheQuery()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    HashcatRunner::CancelToken token;
    IdentifyDialog dialog(
        nullptr, cannedQuery(normalResult(QStringLiteral("too late")), nullptr, &token, 1000));
    dialog.setInput(QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
    auto *identify = button(dialog, "pushButton_identify");
    identify->click();
    auto *stop = button(dialog, "pushButton_cancel");
    QVERIFY(stop->isEnabled());

    stop->click();
    QVERIFY(token);
    QVERIFY(token->load());
    // Stop disables itself at once; the runner still has to kill and reap its
    // process, so Identify stays disabled until the finished handler reports.
    QVERIFY(!stop->isEnabled());
    QVERIFY(!identify->isEnabled());
    QVERIFY(status(dialog)->text().contains(QStringLiteral("Cancelling")));
    QTRY_VERIFY_WITH_TIMEOUT(status(dialog)->text().contains(QStringLiteral("cancelled")), 3000);
    QVERIFY(identify->isEnabled());
}

void TestIdentifyDialog::closingWhileBusyCancelsTheQuery()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    HashcatRunner::CancelToken token;
    IdentifyDialog dialog(
        nullptr, cannedQuery(normalResult(QStringLiteral("too late")), nullptr, &token, 1000));
    dialog.setInput(QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
    dialog.show();
    button(dialog, "pushButton_identify")->click();
    QVERIFY(token);
    QVERIFY(!token->load());

    dialog.close();
    QVERIFY(token->load());
}

void TestIdentifyDialog::progressAndElapsedAreVisibleOnlyWhileBusy()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(nullptr, cannedQuery(normalResult(QString::fromUtf8(IdentifyTable)),
                                               nullptr, nullptr, 300));
    auto *progress = dialog.findChild<QProgressBar *>(QStringLiteral("progressBar_busy"));
    auto *elapsed = dialog.findChild<QLabel *>(QStringLiteral("label_elapsed"));
    QVERIFY(progress->isHidden());
    QVERIFY(elapsed->isHidden());

    dialog.setInput(QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
    button(dialog, "pushButton_identify")->click();
    QVERIFY(!progress->isHidden());
    QVERIFY(!elapsed->isHidden());
    QCOMPARE(progress->minimum(), 0);
    QCOMPARE(progress->maximum(), 0);

    QTRY_COMPARE_WITH_TIMEOUT(results(dialog)->topLevelItemCount(), 2, 3000);
    QVERIFY(progress->isHidden());
    QVERIFY(elapsed->isHidden());
}

void TestIdentifyDialog::modesSortNumerically()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    const QString table =
        QStringLiteral("The following 3 hash-modes match the structure of your input hash:\n\n"
                       "      # | Name | Category\n"
                       "  ======+======+==========\n"
                       "   1000 | NTLM | Operating System\n"
                       "    999 | SIP | Raw Hash\n"
                       "      0 | MD5 | Raw Hash\n\n");
    IdentifyDialog dialog(nullptr, cannedQuery(normalResult(table)));
    dialog.setInput(QStringLiteral("hash"));
    button(dialog, "pushButton_identify")->click();
    QTRY_COMPARE(results(dialog)->topLevelItemCount(), 3);

    // Text order would put 1000 before 999; the id column sorts numerically.
    QCOMPARE(results(dialog)->topLevelItem(0)->text(0), QStringLiteral("0"));
    QCOMPARE(results(dialog)->topLevelItem(1)->text(0), QStringLiteral("999"));
    QCOMPARE(results(dialog)->topLevelItem(2)->text(0), QStringLiteral("1000"));
}

void TestIdentifyDialog::enterInTheInputStartsIdentify()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    QStringList args;
    IdentifyDialog dialog(nullptr,
                          cannedQuery(normalResult(QString::fromUtf8(IdentifyTable)), &args));
    dialog.setInput(QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
    dialog.show();
    auto *input = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_input"));
    QTest::keyClick(input, Qt::Key_Return);

    QTRY_VERIFY(!args.isEmpty());
    QCOMPARE(args.last(), QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
}

void TestIdentifyDialog::enterInTheTreeAppliesTheSelection()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(nullptr, cannedQuery(normalResult(QString::fromUtf8(IdentifyTable))));
    ApplyCapture capture;
    dialog.setApplyHandler(captureHandler(capture));
    dialog.setInput(QStringLiteral("hash"));
    button(dialog, "pushButton_identify")->click();
    QTRY_COMPARE(results(dialog)->topLevelItemCount(), 2);
    dialog.show();

    results(dialog)->setCurrentItem(results(dialog)->topLevelItem(1));
    QTest::keyClick(results(dialog), Qt::Key_Return);

    QCOMPARE(capture.calls, 1);
    QCOMPARE(capture.id, 1000);
    QCOMPARE(dialog.result(), int(QDialog::Accepted));
}

void TestIdentifyDialog::enterDoesNotOpenBrowse()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    QStringList args;
    IdentifyDialog dialog(nullptr,
                          cannedQuery(normalResult(QString::fromUtf8(IdentifyTable)), &args));
    dialog.setInput(QStringLiteral("hash"));
    dialog.show();
    auto *browse = button(dialog, "pushButton_browse");
    browse->setFocus();
    QTest::keyClick(browse, Qt::Key_Return);

    // The dialog default (Identify) fires, not the focused Browse button.
    QTRY_VERIFY(!args.isEmpty());
}

void TestIdentifyDialog::doubleClickSelectsButDoesNotApply()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(nullptr, cannedQuery(normalResult(QString::fromUtf8(IdentifyTable))));
    ApplyCapture capture;
    dialog.setApplyHandler(captureHandler(capture));
    dialog.setInput(QStringLiteral("hash"));
    button(dialog, "pushButton_identify")->click();
    QTRY_COMPARE(results(dialog)->topLevelItemCount(), 2);
    dialog.show();

    auto *tree = results(dialog);
    const QRect rect = tree->visualItemRect(tree->topLevelItem(1));
    QTest::mouseDClick(tree->viewport(), Qt::LeftButton, Qt::NoModifier, rect.center());

    QCOMPARE(capture.calls, 0);
    QVERIFY(dialog.result() != int(QDialog::Accepted));
    QCOMPARE(tree->currentItem(), tree->topLevelItem(1));
}

void TestIdentifyDialog::multiLinePasteIsRejectedWithAHint()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(nullptr, cannedQuery(normalResult()));
    auto *input = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_input"));
    input->setFocus();

    QApplication::clipboard()->setText(QStringLiteral("first\nsecond"));
    QTest::keyClick(input, Qt::Key_V, Qt::ControlModifier);
    QVERIFY(input->text().isEmpty());
    QVERIFY(status(dialog)->text().contains(QStringLiteral("Multiple lines pasted")));
    QVERIFY(status(dialog)->text().contains(QStringLiteral("Browse")));

    QApplication::clipboard()->setText(QStringLiteral("first\r\nsecond"));
    QTest::keyClick(input, Qt::Key_V, Qt::ControlModifier);
    QVERIFY(input->text().isEmpty());
}

void TestIdentifyDialog::singleLinePasteStillInserts()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(nullptr, cannedQuery(normalResult()));
    auto *input = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_input"));
    input->setFocus();

    // One trailing line break belongs to the copied line: \n, \r\n and \r.
    const QStringList pastes{QStringLiteral("hash\n"), QStringLiteral("hash\r\n"),
                             QStringLiteral("hash\r")};
    for (const QString &paste : pastes) {
        input->clear();
        QApplication::clipboard()->setText(paste);
        QTest::keyClick(input, Qt::Key_V, Qt::ControlModifier);
        QCOMPARE(input->text(), QStringLiteral("hash"));
    }
}

// The context menu's Paste action triggers the paste() slot, not a key event.
void TestIdentifyDialog::thePasteSlotPathIsGuarded()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(nullptr, cannedQuery(normalResult()));
    auto *input = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_input"));

    QApplication::clipboard()->setText(QStringLiteral("first\nsecond"));
    input->paste();
    QVERIFY(input->text().isEmpty());
    QVERIFY(status(dialog)->text().contains(QStringLiteral("Multiple lines pasted")));

    QApplication::clipboard()->setText(QStringLiteral("hash\r\n"));
    input->paste();
    QCOMPARE(input->text(), QStringLiteral("hash"));
}

void TestIdentifyDialog::initialStatusGuidesTheUser()
{
    IdentifyDialog dialog(nullptr, cannedQuery(normalResult()));
    QVERIFY(!status(dialog)->text().isEmpty());
    QVERIFY(status(dialog)->text().contains(QStringLiteral("Paste a hash")));
}

void TestIdentifyDialog::pidfileCollisionIsMapped()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(
        nullptr,
        cannedQuery(failedResult(255, QString(),
                                 QStringLiteral("Already an instance '/usr/bin/hashcat' running on "
                                                "pid 1234\n"))));
    dialog.setInput(QStringLiteral("hash"));
    button(dialog, "pushButton_identify")->click();

    QTRY_VERIFY(status(dialog)->text().contains(QStringLiteral("Another hashcat instance")));
}

void TestIdentifyDialog::noHashesLoadedIsMapped()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(
        nullptr, cannedQuery(failedResult(255, QString(), QStringLiteral("No hashes loaded.\n"))));
    dialog.setInput(QStringLiteral("hash"));
    button(dialog, "pushButton_identify")->click();

    QTRY_VERIFY(status(dialog)->text().contains(QStringLiteral("could not load any hashes")));
}

void TestIdentifyDialog::tokenEncodingIsMapped()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(
        nullptr,
        cannedQuery(failedResult(255, QString(), QStringLiteral("Token encoding exception\n"))));
    dialog.setInput(QStringLiteral("hash"));
    button(dialog, "pushButton_identify")->click();

    QTRY_VERIFY(status(dialog)->text().contains(QStringLiteral("cannot encode")));
}

void TestIdentifyDialog::unknownStderrIsShownRaw()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(nullptr,
                          cannedQuery(failedResult(255, QStringLiteral("garbage"),
                                                   QStringLiteral("Something unexpected\n"))));
    dialog.setInput(QStringLiteral("hash"));
    button(dialog, "pushButton_identify")->click();

    QTRY_VERIFY(status(dialog)->text().contains(QStringLiteral("hashcat reported:")));
    QVERIFY(status(dialog)->text().contains(QStringLiteral("Something unexpected")));
}

void TestIdentifyDialog::anUnparsableReplyWithoutStderrSaysSo()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(nullptr, cannedQuery(normalResult(QStringLiteral("garbage"), 255)));
    dialog.setInput(QStringLiteral("hash"));
    button(dialog, "pushButton_identify")->click();

    QTRY_VERIFY(status(dialog)->text().contains(QStringLiteral("could not be parsed")));
}

int main(int argc, char *argv[])
{
    TestIdentifyDialog test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_identifydialog.moc"
