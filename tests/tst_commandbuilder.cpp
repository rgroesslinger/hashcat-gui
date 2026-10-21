/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QDateTime>
#include <QSet>

#include "appconstants.h"
#include "commandbuilder.h"
#include "hashcatoptions.h"

// Everything the argument rules promise: what reaches the hashcat command
// line, in which order, and what is deliberately left out of it.
class TestCommandBuilder : public QObject
{
    Q_OBJECT

private slots:
    void defaultsProduceTheBareCommandLine();
    void noHashTypeWhileNoneIsSelected();
    void hashTypeComesBeforeTheAttackMode();
    void fileRulesWinOverGeneratedRules();
    void argumentLayout_data();
    void argumentLayout();
    void uiDefaultsAreNotRepeated();
    void hexCharsetFollowsTheMaskModes();
    void advancedOptionsAreEmitted();
    void incrementFollowsMaskModesAndTheFlag();
    void wordlistRulesFollowTheirSide();
    void wordlistOptionsFollowWordlistModes();
    void statusTimerNeedsStatus();
    void advancedShortForms();
    void shortFallsBackToLong();
    void outfileIsOptInAndExpanded();
    void expandOutfileTemplateSubstitutesUnixTimeAndHash();
    void nonAsciiArgumentsSurviveVerbatim();
    void modeRequirements_data();
    void modeRequirements();
    void parameterSpelling();
    void unknownParameterIsEmpty();
    void predicates();
};

// A default constructed HashcatOptions is the widget state of a freshly opened
// main window, and the preview of that window has to be this and nothing more.
void TestCommandBuilder::defaultsProduceTheBareCommandLine()
{
    const HashcatOptions options;
    const QStringList expected{"--attack-mode", "0"};

    QCOMPARE(CommandBuilder::build(options), expected);
}

void TestCommandBuilder::noHashTypeWhileNoneIsSelected()
{
    HashcatOptions options;
    options.hashType = -1;

    const QStringList longForm = CommandBuilder::build(options);
    QVERIFY(!longForm.contains(QStringLiteral("--hash-type")));

    // The short form of --hash-type is -m, so that one has to be left out as
    // well.
    options.useShortParameters = true;
    const QStringList shortForm = CommandBuilder::build(options);
    QVERIFY(!shortForm.contains(QStringLiteral("-m")));
}

void TestCommandBuilder::hashTypeComesBeforeTheAttackMode()
{
    HashcatOptions options;
    options.hashType = 1000;

    const QStringList longForm{"--hash-type", "1000", "--attack-mode", "0"};
    QCOMPARE(CommandBuilder::build(options), longForm);

    options.useShortParameters = true;
    const QStringList shortForm{"-m", "1000", "-a", "0"};
    QCOMPARE(CommandBuilder::build(options), shortForm);
}

void TestCommandBuilder::fileRulesWinOverGeneratedRules()
{
    HashcatOptions options;
    options.attackMode = AttackMode::Straight;
    options.rulesFiles << QStringLiteral("best64.rule");
    options.generateRules = 4;

    const QStringList arguments = CommandBuilder::build(options);
    QVERIFY(arguments.contains(QStringLiteral("best64.rule")));
    QVERIFY(!arguments.contains(QStringLiteral("--generate-rules")));
    QVERIFY(!arguments.contains(QStringLiteral("-g")));
}

// One row per attack mode. The setup deliberately mixes everything in that
// the mode may or may not accept, so the row pins down gating *and* ordering
// in one go.
void TestCommandBuilder::argumentLayout_data()
{
    QTest::addColumn<int>("mode");
    QTest::addColumn<QStringList>("expected");

    QTest::newRow("straight") << int(AttackMode::Straight)
                              << QStringList{"--attack-mode", "0",        "--rules-file",
                                             "r.rule",        "hash.txt", "dict.txt"};
    QTest::newRow("combination") << int(AttackMode::Combination)
                                 << QStringList{"--attack-mode", "1", "hash.txt", "dict.txt"};
    QTest::newRow("brute force") << int(AttackMode::BruteForce)
                                 << QStringList{"--attack-mode", "3",        "--custom-charset1",
                                                "abc",           "hash.txt", "?d?d"};
    QTest::newRow("hybrid wordlist + mask")
        << int(AttackMode::HybridWordMask)
        << QStringList{"--attack-mode", "6",   "--custom-charset1", "abc", "hash.txt",
                       "dict.txt",      "?d?d"};
    QTest::newRow("hybrid mask + wordlist")
        << int(AttackMode::HybridMaskWord)
        << QStringList{"--attack-mode", "7",    "--custom-charset1", "abc",
                       "hash.txt",      "?d?d", "dict.txt"};
    QTest::newRow("association") << int(AttackMode::Association)
                                 << QStringList{"--attack-mode", "9",        "--rules-file",
                                                "r.rule",        "hash.txt", "dict.txt"};
}

void TestCommandBuilder::argumentLayout()
{
    QFETCH(int, mode);
    QFETCH(QStringList, expected);

    HashcatOptions options;
    options.attackMode = static_cast<AttackMode>(mode);
    options.hashFile = QStringLiteral("hash.txt");
    options.wordlists << QStringLiteral("dict.txt");
    options.mask = QStringLiteral("?d?d");
    options.rulesFiles << QStringLiteral("r.rule");
    options.generateRules = 4;
    options.customCharsets[0].enabled = true;
    options.customCharsets[0].value = QStringLiteral("abc");

    QCOMPARE(CommandBuilder::build(options), expected);
}

void TestCommandBuilder::uiDefaultsAreNotRepeated()
{
    // The three fields the UI pre-fills are left out of the command, because
    // hashcat would do exactly the same with them.
    HashcatOptions options;
    options.outfileFormat = AppConstants::Defaults::OutfileFormat;
    options.backendDevices = AppConstants::Defaults::BackendDevices;
    options.segmentSize = AppConstants::Defaults::SegmentSize;

    const QStringList bare{"--attack-mode", "0"};
    QCOMPARE(CommandBuilder::build(options), bare);

    // ... and are passed as soon as they differ
    options.outfileFormat = QStringLiteral("1");
    options.backendDevices = QStringLiteral("1,2");
    options.segmentSize = 64;

    const QStringList full{"--attack-mode",     "0",   "--outfile-format", "1",
                           "--backend-devices", "1,2", "--segment-size",   "64"};
    QCOMPARE(CommandBuilder::build(options), full);
}

// --hex-charset describes masks and custom charsets. Hashcat accepts it in
// every attack mode but ignores it outside the mask modes, so the builder
// must not let a stale checkbox reach modes 0, 1 and 9.
void TestCommandBuilder::hexCharsetFollowsTheMaskModes()
{
    const QList<AttackMode> modes{
        AttackMode::Straight,       AttackMode::Combination,    AttackMode::BruteForce,
        AttackMode::HybridWordMask, AttackMode::HybridMaskWord, AttackMode::Association,
    };
    const QSet<AttackMode> maskModes{
        AttackMode::BruteForce,
        AttackMode::HybridWordMask,
        AttackMode::HybridMaskWord,
    };

    for (const AttackMode mode : modes) {
        HashcatOptions options;
        options.attackMode = mode;
        options.hexCharset = true;

        const QStringList arguments = CommandBuilder::build(options);
        QCOMPARE(arguments.contains(QStringLiteral("--hex-charset")), maskModes.contains(mode));
    }
}

// One invocation that uses every advanced option at once, so the test pins
// both the emission and the relative order of the arguments.
void TestCommandBuilder::advancedOptionsAreEmitted()
{
    HashcatOptions options;
    options.attackMode = AttackMode::HybridWordMask;
    options.force = true;
    options.session = QStringLiteral("job");
    options.runtime = 60;
    options.status = true;
    options.statusTimer = 5;
    options.potfileDisable = true;
    options.skip = QStringLiteral("100");
    options.limit = QStringLiteral("200");
    options.increment = true;
    options.incrementMin = 2;
    options.incrementMax = 8;
    options.hexCharset = true;
    options.hexSalt = true;
    options.hexWordlist = true;
    options.encodingFrom = QStringLiteral("iso-8859-15");
    options.encodingTo = QStringLiteral("utf-8");
    options.openclDeviceTypes = QStringLiteral("1,2");
    options.hwmonTempAbort = true;
    options.hwmonTempAbortCelsius = 85;
    options.hashFile = QStringLiteral("hash.txt");
    options.wordlists << QStringLiteral("dict.txt");
    options.mask = QStringLiteral("?d?d");

    const QStringList expected{
        "--attack-mode",
        "6",
        "--force",
        "--session",
        "job",
        "--runtime",
        "60",
        "--status",
        "--status-timer",
        "5",
        "--potfile-disable",
        "--skip",
        "100",
        "--limit",
        "200",
        "--increment",
        "--increment-min",
        "2",
        "--increment-max",
        "8",
        "--hex-charset",
        "--hex-salt",
        "--hex-wordlist",
        "--encoding-from",
        "iso-8859-15",
        "--encoding-to",
        "utf-8",
        "--opencl-device-types",
        "1,2",
        "--hwmon-temp-abort",
        "85",
        "hash.txt",
        "dict.txt",
        "?d?d",
    };
    QCOMPARE(CommandBuilder::build(options), expected);
}

void TestCommandBuilder::incrementFollowsMaskModesAndTheFlag()
{
    const QList<AttackMode> modes{
        AttackMode::Straight,       AttackMode::Combination,    AttackMode::BruteForce,
        AttackMode::HybridWordMask, AttackMode::HybridMaskWord, AttackMode::Association,
    };
    const QSet<AttackMode> maskModes{
        AttackMode::BruteForce,
        AttackMode::HybridWordMask,
        AttackMode::HybridMaskWord,
    };

    for (const AttackMode mode : modes) {
        HashcatOptions options;
        options.attackMode = mode;
        options.increment = true;
        options.incrementMin = 2;
        options.incrementMax = 4;

        const QStringList arguments = CommandBuilder::build(options);
        const bool emitted = arguments.contains(QStringLiteral("--increment"))
                             && arguments.contains(QStringLiteral("--increment-min"))
                             && arguments.contains(QStringLiteral("--increment-max"));
        QCOMPARE(emitted, maskModes.contains(mode));
    }

    // The bounds are part of the increment option; without the flag they
    // would only be noise in the preview.
    HashcatOptions options;
    options.attackMode = AttackMode::HybridWordMask;
    options.incrementMin = 2;
    options.incrementMax = 4;
    const QStringList arguments = CommandBuilder::build(options);
    QVERIFY(!arguments.contains(QStringLiteral("--increment-min")));
    QVERIFY(!arguments.contains(QStringLiteral("--increment-max")));
}

void TestCommandBuilder::wordlistRulesFollowTheirSide()
{
    const QList<AttackMode> modes{
        AttackMode::Straight,       AttackMode::Combination,    AttackMode::BruteForce,
        AttackMode::HybridWordMask, AttackMode::HybridMaskWord, AttackMode::Association,
    };
    const QSet<AttackMode> leftModes{
        AttackMode::Straight,
        AttackMode::Combination,
        AttackMode::HybridWordMask,
        AttackMode::Association,
    };
    const QSet<AttackMode> rightModes{
        AttackMode::Combination,
        AttackMode::HybridMaskWord,
    };

    for (const AttackMode mode : modes) {
        HashcatOptions options;
        options.attackMode = mode;
        options.ruleLeft = QStringLiteral("c");
        options.ruleRight = QStringLiteral("^x");

        const QStringList arguments = CommandBuilder::build(options);
        const bool usesLeft = leftModes.contains(mode);
        const bool usesRight = rightModes.contains(mode);

        QCOMPARE(arguments.contains(QStringLiteral("--rule-left")), usesLeft);
        QCOMPARE(arguments.contains(QStringLiteral("--rule-right")), usesRight);

        if (usesLeft) {
            QCOMPARE(arguments.value(arguments.indexOf(QStringLiteral("--rule-left")) + 1),
                     QStringLiteral("c"));
        }
        if (usesRight) {
            QCOMPARE(arguments.value(arguments.indexOf(QStringLiteral("--rule-right")) + 1),
                     QStringLiteral("^x"));
        }
    }
}

void TestCommandBuilder::wordlistOptionsFollowWordlistModes()
{
    const QList<AttackMode> modes{
        AttackMode::Straight,       AttackMode::Combination,    AttackMode::BruteForce,
        AttackMode::HybridWordMask, AttackMode::HybridMaskWord, AttackMode::Association,
    };
    const QSet<AttackMode> wordlistModes{
        AttackMode::Straight,       AttackMode::Combination, AttackMode::HybridWordMask,
        AttackMode::HybridMaskWord, AttackMode::Association,
    };

    for (const AttackMode mode : modes) {
        HashcatOptions options;
        options.attackMode = mode;
        options.hexWordlist = true;
        options.encodingFrom = QStringLiteral("iso-8859-15");
        options.encodingTo = QStringLiteral("utf-8");

        const QStringList arguments = CommandBuilder::build(options);
        const bool emitted = arguments.contains(QStringLiteral("--hex-wordlist"))
                             && arguments.contains(QStringLiteral("--encoding-from"))
                             && arguments.contains(QStringLiteral("--encoding-to"));
        QCOMPARE(emitted, wordlistModes.contains(mode));
    }
}

void TestCommandBuilder::statusTimerNeedsStatus()
{
    HashcatOptions options;
    options.statusTimer = 10;

    // The interval is never emitted without the flag it belongs to.
    QStringList arguments = CommandBuilder::build(options);
    QVERIFY(!arguments.contains(QStringLiteral("--status")));
    QVERIFY(!arguments.contains(QStringLiteral("--status-timer")));

    options.status = true;
    // hashcat's own default interval is 10 s, so the GUI always passes its
    // own - even at the default value the spin box starts with.
    options.statusTimer = AppConstants::Defaults::StatusTimer;
    arguments = CommandBuilder::build(options);
    QVERIFY(arguments.contains(QStringLiteral("--status")));
    QVERIFY(arguments.contains(QStringLiteral("--status-timer")));
    QCOMPARE(arguments.value(arguments.indexOf(QStringLiteral("--status-timer")) + 1),
             QString::number(AppConstants::Defaults::StatusTimer));

    options.statusTimer = 1;
    arguments = CommandBuilder::build(options);
    QVERIFY(arguments.contains(QStringLiteral("--status")));
    QVERIFY(arguments.contains(QStringLiteral("--status-timer")));
    QCOMPARE(arguments.value(arguments.indexOf(QStringLiteral("--status-timer")) + 1),
             QStringLiteral("1"));
}

void TestCommandBuilder::advancedShortForms()
{
    HashcatOptions options;
    options.useShortParameters = true;
    options.attackMode = AttackMode::Combination;
    options.force = true;
    options.session = QStringLiteral("job");
    options.runtime = 60;
    options.status = true;
    options.statusTimer = 5;
    options.potfileDisable = true;
    options.ruleLeft = QStringLiteral("c");
    options.ruleRight = QStringLiteral("^x");
    options.skip = QStringLiteral("100");
    options.limit = QStringLiteral("200");
    options.hexWordlist = true;
    options.encodingFrom = QStringLiteral("iso-8859-15");
    options.encodingTo = QStringLiteral("utf-8");
    options.openclDeviceTypes = QStringLiteral("1,2");
    options.hwmonTempAbort = true;
    options.hwmonTempAbortCelsius = 85;

    QStringList expected{
        "-a",
        "1",
        "--force",
        "--session",
        "job",
        "--runtime",
        "60",
        "--status",
        "--status-timer",
        "5",
        "--potfile-disable",
        "-j",
        "c",
        "-k",
        "^x",
        "-s",
        "100",
        "-l",
        "200",
        "--hex-wordlist",
        "--encoding-from",
        "iso-8859-15",
        "--encoding-to",
        "utf-8",
        "-D",
        "1,2",
        "--hwmon-temp-abort",
        "85",
    };
    QCOMPARE(CommandBuilder::build(options), expected);

    // --increment is the one option with a short form that the combination
    // mode above cannot exercise.
    options.attackMode = AttackMode::BruteForce;
    options.increment = true;
    QVERIFY(CommandBuilder::build(options).contains(QStringLiteral("-i")));
}

void TestCommandBuilder::shortFallsBackToLong()
{
    HashcatOptions options;
    options.useShortParameters = true;
    options.hashType = 1000;
    options.remove = true;
    options.ignoreUsername = true;
    options.workloadProfile = QStringLiteral("2");
    options.cpuAffinity = QStringLiteral("1-4");

    // --cpu-affinity has no short form; asking for one must not produce an
    // empty argument, which hashcat would read as a stray operand
    const QStringList expected{"-m", "1000",           "-a", "0", "--remove", "--username", "-w",
                               "2",  "--cpu-affinity", "1-4"};
    const QStringList arguments = CommandBuilder::build(options);
    QCOMPARE(arguments, expected);

    for (const QString &argument : arguments) {
        QVERIFY2(!argument.isEmpty(), "build() produced an empty argument");
    }
}

void TestCommandBuilder::outfileIsOptInAndExpanded()
{
    HashcatOptions options;
    options.hashFile = QStringLiteral("/tmp/example.hash");
    options.outfile = QStringLiteral("cracked.txt");

    // Ticking nothing means hashcat writes where it wants to, even though the
    // field already holds a suggestion
    QVERIFY(!CommandBuilder::build(options).contains(QStringLiteral("cracked.txt")));

    options.outfileEnabled = true;
    QStringList arguments = CommandBuilder::build(options);
    QCOMPARE(arguments.value(arguments.indexOf(QStringLiteral("--outfile")) + 1),
             QStringLiteral("cracked.txt"));

    // <hash> is the hash file's name, not its path
    options.outfile = QStringLiteral("out/<hash>.txt");
    arguments = CommandBuilder::build(options);
    QCOMPARE(arguments.value(arguments.indexOf(QStringLiteral("--outfile")) + 1),
             QStringLiteral("out/example.hash.txt"));
}

void TestCommandBuilder::expandOutfileTemplateSubstitutesUnixTimeAndHash()
{
    QVERIFY(CommandBuilder::expandOutfileTemplate(QStringLiteral("plain.txt"),
                                                  QStringLiteral("/tmp/whatever"))
            == QStringLiteral("plain.txt"));

    const qint64 before = QDateTime::currentMSecsSinceEpoch() / 1000;
    const QString stamped = CommandBuilder::expandOutfileTemplate(QStringLiteral("<unixtime>"),
                                                                  QStringLiteral("/tmp/whatever"));
    const qint64 after = QDateTime::currentMSecsSinceEpoch() / 1000;

    bool isNumber = false;
    const qint64 unixTime = stamped.toLongLong(&isNumber);
    QVERIFY2(isNumber, qPrintable(stamped));
    QVERIFY(unixTime >= before);
    QVERIFY(unixTime <= after);

    // the placeholder is matched without regard to case, the file name is not
    QCOMPARE(CommandBuilder::expandOutfileTemplate(QStringLiteral("cracked_<HASH>.txt"),
                                                   QStringLiteral("/tmp/My.Hash.TXT")),
             QStringLiteral("cracked_My.Hash.TXT.txt"));
}

// A mask describes a password, and a password may hold anything - including
// characters outside ASCII. The mask, charset and paths have to reach the
// argument vector unchanged, as the UTF-8 bytes the operating system hands to
// hashcat. The literals are \u escapes, so the test does not depend on its own
// source encoding.
void TestCommandBuilder::nonAsciiArgumentsSurviveVerbatim()
{
    const QString mask = QStringLiteral("Passw\u00f6rd?d");       // Passwörd?d
    const QString charset = QStringLiteral("\u00e4\u00f6\u00fc"); // äöü
    const QString dictionary = QStringLiteral("/tmp/W\u00f6rterbuch.txt");
    const QString hashFile = QStringLiteral("/tmp/H\u00e4sch.txt");
    const QString outfile = QStringLiteral("Ergebnis_<hash>_\u00e4.txt");

    HashcatOptions options;
    // Hybrid wordlist + mask reads a wordlist, a mask and a custom charset,
    // so one invocation exercises all three non-ASCII inputs.
    options.attackMode = AttackMode::HybridWordMask;
    options.hashFile = hashFile;
    options.mask = mask;
    options.wordlists << dictionary;
    options.customCharsets[0].enabled = true;
    options.customCharsets[0].value = charset;
    options.outfileEnabled = true;
    options.outfile = outfile;

    const QStringList arguments = CommandBuilder::build(options);
    QVERIFY(arguments.contains(charset));
    QVERIFY(arguments.contains(mask));
    QVERIFY(arguments.contains(dictionary));
    QVERIFY(arguments.contains(hashFile));
    // <hash> is the hash file's name, non-ASCII included
    QVERIFY(arguments.contains(QStringLiteral("Ergebnis_H\u00e4sch.txt_\u00e4.txt")));

    // The bytes the operating system will see are real UTF-8: "ö" is C3 B6,
    // not the single Latin-1 byte F6 a local-8-bit conversion would produce.
    const QByteArray commandLine = arguments.join(QLatin1Char(' ')).toUtf8();
    QVERIFY(commandLine.contains(QByteArray::fromHex("c3b6")));
}

// What each attack mode needs from the user. These numbers are what the
// launch validation and the hint under the mode selector work with.
void TestCommandBuilder::modeRequirements_data()
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

void TestCommandBuilder::modeRequirements()
{
    QFETCH(int, mode);
    QFETCH(int, minWordlists);
    QFETCH(int, maxWordlists);
    QFETCH(bool, needsMask);

    const CommandBuilder::ModeRequirements requirements =
        CommandBuilder::modeRequirements(static_cast<AttackMode>(mode));

    QCOMPARE(requirements.minWordlists, minWordlists);
    QCOMPARE(requirements.maxWordlists, maxWordlists);
    QCOMPARE(requirements.needsMask, needsMask);
}

void TestCommandBuilder::parameterSpelling()
{
    using Parameter = CommandBuilder::Parameter;

    QCOMPARE(CommandBuilder::parameterName(Parameter::AttackMode), QStringLiteral("--attack-mode"));
    QCOMPARE(CommandBuilder::parameterName(Parameter::AttackMode, true), QStringLiteral("-a"));

    // --cpu-affinity has no short form; asking for one must fall back to the
    // long one rather than produce an empty argument
    QCOMPARE(CommandBuilder::parameterName(Parameter::CpuAffinity, true),
             QStringLiteral("--cpu-affinity"));
    QCOMPARE(CommandBuilder::parameterName(Parameter::CpuAffinity, false),
             QStringLiteral("--cpu-affinity"));
    QCOMPARE(CommandBuilder::parameterName(Parameter::HexCharset, true),
             QStringLiteral("--hex-charset"));

    // The short forms that differ from the parameter name are easy to get
    // wrong, so pin them here as well.
    QCOMPARE(CommandBuilder::parameterName(Parameter::Increment, true), QStringLiteral("-i"));
    QCOMPARE(CommandBuilder::parameterName(Parameter::Skip, true), QStringLiteral("-s"));
    QCOMPARE(CommandBuilder::parameterName(Parameter::Limit, true), QStringLiteral("-l"));
    QCOMPARE(CommandBuilder::parameterName(Parameter::OpenclDeviceTypes, true),
             QStringLiteral("-D"));
    QCOMPARE(CommandBuilder::parameterName(Parameter::RuleLeft, true), QStringLiteral("-j"));
    QCOMPARE(CommandBuilder::parameterName(Parameter::RuleRight, true), QStringLiteral("-k"));

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
        QVERIFY2(!CommandBuilder::parameterName(key).isEmpty(), "an enum has no long spelling");
        QVERIFY2(!CommandBuilder::parameterName(key, true).isEmpty(),
                 "an enum has no spelling at all");
    }
}

void TestCommandBuilder::unknownParameterIsEmpty()
{
    QVERIFY(CommandBuilder::parameterName(static_cast<CommandBuilder::Parameter>(4711)).isEmpty());
}

void TestCommandBuilder::predicates()
{
    using Mode = AttackMode;

    QVERIFY(CommandBuilder::attackUsesMask(Mode::BruteForce));
    QVERIFY(CommandBuilder::attackUsesMask(Mode::HybridWordMask));
    QVERIFY(CommandBuilder::attackUsesMask(Mode::HybridMaskWord));
    QVERIFY(!CommandBuilder::attackUsesMask(Mode::Straight));
    QVERIFY(!CommandBuilder::attackUsesMask(Mode::Combination));
    QVERIFY(!CommandBuilder::attackUsesMask(Mode::Association));

    QVERIFY(!CommandBuilder::attackUsesWordlists(Mode::BruteForce));
    QVERIFY(CommandBuilder::attackUsesWordlists(Mode::Straight));
    QVERIFY(CommandBuilder::attackUsesWordlists(Mode::Combination));
    QVERIFY(CommandBuilder::attackUsesWordlists(Mode::HybridWordMask));
    QVERIFY(CommandBuilder::attackUsesWordlists(Mode::HybridMaskWord));
    QVERIFY(CommandBuilder::attackUsesWordlists(Mode::Association));

    QVERIFY(CommandBuilder::attackUsesRules(Mode::Straight));
    QVERIFY(CommandBuilder::attackUsesRules(Mode::Association));
    QVERIFY(!CommandBuilder::attackUsesRules(Mode::Combination));
    QVERIFY(!CommandBuilder::attackUsesRules(Mode::BruteForce));
    QVERIFY(!CommandBuilder::attackUsesRules(Mode::HybridWordMask));
    QVERIFY(!CommandBuilder::attackUsesRules(Mode::HybridMaskWord));

    QVERIFY(CommandBuilder::attackUsesRuleLeft(Mode::Straight));
    QVERIFY(CommandBuilder::attackUsesRuleLeft(Mode::Combination));
    QVERIFY(!CommandBuilder::attackUsesRuleLeft(Mode::BruteForce));
    QVERIFY(CommandBuilder::attackUsesRuleLeft(Mode::HybridWordMask));
    QVERIFY(!CommandBuilder::attackUsesRuleLeft(Mode::HybridMaskWord));
    QVERIFY(CommandBuilder::attackUsesRuleLeft(Mode::Association));

    QVERIFY(!CommandBuilder::attackUsesRuleRight(Mode::Straight));
    QVERIFY(CommandBuilder::attackUsesRuleRight(Mode::Combination));
    QVERIFY(!CommandBuilder::attackUsesRuleRight(Mode::BruteForce));
    QVERIFY(!CommandBuilder::attackUsesRuleRight(Mode::HybridWordMask));
    QVERIFY(CommandBuilder::attackUsesRuleRight(Mode::HybridMaskWord));
    QVERIFY(!CommandBuilder::attackUsesRuleRight(Mode::Association));
}

int main(int argc, char *argv[])
{
    TestCommandBuilder test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_commandbuilder.moc"
