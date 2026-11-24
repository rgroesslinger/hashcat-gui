/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef POTFILEEXPORTER_H
#define POTFILEEXPORTER_H

#include <QByteArray>
#include <QCoreApplication>
#include <QList>
#include <QString>

#include <cstdint>

// Serializes potfile result rows for the explicit export action.
class PotfileExporter
{
    Q_DECLARE_TR_FUNCTIONS(PotfileExporter)

public:
    enum class Format : std::uint8_t { Csv, Tsv, Json };

    struct Row {
        QString hash;
        QString plaintext;
        QString username;
        int lineNumber = 0;
        bool duplicate = false;
        bool encrypted = false;
        // False leaves duplicate/line empty (null in JSON): --show results
        // carry no potfile line information.
        bool hasLineInfo = false;
    };

    static QByteArray serialize(const QList<Row> &rows, Format format, bool includeUsername,
                                bool includeLine);
    static bool write(const QString &path, const QList<Row> &rows, Format format,
                      bool includeUsername, bool includeLine, QString *error = nullptr);
};

#endif // POTFILEEXPORTER_H