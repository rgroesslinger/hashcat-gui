/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef HELPERUTILS_H
#define HELPERUTILS_H

#include <QString>

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
        Outfile,
        OutfileFormat,
        Remove,
        RulesFile,
        SegmentSize,
        Username,
        WorkloadProfile,
    };

    static QString executeHashcat(QStringList &args);
    static QMap<QString, QStringList> getAvailableTerminals();
    static QString getParameter(Parameter key, bool useShort = false);

private:
    static QMap<Parameter, QPair<QString, QString>> parameterMap;
};

#endif // HELPERUTILS_H
