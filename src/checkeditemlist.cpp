/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "checkeditemlist.h"

#include <QDir>
#include <QListWidget>
#include <QListWidgetItem>

void CheckedItemList::addPaths(QListWidget *list, const QStringList &paths)
{
    // The file and folder dialogs and the ruleset name field end here: the
    // paths that are not rows yet, once each, in the platform's own separator
    // form. Every row starts checked; unchecking it later keeps the row in
    // the list but leaves it out of the command.
    for (const QString &path : paths) {
        const QString native = QDir::toNativeSeparators(path);
        if (native.isEmpty() || !list->findItems(native, Qt::MatchExactly).isEmpty()) {
            continue;
        }

        auto *item = new QListWidgetItem(native, list);
        item->setCheckState(Qt::Checked);
    }
}

void CheckedItemList::addName(QListWidget *list, const QString &name)
{
    // A bundled or installed ruleset is a name the dialog cannot return, not
    // a file. hashcat treats a name containing a slash as a path, so a pasted
    // path works here too and needs no separate branch.
    const QString trimmed = name.trimmed();
    if (trimmed.isEmpty()) {
        return;
    }
    addPaths(list, {trimmed});
}

QStringList CheckedItemList::checkedItems(const QListWidget *list)
{
    QStringList items;
    items.reserve(list->count());
    for (int i = 0; i < list->count(); ++i) {
        const QListWidgetItem *item = list->item(i);
        if (item->checkState() == Qt::Checked) {
            items << item->text();
        }
    }
    return items;
}

void CheckedItemList::removeSelected(QListWidget *list)
{
    // The rows know their list and take themselves out of it when deleted.
    const QList<QListWidgetItem *> selected = list->selectedItems();
    for (QListWidgetItem *item : selected) {
        delete item;
    }
    list->clearSelection();
}

void CheckedItemList::moveSelected(QListWidget *list, int delta)
{
    // currentRow() is -1 when nothing is selected: without the guards below
    // takeItem(-1) returns nullptr and the list ends up with a null entry.
    const int currentRow = list->currentRow();
    const int targetRow = currentRow + delta;
    if (currentRow < 0 || targetRow < 0 || targetRow >= list->count()) {
        return;
    }
    QListWidgetItem *item = list->takeItem(currentRow);
    list->insertItem(targetRow, item);
    list->setCurrentRow(targetRow);
}
