/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef HASHCATQUERYTESTHELPERS_H
#define HASHCATQUERYTESTHELPERS_H

#include "hashcatrunner.h"

#include <QFile>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QTemporaryDir>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>

// The injectable query shape shared by the read-only hashcat dialogs.
using HashcatQuery = HashcatRunner::Query;

// A file Executable::resolve() accepts on both platforms. Nothing runs it.
inline QString fakeProgram(QTemporaryDir &dir)
{
    QString name = QStringLiteral("hashcat");
#ifdef Q_OS_WIN
    name += QStringLiteral(".exe");
#endif
    const QString path = dir.filePath(name);
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        file.write("stub");
        file.close();
    }
    file.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);
    return path;
}

inline HashcatResult normalResult(const QString &output = QString(), int exitCode = 0)
{
    HashcatResult result;
    result.exitStatus = QProcess::NormalExit;
    result.exitCode = exitCode;
    result.standardOutput = output;
    return result;
}

inline HashcatResult failedResult(int exitCode, const QString &output, const QString &error)
{
    HashcatResult result = normalResult(output, exitCode);
    result.standardError = error;
    return result;
}

inline HashcatResult timedOutResult(const QString &error = QString())
{
    HashcatResult result;
    result.timedOut = true;
    result.standardError = error;
    return result;
}

// Answers with the given result after delayMs, recording the argv and token
// when asked. A cancelled query returns cancelled=true like the real runner,
// so the dialog's finished handler takes the cancellation path.
inline HashcatQuery cannedQuery(const HashcatResult &result, QStringList *args = nullptr,
                                HashcatRunner::CancelToken *token = nullptr, int delayMs = 0)
{
    return [result, args, token, delayMs](const QString &, const QStringList &usedArgs, int,
                                          const HashcatRunner::CancelToken &usedToken) {
        if (args) {
            *args = usedArgs;
        }
        if (token) {
            *token = usedToken;
        }
        return QtConcurrent::run([result, delayMs, usedToken] {
            if (delayMs > 0) {
                QThread::msleep(delayMs);
            }
            HashcatResult answer = result;
            if (usedToken && usedToken->load()) {
                answer.cancelled = true;
            }
            return answer;
        });
    };
}

#endif // HASHCATQUERYTESTHELPERS_H
