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
    // Valid command line parameters
    enum class Parameter : std::uint8_t {
        AttackMode,
        BackendDevices,
        CpuAffinity,
        CustomCharset1,
        CustomCharset2,
        CustomCharset3,
        CustomCharset4,
        EncodingFrom,
        EncodingTo,
        Force,
        GenerateRules,
        HashType,
        HexCharset,
        HexSalt,
        HexWordlist,
        HwmonTempAbort,
        Increment,
        IncrementMax,
        IncrementMin,
        Limit,
        OpenclDeviceTypes,
        OptimizedKernel,
        Outfile,
        OutfileFormat,
        PotfileDisable,
        Remove,
        RuleLeft,
        RuleRight,
        RulesFile,
        Runtime,
        SegmentSize,
        Session,
        Skip,
        SpeedOnly,
        Status,
        StatusTimer,
        Username,
        WorkloadProfile,
    };

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
        NoMask,             // the mode brute-forces a mask, none is given
        InvalidIncrement,   // the increment bounds contradict each other
    };

    // What an attack mode needs from the user before hashcat can run.
    //
    // The numbers come from hashcat's own argument validation: combination
    // mode multiplies two word lists and refuses every other number, the mask
    // based modes brute-force a mask and read at least one word list.
    struct ModeRequirements {
        // How many word lists the mode reads at least.
        int minWordlists = 0;
        // How many at most; -1 means "as many as the user selects".
        int maxWordlists = -1;
        // True when the mode needs a mask.
        bool needsMask = false;
    };

    // Checks everything that has to hold before hashcat can be started.
    //
    // detail receives the offending value - the missing path, or the terminals
    // that would work - when it is not nullptr.
    static LaunchError validateLaunch(const QString &hashFile, const QString &hashcatPath,
                                      const QString &configuredTerminal,
                                      const QStringList &availableTerminals,
                                      QString *detail = nullptr);

    // What the attack mode needs from the user, see ModeRequirements.
    static ModeRequirements modeRequirements(AttackMode mode);

    // Checks what the selected attack mode needs: the word lists it reads,
    // the mask it brute-forces and the bounds of the increment. detail
    // receives the number of selected word lists when it is not nullptr.
    static LaunchError validateOptions(const HashcatOptions &options, QString *detail = nullptr);

    static QFuture<HashcatResult>
    executeHashcat(const QStringList &args, int timeoutMs = AppConstants::Hashcat::QueryTimeoutMs);

    // Every supported terminal and the arguments that belong between the
    // terminal program and the hashcat command line. Pure data - nothing
    // here looks at the filesystem - so the tests can pin every entry.
    static const QMap<QString, QStringList> &terminalArguments();

    static QMap<QString, QStringList> getAvailableTerminals();
    static QString getParameter(Parameter key, bool useShort = false);
};

#endif // HELPERUTILS_H
