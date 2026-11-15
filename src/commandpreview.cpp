/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "commandpreview.h"

#include "argumentquoting.h"
#include "textutils.h"

namespace
{
// One token as the platform's shell reads it: the preview is copied to the
// clipboard and pasted into a shell, so a token the shell would split or
// expand is quoted, and a quoted token's own quotes are escaped.
QString displayToken(const QString &token)
{
#ifdef Q_OS_WIN
    return msvcrtQuoteArgument(token);
#else
    return posixQuoteArgument(token);
#endif
}

// The formatted command split back into its display tokens: at the spaces
// between arguments, not at the ones inside quotes. The quote rules match
// the ones displayToken() quoted with, so a quoted token stays one piece
// and the tooltip cannot break it across lines.
QStringList splitDisplayCommand(const QString &command)
{
#ifdef Q_OS_WIN
    const QChar quoteChar = QLatin1Char('"');
#else
    const QChar quoteChar = QLatin1Char('\'');
#endif
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
        if (c == QLatin1Char('\\')) {
            qsizetype run = 0;
            while (i + run < command.size() && command.at(i + run) == QLatin1Char('\\')) {
                ++run;
            }
#ifdef Q_OS_WIN
            const bool quoteFollows = i + run < command.size() && command.at(i + run) == quoteChar;
            // Inside quotes, an even backslash run lets the quote close; an
            // odd one escapes it and stays inside. Outside, a run before a
            // quote escapes it. The text is kept verbatim either way - the
            // splitter only decides where the tokens end.
            if (inQuotes && quoteFollows && run % 2 == 0) {
                part += command.mid(i, run + 1);
                inQuotes = false;
                i += run;
                continue;
            }
            if (quoteFollows) {
                part += command.mid(i, run + 1);
                i += run;
                continue;
            }
#else
            // Outside single quotes a backslash escapes the next character
            // (the '\'' splice); inside, it is literal.
            if (!inQuotes && i + 1 < command.size()) {
                part += c;
                part += command.at(++i);
                continue;
            }
#endif
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
} // namespace

QString CommandPreview::format(const QString &binary, const QStringList &arguments)
{
    QStringList display;
    display.reserve(arguments.size());
    for (const QString &argument : arguments) {
        display << displayToken(argument);
    }

    // The binary name comes first when one is configured; without it the
    // preview is the bare argument list, which must not start with a blank.
    return (binary.isEmpty() ? QString() : displayToken(binary) + QLatin1Char(' '))
           + display.join(QLatin1Char(' '));
}

QString CommandPreview::tooltip(const QString &command)
{
    if (command.isEmpty()) {
        return {};
    }

    const QStringList parts = splitDisplayCommand(command);
    QStringList lines;
    QString line;
    for (const QString &part : parts) {
        const bool endsWithFlag = line.section(QLatin1Char(' '), -1).startsWith(QLatin1Char('-'));
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
