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

// Mapping of supported command line options
QMap<HelperUtils::Parameter, QPair<QString, QString>> HelperUtils::parameterMap = {
    {HelperUtils::Parameter::AttackMode,        {"-a",  "--attack-mode"}},
    {HelperUtils::Parameter::BackendDevices,    {"-d",  "--backend-devices"}},
    {HelperUtils::Parameter::CpuAffinity,       {"",    "--cpu-affinity"}},
    {HelperUtils::Parameter::CustomCharset1,    {"-1",  "--custom-charset1"}},
    {HelperUtils::Parameter::CustomCharset2,    {"-2",  "--custom-charset2"}},
    {HelperUtils::Parameter::CustomCharset3,    {"-3",  "--custom-charset3"}},
    {HelperUtils::Parameter::CustomCharset4,    {"-4",  "--custom-charset4"}},
    {HelperUtils::Parameter::GenerateRules,     {"-g",  "--generate-rules"}},
    {HelperUtils::Parameter::HashType,          {"-m",  "--hash-type"}},
    {HelperUtils::Parameter::HexCharset,        {"",    "--hex-charset"}},
    {HelperUtils::Parameter::HexSalt,           {"",    "--hex-salt"}},
    {HelperUtils::Parameter::Outfile,           {"-o",  "--outfile"}},
    {HelperUtils::Parameter::OutfileFormat,     {"",    "--outfile-format"}},
    {HelperUtils::Parameter::Remove,            {"",    "--remove"}},
    {HelperUtils::Parameter::RulesFile,         {"-r",  "--rules-file"}},
    {HelperUtils::Parameter::SegmentSize,       {"-c",  "--segment-size"}},
    {HelperUtils::Parameter::Username,          {"",    "--username"}},
    {HelperUtils::Parameter::WorkloadProfile,   {"-w",  "--workload-profile"}},
};

HelperUtils::HelperUtils() {}

QString HelperUtils::getParameter(Parameter key, bool useShort)
{
    auto it = parameterMap.find(key);
    const auto &pair = it.value();

    if (useShort && !pair.first.isEmpty()) {
        // short form
        return pair.first;
    }

    // long form (default)
    return pair.second;
}

// Execute hashcat and return its output
QString HelperUtils::executeHashcat(QStringList &args) {
    QProcess process;
    QString output;

    auto &settings = SettingsManager::instance();

    // Always run in quiet mode when reading output
    args << "--quiet";

    if (!settings.getKey<QString>("hashcatPath").isEmpty()) {
        process.setProgram(settings.getKey<QString>("hashcatPath"));
        process.setArguments(args);
        process.setWorkingDirectory(QFileInfo(settings.getKey<QString>("hashcatPath")).absolutePath());
        process.start();
        process.waitForFinished();
        output = process.readAllStandardOutput();
    }

    if (process.exitCode() == QProcess::NormalExit && process.exitStatus() == QProcess::NormalExit) {
        return output;
    }

    return QString("Error executing " + settings.getKey<QString>("hashcatPath") + " " + args.join(" "));
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
