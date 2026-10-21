/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "helperutils.h"
#include "settingsmanager.h"
#include "appconstants.h"
#include "commandbuilder.h"
#include <QProcess>
#include <QString>
#include <QStandardPaths>
#include <QFileInfo>
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>

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

// Returns what keeps the selected attack mode from running, if anything
HelperUtils::LaunchError HelperUtils::validateOptions(const HashcatOptions &options,
                                                      QString *detail)
{
    if (detail) {
        detail->clear();
    }

    const CommandBuilder::ModeRequirements requirements =
        CommandBuilder::modeRequirements(options.attackMode);
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

    // Hashcat refuses a minimum length that is larger than the maximum. The
    // builder emits the flag and its bounds only in the mask modes, so a
    // stale pair in a disabled group box must not block the other modes.
    if (options.increment && CommandBuilder::attackUsesMask(options.attackMode)
        && options.incrementMin > 0 && options.incrementMax > 0
        && options.incrementMin > options.incrementMax) {
        return LaunchError::InvalidIncrement;
    }

    // Two combinations hashcat answers with a usage error. The increment one
    // is gated like the bounds above, for the same reason: only a mask mode
    // puts -i on the command line.
    if (options.increment && CommandBuilder::attackUsesMask(options.attackMode)
        && options.speedOnly) {
        return LaunchError::IncrementWithSpeedOnly;
    }

    if (options.remove && options.attackMode == AttackMode::Association) {
        return LaunchError::RemoveInAssociation;
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
QMap<QString, QStringList> HelperUtils::availableTerminals()
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
