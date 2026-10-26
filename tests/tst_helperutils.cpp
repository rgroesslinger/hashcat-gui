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

// The conditions that keep Execute from starting anything, the terminal
// table, and the hashcat helper process itself.
class TestHelperUtils : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void validateLaunch_data();
    void validateLaunch();
    void validateOptions_data();
    void validateOptions();
    void terminalArgumentsArePinned();
    void findHashcatInPathFindsTheExecutable();
    void findHashcatInPathIsEmptyWithoutOne();
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

// The checks that keep an attack that hashcat would refuse from starting.
void TestHelperUtils::validateOptions_data()
{
    QTest::addColumn<int>("mode");
    QTest::addColumn<QStringList>("wordlists");
    QTest::addColumn<QString>("mask");
    QTest::addColumn<bool>("increment");
    QTest::addColumn<int>("incrementMin");
    QTest::addColumn<int>("incrementMax");
    QTest::addColumn<bool>("speedOnly");
    QTest::addColumn<bool>("remove");
    QTest::addColumn<int>("expected");
    QTest::addColumn<QString>("expectedDetail");

    const QStringList one{QStringLiteral("/tmp/a.txt")};
    const QStringList two{QStringLiteral("/tmp/a.txt"), QStringLiteral("/tmp/b.txt")};
    const QStringList three{QStringLiteral("/tmp/a.txt"), QStringLiteral("/tmp/b.txt"),
                            QStringLiteral("/tmp/c.txt")};

    QTest::newRow("straight with one word list")
        << int(AttackMode::Straight) << one << QString() << false << 0 << 0 << false << false
        << int(HelperUtils::LaunchError::None) << QString();

    QTest::newRow("straight without a word list")
        << int(AttackMode::Straight) << QStringList() << QString() << false << 0 << 0 << false
        << false << int(HelperUtils::LaunchError::NoWordlist) << QString();

    QTest::newRow("straight with several word lists")
        << int(AttackMode::Straight) << three << QString() << false << 0 << 0 << false << false
        << int(HelperUtils::LaunchError::None) << QString();

    QTest::newRow("combination with two word lists")
        << int(AttackMode::Combination) << two << QString() << false << 0 << 0 << false << false
        << int(HelperUtils::LaunchError::None) << QString();

    QTest::newRow("combination with one word list")
        << int(AttackMode::Combination) << one << QString() << false << 0 << 0 << false << false
        << int(HelperUtils::LaunchError::WrongWordlistCount) << QStringLiteral("1");

    QTest::newRow("combination with three word lists")
        << int(AttackMode::Combination) << three << QString() << false << 0 << 0 << false << false
        << int(HelperUtils::LaunchError::WrongWordlistCount) << QStringLiteral("3");

    QTest::newRow("brute force with a mask")
        << int(AttackMode::BruteForce) << QStringList() << QStringLiteral("?a?a?a?a") << false << 0
        << 0 << false << false << int(HelperUtils::LaunchError::None) << QString();

    QTest::newRow("brute force without a mask")
        << int(AttackMode::BruteForce) << QStringList() << QString() << false << 0 << 0 << false
        << false << int(HelperUtils::LaunchError::NoMask) << QString();

    // Word lists do not matter in brute force mode: they are not part of the
    // command line there.
    QTest::newRow("brute force ignores the word lists")
        << int(AttackMode::BruteForce) << three << QStringLiteral("?a?a?a?a") << false << 0 << 0
        << false << false << int(HelperUtils::LaunchError::None) << QString();

    QTest::newRow("hybrid word + mask needs both")
        << int(AttackMode::HybridWordMask) << one << QString() << false << 0 << 0 << false << false
        << int(HelperUtils::LaunchError::NoMask) << QString();

    QTest::newRow("hybrid mask + word needs a word list")
        << int(AttackMode::HybridMaskWord) << QStringList() << QStringLiteral("?a?a?a?a") << false
        << 0 << 0 << false << false << int(HelperUtils::LaunchError::NoWordlist) << QString();

    // Association reads its hints from a word list; hashcat refuses the
    // command without one (user_options.c: association needs hc_argc >= 2).
    QTest::newRow("association without a word list")
        << int(AttackMode::Association) << QStringList() << QString() << false << 0 << 0 << false
        << false << int(HelperUtils::LaunchError::NoWordlist) << QString();

    QTest::newRow("increment bounds that fit")
        << int(AttackMode::BruteForce) << QStringList() << QStringLiteral("?a?a?a?a") << true << 4
        << 8 << false << false << int(HelperUtils::LaunchError::None) << QString();

    QTest::newRow("increment minimum above the maximum")
        << int(AttackMode::BruteForce) << QStringList() << QStringLiteral("?a?a?a?a") << true << 8
        << 4 << false << false << int(HelperUtils::LaunchError::InvalidIncrement) << QString();

    // Without the flag the bounds are not used at all, so they cannot
    // contradict each other either.
    QTest::newRow("increment bounds without the flag")
        << int(AttackMode::BruteForce) << QStringList() << QStringLiteral("?a?a?a?a") << false << 8
        << 4 << false << false << int(HelperUtils::LaunchError::None) << QString();

    // Only the mask modes put -i on the command line, so stale bounds in the
    // other modes must not keep a run from starting.
    QTest::newRow("straight ignores stale increment bounds")
        << int(AttackMode::Straight) << one << QString() << true << 8 << 4 << false << false
        << int(HelperUtils::LaunchError::None) << QString();

    QTest::newRow("combination ignores stale increment bounds")
        << int(AttackMode::Combination) << two << QString() << true << 8 << 4 << false << false
        << int(HelperUtils::LaunchError::None) << QString();

    QTest::newRow("association ignores stale increment bounds")
        << int(AttackMode::Association) << one << QString() << true << 8 << 4 << false << false
        << int(HelperUtils::LaunchError::None) << QString();

    // hashcat refuses -i together with --speed-only, but only the mask modes
    // emit -i in the first place.
    QTest::newRow("increment with speed only")
        << int(AttackMode::BruteForce) << QStringList() << QStringLiteral("?a?a?a?a") << true << 4
        << 8 << true << false << int(HelperUtils::LaunchError::IncrementWithSpeedOnly) << QString();

    QTest::newRow("speed only without the increment")
        << int(AttackMode::BruteForce) << QStringList() << QStringLiteral("?a?a?a?a") << false << 0
        << 0 << true << false << int(HelperUtils::LaunchError::None) << QString();

    // The bounds are checked first, so both being wrong reports the bounds.
    QTest::newRow("increment bounds win over speed only")
        << int(AttackMode::BruteForce) << QStringList() << QStringLiteral("?a?a?a?a") << true << 8
        << 4 << true << false << int(HelperUtils::LaunchError::InvalidIncrement) << QString();

    QTest::newRow("straight ignores stale increment with speed only")
        << int(AttackMode::Straight) << one << QString() << true << 8 << 4 << true << false
        << int(HelperUtils::LaunchError::None) << QString();

    // --remove edits the hash file, so hashcat refuses it in association mode.
    QTest::newRow("remove in association mode")
        << int(AttackMode::Association) << one << QString() << false << 0 << 0 << false << true
        << int(HelperUtils::LaunchError::RemoveInAssociation) << QString();

    QTest::newRow("remove in straight mode")
        << int(AttackMode::Straight) << one << QString() << false << 0 << 0 << false << true
        << int(HelperUtils::LaunchError::None) << QString();

    // The mode requirements are reported before the option conflicts.
    QTest::newRow("association without a word list and remove")
        << int(AttackMode::Association) << QStringList() << QString() << false << 0 << 0 << false
        << true << int(HelperUtils::LaunchError::NoWordlist) << QString();
}

void TestHelperUtils::validateOptions()
{
    QFETCH(int, mode);
    QFETCH(QStringList, wordlists);
    QFETCH(QString, mask);
    QFETCH(bool, increment);
    QFETCH(int, incrementMin);
    QFETCH(int, incrementMax);
    QFETCH(bool, speedOnly);
    QFETCH(bool, remove);
    QFETCH(int, expected);
    QFETCH(QString, expectedDetail);

    HashcatOptions options;
    options.attackMode = static_cast<AttackMode>(mode);
    options.wordlists = wordlists;
    options.mask = mask;
    options.increment = increment;
    options.incrementMin = incrementMin;
    options.incrementMax = incrementMax;
    options.speedOnly = speedOnly;
    options.remove = remove;

    // A stale value must not survive a call that has nothing to report
    QString detail(QStringLiteral("stale"));
    const HelperUtils::LaunchError error = HelperUtils::validateOptions(options, &detail);

    QCOMPARE(int(error), expected);
    QCOMPARE(detail, expectedDetail);

    // The same has to hold when the caller does not care about the detail
    QCOMPARE(int(HelperUtils::validateOptions(options, nullptr)), expected);
}

// The mapping startClicked() builds its command line from: pin every
// entry, because a wrong one is a terminal that starts and then never runs hashcat.
void TestHelperUtils::terminalArgumentsArePinned()
{
    struct Expected {
        const char *terminal;
        QStringList arguments;
    };
    const Expected expected[] = {
        {"cmd.exe", {"/k"}},
        {"powershell.exe", {"-NoExit", "-NoProfile", "-Command", "&"}},
        {"pwsh", {"-NoExit", "-NoProfile", "-Command", "&"}},
        {"xterm", {"-hold", "-e"}},
        {"gnome-terminal", {"--wait", "--"}},
        {"ptyxis", {"--"}},
        {"konsole", {"--hold", "-e"}},
        {"xfce4-terminal", {"--hold", "-x"}},
        {"alacritty", {"--hold", "-e"}},
        {"kitty", {"--hold"}},
        {"foot", {"--hold"}},
        {"wezterm", {"start", "--"}},
    };

    const QMap<QString, QStringList> &table = HelperUtils::terminalArguments();
    QCOMPARE(int(table.size()), int(std::size(expected)));
    for (const Expected &entry : expected) {
        QCOMPARE(table.value(QString::fromLatin1(entry.terminal)), entry.arguments);
    }
}

// The lookup follows PATH and only reports a file this system can execute.
void TestHelperUtils::findHashcatInPathFindsTheExecutable()
{
    const QString stub =
        TestEnvironment::writeSearchableExecutable(QString(AppConstants::Hashcat::ExecutableName));
    QVERIFY2(!stub.isEmpty(), "the executable stub could not be written");

    TestEnvironment::PathGuard guard(QFileInfo(stub).absolutePath().toUtf8());

    const QString found = HelperUtils::findHashcatInPath();
    QVERIFY2(!found.isEmpty(), "the executable on PATH was not found");
    QCOMPARE(QFileInfo(found).canonicalFilePath(), QFileInfo(stub).canonicalFilePath());
}

void TestHelperUtils::findHashcatInPathIsEmptyWithoutOne()
{
    QTemporaryDir empty;
    QVERIFY(empty.isValid());

    TestEnvironment::PathGuard guard(empty.path().toUtf8());

    QCOMPARE(HelperUtils::findHashcatInPath(), QString());
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
    // hang the "Updating…" state forever is what this guards against.
    QVERIFY2(elapsed.elapsed() < 10000, qPrintable(QStringLiteral("%1 ms").arg(elapsed.elapsed())));
    QCOMPARE(int(result.exitStatus), int(QProcess::CrashExit));
    QCOMPARE(result.exitCode, -1);
    QVERIFY(result.standardError.contains(QStringLiteral("timed out")));
}

// The contract with every caller: the caller's arguments go first, --quiet
// is appended so the output can be read, and the child runs next to its own
// binary - that is where hashcat looks for its modules.
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
