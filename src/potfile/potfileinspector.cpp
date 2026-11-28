/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfileinspector.h"

#include "appconstants.h"
#include "executable.h"
#include "potfileexportui.h"
#include "potfilequery.h"
#include "potfilestatisticsdialog.h"
#include "settingsmanager.h"
#include "textutils.h"
#include "ui_potfileinspector.h"

#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QKeySequence>
#include <QLineEdit>
#include <QPushButton>
#include <QStringList>
#include <QtConcurrent/QtConcurrentRun>

#include <atomic>
#include <utility>

PotfileInspectorDialog::PotfileInspectorDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::PotfileInspectorDialog)
{
    ui->setupUi(this);

    m_exportToolTip = ui->pushButton_export->toolTip();
    ui->label_note->setTextFormat(Qt::PlainText);
    ui->label_status->setTextFormat(Qt::PlainText);
    ui->label_status->setTextInteractionFlags(Qt::TextSelectableByMouse);

    ui->table_view->setExtraColumns({tr("Dup"), tr("Line")});
    ui->table_view->setNumericSortColumn(3);
    ui->table_view->setEmptyStateText(tr("This potfile has no entries."));
    connect(ui->table_view, &PotfileTableView::visibleRowsChanged, this, [this] {
        updateStatus();
        updateButtons();
    });

    connect(ui->pushButton_browse, &QPushButton::clicked, this,
            &PotfileInspectorDialog::browseClicked);
    ui->pushButton_reload->setShortcut(QKeySequence(QStringLiteral("F5")));
    connect(ui->pushButton_reload, &QPushButton::clicked, this,
            &PotfileInspectorDialog::reloadClicked);
    connect(ui->pushButton_stop, &QPushButton::clicked, this, &PotfileInspectorDialog::stopClicked);
    // Focus loss must not load; Enter clicks Reload, the dialog default.
    connect(ui->lineEdit_potfile, &QLineEdit::textEdited, this, [this] {
        m_heuristic = false;
        updateHeuristicLabel();
    });
    connect(ui->pushButton_export, &QPushButton::clicked, this,
            &PotfileInspectorDialog::exportClicked);
    connect(ui->pushButton_statistics, &QPushButton::clicked, this,
            &PotfileInspectorDialog::statisticsClicked);
    connect(ui->pushButton_close, &QPushButton::clicked, this, [this] {
        if (m_cancel) {
            m_cancel->store(true);
        }
        accept();
    });

    clearView();
    ui->label_status->setText(tr("Choose a potfile."));
}

PotfileInspectorDialog::~PotfileInspectorDialog()
{
    resetHashcatQuery(m_cancel, m_watcher);
    delete ui;
}

void PotfileInspectorDialog::setPotfilePath(const QString &path)
{
    const QString trimmed = path.trimmed();
    if (!trimmed.isEmpty()) {
        m_heuristic = false;
        ui->lineEdit_potfile->setText(absolutePath(trimmed));
        updateHeuristicLabel();
        loadPath();
        return;
    }

    const QString suggested = suggestedPotfilePath();
    if (!suggested.isEmpty()) {
        m_heuristic = true;
        ui->lineEdit_potfile->setText(suggested);
        updateHeuristicLabel();
        loadPath();
        return;
    }

    m_heuristic = false;
    ui->lineEdit_potfile->clear();
    updateHeuristicLabel();
    loadPath();
}

void PotfileInspectorDialog::closeEvent(QCloseEvent *event)
{
    if (m_cancel) {
        m_cancel->store(true);
    }
    QDialog::closeEvent(event);
}

void PotfileInspectorDialog::browseClicked()
{
    const QString path =
        QFileDialog::getOpenFileName(this, tr("Select a potfile"), ui->lineEdit_potfile->text());
    if (path.isEmpty()) {
        return;
    }
    m_heuristic = false;
    ui->lineEdit_potfile->setText(QFileInfo(path).absoluteFilePath());
    updateHeuristicLabel();
    loadPath();
}

void PotfileInspectorDialog::reloadClicked()
{
    loadPath(/*preserveViewState=*/true);
}

void PotfileInspectorDialog::stopClicked()
{
    if (m_cancel) {
        m_cancel->store(true);
    }
    ui->pushButton_stop->setEnabled(false);
    ui->label_status->setText(tr("Cancelling…"));
}

QList<PotfileExporter::Row> PotfileInspectorDialog::resultRows() const
{
    QList<PotfileExporter::Row> rows;
    const QList<PotfileTableView::Row> visible = ui->table_view->visibleRows();
    rows.reserve(visible.size());
    for (const PotfileTableView::Row &row : visible) {
        if (row.sourceIndex < 0 || row.sourceIndex >= m_result.entries.size()) {
            continue;
        }
        const PotfileParser::Entry &entry = m_result.entries.at(row.sourceIndex);
        rows.append({entry.hash,
                     entry.plain,
                     {},
                     entry.lineNumber,
                     entry.duplicate,
                     entry.encrypted,
                     true});
    }
    return rows;
}

void PotfileInspectorDialog::exportClicked()
{
    // The button is disabled while masked; do not bypass it programmatically.
    if (!ui->table_view->isRevealed() && ui->table_view->canReveal()) {
        return;
    }
    const QList<PotfileExporter::Row> rows = resultRows();
    // Never suggest the potfile's own directory: an export next to the live
    // potfile is one careless click away from overwriting it.
    const QFileInfo source(ui->lineEdit_potfile->text());
    const QString base = source.completeBaseName();
    const QString suggested =
        QDir::homePath() + QStringLiteral("/")
        + (base.isEmpty() ? QStringLiteral("potfile-entries") : base + QStringLiteral("-entries"));
    QStringList protectedPaths = PotfileQuery::defaultPotfileCandidates(resolvedHashcatPath());
    if (!ui->lineEdit_potfile->text().isEmpty()) {
        protectedPaths << ui->lineEdit_potfile->text();
    }
    PotfileExportUi::exportRows(this, suggested, rows, true, true, m_result.totalCount,
                                protectedPaths);
}

void PotfileInspectorDialog::statisticsClicked()
{
    const QList<PotfileExporter::Row> rows = resultRows();
    const QString path = ui->lineEdit_potfile->text();
    const QString context = path.isEmpty() ? tr("Potfile inspector") : tr("Potfile: %1").arg(path);
    // Statistics cover the visible retained rows; claim exact duplicates
    // only for the whole file.
    const bool duplicateKnown = m_result.duplicateCountComplete
                                && m_result.entries.size() == m_result.totalCount
                                && !ui->table_view->isFiltered();
    PotfileStatisticsDialog::openAsync(this, rows, context, m_result.totalCount, duplicateKnown);
}

void PotfileInspectorDialog::loadPath(bool preserveViewState)
{
    resetHashcatQuery(m_cancel, m_watcher);
    m_preserveViewState = preserveViewState;
    clearView();

    const QString path = ui->lineEdit_potfile->text().trimmed();
    if (path.isEmpty()) {
        m_loading = false;
        ui->table_view->clearRows(m_preserveViewState);
        ui->label_status->setText(tr("Choose a potfile."));
        updateButtons();
        return;
    }

    const QString absolute = absolutePath(path);
    ui->lineEdit_potfile->setText(absolute);
    if (!QFileInfo::exists(absolute)) {
        m_loading = false;
        ui->table_view->clearRows(m_preserveViewState);
        ui->label_status->setText(tr("Potfile not found: %1").arg(absolute));
        updateButtons();
        return;
    }

    m_loading = true;
    m_cancel = std::make_shared<std::atomic_bool>(false);
    m_watcher = std::make_unique<QFutureWatcher<PotfileParser::Result>>();
    connect(m_watcher.get(), &QFutureWatcher<PotfileParser::Result>::finished, this,
            [this] { showResult(m_watcher->result()); });
    ui->label_status->setText(tr("Reading potfile…"));
    updateButtons();

    const HashcatRunner::CancelToken cancel = m_cancel;
    m_watcher->setFuture(QtConcurrent::run([absolute, cancel] {
        return PotfileParser::parseFile(absolute, AppConstants::Hashcat::PotfileMaxEntries,
                                        QLatin1Char(':'), cancel);
    }));
}

void PotfileInspectorDialog::showResult(const PotfileParser::Result &result)
{
    m_loading = false;
    m_result = result;

    // A failed read must not leave rows whose sourceIndex names no entry.
    if (result.cancelled) {
        ui->table_view->clearRows(m_preserveViewState);
        ui->label_status->setText(tr("Reading cancelled."));
        updateButtons();
        return;
    }
    if (!result.error.isEmpty()) {
        ui->table_view->clearRows(m_preserveViewState);
        ui->label_status->setText(result.error);
        updateButtons();
        return;
    }

    populateRows();
}

void PotfileInspectorDialog::clearView()
{
    m_result = {};
    if (!m_preserveViewState) {
        ui->table_view->clearRows();
    }
    ui->label_status->clear();
    updateButtons();
}

void PotfileInspectorDialog::populateRows()
{
    QList<PotfileTableView::Row> rows;
    rows.reserve(m_result.entries.size());
    int index = 0;
    for (const PotfileParser::Entry &entry : std::as_const(m_result.entries)) {
        const QString duplicate = entry.duplicate ? tr("dup") : QString();
        rows.append({entry.hash,
                     entry.plain,
                     {},
                     {duplicate, QString::number(entry.lineNumber)},
                     entry.emptyPlain,
                     entry.encrypted,
                     false,
                     index++});
    }

    ui->table_view->setRows(rows, m_preserveViewState);
    updateStatus();
    updateButtons();
}

void PotfileInspectorDialog::updateHeuristicLabel()
{
    ui->label_heuristic->setVisible(m_heuristic);
}

void PotfileInspectorDialog::updateStatus()
{
    QString text = m_result.totalCount == 1
                       ? tr("1 entry")
                       : tr("%1 entries").arg(formatCount(m_result.totalCount));
    if (m_result.duplicateCount > 0) {
        text += QStringLiteral(" · ")
                + countText(m_result.duplicateCount,
                            m_result.duplicateCountComplete ? tr("1 duplicate line")
                                                            : tr("1 duplicate line (at least)"),
                            m_result.duplicateCountComplete ? tr("%1 duplicate lines")
                                                            : tr("%1 duplicate lines (at least)"));
    } else if (!m_result.duplicateCountComplete) {
        text += QStringLiteral(" · ") + tr("duplicate count incomplete");
    }
    if (m_result.malformedCount > 0) {
        text +=
            QStringLiteral(" · ")
            + countText(m_result.malformedCount, tr("1 malformed line"), tr("%1 malformed lines"));
    }
    if (m_result.encryptedCount > 0) {
        text += QStringLiteral(" · ")
                + countText(m_result.encryptedCount, tr("1 encrypted entry"),
                            tr("%1 encrypted entries"));
    }
    if (m_result.noPasswordCount > 0) {
        text += QStringLiteral(" · ")
                + countText(m_result.noPasswordCount, tr("1 entry without a password"),
                            tr("%1 entries without a password"));
    }
    if (m_result.truncatedCount > 0) {
        text += QStringLiteral(" · ")
                + countText(m_result.truncatedCount, tr("1 incomplete line skipped"),
                            tr("%1 incomplete lines skipped"));
    }
    if (m_result.entries.size() < m_result.totalCount) {
        text += tr(" · showing first %1 of %2")
                    .arg(formatCount(static_cast<int>(m_result.entries.size())),
                         formatCount(m_result.totalCount));
    }
    const int hidden = ui->table_view->rowCount() - ui->table_view->visibleRowCount();
    if (hidden > 0) {
        text += tr(" · %1 hidden by filter").arg(hidden);
    }
    ui->label_status->setText(text);
}

void PotfileInspectorDialog::updateButtons()
{
    const bool hasRows = !m_loading && ui->table_view->visibleRowCount() > 0;
    // Export writes the real plaintexts even while the table masks them.
    const bool exportable =
        hasRows && (ui->table_view->isRevealed() || !ui->table_view->canReveal());
    ui->pushButton_export->setEnabled(exportable);
    ui->pushButton_export->setToolTip(
        exportable ? m_exportToolTip : tr("Reveal plaintexts to export the real values."));
    ui->pushButton_statistics->setEnabled(hasRows);
    ui->pushButton_browse->setEnabled(!m_loading);
    ui->pushButton_reload->setEnabled(!m_loading);
    ui->lineEdit_potfile->setEnabled(!m_loading);
    ui->pushButton_stop->setEnabled(m_loading);
    ui->progressBar_busy->setVisible(m_loading);
}

QString PotfileInspectorDialog::suggestedPotfilePath() const
{
    const QStringList candidates = PotfileQuery::defaultPotfileCandidates(resolvedHashcatPath());
    for (const QString &candidate : candidates) {
        if (QFileInfo::exists(candidate)) {
            return candidate;
        }
    }
    return {};
}

QString PotfileInspectorDialog::resolvedHashcatPath() const
{
    const QString configured =
        SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    return Executable::resolve(configured);
}

QString PotfileInspectorDialog::absolutePath(const QString &enteredPath) const
{
    const QString hashcatPath = resolvedHashcatPath();
    if (!hashcatPath.isEmpty()) {
        return PotfileQuery::resolvePath(enteredPath, hashcatPath);
    }
    return QFileInfo(enteredPath).absoluteFilePath();
}