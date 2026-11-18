/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef COMMANDPREVIEW_H
#define COMMANDPREVIEW_H

#include <QString>
#include <QStringList>
#include <cstdint>

// The command line as the preview and the clipboard show it. Display-only:
// the string never feeds the launch path, which quotes for the shell on its
// own (TerminalCatalog::launchCommand).
class CommandPreview
{
public:
    // The shell the copied command is meant for. It follows the configured
    // terminal, not the platform the GUI happens to run on.
    enum class Dialect : std::uint8_t {
        Posix,      // /bin/sh and its relatives: single-quoted tokens
        PowerShell, // powershell.exe and pwsh: call operator, single quotes
    };

    // The binary name (empty when unconfigured) and one quoted token per
    // argument, joined with spaces, so the command can be pasted into the
    // dialect's shell. The argv handed to hashcat stays unquoted.
    static QString format(const QString &binary, const QStringList &arguments, Dialect dialect);

    // The tooltip shows the whole command, plain text so it keeps its line
    // breaks, with the arguments joined at 120 columns. Markup-like text is
    // escaped; a rich-text tooltip would grow tall instead of wide.
    static QString tooltip(const QString &command, Dialect dialect);
};

#endif // COMMANDPREVIEW_H