/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef PATHLISTCONTROLLER_H
#define PATHLISTCONTROLLER_H

#include "pathmonitor.h"

#include <QIcon>
#include <QObject>
#include <memory>

class QListWidget;

// One argument list, its checked rows and the monitor that says which paths
// hashcat can use. It takes paths and never opens a dialog.
//
// Destroy before the list's ui or disconnect: ~QListWidget resets its model
// while the connections still run (rowsRemoved fires before ~QObject).
class PathListController : public QObject
{
    Q_OBJECT

public:
    // A list no monitor watches (the feed arguments).
    PathListController(QListWidget *list, QObject *parent);

    // A monitored list. The icons are the style's own: a file, a folder, and
    // the warning that takes their place while a path is unusable.
    PathListController(QListWidget *list, PathMonitor::Kind kind, QIcon fileIcon, QIcon folderIcon,
                       QIcon warningIcon, QObject *parent);

    void addPaths(const QStringList &paths);
    [[nodiscard]] QStringList checkedItems() const;
    void removeSelected();
    void moveSelected(int delta);

signals:
    // The rows changed in a way that reaches the command.
    void changed();

private:
    void connectChanges();
    void refreshMonitor();
    void setRowAvailability(const QString &path, bool available);
    [[nodiscard]] QString unavailableTooltip(const QString &path) const;

    QListWidget *m_list;
    std::unique_ptr<PathMonitor> m_monitor;
    QIcon m_fileIcon;
    QIcon m_folderIcon;
    QIcon m_warningIcon;
};

#endif // PATHLISTCONTROLLER_H