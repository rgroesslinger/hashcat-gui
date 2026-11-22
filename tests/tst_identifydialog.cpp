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

#include <QFile>
#include <QFileInfo>
#include <QLabel>
#include <QProcess>
#include <QPushButton>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QTreeWidget>

namespace
{
constexpr auto IdentifyTable = R"(The following 2 hash-modes match the structure of your input hash:

      # | Name                                                       | Category
  ======+============================================================+======================================
      0 | MD5                                                        | Raw Hash
   1000 | NTLM                                                       | Operating System

)";
} // namespace

class TestIdentifyDialog : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void fillsTheTableFromIdentifyOutput();
    void singleMatchUsesSingularMessage();
    void noMatchIsAnEmptyAnswer();
    void applyEmitsTheSelectedIdAndTheInput();
    void leadingDashIsProtectedByTheTerminator();
    void aTypedPathIsMadeAbsolute();
    void aSlashContainingHashStaysLiteral();
    void noMatchOnStderrIsAnEmptyAnswer();
    void editingTheInputDropsStaleResults();
    void anUnconfiguredPathStartsNoQuery();
    void cancelStopsTheQuery();

private:
    static QLabel *status(IdentifyDialog &dialog)
    {
        return dialog.findChild<QLabel *>(QStringLiteral("label_status"));
    }

    static QTreeWidget *results(IdentifyDialog &dialog)
    {
        return dialog.findChild<QTreeWidget *>(QStringLiteral("treeWidget_results"));
    }
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
    auto *identify = dialog.findChild<QPushButton *>(QStringLiteral("pushButton_identify"));
    identify->click();

    QTRY_COMPARE(results(dialog)->topLevelItemCount(), 2);
    const QStringList expected{QString(AppConstants::Hashcat::Identify),
                               QStringLiteral("5d41402abc4b2a76b9719d911017c592")};
    QCOMPARE(args, expected);
    QCOMPARE(results(dialog)->topLevelItem(1)->text(0), QStringLiteral("1000"));
    QCOMPARE(results(dialog)->topLevelItem(1)->text(1), QStringLiteral("NTLM"));
    QVERIFY(status(dialog)->text().contains(QStringLiteral("2 hash modes match")));
    QVERIFY(!dialog.findChild<QPushButton *>(QStringLiteral("pushButton_apply"))->isEnabled());

    results(dialog)->setCurrentItem(results(dialog)->topLevelItem(1));
    QVERIFY(dialog.findChild<QPushButton *>(QStringLiteral("pushButton_apply"))->isEnabled());
}

void TestIdentifyDialog::singleMatchUsesSingularMessage()
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
    dialog.findChild<QPushButton *>(QStringLiteral("pushButton_identify"))->click();

    QTRY_COMPARE(results(dialog)->topLevelItemCount(), 1);
    QVERIFY(status(dialog)->text().contains(QStringLiteral("1 hash mode matches")));
    QVERIFY(!status(dialog)->text().contains(QStringLiteral("1 hash modes")));
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
    dialog.findChild<QPushButton *>(QStringLiteral("pushButton_identify"))->click();

    QTRY_VERIFY(status(dialog)->text().contains(QStringLiteral("No hash modes match")));
    QCOMPARE(results(dialog)->topLevelItemCount(), 0);
}

void TestIdentifyDialog::applyEmitsTheSelectedIdAndTheInput()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(nullptr, cannedQuery(normalResult(QString::fromUtf8(IdentifyTable))));
    dialog.setInput(QStringLiteral("hash-to-identify"));
    QSignalSpy applied(&dialog, &IdentifyDialog::applyRequested);
    dialog.findChild<QPushButton *>(QStringLiteral("pushButton_identify"))->click();
    QTRY_COMPARE(results(dialog)->topLevelItemCount(), 2);

    results(dialog)->setCurrentItem(results(dialog)->topLevelItem(1));
    dialog.findChild<QPushButton *>(QStringLiteral("pushButton_apply"))->click();

    QCOMPARE(applied.count(), 1);
    QCOMPARE(applied.at(0).at(0).toInt(), 1000);
    QCOMPARE(applied.at(0).at(1).toString(), QStringLiteral("hash-to-identify"));
}

void TestIdentifyDialog::leadingDashIsProtectedByTheTerminator()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    QStringList args;
    IdentifyDialog dialog(nullptr,
                          cannedQuery(normalResult(QStringLiteral("garbage"), 255), &args));
    dialog.setInput(QStringLiteral("-suspicious"));
    dialog.findChild<QPushButton *>(QStringLiteral("pushButton_identify"))->click();

    QTRY_VERIFY(!args.isEmpty());
    const QStringList expected{QString(AppConstants::Hashcat::Identify),
                               QString(AppConstants::Hashcat::OptionTerminator),
                               QStringLiteral("-suspicious")};
    QCOMPARE(args, expected);
}

void TestIdentifyDialog::aTypedPathIsMadeAbsolute()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    QStringList args;
    IdentifyDialog dialog(nullptr,
                          cannedQuery(normalResult(QStringLiteral("garbage"), 255), &args));
    const QString path = dir.filePath(QStringLiteral("candidate.hash"));
    QFile file(path);
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write("hash\n");
    file.close();
    dialog.setInput(path);
    dialog.findChild<QPushButton *>(QStringLiteral("pushButton_identify"))->click();

    QTRY_COMPARE(args.size(), 2);
    QCOMPARE(args.at(1), QFileInfo(path).absoluteFilePath());
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
    dialog.findChild<QPushButton *>(QStringLiteral("pushButton_identify"))->click();

    QTRY_COMPARE(args.size(), 2);
    QCOMPARE(args.at(1), hash);
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
    dialog.findChild<QPushButton *>(QStringLiteral("pushButton_identify"))->click();

    QTRY_VERIFY(status(dialog)->text().contains(QStringLiteral("No hash modes match")));
    QCOMPARE(results(dialog)->topLevelItemCount(), 0);
}

// Editing the input after a query must not leave results that Apply would
// attach to the new, unqueried hash.
void TestIdentifyDialog::editingTheInputDropsStaleResults()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    IdentifyDialog dialog(nullptr, cannedQuery(normalResult(QString::fromUtf8(IdentifyTable))));
    dialog.setInput(QStringLiteral("old-hash"));
    dialog.findChild<QPushButton *>(QStringLiteral("pushButton_identify"))->click();
    QTRY_COMPARE(results(dialog)->topLevelItemCount(), 2);
    results(dialog)->setCurrentItem(results(dialog)->topLevelItem(0));
    QVERIFY(dialog.findChild<QPushButton *>(QStringLiteral("pushButton_apply"))->isEnabled());

    dialog.setInput(QStringLiteral("new-hash"));
    QCOMPARE(results(dialog)->topLevelItemCount(), 0);
    QVERIFY(!dialog.findChild<QPushButton *>(QStringLiteral("pushButton_apply"))->isEnabled());
    QVERIFY(status(dialog)->text().contains(QStringLiteral("Input changed")));
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
    dialog.findChild<QPushButton *>(QStringLiteral("pushButton_identify"))->click();

    QCOMPARE(calls, 0);
    QVERIFY(status(dialog)->text().contains(QStringLiteral("is configured")));
}

void TestIdentifyDialog::cancelStopsTheQuery()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    HashcatRunner::CancelToken token;
    IdentifyDialog dialog(
        nullptr, cannedQuery(normalResult(QStringLiteral("too late")), nullptr, &token, 1000));
    dialog.setInput(QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
    auto *identify = dialog.findChild<QPushButton *>(QStringLiteral("pushButton_identify"));
    identify->click();
    auto *cancel = dialog.findChild<QPushButton *>(QStringLiteral("pushButton_cancel"));
    QVERIFY(cancel->isEnabled());

    cancel->click();
    QVERIFY(token);
    QVERIFY(token->load());
    // The runner still has to kill and reap its process: Identify stays
    // disabled until the finished handler reports the cancellation.
    QVERIFY(!identify->isEnabled());
    QVERIFY(!status(dialog)->text().contains(QStringLiteral("cancelled")));
    QTRY_VERIFY_WITH_TIMEOUT(status(dialog)->text().contains(QStringLiteral("cancelled")), 3000);
    QVERIFY(identify->isEnabled());
}

int main(int argc, char *argv[])
{
    TestIdentifyDialog test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_identifydialog.moc"
