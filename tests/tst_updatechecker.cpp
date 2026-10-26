/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QHostAddress>
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QSignalSpy>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUrl>

#include "updatechecker.h"

namespace
{
QByteArray releaseJson(const QByteArray &tag)
{
    return QByteArrayLiteral("{\"tag_name\":\"") + tag + QByteArrayLiteral("\"}");
}

QByteArray httpResponse(const QByteArray &body, int status = 200)
{
    const QByteArray reason =
        status == 200 ? QByteArrayLiteral("OK") : QByteArrayLiteral("Forbidden");
    return QByteArrayLiteral("HTTP/1.1 ") + QByteArray::number(status) + ' ' + reason
           + QByteArrayLiteral("\r\nContent-Type: application/json\r\nContent-Length: ")
           + QByteArray::number(body.size()) + QByteArrayLiteral("\r\nConnection: close\r\n\r\n")
           + body;
}

// One request and one canned response, so the checker talks to a real socket
// without touching the network.
class HttpServer : public QTcpServer
{
public:
    explicit HttpServer(const QByteArray &response, QObject *parent = nullptr)
        : QTcpServer(parent), m_response(response)
    {
        connect(this, &QTcpServer::newConnection, this, [this]() {
            QTcpSocket *socket = nextPendingConnection();
            connect(socket, &QTcpSocket::readyRead, socket, [this, socket]() {
                m_request += socket->readAll();
                if (m_sent || !m_request.contains(QByteArrayLiteral("\r\n\r\n"))) {
                    return;
                }
                m_sent = true;
                socket->write(m_response);
                socket->disconnectFromHost();
            });
            connect(socket, &QTcpSocket::disconnected, socket, &QObject::deleteLater);
        });
    }

    QByteArray request() const { return m_request; }

    QUrl url(const QString &path = QStringLiteral("/latest")) const
    {
        return QUrl(QStringLiteral("http://127.0.0.1:%1%2").arg(serverPort()).arg(path));
    }

private:
    QByteArray m_response;
    QByteArray m_request;
    bool m_sent = false;
};
} // namespace

class TestUpdateChecker : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void updateAvailableWhenLatestIsNewer();
    void upToDateWhenVersionsMatch();
    void comparesVersionsNumerically();
    void acceptsDifferentComponentCounts();
    void stripsThePrefixAndWhitespace();
    void rejectsMissingVersion();
    void rejectsNonStringVersion();
    void rejectsMalformedJson();
    void rejectsInvalidCurrentVersion();
    void rejectsVersionSuffixes();
    void readsTheLatestRelease();
    void reportsHttpError();
    void reportsNetworkError();
};

void TestUpdateChecker::initTestCase()
{
    QCoreApplication::setApplicationVersion(QStringLiteral("0.8.0"));
    qRegisterMetaType<UpdateCheckResult>("UpdateCheckResult");
}

void TestUpdateChecker::updateAvailableWhenLatestIsNewer()
{
    const UpdateCheckResult result =
        UpdateChecker::evaluateResponse(releaseJson("v0.9.0"), QStringLiteral("0.8.0"));

    QCOMPARE(result.status, UpdateCheckResult::Status::UpdateAvailable);
    QCOMPARE(result.latestVersion, QStringLiteral("0.9.0"));
}

void TestUpdateChecker::upToDateWhenVersionsMatch()
{
    const UpdateCheckResult result =
        UpdateChecker::evaluateResponse(releaseJson("v0.8.0"), QStringLiteral("0.8.0"));

    QCOMPARE(result.status, UpdateCheckResult::Status::UpToDate);
    QCOMPARE(result.latestVersion, QStringLiteral("0.8.0"));
}

void TestUpdateChecker::comparesVersionsNumerically()
{
    const UpdateCheckResult result =
        UpdateChecker::evaluateResponse(releaseJson("v0.10.0"), QStringLiteral("0.9.0"));

    QCOMPARE(result.status, UpdateCheckResult::Status::UpdateAvailable);
}

void TestUpdateChecker::acceptsDifferentComponentCounts()
{
    const UpdateCheckResult result =
        UpdateChecker::evaluateResponse(releaseJson("v0.9"), QStringLiteral("0.9.0"));

    QCOMPARE(result.status, UpdateCheckResult::Status::UpToDate);
}

void TestUpdateChecker::stripsThePrefixAndWhitespace()
{
    const UpdateCheckResult result =
        UpdateChecker::evaluateResponse(releaseJson("  V0.8.1  "), QStringLiteral("0.8.0"));

    QCOMPARE(result.status, UpdateCheckResult::Status::UpdateAvailable);
    QCOMPARE(result.latestVersion, QStringLiteral("0.8.1"));
}

void TestUpdateChecker::rejectsMissingVersion()
{
    const UpdateCheckResult result = UpdateChecker::evaluateResponse(
        QByteArrayLiteral("{\"name\":\"0.9.0\"}"), QStringLiteral("0.8.0"));

    QCOMPARE(result.status, UpdateCheckResult::Status::Failed);
    QVERIFY(!result.detail.isEmpty());
}

void TestUpdateChecker::rejectsNonStringVersion()
{
    const UpdateCheckResult result = UpdateChecker::evaluateResponse(
        QByteArrayLiteral("{\"tag_name\":9}"), QStringLiteral("0.8.0"));

    QCOMPARE(result.status, UpdateCheckResult::Status::Failed);
    QVERIFY(!result.detail.isEmpty());
}

void TestUpdateChecker::rejectsMalformedJson()
{
    const UpdateCheckResult result =
        UpdateChecker::evaluateResponse(QByteArrayLiteral("not json"), QStringLiteral("0.8.0"));

    QCOMPARE(result.status, UpdateCheckResult::Status::Failed);
    QVERIFY(!result.detail.isEmpty());
}

void TestUpdateChecker::rejectsInvalidCurrentVersion()
{
    const UpdateCheckResult result =
        UpdateChecker::evaluateResponse(releaseJson("v0.9.0"), QStringLiteral("development"));

    QCOMPARE(result.status, UpdateCheckResult::Status::Failed);
    QVERIFY(!result.detail.isEmpty());
}

void TestUpdateChecker::rejectsVersionSuffixes()
{
    const UpdateCheckResult result =
        UpdateChecker::evaluateResponse(releaseJson("v0.9.0-rc1"), QStringLiteral("0.8.0"));

    QCOMPARE(result.status, UpdateCheckResult::Status::Failed);
    QVERIFY(!result.detail.isEmpty());
}

void TestUpdateChecker::readsTheLatestRelease()
{
    HttpServer server(httpResponse(releaseJson("v0.9.0")));
    QVERIFY(server.listen(QHostAddress::LocalHost));

    QNetworkAccessManager manager;
    manager.setProxy(QNetworkProxy::NoProxy);
    UpdateChecker checker(&manager, server.url());
    QSignalSpy spy(&checker, &UpdateChecker::finished);

    checker.check();
    QVERIFY(spy.wait(5000));

    QCOMPARE(spy.count(), 1);
    const UpdateCheckResult result = spy.at(0).at(0).value<UpdateCheckResult>();
    QCOMPARE(result.status, UpdateCheckResult::Status::UpdateAvailable);
    QCOMPARE(result.latestVersion, QStringLiteral("0.9.0"));

    // The request goes to the endpoint the checker was given and says who
    // asked, which the API requires.
    QVERIFY(server.request().contains(QByteArrayLiteral("GET /latest HTTP/1.1")));
    QVERIFY(server.request().contains(QByteArrayLiteral("User-Agent: hashcat-gui/0.8.0")));
}

void TestUpdateChecker::reportsHttpError()
{
    HttpServer server(httpResponse(QByteArrayLiteral("{}"), 403));
    QVERIFY(server.listen(QHostAddress::LocalHost));

    QNetworkAccessManager manager;
    manager.setProxy(QNetworkProxy::NoProxy);
    UpdateChecker checker(&manager, server.url());
    QSignalSpy spy(&checker, &UpdateChecker::finished);

    checker.check();
    QVERIFY(spy.wait(5000));

    QCOMPARE(spy.count(), 1);
    const UpdateCheckResult result = spy.at(0).at(0).value<UpdateCheckResult>();
    QCOMPARE(result.status, UpdateCheckResult::Status::Failed);
    QVERIFY(result.detail.contains(QStringLiteral("403")));
}

void TestUpdateChecker::reportsNetworkError()
{
    QTcpServer server;
    QVERIFY(server.listen(QHostAddress::LocalHost));
    const quint16 port = server.serverPort();
    server.close();

    QNetworkAccessManager manager;
    manager.setProxy(QNetworkProxy::NoProxy);
    UpdateChecker checker(&manager, QUrl(QStringLiteral("http://127.0.0.1:%1/latest").arg(port)));
    QSignalSpy spy(&checker, &UpdateChecker::finished);

    checker.check();
    QVERIFY(spy.wait(5000));

    QCOMPARE(spy.count(), 1);
    const UpdateCheckResult result = spy.at(0).at(0).value<UpdateCheckResult>();
    QCOMPARE(result.status, UpdateCheckResult::Status::Failed);
    QVERIFY(!result.detail.isEmpty());
}

int main(int argc, char *argv[])
{
    TestUpdateChecker test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_updatechecker.moc"
