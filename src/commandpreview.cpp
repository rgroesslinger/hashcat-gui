/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "commandpreview.h"

#include "appconstants.h"
#include "textutils.h"

QString CommandPreview::format(const QString &binary, const QStringList &arguments)
{
    QStringList display = arguments;
#ifdef Q_OS_WIN
    const QChar quote = QLatin1Char('"');
#else
    const QChar quote = QLatin1Char('\'');
#endif
    const qsizetype recipe = display.indexOf(QString(AppConstants::Hashcat::HashRecipe));
    if (recipe >= 0 && recipe + 1 < display.size()) {
        display.replace(recipe + 1, quote + display.at(recipe + 1) + quote);
    }

    // The binary name comes first when one is configured; without it the
    // preview is the bare argument list, which must not start with a blank.
    return (binary.isEmpty() ? QString() : binary + QLatin1Char(' '))
           + display.join(QLatin1Char(' '));
}

QString CommandPreview::tooltip(const QString &command)
{
    if (command.isEmpty()) {
        return {};
    }

    const QStringList parts = command.split(QLatin1Char(' '));
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