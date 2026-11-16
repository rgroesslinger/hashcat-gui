/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "terminalcatalog.h"

#include "appconstants.h"
#include "argumentquoting.h"
#include "executable.h"

#include <QFileInfo>
#include <QProcess>
#include <QStringEncoder>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace
{
// The directory hashcat runs in: the canonical directory of its program, so a
// symlink in ~/bin does not send it looking for its kernels in ~/bin. The
// program path stays the resolved one; only the directory follows the link.
QString programDirectory(const QString &programPath)
{
    // canonicalFilePath() follows the symlink to the real file; an
    // unresolvable path (a test path, or one that vanished) keeps the plain
    // absolute directory instead of Qt's "." fallback.
    const QString canonical = QFileInfo(programPath).canonicalFilePath();
    if (canonical.isEmpty()) {
        return QFileInfo(programPath).absolutePath();
    }
    return QFileInfo(canonical).absolutePath();
}
} // namespace

// The arguments that belong between the terminal program and the hashcat
// command line, for every supported terminal. The entries are pure data -
// nothing here looks at the filesystem.
//
// PowerShell and pwsh are discovered through this table but launched through
// launchCommand(), which builds an -EncodedCommand script; their entries hold
// no arguments, so no -Command text can be assembled from the table.
//
// The remaining entries only select the terminal's command mode (or, for
// kitty, foot and wezterm, the subcommand). Staying open, holding the
// working directory and showing the exit code are the wrapper's job now;
// the per-terminal hold flags could not do all three everywhere.
//
// ptyxis is not offered: it starts the command through systemd-run, which
// expands "$" words a second time, so a bcrypt hash or a mask arrived at
// hashcat changed. See the FAQ.
const QMap<QString, QStringList> &TerminalCatalog::terminalArguments()
{
    static const QMap<QString, QStringList> table = {
        // The table holds both PowerShell shells on every platform so the script
        // branch and its tests are the same everywhere; availableTerminals()
        // offers only them on Windows, where a console can be created and no sh
        // exists for the Unix wrapper.
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

QMap<QString, QString> TerminalCatalog::availableTerminals()
{
    QMap<QString, QString> terminals;

    const QMap<QString, QStringList> &table = terminalArguments();
    for (auto it = table.constBegin(); it != table.constEnd(); ++it) {
#ifdef Q_OS_WIN
        // Windows has no sh for the Unix wrapper; only the PowerShell path
        // can start hashcat there.
        if (!isPowerShell(it.key())) {
            continue;
        }
#else
        // pwsh is installed on Linux and macOS, but it is a shell, not a
        // terminal: started by the GUI it has no window and its output goes
        // nowhere. Only Windows creates a console for it (CREATE_NEW_CONSOLE).
        if (isPowerShell(it.key())) {
            continue;
        }
#endif
        const QString path = Executable::resolve(it.key());
        if (!path.isEmpty()) {
            terminals.insert(it.key(), path);
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
    return tr("No supported terminal was found on this system. "
              "hashcat-gui needs one to show the hashcat output.");
}

QString TerminalCatalog::powerShellScript(const QString &hashcatPath, const QStringList &arguments)
{
    return QStringLiteral("$si = New-Object System.Diagnostics.ProcessStartInfo\n$si.FileName = ")
           + powerShellQuoteArgument(hashcatPath) + QStringLiteral("\n$si.Arguments = ")
           + powerShellQuoteArgument(msvcrtJoinArguments(arguments))
           + QStringLiteral("\n$si.UseShellExecute = $false\n$si.WorkingDirectory = ")
           + powerShellQuoteArgument(programDirectory(hashcatPath))
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

    // -Command would re-parse the hashcat command line as PowerShell code:
    // hashes, masks, outfiles and session values would run as expressions.
    // -EncodedCommand takes the script as UTF-16LE base64 instead, and the
    // script quotes every token, so nothing a session carries can execute.
    //
    // The script starts hashcat through ProcessStartInfo, not through
    // PowerShell's own native invocation: Windows PowerShell 5.1 (and pwsh
    // before 7.3) does not escape embedded quotes on the way to a native
    // command line and drops empty arguments. ProcessStartInfo.Arguments is
    // used verbatim by CreateProcess, so the MSVCRT quoting here is the only
    // transformation between the collected argv and hashcat's argv, on every
    // PowerShell version.
    //
    // WaitForExit holds the session while hashcat runs; the script must not
    // call exit afterwards - an explicit exit ends the session even under
    // -NoExit and would close the terminal the moment hashcat is done.
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

    // Every Unix terminal runs the same wrapper. The hashcat command reaches
    // sh -c only as positional parameters, so no hash, mask, path or session
    // value is ever parsed as shell code. The wrapper sets the working
    // directory itself - a terminal that hands the command to a server
    // process may not inherit ours - reports a directory it cannot enter,
    // and holds the window open with the exit code after hashcat ends.
    //
    // The INT trap is a handler, not an ignored signal: dash otherwise dies
    // with hashcat on Ctrl-C and the window closes before the status line.
    // A trapped signal is reset to the default in the child, so hashcat still
    // receives Ctrl-C normally.
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
                      << QStringLiteral("hashcat-gui") << programDirectory(hashcatPath)
                      << hashcatPath << arguments;
    return command;
}

bool TerminalCatalog::launchDetached(const LaunchCommand &command, const QString &hashcatPath,
                                     QString *errorMessage)
{
    QProcess proc;

#ifdef Q_OS_WIN
    /* Need CREATE_NEW_CONSOLE flag on windows to spawn visible terminal */
    proc.setCreateProcessArgumentsModifier(
        [](QProcess::CreateProcessArguments *args) { args->flags |= CREATE_NEW_CONSOLE; });
#endif

    proc.setProgram(command.program);
    proc.setArguments(command.arguments);
    proc.setWorkingDirectory(programDirectory(hashcatPath));

    if (!proc.startDetached()) {
        if (errorMessage) {
            *errorMessage = proc.errorString();
        }
        return false;
    }
    return true;
}