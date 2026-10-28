/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef HELPERUTILS_H
#define HELPERUTILS_H

#include <QCoreApplication>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QFuture>

#include <cstdint>

#include "appconstants.h"
#include "hashcatoptions.h"

// Result of a single hashcat invocation.
//
// The status members default to "no process ran", so an early return (missing
// configuration, process never started, timeout) cannot look like a successful
// run to the consumers that check exitStatus/exitCode first.
struct HashcatResult {
    QProcess::ExitStatus exitStatus = QProcess::CrashExit;
    int exitCode = -1;
    QString standardOutput;
    QString standardError;
};

class HelperUtils
{
    // The messages below end up in dialogs opened by the caller
    Q_DECLARE_TR_FUNCTIONS(HelperUtils)

public:
    // What can keep the Execute button from starting anything
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
        NoMask,             // the mode brute-forces a mask, none is given
        InvalidIncrement,   // the increment bounds contradict each other
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
        InvalidPcfgSetting,              // a mode 4 setting out of range or twice
        InvalidTableSetting,             // a mode 5 setting out of range or twice
        InvalidAssociationSetting,       // a mode 9 token that is not key=value
        AssociationSettingsWithWordlist, // settings only with no wordlist
    };

    // Checks everything that has to hold before hashcat can be started.
    //
    // detail receives the offending value - the missing path, or the terminals
    // that would work - when it is not nullptr.
    static LaunchError validateLaunch(const QString &hashFile, const QString &hashcatPath,
                                      const QString &configuredTerminal,
                                      const QStringList &availableTerminals,
                                      QString *detail = nullptr);

    // Checks what the selected attack mode needs: the word lists it reads,
    // the mask it brute-forces, the settings of the feed modes, the bounds
    // of the increment and the option combinations hashcat refuses. detail
    // receives the offending value - a count, or the field and value (or the
    // repeated key) of a setting - when it is not nullptr.
    //
    // What the mode needs comes from CommandBuilder::modeRequirements(), so
    // the group boxes, the mode hint and this check cannot disagree.
    static LaunchError validateOptions(const HashcatOptions &options, QString *detail = nullptr);

    static QFuture<HashcatResult>
    executeHashcat(const QStringList &args, int timeoutMs = AppConstants::Hashcat::QueryTimeoutMs);

    // Every supported terminal and the arguments that belong between the
    // terminal program and the hashcat command line. Pure data - nothing
    // here looks at the filesystem - so the tests can pin every entry.
    static const QMap<QString, QStringList> &terminalArguments();

    static QMap<QString, QStringList> availableTerminals();

    // The hashcat executable on the PATH, empty when this system has none.
    //
    // The settings dialog only offers it: a configured path is never replaced
    // by the one this lookup finds.
    static QString findHashcatInPath();

    // The note the settings dialog and a failed launch both show when this
    // system has no terminal hashcat-gui can start hashcat in.
    static QString noSupportedTerminalMessage();

    // Applies the color scheme the user chose.
    //
    // An empty scheme follows the system again - the desktop decides - while
    // the other two force a light or a dark appearance.
    static void applyColorScheme(const QString &scheme);
};

#endif // HELPERUTILS_H
