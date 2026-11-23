/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfileresultsdialog.h"

#include "potfileexportui.h"
#include "potfilestatisticsdialog.h"
#include "ui_potfileresultsdialog.h"

#include <QDir>
#include <QFont>
#include <QPushButton>

#include <algorithm>
#include <utility>

PotfileResultsDialog::PotfileResultsDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::PotfileResultsDialog)
{
    ui->setupUi(this);

    QFont headerFont = ui->label_header->font();
    headerFont.setBold(true);
    ui->label_header->setFont(headerFont);
    ui->label_header->setTextFormat(Qt::PlainText);
    ui->label_note->setTextFormat(Qt::PlainText);
    ui->label_status->setTextFormat(Qt::PlainText);

    ui->table_view->setExtraColumns({tr("Username")});
    ui->table_view->setSearchPlaceholder(tr("Filter hash or username (plaintext after Reveal)"));
    ui->table_view->setEmptyStateText(tr("No cracked hashes for this input."));

    connect(ui->pushButton_export, &QPushButton::clicked, this,
            &PotfileResultsDialog::exportClicked);
    connect(ui->pushButton_statistics, &QPushButton::clicked, this,
            &PotfileResultsDialog::statisticsClicked);
    connect(ui->pushButton_close, &QPushButton::clicked, this, &QDialog::accept);

    clearView();
}

PotfileResultsDialog::~PotfileResultsDialog()
{
    delete ui;
}

void PotfileResultsDialog::setResult(const PotfileCheckResult &result, const QString &modeName,
                                     const QString &potfilePath)
{
    clearView();
    m_modeName = modeName;
    m_potfilePath = potfilePath;
    showResult(result);
}

QList<PotfileExporter::Row> PotfileResultsDialog::resultRows() const
{
    QList<PotfileExporter::Row> rows;
    rows.reserve(m_result.entries.size());
    for (const PotfileShowParser::Entry &entry : m_result.entries) {
        rows.append({entry.hash, entry.plain, entry.username, 0, false, entry.encrypted});
    }
    return rows;
}

void PotfileResultsDialog::exportClicked()
{
    const QList<PotfileExporter::Row> rows = resultRows();
    PotfileExportUi::exportRows(this, QDir::homePath() + QStringLiteral("/cracked-hashes"), rows,
                                m_showUsername, false, m_result.totalCount, m_potfilePath);
}

void PotfileResultsDialog::statisticsClicked()
{
    const QList<PotfileExporter::Row> rows = resultRows();
    const QString context = m_modeName.isEmpty() ? tr("Cracked hashes")
                                                 : tr("Cracked hashes — mode %1").arg(m_modeName);
    PotfileStatisticsDialog dialog(this, PotfileStatistics::calculate(rows), context,
                                   m_result.totalCount);
    dialog.exec();
}

void PotfileResultsDialog::showResult(const PotfileCheckResult &result)
{
    m_result = result;
    m_showUsername = m_showUsername
                     || std::ranges::any_of(std::as_const(m_result.entries),
                                            [](const PotfileShowParser::Entry &entry) {
                                                return !entry.username.isEmpty();
                                            });
    updateHeader();
    populateRows();
}

void PotfileResultsDialog::clearView()
{
    m_result = {};
    m_showUsername = false;
    ui->table_view->clearRows();
    ui->label_status->clear();
    updateButtons();
}

void PotfileResultsDialog::populateRows()
{
    if (m_result.status != PotfileCheckResult::Status::Ok
        && m_result.status != PotfileCheckResult::Status::LeftUnknown) {
        // Cancelled or failed checks leave no actionable rows, and the shared
        // view owns the copy buttons.
        ui->table_view->clearRows();
        updateStatus();
        updateButtons();
        return;
    }

    QList<PotfileTableView::Row> rows;
    rows.reserve(m_result.entries.size());
    for (const PotfileShowParser::Entry &entry : std::as_const(m_result.entries)) {
        rows.append({entry.hash,
                     entry.plain,
                     entry.username,
                     {entry.username},
                     entry.emptyPlain,
                     entry.encrypted});
    }

    ui->table_view->setColumnHidden(2, !m_showUsername);
    ui->table_view->setRows(rows);
    updateStatus();
    updateButtons();
}

void PotfileResultsDialog::updateHeader()
{
    ui->label_header->setText(m_modeName.isEmpty()
                                  ? tr("Cracked hashes")
                                  : tr("Cracked hashes — mode %1").arg(m_modeName));
}

void PotfileResultsDialog::updateStatus()
{
    QString text;
    switch (m_result.status) {
    case PotfileCheckResult::Status::Ok:
        text = tr("%1 of %2 cracked").arg(m_result.cracked).arg(m_result.cracked + m_result.left);
        break;
    case PotfileCheckResult::Status::LeftUnknown:
        text = tr("%1 cracked (remaining unknown)").arg(m_result.cracked);
        break;
    case PotfileCheckResult::Status::Cancelled:
        text = tr("Check cancelled.");
        break;
    case PotfileCheckResult::Status::ShowFailed:
        text = m_result.diagnostic.isEmpty() ? tr("hashcat could not query the potfile.")
                                             : m_result.diagnostic;
        break;
    }

    if (m_result.status == PotfileCheckResult::Status::Ok
        || m_result.status == PotfileCheckResult::Status::LeftUnknown) {
        if (m_result.entries.size() < m_result.totalCount) {
            text += tr(" · showing first %1 of %2")
                        .arg(m_result.entries.size())
                        .arg(m_result.totalCount);
        }
        const int encrypted = static_cast<int>(std::ranges::count_if(
            std::as_const(m_result.entries),
            [](const PotfileShowParser::Entry &entry) { return entry.encrypted; }));
        if (encrypted > 0) {
            text += tr(" · %1 encrypted").arg(encrypted);
        }
        const bool hashFile = std::ranges::any_of(
            std::as_const(m_result.entries),
            [](const PotfileShowParser::Entry &entry) { return entry.hashIsFile; });
        if (hashFile) {
            text += tr(" · includes a binary hash file");
        }
    }

    ui->label_status->setText(text);
}

void PotfileResultsDialog::updateButtons()
{
    const bool hasRows = (m_result.status == PotfileCheckResult::Status::Ok
                          || m_result.status == PotfileCheckResult::Status::LeftUnknown)
                         && ui->table_view->hasRows();
    ui->pushButton_export->setEnabled(hasRows);
    ui->pushButton_statistics->setEnabled(hasRows);
}