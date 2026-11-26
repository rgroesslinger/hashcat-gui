/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef POTFILERESULTSDIALOG_H
#define POTFILERESULTSDIALOG_H

#include "potfilecheck.h"
#include "potfileexporter.h"

#include <QDialog>
#include <QString>

namespace Ui
{
class PotfileResultsDialog;
}

// Read-only view of the cracked entries from a potfile query. Plaintexts stay
// masked until explicitly revealed.
class PotfileResultsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PotfileResultsDialog(QWidget *parent = nullptr);
    ~PotfileResultsDialog() override;

    PotfileResultsDialog(const PotfileResultsDialog &) = delete;
    PotfileResultsDialog &operator=(const PotfileResultsDialog &) = delete;
    PotfileResultsDialog(PotfileResultsDialog &&) = delete;
    PotfileResultsDialog &operator=(PotfileResultsDialog &&) = delete;

    // Shows a result from the preflight; no query is started.
    void setResult(const PotfileCheckResult &result, const QString &modeName,
                   const QString &potfilePath = QString());

private slots:
    void exportClicked();
    void statisticsClicked();

private:
    void showResult(const PotfileCheckResult &result);
    void clearView();
    void populateRows();
    void updateHeader();
    void updateStatus();
    void updateButtons();
    [[nodiscard]] QList<PotfileExporter::Row> resultRows() const;

    Ui::PotfileResultsDialog *ui;
    PotfileCheckResult m_result;
    QString m_modeName;
    QString m_potfilePath;
    QString m_exportToolTip;
    bool m_showUsername = false;
};

#endif // POTFILERESULTSDIALOG_H