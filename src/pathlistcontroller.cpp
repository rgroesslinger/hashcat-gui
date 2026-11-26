/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "pathlistcontroller.h"

#include "checkeditemlist.h"
#include "textutils.h"

#include <QAbstractItemModel>
#include <QFileInfo>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSignalBlocker>
#include <utility>

PathListController::PathListController(QListWidget *list, QObject *parent)
    : QObject(parent), m_list(list)
{
    connectChanges();
}

PathListController::PathListController(QListWidget *list, PathMonitor::Kind kind, QIcon fileIcon,
                                       QIcon folderIcon, QIcon warningIcon, QObject *parent)
    : QObject(parent), m_list(list), m_monitor(std::make_unique<PathMonitor>(kind)),
      m_fileIcon(std::move(fileIcon)), m_folderIcon(std::move(folderIcon)),
      m_warningIcon(std::move(warningIcon))
{
    connectChanges();

    // The rows are the source of the watched paths: a path arrives through
    // the file dialog, a drop, a session or a reset, and all of them change
    // the model. clear() - and with it a session restore or a reset - resets
    // the model instead of removing the rows one by one.
    connect(m_list->model(), &QAbstractItemModel::rowsInserted, this,
            &PathListController::refreshMonitor);
    connect(m_list->model(), &QAbstractItemModel::rowsRemoved, this,
            &PathListController::refreshMonitor);
    connect(m_list->model(), &QAbstractItemModel::modelReset, this,
            &PathListController::refreshMonitor);
    connect(m_monitor.get(), &PathMonitor::availabilityChanged, this,
            &PathListController::setRowAvailability);
}

void PathListController::connectChanges()
{
    connect(m_list->model(), &QAbstractItemModel::rowsInserted, this, &PathListController::changed);
    connect(m_list->model(), &QAbstractItemModel::rowsRemoved, this, &PathListController::changed);
    // A move emits rowsMoved as its single notification (see
    // CheckedItemList::moveSelected). The watched paths do not change, so only
    // the command is rebuilt - no monitor refresh is connected.
    connect(m_list->model(), &QAbstractItemModel::rowsMoved, this, &PathListController::changed);
    connect(m_list, &QListWidget::itemChanged, this, &PathListController::changed);
}

void PathListController::addPaths(const QStringList &paths)
{
    CheckedItemList::addPaths(m_list, paths);
}

QStringList PathListController::checkedItems() const
{
    return CheckedItemList::checkedItems(m_list);
}

void PathListController::setHashcatPath(const QString &hashcatPath)
{
    if (m_monitor) {
        m_monitor->setHashcatPath(hashcatPath);
    }
}

void PathListController::removeSelected()
{
    CheckedItemList::removeSelected(m_list);
}

void PathListController::moveSelected(int delta)
{
    CheckedItemList::moveSelected(m_list, delta);
}

void PathListController::refreshMonitor()
{
    QStringList paths;
    paths.reserve(m_list->count());
    for (int i = 0; i < m_list->count(); ++i) {
        paths << m_list->item(i)->text();
    }
    m_monitor->setPaths(paths);
}

// Why a row hashcat cannot use is unusable, in the terms of its list. The path
// may contain markup-like text, so the result is escaped for the rich text
// tooltip.
QString PathListController::unavailableTooltip(const QString &path) const
{
    const bool folder = QFileInfo(m_monitor->resolvePath(path)).isDir();

    switch (m_monitor->kind()) {
    case PathMonitor::Kind::Wordlist:
        if (folder) {
            return escapeRichText(tr("This folder has no wordlists. hashcat reads the regular "
                                     "files directly inside it, not subfolders."));
        }
        return escapeRichText(tr("This wordlist does not exist:\n%1").arg(path));
    case PathMonitor::Kind::Pcfg:
        if (folder) {
            return escapeRichText(
                tr("This ruleset folder has no Grammar/grammar.txt:\n%1").arg(path));
        }
        return escapeRichText(tr("This ruleset does not exist:\n%1").arg(path));
    case PathMonitor::Kind::Table:
        if (folder) {
            return escapeRichText(
                tr("A table must be a file; hashcat does not read a folder:\n%1").arg(path));
        }
        return escapeRichText(tr("This table does not exist:\n%1").arg(path));
    }

    return {};
}

void PathListController::setRowAvailability(const QString &path, bool available)
{
    // Writing an icon or a tooltip is an item change, which rebuilds the
    // preview through the changed() signal. The blocker keeps that out: the
    // decoration is not part of the command.
    const QSignalBlocker blocker(m_list);

    // A usable path shows what it is; an unusable one cannot be trusted with
    // a type, so the warning takes the icon's place. A bundled ruleset name
    // has neither a file nor a folder behind it and stays without one.
    const QFileInfo info(m_monitor->resolvePath(path));
    QIcon icon;
    if (!available) {
        icon = m_warningIcon;
    } else if (info.isDir()) {
        icon = m_folderIcon;
    } else if (info.isFile()) {
        icon = m_fileIcon;
    }

    const QString tooltip = available ? QString() : unavailableTooltip(path);

    for (int i = 0; i < m_list->count(); ++i) {
        QListWidgetItem *item = m_list->item(i);
        if (item->text() != path) {
            continue;
        }

        if (item->icon().cacheKey() != icon.cacheKey()) {
            item->setIcon(icon);
        }
        if (item->toolTip() != tooltip) {
            item->setToolTip(tooltip);
        }
    }
}