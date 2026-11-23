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

// Aggregate password statistics for the retained rows of one result view.
// Plaintext values are never displayed.
class PotfileStatisticsDialog : public QDialog
{
    Q_OBJECT

public:
    PotfileStatisticsDialog(QWidget *parent, const PotfileStatistics::Result &statistics,
                            const QString &context, int totalCount);
    ~PotfileStatisticsDialog() override;

    PotfileStatisticsDialog(const PotfileStatisticsDialog &) = delete;
    PotfileStatisticsDialog &operator=(const PotfileStatisticsDialog &) = delete;
    PotfileStatisticsDialog(PotfileStatisticsDialog &&) = delete;
    PotfileStatisticsDialog &operator=(PotfileStatisticsDialog &&) = delete;

private:
    Ui::PotfileStatisticsDialog *ui;
};

#endif // POTFILESTATISTICSDIALOG_H