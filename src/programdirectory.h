/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef PROGRAMDIRECTORY_H
#define PROGRAMDIRECTORY_H

#include <QDir>
#include <QFileInfo>
#include <QString>

// The directory hashcat runs in: the canonical directory of its program, so a
// symlink in ~/bin does not send it looking for its kernels in ~/bin. The
// program path itself stays the resolved one; only the directory follows the
// link. Both the asynchronous --example-hashes query and the detached launch
// use this helper so they cannot disagree.
namespace ProgramDirectory
{
inline QString forProgram(const QString &programPath)
{
    // canonicalFilePath() follows the symlink to the real file; an
    // unresolvable path (a test path, or one that vanished) keeps the plain
    // absolute directory instead of Qt's "." fallback.
    const QString canonical = QFileInfo(programPath).canonicalFilePath();
    if (canonical.isEmpty()) {
        return QFileInfo(programPath).absolutePath();
    }
    return QFileInfo(canonical).absolutePath();
}
} // namespace ProgramDirectory

// How hashcat reads a path: relative to the program directory above.
// Absolute and Windows drive-relative paths are left alone.
namespace ProgramPath
{
inline QString forHashcat(const QString &path, const QString &hashcatPath)
{
    if (path.isEmpty() || QDir::isAbsolutePath(path)) {
        return path;
    }
#ifdef Q_OS_WIN
    if (path.size() >= 2 && path.at(1) == QLatin1Char(':') && !QDir::isAbsolutePath(path)) {
        return QFileInfo(path).absoluteFilePath();
    }
#endif
    if (!hashcatPath.isEmpty()) {
        const QString base = ProgramDirectory::forProgram(hashcatPath);
        if (!base.isEmpty()) {
            return QDir::cleanPath(QDir(base).filePath(path));
        }
    }
    return QFileInfo(path).absoluteFilePath();
}
} // namespace ProgramPath

#endif // PROGRAMDIRECTORY_H
