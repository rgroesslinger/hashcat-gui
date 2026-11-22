/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "attackmodeview.h"
#include "commandbuilder.h"
#include "testenvironment.h"
#include "ui_mainwindow.h"

#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>

// The mode gating: every group follows the builder's predicate for the
// applied mode, and the rule sides keep their enabled distinction.
class TestAttackModeView : public QObject
{
    Q_OBJECT

private slots:
    void everyGroupFollowsItsPredicate();
    void theRuleSidesKeepTheirEnabledState();

private:
    struct Form {
        QMainWindow window;
        Ui::MainWindow ui;
        AttackModeView view;

        Form() : ui{}, view(ui)
        {
            ui.setupUi(&window);
        }
    };
};

void TestAttackModeView::everyGroupFollowsItsPredicate()
{
    Form form;
    for (const AttackMode mode : AllAttackModes) {
        form.view.apply(mode);

        QCOMPARE(!form.ui.groupBox_wordlists->isHidden(),
                 CommandBuilder::attackUsesWordlists(mode));
        QCOMPARE(!form.ui.groupBox_rules->isHidden(), CommandBuilder::attackUsesRules(mode));
        QCOMPARE(!form.ui.groupBox_custom_charset->isHidden(),
                 CommandBuilder::attackUsesCustomCharset(mode));
        QCOMPARE(!form.ui.groupBox_mask->isHidden(), CommandBuilder::attackUsesMask(mode));
        QCOMPARE(!form.ui.groupBox_pcfg->isHidden(), mode == AttackMode::Pcfg);
        QCOMPARE(!form.ui.groupBox_table->isHidden(), mode == AttackMode::Table);
        QCOMPARE(!form.ui.groupBox_feed->isHidden(), mode == AttackMode::Generic);
        QCOMPARE(!form.ui.label_association_settings->isHidden(), mode == AttackMode::Association);
        QCOMPARE(!form.ui.lineEdit_association_settings->isHidden(),
                 mode == AttackMode::Association);
        QCOMPARE(!form.ui.checkBox_hex_wordlist->isHidden(),
                 CommandBuilder::attackUsesHexWordlist(mode));
        const bool usesEncoding = CommandBuilder::attackUsesWordlistEncoding(mode);
        QCOMPARE(!form.ui.label_encoding_from->isHidden(), usesEncoding);
        QCOMPARE(!form.ui.lineEdit_encoding_from->isHidden(), usesEncoding);
        QCOMPARE(!form.ui.label_encoding_to->isHidden(), usesEncoding);
        QCOMPARE(!form.ui.lineEdit_encoding_to->isHidden(), usesEncoding);
        const bool usesIncrement = CommandBuilder::attackUsesIncrement(mode);
        QCOMPARE(!form.ui.checkBox_increment->isHidden(), usesIncrement);
        QCOMPARE(!form.ui.checkBox_increment_inverse->isHidden(), usesIncrement);
        QCOMPARE(!form.ui.label_increment_min->isHidden(), usesIncrement);
        QCOMPARE(!form.ui.spinBox_increment_min->isHidden(), usesIncrement);
        QCOMPARE(!form.ui.label_increment_max->isHidden(), usesIncrement);
        QCOMPARE(!form.ui.spinBox_increment_max->isHidden(), usesIncrement);
        // Mode 9 turns the potfile off, so the path row follows the builder.
        const bool usesPotfile = mode != AttackMode::Association;
        QCOMPARE(!form.ui.label_potfile_path->isHidden(), usesPotfile);
        QCOMPARE(!form.ui.lineEdit_potfile_path->isHidden(), usesPotfile);
        QCOMPARE(!form.ui.pushButton_open_potfile_path->isHidden(), usesPotfile);
        QCOMPARE(form.ui.label_mode_hint->text(), AttackModeView::hint(mode));
    }
}

void TestAttackModeView::theRuleSidesKeepTheirEnabledState()
{
    Form form;
    for (const AttackMode mode : AllAttackModes) {
        form.view.apply(mode);

        const bool left = CommandBuilder::attackUsesRuleLeft(mode);
        const bool right = CommandBuilder::attackUsesRuleRight(mode);
        // The group is visible and enabled only while one side applies; the
        // fields stay enabled by side. Hidden, not disabled, is the group's
        // lot when neither side applies.
        QCOMPARE(!form.ui.groupBox_wordlist_rules->isHidden(), left || right);
        QCOMPARE(form.ui.groupBox_wordlist_rules->isEnabled(), left || right);
        QCOMPARE(form.ui.label_rule_left->isEnabled(), left);
        QCOMPARE(form.ui.lineEdit_rule_left->isEnabled(), left);
        QCOMPARE(form.ui.label_rule_right->isEnabled(), right);
        QCOMPARE(form.ui.lineEdit_rule_right->isEnabled(), right);
    }
}

int main(int argc, char *argv[])
{
    TestAttackModeView test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_attackmodeview.moc"