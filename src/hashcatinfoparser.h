/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef HASHCATINFOPARSER_H
#define HASHCATINFOPARSER_H

#include <QCoreApplication>
#include <QList>
#include <QMap>
#include <QString>

// Parser for the reply to `hashcat --example-hashes --machine-readable`.
//
// Kept out of MainWindow so the reply can be fed to it directly - including
// the malformed replies hashcat produces when a plugin fails - without
// bringing up a window.
class HashcatInfoParser
{
    Q_DECLARE_TR_FUNCTIONS(HashcatInfoParser)

public:
    // Fills hashModes with "<id> | <name>" entries. Returns false and writes a
    // translated reason when the reply cannot be used; hashModes is left
    // untouched on failure, so a refresh does not clear the list on screen.
    static bool parseExampleHashes(const QString &rawOutput, QMap<quint32, QString> &hashModes,
                                   QString *errorMessage = nullptr);

    struct IdentifiedHashMode {
        quint32 id = 0;
        QString name;
        QString category;
    };

    // Parses --identify's `# | Name | Category` table; "no match" (exit 255)
    // is an empty answer, not a failure.
    static bool parseIdentify(const QString &rawOutput, QList<IdentifiedHashMode> &matches,
                              QString *errorMessage = nullptr);
};

#endif // HASHCATINFOPARSER_H
