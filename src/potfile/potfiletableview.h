/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef POTFILETABLEVIEW_H
#define POTFILETABLEVIEW_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QWidget>

namespace Ui
{
class PotfileTableView;
}

class QTreeWidgetItem;

// The search, row and copy view shared by the potfile dialogs. Plaintexts are
// masked until explicitly revealed, and the search never matches a hidden
// plaintext; the owning dialog only maps its own entry type onto Row.
class PotfileTableView : public QWidget
{
    Q_OBJECT

public:
    struct Row {
        QString hash;
        QString plain;
        // Extra searchable text that is not a plaintext, e.g. the username.
        QString searchExtra;
        // Texts for the columns after Hash and Plaintext.
        QStringList extraColumns;
        bool emptyPlain = false;
        bool encrypted = false;
    };

    explicit PotfileTableView(QWidget *parent = nullptr);
    ~PotfileTableView() override;

    PotfileTableView(const PotfileTableView &) = delete;
    PotfileTableView &operator=(const PotfileTableView &) = delete;
    PotfileTableView(PotfileTableView &&) = delete;
    PotfileTableView &operator=(PotfileTableView &&) = delete;

    // Headers after Hash and Plaintext; the owning dialog chooses them.
    void setExtraColumns(const QStringList &headers);
    void setSearchPlaceholder(const QString &text);
    void setColumnHidden(int column, bool hidden);
    // Shown instead of the table when setRows() supplies no rows.
    void setEmptyStateText(const QString &text);

    // Replaces the rows and resets search and reveal.
    void setRows(const QList<Row> &rows);
    void clearRows();

    [[nodiscard]] bool hasRows() const;
    [[nodiscard]] bool canReveal() const;

private slots:
    void searchChanged();
    void revealToggled(bool revealed);
    void copySelected();
    void copyAll();
    void updateButtons();

private:
    void applyRows(const QList<Row> &rows, bool showEmptyState);
    void populateRows();
    void applyFilter();
    void updateEmptyState();
    [[nodiscard]] bool hasVisibleRows() const;
    [[nodiscard]] bool hasVisibleSelection() const;
    [[nodiscard]] const Row *rowForItem(const QTreeWidgetItem *item) const;
    [[nodiscard]] QString plainTextFor(const Row &row) const;
    [[nodiscard]] QString copyValueFor(const Row &row) const;
    [[nodiscard]] bool rowMatches(const Row &row) const;
    [[nodiscard]] QStringList selectedCopyValues() const;

    Ui::PotfileTableView *ui;
    QList<Row> m_rows;
    bool m_revealed = false;
    bool m_showEmptyState = false;
};

#endif // POTFILETABLEVIEW_H