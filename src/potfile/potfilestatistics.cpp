/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfilestatistics.h"

#include <QChar>
#include <QSet>

#include <algorithm>
#include <climits>

PotfileStatistics::Result PotfileStatistics::calculate(const QList<PotfileExporter::Row> &rows)
{
    Result result;
    result.entries = static_cast<int>(rows.size());

    QSet<QString> unique;
    qint64 totalLength = 0;
    int minimum = INT_MAX;

    for (const PotfileExporter::Row &row : rows) {
        if (row.duplicate) {
            ++result.duplicate;
        }
        if (row.encrypted) {
            ++result.encrypted;
            continue;
        }
        if (row.plaintext.isEmpty()) {
            ++result.empty;
            continue;
        }

        ++result.analyzed;
        bool lowercase = false;
        bool uppercase = false;
        bool digits = false;
        bool symbols = false;
        bool whitespace = false;
        bool nonAscii = false;
        // Iterate code points, not UTF-16 code units, so supplementary-plane
        // characters use their real Unicode category.
        const QList<uint> codepoints = row.plaintext.toUcs4();
        for (const uint codepoint : codepoints) {
            lowercase = lowercase || QChar::isLower(codepoint);
            uppercase = uppercase || QChar::isUpper(codepoint);
            digits = digits || QChar::isDigit(codepoint);
            symbols = symbols || QChar::isSymbol(codepoint) || QChar::isPunct(codepoint);
            whitespace = whitespace || QChar::isSpace(codepoint);
            nonAscii = nonAscii || codepoint > 0x7f;
        }
        const int length = static_cast<int>(codepoints.size());

        totalLength += length;
        minimum = std::min(minimum, length);
        result.maxLength = std::max(result.maxLength, length);
        ++result.lengthHistogram[length];
        unique.insert(row.plaintext);

        result.classes.lowercase += lowercase ? 1 : 0;
        result.classes.uppercase += uppercase ? 1 : 0;
        result.classes.digits += digits ? 1 : 0;
        result.classes.symbols += symbols ? 1 : 0;
        result.classes.whitespace += whitespace ? 1 : 0;
        result.classes.nonAscii += nonAscii ? 1 : 0;
    }

    result.unique = static_cast<int>(unique.size());
    result.minLength = result.analyzed > 0 ? minimum : 0;
    result.averageLength =
        result.analyzed > 0 ? static_cast<double>(totalLength) / result.analyzed : 0.0;
    return result;
}