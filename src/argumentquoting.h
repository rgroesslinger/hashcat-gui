/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef ARGUMENTQUOTING_H
#define ARGUMENTQUOTING_H

#include <QString>
#include <QStringList>

// Command-line quoting for the places a command line is assembled as text:
// the PowerShell launch script, whose Arguments string goes to CreateProcess
// verbatim, and the preview/clipboard, which follows the shell the user will
// paste into. Every other launch path passes argv as a list and needs no
// quoting at all.

// One token for the MSVCRT / CommandLineToArgvW parser: an embedded quote is
// escaped with a backslash and the backslashes before it are doubled; a token
// that is empty or holds whitespace is wrapped in quotes, with its trailing
// backslashes doubled so they cannot escape the closing quote.
inline QString msvcrtQuoteArgument(const QString &token)
{
    QString quoted;
    quoted.reserve(token.size() + 2);
    int backslashes = 0;
    bool needsQuotes = token.isEmpty();
    for (const QChar c : token) {
        if (c == QLatin1Char('\\')) {
            ++backslashes;
            quoted += c;
            continue;
        }
        if (c == QLatin1Char('"')) {
            // 2n backslashes stay n literal ones, the extra one escapes the quote.
            quoted += QString(backslashes + 1, QLatin1Char('\\'));
        } else if (c.isSpace()) {
            needsQuotes = true;
        }
        quoted += c;
        backslashes = 0;
    }
    if (!needsQuotes) {
        return quoted;
    }
    quoted += QString(backslashes, QLatin1Char('\\'));
    return QLatin1Char('"') + quoted + QLatin1Char('"');
}

// The argument list as one MSVCRT command-line tail, the tokens joined with
// spaces. This is what ProcessStartInfo.Arguments takes.
inline QString msvcrtJoinArguments(const QStringList &arguments)
{
    QStringList quoted;
    quoted.reserve(arguments.size());
    for (const QString &argument : arguments) {
        quoted << msvcrtQuoteArgument(argument);
    }
    return quoted.join(QLatin1Char(' '));
}

// One token for a POSIX shell: left alone while it is non-empty and made
// only of characters the shell reads literally, otherwise wrapped in single
// quotes with every embedded quote spliced out as '\''.
inline QString posixQuoteArgument(const QString &token)
{
    const auto literal = [](QChar c) {
        const char16_t u = c.unicode();
        return (u >= u'a' && u <= u'z') || (u >= u'A' && u <= u'Z') || (u >= u'0' && u <= u'9')
               || u == u'_' || u == u'@' || u == u'%' || u == u'+' || u == u'=' || u == u':'
               || u == u',' || u == u'.' || u == u'/' || u == u'-';
    };
    bool safe = !token.isEmpty();
    for (const QChar c : token) {
        if (!literal(c)) {
            safe = false;
            break;
        }
    }
    if (safe) {
        return token;
    }
    QString quoted = token;
    quoted.replace(QLatin1Char('\''), QStringLiteral("'\\''"));
    return QLatin1Char('\'') + quoted + QLatin1Char('\'');
}

// PowerShell reads U+2018-U+201B as single-quote delimiters too, so a token
// that holds one cannot be quoted literally.
inline bool powerShellNeedsBase64(const QString &token)
{
    for (const QChar c : token) {
        if (c >= QChar(0x2018) && c <= QChar(0x201b)) {
            return true;
        }
    }
    return false;
}

// One token as a PowerShell expression: a single-quoted literal with an
// embedded quote doubled, or - for the smart quotes PowerShell would read as
// delimiters - the token's UTF-8 bytes as base64, decoded inside the script.
// Everything else ($, backticks, ;) is literal inside single quotes.
inline QString powerShellQuoteArgument(const QString &token)
{
    if (powerShellNeedsBase64(token)) {
        return QStringLiteral("([Text.Encoding]::UTF8.GetString([Convert]::FromBase64String('")
               + QString::fromLatin1(token.toUtf8().toBase64()) + QStringLiteral("')))");
    }
    QString quoted = token;
    quoted.replace(QLatin1Char('\''), QStringLiteral("''"));
    return QLatin1Char('\'') + quoted + QLatin1Char('\'');
}

#endif // ARGUMENTQUOTING_H
