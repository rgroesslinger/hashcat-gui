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
        {Parameter::AttackMode, {QStringLiteral("-a"), QStringLiteral("--attack-mode")}},
        {Parameter::BackendDevices, {QStringLiteral("-d"), QStringLiteral("--backend-devices")}},
        {Parameter::CpuAffinity, {QString(), QStringLiteral("--cpu-affinity")}},
        {Parameter::CustomCharset1, {QStringLiteral("-1"), QStringLiteral("--custom-charset1")}},
        {Parameter::CustomCharset2, {QStringLiteral("-2"), QStringLiteral("--custom-charset2")}},
        {Parameter::CustomCharset3, {QStringLiteral("-3"), QStringLiteral("--custom-charset3")}},
        {Parameter::CustomCharset4, {QStringLiteral("-4"), QStringLiteral("--custom-charset4")}},
        {Parameter::EncodingFrom, {QString(), QStringLiteral("--encoding-from")}},
        {Parameter::EncodingTo, {QString(), QStringLiteral("--encoding-to")}},
        {Parameter::Force, {QString(), QStringLiteral("--force")}},
        {Parameter::GenerateRules, {QStringLiteral("-g"), QStringLiteral("--generate-rules")}},
        {Parameter::HashType, {QStringLiteral("-m"), QStringLiteral("--hash-type")}},
        {Parameter::HexCharset, {QString(), QStringLiteral("--hex-charset")}},
        {Parameter::HexSalt, {QString(), QStringLiteral("--hex-salt")}},
        {Parameter::HexWordlist, {QString(), QStringLiteral("--hex-wordlist")}},
        {Parameter::HwmonTempAbort, {QString(), QStringLiteral("--hwmon-temp-abort")}},
        {Parameter::Increment, {QStringLiteral("-i"), QStringLiteral("--increment")}},
        {Parameter::IncrementMax, {QString(), QStringLiteral("--increment-max")}},
        {Parameter::IncrementMin, {QString(), QStringLiteral("--increment-min")}},
        {Parameter::Limit, {QStringLiteral("-l"), QStringLiteral("--limit")}},
        {Parameter::OpenclDeviceTypes,
         {QStringLiteral("-D"), QStringLiteral("--opencl-device-types")}},
        {Parameter::OptimizedKernel,
         {QStringLiteral("-O"), QStringLiteral("--optimized-kernel-enable")}},
        {Parameter::Outfile, {QStringLiteral("-o"), QStringLiteral("--outfile")}},
        {Parameter::OutfileFormat, {QString(), QStringLiteral("--outfile-format")}},
        {Parameter::PotfileDisable, {QString(), QStringLiteral("--potfile-disable")}},
        {Parameter::Remove, {QString(), QStringLiteral("--remove")}},
        {Parameter::RuleLeft, {QStringLiteral("-j"), QStringLiteral("--rule-left")}},
        {Parameter::RuleRight, {QStringLiteral("-k"), QStringLiteral("--rule-right")}},
        {Parameter::RulesFile, {QStringLiteral("-r"), QStringLiteral("--rules-file")}},
        {Parameter::Runtime, {QString(), QStringLiteral("--runtime")}},
        {Parameter::SegmentSize, {QStringLiteral("-c"), QStringLiteral("--segment-size")}},
        {Parameter::Session, {QString(), QStringLiteral("--session")}},
        {Parameter::Skip, {QStringLiteral("-s"), QStringLiteral("--skip")}},
        {Parameter::SpeedOnly, {QString(), QStringLiteral("--speed-only")}},
        {Parameter::Status, {QString(), QStringLiteral("--status")}},
        {Parameter::StatusTimer, {QString(), QStringLiteral("--status-timer")}},
        {Parameter::Username, {QString(), QStringLiteral("--username")}},
        {Parameter::WorkloadProfile, {QStringLiteral("-w"), QStringLiteral("--workload-profile")}},
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

// Runs hashcat asynchronously; the caller attaches a QFutureWatcher and reads
// the result in its finished handler:
//   auto *w = new QFutureWatcher<HashcatResult>(this);
//   connect(w, &QFutureWatcher<HashcatResult>::finished, this, ...);
//   w->setFuture(HelperUtils::executeHashcat({"--help"}));
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
        // QProcess hands back raw bytes; hashcat speaks UTF-8, so decode them
        // as such rather than through the conversion this project compiles without.
        result.standardOutput = QString::fromUtf8(proc.readAllStandardOutput());
        result.standardError = QString::fromUtf8(proc.readAllStandardError());

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

// What the user has to provide for the attack mode: hashcat's own argument
// validation decides which numbers of word lists a mode accepts.
HelperUtils::ModeRequirements HelperUtils::modeRequirements(AttackMode mode)
{
    switch (mode) {
    case AttackMode::Combination:
        // Combination multiplies two word lists. Hashcat answers every other
        // number with a usage error.
        return {2, 2, false};
    case AttackMode::BruteForce:
        // The mask is the whole attack; word lists are not read at all.
        return {0, -1, true};
    case AttackMode::HybridWordMask:
    case AttackMode::HybridMaskWord:
        // One word list and one mask.
        return {1, -1, true};
    case AttackMode::Straight:
    case AttackMode::Association:
        // Any number of word lists, but at least one.
        return {1, -1, false};
    }

    return {1, -1, false};
}

// Returns what keeps the selected attack mode from running, if anything
HelperUtils::LaunchError HelperUtils::validateOptions(const HashcatOptions &options,
                                                      QString *detail)
{
    if (detail) {
        detail->clear();
    }

    const ModeRequirements requirements = modeRequirements(options.attackMode);
    const int wordlistCount = static_cast<int>(options.wordlists.size());

    if (wordlistCount < requirements.minWordlists) {
        if (requirements.maxWordlists == requirements.minWordlists) {
            // The mode wants an exact number of word lists.
            if (detail) {
                *detail = QString::number(wordlistCount);
            }
            return LaunchError::WrongWordlistCount;
        }
        return LaunchError::NoWordlist;
    }

    if (requirements.maxWordlists >= 0 && wordlistCount > requirements.maxWordlists) {
        if (detail) {
            *detail = QString::number(wordlistCount);
        }
        return LaunchError::WrongWordlistCount;
    }

    // A mask mode without a mask would run an unintended default mask.
    if (requirements.needsMask && options.mask.trimmed().isEmpty()) {
        return LaunchError::NoMask;
    }

    // Hashcat refuses a minimum length that is larger than the maximum.
    if (options.increment && options.incrementMin > 0 && options.incrementMax > 0
        && options.incrementMin > options.incrementMax) {
        return LaunchError::InvalidIncrement;
    }

    return LaunchError::None;
}

// The arguments that belong between the terminal program and the hashcat
// command line, for every supported terminal. The entries are pure data -
// nothing here looks at the filesystem. Terminals whose option takes a single
// shell-quoted string (terminator -e) are deliberately not listed.
const QMap<QString, QStringList> &HelperUtils::terminalArguments()
{
    static const QMap<QString, QStringList> table = {
        {QStringLiteral("cmd.exe"), {QStringLiteral("/k")}},
        {QStringLiteral("xterm"), {QStringLiteral("-hold"), QStringLiteral("-e")}},
        {QStringLiteral("gnome-terminal"), {QStringLiteral("--wait"), QStringLiteral("--")}},
        {QStringLiteral("ptyxis"), {QStringLiteral("--")}},
        {QStringLiteral("konsole"), {QStringLiteral("--hold"), QStringLiteral("-e")}},
        {QStringLiteral("xfce4-terminal"), {QStringLiteral("--hold"), QStringLiteral("-x")}},
        {QStringLiteral("alacritty"), {QStringLiteral("-e")}},
        {QStringLiteral("kitty"), {QStringLiteral("--hold")}},
        {QStringLiteral("foot"), {QStringLiteral("--hold")}},
        {QStringLiteral("wezterm"), {QStringLiteral("start"), QStringLiteral("--")}},
        {QStringLiteral("wt.exe"), {QStringLiteral("-d"), QStringLiteral(".")}},
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
