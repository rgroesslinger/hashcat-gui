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
#include "programdirectory.h"
#include "settingsmanager.h"
#include "textutils.h"
#include "ui_potfileinspector.h"

#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QLineEdit>
#include <QPushButton>
#include <QStandardPaths>
#include <QStringList>
#include <QtConcurrent/QtConcurrentRun>

#include <atomic>
#include <utility>

PotfileInspectorDialog::PotfileInspectorDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::PotfileInspectorDialog)
{
    ui->setupUi(this);

    QFont headerFont = ui->label_header->font();
    headerFont.setBold(true);
    ui->label_header->setFont(headerFont);
    ui->label_header->setTextFormat(Qt::PlainText);
    ui->label_note->setTextFormat(Qt::PlainText);
    ui->label_status->setTextFormat(Qt::PlainText);

    ui->table_view->setExtraColumns({tr("Dup"), tr("Line")});
    ui->table_view->setEmptyStateText(tr("This potfile has no entries."));

    connect(ui->pushButton_browse, &QPushButton::clicked, this,
            &PotfileInspectorDialog::browseClicked);
    connect(ui->pushButton_reload, &QPushButton::clicked, this,
            &PotfileInspectorDialog::reloadClicked);
    connect(ui->lineEdit_potfile, &QLineEdit::editingFinished, this,
            &PotfileInspectorDialog::pathEditingFinished);
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
    updateNote();
    ui->label_status->setText(tr("Choose a potfile."));
}

PotfileInspectorDialog::~PotfileInspectorDialog()
{
    if (m_cancel) {
        m_cancel->store(true);
    }
    delete ui;
}

void PotfileInspectorDialog::setPotfilePath(const QString &path)
{
    const QString trimmed = path.trimmed();
    if (!trimmed.isEmpty()) {
        m_heuristic = false;
        ui->lineEdit_potfile->setText(absolutePath(trimmed));
        loadPath();
        return;
    }

    const QString suggested = suggestedPotfilePath();
    if (!suggested.isEmpty()) {
        m_heuristic = true;
        ui->lineEdit_potfile->setText(suggested);
        loadPath();
        return;
    }

    m_heuristic = false;
    ui->lineEdit_potfile->clear();
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
    loadPath();
}

void PotfileInspectorDialog::reloadClicked()
{
    loadPath();
}

void PotfileInspectorDialog::pathEditingFinished()
{
    m_heuristic = false;
    loadPath();
}

QList<PotfileExporter::Row> PotfileInspectorDialog::resultRows() const
{
    QList<PotfileExporter::Row> rows;
    rows.reserve(m_result.entries.size());
    for (const PotfileParser::Entry &entry : m_result.entries) {
        rows.append(
            {entry.hash, entry.plain, {}, entry.lineNumber, entry.duplicate, entry.encrypted});
    }
    return rows;
}

void PotfileInspectorDialog::exportClicked()
{
    const QList<PotfileExporter::Row> rows = resultRows();
    const QFileInfo source(ui->lineEdit_potfile->text());
    QString suggested =
        source.dir().filePath(source.completeBaseName().isEmpty() ? QStringLiteral("potfile")
                                                                  : source.completeBaseName());
    if (source.filePath().isEmpty()) {
        suggested = QDir::homePath() + QStringLiteral("/potfile");
    }
    PotfileExportUi::exportRows(this, suggested, rows, false, true, m_result.totalCount,
                                ui->lineEdit_potfile->text());
}

void PotfileInspectorDialog::statisticsClicked()
{
    const QList<PotfileExporter::Row> rows = resultRows();
    const QString path = ui->lineEdit_potfile->text();
    const QString context = path.isEmpty() ? tr("Potfile inspector") : tr("Potfile: %1").arg(path);
    PotfileStatisticsDialog dialog(this, PotfileStatistics::calculate(rows), context,
                                   m_result.totalCount);
    dialog.exec();
}

void PotfileInspectorDialog::loadPath()
{
    if (m_cancel) {
        m_cancel->store(true);
    }
    m_watcher.reset();
    clearView();
    updateNote();

    const QString path = ui->lineEdit_potfile->text().trimmed();
    if (path.isEmpty()) {
        m_loading = false;
        ui->label_status->setText(tr("Choose a potfile."));
        updateButtons();
        return;
    }

    const QString absolute = absolutePath(path);
    ui->lineEdit_potfile->setText(absolute);
    if (!QFileInfo::exists(absolute)) {
        m_loading = false;
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

    if (result.cancelled) {
        ui->label_status->setText(tr("Reading cancelled."));
        updateButtons();
        return;
    }
    if (!result.error.isEmpty()) {
        ui->label_status->setText(result.error);
        updateButtons();
        return;
    }

    populateRows();
}

void PotfileInspectorDialog::clearView()
{
    m_result = {};
    ui->table_view->clearRows();
    ui->label_status->clear();
    updateButtons();
}

void PotfileInspectorDialog::populateRows()
{
    QList<PotfileTableView::Row> rows;
    rows.reserve(m_result.entries.size());
    for (const PotfileParser::Entry &entry : std::as_const(m_result.entries)) {
        const QString duplicate = entry.duplicate ? tr("dup") : QString();
        rows.append({entry.hash,
                     entry.plain,
                     {},
                     {duplicate, QString::number(entry.lineNumber)},
                     entry.emptyPlain,
                     entry.encrypted});
    }

    ui->table_view->setRows(rows);
    updateStatus();
    updateButtons();
}

void PotfileInspectorDialog::updateNote()
{
    QString text = tr("No hash type is recorded; a direct parse does not verify that a "
                      "plaintext matches its hash. Duplicates are exact repeated lines only. "
                      "Default ':' separator only.");
    if (m_heuristic) {
        text += tr(" The prefilled path is a heuristic; hashcat may use another path.");
    }
    ui->label_note->setText(text);
}

void PotfileInspectorDialog::updateStatus()
{
    QString text = countText(m_result.totalCount, tr("1 entry"), tr("%1 entries"));
    if (m_result.duplicateCount > 0) {
        text +=
            QStringLiteral(" · ")
            + countText(m_result.duplicateCount, tr("1 duplicate line"), tr("%1 duplicate lines"));
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
        text +=
            tr(" · showing first %1 of %2").arg(m_result.entries.size()).arg(m_result.totalCount);
    }
    ui->label_status->setText(text);
}

void PotfileInspectorDialog::updateButtons()
{
    const bool hasRows = ui->table_view->hasRows();
    ui->pushButton_export->setEnabled(hasRows);
    ui->pushButton_statistics->setEnabled(hasRows);
    ui->pushButton_browse->setEnabled(!m_loading);
    ui->pushButton_reload->setEnabled(!m_loading);
    ui->lineEdit_potfile->setEnabled(!m_loading);
}

QString PotfileInspectorDialog::suggestedPotfilePath() const
{
    QStringList candidates;
    const QString resolved = resolvedHashcatPath();
    if (!resolved.isEmpty()) {
        const QString directory = ProgramDirectory::forProgram(resolved);
        candidates << QDir(directory).filePath(QStringLiteral("hashcat.potfile"));
    }

    const QString dataLocation =
        QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    if (!dataLocation.isEmpty()) {
        candidates << dataLocation + QStringLiteral("/hashcat/hashcat.potfile");
    }

    for (const QString &candidate : std::as_const(candidates)) {
        if (QFileInfo::exists(candidate)) {
            return QDir::cleanPath(candidate);
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