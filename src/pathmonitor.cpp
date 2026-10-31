/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "pathmonitor.h"

#include <QDir>
#include <QFileInfo>

#include <utility>

namespace
{
// A name without a separator is what hashcat resolves in its own directory
// ("default-passwords", "hints"), so the GUI cannot know whether it exists.
bool isBundledName(const QString &path)
{
    return !path.contains(QLatin1Char('/')) && !path.contains(QLatin1Char('\\'))
           && !QDir::isAbsolutePath(path);
}

// hashcat's scan_directory() reads the non-hidden regular files directly inside
// a folder and does not recurse; a folder without one leaves the feed with
// nothing to read and the run stops with "No usable dictionary file found."
bool hasWordlistFiles(const QString &path)
{
    const QDir folder(path);
    // hashcat's scan_directory() skips names starting with '.' on every platform,
    // but QDir only treats those as hidden on Unix: on Windows a dotfile is an
    // ordinary file, and QDir::Files hides the attribute-hidden ones instead.
    // Ask for both and apply hashcat's rule, so the folder is judged the way the
    // feed will read it.
    const QFileInfoList entries =
        folder.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot);
    for (const QFileInfo &info : entries) {
        if (!info.fileName().startsWith(QLatin1Char('.'))) {
            return true;
        }
    }
    return false;
}
} // namespace

PathMonitor::PathMonitor(Kind kind, QObject *parent) : QObject(parent), m_kind(kind)
{
    connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this, [this] {
        recheck();
        // A directory that was renamed or removed loses its watch; add it
        // again if it is back under the same name.
        updateWatches();
    });
}

void PathMonitor::setPaths(const QStringList &paths)
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

bool PathMonitor::isAvailable(const QString &path) const
{
    return m_available.value(path, false);
}

bool PathMonitor::isUsable(const QString &path) const
{
    const QFileInfo info(path);

    switch (m_kind) {
    case Kind::Wordlist:
        // Since hashcat 8.0.0 a folder is a base wordlist input; it has to hold
        // at least one file, because the feed skips subdirectories and dotfiles.
        return info.isFile() || (info.isDir() && hasWordlistFiles(path));
    case Kind::Table:
        // The table feed opens every argument as a file; it never scans a
        // folder, so a folder is unusable whatever it contains.
        return info.isFile();
    case Kind::Pcfg:
        if (info.isFile()) {
            // A single archive is one of the two ruleset forms.
            return true;
        }
        if (info.isDir()) {
            // The other form: a folder named by its grammar.
            return QFileInfo(QDir(path).filePath(QStringLiteral("Grammar/grammar.txt"))).isFile();
        }
        return isBundledName(path);
    }

    return false;
}

void PathMonitor::recheck()
{
    for (const QString &path : std::as_const(m_paths)) {
        const bool available = isUsable(path);
        if (m_available.contains(path) && m_available.value(path) == available) {
            continue;
        }

        m_available.insert(path, available);
        emit availabilityChanged(path, available);
    }
}

QStringList PathMonitor::watchDirectories(const QString &path) const
{
    QStringList directories;

    // The parent is gone: watch the nearest existing ancestor, so that a
    // recreated directory produces an event and the next update can follow it
    // down again. The walk stops at the root - and at a drive or share that is
    // not available at all.
    QString directory = QFileInfo(path).absolutePath();
    while (!QFileInfo(directory).isDir()) {
        const QString parent = QFileInfo(directory).absolutePath();
        if (parent.isEmpty() || parent == directory) {
            directory.clear();
            break;
        }
        directory = parent;
    }
    if (!directory.isEmpty()) {
        directories << directory;
    }

    // The entries of a folder decide whether it is usable: an empty wordlist
    // folder becomes usable when a file arrives, and a ruleset folder becomes
    // usable when its grammar arrives. A table folder is unusable whatever is
    // inside it, so its parent is enough there.
    const QFileInfo info(path);
    if (info.isDir() && m_kind != Kind::Table) {
        const QString folder = info.absoluteFilePath();
        if (!directories.contains(folder)) {
            directories << folder;
        }
        if (m_kind == Kind::Pcfg) {
            // An existing Grammar/ can gain its grammar.txt without the ruleset
            // folder itself changing; a missing one arrives as an entry of the
            // folder watch above.
            const QString grammar = QDir(folder).filePath(QStringLiteral("Grammar"));
            if (QFileInfo(grammar).isDir() && !directories.contains(grammar)) {
                directories << grammar;
            }
        }
    }

    return directories;
}

void PathMonitor::updateWatches()
{
    QStringList directories;
    for (const QString &path : std::as_const(m_paths)) {
        const QStringList candidates = watchDirectories(path);
        for (const QString &directory : candidates) {
            if (!directories.contains(directory)) {
                directories << directory;
            }
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
