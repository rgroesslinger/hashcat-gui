/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appconstants.h"
#include "checkeditemlist.h"
#include "hashcatoptions.h"
#include "launchvalidator.h"
#include "mainwindow.h"
#include "mainwindowtestaccess.h"
#include "mainwindowtesthelpers.h"
#include "settingsmanager.h"
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

namespace
{
// The option checks on their own: the launch fields hold values that pass,
// so only the options can fail. validate() runs the option checks first,
// which is the precedence these tests pin.
LaunchValidator::LaunchProblem validateOptions(const HashcatOptions &options)
{
    LaunchValidator::LaunchContext context;
    context.options = options;
    context.options.hashFile = QStringLiteral("hash.txt");
    context.hashcatPath = QStringLiteral("hashcat");
    context.configuredTerminal = QStringLiteral("konsole");
    context.availableTerminals = QStringList{QStringLiteral("konsole")};
    return LaunchValidator::validate(context);
}

} // namespace

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
    void thePreviewTooltipHoldsTheWholeCommand();
    void copyingTheCommandPutsItOnTheClipboard();
    void thePreviewFormattingIsPinned();

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

// The line edit shows the beginning of the command only; the tooltip has to
// carry all of it.
void TestLaunch::thePreviewTooltipHoldsTheWholeCommand()
{
    MainWindow window;

    // More word lists than the window can show, and a path that looks like a
    // tag.
    for (int i = 0; i < 12; ++i) {
        auto *item = new QListWidgetItem(QStringLiteral("/tmp/wordlist-number-%1.txt").arg(i),
                                         MainWindowTestAccess::ui(window)->listWidget_wordlist);
        item->setCheckState(Qt::Checked);
    }
    MainWindowTestAccess::ui(window)->checkBox_outfile->setChecked(true);
    MainWindowTestAccess::ui(window)->lineEdit_outfile->setText(
        QStringLiteral("/tmp/<b>out</b>.txt"));

    const QString command = MainWindowTestAccess::ui(window)->lineEdit_command->text();
    const QString tooltip = MainWindowTestAccess::ui(window)->lineEdit_command->toolTip();

    // Every part of the command is in the tooltip. The path that looks like a
    // tag is escaped, so that it is shown as typed.
    const QStringList parts = command.split(QLatin1Char(' '));
    for (const QString &part : std::as_const(parts)) {
        QVERIFY2(tooltip.contains(part.toHtmlEscaped()),
                 qPrintable(QStringLiteral("the tooltip does not hold \"%1\"").arg(part)));
    }

    QVERIFY(tooltip.contains(QStringLiteral("&lt;b&gt;out&lt;/b&gt;.txt")));

    // A command that looks like markup becomes rich text, where a line break
    // has to be explicit.
    QVERIFY(tooltip.contains(QStringLiteral("<br>")));

    // Without markup the tooltip stays plain text and keeps its line breaks.
    MainWindowTestAccess::ui(window)->lineEdit_outfile->setText(QStringLiteral("/tmp/out.txt"));
    const QString plainTooltip = MainWindowTestAccess::ui(window)->lineEdit_command->toolTip();
    QVERIFY(plainTooltip.contains(QStringLiteral("/tmp/out.txt")));
    QVERIFY(plainTooltip.contains(QLatin1Char('\n')));
    QVERIFY(!plainTooltip.contains(QStringLiteral("<br>")));
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

// The preview's display formatting, pinned before it moves to its own unit:
// every token the shell would split is quoted for display, markup-like text
// is escaped in the tooltip, and the tooltip wraps at 120 columns.
void TestLaunch::thePreviewFormattingIsPinned()
{
    const QString stub = TestEnvironment::writeShellStub(QStringLiteral("stub-hashcat-preview"),
                                                         "#!/bin/sh\n"
                                                         "printf '{\"0\":{\"name\":\"MD5\"},"
                                                         "\"4000\":{\"name\":\"recipe\"}}\\n'\n"
                                                         "exit 0\n");
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    MainWindow window;
    QTRY_VERIFY_WITH_TIMEOUT(MainWindowTestAccess::ui(window)->comboBox_hash_type->count() == 2,
                             5000);

    // A recipe with spaces, quotes and markup: the command line quotes it for
    // display - escaping the recipe's own quotes - and the tooltip escapes
    // the markup.
    MainWindowTestAccess::ui(window)->comboBox_hash_type->setCurrentIndex(
        MainWindowTestAccess::ui(window)->comboBox_hash_type->findData(4000));
    MainWindowTestAccess::ui(window)->lineEdit_hash_recipe->setText(
        QStringLiteral("md5($p.\"<b>\")"));
#ifdef Q_OS_WIN
    // The recipe holds no whitespace, so the MSVCRT quoting escapes its
    // double quotes but does not wrap it.
    const QString quotedRecipe = QStringLiteral("md5($p.\\\"<b>\\\")");
    const QString quotedRecipeTooltip = QStringLiteral("md5($p.\\&quot;&lt;b&gt;\\&quot;)");
#else
    const QString quotedRecipe = QStringLiteral("'md5($p.\"<b>\")'");
    const QString quotedRecipeTooltip = QStringLiteral("'md5($p.&quot;&lt;b&gt;&quot;)'");
#endif
    const QString recipeCommand = QStringLiteral("stub-hashcat-preview --hash-type 4000 "
                                                 "--hash-recipe ")
                                  + quotedRecipe + QStringLiteral(" --attack-mode 0");
    QCOMPARE(MainWindowTestAccess::ui(window)->lineEdit_command->text(), recipeCommand);
    QCOMPARE(MainWindowTestAccess::ui(window)->lineEdit_command->toolTip(),
             QStringLiteral("stub-hashcat-preview --hash-type 4000 --hash-recipe ")
                 + quotedRecipeTooltip + QStringLiteral(" --attack-mode 0"));

    // Two long positional wordlists: the tooltip breaks between arguments at
    // 120 columns, a flag keeps its value on the same line.
    MainWindowTestAccess::ui(window)->comboBox_hash_type->setCurrentIndex(
        MainWindowTestAccess::ui(window)->comboBox_hash_type->findData(0));
    const QString pathA = QString(80, QLatin1Char('a'));
    const QString pathB = QString(80, QLatin1Char('b'));
    CheckedItemList::addPaths(MainWindowTestAccess::ui(window)->listWidget_wordlist,
                              {pathA, pathB});
    const QString wrappedCommand = QStringLiteral("stub-hashcat-preview --hash-type 0 "
                                                  "--attack-mode 0 ")
                                   + pathA + QLatin1Char(' ') + pathB;
    QCOMPARE(MainWindowTestAccess::ui(window)->lineEdit_command->text(), wrappedCommand);
    QCOMPARE(MainWindowTestAccess::ui(window)->lineEdit_command->toolTip(),
             QStringLiteral("stub-hashcat-preview --hash-type 0 --attack-mode 0\n") + pathA
                 + QLatin1Char('\n') + pathB);
}

int main(int argc, char *argv[])
{
    TestLaunch test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_launch.moc"
