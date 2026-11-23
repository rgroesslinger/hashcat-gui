/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef POTFILESHOWPARSER_H
#define POTFILESHOWPARSER_H

#include "appconstants.h"

#include <QCoreApplication>
#include <QList>
#include <QString>

// Parser for the newline-delimited JSON of `hashcat --show --json`.
class PotfileShowParser
{
    Q_DECLARE_TR_FUNCTIONS(PotfileShowParser)

public:
    struct Entry {
        QString hash;
        QString plain;
        QString username;
        bool emptyPlain = false;
        bool encrypted = false;
        // Binary-hashfile modes make hashcat report the file name instead of
        // a per-hash identity.
        bool hashIsFile = false;
    };

    using Entries = QList<Entry>;

    struct Result {
        Entries entries;
        // Every valid line, including the ones beyond the retention cap.
        int totalCount = 0;
        // Lines that are not a JSON object carrying a usable hash_hex.
        int malformedCount = 0;
    };

    static Result parse(const QString &output,
                        int maxEntries = AppConstants::Hashcat::PotfileMaxEntries);
};

#endif // POTFILESHOWPARSER_H