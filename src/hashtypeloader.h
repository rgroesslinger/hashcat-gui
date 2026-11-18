/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef HASHTYPELOADER_H
#define HASHTYPELOADER_H

#include "hashcatrunner.h"

#include <QFutureWatcher>
#include <QMap>
#include <QObject>
#include <QString>
#include <cstdint>
#include <functional>
#include <memory>

// Why a hash-type query failed. There is no "not configured": an empty path
// starts no query at all. The caller maps each kind to its own message.
struct HashTypeFailure {
    enum class Kind : std::uint8_t { ProcessFailed, Timeout, Unparseable };
    Kind kind = Kind::ProcessFailed;
    // stderr, or the parser's error - display-only.
    QString detail;
};

Q_DECLARE_METATYPE(HashTypeFailure)

// The asynchronous --example-hashes query: one watcher, one generation counter,
// and the parsed list or failure as a signal. The path is read on the calling
// thread. A superseded query is cancelled - its process killed, not just its
// reply dropped by the generation guard.
class HashTypeLoader : public QObject
{
    Q_OBJECT

public:
    // The query is injectable so the tests do not wait for a real process.
    // It receives the token that cancels its process when it is superseded.
    using Query = std::function<QFuture<HashcatResult>(const QString &, const QStringList &, int,
                                                       HashcatRunner::CancelToken)>;

    explicit HashTypeLoader(QObject *parent, Query query = HashcatRunner::execute);
    ~HashTypeLoader() override;

    HashTypeLoader(const HashTypeLoader &) = delete;
    HashTypeLoader &operator=(const HashTypeLoader &) = delete;
    HashTypeLoader(HashTypeLoader &&) = delete;
    HashTypeLoader &operator=(HashTypeLoader &&) = delete;

    // Starts the query for the given binary; an empty path starts none and
    // ends any loading state. Remembers the path as the one queried.
    void reload(const QString &hashcatPath);

    [[nodiscard]] QString queriedPath() const
    {
        return m_queriedHashcatPath;
    }
    [[nodiscard]] bool isLoading() const
    {
        return m_loading;
    }

signals:
    void loadingChanged(bool loading);
    void loaded(const QMap<quint32, QString> &hashTypes);
    void failed(const HashTypeFailure &failure);

private:
    void setLoading(bool loading);

    Query m_query;
    QString m_queriedHashcatPath;
    std::unique_ptr<QFutureWatcher<HashcatResult>> m_watcher;
    HashcatRunner::CancelToken m_cancel;
    quint64 m_generation = 0;
    bool m_loading = false;
};

#endif // HASHTYPELOADER_H