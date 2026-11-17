/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef TERMINALCATALOG_H
#define TERMINALCATALOG_H

#include <QCoreApplication>
#include <QMap>
#include <QString>
#include <QStringList>
#include <cstdint>

// The program and the arguments that start hashcat in a terminal, and - when
// they could not be built - why. A refusal type keeps the caller from showing
// the "too long" message for an unknown terminal.
struct LaunchCommand {
    enum class Refusal : std::uint8_t {
        None,
        UnknownTerminal,
        CommandTooLong,
    };

    QString program;
    QStringList arguments;
    Refusal refusal = Refusal::None;
};

// The supported terminals: discovery, the launch command line, and the one
// place a terminal process is started.
class TerminalCatalog
{
    // The message ends up in dialogs opened by the caller
    Q_DECLARE_TR_FUNCTIONS(TerminalCatalog)

public:
    // Every supported terminal and the arguments that belong between the
    // terminal program and the hashcat command line. Pure data - nothing
    // here looks at the filesystem - so the tests can pin every entry.
    // PowerShell and pwsh are discovered through this table but launched
    // through launchCommand(); their entries hold no arguments. Both shells
    // stay in the table on every platform so the script branch and its tests
    // are the same everywhere; availableTerminals() offers only them on Windows.
    static const QMap<QString, QStringList> &terminalArguments();

    // Every supported terminal that is installed, mapped to the absolute
    // program that starts it.
    static QMap<QString, QString> availableTerminals();

    // True for the two shells that take the encoded script instead of a
    // plain argv: powershell.exe and pwsh.
    static bool isPowerShell(const QString &terminal);

    // The hashcat executable on the PATH, empty when this system has none.
    //
    // The settings dialog only offers it: a configured path is never replaced
    // by the one this lookup finds.
    static QString findHashcatInPath();

    // The note the settings dialog and a failed launch both show when this
    // system has no terminal hashcat-gui can start hashcat in.
    static QString noSupportedTerminalMessage();

    // The command line that starts hashcat in the given terminal, built
    // without starting anything. terminalProgram is the resolved absolute
    // path from availableTerminals(); the name selects the terminal's own
    // arguments and the PowerShell branch. PowerShell and pwsh get an
    // -EncodedCommand script that quotes every token and starts hashcat
    // through ProcessStartInfo, so no hash, mask, path or session value is
    // ever parsed as shell code or rewritten by PowerShell's own native
    // argument passing; every other terminal runs a fixed sh wrapper with
    // the command as positional parameters. keepOpen controls -NoExit for
    // PowerShell; the Unix wrapper always holds the window open.
    static LaunchCommand launchCommand(const QString &terminal, const QString &terminalProgram,
                                       const QString &hashcatPath, const QStringList &arguments,
                                       bool keepOpen);

    // The readable PowerShell script that starts hashcat through
    // ProcessStartInfo. launchCommand() encodes exactly this script, and the
    // Tools action copies it unencoded, so the two cannot drift apart.
    static QString powerShellScript(const QString &hashcatPath, const QStringList &arguments);

    // Starts the terminal detached, in the hashcat binary's own directory.
    // Returns false and fills errorMessage when the start fails. The caller
    // owns the refusal of an empty command and every message box.
    static bool launchDetached(const LaunchCommand &command, const QString &hashcatPath,
                               QString *errorMessage);
};

#endif // TERMINALCATALOG_H