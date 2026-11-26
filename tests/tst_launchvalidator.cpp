/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appconstants.h"
#include "launchvalidator.h"
#include "testenvironment.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTemporaryDir>
#include <array>

using TestEnvironment::validateOptions;

namespace
{
// The launch checks on their own: the options hold values that pass.
LaunchValidator::LaunchProblem validateLaunch(const QString &hashFile, const QString &hashcatPath,
                                              const QString &resolvedHashcatPath,
                                              const QString &configuredTerminal,
                                              const QStringList &availableTerminals)
{
    LaunchValidator::LaunchContext context;
    context.hashcatPath = hashcatPath;
    context.resolvedHashcatPath = resolvedHashcatPath;
    context.configuredTerminal = configuredTerminal;
    context.availableTerminals = availableTerminals;
    context.options.hashFile = hashFile;
    context.options.attackMode = AttackMode::Straight;
    context.options.wordlists = QStringList{QStringLiteral("/tmp/a.txt")};
    return LaunchValidator::validate(context);
}

// The detail read separately, for the tests that pin it apart from the code.
LaunchValidator::LaunchError validateOptionsWithDetail(const HashcatOptions &options,
                                                       QString *detail)
{
    const LaunchValidator::LaunchProblem problem = validateOptions(options);
    if (detail) {
        *detail = problem.detail;
    }
    return problem.code;
}

// The option checks against an explicit program, for the paths that hashcat
// reads relative to its own directory.
LaunchValidator::LaunchProblem validateOptionsAt(const HashcatOptions &options,
                                                 const QString &resolvedHashcatPath)
{
    LaunchValidator::LaunchContext context;
    context.options = options;
    if (context.options.hashFile.isEmpty()) {
        context.options.hashFile = QStringLiteral("hash.txt");
    }
    context.hashcatPath = resolvedHashcatPath;
    context.resolvedHashcatPath = resolvedHashcatPath;
    context.configuredTerminal = QStringLiteral("konsole");
    context.availableTerminals = QStringList{QStringLiteral("konsole")};
    return LaunchValidator::validate(context);
}
} // namespace

// The conditions that keep Execute from starting anything: the launch and
// option validation, including the feed settings hashcat only parses itself.
class TestLaunchValidator : public QObject
{
    Q_OBJECT

private slots:
    void validateLaunch_data();
    void validateLaunch();
    void validateOptions_data();
    void validateOptions();
    void optionConflictsAreRefused();
    void feedModeRequirements();
    void amplifierFolderIsRefused();
    void pcfgRulesetsAreBounded();
    void pcfgSettingsAreValidated();
    void tableSettingsAreValidated();
    void associationSettingsAreValidated();
    void maskMarkersAreValidated();
    void hashRecipeIsRequiredFor4000();
    void cachePathNeedsAnExistingDirectory();
    void relativePathsResolveAgainstTheProgramDirectory();
    void sessionSuppliedValuesAreRevalidated();
    void pathCollisionsAreRefused();
    void controlCharactersAreRefused();
    void outfileFormatIsValidated();
};

void TestLaunchValidator::validateLaunch_data()
{
    QTest::addColumn<QString>("hashFile");
    QTest::addColumn<QString>("hashcatPath");
    QTest::addColumn<QString>("resolvedHashcatPath");
    QTest::addColumn<QString>("configuredTerminal");
    QTest::addColumn<QStringList>("availableTerminals");
    QTest::addColumn<int>("expected");
    QTest::addColumn<QString>("expectedDetail");

    const QString gone =
        QDir(QDir::tempPath()).filePath(QStringLiteral("hashcat-gui-test-no-such-binary"));
    const QString resolved = QStringLiteral("/usr/bin/hashcat");

    const QStringList terminals{"konsole", "kitty"};

    QTest::newRow("no hash file") << QString() << QStringLiteral("/usr/bin/hashcat") << resolved
                                  << QStringLiteral("konsole") << terminals
                                  << int(LaunchValidator::LaunchError::NoHashFile) << QString();

    QTest::newRow("hashcat never configured")
        << QStringLiteral("hash.txt") << QString() << QString() << QStringLiteral("konsole")
        << terminals << int(LaunchValidator::LaunchError::NoHashcatPath) << QString();

    QTest::newRow("hashcat is gone")
        << QStringLiteral("hash.txt") << gone << QString() << QStringLiteral("konsole") << terminals
        << int(LaunchValidator::LaunchError::HashcatPathMissing) << gone;

    QTest::newRow("bare name off PATH")
        << QStringLiteral("hash.txt") << QStringLiteral("hashcat-gui-no-such-binary") << QString()
        << QStringLiteral("konsole") << terminals
        << int(LaunchValidator::LaunchError::HashcatPathMissing)
        << QStringLiteral("hashcat-gui-no-such-binary");

    // An existing path that is not a file (the temp directory is one).
    QTest::newRow("not an executable")
        << QStringLiteral("hash.txt") << QDir::tempPath() << QString() << QStringLiteral("konsole")
        << terminals << int(LaunchValidator::LaunchError::HashcatNotExecutable) << QDir::tempPath();

    // A caller that hands over a non-absolute program is refused too.
    QTest::newRow("resolved path is not absolute")
        << QStringLiteral("hash.txt") << QStringLiteral("hashcat") << QStringLiteral("hashcat")
        << QStringLiteral("konsole") << terminals
        << int(LaunchValidator::LaunchError::HashcatNotExecutable) << QStringLiteral("hashcat");

    QTest::newRow("no terminal selected")
        << QStringLiteral("hash.txt") << QStringLiteral("hashcat") << resolved << QString()
        << terminals << int(LaunchValidator::LaunchError::NoTerminal) << QString();

    QTest::newRow("no terminal installed")
        << QStringLiteral("hash.txt") << QStringLiteral("hashcat") << resolved
        << QStringLiteral("konsole") << QStringList()
        << int(LaunchValidator::LaunchError::NoTerminals) << QString();

    QTest::newRow("configured terminal is not installed")
        << QStringLiteral("hash.txt") << QStringLiteral("hashcat") << resolved
        << QStringLiteral("yakuake") << terminals
        << int(LaunchValidator::LaunchError::UnknownTerminal) << QStringLiteral("konsole, kitty");

    QTest::newRow("ready to go") << QStringLiteral("hash.txt") << QStringLiteral("hashcat")
                                 << resolved << QStringLiteral("konsole") << terminals
                                 << int(LaunchValidator::LaunchError::None) << QString();
}

void TestLaunchValidator::validateLaunch()
{
    QFETCH(QString, hashFile);
    QFETCH(QString, hashcatPath);
    QFETCH(QString, resolvedHashcatPath);
    QFETCH(QString, configuredTerminal);
    QFETCH(QStringList, availableTerminals);
    QFETCH(int, expected);
    QFETCH(QString, expectedDetail);

    const LaunchValidator::LaunchProblem problem = ::validateLaunch(
        hashFile, hashcatPath, resolvedHashcatPath, configuredTerminal, availableTerminals);

    QCOMPARE(int(problem.code), expected);
    QCOMPARE(problem.detail, expectedDetail);

    if (expected == int(LaunchValidator::LaunchError::HashcatPathMissing)) {
        QVERIFY2(!QFileInfo::exists(hashcatPath), "the row needs a path that does not exist");
    }
    if (expected == int(LaunchValidator::LaunchError::HashcatNotExecutable)) {
        QVERIFY2(resolvedHashcatPath.isEmpty() || !QDir::isAbsolutePath(resolvedHashcatPath)
                     || QFileInfo::exists(hashcatPath),
                 "the row needs an unresolved or existing path");
    }
}

// The checks that keep an attack that hashcat would refuse from starting.
void TestLaunchValidator::validateOptions_data()
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
        << int(LaunchValidator::LaunchError::None) << QString();

    QTest::newRow("straight without a word list")
        << int(AttackMode::Straight) << QStringList() << QString() << false << 0 << 0 << false
        << false << int(LaunchValidator::LaunchError::NoWordlist) << QString();

    QTest::newRow("straight with several word lists")
        << int(AttackMode::Straight) << three << QString() << false << 0 << 0 << false << false
        << int(LaunchValidator::LaunchError::None) << QString();

    QTest::newRow("combination with two word lists")
        << int(AttackMode::Combination) << two << QString() << false << 0 << 0 << false << false
        << int(LaunchValidator::LaunchError::None) << QString();

    QTest::newRow("combination with one word list")
        << int(AttackMode::Combination) << one << QString() << false << 0 << 0 << false << false
        << int(LaunchValidator::LaunchError::WrongWordlistCount) << QStringLiteral("1");

    QTest::newRow("combination with three word lists")
        << int(AttackMode::Combination) << three << QString() << false << 0 << 0 << false << false
        << int(LaunchValidator::LaunchError::WrongWordlistCount) << QStringLiteral("3");

    QTest::newRow("brute force with a mask")
        << int(AttackMode::BruteForce) << QStringList() << QStringLiteral("?a?a?a?a") << false << 0
        << 0 << false << false << int(LaunchValidator::LaunchError::None) << QString();

    QTest::newRow("brute force without a mask")
        << int(AttackMode::BruteForce) << QStringList() << QString() << false << 0 << 0 << false
        << false << int(LaunchValidator::LaunchError::NoMask) << QString();

    // Word lists do not matter in brute force mode: they are not part of the
    // command line there.
    QTest::newRow("brute force ignores the word lists")
        << int(AttackMode::BruteForce) << three << QStringLiteral("?a?a?a?a") << false << 0 << 0
        << false << false << int(LaunchValidator::LaunchError::None) << QString();

    QTest::newRow("hybrid word + mask needs both")
        << int(AttackMode::HybridWordMask) << one << QString() << false << 0 << 0 << false << false
        << int(LaunchValidator::LaunchError::NoMask) << QString();

    QTest::newRow("hybrid mask + word needs a word list")
        << int(AttackMode::HybridMaskWord) << QStringList() << QStringLiteral("?a?a?a?a") << false
        << 0 << 0 << false << false << int(LaunchValidator::LaunchError::NoWordlist) << QString();

    // Hashcat 8.0.0 reads the words of an association attack from the hash
    // file: without a wordlist the mode runs, and the settings line names the
    // phases to run.
    QTest::newRow("association without a word list")
        << int(AttackMode::Association) << QStringList() << QString() << false << 0 << 0 << false
        << false << int(LaunchValidator::LaunchError::None) << QString();

    QTest::newRow("increment bounds that fit")
        << int(AttackMode::BruteForce) << QStringList() << QStringLiteral("?a?a?a?a") << true << 4
        << 8 << false << false << int(LaunchValidator::LaunchError::None) << QString();

    QTest::newRow("increment minimum above the maximum")
        << int(AttackMode::BruteForce) << QStringList() << QStringLiteral("?a?a?a?a") << true << 8
        << 4 << false << false << int(LaunchValidator::LaunchError::InvalidIncrement) << QString();

    // Without the flag the bounds are not used at all, so they cannot
    // contradict each other either.
    QTest::newRow("increment bounds without the flag")
        << int(AttackMode::BruteForce) << QStringList() << QStringLiteral("?a?a?a?a") << false << 8
        << 4 << false << false << int(LaunchValidator::LaunchError::None) << QString();

    // Only the mask modes put -i on the command line, so stale bounds in the
    // other modes must not keep a run from starting.
    QTest::newRow("straight ignores stale increment bounds")
        << int(AttackMode::Straight) << one << QString() << true << 8 << 4 << false << false
        << int(LaunchValidator::LaunchError::None) << QString();

    QTest::newRow("combination ignores stale increment bounds")
        << int(AttackMode::Combination) << two << QString() << true << 8 << 4 << false << false
        << int(LaunchValidator::LaunchError::None) << QString();

    QTest::newRow("association ignores stale increment bounds")
        << int(AttackMode::Association) << one << QString() << true << 8 << 4 << false << false
        << int(LaunchValidator::LaunchError::None) << QString();

    // hashcat refuses -i together with --speed-only, but only the mask modes
    // emit -i in the first place.
    QTest::newRow("increment with speed only")
        << int(AttackMode::BruteForce) << QStringList() << QStringLiteral("?a?a?a?a") << true << 4
        << 8 << true << false << int(LaunchValidator::LaunchError::IncrementWithSpeedOnly)
        << QString();

    QTest::newRow("speed only without the increment")
        << int(AttackMode::BruteForce) << QStringList() << QStringLiteral("?a?a?a?a") << false << 0
        << 0 << true << false << int(LaunchValidator::LaunchError::None) << QString();

    // The bounds are checked first, so both being wrong reports the bounds.
    QTest::newRow("increment bounds win over speed only")
        << int(AttackMode::BruteForce) << QStringList() << QStringLiteral("?a?a?a?a") << true << 8
        << 4 << true << false << int(LaunchValidator::LaunchError::InvalidIncrement) << QString();

    QTest::newRow("straight ignores stale increment with speed only")
        << int(AttackMode::Straight) << one << QString() << true << 8 << 4 << true << false
        << int(LaunchValidator::LaunchError::None) << QString();

    // --remove edits the hash file, so hashcat refuses it in association mode.
    QTest::newRow("remove in association mode")
        << int(AttackMode::Association) << one << QString() << false << 0 << 0 << false << true
        << int(LaunchValidator::LaunchError::RemoveInAssociation) << QString();

    QTest::newRow("remove in straight mode")
        << int(AttackMode::Straight) << one << QString() << false << 0 << 0 << false << true
        << int(LaunchValidator::LaunchError::None) << QString();

    // The mode requirements are reported before the option conflicts.
    QTest::newRow("association without a word list and remove")
        << int(AttackMode::Association) << QStringList() << QString() << false << 0 << 0 << false
        << true << int(LaunchValidator::LaunchError::RemoveInAssociation) << QString();

    // The GUI caps the pairing at one wordlist; hashcat reads two, but the
    // list offers no second role for a second list. The mode has a range
    // (0 or 1), so the error is the at-most one, not the exact count.
    QTest::newRow("association with two word lists")
        << int(AttackMode::Association) << two << QString() << false << 0 << 0 << false << false
        << int(LaunchValidator::LaunchError::TooManyWordlists) << QStringLiteral("2");
}

void TestLaunchValidator::validateOptions()
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

    const LaunchValidator::LaunchProblem problem = ::validateOptions(options);

    QCOMPARE(int(problem.code), expected);
    QCOMPARE(problem.detail, expectedDetail);
}

// The option pair hashcat answers with a usage error: keep-guessing with
// remove. The potfile pair resolves in the builder, not here.
void TestLaunchValidator::optionConflictsAreRefused()
{
    HashcatOptions options;
    options.attackMode = AttackMode::Straight;
    options.wordlists = QStringList{QStringLiteral("/tmp/a.txt")};

    options.keepGuessing = true;
    options.remove = true;
    QCOMPARE(::validateOptions(options).code, LaunchValidator::LaunchError::KeepGuessingWithRemove);

    options.remove = false;
    QCOMPARE(::validateOptions(options).code, LaunchValidator::LaunchError::None);

    // The mode-9 refusal of --remove is reported before the pair conflict.
    options.attackMode = AttackMode::Association;
    options.remove = true;
    QCOMPARE(::validateOptions(options).code, LaunchValidator::LaunchError::RemoveInAssociation);

    // The builder drops the path under the disable box, so the pair is no
    // conflict.
    options.attackMode = AttackMode::Straight;
    options.keepGuessing = false;
    options.remove = false;
    options.potfilePath = QStringLiteral("/tmp/custom.pot");
    options.potfileDisable = true;
    QCOMPARE(::validateOptions(options).code, LaunchValidator::LaunchError::None);
}

// What the feed modes owe beyond the shared checks: the table a mode 5 run
// reads, the feed name of mode 8 and the wordlist caps.
void TestLaunchValidator::feedModeRequirements()
{
    const QStringList one{QStringLiteral("/tmp/a.txt")};
    const QStringList two{QStringLiteral("/tmp/a.txt"), QStringLiteral("/tmp/b.txt")};

    // Mode 5 takes exactly one wordlist: none is a missing list, two is one
    // too many - and without a table there is nothing to run.
    HashcatOptions table;
    table.attackMode = AttackMode::Table;
    table.tableFiles = QStringList{QStringLiteral("/tmp/table.txt")};
    QCOMPARE(int(::validateOptions(table).code), int(LaunchValidator::LaunchError::NoWordlist));

    table.wordlists = one;
    QCOMPARE(int(::validateOptions(table).code), int(LaunchValidator::LaunchError::None));

    table.wordlists = two;
    QCOMPARE(int(::validateOptions(table).code),
             int(LaunchValidator::LaunchError::TooManyWordlists));

    table.wordlists = one;
    table.tableFiles.clear();
    QCOMPARE(int(::validateOptions(table).code), int(LaunchValidator::LaunchError::NoTable));

    // Mode 8 needs a feed name. The argument count is the feed's own
    // business - random and stdin take none, wordlist needs a path - so a
    // blanket check here would block the feeds that need no argument.
    HashcatOptions feed;
    feed.attackMode = AttackMode::Generic;
    QCOMPARE(int(::validateOptions(feed).code), int(LaunchValidator::LaunchError::NoFeed));

    feed.feedName = QStringLiteral("random");
    QCOMPARE(int(::validateOptions(feed).code), int(LaunchValidator::LaunchError::None));

    feed.feedName = QStringLiteral("wordlist");
    feed.feedArguments = one;
    QCOMPARE(int(::validateOptions(feed).code), int(LaunchValidator::LaunchError::None));

    // Mode 9 pairs at most one wordlist in this GUI.
    HashcatOptions association;
    association.attackMode = AttackMode::Association;
    association.wordlists = two;
    QCOMPARE(int(::validateOptions(association).code),
             int(LaunchValidator::LaunchError::TooManyWordlists));

    // Mode 12 takes the two wordlists its markers name; a third is refused
    // before the markers are looked at.
    HashcatOptions hybrid;
    hybrid.attackMode = AttackMode::Hybrid;
    hybrid.mask = QStringLiteral("?d?w?d?q");
    hybrid.wordlists = two;
    QCOMPARE(int(::validateOptions(hybrid).code), int(LaunchValidator::LaunchError::None));

    hybrid.wordlists << QStringLiteral("/tmp/c.txt");
    QCOMPARE(int(::validateOptions(hybrid).code),
             int(LaunchValidator::LaunchError::TooManyWordlists));
}

// A folder is a base wordlist; the list hashcat multiplies is read by one feed
// instance and has to be a file. Which row that is follows from the mode, so a
// folder in the wrong slot is refused before the run starts.
void TestLaunchValidator::amplifierFolderIsRefused()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString folder = directory.filePath(QStringLiteral("wordlists"));
    QVERIFY(QDir().mkpath(folder));
    const QString file = directory.filePath(QStringLiteral("words.txt"));
    QFile words(file);
    QVERIFY(words.open(QIODevice::WriteOnly));
    QCOMPARE(words.write("password\n"), qint64(9));
    words.close();

    const auto expect = [](const HashcatOptions &options, LaunchValidator::LaunchError expected,
                           const QString &expectedDetail = QString()) -> void {
        const LaunchValidator::LaunchProblem problem = ::validateOptions(options);
        QCOMPARE(int(problem.code), int(expected));
        QCOMPARE(problem.detail, expectedDetail);
    };

    // Combination: the first list is the base, the second is multiplied.
    HashcatOptions combination;
    combination.attackMode = AttackMode::Combination;
    combination.wordlists = {folder, file};
    expect(combination, LaunchValidator::LaunchError::None);

    combination.wordlists = {file, folder};
    expect(combination, LaunchValidator::LaunchError::WordlistMustBeFile, folder);

    // Mode 12: the same split, the ?q row is the second one.
    HashcatOptions hybrid;
    hybrid.attackMode = AttackMode::Hybrid;
    hybrid.mask = QStringLiteral("?w?q");
    hybrid.wordlists = {folder, file};
    expect(hybrid, LaunchValidator::LaunchError::None);

    hybrid.wordlists = {file, folder};
    expect(hybrid, LaunchValidator::LaunchError::WordlistMustBeFile, folder);

    // Mode 7 spells its mask first, but hashcat reads the dictionary as the
    // base, so a folder is fine there.
    HashcatOptions maskWord;
    maskWord.attackMode = AttackMode::HybridMaskWord;
    maskWord.mask = QStringLiteral("?d");
    maskWord.wordlists = {folder};
    expect(maskWord, LaunchValidator::LaunchError::None);

    // The other wordlist modes take a folder as well.
    HashcatOptions straight;
    straight.attackMode = AttackMode::Straight;
    straight.wordlists = {folder};
    expect(straight, LaunchValidator::LaunchError::None);

    HashcatOptions table;
    table.attackMode = AttackMode::Table;
    table.wordlists = {folder};
    table.tableFiles = {file};
    expect(table, LaunchValidator::LaunchError::None);

    HashcatOptions wordMask;
    wordMask.attackMode = AttackMode::HybridWordMask;
    wordMask.mask = QStringLiteral("?d?d");
    wordMask.wordlists = {folder};
    expect(wordMask, LaunchValidator::LaunchError::None);

    HashcatOptions association;
    association.attackMode = AttackMode::Association;
    association.wordlists = {folder};
    expect(association, LaunchValidator::LaunchError::None);

    // An amplifier row that is missing altogether is the count check's
    // business, not this one.
    HashcatOptions oneForCombination;
    oneForCombination.attackMode = AttackMode::Combination;
    oneForCombination.wordlists = {folder};
    expect(oneForCombination, LaunchValidator::LaunchError::WrongWordlistCount,
           QStringLiteral("1"));
}

// The feed reads at most 16 rulesets (PCFG_MAXROOT) and refuses more only
// after backend device init - the late failure the validator exists for.
void TestLaunchValidator::pcfgRulesetsAreBounded()
{
    HashcatOptions options;
    options.attackMode = AttackMode::Pcfg;
    for (int i = 0; i < AppConstants::Hashcat::MaxPcfgRulesets; ++i) {
        options.pcfgRulesets << QStringLiteral("/tmp/ruleset%1.pcfg").arg(i);
    }
    QCOMPARE(int(::validateOptions(options).code), int(LaunchValidator::LaunchError::None));

    options.pcfgRulesets << QStringLiteral("/tmp/one-too-many.pcfg");
    QString detail;
    QCOMPARE(int(validateOptionsWithDetail(options, &detail)),
             int(LaunchValidator::LaunchError::TooManyPcfgRulesets));
    QCOMPARE(detail, QStringLiteral("17"));
}

// The mode 4 settings: the feed parses them only after the backend devices
// are set up, and maxgain is never checked upstream at all.
void TestLaunchValidator::pcfgSettingsAreValidated()
{
    const auto expect = [](const HashcatOptions &options,
                           LaunchValidator::LaunchError expected) -> void {
        const LaunchValidator::LaunchProblem problem = ::validateOptions(options);
        QCOMPARE(int(problem.code), int(expected));
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
    expect(clean, LaunchValidator::LaunchError::None);

    struct Case {
        QString HashcatOptions::*field;
        QString value;
        QString reported; // the detail: the setting as the feed sees it
    };
    const auto typedCases = std::to_array<Case>({
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
    });
    for (const Case &bad : typedCases) {
        HashcatOptions options = clean;
        options.*bad.field = bad.value;
        const LaunchValidator::LaunchProblem problem = ::validateOptions(options);
        QCOMPARE(int(problem.code), int(LaunchValidator::LaunchError::InvalidPcfgSetting));
        QCOMPARE(problem.detail, bad.reported);
    }

    // weights: a colon separated list of shares above zero, one per ruleset
    // - an empty list is the one bundled default. The feed parses each share
    // with strtod(), so fractions and exponents are numbers too.
    const QStringList validWeights{QStringLiteral("2.5:1"), QStringLiteral("1e2:3"),
                                   QStringLiteral("1E2:0.5"), QStringLiteral("2:1e-2")};
    for (const QString &weights : validWeights) {
        HashcatOptions options = clean;
        options.pcfgWeights = weights;
        expect(options, LaunchValidator::LaunchError::None);
    }

    // The shape stays conservative: no bare-dot forms, no inf/nan, and an
    // exponent that overflows to infinity is no finite share.
    const QStringList weightCases{
        QStringLiteral("0"),  QStringLiteral("a:b"), QStringLiteral("1:1"), QStringLiteral("1e999"),
        QStringLiteral("-1"), QStringLiteral(".5"),  QStringLiteral("2.")};
    for (const QString &weights : weightCases) {
        HashcatOptions options = clean;
        options.pcfgRulesets.clear();
        options.pcfgWeights = weights;
        QCOMPARE(int(::validateOptions(options).code),
                 int(LaunchValidator::LaunchError::InvalidPcfgSetting));
    }

    // A fraction above zero is a share like any other, also for the one
    // bundled default ruleset.
    HashcatOptions oneShare = clean;
    oneShare.pcfgRulesets.clear();
    oneShare.pcfgWeights = QStringLiteral("0.5");
    expect(oneShare, LaunchValidator::LaunchError::None);

    // One share per ruleset, in both directions.
    HashcatOptions weightsCount = clean;
    weightsCount.pcfgWeights = QStringLiteral("2.5");
    expect(weightsCount, LaunchValidator::LaunchError::InvalidPcfgSetting);
    weightsCount.pcfgWeights = QStringLiteral("2.5:1:1");
    expect(weightsCount, LaunchValidator::LaunchError::InvalidPcfgSetting);

    // The grammar held to the mask refuses a named, non-zero omen - a U64,
    // so omen=01 is omen=1 - and the hint words from the accounts have no
    // source while the mask is on.
    const QStringList maskConflicts{QStringLiteral("omen=1"), QStringLiteral("omen=01"),
                                    QStringLiteral("omen=2"), QStringLiteral("hintaccount=8")};
    for (const QString &token : maskConflicts) {
        HashcatOptions options = clean;
        options.pcfgExtraSettings = QStringList{token};
        QCOMPARE(int(::validateOptions(options).code),
                 int(LaunchValidator::LaunchError::InvalidPcfgSetting));
    }

    // A mask with the hybrid markers is mode 12's rewriting, not a grammar.
    HashcatOptions markers = clean;
    markers.pcfgMask = QStringLiteral("?u?w");
    expect(markers, LaunchValidator::LaunchError::InvalidPcfgSetting);

    // The conflicts only count with a mask, and a named source (or
    // hintaccount=0) is no conflict at all.
    HashcatOptions hintwords = clean;
    hintwords.pcfgExtraSettings =
        QStringList{QStringLiteral("hintaccount=8"), QStringLiteral("hintwords=w")};
    expect(hintwords, LaunchValidator::LaunchError::None);

    HashcatOptions noHint = clean;
    noHint.pcfgExtraSettings = QStringList{QStringLiteral("hintaccount=0")};
    expect(noHint, LaunchValidator::LaunchError::None);

    // omen=0 spelled with leading zeros is still zero: no conflict.
    HashcatOptions omenZero = clean;
    omenZero.pcfgExtraSettings = QStringList{QStringLiteral("omen=00")};
    expect(omenZero, LaunchValidator::LaunchError::None);

    HashcatOptions noMask = clean;
    noMask.pcfgMask.clear();
    noMask.pcfgExtraSettings = QStringList{QStringLiteral("omen=1")};
    expect(noMask, LaunchValidator::LaunchError::None);

    // A custom charset is accepted only inside a mask, which the feed refuses
    // rather than ignoring. The builder emits one only while its checkbox is
    // on and it has a value. This is not a bad setting value, so it has its own
    // error and names no value: the mask is missing, not invalid.
    HashcatOptions charsetNoMask = clean;
    charsetNoMask.pcfgMask.clear();
    charsetNoMask.customCharsets.at(0).enabled = true;
    charsetNoMask.customCharsets.at(0).value = QStringLiteral("abc");
    QString charsetDetail;
    QCOMPARE(int(validateOptionsWithDetail(charsetNoMask, &charsetDetail)),
             int(LaunchValidator::LaunchError::PcfgCharsetWithoutMask));
    QVERIFY(charsetDetail.isEmpty());

    HashcatOptions charsetWithMask = clean;
    charsetWithMask.customCharsets.at(0).enabled = true;
    charsetWithMask.customCharsets.at(0).value = QStringLiteral("abc");
    expect(charsetWithMask, LaunchValidator::LaunchError::None);

    HashcatOptions charsetDisabled = charsetNoMask;
    charsetDisabled.customCharsets.at(0).enabled = false;
    expect(charsetDisabled, LaunchValidator::LaunchError::None);

    HashcatOptions charsetEmpty = charsetNoMask;
    charsetEmpty.customCharsets.at(0).value.clear();
    expect(charsetEmpty, LaunchValidator::LaunchError::None);

    // mask= is a setting like any other, so the extra line may carry it; then
    // the charset has its mask, and the mask conflicts are checked against
    // that value as well.
    HashcatOptions charsetViaExtra = charsetNoMask;
    charsetViaExtra.pcfgExtraSettings = QStringList{QStringLiteral("mask=?l?l")};
    expect(charsetViaExtra, LaunchValidator::LaunchError::None);

    HashcatOptions omenViaExtra = charsetNoMask;
    omenViaExtra.customCharsets.at(0) = HashcatOptions::Charset();
    omenViaExtra.pcfgExtraSettings =
        QStringList{QStringLiteral("mask=?l?l"), QStringLiteral("omen=1")};
    expect(omenViaExtra, LaunchValidator::LaunchError::InvalidPcfgSetting);

    // A bare token before the first setting would be read as another
    // ruleset.
    HashcatOptions bare = clean;
    bare.pcfgExtraSettings = QStringList{QStringLiteral("pwmin")};
    QString detail;
    QCOMPARE(int(validateOptionsWithDetail(bare, &detail)),
             int(LaunchValidator::LaunchError::InvalidPcfgSetting));
    QCOMPARE(detail, QStringLiteral("pwmin"));

    // The GUI can create a repeated key itself: a typed field plus an extra
    // token. Every feed refuses one, but only after device init.
    HashcatOptions repeated = clean;
    repeated.pcfgExtraSettings = QStringList{QStringLiteral("scale=3")};
    QCOMPARE(int(validateOptionsWithDetail(repeated, &detail)),
             int(LaunchValidator::LaunchError::InvalidPcfgSetting));
    QCOMPARE(detail, QStringLiteral("scale: given more than once"));
}

// The mode 5 settings, for the same late-failure reason as the mode 4 ones.
void TestLaunchValidator::tableSettingsAreValidated()
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
    QCOMPARE(int(::validateOptions(clean).code), int(LaunchValidator::LaunchError::None));

    HashcatOptions flag = clean;
    flag.tableTemplate = QStringLiteral("2");
    QString detail;
    QCOMPARE(int(validateOptionsWithDetail(flag, &detail)),
             int(LaunchValidator::LaunchError::InvalidTableSetting));
    QCOMPARE(detail, QStringLiteral("template=2"));

    HashcatOptions substitution = clean;
    substitution.tableSingle = QStringLiteral("1");
    substitution.tableIdentity = QStringLiteral("0");
    QCOMPARE(int(validateOptionsWithDetail(substitution, &detail)),
             int(LaunchValidator::LaunchError::InvalidTableSetting));
    QCOMPARE(detail, QStringLiteral("identity=0"));

    // The flags are feed booleans: the typed field takes the same spellings
    // as the extra line, and the conflict reads either of them - typed plus
    // extra, extra plus typed, and extra plus extra.
    const QStringList trueSpellings{QStringLiteral("1"), QStringLiteral("yes"),
                                    QStringLiteral("true"), QStringLiteral("on")};
    const QStringList falseSpellings{QStringLiteral("0"), QStringLiteral("no"),
                                     QStringLiteral("false"), QStringLiteral("off")};
    for (const QString &yes : trueSpellings) {
        for (const QString &no : falseSpellings) {
            HashcatOptions typed = clean;
            typed.tableSingle = yes;
            typed.tableIdentity = no;
            QCOMPARE(int(validateOptionsWithDetail(typed, &detail)),
                     int(LaunchValidator::LaunchError::InvalidTableSetting));
            QCOMPARE(detail, QStringLiteral("identity=") + no);

            HashcatOptions typedSingle = clean;
            typedSingle.tableSingle = yes;
            typedSingle.tableIdentity.clear();
            typedSingle.tableExtraSettings = QStringList{QStringLiteral("identity=") + no};
            QCOMPARE(int(validateOptionsWithDetail(typedSingle, &detail)),
                     int(LaunchValidator::LaunchError::InvalidTableSetting));
            QCOMPARE(detail, QStringLiteral("identity=") + no);

            HashcatOptions typedIdentity = clean;
            typedIdentity.tableSingle.clear();
            typedIdentity.tableIdentity = no;
            typedIdentity.tableExtraSettings = QStringList{QStringLiteral("single=") + yes};
            QCOMPARE(int(validateOptionsWithDetail(typedIdentity, &detail)),
                     int(LaunchValidator::LaunchError::InvalidTableSetting));
            QCOMPARE(detail, QStringLiteral("identity=") + no);

            HashcatOptions extra = clean;
            extra.tableSingle.clear();
            extra.tableIdentity.clear();
            extra.tableExtraSettings =
                QStringList{QStringLiteral("single=") + yes, QStringLiteral("identity=") + no};
            QCOMPARE(int(validateOptionsWithDetail(extra, &detail)),
                     int(LaunchValidator::LaunchError::InvalidTableSetting));
            QCOMPARE(detail, QStringLiteral("identity=") + no);
        }
    }

    // The same spellings without the conflict are valid settings, and a
    // spelling the feed does not know is refused - case-sensitive, like the
    // feed's own comparison.
    HashcatOptions spelled = clean;
    spelled.tableSingle = QStringLiteral("true");
    spelled.tableIdentity = QStringLiteral("yes");
    spelled.tableTemplate = QStringLiteral("off");
    QCOMPARE(int(::validateOptions(spelled).code), int(LaunchValidator::LaunchError::None));

    HashcatOptions noConflict = clean;
    noConflict.tableSingle = QStringLiteral("on");
    noConflict.tableIdentity = QStringLiteral("on");
    QCOMPARE(int(::validateOptions(noConflict).code), int(LaunchValidator::LaunchError::None));

    HashcatOptions badSpelling = clean;
    badSpelling.tableSingle = QStringLiteral("TRUE");
    QCOMPARE(int(validateOptionsWithDetail(badSpelling, &detail)),
             int(LaunchValidator::LaunchError::InvalidTableSetting));
    QCOMPARE(detail, QStringLiteral("single=TRUE"));

    // cap and maxperm reach beyond every Qt integer type, so their bounds
    // are checked here, not by a spin box.
    HashcatOptions cap = clean;
    cap.tableCap = QStringLiteral("4294967296");
    QCOMPARE(int(validateOptionsWithDetail(cap, &detail)),
             int(LaunchValidator::LaunchError::InvalidTableSetting));
    QCOMPARE(detail, QStringLiteral("cap=4294967296"));

    HashcatOptions maxperm = clean;
    maxperm.tableMaxperm = QStringLiteral("18446744073709551616"); // 2^64
    QCOMPARE(int(validateOptionsWithDetail(maxperm, &detail)),
             int(LaunchValidator::LaunchError::InvalidTableSetting));
    QCOMPARE(detail, QStringLiteral("maxperm=18446744073709551616"));

    // A bare extra token would be read as another table.
    HashcatOptions bare = clean;
    bare.tableExtraSettings = QStringList{QStringLiteral("cap")};
    QCOMPARE(int(validateOptionsWithDetail(bare, &detail)),
             int(LaunchValidator::LaunchError::InvalidTableSetting));
    QCOMPARE(detail, QStringLiteral("cap"));

    // The typed field plus an extra token: the same repeated key the feed
    // refuses only after device init.
    HashcatOptions repeated = clean;
    repeated.tableExtraSettings = QStringList{QStringLiteral("cap=5")};
    QCOMPARE(int(validateOptionsWithDetail(repeated, &detail)),
             int(LaunchValidator::LaunchError::InvalidTableSetting));
    QCOMPARE(detail, QStringLiteral("cap: given more than once"));

    // A full run with the extra line.
    HashcatOptions full = clean;
    full.tableExtraSettings = QStringList{QStringLiteral("cache=1"), QStringLiteral("pwmax=64")};
    QCOMPARE(int(::validateOptions(full).code), int(LaunchValidator::LaunchError::None));
}

// The mode 9 settings line: the two forms are alternatives, and a token
// that is not a setting would become a wordlist upstream.
void TestLaunchValidator::associationSettingsAreValidated()
{
    const QStringList valid{QStringLiteral("phases=words,rules"),
                            QStringLiteral("rules.rulemax=300")};

    HashcatOptions clean;
    clean.attackMode = AttackMode::Association;
    clean.associationSettings = valid;
    QCOMPARE(int(::validateOptions(clean).code), int(LaunchValidator::LaunchError::None));

    // The shape is about the key only: hashcat reports the missing phase
    // itself, once the session starts.
    HashcatOptions emptyPhase;
    emptyPhase.attackMode = AttackMode::Association;
    emptyPhase.associationSettings = QStringList{QStringLiteral("phases=")};
    QCOMPARE(int(::validateOptions(emptyPhase).code), int(LaunchValidator::LaunchError::None));

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
        QCOMPARE(int(validateOptionsWithDetail(options, &detail)),
                 int(LaunchValidator::LaunchError::InvalidAssociationSetting));
        QCOMPARE(detail, token);
    }

    // With a wordlist the two files are paired by line and no setting is
    // accepted; the combination is reported before the shape.
    HashcatOptions both;
    both.attackMode = AttackMode::Association;
    both.wordlists = QStringList{QStringLiteral("/tmp/a.txt")};
    both.associationSettings = QStringList{QStringLiteral("phases words,rules")};
    QCOMPARE(int(::validateOptions(both).code),
             int(LaunchValidator::LaunchError::AssociationSettingsWithWordlist));
}

// The mode 12 mask markers, and the markers in the modes that rewrite them.
void TestLaunchValidator::maskMarkersAreValidated()
{
    const QStringList one{QStringLiteral("/tmp/a.txt")};
    const QStringList two{QStringLiteral("/tmp/a.txt"), QStringLiteral("/tmp/b.txt")};

    const auto hybridError = [](const QString &mask, const QStringList &wordlists) {
        HashcatOptions options;
        options.attackMode = AttackMode::Hybrid;
        options.mask = mask;
        options.wordlists = wordlists;
        return int(::validateOptions(options).code);
    };

    QCOMPARE(hybridError(QStringLiteral("?d?w?d"), one), int(LaunchValidator::LaunchError::None));
    QCOMPARE(hybridError(QStringLiteral("?w?q"), two), int(LaunchValidator::LaunchError::None));
    // An escaped marker pair stands for the literal characters.
    QCOMPARE(hybridError(QStringLiteral("??w"), one),
             int(LaunchValidator::LaunchError::MaskNeedsWordMarker));

    QCOMPARE(hybridError(QStringLiteral("?d?d?d"), one),
             int(LaunchValidator::LaunchError::MaskNeedsWordMarker));
    QCOMPARE(hybridError(QStringLiteral("?w?w"), one),
             int(LaunchValidator::LaunchError::MaskMultipleWordMarkers));
    QCOMPARE(hybridError(QStringLiteral("?w?q?q"), one),
             int(LaunchValidator::LaunchError::MaskMultipleQMarks));
    QCOMPARE(hybridError(QStringLiteral("?q?w"), one),
             int(LaunchValidator::LaunchError::MaskQBeforeWord));
    // The markers have to line up with what is selected.
    QCOMPARE(hybridError(QStringLiteral("?w?q"), one),
             int(LaunchValidator::LaunchError::MaskQWithoutWordlist));
    QCOMPARE(hybridError(QStringLiteral("?w"), two),
             int(LaunchValidator::LaunchError::SecondWordlistWithoutQ));

    // The increment is refused in mode 12 as well: the widget is hidden
    // there and collect drops the flag, so this is defence in depth.
    HashcatOptions increment;
    increment.attackMode = AttackMode::Hybrid;
    increment.mask = QStringLiteral("?w");
    increment.wordlists = one;
    increment.increment = true;
    QCOMPARE(int(::validateOptions(increment).code),
             int(LaunchValidator::LaunchError::IncrementInMode12));

    // The other mask modes would take the markers for their literal ?w/?q
    // or rewrite them.
    HashcatOptions brute;
    brute.attackMode = AttackMode::BruteForce;
    brute.mask = QStringLiteral("?w?d");
    QCOMPARE(int(::validateOptions(brute).code),
             int(LaunchValidator::LaunchError::MaskMarkersNotAllowed));

    HashcatOptions hybridWord;
    hybridWord.attackMode = AttackMode::HybridWordMask;
    hybridWord.wordlists = one;
    hybridWord.mask = QStringLiteral("?d?q");
    QCOMPARE(int(::validateOptions(hybridWord).code),
             int(LaunchValidator::LaunchError::MaskMarkersNotAllowed));

    // A mask without the markers is fine in every mode.
    brute.mask = QStringLiteral("?d?d");
    QCOMPARE(int(::validateOptions(brute).code), int(LaunchValidator::LaunchError::None));
}

// Hash mode 4000 describes its hashes as a recipe; without one hashcat
// refuses the run, and the row that takes it only exists for this type.
void TestLaunchValidator::hashRecipeIsRequiredFor4000()
{
    HashcatOptions options;
    options.attackMode = AttackMode::Straight;
    options.hashType = AppConstants::Hashcat::RecipeHashType;
    options.wordlists = QStringList{QStringLiteral("/tmp/a.txt")};
    QCOMPARE(int(::validateOptions(options).code), int(LaunchValidator::LaunchError::NoHashRecipe));

    options.hashRecipe = QStringLiteral("md5(md5(salt) . pass)");
    QCOMPARE(int(::validateOptions(options).code), int(LaunchValidator::LaunchError::None));

    // A stale recipe next to any other hash type is left out by the builder
    // and must not keep the run from starting.
    options.hashType = 1000;
    QCOMPARE(int(::validateOptions(options).code), int(LaunchValidator::LaunchError::None));
}

// hashcat only accepts an existing directory as its cache path; the browse
// dialog offers directories, so a failure here is a hand edit or a
// directory that has gone away.
void TestLaunchValidator::cachePathNeedsAnExistingDirectory()
{
    HashcatOptions options;
    options.attackMode = AttackMode::Straight;
    options.wordlists = QStringList{QStringLiteral("/tmp/a.txt")};

    // Empty means "hashcat decides", so it is never checked.
    QCOMPARE(int(::validateOptions(options).code), int(LaunchValidator::LaunchError::None));

    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    options.cachePath = directory.path();
    QCOMPARE(int(::validateOptions(options).code), int(LaunchValidator::LaunchError::None));

    // A file is no directory.
    QFile file(directory.filePath(QStringLiteral("plain.txt")));
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.close();
    options.cachePath = file.fileName();
    LaunchValidator::LaunchProblem problem = ::validateOptions(options);
    QCOMPARE(int(problem.code), int(LaunchValidator::LaunchError::InvalidCachePath));
    // The message names the path, so the validator has to hand it over.
    QCOMPARE(problem.detail, options.cachePath);

    // Neither is a directory that has gone away.
    options.cachePath = directory.filePath(QStringLiteral("gone"));
    problem = ::validateOptions(options);
    QCOMPARE(int(problem.code), int(LaunchValidator::LaunchError::InvalidCachePath));
    QCOMPARE(problem.detail, options.cachePath);

    // Whitespace is emitted verbatim, so the raw check must catch it.
    options.cachePath = QStringLiteral("   ");
    problem = ::validateOptions(options);
    QCOMPARE(int(problem.code), int(LaunchValidator::LaunchError::InvalidCachePath));
    QCOMPARE(problem.detail, options.cachePath);
}

// The paths hashcat reads are relative to the directory it runs in, so a
// relative cache path or ?q wordlist must be checked there, not against the
// GUI process's current directory.
void TestLaunchValidator::relativePathsResolveAgainstTheProgramDirectory()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString program = directory.filePath(QStringLiteral("hashcat"));
    QFile binary(program);
    QVERIFY(binary.open(QIODevice::WriteOnly));
    binary.close();
    QVERIFY(QDir(directory.path()).mkpath(QStringLiteral("cache")));

    HashcatOptions options;
    options.attackMode = AttackMode::Straight;
    options.wordlists = QStringList{QStringLiteral("words.txt")};

    options.cachePath = QStringLiteral("cache");
    QCOMPARE(int(validateOptionsAt(options, program).code),
             int(LaunchValidator::LaunchError::None));

    // A relative name that is a file, not a directory, is refused; the detail
    // stays the raw value the user typed.
    QFile notADirectory(directory.filePath(QStringLiteral("notadir")));
    QVERIFY(notADirectory.open(QIODevice::WriteOnly));
    notADirectory.close();
    options.cachePath = QStringLiteral("notadir");
    LaunchValidator::LaunchProblem problem = validateOptionsAt(options, program);
    QCOMPARE(int(problem.code), int(LaunchValidator::LaunchError::InvalidCachePath));
    QCOMPARE(problem.detail, QStringLiteral("notadir"));

    // The ?q wordlist is the row that must be a file; a folder next to the
    // program is refused just like an absolute one.
    QVERIFY(QDir(directory.path()).mkpath(QStringLiteral("wordfolder")));
    HashcatOptions hybrid;
    hybrid.attackMode = AttackMode::Hybrid;
    hybrid.mask = QStringLiteral("?w?q");
    hybrid.wordlists = {QStringLiteral("words.txt"), QStringLiteral("wordfolder")};
    problem = validateOptionsAt(hybrid, program);
    QCOMPARE(int(problem.code), int(LaunchValidator::LaunchError::WordlistMustBeFile));
    QCOMPARE(problem.detail, QStringLiteral("wordfolder"));
}

// A session file sets widget text directly, so every field the widgets would
// filter has to be checked again before a launch. Valid and invalid pairs,
// each against a program directory so the relative paths mean something.
void TestLaunchValidator::sessionSuppliedValuesAreRevalidated()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString program = directory.filePath(QStringLiteral("hashcat"));
    QFile binary(program);
    QVERIFY(binary.open(QIODevice::WriteOnly));
    binary.close();
    QVERIFY(QDir(directory.path()).mkpath(QStringLiteral("cache")));
    QVERIFY(QDir(directory.path()).mkpath(QStringLiteral("empty-rules")));
    QVERIFY(QDir(directory.path()).mkpath(QStringLiteral("grammar-rules/Grammar")));
    QFile grammar(directory.filePath(QStringLiteral("grammar-rules/Grammar/grammar.txt")));
    QVERIFY(grammar.open(QIODevice::WriteOnly));
    grammar.close();
    QVERIFY(QDir(directory.path()).mkpath(QStringLiteral("table-folder")));

    HashcatOptions options;
    options.attackMode = AttackMode::Straight;
    options.wordlists = QStringList{QStringLiteral("words.txt")};

    const auto expect = [&program](const HashcatOptions &checked,
                                   LaunchValidator::LaunchError expected,
                                   const QString &expectedDetail = QString()) {
        const LaunchValidator::LaunchProblem problem = validateOptionsAt(checked, program);
        QCOMPARE(int(problem.code), int(expected));
        QCOMPARE(problem.detail, expectedDetail);
    };

    // A base wordlist may be a folder; the ?q row is the file-only one.
    options.wordlists = QStringList{QStringLiteral("cache")};
    expect(options, LaunchValidator::LaunchError::None);
    options.wordlists = QStringList{QStringLiteral("words.txt")};

    options.skip = QStringLiteral("0");
    expect(options, LaunchValidator::LaunchError::None);
    options.skip = QStringLiteral("18446744073709551615");
    expect(options, LaunchValidator::LaunchError::None);
    // hashcat parses both with hc_strtou128(), so the u64 maximum is not the
    // limit.
    options.skip = QStringLiteral("18446744073709551616");
    expect(options, LaunchValidator::LaunchError::None);
    options.skip = QString(AppConstants::Validation::MaxUnsigned128);
    expect(options, LaunchValidator::LaunchError::None);
    options.skip = QStringLiteral("340282366920938463463374607431768211456");
    expect(options, LaunchValidator::LaunchError::InvalidSkipLimit, options.skip);
    options.skip = QStringLiteral("000000000000000000000000000000000000000042");
    expect(options, LaunchValidator::LaunchError::None);
    options.skip = QStringLiteral("abc");
    expect(options, LaunchValidator::LaunchError::InvalidSkipLimit, options.skip);
    options.skip.clear();
    options.limit = QStringLiteral("-1");
    expect(options, LaunchValidator::LaunchError::InvalidSkipLimit, options.limit);
    options.limit = QStringLiteral("18446744073709551616");
    expect(options, LaunchValidator::LaunchError::None);
    options.limit.clear();

    options.openclDeviceTypes = QStringLiteral("1,2");
    expect(options, LaunchValidator::LaunchError::None);
    options.openclDeviceTypes = QStringLiteral("3");
    expect(options, LaunchValidator::LaunchError::InvalidOpenclDeviceTypes,
           options.openclDeviceTypes);
    options.openclDeviceTypes.clear();

    options.cpuAffinity = QStringLiteral("0,1");
    expect(options, LaunchValidator::LaunchError::None);
    options.cpuAffinity = QStringLiteral("1-4");
    expect(options, LaunchValidator::LaunchError::InvalidCpuAffinity, options.cpuAffinity);
    options.cpuAffinity.clear();

    options.backendDevices = QStringLiteral("1,2");
    expect(options, LaunchValidator::LaunchError::None);
    options.backendDevices = QStringLiteral("0");
    expect(options, LaunchValidator::LaunchError::InvalidBackendDevices, options.backendDevices);
    options.backendDevices.clear();

    options.session = QStringLiteral("attack one");
    expect(options, LaunchValidator::LaunchError::None);
    options.session = QStringLiteral("sub/attack");
    expect(options, LaunchValidator::LaunchError::InvalidSessionName, options.session);
    options.session = QStringLiteral("sub\\attack");
    expect(options, LaunchValidator::LaunchError::InvalidSessionName, options.session);
    options.session.clear();

    options.potfilePath = QStringLiteral("hashcat.potfile");
    expect(options, LaunchValidator::LaunchError::None);
    options.potfilePath = QStringLiteral("cache");
    expect(options, LaunchValidator::LaunchError::InvalidPotfilePath, options.potfilePath);
    options.potfilePath = QStringLiteral("missing-dir/hashcat.potfile");
    expect(options, LaunchValidator::LaunchError::InvalidPotfilePath, options.potfilePath);
    // The checkbox keeps the path out of the command, so it is not checked.
    options.potfileDisable = true;
    options.potfilePath = QStringLiteral("cache");
    expect(options, LaunchValidator::LaunchError::None);
    options.potfileDisable = false;
    options.potfilePath.clear();

    // --speed-only turns the potfile off upstream; a path there is unused.
    options.speedOnly = true;
    options.potfilePath = QStringLiteral("cache");
    expect(options, LaunchValidator::LaunchError::None);
    options.speedOnly = false;
    options.potfilePath.clear();

    options.rulesFiles = QStringList{QStringLiteral("hashcat.rules")};
    expect(options, LaunchValidator::LaunchError::None);
    options.rulesFiles = QStringList{QStringLiteral("empty-rules")};
    expect(options, LaunchValidator::LaunchError::InvalidRulesetPath, options.rulesFiles.first());
    options.rulesFiles = QStringList{QStringLiteral("grammar-rules")};
    expect(options, LaunchValidator::LaunchError::None);
    options.rulesFiles.clear();

    options.attackMode = AttackMode::Table;
    options.tableFiles = QStringList{QStringLiteral("table-folder")};
    expect(options, LaunchValidator::LaunchError::InvalidTablePath, options.tableFiles.first());
    options.tableFiles = QStringList{QStringLiteral("table.txt")};
    expect(options, LaunchValidator::LaunchError::None);
}

// hashcat opens the potfile and outfile for writing and does not check the
// potfile against its inputs. Two roles on one path would corrupt each other,
// so the launch refuses the exact conflict and names both roles.
void TestLaunchValidator::pathCollisionsAreRefused()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const auto writeFile = [](const QString &path) {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly)) {
            return false;
        }
        file.write("x\n");
        file.close();
        return true;
    };

    const QString program = directory.filePath(QStringLiteral("hashcat"));
    const QString hashFile = directory.filePath(QStringLiteral("hashes.txt"));
    const QString wordlist = directory.filePath(QStringLiteral("words.txt"));
    const QString secondWordlist = directory.filePath(QStringLiteral("words2.txt"));
    const QString rulesFile = directory.filePath(QStringLiteral("rules.rule"));
    const QString tableFile = directory.filePath(QStringLiteral("table.txt"));
    const QString outfile = directory.filePath(QStringLiteral("out.txt"));
    QVERIFY(writeFile(program));
    QVERIFY(writeFile(hashFile));
    QVERIFY(writeFile(wordlist));
    QVERIFY(writeFile(secondWordlist));
    QVERIFY(writeFile(rulesFile));
    QVERIFY(writeFile(tableFile));
    QVERIFY(QDir(directory.path()).mkpath(QStringLiteral("cache")));

    HashcatOptions options;
    options.attackMode = AttackMode::Straight;
    options.hashFile = hashFile;
    options.wordlists = QStringList{wordlist};

    const auto expectCollision =
        [&program](const HashcatOptions &checked, LaunchValidator::PathRole first,
                   LaunchValidator::PathRole second, const QString &expectedDetail) {
            const LaunchValidator::LaunchProblem problem = validateOptionsAt(checked, program);
            QCOMPARE(int(problem.code), int(LaunchValidator::LaunchError::PathCollision));
            QCOMPARE(int(problem.firstRole), int(first));
            QCOMPARE(int(problem.secondRole), int(second));
            QCOMPARE(problem.detail, expectedDetail);
        };
    const auto expectClean = [&program](const HashcatOptions &checked) {
        QCOMPARE(int(validateOptionsAt(checked, program).code),
                 int(LaunchValidator::LaunchError::None));
    };

    // The potfile is the writer that hashcat itself does not check.
    options.potfilePath = hashFile;
    expectCollision(options, LaunchValidator::PathRole::HashFile,
                    LaunchValidator::PathRole::Potfile, hashFile);

    // --speed-only turns the potfile off upstream, so this path is no writer.
    options.speedOnly = true;
    expectClean(options);
    options.speedOnly = false;

    // A relative spelling resolves against the program directory and meets
    // the absolute one.
    options.potfilePath = QStringLiteral("hashes.txt");
    expectCollision(options, LaunchValidator::PathRole::HashFile,
                    LaunchValidator::PathRole::Potfile,
                    hashFile + QLatin1Char('\n') + QStringLiteral("hashes.txt"));

    options.potfilePath = wordlist;
    expectCollision(options, LaunchValidator::PathRole::Potfile,
                    LaunchValidator::PathRole::Wordlist, wordlist);
    options.potfilePath.clear();

    // Mode 4 reads its rulesets as positional inputs, not through -r.
    options.attackMode = AttackMode::Pcfg;
    options.pcfgRulesets = QStringList{rulesFile};
    options.potfilePath = rulesFile;
    expectCollision(options, LaunchValidator::PathRole::Potfile, LaunchValidator::PathRole::Ruleset,
                    rulesFile);
    options.pcfgRulesets.clear();
    options.potfilePath.clear();
    options.attackMode = AttackMode::Straight;

    options.outfileEnabled = true;
    options.outfile = hashFile;
    expectCollision(options, LaunchValidator::PathRole::HashFile,
                    LaunchValidator::PathRole::Outfile, hashFile);
    options.outfile = outfile;
    options.potfilePath = outfile;
    expectCollision(options, LaunchValidator::PathRole::Outfile, LaunchValidator::PathRole::Potfile,
                    outfile);
    options.potfilePath.clear();
    options.cachePath = QStringLiteral("cache");
    options.outfile = QStringLiteral("cache");
    expectCollision(options, LaunchValidator::PathRole::Outfile, LaunchValidator::PathRole::Cache,
                    QStringLiteral("cache"));

    // A symlink is the same file to hashcat's stat(), so it collides even
    // though the two spellings differ.
#ifndef Q_OS_WIN
    const QString link = directory.filePath(QStringLiteral("words-link.txt"));
    QVERIFY(QFile::link(wordlist, link));
    options.outfile = link;
    expectCollision(options, LaunchValidator::PathRole::Outfile,
                    LaunchValidator::PathRole::Wordlist, link + QLatin1Char('\n') + wordlist);
#endif

    // Distinct paths pass, and readers may share one.
    options.outfile = outfile;
    options.potfilePath = QStringLiteral("hashcat.potfile");
    expectClean(options);
    options.outfileEnabled = false;
    options.potfilePath.clear();
    options.wordlists = QStringList{wordlist, wordlist};
    expectClean(options);

    // The outfile placeholders are expanded before launch, so the collision
    // check has to use the expanded path and compare case-insensitively.
    options.hashFile = directory.filePath(QStringLiteral("work/hashes.txt"));
    options.outfileEnabled = true;
    options.outfile = QStringLiteral("<HASH>");
    options.potfilePath = QStringLiteral("hashes.txt");
    expectCollision(options, LaunchValidator::PathRole::Outfile, LaunchValidator::PathRole::Potfile,
                    QStringLiteral("hashes.txt"));

    // A template that expands to a distinct path passes.
    options.outfile = QStringLiteral("<hash>.out");
    options.potfilePath = QStringLiteral("hashcat.potfile");
    expectClean(options);
}

// A NUL would silently end the token at exec, so what hashcat receives would
// not be what the preview shows. Every other C0 control except tab reaches
// the terminal and the clipboard as itself. The detail spells the character
// out so the message box can name the value.
void TestLaunchValidator::controlCharactersAreRefused()
{
    HashcatOptions maskRun;
    maskRun.attackMode = AttackMode::BruteForce;
    maskRun.mask = QStringLiteral("?d") + QChar(0) + QStringLiteral("?d");
    LaunchValidator::LaunchProblem problem = ::validateOptions(maskRun);
    QCOMPARE(int(problem.code), int(LaunchValidator::LaunchError::InvalidCharacter));
    QCOMPARE(problem.detail, QStringLiteral("?d\\x00?d"));

    maskRun.mask = QStringLiteral("?d?d") + QChar(0x1b);
    problem = ::validateOptions(maskRun);
    QCOMPARE(int(problem.code), int(LaunchValidator::LaunchError::InvalidCharacter));
    QCOMPARE(problem.detail, QStringLiteral("?d?d\\x1b"));

    // DEL (0x7f) and C1 (0x80-0x9f) are controls too.
    maskRun.mask = QStringLiteral("?d") + QChar(0x7f);
    problem = ::validateOptions(maskRun);
    QCOMPARE(int(problem.code), int(LaunchValidator::LaunchError::InvalidCharacter));
    QCOMPARE(problem.detail, QStringLiteral("?d\\x7f"));
    maskRun.mask = QStringLiteral("?d") + QChar(0x85);
    problem = ::validateOptions(maskRun);
    QCOMPARE(int(problem.code), int(LaunchValidator::LaunchError::InvalidCharacter));
    QCOMPARE(problem.detail, QStringLiteral("?d\\x85"));

    // A wordlist in the straight mode is part of the command line too.
    HashcatOptions wordlistRun;
    wordlistRun.attackMode = AttackMode::Straight;
    wordlistRun.wordlists =
        QStringList{QStringLiteral("dict") + QChar(0x1b) + QStringLiteral(".txt")};
    problem = ::validateOptions(wordlistRun);
    QCOMPARE(int(problem.code), int(LaunchValidator::LaunchError::InvalidCharacter));
    QCOMPARE(problem.detail, QStringLiteral("dict\\x1b.txt"));

    // Tab is no control character for this check.
    wordlistRun.wordlists = QStringList{QStringLiteral("two\twords.txt")};
    QCOMPARE(int(::validateOptions(wordlistRun).code), int(LaunchValidator::LaunchError::None));

    // A value the attack mode leaves out of the command line is not refused:
    // mode 3 reads no wordlist, so its control character never reaches hashcat.
    HashcatOptions ignored;
    ignored.attackMode = AttackMode::BruteForce;
    ignored.mask = QStringLiteral("?d?d");
    ignored.wordlists = QStringList{QStringLiteral("dict") + QChar(0x1b) + QStringLiteral(".txt")};
    QCOMPARE(int(::validateOptions(ignored).code), int(LaunchValidator::LaunchError::None));
}

void TestLaunchValidator::outfileFormatIsValidated()
{
    const auto withWordlist = [] {
        HashcatOptions options;
        options.wordlists = QStringList{QStringLiteral("/tmp/a.txt")};
        return options;
    };

    HashcatOptions options = withWordlist();
    options.outfileFormat = QStringLiteral("3,4");
    QCOMPARE(int(::validateOptions(options).code), int(LaunchValidator::LaunchError::None));

    const auto refused = [&withWordlist](const QString &value, QString *detail) {
        HashcatOptions bad = withWordlist();
        bad.outfileFormat = value;
        return ::validateOptionsWithDetail(bad, detail);
    };
    QString detail;
    QCOMPARE(refused(QString(), &detail), LaunchValidator::LaunchError::InvalidOutfileFormat);
    QVERIFY(detail.isEmpty());
    QCOMPARE(refused(QStringLiteral("1,7"), &detail),
             LaunchValidator::LaunchError::InvalidOutfileFormat);
    QCOMPARE(detail, QStringLiteral("1,7"));
    QCOMPARE(refused(QStringLiteral("1, 2"), &detail),
             LaunchValidator::LaunchError::InvalidOutfileFormat);

    // The default and JSON mode leave the field out of the command line, so
    // even a stale value there must not block the launch.
    HashcatOptions stale = withWordlist();
    stale.outfileFormat = QStringLiteral("9");
    stale.outfileJson = true;
    QCOMPARE(int(::validateOptions(stale).code), int(LaunchValidator::LaunchError::None));
}

int main(int argc, char *argv[])
{
    TestLaunchValidator test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_launchvalidator.moc"
