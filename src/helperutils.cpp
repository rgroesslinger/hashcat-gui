/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2025 Rainer Größlinger
 */

#include "helperutils.h"
#include "settingsmanager.h"
#include <QProcess>
#include <QString>

HelperUtils::HelperUtils() {}

// Execute hashcat and return its output
QString HelperUtils::executeHashcat(const QStringList& args) {
    QProcess process;
    QString output;

    auto& settings = SettingsManager::instance();

    if (!settings.hashcatPath().isEmpty()) {
        process.start(settings.hashcatPath(), args);
        process.waitForFinished();
        output = process.readAllStandardOutput();
    }

    if (process.exitCode() == QProcess::NormalExit && process.exitStatus() == QProcess::NormalExit) {
        return output;
    }

    return QString("Error executing " + settings.hashcatPath() + " " + args.join(" "));
}
