/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "optioncollector.h"

#include "appconstants.h"
#include "checkeditemlist.h"
#include "commandbuilder.h"

#include <QComboBox>
#include <QLineEdit>
#include <QListWidget>
#include <QRadioButton>
#include <QRegularExpression>
#include <QSpinBox>
#include <array>

HashcatOptions OptionCollector::collect(const Ui::MainWindow &ui, bool useShortParameters)
{
    HashcatOptions options;
    options.useShortParameters = useShortParameters;

    // currentData() is invalid while the hash type list has not been filled -
    // no hashcat configured, or the query failed. CommandBuilder then leaves
    // -m out entirely instead of asking for hash type 0.
    const QVariant hashType = ui.comboBox_hash_type->currentData();
    options.hashType = hashType.isValid() ? hashType.toInt() : -1;
    options.hashRecipe = ui.lineEdit_hash_recipe->text();

    options.attackMode = static_cast<AttackMode>(ui.comboBox_attack->currentData().toInt());

    options.remove = ui.checkBox_remove->isChecked();
    options.ignoreUsername = ui.checkBox_ignoreusername->isChecked();

    options.force = ui.checkBox_force->isChecked();
    options.session = ui.lineEdit_session->text();
    options.runtime = ui.spinBox_runtime->value();
    options.skip = ui.lineEdit_skip->text();
    options.limit = ui.lineEdit_limit->text();
    options.status = ui.checkBox_status->isChecked();
    options.statusTimer = ui.spinBox_status_timer->value();
    options.potfileDisable = ui.checkBox_potfile_disable->isChecked();

    if (ui.radioButton_use_rules_file->isChecked()) {
        const std::array boxes = {ui.checkBox_rulesfile_1, ui.checkBox_rulesfile_2,
                                  ui.checkBox_rulesfile_3};
        const std::array edits = {ui.lineEdit_rulesfile_1, ui.lineEdit_rulesfile_2,
                                  ui.lineEdit_rulesfile_3};
        for (std::size_t i = 0; i < boxes.size(); ++i) {
            if (boxes.at(i)->isChecked() && !edits.at(i)->text().isEmpty()) {
                options.rulesFiles << edits.at(i)->text();
            }
        }
        // Stored as a plain bool: the builder only emits it next to the -r
        // files, so a ticked box with generated rules selected stays silent.
        options.rulesConcat = ui.checkBox_rules_concat->isChecked();
    } else if (ui.radioButton_generate_rules->isChecked()) {
        options.generateRules = ui.spinBox_generate_rules->value();
    }

    options.ruleLeft = ui.lineEdit_rule_left->text();
    options.ruleRight = ui.lineEdit_rule_right->text();

    options.mask = ui.lineEdit_mask->text();
    // The increment widgets are hidden in the modes that refuse the flag
    // (mode 12), so a session that stored a checked box there must not put
    // -i on the command line.
    options.increment = CommandBuilder::attackUsesIncrement(options.attackMode)
                        && ui.checkBox_increment->isChecked();
    options.incrementMin = ui.spinBox_increment_min->value();
    options.incrementMax = ui.spinBox_increment_max->value();
    options.speedOnly = ui.checkBox_speed_only->isChecked();
    options.optimizedKernel = ui.checkBox_optimized_kernel->isChecked();

    const std::array charsetBoxes = {
        ui.checkBox_custom_charset1,
        ui.checkBox_custom_charset2,
        ui.checkBox_custom_charset3,
        ui.checkBox_custom_charset4,
    };
    const std::array charsetEdits = {
        ui.lineEdit_custom_charset1,
        ui.lineEdit_custom_charset2,
        ui.lineEdit_custom_charset3,
        ui.lineEdit_custom_charset4,
    };
    for (std::size_t i = 0; i < charsetBoxes.size(); ++i) {
        options.customCharsets.at(i).enabled = charsetBoxes.at(i)->isChecked();
        options.customCharsets.at(i).value = charsetEdits.at(i)->text();
    }

    options.hexCharset = ui.checkBox_hex_charset->isChecked();
    options.hexSalt = ui.checkBox_hex_salt->isChecked();
    options.hexWordlist = ui.checkBox_hex_wordlist->isChecked();
    options.encodingFrom = ui.lineEdit_encoding_from->text();
    options.encodingTo = ui.lineEdit_encoding_to->text();

    options.outfileEnabled = ui.checkBox_outfile->isChecked();
    options.outfile = ui.lineEdit_outfile->text();
    options.outfileFormat = ui.lineEdit_outfile_format->text();
    options.outfileJson = ui.checkBox_outfile_json->isChecked();
    options.cachePath = ui.lineEdit_cache_path->text();
    options.cpuAffinity = ui.lineEdit_cpu_affinity->text();
    options.backendDevices = ui.lineEdit_backend_devices->text();
    options.openclDeviceTypes = ui.lineEdit_opencl_device_types->text();
    options.hwmonTempAbort = ui.checkBox_hwmon_temp_abort->isChecked();
    options.hwmonTempAbortCelsius = ui.spinBox_hwmon_temp_abort->value();
    options.hashFile = ui.lineEdit_hashfile->text();

    // Every argument list - the wordlists included - passes only its checked
    // rows: an unchecked row stays in the list and in the session, but not in
    // the command.
    options.wordlists = CheckedItemList::checkedItems(ui.listWidget_wordlist);

    // The feed modes. Every value is read unconditionally: a disabled or
    // hidden field can still hold a value a session stored, and the launch
    // validation is where a combination that cannot work is refused.
    const auto splitSettings = [](const QString &text) {
        static const QRegularExpression separator{QStringLiteral("\\s+")};
        return text.split(separator, Qt::SkipEmptyParts);
    };

    options.pcfgRulesets = CheckedItemList::checkedItems(ui.listWidget_pcfg_rulesets);
    options.pcfgScale = ui.lineEdit_pcfg_scale->text().trimmed();
    options.pcfgCostmax = ui.lineEdit_pcfg_costmax->text().trimmed();
    options.pcfgWeights = ui.lineEdit_pcfg_weights->text().trimmed();
    options.pcfgThreads = ui.lineEdit_pcfg_threads->text().trimmed();
    options.pcfgKbits = ui.lineEdit_pcfg_kbits->text().trimmed();
    options.pcfgMaxword = ui.lineEdit_pcfg_maxword->text().trimmed();
    options.pcfgMaxgain = ui.lineEdit_pcfg_maxgain->text().trimmed();
    options.pcfgWalk = ui.lineEdit_pcfg_walk->text().trimmed();
    options.pcfgMask = ui.lineEdit_pcfg_mask->text().trimmed();
    options.pcfgExtraSettings = splitSettings(ui.lineEdit_pcfg_extra->text());

    options.tableFiles = CheckedItemList::checkedItems(ui.listWidget_tables);
    options.tableIdentity = ui.lineEdit_table_identity->text().trimmed();
    options.tableTemplate = ui.lineEdit_table_template->text().trimmed();
    options.tableSingle = ui.lineEdit_table_single->text().trimmed();
    options.tableCap = ui.lineEdit_table_cap->text().trimmed();
    options.tableMaxperm = ui.lineEdit_table_maxperm->text().trimmed();
    options.tableExtraSettings = splitSettings(ui.lineEdit_table_extra->text());

    options.feedName = ui.lineEdit_feed->text().trimmed();
    options.feedArguments = CheckedItemList::checkedItems(ui.listWidget_feed_arguments);

    // The settings line of mode 9 stays as typed even next to a wordlist:
    // the launch validation names both controls instead of silently dropping
    // what the user typed, and the preview keeps showing it.
    options.associationSettings = splitSettings(ui.lineEdit_association_settings->text());

    return options;
}
