/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appconstants.h"
#include "checkeditemlist.h"
#include "commandbuilder.h"
#include "optioncollector.h"
#include "testenvironment.h"
#include "ui_mainwindow.h"

#include <QComboBox>
#include <QDir>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QRadioButton>
#include <QSpinBox>

// The widget state read into HashcatOptions: the defaults, one non-default
// per group, and the rules the read itself applies (checked rows only,
// trimmed feed values, the empty hash combo).
class TestOptionCollector : public QObject
{
    Q_OBJECT

private slots:
    void defaultsMatchTheWidgetStartValues();
    void theShortParameterChoiceIsPassedThrough();
    void anEmptyHashComboMeansNoHashType();
    void theHashAndRuleGroupsAreRead();
    void theMaskAndCharsetGroupsAreRead();
    void theFeedValuesAreTrimmedAndSplit();
    void theGeneralGroupIsRead();
    void onlyCheckedRowsReachTheOptions();

private:
    // A fresh form, as the constructor leaves it before any query.
    struct Form {
        QMainWindow window;
        Ui::MainWindow ui;

        Form() : ui{}
        {
            ui.setupUi(&window);
        }
    };
};

void TestOptionCollector::defaultsMatchTheWidgetStartValues()
{
    Form form;
    const HashcatOptions options = OptionCollector::collect(form.ui, false);

    QCOMPARE(options.hashType, -1);
    QCOMPARE(options.attackMode, AttackMode::Straight);
    QVERIFY(options.wordlists.isEmpty());
    QVERIFY(options.rulesFiles.isEmpty());
    QCOMPARE(options.generateRules, 0);
    QCOMPARE(options.useShortParameters, false);
    QCOMPARE(options.outfileFormat, QString(AppConstants::Defaults::OutfileFormat));
    QCOMPARE(options.runtime, AppConstants::Defaults::Runtime);
    QCOMPARE(options.statusTimer, AppConstants::Defaults::StatusTimer);
    QCOMPARE(options.hwmonTempAbortCelsius, AppConstants::Defaults::HwmonTempAbort);

    // Ground truth spelled out: the defaults produce the bare command line.
    const QStringList expected{QStringLiteral("--attack-mode"), QStringLiteral("0")};
    QCOMPARE(CommandBuilder::build(options), expected);
}

void TestOptionCollector::theShortParameterChoiceIsPassedThrough()
{
    Form form;
    QCOMPARE(OptionCollector::collect(form.ui, true).useShortParameters, true);
    QCOMPARE(OptionCollector::collect(form.ui, false).useShortParameters, false);
    const QStringList expected{QStringLiteral("-a"), QStringLiteral("0")};
    QCOMPARE(CommandBuilder::build(OptionCollector::collect(form.ui, true)), expected);
}

void TestOptionCollector::anEmptyHashComboMeansNoHashType()
{
    Form form;
    // currentData() is invalid while the list has not been filled: -m must
    // stay out instead of asking for hash type 0.
    QCOMPARE(OptionCollector::collect(form.ui, false).hashType, -1);

    form.ui.comboBox_hash_type->addItem(QStringLiteral("1000 | NTLM"), 1000);
    form.ui.comboBox_hash_type->setCurrentIndex(0);
    QCOMPARE(OptionCollector::collect(form.ui, false).hashType, 1000);
}

void TestOptionCollector::theHashAndRuleGroupsAreRead()
{
    Form form;
    form.ui.comboBox_hash_type->addItem(QStringLiteral("4000 | recipe"), 4000);
    form.ui.comboBox_hash_type->setCurrentIndex(0);
    form.ui.lineEdit_hash_recipe->setText(QStringLiteral("md5($p.$s)"));
    form.ui.lineEdit_hashfile->setText(QStringLiteral("/tmp/hashes.txt"));

    form.ui.checkBox_rulesfile_1->setChecked(true);
    form.ui.lineEdit_rulesfile_1->setText(QStringLiteral("/tmp/r1.rule"));
    form.ui.checkBox_rulesfile_2->setChecked(true);
    // An empty field next to a ticked box contributes no file.
    form.ui.checkBox_rules_concat->setChecked(true);

    HashcatOptions options = OptionCollector::collect(form.ui, false);
    QCOMPARE(options.hashType, 4000);
    QCOMPARE(options.hashRecipe, QStringLiteral("md5($p.$s)"));
    QCOMPARE(options.hashFile, QStringLiteral("/tmp/hashes.txt"));
    QCOMPARE(options.rulesFiles, QStringList{QStringLiteral("/tmp/r1.rule")});
    QCOMPARE(options.rulesConcat, true);
    QCOMPARE(options.generateRules, 0);

    form.ui.radioButton_generate_rules->setChecked(true);
    form.ui.spinBox_generate_rules->setValue(500);
    options = OptionCollector::collect(form.ui, false);
    QCOMPARE(options.generateRules, 500);
    QVERIFY(options.rulesFiles.isEmpty());
}

void TestOptionCollector::theMaskAndCharsetGroupsAreRead()
{
    Form form;
    form.ui.comboBox_attack->addItem(QStringLiteral("Brute-force"),
                                     static_cast<int>(AttackMode::BruteForce));
    form.ui.comboBox_attack->setCurrentIndex(0);
    form.ui.lineEdit_mask->setText(QStringLiteral("?d?d?d?d"));
    form.ui.checkBox_increment->setChecked(true);
    form.ui.spinBox_increment_min->setValue(2);
    form.ui.spinBox_increment_max->setValue(6);
    form.ui.checkBox_custom_charset1->setChecked(true);
    form.ui.lineEdit_custom_charset1->setText(QStringLiteral("?l"));
    form.ui.checkBox_hex_charset->setChecked(true);
    form.ui.checkBox_hex_salt->setChecked(true);

    const HashcatOptions options = OptionCollector::collect(form.ui, false);
    QCOMPARE(options.attackMode, AttackMode::BruteForce);
    QCOMPARE(options.mask, QStringLiteral("?d?d?d?d"));
    QCOMPARE(options.increment, true);
    QCOMPARE(options.incrementMin, 2);
    QCOMPARE(options.incrementMax, 6);
    QCOMPARE(options.customCharsets.at(0).enabled, true);
    QCOMPARE(options.customCharsets.at(0).value, QStringLiteral("?l"));
    QCOMPARE(options.customCharsets.at(1).enabled, false);
    QCOMPARE(options.hexCharset, true);
    QCOMPARE(options.hexSalt, true);

    // A mode that refuses -i drops a checked increment box.
    form.ui.comboBox_attack->addItem(QStringLiteral("Straight"),
                                     static_cast<int>(AttackMode::Straight));
    form.ui.comboBox_attack->setCurrentIndex(1);
    QCOMPARE(OptionCollector::collect(form.ui, false).increment, false);
}

void TestOptionCollector::theFeedValuesAreTrimmedAndSplit()
{
    Form form;
    form.ui.lineEdit_pcfg_scale->setText(QStringLiteral(" 8 "));
    form.ui.lineEdit_pcfg_extra->setText(QStringLiteral("omen=1  hintaccount=2"));
    form.ui.lineEdit_table_cap->setText(QStringLiteral(" 1000 "));
    form.ui.lineEdit_table_extra->setText(QStringLiteral("substitutions=1"));
    form.ui.lineEdit_feed->setText(QStringLiteral(" wordlist "));
    form.ui.lineEdit_association_settings->setText(QStringLiteral("phases=words rules"));

    const HashcatOptions options = OptionCollector::collect(form.ui, false);
    QCOMPARE(options.pcfgScale, QStringLiteral("8"));
    const QStringList pcfgExtra{QStringLiteral("omen=1"), QStringLiteral("hintaccount=2")};
    QCOMPARE(options.pcfgExtraSettings, pcfgExtra);
    QCOMPARE(options.tableCap, QStringLiteral("1000"));
    QCOMPARE(options.tableExtraSettings, QStringList{QStringLiteral("substitutions=1")});
    QCOMPARE(options.feedName, QStringLiteral("wordlist"));
    const QStringList association{QStringLiteral("phases=words"), QStringLiteral("rules")};
    QCOMPARE(options.associationSettings, association);
}

void TestOptionCollector::theGeneralGroupIsRead()
{
    Form form;
    form.ui.checkBox_outfile->setChecked(true);
    form.ui.lineEdit_outfile->setText(QStringLiteral("/tmp/out.txt"));
    form.ui.lineEdit_outfile_format->setText(QStringLiteral("2,3"));
    form.ui.checkBox_outfile_json->setChecked(true);
    form.ui.lineEdit_cache_path->setText(QStringLiteral("/tmp/cache"));
    form.ui.lineEdit_cpu_affinity->setText(QStringLiteral("0,1"));
    form.ui.lineEdit_backend_devices->setText(QStringLiteral("1"));
    form.ui.lineEdit_session->setText(QStringLiteral("run"));
    form.ui.spinBox_runtime->setValue(60);
    form.ui.lineEdit_skip->setText(QStringLiteral("10"));
    form.ui.lineEdit_limit->setText(QStringLiteral("100"));
    form.ui.checkBox_status->setChecked(true);
    form.ui.spinBox_status_timer->setValue(30);
    form.ui.checkBox_force->setChecked(true);
    form.ui.checkBox_potfile_disable->setChecked(true);
    form.ui.checkBox_optimized_kernel->setChecked(true);
    form.ui.checkBox_speed_only->setChecked(true);
    form.ui.lineEdit_opencl_device_types->setText(QStringLiteral("1,2"));
    form.ui.checkBox_hwmon_temp_abort->setChecked(true);
    form.ui.spinBox_hwmon_temp_abort->setValue(85);
    form.ui.checkBox_remove->setChecked(true);
    form.ui.checkBox_ignoreusername->setChecked(true);

    const HashcatOptions options = OptionCollector::collect(form.ui, false);
    QCOMPARE(options.outfileEnabled, true);
    QCOMPARE(options.outfile, QStringLiteral("/tmp/out.txt"));
    QCOMPARE(options.outfileFormat, QStringLiteral("2,3"));
    QCOMPARE(options.outfileJson, true);
    QCOMPARE(options.cachePath, QStringLiteral("/tmp/cache"));
    QCOMPARE(options.cpuAffinity, QStringLiteral("0,1"));
    QCOMPARE(options.backendDevices, QStringLiteral("1"));
    QCOMPARE(options.session, QStringLiteral("run"));
    QCOMPARE(options.runtime, 60);
    QCOMPARE(options.skip, QStringLiteral("10"));
    QCOMPARE(options.limit, QStringLiteral("100"));
    QCOMPARE(options.status, true);
    QCOMPARE(options.statusTimer, 30);
    QCOMPARE(options.force, true);
    QCOMPARE(options.potfileDisable, true);
    QCOMPARE(options.optimizedKernel, true);
    QCOMPARE(options.speedOnly, true);
    QCOMPARE(options.openclDeviceTypes, QStringLiteral("1,2"));
    QCOMPARE(options.hwmonTempAbort, true);
    QCOMPARE(options.hwmonTempAbortCelsius, 85);
    QCOMPARE(options.remove, true);
    QCOMPARE(options.ignoreUsername, true);
}

void TestOptionCollector::onlyCheckedRowsReachTheOptions()
{
    Form form;
    CheckedItemList::addPaths(form.ui.listWidget_wordlist,
                              {QStringLiteral("/tmp/a.txt"), QStringLiteral("/tmp/b.txt")});
    form.ui.listWidget_wordlist->item(1)->setCheckState(Qt::Unchecked);
    CheckedItemList::addPaths(form.ui.listWidget_feed_arguments, {QStringLiteral("arg")});

    const HashcatOptions options = OptionCollector::collect(form.ui, false);
    QCOMPARE(options.wordlists,
             QStringList{QDir::toNativeSeparators(QStringLiteral("/tmp/a.txt"))});
    QCOMPARE(options.feedArguments, QStringList{QStringLiteral("arg")});
}

int main(int argc, char *argv[])
{
    TestOptionCollector test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_optioncollector.moc"