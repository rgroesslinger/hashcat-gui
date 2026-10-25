/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef WORDLISTMONITOR_H
#define WORDLISTMONITOR_H

#include <QFileSystemWatcher>
#include <QHash>
#include <QObject>
#include <QStringList>

// Keeps track of whether the wordlist files still exist.
//
// QFileSystemWatcher cannot watch a path that is gone, and deleting a watched
// file invalidates its watch. The monitor therefore watches the parent
// directories and asks QFileInfo about the paths themselves: a directory emits
// when one of its entries is created, removed or renamed, and the watch keeps
// working across all three.
//
// A path whose parent directory is gone is watched through the nearest
// existing ancestor instead, so recreating the directory is noticed as well.
class WordlistMonitor : public QObject
{
    Q_OBJECT

public:
    explicit WordlistMonitor(QObject *parent = nullptr);

    // Replaces the watched set. Every path is reported, so a row that was just
    // inserted receives its state even when the path was watched before.
    void setPaths(const QStringList &paths);

    // Whether the last check found a regular file at path.
    bool isAvailable(const QString &path) const;

signals:
    void availabilityChanged(const QString &path, bool available);

private:
    void recheck();
    void updateWatches();

    QFileSystemWatcher m_watcher;
    QStringList m_paths;
    QHash<QString, bool> m_available;
};

#endif // WORDLISTMONITOR_H
