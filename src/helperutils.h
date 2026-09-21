/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef HELPERUTILS_H
#define HELPERUTILS_H

#include <QString>
#include <QStringList>
#include <QProcess>
#include <QFuture>

#include "appconstants.h"

// Result of a single hashcat invocation.
//
// The status members are initialised to values that mean "no process ran":
// every consumer checks `exitStatus != NormalExit || exitCode != 0` before
// looking at the output, so an early return (missing configuration, process
// never started, timeout) must not be able to look like a successful run.
struct HashcatResult {
    QProcess::ExitStatus exitStatus = QProcess::CrashExit;
    int exitCode = -1;
    QString standardOutput;
    QString standardError;
};

class HelperUtils
{
public:
    HelperUtils();

    // Valid command line parameters
    enum class Parameter
    {
        AttackMode,
        BackendDevices,
        CpuAffinity,
        CustomCharset1,
        CustomCharset2,
        CustomCharset3,
        CustomCharset4,
        GenerateRules,
        HashType,
        HexCharset,
        HexSalt,
        OptimizedKernel,
        Outfile,
        OutfileFormat,
        Remove,
        RulesFile,
        SegmentSize,
        SpeedOnly,
        Username,
        WorkloadProfile,
    };

    // What can keep the Execute button from starting anything
    enum class LaunchError
    {
        None,
        NoHashFile,         // no hash file selected
        NoHashcatPath,      // the hashcat path was never configured
        HashcatPathMissing, // configured, but the file is not there (anymore)
        NoTerminal,         // no terminal selected
        NoTerminals,        // this system has no supported terminal at all
        UnknownTerminal,    // the configured terminal is not installed
    };

    // Checks everything that has to hold before hashcat can be started.
    //
    // detail receives the offending value - the missing path, or the
    // terminals that would work - when it is not nullptr. Keeping this out of
    // MainWindow means the conditions can be tested without showing dialogs.
    static LaunchError validateLaunch(const QString &hashFile,
                                      const QString &hashcatPath,
                                      const QString &configuredTerminal,
                                      const QStringList &availableTerminals,
                                      QString *detail = nullptr);

    static QFuture<HashcatResult> executeHashcat(const QStringList &args, int timeoutMs = AppConstants::Hashcat::QueryTimeoutMs);
    static QMap<QString, QStringList> getAvailableTerminals();
    static QString getParameter(Parameter key, bool useShort = false);

private:
    static const QMap<Parameter, QPair<QString, QString>> parameterMap;
};

#endif // HELPERUTILS_H
