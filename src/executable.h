/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef EXECUTABLE_H
#define EXECUTABLE_H

#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QString>

// Turns a configured program into the absolute path a process is started
// from. A bare name is looked up on PATH; a path with a directory part is
// made absolute and checked. The settings keep what the user typed - this is
// the one place that decides whether it can start anything.
namespace Executable
{
inline QString resolve(const QString &configured)
{
    if (configured.isEmpty()) {
        return {};
    }

#ifdef Q_OS_WIN
    const bool hasSeparator =
        configured.contains(QLatin1Char('/')) || configured.contains(QLatin1Char('\\'));
#else
    const bool hasSeparator = configured.contains(QLatin1Char('/'));
#endif

    if (!hasSeparator) {
        // A macOS bundle found on PATH comes back as its bare bundle name;
        // anything that is not an absolute program is not usable here.
        const QString found = QStandardPaths::findExecutable(configured);
        return QDir::isAbsolutePath(found) ? found : QString();
    }

    const QFileInfo file(QFileInfo(configured).absoluteFilePath());
    // Windows has no permission bit; Qt reports the known program suffixes
    // (and directories) there, which is the closest thing it has.
    if (!file.isFile() || !file.isExecutable()) {
        return {};
    }
    return file.absoluteFilePath();
}
} // namespace Executable

#endif // EXECUTABLE_H
