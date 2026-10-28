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
    void feedModeRequirements();
    void pcfgSettingsAreValidated();
    void tableSettingsAreValidated();
    void associationSettingsAreValidated();
    void maskMarkersAreValidated();
    void hashRecipeIsRequiredFor4000();
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

    // Hashcat 8.0.0 reads the words of an association attack from the hash
    // file: without a wordlist the mode runs, and the settings line names the
    // phases to run.
    QTest::newRow("association without a word list")
        << int(AttackMode::Association) << QStringList() << QString() << false << 0 << 0 << false
        << false << int(HelperUtils::LaunchError::None) << QString();

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
        << true << int(HelperUtils::LaunchError::RemoveInAssociation) << QString();

    // The GUI caps the pairing at one wordlist; hashcat reads two, but the
    // list offers no second role for a second list. The mode has a range
    // (0 or 1), so the error is the at-most one, not the exact count.
    QTest::newRow("association with two word lists")
        << int(AttackMode::Association) << two << QString() << false << 0 << 0 << false << false
        << int(HelperUtils::LaunchError::TooManyWordlists) << QStringLiteral("2");
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

// What the feed modes owe beyond the shared checks: the table a mode 5 run
// reads, the feed name of mode 8 and the wordlist caps.
void TestHelperUtils::feedModeRequirements()
{
    const QStringList one{QStringLiteral("/tmp/a.txt")};
    const QStringList two{QStringLiteral("/tmp/a.txt"), QStringLiteral("/tmp/b.txt")};

    // Mode 5 takes exactly one wordlist: none is a missing list, two is one
    // too many - and without a table there is nothing to run.
    HashcatOptions table;
    table.attackMode = AttackMode::Table;
    table.tableFiles = QStringList{QStringLiteral("/tmp/table.txt")};
    QCOMPARE(int(HelperUtils::validateOptions(table)), int(HelperUtils::LaunchError::NoWordlist));

    table.wordlists = one;
    QCOMPARE(int(HelperUtils::validateOptions(table)), int(HelperUtils::LaunchError::None));

    table.wordlists = two;
    QCOMPARE(int(HelperUtils::validateOptions(table)),
             int(HelperUtils::LaunchError::TooManyWordlists));

    table.wordlists = one;
    table.tableFiles.clear();
    QCOMPARE(int(HelperUtils::validateOptions(table)), int(HelperUtils::LaunchError::NoTable));

    // Mode 8 needs a feed name. The argument count is the feed's own
    // business - random and stdin take none, wordlist needs a path - so a
    // blanket check here would block the feeds that need no argument.
    HashcatOptions feed;
    feed.attackMode = AttackMode::Generic;
    QCOMPARE(int(HelperUtils::validateOptions(feed)), int(HelperUtils::LaunchError::NoFeed));

    feed.feedName = QStringLiteral("random");
    QCOMPARE(int(HelperUtils::validateOptions(feed)), int(HelperUtils::LaunchError::None));

    feed.feedName = QStringLiteral("wordlist");
    feed.feedArguments = one;
    QCOMPARE(int(HelperUtils::validateOptions(feed)), int(HelperUtils::LaunchError::None));

    // Mode 9 pairs at most one wordlist in this GUI.
    HashcatOptions association;
    association.attackMode = AttackMode::Association;
    association.wordlists = two;
    QCOMPARE(int(HelperUtils::validateOptions(association)),
             int(HelperUtils::LaunchError::TooManyWordlists));

    // Mode 12 takes the two wordlists its markers name; a third is refused
    // before the markers are looked at.
    HashcatOptions hybrid;
    hybrid.attackMode = AttackMode::Hybrid;
    hybrid.mask = QStringLiteral("?d?w?d?q");
    hybrid.wordlists = two;
    QCOMPARE(int(HelperUtils::validateOptions(hybrid)), int(HelperUtils::LaunchError::None));

    hybrid.wordlists << QStringLiteral("/tmp/c.txt");
    QCOMPARE(int(HelperUtils::validateOptions(hybrid)),
             int(HelperUtils::LaunchError::TooManyWordlists));
}

// The mode 4 settings: the feed parses them only after the backend devices
// are set up, and maxgain is never checked upstream at all.
void TestHelperUtils::pcfgSettingsAreValidated()
{
    const auto expect = [](const HashcatOptions &options,
                           HelperUtils::LaunchError expected) -> void {
        QString detail(QStringLiteral("stale"));
        QCOMPARE(int(HelperUtils::validateOptions(options, &detail)), int(expected));
        // The same has to hold when the caller does not care about the detail
        QCOMPARE(int(HelperUtils::validateOptions(options, nullptr)), int(expected));
    };

    HashcatOptions clean;
    clean.attackMode = AttackMode::Pcfg;
    clean.pcfgRulesets =
        QStringList{QStringLiteral("/tmp/one.pcfg"), QStringLiteral("/tmp/two.pcfg")};
    clean.pcfgScale = QStringLiteral("2");
    clean.pcfgCostmax = QStringLiteral("40");
    clean.pcfgWeights = QStringLiteral("2:1");
    clean.pcfgMask = QStringLiteral("?u?l?l?l?l?d?d?d");
    clean.pcfgExtraSettings = QStringList{QStringLiteral("omen=0"), QStringLiteral("pwmin=8")};
    expect(clean, HelperUtils::LaunchError::None);

    struct Case {
        QString HashcatOptions::*field;
        QString value;
        QString reported; // the detail: the setting as the feed sees it
    };
    const Case typedCases[]{
        {&HashcatOptions::pcfgScale, QStringLiteral("0"), QStringLiteral("scale=0")},
        {&HashcatOptions::pcfgScale, QStringLiteral("65"), QStringLiteral("scale=65")},
        {&HashcatOptions::pcfgCostmax, QStringLiteral("0"), QStringLiteral("costmax=0")},
        {&HashcatOptions::pcfgCostmax, QStringLiteral("65"), QStringLiteral("costmax=65")},
        {&HashcatOptions::pcfgThreads, QStringLiteral("17"), QStringLiteral("threads=17")},
        {&HashcatOptions::pcfgKbits, QStringLiteral("28"), QStringLiteral("kbits=28")},
        {&HashcatOptions::pcfgWalk, QStringLiteral("2"), QStringLiteral("walk=2")},
        // maxword is 0, 16 or 32.
        {&HashcatOptions::pcfgMaxword, QStringLiteral("15"), QStringLiteral("maxword=15")},
        // maxgain's declared 1.0-64.0 range is the one check upstream never
        // makes, so this one exists only here.
        {&HashcatOptions::pcfgMaxgain, QStringLiteral("0.5"), QStringLiteral("maxgain=0.5")},
        {&HashcatOptions::pcfgMaxgain, QStringLiteral("65"), QStringLiteral("maxgain=65")},
    };
    for (const Case &bad : typedCases) {
        HashcatOptions options = clean;
        options.*bad.field = bad.value;
        QString detail;
        QCOMPARE(int(HelperUtils::validateOptions(options, &detail)),
                 int(HelperUtils::LaunchError::InvalidPcfgSetting));
        QCOMPARE(detail, bad.reported);
    }

    // weights: a plain colon separated list of values above zero, one per
    // ruleset - an empty list is the one bundled default.
    const QStringList weightCases{QStringLiteral("0"), QStringLiteral("a:b"),
                                  QStringLiteral("1:1")};
    for (const QString &weights : weightCases) {
        HashcatOptions options = clean;
        options.pcfgRulesets.clear();
        options.pcfgWeights = weights;
        QCOMPARE(int(HelperUtils::validateOptions(options)),
                 int(HelperUtils::LaunchError::InvalidPcfgSetting));
    }

    // The grammar held to the mask refuses the pair with omen=1, and the
    // hint words from the accounts have no source while the mask is on.
    const QStringList maskConflicts{QStringLiteral("omen=1"), QStringLiteral("hintaccount=8")};
    for (const QString &token : maskConflicts) {
        HashcatOptions options = clean;
        options.pcfgExtraSettings = QStringList{token};
        QCOMPARE(int(HelperUtils::validateOptions(options)),
                 int(HelperUtils::LaunchError::InvalidPcfgSetting));
    }

    // A mask with the hybrid markers is mode 12's rewriting, not a grammar.
    HashcatOptions markers = clean;
    markers.pcfgMask = QStringLiteral("?u?w");
    expect(markers, HelperUtils::LaunchError::InvalidPcfgSetting);

    // The conflicts only count with a mask, and a named source (or
    // hintaccount=0) is no conflict at all.
    HashcatOptions hintwords = clean;
    hintwords.pcfgExtraSettings =
        QStringList{QStringLiteral("hintaccount=8"), QStringLiteral("hintwords=w")};
    expect(hintwords, HelperUtils::LaunchError::None);

    HashcatOptions noHint = clean;
    noHint.pcfgExtraSettings = QStringList{QStringLiteral("hintaccount=0")};
    expect(noHint, HelperUtils::LaunchError::None);

    HashcatOptions noMask = clean;
    noMask.pcfgMask.clear();
    noMask.pcfgExtraSettings = QStringList{QStringLiteral("omen=1")};
    expect(noMask, HelperUtils::LaunchError::None);

    // A bare token before the first setting would be read as another
    // ruleset.
    HashcatOptions bare = clean;
    bare.pcfgExtraSettings = QStringList{QStringLiteral("pwmin")};
    QString detail;
    QCOMPARE(int(HelperUtils::validateOptions(bare, &detail)),
             int(HelperUtils::LaunchError::InvalidPcfgSetting));
    QCOMPARE(detail, QStringLiteral("pwmin"));

    // The GUI can create a repeated key itself: a typed field plus an extra
    // token. Every feed refuses one, but only after device init.
    HashcatOptions repeated = clean;
    repeated.pcfgExtraSettings = QStringList{QStringLiteral("scale=3")};
    QCOMPARE(int(HelperUtils::validateOptions(repeated, &detail)),
             int(HelperUtils::LaunchError::InvalidPcfgSetting));
    QCOMPARE(detail, QStringLiteral("scale: given more than once"));
}

// The mode 5 settings, for the same late-failure reason as the mode 4 ones.
void TestHelperUtils::tableSettingsAreValidated()
{
    HashcatOptions clean;
    clean.attackMode = AttackMode::Table;
    clean.wordlists = QStringList{QStringLiteral("/tmp/a.txt")};
    clean.tableFiles = QStringList{QStringLiteral("/tmp/table.txt")};
    clean.tableIdentity = QStringLiteral("1");
    clean.tableTemplate = QStringLiteral("0");
    clean.tableSingle = QStringLiteral("0");
    clean.tableCap = QStringLiteral("0");
    clean.tableMaxperm = QStringLiteral("1048576");
    QCOMPARE(int(HelperUtils::validateOptions(clean)), int(HelperUtils::LaunchError::None));

    // The three flags: empty (the default), 0 or 1.
    HashcatOptions flag = clean;
    flag.tableTemplate = QStringLiteral("2");
    QString detail;
    QCOMPARE(int(HelperUtils::validateOptions(flag, &detail)),
             int(HelperUtils::LaunchError::InvalidTableSetting));
    QCOMPARE(detail, QStringLiteral("template=2"));

    // One substitution needs the other positions left alone.
    HashcatOptions substitution = clean;
    substitution.tableSingle = QStringLiteral("1");
    substitution.tableIdentity = QStringLiteral("0");
    QCOMPARE(int(HelperUtils::validateOptions(substitution, &detail)),
             int(HelperUtils::LaunchError::InvalidTableSetting));
    QCOMPARE(detail, QStringLiteral("identity=0"));

    // cap and maxperm reach beyond every Qt integer type, so their bounds
    // are checked here, not by a spin box.
    HashcatOptions cap = clean;
    cap.tableCap = QStringLiteral("4294967296");
    QCOMPARE(int(HelperUtils::validateOptions(cap, &detail)),
             int(HelperUtils::LaunchError::InvalidTableSetting));
    QCOMPARE(detail, QStringLiteral("cap=4294967296"));

    HashcatOptions maxperm = clean;
    maxperm.tableMaxperm = QStringLiteral("18446744073709551616"); // 2^64
    QCOMPARE(int(HelperUtils::validateOptions(maxperm, &detail)),
             int(HelperUtils::LaunchError::InvalidTableSetting));
    QCOMPARE(detail, QStringLiteral("maxperm=18446744073709551616"));

    // A bare extra token would be read as another table.
    HashcatOptions bare = clean;
    bare.tableExtraSettings = QStringList{QStringLiteral("cap")};
    QCOMPARE(int(HelperUtils::validateOptions(bare, &detail)),
             int(HelperUtils::LaunchError::InvalidTableSetting));
    QCOMPARE(detail, QStringLiteral("cap"));

    // The typed field plus an extra token: the same repeated key the feed
    // refuses only after device init.
    HashcatOptions repeated = clean;
    repeated.tableExtraSettings = QStringList{QStringLiteral("cap=5")};
    QCOMPARE(int(HelperUtils::validateOptions(repeated, &detail)),
             int(HelperUtils::LaunchError::InvalidTableSetting));
    QCOMPARE(detail, QStringLiteral("cap: given more than once"));

    // A full run with the extra line.
    HashcatOptions full = clean;
    full.tableExtraSettings = QStringList{QStringLiteral("cache=1"), QStringLiteral("pwmax=64")};
    QCOMPARE(int(HelperUtils::validateOptions(full)), int(HelperUtils::LaunchError::None));
}

// The mode 9 settings line: the two forms are alternatives, and a token
// that is not a setting would become a wordlist upstream.
void TestHelperUtils::associationSettingsAreValidated()
{
    const QStringList valid{QStringLiteral("phases=words,rules"),
                            QStringLiteral("rules.rulemax=300")};

    HashcatOptions clean;
    clean.attackMode = AttackMode::Association;
    clean.associationSettings = valid;
    QCOMPARE(int(HelperUtils::validateOptions(clean)), int(HelperUtils::LaunchError::None));

    // The shape is about the key only: hashcat reports the missing phase
    // itself, once the session starts.
    HashcatOptions emptyPhase;
    emptyPhase.attackMode = AttackMode::Association;
    emptyPhase.associationSettings = QStringList{QStringLiteral("phases=")};
    QCOMPARE(int(HelperUtils::validateOptions(emptyPhase)), int(HelperUtils::LaunchError::None));

    // A token that fails the shape is not refused at all upstream: it would
    // either fail as a path naming the setting, or silently add a second
    // wordlist. This check is the one hashcat cannot make for us.
    const QStringList malformed{QStringLiteral("phases words,rules"), QStringLiteral("=x"),
                                QStringLiteral("9=x"), QStringLiteral("-x=1")};
    for (const QString &token : malformed) {
        HashcatOptions options;
        options.attackMode = AttackMode::Association;
        options.associationSettings = QStringList{token};
        QString detail;
        QCOMPARE(int(HelperUtils::validateOptions(options, &detail)),
                 int(HelperUtils::LaunchError::InvalidAssociationSetting));
        QCOMPARE(detail, token);
    }

    // With a wordlist the two files are paired by line and no setting is
    // accepted; the combination is reported before the shape.
    HashcatOptions both;
    both.attackMode = AttackMode::Association;
    both.wordlists = QStringList{QStringLiteral("/tmp/a.txt")};
    both.associationSettings = QStringList{QStringLiteral("phases words,rules")};
    QCOMPARE(int(HelperUtils::validateOptions(both)),
             int(HelperUtils::LaunchError::AssociationSettingsWithWordlist));
}

// The mode 12 mask markers, and the markers in the modes that rewrite them.
void TestHelperUtils::maskMarkersAreValidated()
{
    const QStringList one{QStringLiteral("/tmp/a.txt")};
    const QStringList two{QStringLiteral("/tmp/a.txt"), QStringLiteral("/tmp/b.txt")};

    const auto hybridError = [&one, &two](const QString &mask, const QStringList &wordlists) {
        HashcatOptions options;
        options.attackMode = AttackMode::Hybrid;
        options.mask = mask;
        options.wordlists = wordlists;
        return int(HelperUtils::validateOptions(options));
    };

    QCOMPARE(hybridError(QStringLiteral("?d?w?d"), one), int(HelperUtils::LaunchError::None));
    QCOMPARE(hybridError(QStringLiteral("?w?q"), two), int(HelperUtils::LaunchError::None));
    // An escaped marker pair stands for the literal characters.
    QCOMPARE(hybridError(QStringLiteral("??w"), one),
             int(HelperUtils::LaunchError::MaskNeedsWordMarker));

    QCOMPARE(hybridError(QStringLiteral("?d?d?d"), one),
             int(HelperUtils::LaunchError::MaskNeedsWordMarker));
    QCOMPARE(hybridError(QStringLiteral("?w?w"), one),
             int(HelperUtils::LaunchError::MaskMultipleWordMarkers));
    QCOMPARE(hybridError(QStringLiteral("?w?q?q"), one),
             int(HelperUtils::LaunchError::MaskMultipleQMarks));
    QCOMPARE(hybridError(QStringLiteral("?q?w"), one),
             int(HelperUtils::LaunchError::MaskQBeforeWord));
    // The markers have to line up with what is selected.
    QCOMPARE(hybridError(QStringLiteral("?w?q"), one),
             int(HelperUtils::LaunchError::MaskQWithoutWordlist));
    QCOMPARE(hybridError(QStringLiteral("?w"), two),
             int(HelperUtils::LaunchError::SecondWordlistWithoutQ));

    // The increment is refused in mode 12 as well: the widget is hidden
    // there and collect drops the flag, so this is defence in depth.
    HashcatOptions increment;
    increment.attackMode = AttackMode::Hybrid;
    increment.mask = QStringLiteral("?w");
    increment.wordlists = one;
    increment.increment = true;
    QCOMPARE(int(HelperUtils::validateOptions(increment)),
             int(HelperUtils::LaunchError::IncrementInMode12));

    // The other mask modes would take the markers for their literal ?w/?q
    // or rewrite them.
    HashcatOptions brute;
    brute.attackMode = AttackMode::BruteForce;
    brute.mask = QStringLiteral("?w?d");
    QCOMPARE(int(HelperUtils::validateOptions(brute)),
             int(HelperUtils::LaunchError::MaskMarkersNotAllowed));

    HashcatOptions hybridWord;
    hybridWord.attackMode = AttackMode::HybridWordMask;
    hybridWord.wordlists = one;
    hybridWord.mask = QStringLiteral("?d?q");
    QCOMPARE(int(HelperUtils::validateOptions(hybridWord)),
             int(HelperUtils::LaunchError::MaskMarkersNotAllowed));

    // A mask without the markers is fine in every mode.
    brute.mask = QStringLiteral("?d?d");
    QCOMPARE(int(HelperUtils::validateOptions(brute)), int(HelperUtils::LaunchError::None));
}

// Hash mode 4000 describes its hashes as a recipe; without one hashcat
// refuses the run, and the row that takes it only exists for this type.
void TestHelperUtils::hashRecipeIsRequiredFor4000()
{
    HashcatOptions options;
    options.attackMode = AttackMode::Straight;
    options.hashType = AppConstants::Hashcat::RecipeHashType;
    options.wordlists = QStringList{QStringLiteral("/tmp/a.txt")};
    QCOMPARE(int(HelperUtils::validateOptions(options)),
             int(HelperUtils::LaunchError::NoHashRecipe));

    options.hashRecipe = QStringLiteral("md5(md5(salt) . pass)");
    QCOMPARE(int(HelperUtils::validateOptions(options)), int(HelperUtils::LaunchError::None));

    // A stale recipe next to any other hash type is left out by the builder
    // and must not keep the run from starting.
    options.hashType = 1000;
    QCOMPARE(int(HelperUtils::validateOptions(options)), int(HelperUtils::LaunchError::None));
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
