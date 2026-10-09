/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef HASHCATINFOPARSER_H
#define HASHCATINFOPARSER_H

#include <QCoreApplication>
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
    // Fills hashModes with "<id> | <name>" entries keyed by hash type id.
    //
    // Returns false and writes an already translated reason into errorMessage
    // (when that is not nullptr) when the reply cannot be used. hashModes is
    // left untouched on failure so a failed refresh does not throw away a
    // list that is still on screen.
    static bool parseExampleHashes(const QString &rawOutput, QMap<quint32, QString> &hashModes,
                                   QString *errorMessage = nullptr);
};

#endif // HASHCATINFOPARSER_H
