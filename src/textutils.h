/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef TEXTUTILS_H
#define TEXTUTILS_H

#include <QLocale>
#include <QString>
#include <QTextDocument>

// A tooltip that contains something like "<b>" would be read as markup and
// mangle the display. Escape such text once, and turn its breaks into <br> for
// the rich text renderer. Plain text is left alone: escaping it would show
// entities such as &amp; literally.
inline QString escapeRichText(QString text)
{
    if (!Qt::mightBeRichText(text)) {
        return text;
    }
    return text.toHtmlEscaped().replace(QLatin1Char('\n'), QStringLiteral("<br>"));
}

// Picks a singular or plural template. The caller passes already-translated
// strings, and the plural one carries the count placeholder.
inline QString countText(int count, const QString &one, const QString &many)
{
    return count == 1 ? one : many.arg(count);
}

// A count with the locale's group separators.
inline QString formatCount(int count)
{
    return QLocale().toString(count);
}

#endif // TEXTUTILS_H