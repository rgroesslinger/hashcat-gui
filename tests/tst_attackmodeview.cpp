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
// applied mode, the rule sides keep their enabled distinction, and the
// names and hints are the unit's own texts.
class TestAttackModeView : public QObject
{
    Q_OBJECT

private slots:
    void everyGroupFollowsItsPredicate();
    void theRuleSidesKeepTheirEnabledState();
    void theNamesArePinned();
    void theHintsArePinned();

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
        QCOMPARE(!form.ui.label_increment_min->isHidden(), usesIncrement);
        QCOMPARE(!form.ui.spinBox_increment_min->isHidden(), usesIncrement);
        QCOMPARE(!form.ui.label_increment_max->isHidden(), usesIncrement);
        QCOMPARE(!form.ui.spinBox_increment_max->isHidden(), usesIncrement);
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

void TestAttackModeView::theNamesArePinned()
{
    QCOMPARE(AttackModeView::name(AttackMode::Straight), QStringLiteral("Straight"));
    QCOMPARE(AttackModeView::name(AttackMode::Combination), QStringLiteral("Combination"));
    QCOMPARE(AttackModeView::name(AttackMode::BruteForce), QStringLiteral("Brute-force"));
    QCOMPARE(AttackModeView::name(AttackMode::Pcfg), QStringLiteral("PCFG"));
    QCOMPARE(AttackModeView::name(AttackMode::Table), QStringLiteral("Table"));
    QCOMPARE(AttackModeView::name(AttackMode::HybridWordMask),
             QStringLiteral("Hybrid Wordlist + Mask"));
    QCOMPARE(AttackModeView::name(AttackMode::HybridMaskWord),
             QStringLiteral("Hybrid Mask + Wordlist"));
    QCOMPARE(AttackModeView::name(AttackMode::Generic), QStringLiteral("Generic (feed)"));
    QCOMPARE(AttackModeView::name(AttackMode::Association), QStringLiteral("Association"));
    QCOMPARE(AttackModeView::name(AttackMode::Hybrid), QStringLiteral("Hybrid (mask + ?w/?q)"));
}

void TestAttackModeView::theHintsArePinned()
{
    QCOMPARE(AttackModeView::hint(AttackMode::Straight),
             QStringLiteral("This mode needs at least one wordlist."));
    QCOMPARE(AttackModeView::hint(AttackMode::Combination),
             QStringLiteral("This mode needs exactly 2 wordlists (left, right)."));
    QCOMPARE(AttackModeView::hint(AttackMode::BruteForce),
             QStringLiteral("This mode needs a mask."));
    QCOMPARE(AttackModeView::hint(AttackMode::Pcfg),
             QStringLiteral("This mode runs hashcat’s bundled default-passwords ruleset; "
                            "name one or more to use your own."));
    QCOMPARE(AttackModeView::hint(AttackMode::Table),
             QStringLiteral("This mode needs one wordlist and at least one table."));
    QCOMPARE(AttackModeView::hint(AttackMode::HybridWordMask),
             QStringLiteral("This mode needs a wordlist and a mask."));
    QCOMPARE(AttackModeView::hint(AttackMode::HybridMaskWord),
             QStringLiteral("This mode needs a wordlist and a mask."));
    QCOMPARE(AttackModeView::hint(AttackMode::Generic),
             QStringLiteral("This mode needs a feed. Arguments depend on the feed: "
                            "random and stdin need none, wordlist needs a path."));
    QCOMPARE(AttackModeView::hint(AttackMode::Association),
             QStringLiteral("This mode needs at most one wordlist. Without one the words come "
                            "from the hash file and the settings line can bound the run; with "
                            "one the two files are paired by line."));
    QCOMPARE(AttackModeView::hint(AttackMode::Hybrid),
             QStringLiteral("This mode needs a mask with exactly one ?w; a ?q after ?w "
                            "names a second wordlist."));
}

int main(int argc, char *argv[])
{
    TestAttackModeView test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_attackmodeview.moc"