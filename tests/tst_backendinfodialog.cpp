/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appconstants.h"
#include "backendinfodialog.h"
#include "hashcatquerytesthelpers.h"
#include "hashcatrunner.h"
#include "settingsmanager.h"
#include "testenvironment.h"

#include <QLabel>
#include <QPlainTextEdit>
#include <QProcess>
#include <QPushButton>
#include <QTemporaryDir>

class TestBackendInfoDialog : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void showsTheRawOutputAndAsksForBackendInfo();
    void eachQueryUsesItsOwnSession();
    void anUnconfiguredPathStartsNoQuery();
    void aMissingExecutableStartsNoQuery();
    void aProcessFailureIsReported();
    void aTimeoutIsReported();
    void cancelStopsTheQuery();

private:
    static QLabel *status(BackendInfoDialog &dialog)
    {
        return dialog.findChild<QLabel *>(QStringLiteral("label_status"));
    }
};

void TestBackendInfoDialog::init()
{
    TestEnvironment::resetSettings();
}

void TestBackendInfoDialog::showsTheRawOutputAndAsksForBackendInfo()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    QStringList args;
    BackendInfoDialog dialog(
        nullptr,
        cannedQuery(normalResult(QStringLiteral("OpenCL Info:\n  Vendor..: test\n")), &args));
    auto *output = dialog.findChild<QPlainTextEdit *>(QStringLiteral("plainTextEdit_output"));
    QTRY_VERIFY(output->toPlainText().contains(QStringLiteral("OpenCL Info")));
    QCOMPARE(args.value(0), QString(AppConstants::Hashcat::BackendInfo));
    QVERIFY(args.contains(QString(AppConstants::Hashcat::LogfileDisable)));
    const QStringList sessions = args.filter(QStringLiteral("--session="));
    QCOMPARE(sessions.size(), 1);
    QVERIFY(sessions.first().startsWith(
        QStringLiteral("--session=") + QString(AppConstants::Hashcat::BackendInfoSessionPrefix)));
    QVERIFY(status(dialog)->text().contains(QStringLiteral("reported by hashcat")));
}

void TestBackendInfoDialog::eachQueryUsesItsOwnSession()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    QStringList first;
    QStringList second;
    BackendInfoDialog firstDialog(nullptr, cannedQuery(normalResult(), &first));
    BackendInfoDialog secondDialog(nullptr, cannedQuery(normalResult(), &second));
    QTRY_VERIFY_WITH_TIMEOUT(!first.isEmpty() && !second.isEmpty(), 5000);

    const auto session = [](const QStringList &args) {
        for (const QString &arg : args) {
            if (arg.startsWith(QStringLiteral("--session="))) {
                return arg;
            }
        }
        return QString();
    };
    QVERIFY(!session(first).isEmpty());
    QVERIFY(session(first) != session(second));
}

void TestBackendInfoDialog::anUnconfiguredPathStartsNoQuery()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, QString());
    int calls = 0;
    BackendInfoDialog dialog(nullptr, [&calls](const QString &, const QStringList &, int,
                                               const HashcatRunner::CancelToken &) {
        ++calls;
        return QtConcurrent::run([] { return HashcatResult{}; });
    });

    QCOMPARE(calls, 0);
    QVERIFY(status(dialog)->text().contains(QStringLiteral("is configured")));
}

void TestBackendInfoDialog::aMissingExecutableStartsNoQuery()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       dir.filePath(QStringLiteral("does-not-exist")));
    int calls = 0;
    BackendInfoDialog dialog(nullptr, [&calls](const QString &, const QStringList &, int,
                                               const HashcatRunner::CancelToken &) {
        ++calls;
        return QtConcurrent::run([] { return HashcatResult{}; });
    });

    QCOMPARE(calls, 0);
    QVERIFY(status(dialog)->text().contains(QStringLiteral("not found")));
}

void TestBackendInfoDialog::aProcessFailureIsReported()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    HashcatResult result;
    result.exitStatus = QProcess::NormalExit;
    result.exitCode = 1;
    result.standardError = QStringLiteral("backend exploded");
    BackendInfoDialog dialog(nullptr, cannedQuery(result));

    QTRY_VERIFY(status(dialog)->text().contains(QStringLiteral("backend exploded")));
}

void TestBackendInfoDialog::aTimeoutIsReported()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    BackendInfoDialog dialog(nullptr, cannedQuery(timedOutResult()));

    QTRY_VERIFY(status(dialog)->text().contains(QStringLiteral("timed out")));
}

void TestBackendInfoDialog::cancelStopsTheQuery()
{
    QTemporaryDir dir;
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, fakeProgram(dir));

    HashcatRunner::CancelToken token;
    BackendInfoDialog dialog(
        nullptr, cannedQuery(normalResult(QStringLiteral("too late")), nullptr, &token, 1000));
    auto *refresh = dialog.findChild<QPushButton *>(QStringLiteral("pushButton_refresh"));
    auto *cancel = dialog.findChild<QPushButton *>(QStringLiteral("pushButton_cancel"));
    QVERIFY(cancel->isEnabled());

    cancel->click();
    QVERIFY(token);
    QVERIFY(token->load());
    // Refresh stays disabled until the runner has killed and reaped its
    // process; a second query now would hit hashcat's pid lock.
    QVERIFY(!refresh->isEnabled());
    QVERIFY(!status(dialog)->text().contains(QStringLiteral("cancelled")));
    QTRY_VERIFY_WITH_TIMEOUT(status(dialog)->text().contains(QStringLiteral("cancelled")), 3000);
    QVERIFY(refresh->isEnabled());
}

int main(int argc, char *argv[])
{
    TestBackendInfoDialog test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_backendinfodialog.moc"
