/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef STATEFILE_H
#define STATEFILE_H

#include <QCoreApplication>
#include <QJsonObject>
#include <QLatin1StringView>
#include <QString>
#include <QStringList>
#include <cstdint>

// A file the application writes and reads again: the session between two runs
// and the exported settings share one shape.
//
//   {
//     "sections": {
//       "session": { "version": 1, "values": { ... } }
//     }
//   }
//
// The envelope is frozen: an older version cannot read a changed one, and a
// section's version lives inside the section, so it cannot protect the
// envelope. The class owns the envelope only; the caller passes in and
// compares section versions.
class StateFile
{
    Q_DECLARE_TR_FUNCTIONS(StateFile)

public:
    // The file the application keeps its session in between two runs.
    //
    // The location comes from QStandardPaths::AppDataLocation, which the
    // application name alone decides - the organization name is deliberately
    // left unset, see main.cpp.
    static QString sessionFilePath();

    // What one read of a section means for the caller.
    enum class Status : std::uint8_t {
        Read,       // the values may be applied
        Unreadable, // no JSON, no section, no values - error says why
        Newer,      // a format version this build does not know
        Older,      // an earlier format version, applied with a note
    };

    // One section read with its version policy applied: the caller acts on
    // the status instead of repeating the comparison. A file that names no
    // version reads as -1 and is applied like an older file's values, but
    // without the older-version note - it is hand-edited, not historical.
    struct Document {
        Status status = Status::Unreadable;
        QJsonObject values;
        int version = -1;
        QStringList otherSections;
        QString error;
    };

    // Reads the section and compares its version with maxVersion, the
    // version this build writes. The session and the settings share this
    // one policy.
    static Document readDocument(const QString &filename, QLatin1StringView section,
                                 int maxVersion);

    // Writes the values as the named section, replacing the file: a section a
    // previous write put there is gone, and a combined file would be an
    // explicit write, never a merge. The write is atomic, so a crash leaves
    // the old file intact.
    static bool write(const QString &filename, QLatin1StringView section, int version,
                      const QJsonObject &values, QString *errorMessage);

    // Reads one section's values, its version (-1 when absent) and the other
    // section names, also on a failed read. See readDocument() for the version
    // policy; errorMessage names the section and says why the file was refused.
    static bool read(const QString &filename, QLatin1StringView section, QJsonObject *values,
                     int *version, QStringList *otherSections, QString *errorMessage,
                     bool *versionPresent = nullptr);

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
