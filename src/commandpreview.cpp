/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "commandpreview.h"

#include "argumentquoting.h"
#include "textutils.h"

namespace
{
// One token as the dialect's shell reads it: the preview is copied to the
// clipboard and pasted into a shell, so a token the shell would split or
// expand is quoted, and a quoted token's own quotes are escaped. Tokens the
// shell reads literally stay bare so the command remains readable.
QString displayToken(const QString &token, CommandPreview::Dialect dialect)
{
    return dialect == CommandPreview::Dialect::PowerShell ? powerShellDisplayArgument(token)
                                                          : posixQuoteArgument(token);
}

// The formatted command split back into its display tokens: at the spaces
// between arguments, not at the ones inside quotes. The quote rules match
// the ones displayToken() quoted with, so a quoted token stays one piece and
// the tooltip cannot break it across lines.
QStringList splitDisplayCommand(const QString &command, CommandPreview::Dialect dialect)
{
    // Both dialects quote with the POSIX single quote; the difference is how
    // the token escapes it - a '\'' splice on POSIX, a doubled quote in
    // PowerShell - and that backslash is literal in PowerShell.
    const QChar quoteChar = QLatin1Char('\'');
    QStringList parts;
    QString part;
    bool inQuotes = false;
    for (qsizetype i = 0; i < command.size(); ++i) {
        const QChar c = command.at(i);
        if (!inQuotes && c == QLatin1Char(' ')) {
            if (!part.isEmpty()) {
                parts << part;
                part.clear();
            }
            continue;
        }
        if (c == QLatin1Char('\\') && dialect == CommandPreview::Dialect::Posix) {
            // Outside single quotes a backslash escapes the next character
            // (the '\'' splice); inside, it is literal.
            if (!inQuotes && i + 1 < command.size()) {
                part += c;
                part += command.at(++i);
                continue;
            }
            qsizetype run = 0;
            while (i + run < command.size() && command.at(i + run) == QLatin1Char('\\')) {
                ++run;
            }
            part += command.mid(i, run);
            i += run - 1;
            continue;
        }
        if (c == quoteChar) {
            inQuotes = !inQuotes;
        }
        part += c;
    }
    if (!part.isEmpty()) {
        parts << part;
    }
    return parts;
}

// The last token of a line, without the quotes a display token may carry, so
// the wrap can tell a flag from a value in either dialect.
QString lastToken(const QString &line)
{
    QString token = line.section(QLatin1Char(' '), -1);
    if (token.startsWith(QLatin1Char('\''))) {
        token.remove(0, 1);
    }
    return token;
}
} // namespace

QString CommandPreview::format(const QString &binary, const QStringList &arguments, Dialect dialect)
{
    QStringList display;
    display.reserve(arguments.size());
    for (const QString &argument : arguments) {
        display << displayToken(argument, dialect);
    }

    // The binary name comes first when one is configured; without it the
    // preview is the bare argument list, which must not start with a blank.
    if (binary.isEmpty()) {
        return display.join(QLatin1Char(' '));
    }

    const QString called = displayToken(binary, dialect) + QLatin1Char(' ');
    return (dialect == Dialect::PowerShell ? QStringLiteral("& ") : QString()) + called
           + display.join(QLatin1Char(' '));
}

QString CommandPreview::tooltip(const QString &command, Dialect dialect)
{
    if (command.isEmpty()) {
        return {};
    }

    const QStringList parts = splitDisplayCommand(command, dialect);
    QStringList lines;
    QString line;
    for (const QString &part : parts) {
        // A line that ends in the call operator or in a flag keeps the value
        // that follows it, even past the limit.
        const QString token = lastToken(line);
        const bool endsWithFlag = line == QStringLiteral("&") || token.startsWith(QLatin1Char('-'));
        // The space between the parts counts too: the line is one character
        // longer than the two of them together.
        if (!line.isEmpty() && line.size() + part.size() + 1 > 120 && !endsWithFlag) {
            lines << line;
            line = part;
        } else {
            line += (line.isEmpty() ? QString() : QStringLiteral(" ")) + part;
        }
    }
    if (!line.isEmpty()) {
        lines << line;
    }

    return escapeRichText(lines.join(QLatin1Char('\n')));
}