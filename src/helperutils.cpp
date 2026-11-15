/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "helperutils.h"

#include "appconstants.h"
#include "hashcatrunner.h"
#include "launchvalidator.h"
#include "settingsmanager.h"

#include <QFuture>
#include <QGuiApplication>
#include <QString>
#include <QStyleHints>

// Reads the configured path on the calling thread and forwards to the runner:
// QSettings is only reentrant, and the runner's worker runs on the thread pool.
QFuture<HashcatResult> HelperUtils::executeHashcat(const QStringList &args, int timeoutMs)
{
    const QString hashcatPath =
        SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    return HashcatRunner::execute(hashcatPath, args, timeoutMs);
}

// Transitional facade: the old signatures forward to LaunchValidator while
// the tests move to the new API. Deleted together with HelperUtils.
HelperUtils::LaunchError HelperUtils::validateLaunch(const QString &hashFile,
                                                     const QString &hashcatPath,
                                                     const QString &configuredTerminal,
                                                     const QStringList &availableTerminals,
                                                     QString *detail)
{
    LaunchValidator::LaunchContext context;
    context.hashcatPath = hashcatPath;
    context.configuredTerminal = configuredTerminal;
    context.availableTerminals = availableTerminals;
    context.options.hashFile = hashFile;
    return LaunchValidator::validateLaunch(context, detail);
}

HelperUtils::LaunchError HelperUtils::validateOptions(const HashcatOptions &options,
                                                      QString *detail)
{
    return LaunchValidator::validateOptions(options, detail);
}

void HelperUtils::applyColorScheme(const QString &scheme)
{
    QStyleHints *hints = QGuiApplication::styleHints();
    if (!hints) {
        return;
    }

    if (scheme == AppConstants::Appearance::ColorSchemeDark) {
        hints->setColorScheme(Qt::ColorScheme::Dark);
    } else if (scheme == AppConstants::Appearance::ColorSchemeLight) {
        hints->setColorScheme(Qt::ColorScheme::Light);
    } else {
        // The system default: the desktop decides again.
        hints->unsetColorScheme();
    }
}
