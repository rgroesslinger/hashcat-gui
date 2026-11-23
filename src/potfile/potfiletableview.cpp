/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfiletableview.h"

#include "ui_potfiletableview.h"

#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QLineEdit>
#include <QPalette>
#include <QPushButton>
#include <QSignalBlocker>
#include <QStackedLayout>
#include <QTreeWidgetItem>

#include <algorithm>

PotfileTableView::PotfileTableView(QWidget *parent) : QWidget(parent), ui(new Ui::PotfileTableView)
{
    ui->setupUi(this);

    ui->treeWidget_results->setTextElideMode(Qt::ElideMiddle);
    QPalette emptyPalette = ui->label_empty_state->palette();
    emptyPalette.setColor(QPalette::WindowText, emptyPalette.color(QPalette::PlaceholderText));
    ui->label_empty_state->setPalette(emptyPalette);

    connect(ui->lineEdit_search, &QLineEdit::textChanged, this, &PotfileTableView::searchChanged);
    connect(ui->pushButton_reveal, &QPushButton::toggled, this, &PotfileTableView::revealToggled);
    connect(ui->pushButton_copy_selected, &QPushButton::clicked, this,
            &PotfileTableView::copySelected);
    connect(ui->pushButton_copy_all, &QPushButton::clicked, this, &PotfileTableView::copyAll);
    connect(ui->treeWidget_results, &QTreeWidget::itemSelectionChanged, this,
            &PotfileTableView::updateButtons);

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
    header->setSectionResizeMode(0, QHeaderView::Interactive);
    header->resizeSection(0, 320);
    header->setSectionResizeMode(1, QHeaderView::Stretch);
    for (int column = 2; column < labels.size(); ++column) {
        header->setSectionResizeMode(column, QHeaderView::ResizeToContents);
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
    ui->label_empty_state->setText(text);
    updateEmptyState();
}

void PotfileTableView::setRows(const QList<Row> &rows)
{
    applyRows(rows, true);
}

void PotfileTableView::clearRows()
{
    applyRows({}, false);
}

void PotfileTableView::applyRows(const QList<Row> &rows, bool showEmptyState)
{
    m_rows = rows;
    m_showEmptyState = showEmptyState;
    m_revealed = false;
    const QSignalBlocker searchBlocker(ui->lineEdit_search);
    const QSignalBlocker revealBlocker(ui->pushButton_reveal);
    ui->lineEdit_search->clear();
    ui->pushButton_reveal->setChecked(false);
    populateRows();
}

bool PotfileTableView::hasRows() const
{
    return !m_rows.isEmpty();
}

bool PotfileTableView::canReveal() const
{
    return std::ranges::any_of(m_rows,
                               [](const Row &row) { return !row.emptyPlain && !row.encrypted; });
}

void PotfileTableView::searchChanged()
{
    applyFilter();
}

void PotfileTableView::revealToggled(bool revealed)
{
    m_revealed = revealed;
    for (int i = 0; i < ui->treeWidget_results->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->treeWidget_results->topLevelItem(i);
        const Row *row = rowForItem(item);
        if (row) {
            item->setText(1, plainTextFor(*row));
            item->setHidden(!rowMatches(*row));
        }
    }
    updateButtons();
}

void PotfileTableView::copySelected()
{
    const QStringList values = selectedCopyValues();
    if (!values.isEmpty()) {
        QApplication::clipboard()->setText(values.join(QLatin1Char('\n')));
    }
}

void PotfileTableView::copyAll()
{
    QStringList values;
    for (int i = 0; i < ui->treeWidget_results->topLevelItemCount(); ++i) {
        const QTreeWidgetItem *item = ui->treeWidget_results->topLevelItem(i);
        if (item->isHidden()) {
            continue;
        }
        if (const Row *row = rowForItem(item)) {
            values << copyValueFor(*row);
        }
    }
    if (!values.isEmpty()) {
        QApplication::clipboard()->setText(values.join(QLatin1Char('\n')));
    }
}

void PotfileTableView::updateButtons()
{
    const bool hasRows = !m_rows.isEmpty();
    const bool hasVisible = hasVisibleRows();
    ui->pushButton_copy_all->setEnabled(hasVisible);
    ui->pushButton_copy_selected->setEnabled(hasVisible && hasVisibleSelection());
    ui->pushButton_reveal->setEnabled(hasRows && canReveal());
}

bool PotfileTableView::hasVisibleRows() const
{
    for (int i = 0; i < ui->treeWidget_results->topLevelItemCount(); ++i) {
        if (!ui->treeWidget_results->topLevelItem(i)->isHidden()) {
            return true;
        }
    }
    return false;
}

bool PotfileTableView::hasVisibleSelection() const
{
    const auto selected = ui->treeWidget_results->selectedItems();
    return std::ranges::any_of(selected,
                               [](const QTreeWidgetItem *item) { return !item->isHidden(); });
}

void PotfileTableView::populateRows()
{
    ui->treeWidget_results->clear();
    for (int i = 0; i < m_rows.size(); ++i) {
        const Row &row = m_rows.at(i);
        auto *item = new QTreeWidgetItem(ui->treeWidget_results);
        item->setText(0, row.hash);
        item->setText(1, plainTextFor(row));
        for (int column = 0; column < row.extraColumns.size(); ++column) {
            item->setText(2 + column, row.extraColumns.at(column));
        }
        item->setData(0, Qt::UserRole, i);
    }

    applyFilter();
    updateEmptyState();
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
    for (int i = 0; i < ui->treeWidget_results->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->treeWidget_results->topLevelItem(i);
        if (const Row *row = rowForItem(item)) {
            item->setHidden(!rowMatches(*row));
        }
    }
    updateButtons();
}

void PotfileTableView::updateEmptyState()
{
    const bool show =
        m_showEmptyState && m_rows.isEmpty() && !ui->label_empty_state->text().isEmpty();
    QWidget *page = show ? static_cast<QWidget *>(ui->label_empty_state)
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

QString PotfileTableView::copyValueFor(const Row &row) const
{
    // The encrypted token is not a plaintext, so reveal never changes it.
    return row.plain;
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

QStringList PotfileTableView::selectedCopyValues() const
{
    QStringList values;
    for (int i = 0; i < ui->treeWidget_results->topLevelItemCount(); ++i) {
        const QTreeWidgetItem *item = ui->treeWidget_results->topLevelItem(i);
        if (!item->isSelected() || item->isHidden()) {
            continue;
        }
        if (const Row *row = rowForItem(item)) {
            values << copyValueFor(*row);
        }
    }
    return values;
}