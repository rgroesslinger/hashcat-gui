/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QElapsedTimer>
#include <QFile>
#include <QFileInfo>
#include <QDir>

#include "appconstants.h"
#include "helperutils.h"
#include "settingsmanager.h"

// The conditions that keep Execute from starting anything, the spelling of the
// parameters, and the hashcat helper process itself.
class TestHelperUtils : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void validateLaunch_data();
    void validateLaunch();
    void parameterSpelling();
    void unknownParameterIsEmpty();
    void executeHashcatWithoutAPath();
    void executeHashcatCapturesOutput();
    void executeHashcatGivesUpOnTimeout();

private:
    // The stubs only make sense where a #! line does
    bool stubsAvailable() const { return !m_okScript.isEmpty(); }

    QString m_okScript;
    QString m_sleepingScript;
    QString m_savedHashcatPath;
};

void TestHelperUtils::initTestCase()
{
    m_okScript = TestEnvironment::writeShellStub(
        QStringLiteral("stub-hashcat"),
        "#!/bin/sh\n"
        "printf '{\"0\":{\"name\":\"MD5\"},\"1000\":{\"name\":\"NTLM\"}}\\n'\n"
        "exit 0\n");
    m_sleepingScript = TestEnvironment::writeShellStub(
        QStringLiteral("stub-hashcat-slow"),
        // exec replaces the shell, so the process QProcess kills is the
        // sleeper itself instead of a shell whose sleep child is orphaned
        "#!/bin/sh\n"
        "exec sleep 30\n");
}

void TestHelperUtils::init()
{
    m_savedHashcatPath =
        SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
}

// executeHashcat() reads the configured path on the calling thread, so every
// test that touches it has to set - and undo - the setting itself.
void TestHelperUtils::cleanup()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, m_savedHashcatPath);
}

void TestHelperUtils::validateLaunch_data()
{
    QTest::addColumn<QString>("hashFile");
    QTest::addColumn<QString>("hashcatPath");
    QTest::addColumn<QString>("configuredTerminal");
    QTest::addColumn<QStringList>("availableTerminals");
    QTest::addColumn<int>("expected");
    QTest::addColumn<QString>("expectedDetail");

    const QString gone = QDir(QDir::tempPath())
                             .filePath(QStringLiteral("hashcat-gui-test-no-such-binary"));

    const QStringList terminals{"konsole", "kitty"};

    QTest::newRow("no hash file")
        << QString() << QStringLiteral("/usr/bin/hashcat") << QStringLiteral("konsole")
        << terminals << int(HelperUtils::LaunchError::NoHashFile) << QString();

    QTest::newRow("hashcat never configured")
        << QStringLiteral("hash.txt") << QString() << QStringLiteral("konsole")
        << terminals << int(HelperUtils::LaunchError::NoHashcatPath) << QString();

    QTest::newRow("hashcat is gone")
        << QStringLiteral("hash.txt") << gone << QStringLiteral("konsole")
        << terminals << int(HelperUtils::LaunchError::HashcatPathMissing) << gone;

    QTest::newRow("no terminal selected")
        << QStringLiteral("hash.txt") << QStringLiteral("hashcat") << QString()
        << terminals << int(HelperUtils::LaunchError::NoTerminal) << QString();

    QTest::newRow("no terminal installed")
        << QStringLiteral("hash.txt") << QStringLiteral("hashcat") << QStringLiteral("konsole")
        << QStringList() << int(HelperUtils::LaunchError::NoTerminals) << QString();

    QTest::newRow("configured terminal is not installed")
        << QStringLiteral("hash.txt") << QStringLiteral("hashcat") << QStringLiteral("yakuake")
        << terminals << int(HelperUtils::LaunchError::UnknownTerminal)
        << QStringLiteral("konsole, kitty");

    QTest::newRow("ready to go")
        << QStringLiteral("hash.txt") << QStringLiteral("hashcat") << QStringLiteral("konsole")
        << terminals << int(HelperUtils::LaunchError::None) << QString();
}

void TestHelperUtils::validateLaunch()
{
    QFETCH(QString, hashFile);
    QFETCH(QString, hashcatPath);
    QFETCH(QString, configuredTerminal);
    QFETCH(QStringList, availableTerminals);
    QFETCH(int, expected);
    QFETCH(QString, expectedDetail);

    // A stale value must not survive a call that has nothing to report
    QString detail(QStringLiteral("stale"));
    const HelperUtils::LaunchError error = HelperUtils::validateLaunch(
        hashFile, hashcatPath, configuredTerminal, availableTerminals, &detail);

    QCOMPARE(int(error), expected);
    QCOMPARE(detail, expectedDetail);

    if (expected == int(HelperUtils::LaunchError::HashcatPathMissing)) {
        QVERIFY2(!QFileInfo::exists(hashcatPath), "the row needs a path that does not exist");
    }

    // The same has to hold when the caller does not care about the detail
    QCOMPARE(int(HelperUtils::validateLaunch(hashFile, hashcatPath, configuredTerminal,
                                             availableTerminals, nullptr)),
             expected);
}

void TestHelperUtils::parameterSpelling()
{
    using Parameter = HelperUtils::Parameter;

    QCOMPARE(HelperUtils::getParameter(Parameter::AttackMode), QStringLiteral("--attack-mode"));
    QCOMPARE(HelperUtils::getParameter(Parameter::AttackMode, true), QStringLiteral("-a"));

    // --cpu-affinity has no short form; asking for one must fall back to the
    // long one rather than produce an empty argument
    QCOMPARE(HelperUtils::getParameter(Parameter::CpuAffinity, true), QStringLiteral("--cpu-affinity"));
    QCOMPARE(HelperUtils::getParameter(Parameter::CpuAffinity, false), QStringLiteral("--cpu-affinity"));
    QCOMPARE(HelperUtils::getParameter(Parameter::HexCharset, true), QStringLiteral("--hex-charset"));

    // every enumerator has to be spelled out, otherwise the option silently
    // vanishes from the command line
    const Parameter all[] = {
        Parameter::AttackMode, Parameter::BackendDevices, Parameter::CpuAffinity,
        Parameter::CustomCharset1, Parameter::CustomCharset2, Parameter::CustomCharset3,
        Parameter::CustomCharset4, Parameter::GenerateRules, Parameter::HashType,
        Parameter::HexCharset, Parameter::HexSalt, Parameter::OptimizedKernel,
        Parameter::Outfile, Parameter::OutfileFormat, Parameter::Remove,
        Parameter::RulesFile, Parameter::SegmentSize, Parameter::SpeedOnly,
        Parameter::Username, Parameter::WorkloadProfile,
    };
    for (const Parameter key : all) {
        QVERIFY2(!HelperUtils::getParameter(key).isEmpty(), "an enum has no long spelling");
        QVERIFY2(!HelperUtils::getParameter(key, true).isEmpty(), "an enum has no spelling at all");
    }
}

void TestHelperUtils::unknownParameterIsEmpty()
{
    QVERIFY(HelperUtils::getParameter(static_cast<HelperUtils::Parameter>(4711)).isEmpty());
}

void TestHelperUtils::executeHashcatWithoutAPath()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, QString());

    const HashcatResult result = HelperUtils::executeHashcat({QStringLiteral("--help")}).result();

    // "no process ran" has to look like a failure to every consumer
    QCOMPARE(int(result.exitStatus), int(QProcess::CrashExit));
    QCOMPARE(result.exitCode, -1);
    QVERIFY(result.standardError.contains(QStringLiteral("not configured")));
}

void TestHelperUtils::executeHashcatCapturesOutput()
{
#ifdef Q_OS_WIN
    QSKIP("no shell stubs on this platform");
#else
    // A stub that never got written is a broken test environment, not a
    // reason to report success without having run anything
    QVERIFY2(stubsAvailable(), "the shell stub could not be written");
#endif

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, m_okScript);

    const HashcatResult result =
        HelperUtils::executeHashcat({QStringLiteral("--example-hashes")}).result();

    QCOMPARE(int(result.exitStatus), int(QProcess::NormalExit));
    QCOMPARE(result.exitCode, 0);
    QVERIFY(result.standardOutput.contains(QStringLiteral("NTLM")));
}

void TestHelperUtils::executeHashcatGivesUpOnTimeout()
{
#ifdef Q_OS_WIN
    QSKIP("no shell stubs on this platform");
#else
    QVERIFY2(stubsAvailable(), "the shell stub could not be written");
#endif

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, m_sleepingScript);

    QElapsedTimer elapsed;
    elapsed.start();
    const HashcatResult result =
        HelperUtils::executeHashcat({QStringLiteral("--help")}, 300).result();

    // The child is killed and reaped rather than waited out: a query that can
    // hang the "Updating..." state forever is what this guards against.
    QVERIFY2(elapsed.elapsed() < 10000, qPrintable(QStringLiteral("%1 ms").arg(elapsed.elapsed())));
    QCOMPARE(int(result.exitStatus), int(QProcess::CrashExit));
    QCOMPARE(result.exitCode, -1);
    QVERIFY(result.standardError.contains(QStringLiteral("timed out")));
}

int main(int argc, char *argv[])
{
    TestHelperUtils test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_helperutils.moc"
