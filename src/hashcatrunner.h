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
    static QFuture<HashcatResult> execute(const QString &hashcatPath, const QStringList &args,
                                          int timeoutMs = AppConstants::Hashcat::QueryTimeoutMs);
};

#endif // HASHCATRUNNER_H