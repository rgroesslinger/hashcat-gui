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

// The program and the arguments that start hashcat in a terminal. An empty
// program means the launch is refused: the terminal is unknown, or the
// command line would exceed the length a Windows process can take.
struct LaunchCommand {
    QString program;
    QStringList arguments;
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
    // through launchCommand(); their entries hold no arguments.
    static const QMap<QString, QStringList> &terminalArguments();

    static QMap<QString, QStringList> availableTerminals();

    // The hashcat executable on the PATH, empty when this system has none.
    //
    // The settings dialog only offers it: a configured path is never replaced
    // by the one this lookup finds.
    static QString findHashcatInPath();

    // The note the settings dialog and a failed launch both show when this
    // system has no terminal hashcat-gui can start hashcat in.
    static QString noSupportedTerminalMessage();

    // The command line that starts hashcat in the given terminal, built
    // without starting anything. PowerShell and pwsh get an -EncodedCommand
    // script that quotes every token and starts hashcat through
    // ProcessStartInfo, so no hash, mask, path or session value is ever
    // parsed as shell code or rewritten by PowerShell's own native argument
    // passing; every other terminal execs hashcat directly. keepOpen asks
    // the terminal to stay open after hashcat exits.
    static LaunchCommand launchCommand(const QString &terminal, const QString &hashcatPath,
                                       const QStringList &arguments, bool keepOpen);

    // Starts the terminal detached, in the hashcat binary's own directory.
    // Returns false and fills errorMessage when the start fails. The caller
    // owns the refusal of an empty command and every message box.
    static bool launchDetached(const LaunchCommand &command, const QString &hashcatPath,
                               QString *errorMessage);
};

#endif // TERMINALCATALOG_H