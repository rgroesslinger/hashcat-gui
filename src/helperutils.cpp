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
QString HelperUtils::executeHashcat(QStringList& args) {
    QProcess process;
    QString output;

    auto& settings = SettingsManager::instance();

    // Always run in quiet mode when reading output
    args << "--quiet";

    if (!settings.hashcatPath().isEmpty()) {
        process.setProgram(settings.hashcatPath());
        process.setArguments(args);
        process.start();
        process.waitForFinished();
        output = process.readAllStandardOutput();
    }

    if (process.exitCode() == QProcess::NormalExit && process.exitStatus() == QProcess::NormalExit) {
        return output;
    }

    return QString("Error executing " + settings.hashcatPath() + " " + args.join(" "));
}
