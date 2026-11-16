/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef PROGRAMDIRECTORY_H
#define PROGRAMDIRECTORY_H

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

#endif // PROGRAMDIRECTORY_H
