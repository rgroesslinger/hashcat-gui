/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef HELPERUTILS_H
#define HELPERUTILS_H

#include "appconstants.h"
#include "hashcatoptions.h"
#include "hashcatrunner.h"
#include "launchvalidator.h"

#include <QCoreApplication>
#include <QFuture>
#include <QString>
#include <QStringList>
#include <cstdint>

class HelperUtils
{
    // The messages below end up in dialogs opened by the caller
    Q_DECLARE_TR_FUNCTIONS(HelperUtils)

public:
    // Transitional alias while the tests move to LaunchValidator.
    using LaunchError = LaunchValidator::LaunchError;

    // Transitional facade: the old signatures forward to LaunchValidator.
    // Deleted together with HelperUtils.
    static LaunchError validateLaunch(const QString &hashFile, const QString &hashcatPath,
                                      const QString &configuredTerminal,
                                      const QStringList &availableTerminals,
                                      QString *detail = nullptr);
    static LaunchError validateOptions(const HashcatOptions &options, QString *detail = nullptr);

    // Reads the configured path on the calling thread and forwards to
    // HashcatRunner::execute(). The callers that already hold the path use
    // the runner directly.
    static QFuture<HashcatResult>
    executeHashcat(const QStringList &args, int timeoutMs = AppConstants::Hashcat::QueryTimeoutMs);

    // Applies the color scheme the user chose.
    //
    // An empty scheme follows the system again - the desktop decides - while
    // the other two force a light or a dark appearance.
    static void applyColorScheme(const QString &scheme);
};

#endif // HELPERUTILS_H