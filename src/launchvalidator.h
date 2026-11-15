/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef LAUNCHVALIDATOR_H
#define LAUNCHVALIDATOR_H

#include "hashcatoptions.h"

#include <QString>
#include <QStringList>
#include <cstdint>

// What can keep the Execute button from starting anything, and the checks
// that find it before hashcat is started. The validator reads no settings:
// everything it needs is in the context, read on the calling thread.
namespace LaunchValidator
{
enum class LaunchError : std::uint8_t {
    None,
    NoHashFile,         // no hash file selected
    NoHashcatPath,      // the hashcat path was never configured
    HashcatPathMissing, // configured, but the file is not there (anymore)
    NoTerminal,         // no terminal selected
    NoTerminals,        // this system has no supported terminal at all
    UnknownTerminal,    // the configured terminal is not installed
    NoWordlist,         // the mode reads word lists, none is selected
    WrongWordlistCount, // the mode wants an exact number of word lists
    TooManyWordlists,   // more word lists than the mode accepts
    // The wordlist hashcat multiplies (?q) is read by one feed instance
    // from start to end, so unlike the base it cannot be a folder.
    WordlistMustBeFile,
    NoMask,           // the mode brute-forces a mask, none is given
    InvalidIncrement, // the increment bounds contradict each other
    // hashcat 8.0.0 silently ignores -i under --speed-only (it allows the
    // flag for ACT_CRACK | ACT_STDOUT | ACT_KEYSPACE), so refusing the
    // combination is a GUI policy: incrementing would make a fixed speed
    // measurement meaningless.
    IncrementWithSpeedOnly,
    RemoveInAssociation,             // hashcat refuses --remove in attack mode 9
    NoTable,                         // the table feed got no table
    NoFeed,                          // the generic feed got no feed name
    MaskNeedsWordMarker,             // the mode 12 mask names no ?w word
    MaskMultipleWordMarkers,         // more than one ?w in the mask
    MaskQBeforeWord,                 // a ?q before the first ?w in the mask
    MaskMultipleQMarks,              // more than one ?q in the mask
    MaskQWithoutWordlist,            // ?q names a second word, no second wordlist
    SecondWordlistWithoutQ,          // two wordlists, no ?q for the second
    IncrementInMode12,               // hashcat refuses -i in the hybrid mode 12
    MaskMarkersNotAllowed,           // ?w/?q in a mode that rewrites them
    PcfgCharsetWithoutMask,          // a mode 4 charset has no mask= to sit in
    InvalidPcfgSetting,              // a mode 4 setting out of range or twice
    InvalidTableSetting,             // a mode 5 setting out of range or twice
    InvalidAssociationSetting,       // a mode 9 token that is not key=value
    AssociationSettingsWithWordlist, // settings only with no wordlist
    NoHashRecipe,                    // hash mode 4000 without its recipe
    InvalidCachePath,                // --cache-path is not an existing directory
};

// Everything the validation reads. attackMode and hashFile live in options;
// there are no duplicate fields.
struct LaunchContext {
    QString hashcatPath;
    QString configuredTerminal;
    QStringList availableTerminals;
    HashcatOptions options;
};

struct LaunchProblem {
    LaunchError code = LaunchError::None;
    // The offending value - display-only, never parsed.
    QString detail;

    [[nodiscard]] explicit operator bool() const
    {
        return code != LaunchError::None;
    }
};

// The option checks run first - they are what the user has just configured,
// and hashcat would only answer them with a usage error after the run has
// started - and the launch checks second. Within each group the order is
// part of the contract: the first problem found is the one reported.
LaunchProblem validate(const LaunchContext &context);

// The two groups on their own. Transitional: they exist while HelperUtils
// forwards its old signatures, and go back into the implementation when the
// facade is deleted.
LaunchError validateOptions(const HashcatOptions &options, QString *detail = nullptr);
LaunchError validateLaunch(const LaunchContext &context, QString *detail = nullptr);
} // namespace LaunchValidator

#endif // LAUNCHVALIDATOR_H