/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "helperutils.h"
#include "settingsmanager.h"
#include <QProcess>
#include <QString>
#include <QStandardPaths>
#include <QFileInfo>

HelperUtils::HelperUtils() {}

// Execute hashcat and return its output
QString HelperUtils::executeHashcat(QStringList& args) {
    QProcess process;
    QString output;

    auto& settings = SettingsManager::instance();

    // Always run in quiet mode when reading output
    args << "--quiet";

    if (!settings.getKey("hashcatPath").isEmpty()) {
        process.setProgram(settings.getKey("hashcatPath"));
        process.setArguments(args);
        process.setWorkingDirectory(QFileInfo(settings.getKey("hashcatPath")).absolutePath());
        process.start();
        process.waitForFinished();
        output = process.readAllStandardOutput();
    }

    if (process.exitCode() == QProcess::NormalExit && process.exitStatus() == QProcess::NormalExit) {
        return output;
    }

    return QString("Error executing " + settings.getKey("hashcatPath") + " " + args.join(" "));
}

// Returns all supported terminals
QMap<QString, QStringList> HelperUtils::getAvailableTerminals() {
    QMap<QString, QStringList> terminals;

    // List of terminals and needed arguments to launch them with an external command
    QMap<QString, QStringList> terminalMap = {
        {"cmd.exe", {"/k"}},
        {"xterm", {"-hold", "-e"}},
        {"gnome-terminal", {"--wait", "--"}},
        {"ptyxis", {"--"}},
        {"konsole", {"--hold", "-e"}},
        {"xfce4-terminal", {"--hold", "-e"}},
    };

    // Check which ones are actually available
    for (auto it = terminalMap.begin(); it != terminalMap.end(); ++it) {
        QString fullPath = QStandardPaths::findExecutable(it.key());
        if (!fullPath.isEmpty()) {
            terminals.insert(it.key(), it.value());
        }
    }

    return terminals;
}
