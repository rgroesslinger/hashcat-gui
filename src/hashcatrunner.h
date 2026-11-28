/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef HASHCATRUNNER_H
#define HASHCATRUNNER_H

#include "appconstants.h"

#include <QCoreApplication>
#include <QFuture>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <atomic>
#include <functional>
#include <memory>

// Result of a single hashcat invocation.
//
// The status members default to "no process ran", so an early return (missing
// configuration, process never started, timeout) cannot look like a successful
// run to the consumers that check exitStatus/exitCode first.
struct HashcatResult {
    QProcess::ExitStatus exitStatus = QProcess::CrashExit;
    int exitCode = -1;
    // The query hit its deadline. The status members alone cannot tell that
    // from a crash: the process was killed to end the wait.
    bool timedOut = false;
    // A superseded query: the process was killed before it answered.
    bool cancelled = false;
    QString standardOutput;
    QString standardError;
};

// Runs hashcat asynchronously. The caller attaches a QFutureWatcher:
//   auto *w = new QFutureWatcher<HashcatResult>(this);
//   connect(w, &QFutureWatcher<HashcatResult>::finished, this, ...);
//   w->setFuture(HashcatRunner::execute(path, {"--help"}));
// The path is read on the calling thread; the worker runs on the thread pool.
class HashcatRunner
{
    Q_DECLARE_TR_FUNCTIONS(HashcatRunner)

public:
    // Shared between the caller and the worker: setting it kills the running
    // process and ends the query early. QFuture::cancel() alone stops neither
    // the task nor its process.
    using CancelToken = std::shared_ptr<std::atomic_bool>;

    // The injectable query shape shared by the asynchronous callers.
    using Query = std::function<QFuture<HashcatResult>(const QString &, const QStringList &, int,
                                                       CancelToken)>;

    // Unique per query: hashcat's pidfile is per session.
    static QString uniqueSession(QLatin1StringView prefix);

    static QFuture<HashcatResult> execute(const QString &hashcatPath, const QStringList &args,
                                          int timeoutMs = AppConstants::Hashcat::QueryTimeoutMs,
                                          CancelToken cancel = {});
};

// Cancels and clears the token, then releases the watchers it guarded.
template <typename... Watchers>
void resetHashcatQuery(HashcatRunner::CancelToken &cancel, Watchers &...watchers)
{
    if (cancel) {
        cancel->store(true);
    }
    cancel.reset();
    (watchers.reset(), ...);
}

#endif // HASHCATRUNNER_H
