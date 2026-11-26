/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef POTFILETABLEVIEW_H
#define POTFILETABLEVIEW_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QTimer>
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
        bool hashIsFile = false;
        // Set by the owning dialog: maps the row to its retained entry.
        int sourceIndex = -1;
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

    // Replaces the rows. preserveViewState keeps search, reveal and sorting
    // (the Inspector's Reload); the default resets them.
    void setRows(const QList<Row> &rows, bool preserveViewState = false);
    // Empties the data without the empty-state hint; for no-result paths.
    void clearRows(bool preserveViewState = false);

    [[nodiscard]] bool hasRows() const;
    [[nodiscard]] int rowCount() const;
    [[nodiscard]] bool canReveal() const;
    // Whether the user has revealed the plaintexts; exports need this.
    [[nodiscard]] bool isRevealed() const
    {
        return m_revealed;
    }
    // Whether an applied filter narrows the rows.
    [[nodiscard]] bool isFiltered() const;
    // Visible rows in current view (sort) order.
    [[nodiscard]] QList<Row> visibleRows() const;
    [[nodiscard]] int visibleRowCount() const;

signals:
    // Filter, reveal state or row set changed.
    void visibleRowsChanged();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void searchChanged();
    void revealToggled(bool revealed);
    void copySelected();
    void copyAll();
    void updateButtons();
    void showContextMenu(const QPoint &pos);

private:
    void applyRows(const QList<Row> &rows, bool showEmptyState, bool preserveViewState);
    void populateRows();
    void applyFilter();
    void updateEmptyState();
    void updateRevealButton();
    [[nodiscard]] bool hasVisibleRows() const;
    [[nodiscard]] bool hasVisibleSelection() const;
    [[nodiscard]] const Row *rowForItem(const QTreeWidgetItem *item) const;
    [[nodiscard]] QString plainTextFor(const Row &row) const;
    [[nodiscard]] QString hashDisplayFor(const Row &row) const;
    [[nodiscard]] QStringList copyValuesFor(const Row &row) const;
    [[nodiscard]] bool rowMatches(const Row &row) const;
    void copyRows(const QList<Row> &rows);

    Ui::PotfileTableView *ui;
    QList<Row> m_rows;
    QString m_emptyStateText;
    QTimer m_filterTimer;
    bool m_revealed = false;
    bool m_showEmptyState = false;
    QString m_appliedFilter;
};

#endif // POTFILETABLEVIEW_H