/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef POTFILEEXPORTUI_H
#define POTFILEEXPORTUI_H

#include "potfileexporter.h"

#include <QCoreApplication>
#include <QList>
#include <QString>

class QWidget;

// The shared file picker, plaintext warning and save flow for both result
// dialogs.
class PotfileExportUi
{
    Q_DECLARE_TR_FUNCTIONS(PotfileExportUi)

public:
    static bool exportRows(QWidget *parent, const QString &suggestedPath,
                           const QList<PotfileExporter::Row> &rows, bool includeUsername,
                           bool includeLine, int totalCount,
                           const QString &protectedPath = QString());

    // True when destination names the same file as source, by path or by
    // canonical path (symlinks). An empty source is never a match.
    [[nodiscard]] static bool isSamePath(const QString &destination, const QString &source);
};

#endif // POTFILEEXPORTUI_H