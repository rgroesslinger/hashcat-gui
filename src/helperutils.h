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

    static QFuture<HashcatResult> executeHashcat(const QStringList &args, int timeoutMs = 20000);
    static QMap<QString, QStringList> getAvailableTerminals();
    static QString getParameter(Parameter key, bool useShort = false);

private:
    static const QMap<Parameter, QPair<QString, QString>> parameterMap;
};

#endif // HELPERUTILS_H
