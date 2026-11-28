/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appconstants.h"
#include "hashcatquerytesthelpers.h"
#include "hashtypeloader.h"
#include "testenvironment.h"

#include <QFile>
#include <QFileInfo>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
#ifndef Q_OS_WIN
// POSIX kill()/pid_t: include-cleaner maps them past this C++ wrapper.
#include <csignal> // IWYU pragma: keep
#endif

// The loader with an injected query: no real process, no 20-second wait.
// The parsed list, the three failure kinds, the loading state, and the
// generation guard that drops a superseded reply - plus the cancellation
// that kills a superseded query's process, against the real runner.
class TestHashTypeLoader : public QObject
{
    Q_OBJECT

private slots:
    void loadedEmitsTheParsedList();
    void theQueryAsksForTheExampleHashes();
    void eachQueryUsesItsOwnSession();
    void aProcessFailureIsReported();
    void aTimeoutIsReported();
    void unparseableOutputIsReported();
    void anEmptyPathStartsNoQuery();
    void theLoadingStateIsAnnounced();
    void aSupersededQueryDoesNotEmit();
    void aSupersededQueryIsCancelled();
    void destructionCancelsTheQuery();
};

void TestHashTypeLoader::loadedEmitsTheParsedList()
{
    HashTypeLoader loader(nullptr, cannedQuery(normalResult(QStringLiteral(
                                       R"({"0":{"name":"MD5"},"1000":{"name":"NTLM"}})"))));
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

void TestHashTypeLoader::theQueryAsksForTheExampleHashes()
{
    QStringList args;
    HashTypeLoader loader(
        nullptr, cannedQuery(normalResult(QStringLiteral(R"({"0":{"name":"MD5"}})")), &args));
    QSignalSpy loaded(&loader, &HashTypeLoader::loaded);

    loader.reload(QStringLiteral("hashcat"));
    QTRY_COMPARE_WITH_TIMEOUT(loaded.count(), 1, 5000);

    QCOMPARE(args.value(0), QString(AppConstants::Hashcat::ExampleHashes));
    QVERIFY(args.contains(QString(AppConstants::Hashcat::MachineReadable)));
    QVERIFY(args.contains(QString(AppConstants::Hashcat::LogfileDisable)));
    QCOMPARE(args.filter(QStringLiteral("--session=")).size(), 1);
    QVERIFY(args.join(QLatin1Char(' '))
                .contains(QStringLiteral("--session=")
                          + QString(AppConstants::Hashcat::HashTypeSessionPrefix)));
}

void TestHashTypeLoader::eachQueryUsesItsOwnSession()
{
    QStringList first;
    QStringList second;
    HashTypeLoader firstLoader(
        nullptr, cannedQuery(normalResult(QStringLiteral(R"({"0":{"name":"MD5"}})")), &first));
    HashTypeLoader secondLoader(
        nullptr, cannedQuery(normalResult(QStringLiteral(R"({"0":{"name":"MD5"}})")), &second));
    QSignalSpy firstLoaded(&firstLoader, &HashTypeLoader::loaded);
    QSignalSpy secondLoaded(&secondLoader, &HashTypeLoader::loaded);

    firstLoader.reload(QStringLiteral("hashcat"));
    secondLoader.reload(QStringLiteral("hashcat"));
    QTRY_COMPARE_WITH_TIMEOUT(firstLoaded.count(), 1, 5000);
    QTRY_COMPARE_WITH_TIMEOUT(secondLoaded.count(), 1, 5000);

    const auto session = [](const QStringList &args) {
        for (const QString &arg : args) {
            if (arg.startsWith(QStringLiteral("--session="))) {
                return arg;
            }
        }
        return QString();
    };
    QVERIFY(!session(first).isEmpty());
    QVERIFY(session(first) != session(second));
}

void TestHashTypeLoader::aProcessFailureIsReported()
{
    HashTypeLoader loader(
        nullptr, cannedQuery(failedResult(-2, QString(), QStringLiteral("wrong attack command"))));
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
    HashTypeLoader loader(nullptr,
                          cannedQuery(timedOutResult(QStringLiteral("hashcat timed out"))));
    QSignalSpy failed(&loader, &HashTypeLoader::failed);

    loader.reload(QStringLiteral("hashcat"));
    QTRY_COMPARE_WITH_TIMEOUT(failed.count(), 1, 5000);
    const auto failure = failed.at(0).at(0).value<HashTypeFailure>();
    QCOMPARE(failure.kind, HashTypeFailure::Kind::Timeout);
}

void TestHashTypeLoader::unparseableOutputIsReported()
{
    HashTypeLoader loader(nullptr, cannedQuery(normalResult(
                                       QStringLiteral("this is not the machine-readable list"))));
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
    HashTypeLoader loader(nullptr, [&calls](const QString &, const QStringList &, int,
                                            const HashcatRunner::CancelToken &) {
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
    HashTypeLoader loader(
        nullptr,
        cannedQuery(normalResult(QStringLiteral(R"({"0":{"name":"MD5"}})")), nullptr, nullptr, 50));
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
    // answer arrives - and hands the slow query's cancel token to the test.
    HashcatRunner::CancelToken slowToken;
    HashTypeLoader loader(nullptr, [&slowToken](const QString &path, const QStringList &, int,
                                                const HashcatRunner::CancelToken &token) {
        const bool slow = path == QStringLiteral("slow");
        if (slow) {
            slowToken = token;
        }
        return QtConcurrent::run([slow] {
            if (slow) {
                QThread::msleep(200);
            }
            return normalResult(slow ? QStringLiteral(R"({"1000":{"name":"NTLM"}})")
                                     : QStringLiteral(R"({"1400":{"name":"SHA2-256"}})"));
        });
    });
    QSignalSpy loaded(&loader, &HashTypeLoader::loaded);

    loader.reload(QStringLiteral("slow"));
    loader.reload(QStringLiteral("fast"));
    QTRY_COMPARE_WITH_TIMEOUT(loaded.count(), 1, 5000);
    const auto types = loaded.at(0).at(0).value<QMap<quint32, QString>>();
    QCOMPARE(types.value(1400), QStringLiteral("1400 | SHA2-256"));

    // The superseded query was asked to stop, not just ignored.
    QVERIFY(slowToken);
    QVERIFY(slowToken->load());

    // The slow answer arrives after the wait: nothing more is emitted.
    QTest::qWait(400);
    QCOMPARE(loaded.count(), 1);
    QCOMPARE(loader.queriedPath(), QStringLiteral("fast"));
}

void TestHashTypeLoader::destructionCancelsTheQuery()
{
    HashcatRunner::CancelToken token;
    {
        HashTypeLoader loader(nullptr, [&token](const QString &, const QStringList &, int,
                                                const HashcatRunner::CancelToken &cancelToken) {
            token = cancelToken;
            return QtConcurrent::run([] {
                QThread::msleep(200);
                return normalResult(QStringLiteral(R"({"0":{"name":"MD5"}})"));
            });
        });
        loader.reload(QStringLiteral("slow"));
        QVERIFY(token);
        QVERIFY(!token->load());
    }
    QVERIFY(token->load());
}

// Against the real runner: a superseded query's hashcat process is killed,
// not left running to its 20-second timeout - and an empty path cancels a
// running query the same way.
void TestHashTypeLoader::aSupersededQueryIsCancelled()
{
#ifdef Q_OS_WIN
    QSKIP("no shell stubs on this platform");
#else
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString pidFile = directory.filePath(QStringLiteral("slow.pid"));
    // The slow stub reports its pid and becomes the sleeper itself; the fast
    // one answers the query at once.
    const QString slow = TestEnvironment::writeShellStub(
        QStringLiteral("stub-hashcat-loader-slow"),
        QStringLiteral("#!/bin/sh\necho $$ > '%1'\nexec sleep 30\n").arg(pidFile).toUtf8(),
        directory.path());
    const QString fast =
        TestEnvironment::writeHashcatStub(QStringLiteral("stub-hashcat-loader-fast"));
    QVERIFY2(!slow.isEmpty() && !fast.isEmpty(), "the shell stubs could not be written");

    const auto readPid = [&pidFile] {
        QFile file(pidFile);
        if (!file.open(QIODevice::ReadOnly)) {
            return qint64(0);
        }
        return file.readAll().trimmed().toLongLong();
    };

    HashTypeLoader loader(nullptr); // the real HashcatRunner::execute
    QSignalSpy loaded(&loader, &HashTypeLoader::loaded);
    QSignalSpy failed(&loader, &HashTypeLoader::failed);

    loader.reload(slow);
    QTRY_VERIFY_WITH_TIMEOUT(QFileInfo(pidFile).size() > 0, 5000);
    const qint64 slowPid = readPid();
    QVERIFY(slowPid > 0);
    QCOMPARE(::kill(static_cast<pid_t>(slowPid), 0), 0);

    loader.reload(fast);
    QTRY_COMPARE_WITH_TIMEOUT(loaded.count(), 1, 5000);
    // The superseded process is gone; only the fast reply is emitted.
    QTRY_VERIFY_WITH_TIMEOUT(::kill(static_cast<pid_t>(slowPid), 0) != 0, 5000);
    QCOMPARE(failed.count(), 0);
    const auto types = loaded.at(0).at(0).value<QMap<quint32, QString>>();
    QCOMPARE(types.value(0), QStringLiteral("0 | MD5"));

    // An empty path starts no query and kills the one still running.
    QVERIFY(QFile::remove(pidFile));
    loader.reload(slow);
    QTRY_VERIFY_WITH_TIMEOUT(QFileInfo(pidFile).size() > 0, 5000);
    const qint64 secondPid = readPid();
    QVERIFY(secondPid > 0);
    loader.reload(QString());
    QTRY_VERIFY_WITH_TIMEOUT(::kill(static_cast<pid_t>(secondPid), 0) != 0, 5000);
    QVERIFY(!loader.isLoading());
    QTest::qWait(200);
    QCOMPARE(loaded.count(), 1);
    QCOMPARE(failed.count(), 0);
#endif
}

int main(int argc, char *argv[])
{
    TestHashTypeLoader test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_hashtypeloader.moc"