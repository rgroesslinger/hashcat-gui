/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfilecheck.h"

#include "appconstants.h"

#include <QProcess>
#include <QStringList>
#include <QtConcurrent/QtConcurrentRun>

#include <atomic>
#include <utility>

namespace
{

bool isTextOutput(const QString &output)
{
    int suspicious = 0;
    for (const QChar character : output) {
        const ushort code = character.unicode();
        if (code == '\n' || code == '\r' || code == '\t') {
            continue;
        }
        // A NUL is a strong binary signal: the binary hash records are
        // zero-padded structures.
        if (code == 0) {
            return false;
        }
        // A single control or replacement character can still belong to a
        // text hash line. Only a substantial share of the output marks a
        // binary record.
        if (code < 0x20 || code == 0x7f || code == 0xfffd) {
            ++suspicious;
        }
    }
    if (suspicious < 4) {
        return true;
    }
    return suspicious <= output.size() / 8;
}

} // namespace

PotfileCheck::PotfileCheck(QObject *parent, Query query)
    : QObject(parent), m_query(std::move(query))
{
}

PotfileCheck::~PotfileCheck()
{
    resetHashcatQuery(m_cancel, m_showWatcher, m_parseWatcher, m_leftWatcher);
}

void PotfileCheck::start(const QString &hashcatPath, const PotfileQueryInput &input)
{
    // A new check supersedes an in-flight one, the way a reload does for the
    // hash-type list. MainWindow also disables the action while busy.
    resetHashcatQuery(m_cancel, m_showWatcher, m_parseWatcher, m_leftWatcher);

    m_hashcatPath = hashcatPath;
    m_input = input;
    m_result = {};
    m_running = true;
    m_cancel = std::make_shared<std::atomic_bool>(false);

    if (m_hashcatPath.isEmpty()) {
        m_result.status = PotfileCheckResult::Status::ShowFailed;
        m_result.diagnostic = tr("The hashcat path is not configured.");
        finish();
        return;
    }
    if (m_input.hashInput.trimmed().isEmpty()) {
        m_result.status = PotfileCheckResult::Status::ShowFailed;
        m_result.diagnostic = tr("Enter a hash or select a hash file first.");
        finish();
        return;
    }
    // Auto-detect is best-effort: a wrong mode silently miscounts the potfile.
    if (m_input.hashType < 0) {
        m_result.status = PotfileCheckResult::Status::ShowFailed;
        m_result.diagnostic = missingHashTypeMessage();
        finish();
        return;
    }

    startShow();
}

QString PotfileCheck::missingHashTypeMessage()
{
    return tr("Choose a hash type. Potfile checks need an explicit hash type; Auto-detect is "
              "best-effort and can miscount. Use Tools → Identify Hash if you do not know the "
              "type.");
}

void PotfileCheck::cancel()
{
    if (m_cancel) {
        m_cancel->store(true);
    }
}

bool PotfileCheck::isRunning() const
{
    return m_running;
}

void PotfileCheck::startShow()
{
    m_showWatcher = std::make_unique<QFutureWatcher<HashcatResult>>();
    connect(m_showWatcher.get(), &QFutureWatcher<HashcatResult>::finished, this, [this] {
        HashcatResult result = m_showWatcher->result();

        if (result.cancelled) {
            m_result.status = PotfileCheckResult::Status::Cancelled;
            finish();
            return;
        }
        if (result.timedOut || result.exitStatus != QProcess::NormalExit || result.exitCode != 0) {
            failShow(result);
            return;
        }

        startParse(std::move(result));
    });

    m_showWatcher->setFuture(m_query(m_hashcatPath, PotfileQuery::showArgs(m_input),
                                     AppConstants::Hashcat::ShowTimeoutMs, m_cancel));
}

void PotfileCheck::startParse(HashcatResult showResult)
{
    // Parsing a large show output runs off the GUI thread; the cap only limits
    // the retained entries, not the bytes that have to be scanned. Both the
    // failure path and the parser share one copy of the output.
    const auto sharedResult = std::make_shared<const HashcatResult>(std::move(showResult));

    m_parseWatcher = std::make_unique<QFutureWatcher<PotfileShowParser::Result>>();
    connect(m_parseWatcher.get(), &QFutureWatcher<PotfileShowParser::Result>::finished, this,
            [this, sharedResult] {
                const PotfileShowParser::Result parsed = m_parseWatcher->result();

                if ((m_cancel && m_cancel->load()) || parsed.cancelled) {
                    m_result.status = PotfileCheckResult::Status::Cancelled;
                    finish();
                    return;
                }
                if (parsed.totalCount == 0 && parsed.malformedCount > 0) {
                    // No entries and only unrecognized lines is a failed query; known
                    // diagnostics alone are an empty answer.
                    failShow(*sharedResult);
                    return;
                }

                m_result.cracked = parsed.totalCount;
                m_result.totalCount = parsed.totalCount;
                m_result.malformedCount = parsed.malformedCount;
                m_result.entries = parsed.entries;
                startLeft();
            });

    m_parseWatcher->setFuture(QtConcurrent::run([sharedResult, cancel = m_cancel] {
        return PotfileShowParser::parse(sharedResult->standardOutput,
                                        AppConstants::Hashcat::PotfileMaxEntries, cancel);
    }));
}

void PotfileCheck::startLeft()
{
    if (m_cancel && m_cancel->load()) {
        m_result.status = PotfileCheckResult::Status::Cancelled;
        finish();
        return;
    }

    m_leftWatcher = std::make_unique<QFutureWatcher<HashcatResult>>();
    connect(m_leftWatcher.get(), &QFutureWatcher<HashcatResult>::finished, this, [this] {
        const HashcatResult result = m_leftWatcher->result();

        if (result.cancelled) {
            m_result.status = PotfileCheckResult::Status::Cancelled;
            finish();
            return;
        }
        if (result.timedOut || result.exitStatus != QProcess::NormalExit || result.exitCode != 0) {
            m_result.status = PotfileCheckResult::Status::LeftUnknown;
            m_result.left = -1;
            m_result.diagnostic = combinedOutput(result);
            finish();
            return;
        }

        if (!isTextOutput(result.standardOutput)) {
            // Some modes write their remaining hashes as binary records (no
            // EOL); a line count would not be a hash count.
            m_result.status = PotfileCheckResult::Status::LeftUnknown;
            m_result.left = -1;
            m_result.diagnostic = tr("The remaining hashes are binary data; the count is unknown.");
            finish();
            return;
        }

        m_result.left = countLines(result.standardOutput);
        m_result.status = PotfileCheckResult::Status::Ok;
        finish();
    });

    // Each query gets the full deadline; a slow --show must not eat --left's.
    m_leftWatcher->setFuture(m_query(m_hashcatPath, PotfileQuery::leftArgs(m_input),
                                     AppConstants::Hashcat::LeftTimeoutMs, m_cancel));
}

void PotfileCheck::finish()
{
    m_running = false;
    emit finished(m_result);
}

void PotfileCheck::failShow(const HashcatResult &result)
{
    m_result.status = PotfileCheckResult::Status::ShowFailed;

    const QString text = combinedOutput(result);
    if (text.contains(QStringLiteral("Please specify the hash-mode"))
        || text.contains(QStringLiteral("hash-modes match the structure of your input hash"))) {
        m_result.diagnostic = tr("Choose a hash type.");
    } else if (text.contains(QStringLiteral("No hashes loaded"))) {
        m_result.diagnostic = tr("hashcat could not load any hashes from the input.");
    } else if (result.exitCode == 255 && result.standardOutput.trimmed().isEmpty()
               && result.standardError.trimmed().isEmpty()) {
        m_result.diagnostic = tr("hashcat could not parse the input for the selected hash type. "
                                 "Check the hash and the hash type.");
    } else {
        m_result.diagnostic = text;
    }
    finish();
}

QString PotfileCheck::combinedOutput(const HashcatResult &result) const
{
    const QString text = result.standardOutput + QStringLiteral("\n") + result.standardError;
    return text.trimmed().isEmpty() ? tr("hashcat could not query the potfile.") : text;
}

int PotfileCheck::countLines(const QString &output)
{
    int count = 0;
    bool hasContent = false;
    for (int i = PotfileShowParser::payloadOffset(output); i < output.size(); ++i) {
        const QChar character = output.at(i);
        if (character == QLatin1Char('\n')) {
            if (hasContent) {
                ++count;
            }
            hasContent = false;
        } else if (!character.isSpace()) {
            hasContent = true;
        }
    }
    if (hasContent) {
        ++count;
    }
    return count;
}