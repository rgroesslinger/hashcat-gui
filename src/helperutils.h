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

struct HashcatResult {
    QProcess::ExitStatus exitStatus;
    int exitCode;
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
    static QMap<Parameter, QPair<QString, QString>> parameterMap;
};

#endif // HELPERUTILS_H
