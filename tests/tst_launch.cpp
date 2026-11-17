/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appconstants.h"
#include "commandbuilder.h"
#include "hashcatoptions.h"
#include "launchvalidator.h"
#include "mainwindow.h"
#include "mainwindowtestaccess.h"
#include "mainwindowtesthelpers.h"
#include "settingsmanager.h"
#include "terminalcatalog.h"
#include "testenvironment.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QClipboard>
#include <QComboBox>
#include <QCompleter>
#include <QDir>
#include <QDoubleSpinBox>
#include <QFile>
#include <QFileInfo>
#include <QGridLayout>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>
#include <QPointer>
#include <QRadioButton>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QSet>
#include <QSpinBox>
#include <QStandardPaths>
#include <QTimer>
#include <QToolTip>
#include <QWidget>
#include <utility>

using TestEnvironment::validateOptions;

// The launch half of the main window: validation through the widgets, error
// presentation, preview and clipboard.
class TestLaunch : public QObject
{
    Q_OBJECT

private slots:
    void init()
    {
        m_fixture.init();
    }
    void cleanup()
    {
        m_fixture.cleanup();
    }
    void commandPreviewFollowsWidgetChanges();
    void launchValidationFollowsTheWidgets();
    void pcfgCharsetNeedsAMask();
    void commandPreviewCarriesTheBinaryName();
    void previewUsesTheConfiguredTerminalsDialect();
    void thePreviewTooltipHoldsTheWholeCommand();
    void copyingTheCommandPutsItOnTheClipboard();
    void copyingThePowerShellScriptPutsItOnTheClipboard();

private:
    MainWindowFixture m_fixture;
};

void TestLaunch::commandPreviewFollowsWidgetChanges()
{
    MainWindow window;
    QLineEdit *preview = MainWindowTestAccess::ui(window)->lineEdit_command;

    QCOMPARE(preview->text(), QStringLiteral("--attack-mode 0"));

    // One representative for each connection helper.
    MainWindowTestAccess::ui(window)->checkBox_remove->setChecked(true);
    QVERIFY(preview->text().contains(QStringLiteral("--remove")));

    MainWindowTestAccess::ui(window)->checkBox_ignoreusername->setChecked(true);
    QVERIFY(preview->text().contains(QStringLiteral("--username")));

    MainWindowTestAccess::ui(window)->lineEdit_backend_devices->setText(QStringLiteral("1,2"));
    QVERIFY(preview->text().contains(QStringLiteral("--backend-devices 1,2")));

    MainWindowTestAccess::ui(window)->checkBox_hwmon_temp_abort->setChecked(true);
    MainWindowTestAccess::ui(window)->spinBox_hwmon_temp_abort->setValue(85);
    QVERIFY(preview->text().contains(QStringLiteral("--hwmon-temp-abort 85")));

    MainWindowTestAccess::ui(window)->lineEdit_session->setText(QStringLiteral("job"));
    QVERIFY(preview->text().contains(QStringLiteral("--session job")));

    MainWindowTestAccess::ui(window)->checkBox_force->setChecked(true);
    QVERIFY(preview->text().contains(QStringLiteral("--force")));

    MainWindowTestAccess::ui(window)->spinBox_runtime->setValue(60);
    QVERIFY(preview->text().contains(QStringLiteral("--runtime 60")));

    // Radio buttons share the toggle helper and keep their side-effect slot.
    MainWindowTestAccess::ui(window)->radioButton_generate_rules->setChecked(true);
    QVERIFY(preview->text().contains(QStringLiteral("--generate-rules 1")));

    // The hash combo is filled asynchronously in the application; fill it by
    // hand here to test the index connection without a hashcat query.
    MainWindowTestAccess::ui(window)->comboBox_hash_type->clear();
    MainWindowTestAccess::ui(window)->comboBox_hash_type->addItem(QStringLiteral("1000 | NTLM"),
                                                                  1000);
    QVERIFY(preview->text().contains(QStringLiteral("--hash-type 1000")));

    // The recipe belongs to hash mode 4000; this is the integration check that
    // its field reaches the preview at all. Its quoting is pinned in
    // tst_commandpreview.
    MainWindowTestAccess::ui(window)->comboBox_hash_type->addItem(QStringLiteral("4000 | recipe"),
                                                                  4000);
    MainWindowTestAccess::ui(window)->comboBox_hash_type->setCurrentIndex(
        MainWindowTestAccess::ui(window)->comboBox_hash_type->findData(4000));
    MainWindowTestAccess::ui(window)->lineEdit_hash_recipe->setText(QStringLiteral("md5($p)"));
    QVERIFY(preview->text().contains(QStringLiteral("--hash-recipe")));
    QVERIFY(preview->text().contains(QStringLiteral("md5($p)")));

    // The attack mode refreshes the preview through updateViewAttackMode(),
    // not through a second commandChanged connection.
    MainWindowTestAccess::ui(window)->comboBox_attack->setCurrentIndex(2); // brute-force
    QVERIFY(preview->text().contains(QStringLiteral("--attack-mode 3")));

    MainWindowTestAccess::ui(window)->lineEdit_mask->setText(QStringLiteral("?d?d"));
    QVERIFY(preview->text().contains(QStringLiteral("?d?d")));

    MainWindowTestAccess::ui(window)->checkBox_increment->setChecked(true);
    QVERIFY(preview->text().contains(QStringLiteral("--increment")));
}

void TestLaunch::launchValidationFollowsTheWidgets()
{
    MainWindow window;

    const auto addWordlist = [&window](const QString &path) {
        auto *item =
            new QListWidgetItem(path, MainWindowTestAccess::ui(window)->listWidget_wordlist);
        item->setCheckState(Qt::Checked);
    };

    // Straight mode without a word list is what the hint warns about.
    QCOMPARE(int(validateOptions(MainWindowTestAccess::collectHashcatOptions(window)).code),
             int(LaunchValidator::LaunchError::NoWordlist));

    addWordlist(QStringLiteral("/tmp/a.txt"));
    QCOMPARE(int(validateOptions(MainWindowTestAccess::collectHashcatOptions(window)).code),
             int(LaunchValidator::LaunchError::None));

    // Combination mode wants exactly two word lists.
    MainWindowTestAccess::ui(window)->comboBox_attack->setCurrentIndex(1); // combination
    QCOMPARE(int(MainWindowTestAccess::ui(window)->comboBox_attack->currentData().toInt()),
             int(AttackMode::Combination));
    QCOMPARE(int(validateOptions(MainWindowTestAccess::collectHashcatOptions(window)).code),
             int(LaunchValidator::LaunchError::WrongWordlistCount));

    addWordlist(QStringLiteral("/tmp/b.txt"));
    QCOMPARE(int(validateOptions(MainWindowTestAccess::collectHashcatOptions(window)).code),
             int(LaunchValidator::LaunchError::None));

    addWordlist(QStringLiteral("/tmp/c.txt"));
    QCOMPARE(int(validateOptions(MainWindowTestAccess::collectHashcatOptions(window)).code),
             int(LaunchValidator::LaunchError::WrongWordlistCount));

    // A mask mode needs a mask, and the bounds of the increment have to fit.
    MainWindowTestAccess::ui(window)->comboBox_attack->setCurrentIndex(2); // brute-force
    QCOMPARE(int(validateOptions(MainWindowTestAccess::collectHashcatOptions(window)).code),
             int(LaunchValidator::LaunchError::NoMask));

    MainWindowTestAccess::ui(window)->lineEdit_mask->setText(QStringLiteral("?d?d?d?d"));
    QCOMPARE(int(validateOptions(MainWindowTestAccess::collectHashcatOptions(window)).code),
             int(LaunchValidator::LaunchError::None));

    MainWindowTestAccess::ui(window)->checkBox_increment->setChecked(true);
    MainWindowTestAccess::ui(window)->spinBox_increment_min->setValue(8);
    MainWindowTestAccess::ui(window)->spinBox_increment_max->setValue(4);
    QCOMPARE(int(validateOptions(MainWindowTestAccess::collectHashcatOptions(window)).code),
             int(LaunchValidator::LaunchError::InvalidIncrement));

    // A valid pair of bounds, but incrementing and speed-only are mutually
    // exclusive in hashcat. Both controls are reachable in a mask mode.
    MainWindowTestAccess::ui(window)->spinBox_increment_min->setValue(4);
    MainWindowTestAccess::ui(window)->spinBox_increment_max->setValue(8);
    MainWindowTestAccess::ui(window)->checkBox_speed_only->setChecked(true);
    QCOMPARE(int(validateOptions(MainWindowTestAccess::collectHashcatOptions(window)).code),
             int(LaunchValidator::LaunchError::IncrementWithSpeedOnly));
    MainWindowTestAccess::ui(window)->checkBox_speed_only->setChecked(false);
    QCOMPARE(int(validateOptions(MainWindowTestAccess::collectHashcatOptions(window)).code),
             int(LaunchValidator::LaunchError::None));

    // Association keeps the checked increment from the brute-force step (its
    // group box is disabled), but the command line carries no -i, so the
    // launch validation has to ignore it. --remove is refused, and its box
    // stays enabled so the user can clear it.
    MainWindowTestAccess::ui(window)->comboBox_attack->setCurrentIndex(8); // association
    QCOMPARE(int(MainWindowTestAccess::ui(window)->comboBox_attack->currentData().toInt()),
             int(AttackMode::Association));

    // The three wordlists of the combination step are one too many for the
    // pairing; the cap takes the first one.
    MainWindowTestAccess::ui(window)->listWidget_wordlist->item(2)->setCheckState(Qt::Unchecked);
    MainWindowTestAccess::ui(window)->listWidget_wordlist->item(1)->setCheckState(Qt::Unchecked);
    QCOMPARE(int(validateOptions(MainWindowTestAccess::collectHashcatOptions(window)).code),
             int(LaunchValidator::LaunchError::None));

    MainWindowTestAccess::ui(window)->checkBox_remove->setChecked(true);
    QCOMPARE(int(validateOptions(MainWindowTestAccess::collectHashcatOptions(window)).code),
             int(LaunchValidator::LaunchError::RemoveInAssociation));
    MainWindowTestAccess::ui(window)->checkBox_remove->setChecked(false);
    QCOMPARE(int(validateOptions(MainWindowTestAccess::collectHashcatOptions(window)).code),
             int(LaunchValidator::LaunchError::None));
}

// A custom charset is used only inside a mask. PCFG takes its mask from the
// Mask field or from a mask= setting, so a charset without either is refused
// under its own error rather than as an invalid setting value.
void TestLaunch::pcfgCharsetNeedsAMask()
{
    MainWindow window;

    MainWindowTestAccess::ui(window)->comboBox_attack->setCurrentIndex(3); // PCFG
    QCOMPARE(int(MainWindowTestAccess::ui(window)->comboBox_attack->currentData().toInt()),
             int(AttackMode::Pcfg));

    MainWindowTestAccess::ui(window)->checkBox_custom_charset1->setChecked(true);
    MainWindowTestAccess::ui(window)->lineEdit_custom_charset1->setText(QStringLiteral("abc"));
    QCOMPARE(int(validateOptions(MainWindowTestAccess::collectHashcatOptions(window)).code),
             int(LaunchValidator::LaunchError::PcfgCharsetWithoutMask));

    // A mask that references ?1 gives the charset its place.
    MainWindowTestAccess::ui(window)->lineEdit_pcfg_mask->setText(QStringLiteral("?1?1?1"));
    QCOMPARE(int(validateOptions(MainWindowTestAccess::collectHashcatOptions(window)).code),
             int(LaunchValidator::LaunchError::None));

    // mask= on the extra settings line is the same setting.
    MainWindowTestAccess::ui(window)->lineEdit_pcfg_mask->clear();
    MainWindowTestAccess::ui(window)->lineEdit_pcfg_extra->setText(QStringLiteral("mask=?1?1?1"));
    QCOMPARE(int(validateOptions(MainWindowTestAccess::collectHashcatOptions(window)).code),
             int(LaunchValidator::LaunchError::None));

    // An unchecked charset is left out of the command, so no mask is needed.
    MainWindowTestAccess::ui(window)->lineEdit_pcfg_extra->clear();
    MainWindowTestAccess::ui(window)->checkBox_custom_charset1->setChecked(false);
    QCOMPARE(int(validateOptions(MainWindowTestAccess::collectHashcatOptions(window)).code),
             int(LaunchValidator::LaunchError::None));
}

// With a hashcat binary configured the preview is [binary][separator][args]:
// the file name, exactly one blank, the arguments - and nothing in front of
// them, a leading blank used to ride into the clipboard with the copy.
void TestLaunch::commandPreviewCarriesTheBinaryName()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("/opt/hashcat/hashcat"));

    MainWindow window;
    QCOMPARE(MainWindowTestAccess::ui(window)->lineEdit_command->text(),
             QStringLiteral("hashcat --attack-mode 0"));
}

// The copied command has to fit the shell it is pasted into: the configured
// terminal decides the dialect, not the platform this build runs on.
void TestLaunch::previewUsesTheConfiguredTerminalsDialect()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("/opt/hashcat/hashcat"));

    MainWindow window;
    QLineEdit *preview = MainWindowTestAccess::ui(window)->lineEdit_command;
    QCOMPARE(preview->text(), QStringLiteral("hashcat --attack-mode 0"));

    // pwsh is a PowerShell terminal: the call operator in front, every token
    // single-quoted, so a bcrypt hash or a subexpression stays literal.
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::Terminal,
                                       QStringLiteral("pwsh"));
    MainWindowTestAccess::ui(window)->lineEdit_session->setText(QStringLiteral("job"));
    QCOMPARE(preview->text(), QStringLiteral("& 'hashcat' '--attack-mode' '0' '--session' 'job'"));

    // A POSIX terminal brings the other dialect back.
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::Terminal,
                                       QStringLiteral("konsole"));
    MainWindowTestAccess::ui(window)->lineEdit_session->setText(QStringLiteral("job2"));
    QCOMPARE(preview->text(), QStringLiteral("hashcat --attack-mode 0 --session job2"));
}

// The line edit shows the beginning of the command only; the tooltip has to
// carry all of it. Escaping and wrapping are pinned in tst_commandpreview.
void TestLaunch::thePreviewTooltipHoldsTheWholeCommand()
{
    MainWindow window;
    auto *ui = MainWindowTestAccess::ui(window);

    for (int i = 0; i < 12; ++i) {
        auto *item = new QListWidgetItem(QStringLiteral("/tmp/wordlist-number-%1.txt").arg(i),
                                         ui->listWidget_wordlist);
        item->setCheckState(Qt::Checked);
    }
    ui->checkBox_outfile->setChecked(true);
    ui->lineEdit_outfile->setText(QStringLiteral("/tmp/out.txt"));

    const QString command = ui->lineEdit_command->text();
    const QString tooltip = ui->lineEdit_command->toolTip();
    const QStringList parts = command.split(QLatin1Char(' '));
    for (const QString &part : std::as_const(parts)) {
        QVERIFY2(tooltip.contains(part),
                 qPrintable(QStringLiteral("the tooltip does not hold \"%1\"").arg(part)));
    }
}

// Copying puts the command on the clipboard and says that it did.
void TestLaunch::copyingTheCommandPutsItOnTheClipboard()
{
    MainWindow window;
    MainWindowTestAccess::ui(window)->checkBox_remove->setChecked(true);

    const QString command = MainWindowTestAccess::ui(window)->lineEdit_command->text();
    QVERIFY(!command.isEmpty());

    QVERIFY(QMetaObject::invokeMethod(&window, "copyCommandToClipboard"));

    QCOMPARE(QApplication::clipboard()->text(), command);

    // The confirmation is a one-shot tooltip at the pointer.
    QVERIFY(QToolTip::text().contains(QStringLiteral("Copied")));
    QToolTip::hideText();
}

// The Tools action copies the readable script the PowerShell launch encodes,
// and it is reachable from the Tools menu.
void TestLaunch::copyingThePowerShellScriptPutsItOnTheClipboard()
{
    const QString stub = TestEnvironment::writeShellStub(QStringLiteral("stub-hashcat-script-copy"),
                                                         "#!/bin/sh\nexit 0\n");
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    MainWindow window;
    Ui::MainWindow *ui = MainWindowTestAccess::ui(window);
    QVERIFY2(ui->menuTools->actions().contains(ui->actionCopyPowerShellScript),
             "the action is not in the Tools menu");

    QApplication::clipboard()->clear();
    ui->actionCopyPowerShellScript->trigger();

    const QString expected = TerminalCatalog::powerShellScript(
        stub, CommandBuilder::build(MainWindowTestAccess::collectHashcatOptions(window)));
    QCOMPARE(QApplication::clipboard()->text(), expected);
    QVERIFY(
        expected.contains(QStringLiteral("$si = New-Object System.Diagnostics.ProcessStartInfo")));
    QVERIFY(expected.contains(stub));
}

int main(int argc, char *argv[])
{
    TestLaunch test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_launch.moc"
