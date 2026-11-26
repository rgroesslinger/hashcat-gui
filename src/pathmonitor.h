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

// Whether the paths of one list are usable by hashcat.
//
// QFileSystemWatcher cannot watch a gone path, so the parents are watched and
// QFileInfo answers for the paths. Wordlist and PCFG folders are watched
// themselves; a gone parent is watched through its nearest existing ancestor.
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

    [[nodiscard]] Kind kind() const
    {
        return m_kind;
    }

    // Replaces the watched set. Every path is reported, so a row that was just
    // inserted receives its state even when the path was watched before.
    void setPaths(const QStringList &paths);

    // Sets the program directory for relative paths and rechecks them.
    void setHashcatPath(const QString &hashcatPath);

    // The path as hashcat reads it; bundled ruleset names stay names.
    [[nodiscard]] QString resolvePath(const QString &path) const;

    [[nodiscard]] bool isAvailable(const QString &path) const;

signals:
    void availabilityChanged(const QString &path, bool available);

private:
    [[nodiscard]] bool isUsable(const QString &path) const;
    [[nodiscard]] QStringList watchDirectories(const QString &path) const;
    void recheck();
    void updateWatches();

    Kind m_kind;
    QFileSystemWatcher m_watcher;
    QStringList m_paths;
    QString m_hashcatPath;
    QHash<QString, bool> m_available;
};

#endif // PATHMONITOR_H
