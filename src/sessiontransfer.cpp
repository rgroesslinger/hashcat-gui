/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "sessiontransfer.h"

#include "appconstants.h"
#include "widgetstateserializer.h"

#include <QWidget>

QStringList SessionTransfer::applyToWidgets(const StateFile::Document &document, QWidget *root,
                                            const QStringList &ignoredNames)
{
    const WidgetStateSerializer::ApplyResult notApplied =
        WidgetStateSerializer::applyToWidgets(document.values, root, ignoredNames);

    QStringList notes =
        fileNotes(document.version, AppConstants::Sessions::Version, document.otherSections);

    // The notes are only there when there is something to say.
    const QString unknownNote = StateFile::unknownValuesNote(notApplied.unknownKeys);
    if (!unknownNote.isEmpty()) {
        notes << unknownNote;
    }

    const QString wrongTypeNote = StateFile::wrongTypeValuesNote(notApplied.wrongTypes);
    if (!wrongTypeNote.isEmpty()) {
        notes << wrongTypeNote;
    }

    return notes;
}

QJsonObject SessionTransfer::toDocument(QWidget *root)
{
    return WidgetStateSerializer::toJson(root, {AppConstants::Files::PreviewWidget});
}

QStringList SessionTransfer::fileNotes(int fileVersion, int currentVersion,
                                       const QStringList &otherSections)
{
    QStringList notes;

    // A newer format version is refused before it gets here: a name that
    // survived may mean something else there. A file that names no version
    // is hand-edited; its values are applied like an older file's.
    if (fileVersion >= 0 && fileVersion < currentVersion) {
        notes << tr("The file was made by an older version of hashcat-gui (format version %1; "
                    "this version reads format version %2).")
                     .arg(fileVersion)
                     .arg(currentVersion);
    }

    if (!otherSections.isEmpty()) {
        notes << tr("The file holds sections this import does not use: %1.")
                     .arg(StateFile::nameList(otherSections));
    }

    return notes;
}

QString SessionTransfer::newerFileNote(int fileVersion, int currentVersion)
{
    return tr("The file is in format version %1, but this version reads format version %2, so it "
              "was not applied. Update hashcat-gui to read it.")
        .arg(fileVersion)
        .arg(currentVersion);
}

QString SessionTransfer::newerSessionNote(int fileVersion)
{
    return tr("The session file is in format version %1, but this version reads format version %2, "
              "so the session was not restored. The file was left as it is, so the session of "
              "this run is not stored either. Update hashcat-gui to read it.")
        .arg(fileVersion)
        .arg(AppConstants::Sessions::Version);
}