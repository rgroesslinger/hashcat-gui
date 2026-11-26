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

HashTypeLoader::~HashTypeLoader()
{
    // A query still on its way must not keep hashcat running after the GUI
    // is gone; the thread pool would wait for it at exit.
    if (m_cancel) {
        m_cancel->store(true);
    }
}

void HashTypeLoader::reload(const QString &hashcatPath)
{
    m_queriedHashcatPath = hashcatPath;
    ++m_generation;

    // Stop the earlier query's process, not just its reply: left running, it
    // would hold a hashcat --example-hashes until its timeout, and several
    // reconfigurations in a row would stack them up.
    if (m_cancel) {
        m_cancel->store(true);
    }
    m_cancel.reset();
    m_watcher.reset();

    if (hashcatPath.isEmpty()) {
        // No query: the list no longer comes from any path.
        setLoading(false);
        return;
    }

    setLoading(true);
    m_cancel = std::make_shared<std::atomic_bool>(false);
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
    // Own session: probes would otherwise collide on hashcat's pidfile.
    QStringList args;
    args << AppConstants::Hashcat::ExampleHashes << AppConstants::Hashcat::MachineReadable
         << AppConstants::Hashcat::LogfileDisable
         << QStringLiteral("--session=%1")
                .arg(HashcatRunner::uniqueSession(AppConstants::Hashcat::HashTypeSessionPrefix));
    m_watcher->setFuture(
        m_query(hashcatPath, args, AppConstants::Hashcat::QueryTimeoutMs, m_cancel));
}

void HashTypeLoader::setLoading(bool loading)
{
    if (m_loading == loading) {
        return;
    }
    m_loading = loading;
    emit loadingChanged(m_loading);
}