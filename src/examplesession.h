/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef EXAMPLESESSION_H
#define EXAMPLESESSION_H

#include "examples.h"

#include <QCoreApplication>
#include <QJsonValue>
#include <QString>

// The bundled example sessions: finding one, and turning the resource paths
// it names into real files hashcat can read. The dialogs and the confirmation
// stay with the caller.
class ExampleSession
{
    // The unpacking failures end up in dialogs opened by the caller
    Q_DECLARE_TR_FUNCTIONS(ExampleSession)

public:
    // The bundled example for one attack mode, nullptr when there is none.
    static const Examples::Entry *find(AttackMode mode);

    // Whether a session asks for one of the bundled example data files. Only
    // such a session pays the unpacking cost; the brute-force example has no
    // data.
    static bool containsExamplePrefix(const QJsonValue &value);

    // Replaces the <examples>/ token in every string of the session,
    // including the text of list rows, with the directory the resources were
    // unpacked to.
    static QJsonValue withExamplePaths(const QJsonValue &value, const QString &directory);

    // Writes every bundled data file to the example directory, replacing the
    // copy a previous load left there, and writes the native directory, with
    // a trailing separator, to *directory. The whole set is written, not only
    // the files the selected session names: the files are tiny, and an example
    // must never depend on what another one unpacked before it.
    static bool materialize(QString *directory, QString *error);
};

#endif // EXAMPLESESSION_H