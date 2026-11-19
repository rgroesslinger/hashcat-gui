/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "terminalcatalog.h"

#include "appconstants.h"
#include "argumentquoting.h"
#include "executable.h"
#include "programdirectory.h"

#include <QProcess>
#include <QStringEncoder>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

// The Unix entries only select the terminal's command mode (kitty, foot and
// wezterm: the subcommand). Staying open, the working directory and the exit
// code are the wrapper's job. ptyxis is not offered: systemd-run expands "$"
// a second time and would change a hash or mask. See the FAQ.
const QMap<QString, QStringList> &TerminalCatalog::terminalArguments()
{
    static const QMap<QString, QStringList> table = {
        {QStringLiteral("powershell.exe"), {}},
        {QStringLiteral("pwsh"), {}},
        {QStringLiteral("xterm"), {QStringLiteral("-e")}},
        {QStringLiteral("gnome-terminal"), {QStringLiteral("--")}},
        {QStringLiteral("konsole"), {QStringLiteral("-e")}},
        {QStringLiteral("xfce4-terminal"), {QStringLiteral("-x")}},
        {QStringLiteral("alacritty"), {QStringLiteral("-e")}},
        {QStringLiteral("kitty"), {}},
        {QStringLiteral("foot"), {}},
        {QStringLiteral("wezterm"), {QStringLiteral("start"), QStringLiteral("--")}},
        {QStringLiteral("ghostty"), {QStringLiteral("-e")}},
    };
    return table;
}

QStringList TerminalCatalog::offeredTerminals()
{
    QStringList terminals;

    const QMap<QString, QStringList> &table = terminalArguments();
    for (auto it = table.constBegin(); it != table.constEnd(); ++it) {
#ifdef Q_OS_WIN
        // Windows has no sh for the Unix wrapper; only the PowerShell path
        // can start hashcat there.
        const bool offered = isPowerShell(it.key());
#else
        // pwsh is installed on Linux and macOS, but it is a shell, not a
        // terminal: started by the GUI it has no window and its output goes
        // nowhere. Only Windows creates a console for it (CREATE_NEW_CONSOLE).
        const bool offered = !isPowerShell(it.key());
#endif
        if (offered) {
            terminals << it.key();
        }
    }

    return terminals;
}

bool TerminalCatalog::isInstalled(const QString &terminal)
{
    return !Executable::resolve(terminal).isEmpty();
}

QMap<QString, QString> TerminalCatalog::availableTerminals()
{
    QMap<QString, QString> terminals;

    const QStringList offered = offeredTerminals();
    for (const QString &terminal : offered) {
        if (isInstalled(terminal)) {
            terminals.insert(terminal, Executable::resolve(terminal));
        }
    }

    return terminals;
}

bool TerminalCatalog::isPowerShell(const QString &terminal)
{
    return terminal == QStringLiteral("powershell.exe") || terminal == QStringLiteral("pwsh");
}

QString TerminalCatalog::findHashcatInPath()
{
    // QStandardPaths checks that the file exists and can be executed; every
    // name already carries the platform's suffix. The installed name comes
    // first, the release archive's second - see ExecutableNames.
    for (const QLatin1StringView name : AppConstants::Hashcat::ExecutableNames) {
        const QString path = Executable::resolve(QString(name));
        if (!path.isEmpty()) {
            return path;
        }
    }
    return {};
}

QString TerminalCatalog::noSupportedTerminalMessage()
{
#ifndef Q_OS_WIN
    // ptyxis is the default terminal on some distributions. When it is the
    // only one installed, naming the reason is more useful than the generic
    // "no terminal" text.
    if (!Executable::resolve(QStringLiteral("ptyxis")).isEmpty()) {
        return tr("No supported terminal was found. ptyxis is installed, but it changes “$” "
                  "values before hashcat sees them, so it is not offered. Install another "
                  "terminal; see the FAQ.");
    }
#endif
    return tr("No supported terminal was found on this system. "
              "hashcat-gui needs one to show the hashcat output.");
}

QString TerminalCatalog::powerShellScript(const QString &hashcatPath, const QStringList &arguments)
{
    return QStringLiteral("$si = New-Object System.Diagnostics.ProcessStartInfo\n$si.FileName = ")
           + powerShellQuoteArgument(hashcatPath) + QStringLiteral("\n$si.Arguments = ")
           + powerShellQuoteArgument(msvcrtJoinArguments(arguments))
           + QStringLiteral("\n$si.UseShellExecute = $false\n$si.WorkingDirectory = ")
           + powerShellQuoteArgument(ProgramDirectory::forProgram(hashcatPath))
           + QStringLiteral("\n$p = [System.Diagnostics.Process]::Start($si)\n"
                            "$p.WaitForExit()");
}

LaunchCommand TerminalCatalog::launchCommand(const QString &terminal,
                                             const QString &terminalProgram,
                                             const QString &hashcatPath,
                                             const QStringList &arguments, bool keepOpen)
{
    const QMap<QString, QStringList> &table = terminalArguments();
    const auto it = table.constFind(terminal);
    if (it == table.constEnd()) {
        LaunchCommand refused;
        refused.refusal = LaunchCommand::Refusal::UnknownTerminal;
        return refused;
    }

    // -Command would re-parse argv as PowerShell code; -EncodedCommand takes
    // the quoted script as UTF-16LE base64, and ProcessStartInfo.Arguments
    // keeps the collected argv verbatim on every PowerShell version.
    // WaitForExit holds the session: the script must not call exit afterwards.
    if (isPowerShell(terminal)) {
        const QString script = powerShellScript(hashcatPath, arguments);

        QStringEncoder encoder(QStringEncoder::Utf16LE);
        const QByteArray encodedBytes = encoder.encode(script);
        const QString encoded = QString::fromLatin1(encodedBytes.toBase64());

        // CreateProcessW refuses a command line longer than 32,767 characters.
        // The encoded script is the only large token; the margin covers the
        // program name, its quotes and the fixed flags. The same limit applies
        // on every platform, so a refusal does not depend on the OS.
        constexpr int maxCommandLine = 32767;
        constexpr int commandLineMargin = 64;
        if (terminalProgram.size() + encoded.size() + commandLineMargin > maxCommandLine) {
            LaunchCommand refused;
            refused.refusal = LaunchCommand::Refusal::CommandTooLong;
            return refused;
        }

        LaunchCommand command;
        command.program = terminalProgram;
        command.arguments << QStringLiteral("-NoProfile");
        if (keepOpen) {
            command.arguments << QStringLiteral("-NoExit");
        }
        command.arguments << QStringLiteral("-EncodedCommand") << encoded;
        return command;
    }

    LaunchCommand command;
    command.program = terminalProgram;
    command.arguments = it.value();

    // Fixed sh -c wrapper: cd to $1, run "$@" as positional parameters, then
    // print the exit code and hold open. The INT trap keeps dash alive on Ctrl-C
    // and is reset to default in the child so hashcat still gets the signal.
    const QString wrapper = QStringLiteral(R"(trap : INT
cd "$1" || {
    printf 'hashcat-gui: cannot enter %s\n' "$1" >&2
    printf '[press Enter to close]\n' >&2
    read -r _
    exit 1
}
shift
"$@"
rc=$?
printf '\n[hashcat exited with %s - press Enter to close]\n' "$rc"
read -r _)");
    command.arguments << QStringLiteral("sh") << QStringLiteral("-c")
                      << wrapper
                      // $0 for the wrapper, then the directory, the hashcat
                      // program and its argv as $1 ... $n.
                      << QStringLiteral("hashcat-gui") << ProgramDirectory::forProgram(hashcatPath)
                      << hashcatPath << arguments;
    return command;
}

#ifdef Q_OS_WIN
void TerminalCatalog::configureConsoleLaunch(QProcess::CreateProcessArguments *args)
{
    // CREATE_NEW_CONSOLE gives the terminal its own window, but QProcess still
    // sets STARTF_USESTDHANDLES with this process's handles, so the child would
    // write to the launching shell. Clear the flag so it gets the new console's.
    args->flags |= CREATE_NEW_CONSOLE;
    args->startupInfo->dwFlags &= ~STARTF_USESTDHANDLES;
}
#endif

bool TerminalCatalog::launchDetached(const LaunchCommand &command, const QString &hashcatPath,
                                     QString *errorMessage)
{
    QProcess proc;

#ifdef Q_OS_WIN
    proc.setCreateProcessArgumentsModifier(&TerminalCatalog::configureConsoleLaunch);
#endif

    proc.setProgram(command.program);
    proc.setArguments(command.arguments);
    proc.setWorkingDirectory(ProgramDirectory::forProgram(hashcatPath));

    // The member overload is required: only it runs the modifier set above.
    if (!proc.startDetached()) {
        if (errorMessage) {
            *errorMessage = proc.errorString();
        }
        return false;
    }
    return true;
}