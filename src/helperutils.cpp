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

QString HelperUtils::getParameter(Parameter key, bool useShort)
{
    // Built once, on first call: a namespace-scope QMap with runtime
    // initialization could throw during pre-main static initialization and
    // end in std::terminate, so the table is a deferred function-local static.
    static const QMap<Parameter, QPair<QString, QString>> parameterMap = {
        {Parameter::AttackMode, {"-a", "--attack-mode"}},
        {Parameter::BackendDevices, {"-d", "--backend-devices"}},
        {Parameter::CpuAffinity, {"", "--cpu-affinity"}},
        {Parameter::CustomCharset1, {"-1", "--custom-charset1"}},
        {Parameter::CustomCharset2, {"-2", "--custom-charset2"}},
        {Parameter::CustomCharset3, {"-3", "--custom-charset3"}},
        {Parameter::CustomCharset4, {"-4", "--custom-charset4"}},
        {Parameter::GenerateRules, {"-g", "--generate-rules"}},
        {Parameter::HashType, {"-m", "--hash-type"}},
        {Parameter::HexCharset, {"", "--hex-charset"}},
        {Parameter::HexSalt, {"", "--hex-salt"}},
        {Parameter::OptimizedKernel, {"-O", "--optimized-kernel-enable"}},
        {Parameter::Outfile, {"-o", "--outfile"}},
        {Parameter::OutfileFormat, {"", "--outfile-format"}},
        {Parameter::Remove, {"", "--remove"}},
        {Parameter::RulesFile, {"-r", "--rules-file"}},
        {Parameter::SegmentSize, {"-c", "--segment-size"}},
        {Parameter::SpeedOnly, {"", "--speed-only"}},
        {Parameter::Username, {"", "--username"}},
        {Parameter::WorkloadProfile, {"-w", "--workload-profile"}},
    };

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
QFuture<HashcatResult> HelperUtils::executeHashcat(const QStringList &args, int timeoutMs)
{
    // QSettings is only reentrant. The lambda below runs on the thread pool,
    // where reading the settings could race with the GUI thread writing them,
    // so read the path once on the calling thread and capture it by value.
    const QString hashcatPath =
        SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath);

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
            result.standardError = tr("Failed to start hashcat\n%1").arg(proc.errorString());
            return result;
        }

        if (!proc.waitForFinished(timeoutMs)) {
            proc.kill();
            // Reap the killed child instead of letting QProcess::waitForFinished
            // return immediately and destroy a still-running process.
            proc.waitForFinished(5000);
            result.standardError = tr("hashcat timed out\n%1").arg(proc.errorString());
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

// The arguments that belong between the terminal program and the hashcat
// command line, for every supported terminal. The entries are pure data -
// nothing here looks at the filesystem - so the tests can pin what each one
// promises. Terminals whose option takes a single shell-quoted string
// instead of an argument vector (terminator -e) cannot be expressed here
// and are deliberately not listed.
const QMap<QString, QStringList> &HelperUtils::terminalArguments()
{
    static const QMap<QString, QStringList> table = {
        {"cmd.exe", {"/k"}},
        {"xterm", {"-hold", "-e"}},
        {"gnome-terminal", {"--wait", "--"}},
        {"ptyxis", {"--"}},
        {"konsole", {"--hold", "-e"}},
        {"xfce4-terminal", {"--hold", "-x"}},
        {"alacritty", {"-e"}},        // -e consumes the rest of the arguments
        {"kitty", {}},                // the program is the first positional argument
        {"foot", {}},                 // trailing arguments are run as the command
        {"wezterm", {"start", "--"}}, // wezterm start -- <program>
        {"wt.exe", {"-d", "."}},      // Windows Terminal
    };
    return table;
}

// Returns all supported terminals
QMap<QString, QStringList> HelperUtils::getAvailableTerminals()
{
    QMap<QString, QStringList> terminals;

    // Check which ones are actually available
    const QMap<QString, QStringList> &table = terminalArguments();
    for (auto it = table.constBegin(); it != table.constEnd(); ++it) {
        QString fullPath = QStandardPaths::findExecutable(it.key());
        if (!fullPath.isEmpty()) {
            terminals.insert(it.key(), it.value());
        }
    }

    return terminals;
}
