/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef PATHMONITOR_H
#define PATHMONITOR_H

#include <QFileSystemWatcher>
#include <QHash>
#include <QObject>
#include <QStringList>

#include <cstdint>

// Keeps track of whether the paths of one list are usable by hashcat.
//
// QFileSystemWatcher cannot watch a path that is gone, and deleting a watched
// path invalidates its watch. The monitor therefore watches the parent
// directories and asks QFileInfo about the paths themselves: a directory emits
// when one of its entries is created, removed or renamed, and the watch keeps
// working across all three.
//
// A folder is an input of its own for wordlists and PCFG rulesets, so those
// monitors watch the folder too: a wordlist folder says nothing about its files,
// and a ruleset folder says nothing about its grammar. A path whose parent
// directory is gone is watched through the nearest existing ancestor instead,
// so recreating the directory is noticed as well.
class PathMonitor : public QObject
{
    Q_OBJECT

public:
    // What hashcat accepts for the paths of a list.
    enum class Kind : std::uint8_t {
        Wordlist, // a file, or a folder with at least one wordlist in it
        Pcfg,     // a ruleset file, a folder with a grammar, or a bundled name
        Table,    // a file only
    };

    explicit PathMonitor(Kind kind, QObject *parent = nullptr);

    // Replaces the watched set. Every path is reported, so a row that was just
    // inserted receives its state even when the path was watched before.
    void setPaths(const QStringList &paths);

    // Whether the last check found a path hashcat can use.
    bool isAvailable(const QString &path) const;

signals:
    void availabilityChanged(const QString &path, bool available);

private:
    bool isUsable(const QString &path) const;
    QStringList watchDirectories(const QString &path) const;
    void recheck();
    void updateWatches();

    Kind m_kind;
    QFileSystemWatcher m_watcher;
    QStringList m_paths;
    QHash<QString, bool> m_available;
};

#endif // PATHMONITOR_H
