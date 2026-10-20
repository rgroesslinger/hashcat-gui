/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QElapsedTimer>
#include <QFile>
#include <QFileInfo>
#include <QDir>

#include <iterator>

#include "appconstants.h"
#include "helperutils.h"
#include "settingsmanager.h"

// The conditions that keep Execute from starting anything, the spelling of
// the parameters, the terminal table, and the hashcat helper process itself.
class TestHelperUtils : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void validateLaunch_data();
    void validateLaunch();
    void modeRequirements_data();
    void modeRequirements();
    void validateOptions_data();
    void validateOptions();
    void parameterSpelling();
    void unknownParameterIsEmpty();
    void terminalArgumentsArePinned();
    void executeHashcatWithoutAPath();
    void executeHashcatCapturesOutput();
    void executeHashcatGivesUpOnTimeout();
    void executeHashcatAppendsQuietAndUsesBinaryDirectory();
    void executeHashcatDecodesUtf8Output();

private:
    // The stubs only make sense where a #! line does
    bool stubsAvailable() const { return !m_okScript.isEmpty(); }

    QString m_okScript;
    QString m_sleepingScript;
    QString m_echoScript;
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
    m_echoScript = TestEnvironment::writeShellStub(
        QStringLiteral("stub-hashcat-echo"),
        // reports where it ran and what it was given, one item per line
        "#!/bin/sh\n"
        "pwd\n"
        "printf '%s\\n' \"$@\"\n");
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

    const QString gone =
        QDir(QDir::tempPath()).filePath(QStringLiteral("hashcat-gui-test-no-such-binary"));

    const QStringList terminals{"konsole", "kitty"};

    QTest::newRow("no hash file") << QString() << QStringLiteral("/usr/bin/hashcat")
                                  << QStringLiteral("konsole") << terminals
                                  << int(HelperUtils::LaunchError::NoHashFile) << QString();

    QTest::newRow("hashcat never configured")
        << QStringLiteral("hash.txt") << QString() << QStringLiteral("konsole") << terminals
        << int(HelperUtils::LaunchError::NoHashcatPath) << QString();

    QTest::newRow("hashcat is gone")
        << QStringLiteral("hash.txt") << gone << QStringLiteral("konsole") << terminals
        << int(HelperUtils::LaunchError::HashcatPathMissing) << gone;

    QTest::newRow("no terminal selected")
        << QStringLiteral("hash.txt") << QStringLiteral("hashcat") << QString() << terminals
        << int(HelperUtils::LaunchError::NoTerminal) << QString();

    QTest::newRow("no terminal installed")
        << QStringLiteral("hash.txt") << QStringLiteral("hashcat") << QStringLiteral("konsole")
        << QStringList() << int(HelperUtils::LaunchError::NoTerminals) << QString();

    QTest::newRow("configured terminal is not installed")
        << QStringLiteral("hash.txt") << QStringLiteral("hashcat") << QStringLiteral("yakuake")
        << terminals << int(HelperUtils::LaunchError::UnknownTerminal)
        << QStringLiteral("konsole, kitty");

    QTest::newRow("ready to go") << QStringLiteral("hash.txt") << QStringLiteral("hashcat")
                                 << QStringLiteral("konsole") << terminals
                                 << int(HelperUtils::LaunchError::None) << QString();
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

// What each attack mode needs from the user. These numbers are what the
// launch validation and the hint under the mode selector work with.
void TestHelperUtils::modeRequirements_data()
{
    QTest::addColumn<int>("mode");
    QTest::addColumn<int>("minWordlists");
    QTest::addColumn<int>("maxWordlists");
    QTest::addColumn<bool>("needsMask");

    QTest::newRow("straight") << int(AttackMode::Straight) << 1 << -1 << false;
    QTest::newRow("combination") << int(AttackMode::Combination) << 2 << 2 << false;
    QTest::newRow("brute force") << int(AttackMode::BruteForce) << 0 << -1 << true;
    QTest::newRow("hybrid word + mask") << int(AttackMode::HybridWordMask) << 1 << -1 << true;
    QTest::newRow("hybrid mask + word") << int(AttackMode::HybridMaskWord) << 1 << -1 << true;
    QTest::newRow("association") << int(AttackMode::Association) << 1 << -1 << false;
}

void TestHelperUtils::modeRequirements()
{
    QFETCH(int, mode);
    QFETCH(int, minWordlists);
    QFETCH(int, maxWordlists);
    QFETCH(bool, needsMask);

    const HelperUtils::ModeRequirements requirements =
        HelperUtils::modeRequirements(static_cast<AttackMode>(mode));

    QCOMPARE(requirements.minWordlists, minWordlists);
    QCOMPARE(requirements.maxWordlists, maxWordlists);
    QCOMPARE(requirements.needsMask, needsMask);
}

// The checks that keep an attack that hashcat would refuse from starting.
void TestHelperUtils::validateOptions_data()
{
    QTest::addColumn<int>("mode");
    QTest::addColumn<QStringList>("wordlists");
    QTest::addColumn<QString>("mask");
    QTest::addColumn<bool>("increment");
    QTest::addColumn<int>("incrementMin");
    QTest::addColumn<int>("incrementMax");
    QTest::addColumn<int>("expected");
    QTest::addColumn<QString>("expectedDetail");

    const QStringList one{QStringLiteral("/tmp/a.txt")};
    const QStringList two{QStringLiteral("/tmp/a.txt"), QStringLiteral("/tmp/b.txt")};
    const QStringList three{QStringLiteral("/tmp/a.txt"), QStringLiteral("/tmp/b.txt"),
                            QStringLiteral("/tmp/c.txt")};

    QTest::newRow("straight with one word list")
        << int(AttackMode::Straight) << one << QString() << false << 0 << 0
        << int(HelperUtils::LaunchError::None) << QString();

    QTest::newRow("straight without a word list")
        << int(AttackMode::Straight) << QStringList() << QString() << false << 0 << 0
        << int(HelperUtils::LaunchError::NoWordlist) << QString();

    QTest::newRow("straight with several word lists")
        << int(AttackMode::Straight) << three << QString() << false << 0 << 0
        << int(HelperUtils::LaunchError::None) << QString();

    QTest::newRow("combination with two word lists")
        << int(AttackMode::Combination) << two << QString() << false << 0 << 0
        << int(HelperUtils::LaunchError::None) << QString();

    QTest::newRow("combination with one word list")
        << int(AttackMode::Combination) << one << QString() << false << 0 << 0
        << int(HelperUtils::LaunchError::WrongWordlistCount) << QStringLiteral("1");

    QTest::newRow("combination with three word lists")
        << int(AttackMode::Combination) << three << QString() << false << 0 << 0
        << int(HelperUtils::LaunchError::WrongWordlistCount) << QStringLiteral("3");

    QTest::newRow("brute force with a mask")
        << int(AttackMode::BruteForce) << QStringList() << QStringLiteral("?a?a?a?a") << false << 0
        << 0 << int(HelperUtils::LaunchError::None) << QString();

    QTest::newRow("brute force without a mask")
        << int(AttackMode::BruteForce) << QStringList() << QString() << false << 0 << 0
        << int(HelperUtils::LaunchError::NoMask) << QString();

    // Word lists do not matter in brute force mode: they are not part of the
    // command line there.
    QTest::newRow("brute force ignores the word lists")
        << int(AttackMode::BruteForce) << three << QStringLiteral("?a?a?a?a") << false << 0 << 0
        << int(HelperUtils::LaunchError::None) << QString();

    QTest::newRow("hybrid word + mask needs both")
        << int(AttackMode::HybridWordMask) << one << QString() << false << 0 << 0
        << int(HelperUtils::LaunchError::NoMask) << QString();

    QTest::newRow("hybrid mask + word needs a word list")
        << int(AttackMode::HybridMaskWord) << QStringList() << QStringLiteral("?a?a?a?a") << false
        << 0 << 0 << int(HelperUtils::LaunchError::NoWordlist) << QString();

    QTest::newRow("association without a word list")
        << int(AttackMode::Association) << QStringList() << QString() << false << 0 << 0
        << int(HelperUtils::LaunchError::NoWordlist) << QString();

    QTest::newRow("increment bounds that fit")
        << int(AttackMode::BruteForce) << QStringList() << QStringLiteral("?a?a?a?a") << true << 4
        << 8 << int(HelperUtils::LaunchError::None) << QString();

    QTest::newRow("increment minimum above the maximum")
        << int(AttackMode::BruteForce) << QStringList() << QStringLiteral("?a?a?a?a") << true << 8
        << 4 << int(HelperUtils::LaunchError::InvalidIncrement) << QString();

    // Without the flag the bounds are not used at all, so they cannot
    // contradict each other either.
    QTest::newRow("increment bounds without the flag")
        << int(AttackMode::BruteForce) << QStringList() << QStringLiteral("?a?a?a?a") << false << 8
        << 4 << int(HelperUtils::LaunchError::None) << QString();
}

void TestHelperUtils::validateOptions()
{
    QFETCH(int, mode);
    QFETCH(QStringList, wordlists);
    QFETCH(QString, mask);
    QFETCH(bool, increment);
    QFETCH(int, incrementMin);
    QFETCH(int, incrementMax);
    QFETCH(int, expected);
    QFETCH(QString, expectedDetail);

    HashcatOptions options;
    options.attackMode = static_cast<AttackMode>(mode);
    options.wordlists = wordlists;
    options.mask = mask;
    options.increment = increment;
    options.incrementMin = incrementMin;
    options.incrementMax = incrementMax;

    // A stale value must not survive a call that has nothing to report
    QString detail(QStringLiteral("stale"));
    const HelperUtils::LaunchError error = HelperUtils::validateOptions(options, &detail);

    QCOMPARE(int(error), expected);
    QCOMPARE(detail, expectedDetail);

    // The same has to hold when the caller does not care about the detail
    QCOMPARE(int(HelperUtils::validateOptions(options, nullptr)), expected);
}

void TestHelperUtils::parameterSpelling()
{
    using Parameter = HelperUtils::Parameter;

    QCOMPARE(HelperUtils::getParameter(Parameter::AttackMode), QStringLiteral("--attack-mode"));
    QCOMPARE(HelperUtils::getParameter(Parameter::AttackMode, true), QStringLiteral("-a"));

    // --cpu-affinity has no short form; asking for one must fall back to the
    // long one rather than produce an empty argument
    QCOMPARE(HelperUtils::getParameter(Parameter::CpuAffinity, true),
             QStringLiteral("--cpu-affinity"));
    QCOMPARE(HelperUtils::getParameter(Parameter::CpuAffinity, false),
             QStringLiteral("--cpu-affinity"));
    QCOMPARE(HelperUtils::getParameter(Parameter::HexCharset, true),
             QStringLiteral("--hex-charset"));

    // The short forms that differ from the parameter name are easy to get
    // wrong, so pin them here as well.
    QCOMPARE(HelperUtils::getParameter(Parameter::Increment, true), QStringLiteral("-i"));
    QCOMPARE(HelperUtils::getParameter(Parameter::Skip, true), QStringLiteral("-s"));
    QCOMPARE(HelperUtils::getParameter(Parameter::Limit, true), QStringLiteral("-l"));
    QCOMPARE(HelperUtils::getParameter(Parameter::OpenclDeviceTypes, true), QStringLiteral("-D"));
    QCOMPARE(HelperUtils::getParameter(Parameter::RuleLeft, true), QStringLiteral("-j"));
    QCOMPARE(HelperUtils::getParameter(Parameter::RuleRight, true), QStringLiteral("-k"));

    // every enumerator has to be spelled out, otherwise the option silently
    // vanishes from the command line
    const Parameter all[] = {
        Parameter::AttackMode,      Parameter::BackendDevices,  Parameter::CpuAffinity,
        Parameter::CustomCharset1,  Parameter::CustomCharset2,  Parameter::CustomCharset3,
        Parameter::CustomCharset4,  Parameter::EncodingFrom,    Parameter::EncodingTo,
        Parameter::Force,           Parameter::GenerateRules,   Parameter::HashType,
        Parameter::HexCharset,      Parameter::HexSalt,         Parameter::HexWordlist,
        Parameter::HwmonTempAbort,  Parameter::Increment,       Parameter::IncrementMax,
        Parameter::IncrementMin,    Parameter::Limit,           Parameter::OpenclDeviceTypes,
        Parameter::OptimizedKernel, Parameter::Outfile,         Parameter::OutfileFormat,
        Parameter::PotfileDisable,  Parameter::Remove,          Parameter::RuleLeft,
        Parameter::RuleRight,       Parameter::RulesFile,       Parameter::Runtime,
        Parameter::SegmentSize,     Parameter::Session,         Parameter::Skip,
        Parameter::SpeedOnly,       Parameter::Status,          Parameter::StatusTimer,
        Parameter::Username,        Parameter::WorkloadProfile,
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

// The mapping executeClicked() builds its command line from: pin every
// entry, because a wrong one is a terminal that starts and then never runs hashcat.
void TestHelperUtils::terminalArgumentsArePinned()
{
    struct Expected {
        const char *terminal;
        QStringList arguments;
    };
    const Expected expected[] = {
        {"cmd.exe", {"/k"}},
        {"xterm", {"-hold", "-e"}},
        {"gnome-terminal", {"--wait", "--"}},
        {"ptyxis", {"--"}},
        {"konsole", {"--hold", "-e"}},
        {"xfce4-terminal", {"--hold", "-x"}},
        {"alacritty", {"-e"}},
        {"kitty", {"--hold"}},
        {"foot", {"--hold"}},
        {"wezterm", {"start", "--"}},
        {"wt.exe", {"-d", "."}},
    };

    const QMap<QString, QStringList> &table = HelperUtils::terminalArguments();
    QCOMPARE(int(table.size()), int(std::size(expected)));
    for (const Expected &entry : expected) {
        QCOMPARE(table.value(QString::fromLatin1(entry.terminal)), entry.arguments);
    }
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

// The contract with every caller: the caller's arguments go first, --quiet
// is appended so the output can be read, and the child runs next to its own
// binary - that is where hashcat looks for its profiles and modules.
void TestHelperUtils::executeHashcatAppendsQuietAndUsesBinaryDirectory()
{
#ifdef Q_OS_WIN
    QSKIP("no shell stubs on this platform");
#else
    QVERIFY2(!m_echoScript.isEmpty(), "the shell stub could not be written");
#endif

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, m_echoScript);

    const HashcatResult result =
        HelperUtils::executeHashcat({QStringLiteral("--version")}).result();

    QCOMPARE(int(result.exitStatus), int(QProcess::NormalExit));
    QCOMPARE(result.exitCode, 0);

    const QStringList output = result.standardOutput.split(QLatin1Char('\n'), Qt::SkipEmptyParts);
    QCOMPARE(output.size(), 3);

    // working directory = the directory of the binary (canonical, so a
    // symlinked temp path cannot tell the two apart)
    QCOMPARE(QDir(output.value(0)).canonicalPath(),
             QDir(QFileInfo(m_echoScript).absolutePath()).canonicalPath());

    // the caller's arguments, then --quiet - in that order
    const QStringList expectedArguments{QStringLiteral("--version"),
                                        QString(AppConstants::Hashcat::Quiet)};
    QCOMPARE(output.mid(1), expectedArguments);
}

// The bytes QProcess hands back are decoded as UTF-8, so a non-ASCII reply
// arrives intact. The literals are \u escapes.
void TestHelperUtils::executeHashcatDecodesUtf8Output()
{
#ifdef Q_OS_WIN
    QSKIP("no shell stubs on this platform");
#else
    QVERIFY2(stubsAvailable(), "the shell stub could not be written");
#endif

    const QString text =
        QStringLiteral("Gr\u00fc\u00dfe \u2013 \u00e4\u00f6\u00fc\u00df \u65e5\u672c\u8a9e");

    // The stub prints the UTF-8 bytes of the text, exactly as hashcat would.
    const QByteArray script =
        QStringLiteral("#!/bin/sh\nprintf '%s\\n' \"%1\"\n").arg(text).toUtf8();
    const QString stub =
        TestEnvironment::writeShellStub(QStringLiteral("stub-hashcat-utf8"), script);
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    const HashcatResult result =
        HelperUtils::executeHashcat({QStringLiteral("--version")}).result();

    QCOMPARE(int(result.exitStatus), int(QProcess::NormalExit));
    QCOMPARE(result.exitCode, 0);
    QCOMPARE(result.standardOutput.trimmed(), text);
}

int main(int argc, char *argv[])
{
    TestHelperUtils test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_helperutils.moc"
