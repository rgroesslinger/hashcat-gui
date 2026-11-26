/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfileresultsdialog.h"

#include "appconstants.h"
#include "executable.h"
#include "potfileexportui.h"
#include "potfilequery.h"
#include "potfilestatisticsdialog.h"
#include "settingsmanager.h"
#include "textutils.h"
#include "ui_potfileresultsdialog.h"

#include <QDir>
#include <QPushButton>

#include <algorithm>
#include <utility>

PotfileResultsDialog::PotfileResultsDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::PotfileResultsDialog)
{
    ui->setupUi(this);

    m_exportToolTip = ui->pushButton_export->toolTip();
    ui->label_note->setTextFormat(Qt::PlainText);
    ui->label_status->setTextFormat(Qt::PlainText);
    ui->label_status->setTextInteractionFlags(Qt::TextSelectableByMouse);

    ui->table_view->setExtraColumns({tr("Username")});
    ui->table_view->setSearchPlaceholder(tr("Filter hash or username (plaintext after Reveal)"));
    ui->table_view->setEmptyStateText(tr("No cracked hashes for this input."));
    connect(ui->table_view, &PotfileTableView::visibleRowsChanged, this, [this] {
        updateStatus();
        updateButtons();
    });

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
    const QList<PotfileTableView::Row> visible = ui->table_view->visibleRows();
    rows.reserve(visible.size());
    for (const PotfileTableView::Row &row : visible) {
        if (row.sourceIndex < 0 || row.sourceIndex >= m_result.entries.size()) {
            continue;
        }
        const PotfileShowParser::Entry &entry = m_result.entries.at(row.sourceIndex);
        rows.append({entry.hash, entry.plain, entry.username, 0, false, entry.encrypted, false});
    }
    return rows;
}

void PotfileResultsDialog::exportClicked()
{
    // The button is disabled while masked; do not bypass it programmatically.
    if (!ui->table_view->isRevealed() && ui->table_view->canReveal()) {
        return;
    }
    const QList<PotfileExporter::Row> rows = resultRows();
    const QString configured =
        SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    QStringList protectedPaths =
        PotfileQuery::defaultPotfileCandidates(Executable::resolve(configured));
    if (!m_potfilePath.isEmpty()) {
        protectedPaths << m_potfilePath;
    }
    PotfileExportUi::exportRows(this, QDir::homePath() + QStringLiteral("/cracked-hashes"), rows,
                                true, true, m_result.totalCount, protectedPaths);
}

void PotfileResultsDialog::statisticsClicked()
{
    const QList<PotfileExporter::Row> rows = resultRows();
    const QString context = m_modeName.isEmpty() ? tr("Cracked hashes")
                                                 : tr("Cracked hashes — mode %1").arg(m_modeName);
    PotfileStatisticsDialog::openAsync(this, rows, context, m_result.totalCount,
                                       /*duplicateKnown=*/false);
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
    int index = 0;
    for (const PotfileShowParser::Entry &entry : std::as_const(m_result.entries)) {
        rows.append({entry.hash,
                     entry.plain,
                     entry.username,
                     {entry.username},
                     entry.emptyPlain,
                     entry.encrypted,
                     entry.hashIsFile,
                     index++});
    }

    ui->table_view->setColumnHidden(2, !m_showUsername);
    ui->table_view->setRows(rows);
    updateStatus();
    updateButtons();
}

void PotfileResultsDialog::updateHeader()
{
    setWindowTitle(m_modeName.isEmpty() ? tr("Cracked hashes")
                                        : tr("Cracked hashes — mode %1").arg(m_modeName));
    ui->label_note->setText(
        m_potfilePath.isEmpty()
            ? tr("Potfile: hashcat's default potfile. Hashcat matches potfile entries by hash; "
                 "the selected hash type narrows the search.")
            : tr("Potfile: %1. Hashcat matches potfile entries by hash; the selected hash type "
                 "narrows the search.")
                  .arg(m_potfilePath));
}

void PotfileResultsDialog::updateStatus()
{
    QString text;
    switch (m_result.status) {
    case PotfileCheckResult::Status::Ok:
        text =
            tr("%1 of %2 cracked")
                .arg(formatCount(m_result.cracked), formatCount(m_result.cracked + m_result.left));
        break;
    case PotfileCheckResult::Status::LeftUnknown:
        text = tr("%1 cracked (remaining unknown)").arg(formatCount(m_result.cracked));
        if (!m_result.diagnostic.isEmpty()) {
            text += QStringLiteral(" · ") + m_result.diagnostic;
        }
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
                        .arg(formatCount(static_cast<int>(m_result.entries.size())),
                             formatCount(m_result.totalCount));
        }
        if (m_result.malformedCount > 0) {
            text += QStringLiteral(" · ")
                    + countText(m_result.malformedCount, tr("1 unreadable result line skipped."),
                                tr("%1 unreadable result lines skipped."));
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
        const int hidden = ui->table_view->rowCount() - ui->table_view->visibleRowCount();
        if (hidden > 0) {
            text += tr(" · %1 hidden by filter").arg(hidden);
        }
    }

    ui->label_status->setText(text);
}

void PotfileResultsDialog::updateButtons()
{
    const bool hasRows = (m_result.status == PotfileCheckResult::Status::Ok
                          || m_result.status == PotfileCheckResult::Status::LeftUnknown)
                         && ui->table_view->visibleRowCount() > 0;
    // Export writes the real plaintexts even while the table masks them.
    const bool exportable =
        hasRows && (ui->table_view->isRevealed() || !ui->table_view->canReveal());
    ui->pushButton_export->setEnabled(exportable);
    ui->pushButton_export->setToolTip(
        exportable ? m_exportToolTip : tr("Reveal plaintexts to export the real values."));
    ui->pushButton_statistics->setEnabled(hasRows);
}