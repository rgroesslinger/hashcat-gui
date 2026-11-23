/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef POTFILEQUERY_H
#define POTFILEQUERY_H

#include "appconstants.h"

#include <QString>
#include <QStringList>

// The form values a potfile query needs. The hash input is already resolved:
// an absolute file path or a literal hash. The potfile path is absolute, or
// empty to let hashcat use its default.
struct PotfileQueryInput {
    QString hashInput;
    int hashType = AppConstants::Hashcat::AutoDetectHashType;
    QString potfilePath;
    bool username = false;
};

// The argv half of the read-only potfile queries, kept free of widgets and
// processes so the order and the omissions can be tested directly.
namespace PotfileQuery
{
QString uniqueSession();
QStringList showArgs(const PotfileQueryInput &input);
QStringList leftArgs(const PotfileQueryInput &input);

// Resolves a path the way the query runs: relative to the directory hashcat
// uses as its working directory, so the query reads the same file a launch
// would. An empty or absolute path is returned unchanged.
QString resolvePath(const QString &entered, const QString &hashcatPath);

// A hash input is treated as a path only when it exists; otherwise it is an
// inline hash. An existing relative path is resolved like resolvePath().
QString resolveHashInput(const QString &entered, const QString &hashcatPath = QString());
} // namespace PotfileQuery

#endif // POTFILEQUERY_H