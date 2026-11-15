/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "hashtypeloader.h"
#include "testenvironment.h"

#include <QSignalSpy>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>

// The loader with an injected query: no real process, no 20-second wait.
// The parsed list, the three failure kinds, the loading state, and the
// generation guard that drops a superseded reply.
class TestHashTypeLoader : public QObject
{
    Q_OBJECT

private slots:
    void loadedEmitsTheParsedList();
    void aProcessFailureIsReported();
    void aTimeoutIsReported();
    void unparseableOutputIsReported();
    void anEmptyPathStartsNoQuery();
    void theLoadingStateIsAnnounced();
    void aSupersededQueryDoesNotEmit();

private:
    static HashcatResult okResult(const QByteArray &output)
    {
        HashcatResult result;
        result.exitStatus = QProcess::NormalExit;
        result.exitCode = 0;
        result.standardOutput = QString::fromUtf8(output);
        return result;
    }

    // A query that answers with the given result, optionally after a delay.
    static HashTypeLoader::Query canned(const HashcatResult &result, int delayMs = 0)
    {
        return [result, delayMs](const QString &, const QStringList &, int) {
            return QtConcurrent::run([result, delayMs] {
                if (delayMs > 0) {
                    QThread::msleep(delayMs);
                }
                return result;
            });
        };
    }
};

void TestHashTypeLoader::loadedEmitsTheParsedList()
{
    HashTypeLoader loader(nullptr,
                          canned(okResult(R"({"0":{"name":"MD5"},"1000":{"name":"NTLM"}})")));
    QSignalSpy loaded(&loader, &HashTypeLoader::loaded);
    QSignalSpy failed(&loader, &HashTypeLoader::failed);

    loader.reload(QStringLiteral("hashcat"));
    QTRY_COMPARE_WITH_TIMEOUT(loaded.count(), 1, 5000);
    QCOMPARE(failed.count(), 0);
    QCOMPARE(loader.queriedPath(), QStringLiteral("hashcat"));
    QVERIFY(!loader.isLoading());

    const auto types = loaded.at(0).at(0).value<QMap<quint32, QString>>();
    QCOMPARE(types.size(), 2);
    QCOMPARE(types.value(0), QStringLiteral("0 | MD5"));
    QCOMPARE(types.value(1000), QStringLiteral("1000 | NTLM"));
}

void TestHashTypeLoader::aProcessFailureIsReported()
{
    HashcatResult result;
    result.exitStatus = QProcess::NormalExit;
    result.exitCode = -2;
    result.standardError = QStringLiteral("wrong attack command");

    HashTypeLoader loader(nullptr, canned(result));
    QSignalSpy failed(&loader, &HashTypeLoader::failed);
    QSignalSpy loaded(&loader, &HashTypeLoader::loaded);

    loader.reload(QStringLiteral("hashcat"));
    QTRY_COMPARE_WITH_TIMEOUT(failed.count(), 1, 5000);
    QCOMPARE(loaded.count(), 0);
    const auto failure = failed.at(0).at(0).value<HashTypeFailure>();
    QCOMPARE(failure.kind, HashTypeFailure::Kind::ProcessFailed);
    QCOMPARE(failure.detail, QStringLiteral("wrong attack command"));
}

void TestHashTypeLoader::aTimeoutIsReported()
{
    HashcatResult result;
    result.timedOut = true;
    result.standardError = QStringLiteral("hashcat timed out");

    HashTypeLoader loader(nullptr, canned(result));
    QSignalSpy failed(&loader, &HashTypeLoader::failed);

    loader.reload(QStringLiteral("hashcat"));
    QTRY_COMPARE_WITH_TIMEOUT(failed.count(), 1, 5000);
    const auto failure = failed.at(0).at(0).value<HashTypeFailure>();
    QCOMPARE(failure.kind, HashTypeFailure::Kind::Timeout);
}

void TestHashTypeLoader::unparseableOutputIsReported()
{
    HashTypeLoader loader(nullptr, canned(okResult("this is not the machine-readable list")));
    QSignalSpy failed(&loader, &HashTypeLoader::failed);
    QSignalSpy loaded(&loader, &HashTypeLoader::loaded);

    loader.reload(QStringLiteral("hashcat"));
    QTRY_COMPARE_WITH_TIMEOUT(failed.count(), 1, 5000);
    QCOMPARE(loaded.count(), 0);
    const auto failure = failed.at(0).at(0).value<HashTypeFailure>();
    QCOMPARE(failure.kind, HashTypeFailure::Kind::Unparseable);
    QVERIFY(!failure.detail.isEmpty());
}

void TestHashTypeLoader::anEmptyPathStartsNoQuery()
{
    int calls = 0;
    HashTypeLoader loader(nullptr, [&calls](const QString &, const QStringList &, int) {
        ++calls;
        return QtConcurrent::run([] { return HashcatResult{}; });
    });
    QSignalSpy loaded(&loader, &HashTypeLoader::loaded);
    QSignalSpy failed(&loader, &HashTypeLoader::failed);

    loader.reload(QString());
    QTest::qWait(100);
    QCOMPARE(calls, 0);
    QCOMPARE(loaded.count(), 0);
    QCOMPARE(failed.count(), 0);
    QVERIFY(!loader.isLoading());
    QCOMPARE(loader.queriedPath(), QString());
}

void TestHashTypeLoader::theLoadingStateIsAnnounced()
{
    HashTypeLoader loader(nullptr, canned(okResult(R"({"0":{"name":"MD5"}})"), 50));
    QSignalSpy loading(&loader, &HashTypeLoader::loadingChanged);

    loader.reload(QStringLiteral("hashcat"));
    QVERIFY(loader.isLoading());
    QCOMPARE(loading.count(), 1);
    QCOMPARE(loading.at(0).at(0).toBool(), true);

    QTRY_VERIFY_WITH_TIMEOUT(!loader.isLoading(), 5000);
    QCOMPARE(loading.count(), 2);
    QCOMPARE(loading.at(1).at(0).toBool(), false);
}

void TestHashTypeLoader::aSupersededQueryDoesNotEmit()
{
    // The query answers slowly for "slow" and fast for "fast"; the second
    // reload replaces the watcher and raises the generation before the slow
    // answer arrives.
    HashTypeLoader loader(nullptr, [](const QString &path, const QStringList &, int) {
        const bool slow = path == QStringLiteral("slow");
        return QtConcurrent::run([slow] {
            if (slow) {
                QThread::msleep(200);
            }
            HashcatResult result;
            result.exitStatus = QProcess::NormalExit;
            result.exitCode = 0;
            result.standardOutput = slow ? QStringLiteral(R"({"1000":{"name":"NTLM"}})")
                                         : QStringLiteral(R"({"1400":{"name":"SHA2-256"}})");
            return result;
        });
    });
    QSignalSpy loaded(&loader, &HashTypeLoader::loaded);

    loader.reload(QStringLiteral("slow"));
    loader.reload(QStringLiteral("fast"));
    QTRY_COMPARE_WITH_TIMEOUT(loaded.count(), 1, 5000);
    const auto types = loaded.at(0).at(0).value<QMap<quint32, QString>>();
    QCOMPARE(types.value(1400), QStringLiteral("1400 | SHA2-256"));

    // The slow answer arrives after the wait: nothing more is emitted.
    QTest::qWait(400);
    QCOMPARE(loaded.count(), 1);
    QCOMPARE(loader.queriedPath(), QStringLiteral("fast"));
}

int main(int argc, char *argv[])
{
    TestHashTypeLoader test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_hashtypeloader.moc"