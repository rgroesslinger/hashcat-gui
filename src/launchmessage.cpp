/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "launchmessage.h"

#include "commandbuilder.h"
#include "terminalcatalog.h"

#include <QCoreApplication>

// The texts are translated in the LaunchMessage context from the first
// extraction; the result is an aggregate, so Q_DECLARE_TR_FUNCTIONS cannot
// be used here.
LaunchMessage messageFor(const LaunchValidator::LaunchProblem &problem,
                         const LaunchValidator::LaunchContext &context,
                         const QString &fileMenuLabel, const QString &settingsMenuLabel)
{
    LaunchMessage message;

    // No default case: a new LaunchError has to earn its message here.
    switch (problem.code) {
    case LaunchValidator::LaunchError::NoHashFile:
        message.icon = QMessageBox::Information;
        message.text = QCoreApplication::translate("LaunchMessage", "Please choose a hash file.");
        break;

    case LaunchValidator::LaunchError::NoHashcatPath:
        message.icon = QMessageBox::Information;
        message.format = Qt::RichText;
        message.text =
            QCoreApplication::translate(
                "LaunchMessage",
                "Navigate to <b>%1 → %2</b> to configure the path to the hashcat executable.")
                .arg(fileMenuLabel, settingsMenuLabel);
        break;

    // The messages that interpolate user supplied strings are set to plain
    // text explicitly: the default is AutoText, which would take a path or a
    // terminal name containing a '<' for markup and display it mangled.
    case LaunchValidator::LaunchError::HashcatPathMissing:
        message.format = Qt::PlainText;
        message.text =
            QCoreApplication::translate("LaunchMessage",
                                        "The configured hashcat executable does not exist:\n%1\n"
                                        "Navigate to %2 → %3 to change it.")
                .arg(problem.detail, fileMenuLabel, settingsMenuLabel);
        break;

    case LaunchValidator::LaunchError::NoTerminal:
        message.icon = QMessageBox::Information;
        message.format = Qt::RichText;
        message.text = QCoreApplication::translate(
                           "LaunchMessage",
                           "Navigate to <b>%1 → %2</b> to select the terminal used for launching.")
                           .arg(fileMenuLabel, settingsMenuLabel);
        break;

    case LaunchValidator::LaunchError::NoTerminals:
        message.text = TerminalCatalog::noSupportedTerminalMessage();
        break;

    case LaunchValidator::LaunchError::UnknownTerminal:
        message.format = Qt::PlainText;
        message.text =
            QCoreApplication::translate(
                "LaunchMessage",
                "The configured terminal “%1” is not available. Available terminals: %2\n"
                "Navigate to %3 → %4 to change it.")
                .arg(context.configuredTerminal, problem.detail, fileMenuLabel, settingsMenuLabel);
        break;

    // The mode specific requirements. They are not misconfigurations of the
    // installation but of the attack itself, so the message says what the
    // mode needs instead of pointing at the settings.
    case LaunchValidator::LaunchError::NoWordlist:
        message.icon = QMessageBox::Information;
        message.text =
            QCoreApplication::translate("LaunchMessage", "This mode needs at least one wordlist.");
        break;

    case LaunchValidator::LaunchError::WrongWordlistCount: {
        // The wording below uses minWordlists as the exact count. That is only
        // correct while Combination is the sole mode with maxWordlists >= 0,
        // and there min == max; a mode with a range reports TooManyWordlists.
        const CommandBuilder::ModeRequirements requirements =
            CommandBuilder::modeRequirements(context.options.attackMode);
        message.icon = QMessageBox::Information;
        message.text =
            QCoreApplication::translate(
                "LaunchMessage",
                "This mode needs exactly %1 wordlists (left, right).\nCurrently selected: %2.")
                .arg(requirements.minWordlists)
                .arg(problem.detail);
        break;
    }

    case LaunchValidator::LaunchError::TooManyWordlists: {
        const CommandBuilder::ModeRequirements requirements =
            CommandBuilder::modeRequirements(context.options.attackMode);
        message.icon = QMessageBox::Information;
        message.text =
            QCoreApplication::translate(
                "LaunchMessage",
                "Maximum number of wordlists for this mode: %1.\nCurrently selected: %2.")
                .arg(requirements.maxWordlists)
                .arg(problem.detail);
        if (context.options.attackMode == AttackMode::Association) {
            message.text +=
                QLatin1Char('\n')
                + QCoreApplication::translate(
                    "LaunchMessage", "hashcat pairs the two files by line here; this GUI offers "
                                     "only one wordlist for that role.");
        }
        break;
    }

    // The row is user supplied, so plain text: a '<' in the path would be
    // taken for markup by the default AutoText.
    case LaunchValidator::LaunchError::WordlistMustBeFile:
        message.format = Qt::PlainText;
        message.text = QCoreApplication::translate(
                           "LaunchMessage",
                           "This wordlist has to be a file: hashcat reads it as the multiplied "
                           "wordlist (the ?q side), and a folder cannot be used there:\n%1")
                           .arg(problem.detail);
        break;

    case LaunchValidator::LaunchError::NoMask:
        message.icon = QMessageBox::Information;
        message.text = QCoreApplication::translate("LaunchMessage", "This mode needs a mask.");
        break;

    case LaunchValidator::LaunchError::NoTable:
        message.icon = QMessageBox::Information;
        message.text =
            QCoreApplication::translate("LaunchMessage", "This mode needs at least one table.");
        break;

    case LaunchValidator::LaunchError::NoFeed:
        message.icon = QMessageBox::Information;
        message.text = QCoreApplication::translate(
            "LaunchMessage", "This mode needs a feed: a bundled name (wordlist, stdin, random, …) "
                             "or the path of a feed file.");
        break;

    case LaunchValidator::LaunchError::MaskNeedsWordMarker:
        message.icon = QMessageBox::Information;
        message.text = QCoreApplication::translate(
            "LaunchMessage", "This mode pairs the wordlists with the mask; the mask needs a ?w "
                             "for the word.");
        break;

    case LaunchValidator::LaunchError::MaskMultipleWordMarkers:
        message.icon = QMessageBox::Information;
        message.text =
            QCoreApplication::translate("LaunchMessage", "The mask contains more than one ?w.");
        break;

    case LaunchValidator::LaunchError::MaskQBeforeWord:
        message.icon = QMessageBox::Information;
        message.text = QCoreApplication::translate("LaunchMessage",
                                                   "The mask contains a ?q before the first ?w.");
        break;

    case LaunchValidator::LaunchError::MaskMultipleQMarks:
        message.icon = QMessageBox::Information;
        message.text =
            QCoreApplication::translate("LaunchMessage", "The mask contains more than one ?q.");
        break;

    case LaunchValidator::LaunchError::MaskQWithoutWordlist:
        message.icon = QMessageBox::Information;
        message.text = QCoreApplication::translate(
            "LaunchMessage", "The mask names a second word with ?q, but only one wordlist "
                             "is selected.");
        break;

    case LaunchValidator::LaunchError::SecondWordlistWithoutQ:
        message.icon = QMessageBox::Information;
        message.text = QCoreApplication::translate(
            "LaunchMessage", "Two wordlists are selected, but the mask has no ?q for the "
                             "second one.");
        break;

    case LaunchValidator::LaunchError::IncrementInMode12:
        message.icon = QMessageBox::Information;
        message.text = QCoreApplication::translate(
            "LaunchMessage", "The increment is not offered in this hybrid mode.");
        break;

    case LaunchValidator::LaunchError::MaskMarkersNotAllowed:
        message.icon = QMessageBox::Information;
        message.text = QCoreApplication::translate(
            "LaunchMessage", "The ?w and ?q markers are only offered in the hybrid mode "
                             "(mask + wordlists).");
        break;

    case LaunchValidator::LaunchError::PcfgCharsetWithoutMask:
        message.icon = QMessageBox::Information;
        message.text = QCoreApplication::translate(
            "LaunchMessage", "A custom charset is used only inside a mask, and this run named no "
                             "mask. Add a mask= setting that references ?1–?4, for example "
                             "mask=?1?1?1?1, or clear the custom charset.");
        break;

    // The settings errors name a value or token the user typed, so they are
    // set to plain text explicitly: a value containing '<' would be taken
    // for markup by the default AutoText.
    case LaunchValidator::LaunchError::InvalidPcfgSetting:
    case LaunchValidator::LaunchError::InvalidTableSetting:
        message.icon = QMessageBox::Information;
        message.format = Qt::PlainText;
        message.text = QCoreApplication::translate("LaunchMessage",
                                                   "The setting “%1” is not valid for this mode.")
                           .arg(problem.detail);
        break;

    case LaunchValidator::LaunchError::InvalidAssociationSetting:
        message.icon = QMessageBox::Information;
        message.format = Qt::PlainText;
        message.text = QCoreApplication::translate(
                           "LaunchMessage",
                           "The token “%1” is not a setting; settings are written as key=value.")
                           .arg(problem.detail);
        break;

    case LaunchValidator::LaunchError::AssociationSettingsWithWordlist:
        message.icon = QMessageBox::Information;
        message.text = QCoreApplication::translate(
            "LaunchMessage", "The settings line applies only while the words come from the hash "
                             "file. Clear either the settings line or the wordlist list.");
        break;

    case LaunchValidator::LaunchError::NoHashRecipe:
        message.icon = QMessageBox::Information;
        message.text = QCoreApplication::translate(
            "LaunchMessage", "The hash type 4000 describes its hashes as a recipe, for example\n"
                             "md5(md5(salt) . pass). Enter it in the hash recipe field.");
        break;

    case LaunchValidator::LaunchError::InvalidIncrement:
        message.icon = QMessageBox::Information;
        message.text = QCoreApplication::translate(
            "LaunchMessage", "The minimum increment length is larger than the maximum.");
        break;

    // The path below is user supplied, so plain text: a '<' in it would be
    // taken for markup by the default AutoText.
    case LaunchValidator::LaunchError::InvalidCachePath:
        message.format = Qt::PlainText;
        message.text = QCoreApplication::translate(
                           "LaunchMessage", "The cache path must be an existing directory:\n%1")
                           .arg(problem.detail);
        break;

    case LaunchValidator::LaunchError::IncrementWithSpeedOnly:
        message.icon = QMessageBox::Information;
        message.text = QCoreApplication::translate(
            "LaunchMessage", "The increment cannot be combined with the speed-only option.");
        break;

    case LaunchValidator::LaunchError::RemoveInAssociation:
        message.icon = QMessageBox::Information;
        message.text = QCoreApplication::translate(
            "LaunchMessage",
            "Removing cracked hashes is not possible in the association attack mode.");
        break;

    case LaunchValidator::LaunchError::None:
        return {};
    }

    return message;
}