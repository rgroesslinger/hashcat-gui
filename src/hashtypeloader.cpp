/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "hashtypeloader.h"

#include "appconstants.h"
#include "hashcatinfoparser.h"

HashTypeLoader::HashTypeLoader(QObject *parent, Query query)
    : QObject(parent), m_query(std::move(query))
{
}

void HashTypeLoader::reload(const QString &hashcatPath)
{
    m_queriedHashcatPath = hashcatPath;
    ++m_generation;

    if (hashcatPath.isEmpty()) {
        // No query: an earlier one may still be on its way, but its reply
        // belongs to a path this list no longer comes from.
        m_watcher.reset();
        setLoading(false);
        return;
    }

    setLoading(true);
    m_watcher = std::make_unique<QFutureWatcher<HashcatResult>>();
    const quint64 generation = m_generation;
    connect(m_watcher.get(), &QFutureWatcher<HashcatResult>::finished, this, [this, generation] {
        if (generation != m_generation) {
            return;
        }
        const HashcatResult result = m_watcher->result();
        setLoading(false);

        if (result.exitStatus != QProcess::NormalExit || result.exitCode != 0) {
            HashTypeFailure failure;
            failure.kind = result.timedOut ? HashTypeFailure::Kind::Timeout
                                           : HashTypeFailure::Kind::ProcessFailed;
            failure.detail = result.standardError;
            emit failed(failure);
            return;
        }

        QMap<quint32, QString> parsed;
        QString error;
        if (!HashcatInfoParser::parseExampleHashes(result.standardOutput, parsed, &error)) {
            emit failed({HashTypeFailure::Kind::Unparseable, error});
            return;
        }
        emit loaded(parsed);
    });
    m_watcher->setFuture(m_query(hashcatPath,
                                 QStringList() << AppConstants::Hashcat::ExampleHashes
                                               << AppConstants::Hashcat::MachineReadable,
                                 AppConstants::Hashcat::QueryTimeoutMs));
}

void HashTypeLoader::setLoading(bool loading)
{
    if (m_loading == loading) {
        return;
    }
    m_loading = loading;
    emit loadingChanged(m_loading);
}