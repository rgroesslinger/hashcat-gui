/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef POTFILECHECK_H
#define POTFILECHECK_H

#include "hashcatrunner.h"
#include "potfilequery.h"
#include "potfileshowparser.h"

#include <QFutureWatcher>
#include <QMetaType>
#include <QObject>
#include <QString>

#include <cstdint>
#include <memory>

struct PotfileCheckResult {
    enum class Status : std::uint8_t { Ok, Cancelled, ShowFailed, LeftUnknown };

    Status status = Status::ShowFailed;
    int cracked = 0;
    // -1 when the left query did not answer; the show count is still valid.
    int left = -1;
    PotfileShowParser::Entries entries; // capped at PotfileMaxEntries
    int totalCount = 0;                 // valid show entries before the cap
    int malformedCount = 0;             // unknown non-JSON lines, diagnostics excluded
    QString diagnostic;                 // combined streams, timeout, or the mode message
};

Q_DECLARE_METATYPE(PotfileCheckResult)

// Runs `--show` then `--left` against one hash input. The query is injectable
// so the service can be tested without hashcat; MainWindow owns one instance.
class PotfileCheck : public QObject
{
    Q_OBJECT

public:
    using Query = HashcatRunner::Query;

    explicit PotfileCheck(QObject *parent, Query query = HashcatRunner::execute);
    ~PotfileCheck() override;

    PotfileCheck(const PotfileCheck &) = delete;
    PotfileCheck &operator=(const PotfileCheck &) = delete;
    PotfileCheck(PotfileCheck &&) = delete;
    PotfileCheck &operator=(PotfileCheck &&) = delete;

    void start(const QString &hashcatPath, const PotfileQueryInput &input);
    void cancel();
    [[nodiscard]] bool isRunning() const;

    // Shared with MainWindow's preflight so the two cannot diverge.
    static QString missingHashTypeMessage();

signals:
    void finished(const PotfileCheckResult &result);

private:
    void startShow();
    void startParse(HashcatResult showResult);
    void startLeft();
    void finish();
    void failShow(const HashcatResult &result);
    [[nodiscard]] QString combinedOutput(const HashcatResult &result) const;
    static int countLines(const QString &output);

    Query m_query;
    PotfileQueryInput m_input;
    QString m_hashcatPath;
    std::unique_ptr<QFutureWatcher<HashcatResult>> m_showWatcher;
    std::unique_ptr<QFutureWatcher<PotfileShowParser::Result>> m_parseWatcher;
    std::unique_ptr<QFutureWatcher<HashcatResult>> m_leftWatcher;
    HashcatRunner::CancelToken m_cancel;
    PotfileCheckResult m_result;
    bool m_running = false;
};

#endif // POTFILECHECK_H