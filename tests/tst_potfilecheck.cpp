/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appconstants.h"
#include "hashcatquerytesthelpers.h"
#include "potfilecheck.h"
#include "potfilequery.h"
#include "potfiletesthelpers.h"
#include "testenvironment.h"

#include <QSignalSpy>
#include <QTest>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>

namespace
{

PotfileQueryInput input()
{
    PotfileQueryInput value;
    value.hashInput = QStringLiteral("5d41402abc4b2a76b9719d911017c592");
    value.hashType = 0;
    return value;
}

HashcatQuery queryFrom(const HashcatResult &show, const HashcatResult &left,
                       QList<QStringList> *args = nullptr, QList<int> *timeouts = nullptr)
{
    return [show, left, args, timeouts](const QString &, const QStringList &used, int timeout,
                                        const HashcatRunner::CancelToken &) {
        if (args) {
            args->append(used);
        }
        if (timeouts) {
            timeouts->append(timeout);
        }
        const bool isLeft = used.contains(QStringLiteral("--left"));
        return QtConcurrent::run([show, left, isLeft] { return isLeft ? left : show; });
    };
}

} // namespace

class TestPotfileCheck : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void okResultAddsShowAndLeft();
    void binaryHashFileShowIsAnOkResult();
    void showFailureReportsTheCombinedStreams();
    void identifyTableBecomesChooseAHashType();
    void leftFailureKeepsTheShowCount();
    void binaryLeftOutputIsNotCountedAsText();
    void strayControlAndReplacementCharactersStayText();
    void showTimeoutIsAFailure();
    void leftTimeoutKeepsTheShowCount();
    void nonJsonShowOutputIsAFailure();
    void emptyShowOutputIsAnEmptyAnswer();
    void cancelStopsTheQuery();
    void destructionCancelsTheToken();
    void startingAgainCancelsThePreviousToken();
    void anEmptyInputStartsNoQuery();
};

void TestPotfileCheck::initTestCase()
{
    qRegisterMetaType<PotfileCheckResult>("PotfileCheckResult");
}

void TestPotfileCheck::okResultAddsShowAndLeft()
{
    QList<QStringList> args;
    QList<int> timeouts;
    PotfileCheck check(nullptr,
                       queryFrom(normalResult(showLine()),
                                 normalResult(QStringLiteral("hash-left\n")), &args, &timeouts));
    QSignalSpy spy(&check, &PotfileCheck::finished);

    check.start(QStringLiteral("hashcat"), input());
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 5000);

    const PotfileCheckResult result = spy.at(0).at(0).value<PotfileCheckResult>();
    QCOMPARE(result.status, PotfileCheckResult::Status::Ok);
    QCOMPARE(result.cracked, 1);
    QCOMPARE(result.left, 1);
    QCOMPARE(result.totalCount, 1);
    QCOMPARE(result.entries.size(), 1);
    QVERIFY(!check.isRunning());

    QCOMPARE(args.size(), 2);
    QVERIFY(args.at(0).contains(QStringLiteral("--show")));
    QVERIFY(args.at(0).contains(QStringLiteral("--json")));
    QVERIFY(args.at(1).contains(QStringLiteral("--left")));
    QVERIFY(!args.at(1).contains(QStringLiteral("--json")));
    QCOMPARE(timeouts.at(0), AppConstants::Hashcat::QueryTimeoutMs);
    QVERIFY(timeouts.at(1) > 0);
    QVERIFY(timeouts.at(1) <= 2 * AppConstants::Hashcat::QueryTimeoutMs);
}

void TestPotfileCheck::binaryHashFileShowIsAnOkResult()
{
    PotfileCheck check(nullptr, queryFrom(normalResult(binaryHashFileShowLine()),
                                          normalResult(QStringLiteral("hashes.hc22000\n"))));
    QSignalSpy spy(&check, &PotfileCheck::finished);

    check.start(QStringLiteral("hashcat"), input());
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 5000);

    const PotfileCheckResult result = spy.at(0).at(0).value<PotfileCheckResult>();
    QCOMPARE(result.status, PotfileCheckResult::Status::Ok);
    QCOMPARE(result.cracked, 1);
    QCOMPARE(result.entries.size(), 1);
    QCOMPARE(result.entries.at(0).hash, QStringLiteral("hashes.hc22000"));
    QVERIFY(result.entries.at(0).hashIsFile);
}

void TestPotfileCheck::showFailureReportsTheCombinedStreams()
{
    QList<QStringList> args;
    const HashcatResult failed =
        failedResult(244, QStringLiteral("Hash parsing error in hashfile: x"),
                     QStringLiteral("No hashes loaded."));
    PotfileCheck check(nullptr, queryFrom(failed, normalResult(), &args));
    QSignalSpy spy(&check, &PotfileCheck::finished);

    check.start(QStringLiteral("hashcat"), input());
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 5000);

    const PotfileCheckResult result = spy.at(0).at(0).value<PotfileCheckResult>();
    QCOMPARE(result.status, PotfileCheckResult::Status::ShowFailed);
    QVERIFY(result.diagnostic.contains(QStringLiteral("Hash parsing error")));
    QVERIFY(result.diagnostic.contains(QStringLiteral("No hashes loaded.")));
    QCOMPARE(args.size(), 1);
}

void TestPotfileCheck::identifyTableBecomesChooseAHashType()
{
    const HashcatResult failed =
        failedResult(254,
                     QStringLiteral("The following 2 hash-modes match the structure of your "
                                    "input hash:\n"),
                     QStringLiteral("Please specify the hash-mode with -m [hash-mode]."));
    PotfileCheck check(nullptr, queryFrom(failed, normalResult()));
    QSignalSpy spy(&check, &PotfileCheck::finished);

    check.start(QStringLiteral("hashcat"), input());
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 5000);

    const PotfileCheckResult result = spy.at(0).at(0).value<PotfileCheckResult>();
    QCOMPARE(result.status, PotfileCheckResult::Status::ShowFailed);
    QVERIFY(result.diagnostic.contains(QStringLiteral("Choose a hash type")));
}

void TestPotfileCheck::leftFailureKeepsTheShowCount()
{
    const HashcatResult failed = failedResult(1, QString(), QStringLiteral("left exploded"));
    PotfileCheck check(nullptr, queryFrom(normalResult(showLine()), failed));
    QSignalSpy spy(&check, &PotfileCheck::finished);

    check.start(QStringLiteral("hashcat"), input());
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 5000);

    const PotfileCheckResult result = spy.at(0).at(0).value<PotfileCheckResult>();
    QCOMPARE(result.status, PotfileCheckResult::Status::LeftUnknown);
    QCOMPARE(result.cracked, 1);
    QCOMPARE(result.left, -1);
}

void TestPotfileCheck::binaryLeftOutputIsNotCountedAsText()
{
    QString binary = QStringLiteral("HCPX");
    binary.append(QChar(0));
    binary.append(QChar(1));

    PotfileCheck check(nullptr, queryFrom(normalResult(showLine()), normalResult(binary)));
    QSignalSpy spy(&check, &PotfileCheck::finished);

    check.start(QStringLiteral("hashcat"), input());
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 5000);

    const PotfileCheckResult result = spy.at(0).at(0).value<PotfileCheckResult>();
    QCOMPARE(result.status, PotfileCheckResult::Status::LeftUnknown);
    QCOMPARE(result.cracked, 1);
    QCOMPARE(result.left, -1);
}

void TestPotfileCheck::strayControlAndReplacementCharactersStayText()
{
    // A literal U+FFFD and a stray control character can occur in a text hash
    // line; they must not turn the whole output into a binary record.
    const QString left = QStringLiteral("hash-with-\uFFFD-and-\u000B-control\n");

    PotfileCheck check(nullptr, queryFrom(normalResult(showLine()), normalResult(left)));
    QSignalSpy spy(&check, &PotfileCheck::finished);

    check.start(QStringLiteral("hashcat"), input());
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 5000);

    const PotfileCheckResult result = spy.at(0).at(0).value<PotfileCheckResult>();
    QCOMPARE(result.status, PotfileCheckResult::Status::Ok);
    QCOMPARE(result.cracked, 1);
    QCOMPARE(result.left, 1);
}

void TestPotfileCheck::showTimeoutIsAFailure()
{
    const HashcatResult timeout = timedOutResult(QStringLiteral("hashcat timed out"));
    PotfileCheck check(nullptr, queryFrom(timeout, normalResult()));
    QSignalSpy spy(&check, &PotfileCheck::finished);

    check.start(QStringLiteral("hashcat"), input());
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 5000);

    const PotfileCheckResult result = spy.at(0).at(0).value<PotfileCheckResult>();
    QCOMPARE(result.status, PotfileCheckResult::Status::ShowFailed);
    QVERIFY(result.diagnostic.contains(QStringLiteral("timed out")));
}

void TestPotfileCheck::leftTimeoutKeepsTheShowCount()
{
    const HashcatResult timeout = timedOutResult(QStringLiteral("hashcat timed out"));
    PotfileCheck check(nullptr, queryFrom(normalResult(showLine()), timeout));
    QSignalSpy spy(&check, &PotfileCheck::finished);

    check.start(QStringLiteral("hashcat"), input());
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 5000);

    const PotfileCheckResult result = spy.at(0).at(0).value<PotfileCheckResult>();
    QCOMPARE(result.status, PotfileCheckResult::Status::LeftUnknown);
    QCOMPARE(result.cracked, 1);
    QCOMPARE(result.left, -1);
}

void TestPotfileCheck::nonJsonShowOutputIsAFailure()
{
    QList<QStringList> args;
    PotfileCheck check(nullptr,
                       queryFrom(normalResult(QStringLiteral("not-json")), normalResult(), &args));
    QSignalSpy spy(&check, &PotfileCheck::finished);

    check.start(QStringLiteral("hashcat"), input());
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 5000);

    const PotfileCheckResult result = spy.at(0).at(0).value<PotfileCheckResult>();
    QCOMPARE(result.status, PotfileCheckResult::Status::ShowFailed);
    QCOMPARE(args.size(), 1);
}

void TestPotfileCheck::emptyShowOutputIsAnEmptyAnswer()
{
    PotfileCheck check(nullptr, queryFrom(normalResult(), normalResult(QStringLiteral("a\nb\n"))));
    QSignalSpy spy(&check, &PotfileCheck::finished);

    check.start(QStringLiteral("hashcat"), input());
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 5000);

    const PotfileCheckResult result = spy.at(0).at(0).value<PotfileCheckResult>();
    QCOMPARE(result.status, PotfileCheckResult::Status::Ok);
    QCOMPARE(result.cracked, 0);
    QCOMPARE(result.left, 2);
}

void TestPotfileCheck::cancelStopsTheQuery()
{
    HashcatRunner::CancelToken token;
    PotfileCheck check(nullptr, cannedQuery(normalResult(), nullptr, &token, 1000));
    QSignalSpy spy(&check, &PotfileCheck::finished);

    check.start(QStringLiteral("hashcat"), input());
    QTRY_VERIFY_WITH_TIMEOUT(token != nullptr, 2000);
    check.cancel();
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 5000);

    const PotfileCheckResult result = spy.at(0).at(0).value<PotfileCheckResult>();
    QCOMPARE(result.status, PotfileCheckResult::Status::Cancelled);
}

void TestPotfileCheck::destructionCancelsTheToken()
{
    HashcatRunner::CancelToken token;
    auto check =
        std::make_unique<PotfileCheck>(nullptr, cannedQuery(normalResult(), nullptr, &token, 500));

    check->start(QStringLiteral("hashcat"), input());
    QTRY_VERIFY_WITH_TIMEOUT(token != nullptr, 2000);
    check.reset();

    QVERIFY(token->load());
}

void TestPotfileCheck::startingAgainCancelsThePreviousToken()
{
    QList<HashcatRunner::CancelToken> tokens;
    auto query = [&tokens](const QString &, const QStringList &, int,
                           const HashcatRunner::CancelToken &token) {
        tokens.append(token);
        return QtConcurrent::run([] {
            QThread::msleep(300);
            return HashcatResult{};
        });
    };

    PotfileCheck check(nullptr, query);
    QSignalSpy spy(&check, &PotfileCheck::finished);

    check.start(QStringLiteral("hashcat"), input());
    QTRY_COMPARE_WITH_TIMEOUT(tokens.size(), 1, 2000);
    check.start(QStringLiteral("hashcat"), input());
    QTRY_COMPARE_WITH_TIMEOUT(tokens.size(), 2, 2000);

    QVERIFY(tokens.at(0)->load());
    QVERIFY(!tokens.at(1)->load());
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 5000);
}

void TestPotfileCheck::anEmptyInputStartsNoQuery()
{
    QList<QStringList> args;
    PotfileCheck check(nullptr, queryFrom(normalResult(showLine()), normalResult(), &args));
    QSignalSpy spy(&check, &PotfileCheck::finished);

    PotfileQueryInput empty;
    empty.hashInput = QStringLiteral("   ");
    check.start(QStringLiteral("hashcat"), empty);
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 2000);

    const PotfileCheckResult result = spy.at(0).at(0).value<PotfileCheckResult>();
    QCOMPARE(result.status, PotfileCheckResult::Status::ShowFailed);
    QVERIFY(result.diagnostic.contains(QStringLiteral("Enter a hash")));
    QVERIFY(args.isEmpty());
}

int main(int argc, char *argv[])
{
    TestPotfileCheck test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_potfilecheck.moc"