/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfiletableview.h"

#include "appconstants.h"
#include "textutils.h"
#include "ui_potfiletableview.h"

#include <QApplication>
#include <QClipboard>
#include <QCursor>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMenu>
#include <QPalette>
#include <QPushButton>
#include <QShortcut>
#include <QSignalBlocker>
#include <QStackedLayout>
#include <QToolTip>
#include <QTreeWidgetItem>

#include <algorithm>

PotfileTableView::PotfileTableView(QWidget *parent) : QWidget(parent), ui(new Ui::PotfileTableView)
{
    ui->setupUi(this);

    ui->treeWidget_results->setTextElideMode(Qt::ElideMiddle);
    ui->treeWidget_results->header()->setSortIndicator(0, Qt::AscendingOrder);
    QPalette emptyPalette = ui->label_empty_state->palette();
    emptyPalette.setColor(QPalette::WindowText, emptyPalette.color(QPalette::PlaceholderText));
    ui->label_empty_state->setPalette(emptyPalette);

    // Enter must neither fire a dialog default nor copy.
    ui->treeWidget_results->installEventFilter(this);
    ui->treeWidget_results->viewport()->installEventFilter(this);
    ui->lineEdit_search->installEventFilter(this);

    auto *copyShortcut = new QShortcut(QKeySequence::Copy, ui->treeWidget_results);
    copyShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(copyShortcut, &QShortcut::activated, this, &PotfileTableView::copySelected);
    ui->treeWidget_results->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget_results, &QTreeWidget::customContextMenuRequested, this,
            &PotfileTableView::showContextMenu);

    connect(ui->lineEdit_search, &QLineEdit::textChanged, this, &PotfileTableView::searchChanged);
    connect(ui->pushButton_reveal, &QPushButton::toggled, this, &PotfileTableView::revealToggled);
    connect(ui->pushButton_copy_selected, &QPushButton::clicked, this,
            &PotfileTableView::copySelected);
    connect(ui->pushButton_copy_all, &QPushButton::clicked, this, &PotfileTableView::copyAll);
    connect(ui->treeWidget_results, &QTreeWidget::itemSelectionChanged, this,
            &PotfileTableView::updateButtons);

    m_filterTimer.setSingleShot(true);
    m_filterTimer.setInterval(AppConstants::Hashcat::PotfileFilterDebounceMs);
    connect(&m_filterTimer, &QTimer::timeout, this, &PotfileTableView::applyFilter);

    setExtraColumns({});
    clearRows();
}

PotfileTableView::~PotfileTableView()
{
    delete ui;
}

void PotfileTableView::setExtraColumns(const QStringList &headers)
{
    QStringList labels{tr("Hash"), tr("Plaintext")};
    labels += headers;
    // setHeaderLabels() resets the resize modes, so configure them afterwards.
    ui->treeWidget_results->setHeaderLabels(labels);

    QHeaderView *header = ui->treeWidget_results->header();
    for (int column = 0; column < labels.size(); ++column) {
        header->setSectionResizeMode(column, QHeaderView::Interactive);
    }
    header->resizeSection(0, 320);
    header->resizeSection(1, 360);
    for (int column = 2; column < labels.size(); ++column) {
        header->resizeSection(column, 90);
    }
}

void PotfileTableView::setSearchPlaceholder(const QString &text)
{
    ui->lineEdit_search->setPlaceholderText(text);
}

void PotfileTableView::setColumnHidden(int column, bool hidden)
{
    ui->treeWidget_results->setColumnHidden(column, hidden);
}

void PotfileTableView::setEmptyStateText(const QString &text)
{
    m_emptyStateText = text;
    updateEmptyState();
}

void PotfileTableView::setRows(const QList<Row> &rows, bool preserveViewState)
{
    applyRows(rows, true, preserveViewState);
}

void PotfileTableView::clearRows(bool preserveViewState)
{
    applyRows({}, false, preserveViewState);
}

void PotfileTableView::applyRows(const QList<Row> &rows, bool showEmptyState,
                                 bool preserveViewState)
{
    m_rows = rows;
    m_showEmptyState = showEmptyState;
    if (!preserveViewState) {
        m_revealed = false;
        const QSignalBlocker searchBlocker(ui->lineEdit_search);
        const QSignalBlocker revealBlocker(ui->pushButton_reveal);
        ui->lineEdit_search->clear();
        ui->pushButton_reveal->setChecked(false);
        ui->treeWidget_results->header()->setSortIndicator(0, Qt::AscendingOrder);
    }
    populateRows();
    if (preserveViewState) {
        ui->treeWidget_results->clearSelection();
    }
    updateRevealButton();
    updateButtons();
}

bool PotfileTableView::hasRows() const
{
    return !m_rows.isEmpty();
}

int PotfileTableView::rowCount() const
{
    return static_cast<int>(m_rows.size());
}

bool PotfileTableView::canReveal() const
{
    return std::ranges::any_of(m_rows,
                               [](const Row &row) { return !row.emptyPlain && !row.encrypted; });
}

QList<PotfileTableView::Row> PotfileTableView::visibleRows() const
{
    QList<Row> rows;
    for (int i = 0; i < ui->treeWidget_results->topLevelItemCount(); ++i) {
        const QTreeWidgetItem *item = ui->treeWidget_results->topLevelItem(i);
        if (item->isHidden()) {
            continue;
        }
        if (const Row *row = rowForItem(item)) {
            rows.append(*row);
        }
    }
    return rows;
}

int PotfileTableView::visibleRowCount() const
{
    int count = 0;
    for (int i = 0; i < ui->treeWidget_results->topLevelItemCount(); ++i) {
        if (!ui->treeWidget_results->topLevelItem(i)->isHidden()) {
            ++count;
        }
    }
    return count;
}

bool PotfileTableView::eventFilter(QObject *watched, QEvent *event)
{
    if ((watched == ui->treeWidget_results || watched == ui->treeWidget_results->viewport()
         || watched == ui->lineEdit_search)
        && event->type() == QEvent::KeyPress) {
        const auto *key = dynamic_cast<const QKeyEvent *>(event);
        if (key != nullptr && (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter)) {
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void PotfileTableView::searchChanged()
{
    // At 100k rows a synchronous pass per keystroke is visible; wait for a
    // pause instead. Small tables filter at once, as they always have.
    if (m_rows.size() > AppConstants::Hashcat::PotfileFilterDebounceThreshold) {
        m_filterTimer.start();
        return;
    }
    applyFilter();
}

void PotfileTableView::revealToggled(bool revealed)
{
    m_revealed = revealed;
    for (int i = 0; i < ui->treeWidget_results->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->treeWidget_results->topLevelItem(i);
        if (const Row *row = rowForItem(item)) {
            item->setText(1, plainTextFor(*row));
        }
    }
    updateRevealButton();
    applyFilter();
}

void PotfileTableView::copySelected()
{
    // The button state carries the mask gate; the shortcut shares it.
    if (!ui->pushButton_copy_selected->isEnabled()) {
        return;
    }
    QList<Row> rows;
    for (int i = 0; i < ui->treeWidget_results->topLevelItemCount(); ++i) {
        const QTreeWidgetItem *item = ui->treeWidget_results->topLevelItem(i);
        if (!item->isSelected() || item->isHidden()) {
            continue;
        }
        if (const Row *row = rowForItem(item)) {
            rows.append(*row);
        }
    }
    copyRows(rows);
}

void PotfileTableView::copyAll()
{
    if (!ui->pushButton_copy_all->isEnabled()) {
        return;
    }
    copyRows(visibleRows());
}

void PotfileTableView::copyRows(const QList<Row> &rows)
{
    if (rows.isEmpty()) {
        return;
    }
    QStringList lines;
    lines.reserve(rows.size());
    for (const Row &row : rows) {
        lines.append(copyValuesFor(row).join(QLatin1Char('\t')));
    }
    // QApplication::clipboard() is null on platforms without clipboard support.
    if (QClipboard *clipboard = QApplication::clipboard()) {
        clipboard->setText(lines.join(QLatin1Char('\n')));
        QToolTip::showText(
            QCursor::pos(),
            countText(static_cast<int>(rows.size()), tr("Copied 1 row"), tr("Copied %1 rows")),
            ui->treeWidget_results, QRect(), 2000);
    }
}

void PotfileTableView::showContextMenu(const QPoint &pos)
{
    // Right-clicking an unselected row targets that row, as users expect.
    QTreeWidgetItem *item = ui->treeWidget_results->itemAt(pos);
    if (item != nullptr && !item->isSelected()) {
        ui->treeWidget_results->setCurrentItem(item);
    }
    QMenu menu(this);
    QAction *copy = menu.addAction(tr("Copy selected rows"));
    copy->setEnabled(ui->pushButton_copy_selected->isEnabled());
    connect(copy, &QAction::triggered, this, &PotfileTableView::copySelected);
    menu.exec(ui->treeWidget_results->viewport()->mapToGlobal(pos));
}

void PotfileTableView::updateButtons()
{
    const bool hasVisible = hasVisibleRows();
    // Masked plaintexts must not leak through the clipboard.
    const bool copyable = m_revealed || !canReveal();
    ui->pushButton_copy_all->setEnabled(hasVisible && copyable);
    ui->pushButton_copy_selected->setEnabled(hasVisible && copyable && hasVisibleSelection());
    ui->pushButton_reveal->setEnabled(hasRows() && canReveal());

    const QString copyToolTip =
        copyable ? tr("Copies tab-separated rows. Encrypted rows copy the stored $HCENC$ token.")
                 : tr("Reveal plaintexts to copy the real values.");
    ui->pushButton_copy_all->setToolTip(copyToolTip);
    ui->pushButton_copy_selected->setToolTip(copyToolTip);
}

void PotfileTableView::updateRevealButton()
{
    ui->pushButton_reveal->setText(m_revealed ? tr("Hide plaintexts") : tr("Reveal plaintexts"));
    ui->pushButton_reveal->setToolTip(tr("Show or mask plaintext values."));
}

bool PotfileTableView::hasVisibleRows() const
{
    return visibleRowCount() > 0;
}

bool PotfileTableView::hasVisibleSelection() const
{
    const auto selected = ui->treeWidget_results->selectedItems();
    return std::ranges::any_of(selected,
                               [](const QTreeWidgetItem *item) { return !item->isHidden(); });
}

void PotfileTableView::populateRows()
{
    ui->treeWidget_results->setSortingEnabled(false);
    ui->treeWidget_results->clear();
    for (int i = 0; i < m_rows.size(); ++i) {
        const Row &row = m_rows.at(i);
        auto *item = new QTreeWidgetItem(ui->treeWidget_results);
        item->setText(0, hashDisplayFor(row));
        item->setToolTip(0, row.hashIsFile ? tr("Binary hash file; hashcat reports the file name.")
                                           : row.hash);
        item->setText(1, plainTextFor(row));
        for (int column = 0; column < row.extraColumns.size(); ++column) {
            item->setText(2 + column, row.extraColumns.at(column));
        }
        item->setData(0, Qt::UserRole, i);
    }
    ui->treeWidget_results->setSortingEnabled(true);
    applyFilter();
}

const PotfileTableView::Row *PotfileTableView::rowForItem(const QTreeWidgetItem *item) const
{
    const int index = item->data(0, Qt::UserRole).toInt();
    if (index < 0 || index >= m_rows.size()) {
        return nullptr;
    }
    return &m_rows.at(index);
}

void PotfileTableView::applyFilter()
{
    m_filterTimer.stop();
    for (int i = 0; i < ui->treeWidget_results->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->treeWidget_results->topLevelItem(i);
        if (const Row *row = rowForItem(item)) {
            // Qt's first setHidden() per item is costly (~10 µs); skip no-ops.
            const bool hide = !rowMatches(*row);
            if (item->isHidden() != hide) {
                item->setHidden(hide);
            }
        }
    }
    updateEmptyState();
    updateButtons();
    emit visibleRowsChanged();
}

void PotfileTableView::updateEmptyState()
{
    QString text;
    bool showLabel = false;
    if (m_showEmptyState && m_rows.isEmpty() && !m_emptyStateText.isEmpty()) {
        text = m_emptyStateText;
        showLabel = true;
    } else if (!m_rows.isEmpty() && !hasVisibleRows()) {
        text = tr("No rows match the filter.");
        showLabel = true;
    }
    if (showLabel) {
        ui->label_empty_state->setText(text);
    }
    QWidget *page = showLabel ? static_cast<QWidget *>(ui->label_empty_state)
                              : static_cast<QWidget *>(ui->treeWidget_results);
    ui->table_stack->setCurrentWidget(page);
}

QString PotfileTableView::plainTextFor(const Row &row) const
{
    if (row.encrypted) {
        return tr("(encrypted)");
    }
    if (row.emptyPlain) {
        return tr("(empty)");
    }
    return m_revealed ? row.plain : QStringLiteral("••••••");
}

QString PotfileTableView::hashDisplayFor(const Row &row) const
{
    return row.hashIsFile ? tr("(file) %1").arg(row.hash) : row.hash;
}

QStringList PotfileTableView::copyValuesFor(const Row &row) const
{
    // The raw name even for file rows; the encrypted token is not a plaintext.
    QStringList values{row.hash, row.emptyPlain ? QString() : row.plain};
    values += row.extraColumns;
    return values;
}

bool PotfileTableView::rowMatches(const Row &row) const
{
    const QString query = ui->lineEdit_search->text().trimmed();
    if (query.isEmpty()) {
        return true;
    }

    if (row.hash.contains(query, Qt::CaseInsensitive)
        || row.searchExtra.contains(query, Qt::CaseInsensitive)) {
        return true;
    }

    // Matching hidden plaintexts would leak them one guess at a time.
    return m_revealed && !row.encrypted && row.plain.contains(query, Qt::CaseInsensitive);
}