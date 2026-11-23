/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfilequery.h"

#include "programdirectory.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

#include <atomic>

namespace
{

// "C:hashes.txt" has a drive but no root. QDir::isAbsolutePath() calls it
// relative, and joining it onto another directory would corrupt it.
bool isDriveRelativePath(const QString &path)
{
#ifdef Q_OS_WIN
    return path.size() >= 2 && path.at(1) == QLatin1Char(':') && !QDir::isAbsolutePath(path);
#else
    Q_UNUSED(path);
    return false;
#endif
}

QStringList queryArgs(QLatin1StringView action, const PotfileQueryInput &input, bool json)
{
    QStringList args;
    args << action << AppConstants::Hashcat::LogfileDisable
         << QStringLiteral("--session=%1").arg(PotfileQuery::uniqueSession());

    if (json) {
        args << AppConstants::Hashcat::Json;
    }
    if (input.hashType >= 0) {
        args << AppConstants::Hashcat::HashTypeShort << QString::number(input.hashType);
    }
    if (!input.potfilePath.isEmpty()) {
        args << AppConstants::Hashcat::PotfilePath << input.potfilePath;
    }
    if (input.username) {
        args << AppConstants::Hashcat::Username;
    }
    if (input.hashInput.startsWith(QLatin1Char('-'))) {
        args << AppConstants::Hashcat::OptionTerminator;
    }
    args << input.hashInput;
    return args;
}

} // namespace

QString PotfileQuery::uniqueSession()
{
    // A session name identifies one query. Concurrent queries must not share
    // it: hashcat's transient pidfile would make the second one fail.
    static std::atomic<quint64> counter{0};
    return QStringLiteral("%1%2-%3").arg(
        QString(AppConstants::Hashcat::PotfileSessionPrefix),
        QString::number(QCoreApplication::applicationPid()),
        QString::number(counter.fetch_add(1, std::memory_order_relaxed) + 1));
}

QStringList PotfileQuery::showArgs(const PotfileQueryInput &input)
{
    return queryArgs(AppConstants::Hashcat::Show, input, true);
}

QStringList PotfileQuery::leftArgs(const PotfileQueryInput &input)
{
    return queryArgs(AppConstants::Hashcat::Left, input, false);
}

QString PotfileQuery::resolvePath(const QString &entered, const QString &hashcatPath)
{
    if (entered.isEmpty() || QDir::isAbsolutePath(entered)) {
        return entered;
    }

    if (isDriveRelativePath(entered)) {
        // Let Windows resolve its per-drive current directory; the old
        // QFileInfo() conversion did the same.
        return QFileInfo(entered).absoluteFilePath();
    }

    if (!hashcatPath.isEmpty()) {
        const QString base = ProgramDirectory::forProgram(hashcatPath);
        if (!base.isEmpty()) {
            return QDir::cleanPath(QDir(base).filePath(entered));
        }
    }

    return QFileInfo(entered).absoluteFilePath();
}

QString PotfileQuery::resolveHashInput(const QString &entered, const QString &hashcatPath)
{
    // Only an existing path is a path: bcrypt hashes contain '/' too. The
    // resolution matches the working directory a launch gives hashcat.
    const QString resolved = resolvePath(entered, hashcatPath);
    return QFileInfo::exists(resolved) ? resolved : entered;
}