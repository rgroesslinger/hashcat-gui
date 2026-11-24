/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "hashcatrunner.h"

#include "programdirectory.h"

#include <QElapsedTimer>
#include <QtConcurrent/QtConcurrentRun>
#include <algorithm>

QString HashcatRunner::uniqueSession(QLatin1StringView prefix)
{
    static std::atomic<quint64> counter{0};
    return QStringLiteral("%1%2-%3").arg(
        QString(prefix), QString::number(QCoreApplication::applicationPid()),
        QString::number(counter.fetch_add(1, std::memory_order_relaxed) + 1));
}

QFuture<HashcatResult> HashcatRunner::execute(const QString &hashcatPath, const QStringList &args,
                                              int timeoutMs, CancelToken cancel)
{
    return QtConcurrent::run([hashcatPath, args, timeoutMs, cancel]() -> HashcatResult {
        HashcatResult result;
        QProcess proc;
        QStringList cmdArgs = args;

        if (hashcatPath.isEmpty()) {
            result.standardError = tr("The hashcat path is not configured.");
            return result;
        }

        const auto cancelled = [&cancel] { return cancel && cancel->load(); };
        // kill() is asynchronous: the wait blocks until the process has
        // actually exited, so the destructor of proc is not left with a
        // process that is still running.
        const auto stop = [&proc] {
            proc.kill();
            proc.waitForFinished(5000);
        };

        if (cancelled()) {
            result.cancelled = true;
            return result;
        }

        // Insert before an existing '--': after it, hashcat reads --quiet as
        // a positional. Current callers use '--' only as a terminator.
        const QString terminator(AppConstants::Hashcat::OptionTerminator);
        const auto endOfOptions = std::ranges::find(cmdArgs, terminator);
        if (endOfOptions == cmdArgs.cend()) {
            cmdArgs << AppConstants::Hashcat::Quiet;
        } else {
            cmdArgs.insert(endOfOptions, AppConstants::Hashcat::Quiet);
        }

        proc.setProgram(hashcatPath);
        proc.setArguments(cmdArgs);
        proc.setWorkingDirectory(ProgramDirectory::forProgram(hashcatPath));

        proc.start();

        if (!proc.waitForStarted()) {
            result.standardError = tr("Failed to start hashcat\n%1").arg(proc.errorString());
            return result;
        }

        // The wait runs in short chunks so a cancelled query kills its
        // process instead of running on to the timeout.
        constexpr int pollMs = 100;
        QElapsedTimer deadline;
        deadline.start();
        for (;;) {
            if (cancelled()) {
                stop();
                result.cancelled = true;
                return result;
            }
            const qint64 remaining = timeoutMs - deadline.elapsed();
            if (remaining <= 0) {
                stop();
                result.timedOut = true;
                result.standardError = tr("hashcat timed out\n%1").arg(proc.errorString());
                return result;
            }
            if (proc.waitForFinished(static_cast<int>(std::min<qint64>(remaining, pollMs)))) {
                break;
            }
            // A crash is reported by waitForFinished() itself; a false return
            // with another error is a process error, so name the actual cause.
            if (proc.error() != QProcess::Timedout) {
                stop();
                result.standardError = tr("hashcat failed to run\n%1").arg(proc.errorString());
                return result;
            }
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