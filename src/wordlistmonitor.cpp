/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "wordlistmonitor.h"

#include <QFileInfo>

#include <utility>

WordlistMonitor::WordlistMonitor(QObject *parent) : QObject(parent)
{
    connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this, [this] {
        recheck();
        // A directory that was renamed or removed loses its watch; add it
        // again if it is back under the same name.
        updateWatches();
    });
}

void WordlistMonitor::setPaths(const QStringList &paths)
{
    m_paths.clear();
    for (const QString &path : paths) {
        if (!path.isEmpty() && !m_paths.contains(path)) {
            m_paths << path;
        }
    }

    // Forget the old answers, so recheck() reports every path: the caller has
    // just built rows that have no state yet.
    m_available.clear();
    updateWatches();
    recheck();
}

bool WordlistMonitor::isAvailable(const QString &path) const
{
    return m_available.value(path, false);
}

void WordlistMonitor::recheck()
{
    for (const QString &path : std::as_const(m_paths)) {
        // Since hashcat 8.0.0 a directory is a valid wordlist input (the
        // wordlist and table feeds take one), so it counts as available.
        const bool available = QFileInfo(path).exists();
        if (m_available.contains(path) && m_available.value(path) == available) {
            continue;
        }

        m_available.insert(path, available);
        emit availabilityChanged(path, available);
    }
}

void WordlistMonitor::updateWatches()
{
    QStringList directories;
    for (const QString &path : std::as_const(m_paths)) {
        QString directory = QFileInfo(path).absolutePath();

        // The parent is gone: watch the nearest existing ancestor, so that a
        // recreated directory produces an event and the next update can follow
        // it down again. The walk stops at the root - and at a drive or share
        // that is not available at all.
        while (!QFileInfo(directory).isDir()) {
            const QString parent = QFileInfo(directory).absolutePath();
            if (parent.isEmpty() || parent == directory) {
                directory.clear();
                break;
            }
            directory = parent;
        }

        if (!directory.isEmpty() && !directories.contains(directory)) {
            directories << directory;
        }
    }

    const QStringList watched = m_watcher.directories();
    for (const QString &directory : std::as_const(watched)) {
        if (!directories.contains(directory)) {
            m_watcher.removePath(directory);
        }
    }
    for (const QString &directory : std::as_const(directories)) {
        if (!watched.contains(directory)) {
            m_watcher.addPath(directory);
        }
    }
}
