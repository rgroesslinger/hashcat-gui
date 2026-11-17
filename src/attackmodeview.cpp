/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "attackmodeview.h"

#include "commandbuilder.h"
#include "ui_mainwindow.h"

#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

AttackModeView::AttackModeView(Ui::MainWindow &ui) : m_ui(&ui) {}

QString AttackModeView::name(AttackMode mode)
{
    switch (mode) {
    case AttackMode::Straight:
        return tr("Straight");
    case AttackMode::Combination:
        return tr("Combination");
    case AttackMode::BruteForce:
        return tr("Brute-force");
    case AttackMode::Pcfg:
        return tr("PCFG");
    case AttackMode::Table:
        return tr("Table");
    case AttackMode::HybridWordMask:
        return tr("Hybrid Wordlist + Mask");
    case AttackMode::HybridMaskWord:
        return tr("Hybrid Mask + Wordlist");
    case AttackMode::Generic:
        return tr("Generic (feed)");
    case AttackMode::Association:
        return tr("Association");
    case AttackMode::Hybrid:
        return tr("Hybrid (mask + ?w/?q)");
    }
    return {};
}

void AttackModeView::apply(AttackMode mode) const
{
    const bool usesMask = CommandBuilder::attackUsesMask(mode);
    const bool usesRuleLeft = CommandBuilder::attackUsesRuleLeft(mode);
    const bool usesRuleRight = CommandBuilder::attackUsesRuleRight(mode);
    const bool usesIncrement = CommandBuilder::attackUsesIncrement(mode);
    m_ui->groupBox_wordlists->setVisible(CommandBuilder::attackUsesWordlists(mode));
    m_ui->groupBox_rules->setVisible(CommandBuilder::attackUsesRules(mode));
    // The group starts out disabled in the .ui file, so it keeps the
    // enabled gate next to the visibility one: it is visible only while one
    // of the two sides applies, and enabled exactly then.
    m_ui->groupBox_wordlist_rules->setVisible(usesRuleLeft || usesRuleRight);
    m_ui->groupBox_wordlist_rules->setEnabled(usesRuleLeft || usesRuleRight);
    m_ui->label_rule_left->setEnabled(usesRuleLeft);
    m_ui->lineEdit_rule_left->setEnabled(usesRuleLeft);
    m_ui->label_rule_right->setEnabled(usesRuleRight);
    m_ui->lineEdit_rule_right->setEnabled(usesRuleRight);
    m_ui->groupBox_custom_charset->setVisible(CommandBuilder::attackUsesCustomCharset(mode));
    m_ui->groupBox_mask->setVisible(usesMask);

    // The feed modes get a group each, and mode 9 one extra row: the
    // settings line that bounds a run whose words come from the hash file.
    m_ui->groupBox_pcfg->setVisible(mode == AttackMode::Pcfg);
    m_ui->groupBox_table->setVisible(mode == AttackMode::Table);
    m_ui->groupBox_feed->setVisible(mode == AttackMode::Generic);
    m_ui->label_association_settings->setVisible(mode == AttackMode::Association);
    m_ui->lineEdit_association_settings->setVisible(mode == AttackMode::Association);

    // Inside the wordlist group: only the modes that read a wordlist take
    // the wordlist options, and only the ones hashcat lets name an
    // encoding.
    m_ui->checkBox_hex_wordlist->setVisible(CommandBuilder::attackUsesHexWordlist(mode));
    const bool usesEncoding = CommandBuilder::attackUsesWordlistEncoding(mode);
    m_ui->label_encoding_from->setVisible(usesEncoding);
    m_ui->lineEdit_encoding_from->setVisible(usesEncoding);
    m_ui->label_encoding_to->setVisible(usesEncoding);
    m_ui->lineEdit_encoding_to->setVisible(usesEncoding);

    // Inside the mask group: hashcat 8.0.0 refuses -i in mode 12, so the
    // increment row only exists where the flag is offered.
    m_ui->checkBox_increment->setVisible(usesIncrement);
    m_ui->label_increment_min->setVisible(usesIncrement);
    m_ui->spinBox_increment_min->setVisible(usesIncrement);
    m_ui->label_increment_max->setVisible(usesIncrement);
    m_ui->spinBox_increment_max->setVisible(usesIncrement);

    // Tell the user what the mode expects, so the launch validation does not
    // have to be the first thing that explains it.
    m_ui->label_mode_hint->setText(hint(mode));
}

QString AttackModeView::hint(AttackMode mode)
{
    // One sentence per mode; the .ui wraps the label.
    switch (mode) {
    case AttackMode::Straight:
        return tr("This mode needs at least one wordlist.");
    case AttackMode::Combination:
        return tr("This mode needs exactly 2 wordlists (left, right).");
    case AttackMode::BruteForce:
        return tr("This mode needs a mask.");
    case AttackMode::Pcfg:
        return tr("This mode runs hashcat’s bundled default-passwords ruleset; "
                  "name one or more to use your own.");
    case AttackMode::Table:
        return tr("This mode needs one wordlist and at least one table.");
    case AttackMode::HybridWordMask:
    case AttackMode::HybridMaskWord:
        return tr("This mode needs a wordlist and a mask.");
    case AttackMode::Generic:
        return tr("This mode needs a feed. Arguments depend on the feed: "
                  "random and stdin need none, wordlist needs a path.");
    case AttackMode::Association:
        // The settings grammar (phases, the never-ending default) is the
        // settings line's own tooltip; the hint keeps only what the mode
        // needs, in the wording of modeRequirements().
        return tr("This mode needs at most one wordlist. Without one the words come from the "
                  "hash file and the settings line can bound the run; with one the two files "
                  "are paired by line.");
    case AttackMode::Hybrid:
        return tr("This mode needs a mask with exactly one ?w; a ?q after ?w "
                  "names a second wordlist.");
    }
    return {};
}
