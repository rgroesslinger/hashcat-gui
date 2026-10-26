/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QByteArray>
#include <QMetaType>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QUrl>

class QNetworkAccessManager;
class QNetworkReply;

// What a release check found.
//
// The status is the part the dialog reacts to; the version is normalized
// (without the leading "v" of the release tag), and the detail is the raw
// reason a check failed.
struct UpdateCheckResult {
    enum class Status : quint8 {
        UpdateAvailable,
        UpToDate,
        Failed,
    };

    Status status = Status::Failed;
    QString latestVersion;
    QString detail;
};

Q_DECLARE_METATYPE(UpdateCheckResult)

// Asks GitHub for the latest release and compares it with the running version.
//
// The request is asynchronous: check() returns immediately and finished()
// carries the answer. The manager can be replaced for tests; the default
// manager belongs to the checker.
class UpdateChecker : public QObject
{
    Q_OBJECT

public:
    explicit UpdateChecker(QObject *parent = nullptr);
    UpdateChecker(QNetworkAccessManager *manager, const QUrl &endpoint, QObject *parent = nullptr);
    ~UpdateChecker() override = default;

    // Starts one request. A second call while a request is in flight is
    // ignored.
    virtual void check();

    // Turns a GitHub release response into a result. Public so the parsing
    // rules can be tested without a network.
    static UpdateCheckResult evaluateResponse(const QByteArray &body,
                                              const QString &currentVersion);

signals:
    void finished(const UpdateCheckResult &result);

private:
    QNetworkAccessManager *m_manager = nullptr;
    QUrl m_endpoint;
    QPointer<QNetworkReply> m_reply;
};

#endif // UPDATECHECKER_H
