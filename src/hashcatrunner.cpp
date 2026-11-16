/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "hashcatrunner.h"

#include "programdirectory.h"

#include <QtConcurrent/QtConcurrentRun>

QFuture<HashcatResult> HashcatRunner::execute(const QString &hashcatPath, const QStringList &args,
                                              int timeoutMs)
{
    return QtConcurrent::run([hashcatPath, args, timeoutMs]() -> HashcatResult {
        HashcatResult result;
        QProcess proc;
        QStringList cmdArgs = args;

        if (hashcatPath.isEmpty()) {
            result.standardError = tr("The hashcat path is not configured.");
            return result;
        }

        // Always run in quiet mode when reading output
        cmdArgs << AppConstants::Hashcat::Quiet;

        proc.setProgram(hashcatPath);
        proc.setArguments(cmdArgs);
        proc.setWorkingDirectory(ProgramDirectory::forProgram(hashcatPath));

        proc.start();

        if (!proc.waitForStarted()) {
            result.standardError = tr("Failed to start hashcat\n%1").arg(proc.errorString());
            return result;
        }

        if (!proc.waitForFinished(timeoutMs)) {
            // A crash is reported by waitForFinished() itself; a false return
            // here is a timeout or a process error, so name the actual cause.
            const bool timedOut = proc.error() == QProcess::Timedout;
            proc.kill();
            // kill() is asynchronous: the wait blocks until the process has
            // actually exited, so the destructor of proc is not left with a
            // process that is still running.
            proc.waitForFinished(5000);
            result.timedOut = timedOut;
            result.standardError = timedOut
                                       ? tr("hashcat timed out\n%1").arg(proc.errorString())
                                       : tr("hashcat failed to run\n%1").arg(proc.errorString());
            return result;
        }

        result.exitStatus = proc.exitStatus();
        result.exitCode = proc.exitCode();
        // QProcess hands back raw bytes; hashcat speaks UTF-8, so decode them
        // as such rather than through the conversion this project compiles without.
        result.standardOutput = QString::fromUtf8(proc.readAllStandardOutput());
        result.standardError = QString::fromUtf8(proc.readAllStandardError());

        return result;
    });
}