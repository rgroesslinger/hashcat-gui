/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "helperutils.h"
#include "settingsmanager.h"
#include "appconstants.h"
#include <QProcess>
#include <QString>
#include <QStandardPaths>
#include <QFileInfo>
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>

// Mapping of supported command line options
const QMap<HelperUtils::Parameter, QPair<QString, QString>> HelperUtils::parameterMap = {
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
    {HelperUtils::Parameter::SpeedOnly,         {"",    "--speed-only"}},
    {HelperUtils::Parameter::Username,          {"",    "--username"}},
    {HelperUtils::Parameter::WorkloadProfile,   {"-w",  "--workload-profile"}},
};

QString HelperUtils::getParameter(Parameter key, bool useShort)
{
    auto it = parameterMap.constFind(key);
    if (it == parameterMap.constEnd()) {
        // Every enumerator has an entry above, but dereferencing end() would
        // be undefined behaviour, so fail soft instead.
        return {};
    }
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
    // QSettings is only reentrant. The lambda below runs on the thread pool,
    // where reading the settings could race with the GUI thread writing them,
    // so read the path once on the calling thread and capture it by value.
    const QString hashcatPath = SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath);

    return QtConcurrent::run([args, timeoutMs, hashcatPath]() -> HashcatResult {
        HashcatResult result;
        QProcess proc;
        QStringList cmdArgs = args;

        if (hashcatPath.isEmpty()) {
            result.standardError = tr("The hashcat path is not configured.");
            return result;
        }

        // Always run in quiet mode when reading output
        cmdArgs << AppConstants::Hashcat::Quiet;

        proc.setProgram(hashcatPath);
        proc.setArguments(cmdArgs);
        proc.setWorkingDirectory(QFileInfo(hashcatPath).absolutePath());

        proc.start();

        if (!proc.waitForStarted()) {
            result.standardError = tr("Failed to start hashcat\n") + proc.errorString();
            return result;
        }

        if (!proc.waitForFinished(timeoutMs)) {
            proc.kill();
            // Reap the killed child instead of letting QProcess::waitForFinished
            // return immediately and destroy a still-running process.
            proc.waitForFinished(5000);
            result.standardError = tr("hashcat timed out\n") + proc.errorString();
            return result;
        }

        result.exitStatus = proc.exitStatus();
        result.exitCode = proc.exitCode();
        result.standardOutput = proc.readAllStandardOutput();
        result.standardError = proc.readAllStandardError();

        return result;
    });
}

// Returns what keeps a launch from happening, if anything
HelperUtils::LaunchError HelperUtils::validateLaunch(const QString &hashFile,
                                                     const QString &hashcatPath,
                                                     const QString &configuredTerminal,
                                                     const QStringList &availableTerminals,
                                                     QString *detail)
{
    if (detail) {
        detail->clear();
    }

    if (hashFile.isEmpty()) {
        return LaunchError::NoHashFile;
    }

    if (hashcatPath.isEmpty()) {
        return LaunchError::NoHashcatPath;
    }

    // Only absolute paths are checked here. A bare name is left to the shell
    // search of the terminal, and checking it would need the executable bit,
    // which QFileInfo does not report reliably for .exe files.
    const QFileInfo hashcatInfo(hashcatPath);
    if (hashcatInfo.isAbsolute() && !hashcatInfo.isFile()) {
        if (detail) {
            *detail = hashcatPath;
        }
        return LaunchError::HashcatPathMissing;
    }

    if (configuredTerminal.isEmpty()) {
        return LaunchError::NoTerminal;
    }

    if (availableTerminals.isEmpty()) {
        return LaunchError::NoTerminals;
    }

    if (!availableTerminals.contains(configuredTerminal)) {
        if (detail) {
            *detail = availableTerminals.join(QStringLiteral(", "));
        }
        return LaunchError::UnknownTerminal;
    }

    return LaunchError::None;
}

// Returns all supported terminals
QMap<QString, QStringList> HelperUtils::getAvailableTerminals()
{
    QMap<QString, QStringList> terminals;

    // List of terminals and needed arguments to launch them with an external
    // command. The arguments end up between the terminal program and the
    // hashcat command line, so each entry states exactly what that terminal's
    // own parser expects. Terminals whose option takes a single shell-quoted
    // string instead of an argument vector (terminator -e) cannot be
    // expressed here and are deliberately not listed.
    QMap<QString, QStringList> terminalMap = {
        {"cmd.exe", {"/k"}},
        {"xterm", {"-hold", "-e"}},
        {"gnome-terminal", {"--wait", "--"}},
        {"ptyxis", {"--"}},
        {"konsole", {"--hold", "-e"}},
        {"xfce4-terminal", {"--hold", "-e"}},
        {"alacritty", {"-e"}},        // -e consumes the rest of the arguments
        {"kitty", {}},                // the program is the first positional argument
        {"foot", {}},                 // trailing arguments are run as the command
        {"wezterm", {"start", "--"}}, // wezterm start -- <program>
        {"wt.exe", {"-d", "."}},      // Windows Terminal
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
