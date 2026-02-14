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
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>

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
    {HelperUtils::Parameter::OptimizedKernel,   {"-O",  "--optimized-kernel-enable"}},
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
    auto it = parameterMap.constFind(key);
    const auto &pair = it.value();

    if (useShort && !pair.first.isEmpty()) {
        // short form
        return pair.first;
    }

    // long form (default)
    return pair.second;
}

/**
 * This method runs hashcat asynchronously using QtConcurrent and returns a future
 * that will contain the execution results.
 *
 * Usage Example:
 *
 * QFutureWatcher<HashcatResult> *watcher = new QFutureWatcher<HashcatResult>();
 * connect(watcher, &QFutureWatcher<HashcatResult>::finished, this, [this, watcher]() {
 *     const HashcatResult &result = watcher->result();
 *     // process contents of "result"
 *     watcher->deleteLater();
 * });
 * watcher->setFuture(HelperUtils::executeHashcat(QStringList() << "--help"));
 */
QFuture<HashcatResult> HelperUtils::executeHashcat(const QStringList &args, int timeoutMs) {
    return QtConcurrent::run([args, timeoutMs]() -> HashcatResult {
        HashcatResult result;
        QProcess proc;
        QStringList cmdArgs = args;
        const auto &settings = SettingsManager::instance();

        if (settings.getKey<QString>("hashcatPath").isEmpty()) {
            result.stderr = "hashcatPath not configured";
            return result;
        }

        // Always run in quiet mode when reading output
        cmdArgs << "--quiet";

        proc.setProgram(settings.getKey<QString>("hashcatPath"));
        proc.setArguments(cmdArgs);
        proc.setWorkingDirectory(QFileInfo(settings.getKey<QString>("hashcatPath")).absolutePath());

        proc.start();

        if (!proc.waitForStarted()) {
            result.stderr = "Failed to start hashcat";
            return result;
        }

        if (!proc.waitForFinished(timeoutMs)) {
            proc.kill();
            result.stderr = "hashcat timed out";
            return result;
        }

        result.exitStatus = proc.exitStatus();
        result.exitCode = proc.exitCode();
        result.stdout = proc.readAllStandardOutput();
        result.stderr = proc.readAllStandardError();

        return result;
    });
}

// Returns all supported terminals
QMap<QString, QStringList> HelperUtils::getAvailableTerminals()
{
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
