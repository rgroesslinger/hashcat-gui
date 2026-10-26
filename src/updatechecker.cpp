/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "updatechecker.h"
#include "appconstants.h"

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QVersionNumber>

namespace
{
// Normalizes a release tag such as "v0.8.0" into a comparable version.
//
// A suffix makes the tag ambiguous - "0.9.0-rc1" is not the same release as
// "0.9.0" - so only a complete numeric version is accepted.
QString parseVersion(const QString &text, QVersionNumber *version)
{
    QString tag = text.trimmed();
    if (tag.startsWith(QLatin1Char('v'), Qt::CaseInsensitive)) {
        tag = tag.mid(1);
    }

    qsizetype suffixIndex = 0;
    const QVersionNumber parsed = QVersionNumber::fromString(tag, &suffixIndex);
    if (parsed.isNull() || suffixIndex != tag.size()) {
        return {};
    }

    if (version) {
        *version = parsed;
    }
    return tag;
}
} // namespace

UpdateChecker::UpdateChecker(QObject *parent)
    : QObject(parent), m_manager(new QNetworkAccessManager(this)),
      m_endpoint(QUrl(QString(AppConstants::Urls::LatestReleaseApi)))
{
}

UpdateChecker::UpdateChecker(QNetworkAccessManager *manager, const QUrl &endpoint, QObject *parent)
    : QObject(parent), m_manager(manager), m_endpoint(endpoint)
{
    if (!m_manager) {
        m_manager = new QNetworkAccessManager(this);
    }
}

void UpdateChecker::check()
{
    if (m_reply) {
        return;
    }

    QNetworkRequest request(m_endpoint);
    request.setHeader(QNetworkRequest::UserAgentHeader,
                      QStringLiteral("hashcat-gui/%1").arg(QCoreApplication::applicationVersion()));
    request.setRawHeader(QByteArrayLiteral("Accept"),
                         QByteArrayLiteral("application/vnd.github+json"));
    request.setRawHeader(QByteArrayLiteral("X-GitHub-Api-Version"),
                         QByteArrayLiteral("2022-11-28"));
    request.setTransferTimeout(AppConstants::UpdateCheck::RequestTimeoutMs);

    QNetworkReply *reply = m_manager->get(request);
    m_reply = reply;

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (m_reply == reply) {
            m_reply.clear();
        }

        UpdateCheckResult result;
        const QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

        if (reply->error() != QNetworkReply::NoError) {
            result.detail = statusCode.isValid() ? tr("GitHub answered with HTTP status %1: %2")
                                                       .arg(statusCode.toInt())
                                                       .arg(reply->errorString())
                                                 : reply->errorString();
        } else if (statusCode.isValid() && statusCode.toInt() != 200) {
            result.detail = tr("GitHub answered with HTTP status %1.").arg(statusCode.toInt());
        } else {
            result = evaluateResponse(reply->readAll(), QCoreApplication::applicationVersion());
        }

        reply->deleteLater();
        emit finished(result);
    });
}

UpdateCheckResult UpdateChecker::evaluateResponse(const QByteArray &body,
                                                  const QString &currentVersion)
{
    UpdateCheckResult result;

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(body, &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        result.detail = tr("The release information is not valid JSON.");
        return result;
    }

    const QJsonValue tag = document.object().value(QStringLiteral("tag_name"));
    if (!tag.isString()) {
        result.detail = tr("The release information does not name a version.");
        return result;
    }

    QVersionNumber latest;
    const QString latestVersion = parseVersion(tag.toString(), &latest);
    if (latestVersion.isEmpty()) {
        result.detail =
            tr("The release version %1 is not a valid version number.").arg(tag.toString());
        return result;
    }

    QVersionNumber current;
    if (parseVersion(currentVersion, &current).isEmpty()) {
        result.detail =
            tr("The installed version %1 is not a valid version number.").arg(currentVersion);
        return result;
    }

    result.latestVersion = latestVersion;
    result.status = QVersionNumber::compare(latest, current) > 0
                        ? UpdateCheckResult::Status::UpdateAvailable
                        : UpdateCheckResult::Status::UpToDate;
    return result;
}
