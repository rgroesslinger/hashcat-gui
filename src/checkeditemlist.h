/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef CHECKEDITEMLIST_H
#define CHECKEDITEMLIST_H

#include <QString>
#include <QStringList>
#include <functional>

class QListWidget;
class QObject;

// Row handling shared by every argument list. An unchecked row keeps its
// place, its path and its text - but stays out of the command.
class CheckedItemList
{
public:
    // The paths that are not rows yet, once each, in the platform's own
    // separator form. Every row starts checked.
    static void addPaths(QListWidget *list, const QStringList &paths);

    // A typed name - a bundled ruleset, not a file: trimmed, and added only
    // when something is left.
    static void addName(QListWidget *list, const QString &name);

    // The checked rows, in list order: what reaches the command.
    static QStringList checkedItems(const QListWidget *list);

    static void removeSelected(QListWidget *list);

    // Moves the current row by delta; a missing selection or a move past
    // either end does nothing.
    static void moveSelected(QListWidget *list, int delta);

    // The model events that change the command: rows arriving and leaving,
    // and in-place edits (the check state included). The receiver owns the
    // connection's lifetime.
    static void connectModelChanges(QListWidget *list, QObject *receiver,
                                    const std::function<void()> &changed);
};

#endif // CHECKEDITEMLIST_H