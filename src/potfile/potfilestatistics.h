/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef POTFILESTATISTICS_H
#define POTFILESTATISTICS_H

#include "potfileexporter.h"

#include <QList>
#include <QMap>

// Aggregate statistics for recovered plaintexts. No plaintext value is stored
// in the result; encrypted and empty entries are counted but not analyzed.
class PotfileStatistics
{
public:
    struct CharacterClasses {
        int lowercase = 0;
        int uppercase = 0;
        int digits = 0;
        int symbols = 0;
        int whitespace = 0;
        int nonAscii = 0;
    };

    struct Result {
        int entries = 0;
        int analyzed = 0;
        int empty = 0;
        int encrypted = 0;
        int duplicate = 0;
        int unique = 0;
        int minLength = 0;
        int maxLength = 0;
        double averageLength = 0.0;
        QMap<int, int> lengthHistogram;
        CharacterClasses classes;
    };

    static Result calculate(const QList<PotfileExporter::Row> &rows);
};

#endif // POTFILESTATISTICS_H