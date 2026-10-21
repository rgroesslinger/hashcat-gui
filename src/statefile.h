/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef STATEFILE_H
#define STATEFILE_H

#include <QCoreApplication>
#include <QLatin1StringView>
#include <QJsonObject>
#include <QString>
#include <QStringList>

// A file the application writes and reads again: the session it keeps between
// two runs and the settings the user exports share one shape.
//
// The file is a JSON object with the sections it holds:
//
//   {
//     "createdWith": "hashcat-gui 0.8.0",
//     "sections": {
//       "session": { "version": 1, "values": { ... } }
//     }
//   }
//
// The class owns the envelope and nothing else: it does not know which
// widgets or which settings a section holds, and it does not know the version
// of one - the caller passes it in and compares what it reads.
class StateFile
{
    // The messages of the class end up in dialogs opened by the caller
    Q_DECLARE_TR_FUNCTIONS(StateFile)

public:
    // Writes the values into the file as the section of the given name.
    //
    // The write is atomic: a crash in the middle leaves the old file alone
    // instead of a truncated one.
    static bool write(const QString &filename, QLatin1StringView section, int version,
                      const QJsonObject &values, QString *errorMessage);

    // Reads the section of the given name out of the file.
    //
    // Returns false when the file cannot be used: it is no JSON, it holds no
    // section of that name, or it cannot be read. errorMessage then says why,
    // and it names the section:
    //
    //   "The file does not contain a session section."
    //   "The file does not contain a session section; it holds settings."
    //
    // version receives the version of the values the section holds - the
    // caller compares it with the current one. otherSections receives the
    // names of the sections the file holds besides the one asked for, also
    // when the read failed, so that the message can name them.
    static bool read(const QString &filename, QLatin1StringView section, QJsonObject *values,
                     int *version, QStringList *otherSections, QString *errorMessage);

    // The names in one sentence: "a, b and c" - and "and N more" when there
    // are more of them than fit in a line.
    static QString nameList(const QStringList &names, int limit = 10);

    // The note for the values a file names that this version does not know.
    // An empty list gives an empty string.
    static QString unknownValuesNote(const QStringList &names);

    // The note for the values a file holds that are of the wrong type: the
    // setting they belong to takes another one, and applying them would
    // change something the user did not ask for. An empty list gives an empty
    // string.
    static QString wrongTypeValuesNote(const QStringList &names);
};

#endif // STATEFILE_H
