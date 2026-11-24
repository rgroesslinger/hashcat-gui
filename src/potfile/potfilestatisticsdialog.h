/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef POTFILESTATISTICSDIALOG_H
#define POTFILESTATISTICSDIALOG_H

#include "potfilestatistics.h"

#include <QDialog>
#include <QString>

namespace Ui
{
class PotfileStatisticsDialog;
}

// Aggregate password statistics for the visible rows of one result view.
// Plaintext values are never displayed.
class PotfileStatisticsDialog : public QDialog
{
    Q_OBJECT

public:
    // duplicateKnown is false for --show results: the potfile may hold
    // duplicate lines that --show deduplicates, so a count would be a guess.
    PotfileStatisticsDialog(QWidget *parent, const PotfileStatistics::Result &statistics,
                            const QString &context, int totalCount, bool duplicateKnown);
    ~PotfileStatisticsDialog() override;

    // Calculates off the GUI thread behind a progress dialog, then opens the
    // statistics dialog. Both stay children of parent; closing it discards
    // the result safely.
    static void openAsync(QWidget *parent, const QList<PotfileExporter::Row> &rows,
                          const QString &context, int totalCount, bool duplicateKnown);

    PotfileStatisticsDialog(const PotfileStatisticsDialog &) = delete;
    PotfileStatisticsDialog &operator=(const PotfileStatisticsDialog &) = delete;
    PotfileStatisticsDialog(PotfileStatisticsDialog &&) = delete;
    PotfileStatisticsDialog &operator=(PotfileStatisticsDialog &&) = delete;

private:
    Ui::PotfileStatisticsDialog *ui;
};

#endif // POTFILESTATISTICSDIALOG_H