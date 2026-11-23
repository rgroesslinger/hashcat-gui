/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appconstants.h"
#include "hashcatquerytesthelpers.h"
#include "mainwindow.h"
#include "mainwindowtestaccess.h"
#include "mainwindowtesthelpers.h"
#include "potfilecheck.h"
#include "potfileresultsdialog.h"
#include "potfiletesthelpers.h"
#include "programdirectory.h"
#include "settingsmanager.h"
#include "testenvironment.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QProgressDialog>
#include <QTemporaryDir>
#include <QThread>
#include <QTimer>
#include <QTreeWidget>
#include <QtConcurrent/QtConcurrentRun>

namespace
{

PotfileCheck *installCheck(MainWindow &window, const HashcatResult &show, const HashcatResult &left,
                           int delayMs = 0, QList<QStringList> *args = nullptr)
{
    auto query = [=](const QString &, const QStringList &used, int,
                     const HashcatRunner::CancelToken &token) {
        if (args) {
            args->append(used);
        }
        const bool isLeft = used.contains(QStringLiteral("--left"));
        return QtConcurrent::run([=] {
            if (delayMs > 0) {
                QThread::msleep(delayMs);
            }
            HashcatResult result = isLeft ? left : show;
            if (token && token->load()) {
                result.cancelled = true;
            }
            return result;
        });
    };

    auto check = std::make_unique<PotfileCheck>(nullptr, query);
    PotfileCheck *pointer = check.get();
    MainWindowTestAccess::setPotfileCheck(window, std::move(check));
    return pointer;
}

struct CapturedUi {
    QString message;
    bool plainTextMessage = false;
    bool sawResultsDialog = false;
    int resultRows = 0;
    QString resultStatus;
};

// Closes the active modal so a test cannot hang. The progress dialog is left
// alone unless the test asks to cancel; message boxes and the results dialog
// are recorded.
CapturedUi triggerAndCapture(MainWindow &window, bool cancel, bool *sawDisabled = nullptr)
{
    CapturedUi captured;
    QTimer timer;
    timer.setInterval(20);
    QObject::connect(&timer, &QTimer::timeout, &timer, [&] {
        QWidget *modal = QApplication::activeModalWidget();
        if (!modal) {
            return;
        }
        if (auto *progress = qobject_cast<QProgressDialog *>(modal)) {
            if (sawDisabled) {
                *sawDisabled = !MainWindowTestAccess::ui(window)->actionPotfileCheck->isEnabled();
            }
            if (cancel) {
                progress->cancel();
            }
            return;
        }
        if (auto *box = qobject_cast<QMessageBox *>(modal)) {
            captured.message = box->text();
            captured.plainTextMessage = box->textFormat() == Qt::PlainText;
            box->close();
            return;
        }
        if (auto *results = qobject_cast<PotfileResultsDialog *>(modal)) {
            captured.sawResultsDialog = true;
            auto *tree = results->findChild<QTreeWidget *>(QStringLiteral("treeWidget_results"));
            captured.resultRows = tree ? tree->topLevelItemCount() : 0;
            auto *label = results->findChild<QLabel *>(QStringLiteral("label_status"));
            captured.resultStatus = label ? label->text() : QString();
            results->close();
        }
    });

    timer.start();
    MainWindowTestAccess::ui(window)->actionPotfileCheck->trigger();
    timer.stop();
    return captured;
}

} // namespace

class TestPotfilePreflight : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void emptyInputIsRefusedWithoutAQuery();
    void disabledPotfileIsRefusedWithoutAQuery();
    void invalidHashcatPathIsRefusedWithoutAQuery();
    void whitespaceAroundAnInlineHashIsTrimmed();
    void whitespaceAroundAFilePathIsTrimmed();
    void relativePotfilePathUsesTheHashcatDirectory();
    void successOpensTheResultsDialog();
    void failedQueryShowsTheCombinedDiagnostic();
    void leftFailureShowsRemainingUnknown();
    void cancelReenablesTheAction();

private:
    MainWindowFixture m_fixture;
    QString m_stub;
};

void TestPotfilePreflight::init()
{
    m_fixture.init();
    m_stub = hashcatStub(QStringLiteral("hashcat-potfile"));
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, m_stub);
}

void TestPotfilePreflight::cleanup()
{
    m_fixture.cleanup();
}

void TestPotfilePreflight::emptyInputIsRefusedWithoutAQuery()
{
    if (m_stub.isEmpty()) {
        QSKIP("A shell stub cannot run on this platform.");
    }

    MainWindow window;
    QList<QStringList> args;
    installCheck(window, normalResult(showLine()), normalResult(), 0, &args);

    const CapturedUi captured = triggerAndCapture(window, false);
    QVERIFY(captured.message.contains(QStringLiteral("Enter a hash")));
    QVERIFY(args.isEmpty());
}

void TestPotfilePreflight::disabledPotfileIsRefusedWithoutAQuery()
{
    if (m_stub.isEmpty()) {
        QSKIP("A shell stub cannot run on this platform.");
    }

    MainWindow window;
    auto *ui = MainWindowTestAccess::ui(window);
    ui->lineEdit_hashfile->setText(QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
    ui->checkBox_potfile_disable->setChecked(true);

    QList<QStringList> args;
    installCheck(window, normalResult(showLine()), normalResult(), 0, &args);

    const CapturedUi captured = triggerAndCapture(window, false);
    QVERIFY(captured.message.contains(QStringLiteral("potfile is disabled")));
    QVERIFY(args.isEmpty());
}

void TestPotfilePreflight::invalidHashcatPathIsRefusedWithoutAQuery()
{
    if (m_stub.isEmpty()) {
        QSKIP("A shell stub cannot run on this platform.");
    }

    MainWindow window;
    MainWindowTestAccess::ui(window)->lineEdit_hashfile->setText(
        QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       dir.filePath(QStringLiteral("missing-hashcat")));

    QList<QStringList> args;
    installCheck(window, normalResult(showLine()), normalResult(), 0, &args);

    const CapturedUi captured = triggerAndCapture(window, false);
    QVERIFY(captured.message.contains(QStringLiteral("not found")));
    QVERIFY(args.isEmpty());
}

void TestPotfilePreflight::whitespaceAroundAnInlineHashIsTrimmed()
{
    if (m_stub.isEmpty()) {
        QSKIP("A shell stub cannot run on this platform.");
    }

    const QString hash = QStringLiteral("5d41402abc4b2a76b9719d911017c592");
    MainWindow window;
    MainWindowTestAccess::ui(window)->lineEdit_hashfile->setText(QStringLiteral("  ") + hash
                                                                 + QStringLiteral("  "));

    QList<QStringList> args;
    installCheck(window, normalResult(showLine()), normalResult(QStringLiteral("hash-left\n")), 0,
                 &args);

    const CapturedUi captured = triggerAndCapture(window, false);
    QVERIFY(captured.sawResultsDialog);
    QVERIFY(captured.resultStatus.contains(QStringLiteral("1 of 2")));
    QCOMPARE(args.size(), 2);
    QCOMPARE(args.at(0).last(), hash);
    QCOMPARE(args.at(1).last(), hash);
}

void TestPotfilePreflight::whitespaceAroundAFilePathIsTrimmed()
{
    if (m_stub.isEmpty()) {
        QSKIP("A shell stub cannot run on this platform.");
    }

    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashes.txt"));
    QFile file(path);
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.close();

    MainWindow window;
    MainWindowTestAccess::ui(window)->lineEdit_hashfile->setText(QStringLiteral("  ") + path
                                                                 + QStringLiteral("  "));

    QList<QStringList> args;
    installCheck(window, normalResult(showLine()), normalResult(QStringLiteral("hash-left\n")), 0,
                 &args);

    triggerAndCapture(window, false);
    QCOMPARE(args.size(), 2);
    QCOMPARE(args.at(0).last(), QFileInfo(path).absoluteFilePath());
    QCOMPARE(args.at(1).last(), QFileInfo(path).absoluteFilePath());
}

void TestPotfilePreflight::relativePotfilePathUsesTheHashcatDirectory()
{
    if (m_stub.isEmpty()) {
        QSKIP("A shell stub cannot run on this platform.");
    }

    MainWindow window;
    auto *ui = MainWindowTestAccess::ui(window);
    ui->lineEdit_hashfile->setText(QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
    ui->lineEdit_potfile_path->setText(QStringLiteral("relative.potfile"));

    QList<QStringList> args;
    installCheck(window, normalResult(showLine()), normalResult(), 0, &args);
    triggerAndCapture(window, false);

    QCOMPARE(args.size(), 2);
    const qsizetype index = args.at(0).indexOf(QStringLiteral("--potfile-path"));
    QVERIFY(index >= 0);
    QVERIFY(index + 1 < args.at(0).size());
    QCOMPARE(
        args.at(0).at(index + 1),
        QDir(ProgramDirectory::forProgram(m_stub)).filePath(QStringLiteral("relative.potfile")));
}

void TestPotfilePreflight::successOpensTheResultsDialog()
{
    if (m_stub.isEmpty()) {
        QSKIP("A shell stub cannot run on this platform.");
    }

    MainWindow window;
    MainWindowTestAccess::ui(window)->lineEdit_hashfile->setText(
        QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
    installCheck(window, normalResult(showLine()), normalResult(QStringLiteral("hash-left\n")));

    const CapturedUi captured = triggerAndCapture(window, false);
    QVERIFY(captured.message.isEmpty());
    QVERIFY(captured.sawResultsDialog);
    QCOMPARE(captured.resultRows, 1);
    QVERIFY(captured.resultStatus.contains(QStringLiteral("1 of 2")));
    QVERIFY(MainWindowTestAccess::ui(window)->actionPotfileCheck->isEnabled());
    QVERIFY(!MainWindowTestAccess::potfileCheck(window).isRunning());
}

void TestPotfilePreflight::failedQueryShowsTheCombinedDiagnostic()
{
    if (m_stub.isEmpty()) {
        QSKIP("A shell stub cannot run on this platform.");
    }

    MainWindow window;
    MainWindowTestAccess::ui(window)->lineEdit_hashfile->setText(
        QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
    installCheck(window,
                 failedResult(244, QStringLiteral("Hash parsing error in hashfile: x"),
                              QStringLiteral("No hashes loaded.")),
                 normalResult());

    const CapturedUi captured = triggerAndCapture(window, false);
    QVERIFY(!captured.sawResultsDialog);
    QVERIFY(captured.message.contains(QStringLiteral("Hash parsing error")));
    QVERIFY(captured.message.contains(QStringLiteral("No hashes loaded.")));
    QVERIFY(captured.plainTextMessage);
}

void TestPotfilePreflight::leftFailureShowsRemainingUnknown()
{
    if (m_stub.isEmpty()) {
        QSKIP("A shell stub cannot run on this platform.");
    }

    MainWindow window;
    MainWindowTestAccess::ui(window)->lineEdit_hashfile->setText(
        QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
    installCheck(window, normalResult(showLine()),
                 failedResult(1, QString(), QStringLiteral("left failed")));

    const CapturedUi captured = triggerAndCapture(window, false);
    QVERIFY(captured.message.isEmpty());
    QVERIFY(captured.sawResultsDialog);
    QVERIFY(captured.resultStatus.contains(QStringLiteral("remaining unknown")));
    QVERIFY(captured.resultStatus.contains(QStringLiteral("1")));
}

void TestPotfilePreflight::cancelReenablesTheAction()
{
    if (m_stub.isEmpty()) {
        QSKIP("A shell stub cannot run on this platform.");
    }

    MainWindow window;
    MainWindowTestAccess::ui(window)->lineEdit_hashfile->setText(
        QStringLiteral("5d41402abc4b2a76b9719d911017c592"));
    installCheck(window, normalResult(showLine()), normalResult(QStringLiteral("hash-left\n")),
                 1000);

    bool sawDisabled = false;
    const CapturedUi captured = triggerAndCapture(window, true, &sawDisabled);

    QVERIFY(sawDisabled);
    QVERIFY(!captured.sawResultsDialog);
    QVERIFY(captured.message.contains(QStringLiteral("cancelled"), Qt::CaseInsensitive));
    QVERIFY(MainWindowTestAccess::ui(window)->actionPotfileCheck->isEnabled());
}

int main(int argc, char *argv[])
{
    TestPotfilePreflight test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_potfilepreflight.moc"