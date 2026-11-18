/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef SESSIONTRANSFER_H
#define SESSIONTRANSFER_H

#include "statefile.h"

#include <QCoreApplication>
#include <QJsonObject>
#include <QString>
#include <QStringList>

class QWidget;

// The values of a session file onto the widgets, and back: the transfer
// itself, plus the notes about the file and what could not be applied. The
// file dialogs, the confirmation boxes and the pending hash type stay with
// the caller.
class SessionTransfer
{
    Q_DECLARE_TR_FUNCTIONS(SessionTransfer)

public:
    // Applies the document's values to the widgets below root and returns
    // the notes: the file's version and other sections first, then the
    // unknown values and the wrong types. The caller appends its own
    // hash-type note last, and rebuilds the preview once after the call -
    // the transfer is a bulk write.
    static QStringList applyToWidgets(const StateFile::Document &document, QWidget *root,
                                      const QStringList &ignoredNames);

    // The values a save writes: everything the serializer sees, minus the
    // command preview, which is regenerated on every change. Retired names
    // matter on the read side only.
    static QJsonObject toDocument(QWidget *root);

    // The notes about the file itself: the version it was made by, and the
    // sections it holds besides the one that was imported.
    static QStringList fileNotes(int fileVersion, int currentVersion,
                                 const QStringList &otherSections);

    // Why a file a newer version wrote is not applied at all.
    static QString newerFileNote(int fileVersion, int currentVersion);

    // Why the session of a newer version is not restored - and not stored
    // either.
    static QString newerSessionNote(int fileVersion);
};

#endif // SESSIONTRANSFER_H