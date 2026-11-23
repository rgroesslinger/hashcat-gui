/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfileencoding.h"

#include <QLatin1Char>

namespace PotfileEncoding
{

bool isHexDigit(QChar character)
{
    return (character >= QLatin1Char('0') && character <= QLatin1Char('9'))
           || (character >= QLatin1Char('a') && character <= QLatin1Char('f'))
           || (character >= QLatin1Char('A') && character <= QLatin1Char('F'));
}

bool decodeHex(const QString &text, QByteArray *bytes)
{
    if (text.size() % 2 != 0) {
        return false;
    }
    for (const QChar character : text) {
        if (!isHexDigit(character)) {
            return false;
        }
    }
    *bytes = QByteArray::fromHex(text.toLatin1());
    return true;
}

QString decodePlain(const QString &plain)
{
    static const QString prefix = QStringLiteral("$HEX[");
    if (!plain.startsWith(prefix) || !plain.endsWith(QLatin1Char(']'))) {
        return plain;
    }

    QByteArray bytes;
    const QString hex = plain.mid(prefix.size(), plain.size() - prefix.size() - 1);
    if (!decodeHex(hex, &bytes)) {
        return plain;
    }

    // hashcat hexifies non-printable UTF-8, so the payload is often not valid
    // UTF-8. Keep the literal $HEX[…] token in that case: it is the lossless
    // potfile representation for display, copy and export.
    QString decoded = QString::fromUtf8(bytes);
    if (decoded.toUtf8() != bytes) {
        return plain;
    }
    return decoded;
}

} // namespace PotfileEncoding