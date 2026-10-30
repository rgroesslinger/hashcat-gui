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
    void hashRecipeFollowsItsMode();
    void tierTwoOptionsFollowTheirConditions();
    void fileRulesWinOverGeneratedRules();
    void argumentLayout_data();
    void argumentLayout();
    void uiDefaultsAreNotRepeated();
    void hexCharsetFollowsTheCharsetModes();
    void advancedOptionsAreEmitted();
    void incrementFollowsItsOwnModesAndTheFlag();
    void wordlistRulesFollowTheirSide();
    void wordlistOptionsFollowWordlistModes();
    void statusTimerNeedsStatus();
    void advancedShortForms();
    void shortFallsBackToLong();
    void outfileIsOptInAndExpanded();
    void expandOutfileTemplateSubstitutesUnixTimeAndHash();
    void nonAsciiArgumentsSurviveVerbatim();
    void maskMarkersAreCounted();
    void positionalInputsFollowTheMode();
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

// --hash-recipe belongs to hash mode 4000; the recipe is one argv token and
// keeps its spaces.
void TestCommandBuilder::hashRecipeFollowsItsMode()
{
    HashcatOptions options;
    options.hashType = AppConstants::Hashcat::RecipeHashType;
    options.hashRecipe = QStringLiteral("md5(md5(salt) . pass)");

    const QStringList withRecipe{"--hash-type",           "4000",          "--hash-recipe",
                                 "md5(md5(salt) . pass)", "--attack-mode", "0"};
    QCOMPARE(CommandBuilder::build(options), withRecipe);

    options.useShortParameters = true;
    const QStringList shortForm{"-m", "4000", "--hash-recipe", "md5(md5(salt) . pass)", "-a", "0"};
    QCOMPARE(CommandBuilder::build(options), shortForm);

    // A recipe for any other hash type is left out: hashcat refuses the flag
    // there, and a session file can store a stale value.
    options.useShortParameters = false;
    options.hashType = 1000;
    const QStringList stale{"--hash-type", "1000", "--attack-mode", "0"};
    QCOMPARE(CommandBuilder::build(options), stale);

    // Whitespace is no recipe either.
    options.hashType = AppConstants::Hashcat::RecipeHashType;
    options.hashRecipe = QStringLiteral("   ");
    const QStringList blank{"--hash-type", "4000", "--attack-mode", "0"};
    QCOMPARE(CommandBuilder::build(options), blank);
}

// The tier 2 options: --rules-concat only next to the files it joins,
// --outfile-json instead of --outfile-format, --cache-path verbatim.
void TestCommandBuilder::tierTwoOptionsFollowTheirConditions()
{
    // --rules-concat joins the -r files above; without files it has nothing
    // to join and hashcat answers with its usage line.
    {
        HashcatOptions options;
        options.attackMode = AttackMode::Straight;
        options.hashFile = QStringLiteral("hash.txt");
        options.wordlists << QStringLiteral("dict.txt");
        options.rulesFiles << QStringLiteral("one.rule") << QStringLiteral("two.rule");
        options.rulesConcat = true;

        const QStringList arguments = CommandBuilder::build(options);
        const qsizetype concat = arguments.indexOf(QStringLiteral("--rules-concat"));
        QVERIFY(concat >= 0);
        // The join flag comes after the files it joins.
        QVERIFY(concat > arguments.indexOf(QStringLiteral("two.rule")));

        options.rulesFiles.clear();
        options.generateRules = 4;
        QVERIFY(!CommandBuilder::build(options).contains(QStringLiteral("--rules-concat")));
    }

    // JSON wins over the format: the two are exclusive upstream, and a
    // session can hold both.
    {
        HashcatOptions options;
        options.hashFile = QStringLiteral("hash.txt");
        options.outfileEnabled = true;
        options.outfile = QStringLiteral("out.txt");
        options.outfileFormat = QStringLiteral("1,2,3");
        options.outfileJson = true;

        const QStringList arguments = CommandBuilder::build(options);
        QVERIFY(arguments.contains(QStringLiteral("--outfile-json")));
        QVERIFY(!arguments.contains(QStringLiteral("--outfile-format")));
        QVERIFY(!arguments.contains(QStringLiteral("1,2,3")));
    }

    // Without JSON the format keeps its old place.
    {
        HashcatOptions options;
        options.hashFile = QStringLiteral("hash.txt");
        options.outfileEnabled = true;
        options.outfile = QStringLiteral("out.txt");
        options.outfileFormat = QStringLiteral("1,2,3");

        const QStringList arguments = CommandBuilder::build(options);
        QVERIFY(!arguments.contains(QStringLiteral("--outfile-json")));
        QCOMPARE(arguments.value(arguments.indexOf(QStringLiteral("--outfile-format")) + 1),
                 QStringLiteral("1,2,3"));
    }

    // The cache path is passed as one token; empty stays out.
    {
        HashcatOptions options;
        options.hashFile = QStringLiteral("hash.txt");

        QVERIFY(!CommandBuilder::build(options).contains(QStringLiteral("--cache-path")));

        options.cachePath = QStringLiteral("/tmp/hc-cache");
        const QStringList arguments = CommandBuilder::build(options);
        QCOMPARE(arguments.value(arguments.indexOf(QStringLiteral("--cache-path")) + 1),
                 QStringLiteral("/tmp/hc-cache"));
    }
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

    // Rules apply in every attack mode since hashcat 8.0.0, so every row
    // carries the -r file and none of them falls back to -g.
    QTest::newRow("straight") << int(AttackMode::Straight)
                              << QStringList{"--attack-mode", "0",        "--rules-file",
                                             "r.rule",        "hash.txt", "dict.txt"};
    QTest::newRow("combination") << int(AttackMode::Combination)
                                 << QStringList{"--attack-mode", "1",        "--rules-file",
                                                "r.rule",        "hash.txt", "dict.txt"};
    QTest::newRow("brute force") << int(
        AttackMode::BruteForce) << QStringList{"--attack-mode",     "3",   "--rules-file", "r.rule",
                                               "--custom-charset1", "abc", "hash.txt",     "?d?d"};
    // PCFG: -r and the custom charsets apply, the wordlist and the modeless
    // mask do not (mode 4's mask is its own mask= setting).
    QTest::newRow("pcfg") << int(AttackMode::Pcfg)
                          << QStringList{"--attack-mode",     "4",   "--rules-file", "r.rule",
                                         "--custom-charset1", "abc", "hash.txt"};
    // Table: -r applies, the table feed reads the wordlist itself and takes
    // no custom charset or mask.
    QTest::newRow("table") << int(AttackMode::Table)
                           << QStringList{"--attack-mode", "5",        "--rules-file",
                                          "r.rule",        "hash.txt", "dict.txt"};
    QTest::newRow("hybrid wordlist + mask")
        << int(AttackMode::HybridWordMask)
        << QStringList{"--attack-mode", "6",        "--rules-file", "r.rule", "--custom-charset1",
                       "abc",           "hash.txt", "dict.txt",     "?d?d"};
    QTest::newRow("hybrid mask + wordlist")
        << int(AttackMode::HybridMaskWord)
        << QStringList{"--attack-mode", "7",        "--rules-file", "r.rule",  "--custom-charset1",
                       "abc",           "hash.txt", "?d?d",         "dict.txt"};
    // Generic feed: -r applies; the wordlist list and the mask do not, and
    // the feed's own arguments are not part of this setup.
    QTest::newRow("generic") << int(AttackMode::Generic)
                             << QStringList{"--attack-mode", "8", "--rules-file", "r.rule",
                                            "hash.txt"};
    QTest::newRow("association") << int(AttackMode::Association)
                                 << QStringList{"--attack-mode", "9",        "--rules-file",
                                                "r.rule",        "hash.txt", "dict.txt"};
    // Hybrid 12: the mask comes before the wordlists, -1 and -r apply, and
    // -i is refused by the mode (so it is never emitted).
    QTest::newRow("hybrid") << int(AttackMode::Hybrid)
                            << QStringList{"--attack-mode",     "12",  "--rules-file", "r.rule",
                                           "--custom-charset1", "abc", "hash.txt",     "?d?d",
                                           "dict.txt"};
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
    // The fields the UI pre-fills are left out of the command, because
    // hashcat would do exactly the same with them.
    HashcatOptions options;
    options.outfileFormat = AppConstants::Defaults::OutfileFormat;
    options.backendDevices = AppConstants::Defaults::BackendDevices;

    const QStringList bare{"--attack-mode", "0"};
    QCOMPARE(CommandBuilder::build(options), bare);

    // ... and are passed as soon as they differ
    options.outfileFormat = QStringLiteral("1");
    options.backendDevices = QStringLiteral("1,2");

    const QStringList full{"--attack-mode",     "0",  "--outfile-format", "1",
                           "--backend-devices", "1,2"};
    QCOMPARE(CommandBuilder::build(options), full);
}

// --hex-charset describes masks and custom charsets. The PCFG feed takes the
// same markers in its mask= setting, so mode 4 emits it as well; mode 8 has
// no mask field, and in the other modes hashcat would accept and ignore it.
void TestCommandBuilder::hexCharsetFollowsTheCharsetModes()
{
    const QList<AttackMode> modes{
        AttackMode::Straight,       AttackMode::Combination, AttackMode::BruteForce,
        AttackMode::Pcfg,           AttackMode::Table,       AttackMode::HybridWordMask,
        AttackMode::HybridMaskWord, AttackMode::Generic,     AttackMode::Association,
        AttackMode::Hybrid,
    };
    const QSet<AttackMode> charsetModes{
        AttackMode::BruteForce,     AttackMode::Pcfg,   AttackMode::HybridWordMask,
        AttackMode::HybridMaskWord, AttackMode::Hybrid,
    };

    for (const AttackMode mode : modes) {
        HashcatOptions options;
        options.attackMode = mode;
        options.hexCharset = true;

        const QStringList arguments = CommandBuilder::build(options);
        QCOMPARE(arguments.contains(QStringLiteral("--hex-charset")), charsetModes.contains(mode));
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

void TestCommandBuilder::incrementFollowsItsOwnModesAndTheFlag()
{
    const QList<AttackMode> modes{
        AttackMode::Straight,       AttackMode::Combination, AttackMode::BruteForce,
        AttackMode::Pcfg,           AttackMode::Table,       AttackMode::HybridWordMask,
        AttackMode::HybridMaskWord, AttackMode::Generic,     AttackMode::Association,
        AttackMode::Hybrid,
    };
    // Mode 12 conflicts with -i upstream, and mode 1 takes it there but has
    // no mask in the GUI, so the flag is offered in the three mask modes
    // where a mask is in use.
    const QSet<AttackMode> incrementModes{
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
        QCOMPARE(emitted, incrementModes.contains(mode));
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
        AttackMode::Straight,       AttackMode::Combination, AttackMode::BruteForce,
        AttackMode::Pcfg,           AttackMode::Table,       AttackMode::HybridWordMask,
        AttackMode::HybridMaskWord, AttackMode::Generic,     AttackMode::Association,
        AttackMode::Hybrid,
    };
    const QSet<AttackMode> leftModes{
        AttackMode::Straight,       AttackMode::Combination, AttackMode::HybridWordMask,
        AttackMode::HybridMaskWord, AttackMode::Generic,     AttackMode::Association,
        AttackMode::Hybrid,
    };
    const QSet<AttackMode> rightModes{
        AttackMode::Combination,
        AttackMode::HybridMaskWord,
        AttackMode::Hybrid,
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
        AttackMode::Straight,       AttackMode::Combination, AttackMode::BruteForce,
        AttackMode::Pcfg,           AttackMode::Table,       AttackMode::HybridWordMask,
        AttackMode::HybridMaskWord, AttackMode::Generic,     AttackMode::Association,
        AttackMode::Hybrid,
    };
    // The table feed reads its wordlist itself, but takes both the hex option
    // and the encoding pair like every other mode that reads one; the
    // mask-only modes read no wordlist at all.
    const QSet<AttackMode> hexWordlistModes{
        AttackMode::Straight,       AttackMode::Combination,    AttackMode::Table,
        AttackMode::HybridWordMask, AttackMode::HybridMaskWord, AttackMode::Generic,
        AttackMode::Association,    AttackMode::Hybrid,
    };
    const QSet<AttackMode> encodingModes{
        AttackMode::Straight,       AttackMode::Combination,    AttackMode::Table,
        AttackMode::HybridWordMask, AttackMode::HybridMaskWord, AttackMode::Generic,
        AttackMode::Association,    AttackMode::Hybrid,
    };

    for (const AttackMode mode : modes) {
        HashcatOptions options;
        options.attackMode = mode;
        options.hexWordlist = true;
        options.encodingFrom = QStringLiteral("iso-8859-15");
        options.encodingTo = QStringLiteral("utf-8");

        const QStringList arguments = CommandBuilder::build(options);
        QCOMPARE(arguments.contains(QStringLiteral("--hex-wordlist")),
                 hexWordlistModes.contains(mode));
        QCOMPARE(arguments.contains(QStringLiteral("--encoding-from")),
                 encodingModes.contains(mode));
        QCOMPARE(arguments.contains(QStringLiteral("--encoding-to")), encodingModes.contains(mode));
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
    options.openclDeviceTypes = QStringLiteral("1");
    options.cpuAffinity = QStringLiteral("1-4");

    // --cpu-affinity has no short form; asking for one must not produce an
    // empty argument, which hashcat would read as a stray operand
    const QStringList expected{"-m",         "1000",           "-a",  "0",  "--remove",
                               "--username", "--cpu-affinity", "1-4", "-D", "1"};
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

// ?w and ?q name word positions in the mask of attack mode 12. They are not
// simply the question marks in the mask: ?? is a literal, ?W and ?Q are not
// the markers, and every other ?x is a charset token.
void TestCommandBuilder::maskMarkersAreCounted()
{
    const auto markers = [](const QString &mask) { return CommandBuilder::maskMarkers(mask); };

    // No markers at all.
    QCOMPARE(markers(QString()).word, 0);
    QCOMPARE(markers(QString()).second, 0);
    QCOMPARE(markers(QStringLiteral("?d?d?d")).word, 0);
    QCOMPARE(markers(QStringLiteral("?W?Q")).word, 0);
    QCOMPARE(markers(QStringLiteral("?W?Q")).second, 0);

    // The word position.
    QCOMPARE(markers(QStringLiteral("?w")).word, 1);
    QCOMPARE(markers(QStringLiteral("?w")).second, 0);

    // The second word after the first.
    auto both = markers(QStringLiteral("?w?q"));
    QCOMPARE(both.word, 1);
    QCOMPARE(both.second, 1);
    QVERIFY(!both.secondBeforeWord);

    // A ?q before the first ?w has no first word to follow; hashcat refuses
    // that mask, and the launch validation says so before hashcat runs.
    auto reversed = markers(QStringLiteral("?q?w"));
    QCOMPARE(reversed.word, 1);
    QCOMPARE(reversed.second, 1);
    QVERIFY(reversed.secondBeforeWord);

    // More than one ?q; only the first one matters for the flag.
    auto twice = markers(QStringLiteral("?w?q?q"));
    QCOMPARE(twice.word, 1);
    QCOMPARE(twice.second, 2);
    QVERIFY(!twice.secondBeforeWord);

    // ?? is a literal question mark, so neither pair holds a marker.
    QCOMPARE(markers(QStringLiteral("??w")).word, 0);
    QCOMPARE(markers(QStringLiteral("??w")).second, 0);
    QCOMPARE(markers(QStringLiteral("??q")).second, 0);

    // A custom charset token before the marker is no second word.
    auto custom = markers(QStringLiteral("?1?w"));
    QCOMPARE(custom.word, 1);
    QCOMPARE(custom.second, 0);
    QVERIFY(!custom.secondBeforeWord);

    // A mask of markers, mixed with plain characters.
    auto mixed = markers(QStringLiteral("?d?w?d?q"));
    QCOMPARE(mixed.word, 1);
    QCOMPARE(mixed.second, 1);
    QVERIFY(!mixed.secondBeforeWord);

    // A trailing ? stands for itself and starts no token.
    QCOMPARE(markers(QStringLiteral("abc?")).word, 0);
}

// The positional tail is what hashcat parses per attack mode, so its order
// is part of the command's meaning: the table feed stops at the first
// key=value token, and the settings of mode 9 replace the wordlist.
void TestCommandBuilder::positionalInputsFollowTheMode()
{
    // An empty PCFG run names neither a ruleset nor a setting: the bundled
    // default-passwords ruleset is implicit.
    {
        HashcatOptions options;
        options.attackMode = AttackMode::Pcfg;
        options.hashFile = QStringLiteral("hash.txt");

        const QStringList expected{"--attack-mode", "4", "hash.txt"};
        QCOMPARE(CommandBuilder::build(options), expected);
    }

    // PCFG: the rulesets come before the settings, and every setting is one
    // argv token.
    {
        HashcatOptions options;
        options.attackMode = AttackMode::Pcfg;
        options.hashFile = QStringLiteral("hash.txt");
        options.pcfgRulesets << QStringLiteral("one.pcfg") << QStringLiteral("two.pcfg");
        options.pcfgScale = QStringLiteral("2");
        options.pcfgWeights = QStringLiteral("2:1");
        options.pcfgMask = QStringLiteral("?u?l?l?l");
        options.pcfgExtraSettings << QStringLiteral("omen=1");

        const QStringList expected{"--attack-mode", "4",       "hash.txt",    "one.pcfg",
                                   "two.pcfg",      "scale=2", "weights=2:1", "mask=?u?l?l?l",
                                   "omen=1"};
        QCOMPARE(CommandBuilder::build(options), expected);
    }

    // An empty ruleset list means the one bundled default, so the settings
    // follow the hash directly.
    {
        HashcatOptions options;
        options.attackMode = AttackMode::Pcfg;
        options.hashFile = QStringLiteral("hash.txt");
        options.pcfgCostmax = QStringLiteral("32");

        const QStringList expected{"--attack-mode", "4", "hash.txt", "costmax=32"};
        QCOMPARE(CommandBuilder::build(options), expected);
    }

    // Table: the wordlist, then every table, then the settings - a setting
    // before the table list is the form hashcat answers with its usage line.
    {
        HashcatOptions options;
        options.attackMode = AttackMode::Table;
        options.hashFile = QStringLiteral("hash.txt");
        options.wordlists << QStringLiteral("dict.txt");
        options.tableFiles << QStringLiteral("table1.txt") << QStringLiteral("table2.txt");
        options.tableIdentity = QStringLiteral("0");
        options.tableCap = QStringLiteral("4294967295");
        options.tableExtraSettings << QStringLiteral("more=1");

        const QStringList expected{"--attack-mode", "5",          "hash.txt",   "dict.txt",
                                   "table1.txt",    "table2.txt", "identity=0", "cap=4294967295",
                                   "more=1"};
        QCOMPARE(CommandBuilder::build(options), expected);
    }

    // Generic feed: the feed name and its arguments, in order. An empty name
    // must not reach the command line as an empty token.
    {
        HashcatOptions options;
        options.attackMode = AttackMode::Generic;
        options.hashFile = QStringLiteral("hash.txt");
        options.feedName = QStringLiteral("wordlist");
        options.feedArguments << QStringLiteral("dict.txt") << QStringLiteral("second.txt");

        const QStringList expected{"--attack-mode", "8",        "hash.txt",
                                   "wordlist",      "dict.txt", "second.txt"};
        QCOMPARE(CommandBuilder::build(options), expected);

        options.feedName.clear();
        const QStringList nameless{"--attack-mode", "8", "hash.txt", "dict.txt", "second.txt"};
        QCOMPARE(CommandBuilder::build(options), nameless);

        // A feed that takes no arguments: the tail is the name alone.
        options.feedName = QStringLiteral("random");
        options.feedArguments.clear();
        const QStringList argumentless{"--attack-mode", "8", "hash.txt", "random"};
        QCOMPARE(CommandBuilder::build(options), argumentless);
    }

    // Association without a wordlist: the settings follow the hash, each as
    // its own argv element - splitting the settings line must never merge
    // tokens into one argument.
    {
        HashcatOptions options;
        options.attackMode = AttackMode::Association;
        options.hashFile = QStringLiteral("hash.txt");
        options.associationSettings << QStringLiteral("phases=words,rules")
                                    << QStringLiteral("rules.rulemax=300");

        const QStringList expected{"--attack-mode", "9", "hash.txt", "phases=words,rules",
                                   "rules.rulemax=300"};
        QCOMPARE(CommandBuilder::build(options), expected);
    }

    // Association with a wordlist: the tail is the wordlist and nothing
    // else; the GUI refuses the combination with settings.
    {
        HashcatOptions options;
        options.attackMode = AttackMode::Association;
        options.hashFile = QStringLiteral("hash.txt");
        options.wordlists << QStringLiteral("dict.txt");

        const QStringList expected{"--attack-mode", "9", "hash.txt", "dict.txt"};
        QCOMPARE(CommandBuilder::build(options), expected);
    }

    // Hybrid: the mask is the first positional after the hash, the wordlists
    // follow it - the ?w in the mask names where a word goes.
    {
        HashcatOptions options;
        options.attackMode = AttackMode::Hybrid;
        options.hashFile = QStringLiteral("hash.txt");
        options.mask = QStringLiteral("?w?d");
        options.wordlists << QStringLiteral("a.txt") << QStringLiteral("b.txt");

        const QStringList expected{"--attack-mode", "12", "hash.txt", "?w?d", "a.txt", "b.txt"};
        QCOMPARE(CommandBuilder::build(options), expected);
    }
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
    QTest::newRow("pcfg") << int(AttackMode::Pcfg) << 0 << -1 << false;
    QTest::newRow("table") << int(AttackMode::Table) << 1 << 1 << false;
    QTest::newRow("hybrid word + mask") << int(AttackMode::HybridWordMask) << 1 << -1 << true;
    QTest::newRow("hybrid mask + word") << int(AttackMode::HybridMaskWord) << 1 << -1 << true;
    QTest::newRow("generic") << int(AttackMode::Generic) << 0 << -1 << false;
    // Zero wordlists is the form that reads the words from the hash file;
    // one is the GUI's cap for the paired form, hashcat takes two.
    QTest::newRow("association") << int(AttackMode::Association) << 0 << 1 << false;
    QTest::newRow("hybrid") << int(AttackMode::Hybrid) << 1 << 2 << true;
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
        Parameter::AttackMode,
        Parameter::BackendDevices,
        Parameter::CachePath,
        Parameter::CpuAffinity,
        Parameter::CustomCharset1,
        Parameter::CustomCharset2,
        Parameter::CustomCharset3,
        Parameter::CustomCharset4,
        Parameter::EncodingFrom,
        Parameter::EncodingTo,
        Parameter::Force,
        Parameter::GenerateRules,
        Parameter::HashType,
        Parameter::HashRecipe,
        Parameter::HexCharset,
        Parameter::HexSalt,
        Parameter::HexWordlist,
        Parameter::HwmonTempAbort,
        Parameter::Increment,
        Parameter::IncrementMax,
        Parameter::IncrementMin,
        Parameter::Limit,
        Parameter::OpenclDeviceTypes,
        Parameter::OptimizedKernel,
        Parameter::Outfile,
        Parameter::OutfileFormat,
        Parameter::OutfileJson,
        Parameter::PotfileDisable,
        Parameter::Remove,
        Parameter::RuleLeft,
        Parameter::RuleRight,
        Parameter::RulesConcat,
        Parameter::RulesFile,
        Parameter::Runtime,
        Parameter::Session,
        Parameter::Skip,
        Parameter::SpeedOnly,
        Parameter::Status,
        Parameter::StatusTimer,
        Parameter::Username,
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

// Which attack mode consumes which part of the UI. The table is the whole
// contract: a mode added to the builder has to be written down here, and the
// main window's group boxes work from the same predicates.
void TestCommandBuilder::predicates()
{
    using Mode = AttackMode;

    struct Expectation {
        Mode mode;
        bool mask;
        bool increment;
        bool wordlists;
        bool rules;
        bool ruleLeft;
        bool ruleRight;
        bool customCharset;
        bool hexWordlist;
        bool wordlistEncoding;
    };
    const QList<Expectation> expectations{
        // mode               mask   incr   words  rules  left   right  charset hexwl  enc
        {Mode::Straight, false, false, true, true, true, false, false, true, true},
        {Mode::Combination, false, false, true, true, true, true, false, true, true},
        {Mode::BruteForce, true, true, false, true, false, false, true, false, false},
        {Mode::Pcfg, false, false, false, true, false, false, true, false, false},
        {Mode::Table, false, false, true, true, false, false, false, true, true},
        {Mode::HybridWordMask, true, true, true, true, true, false, true, true, true},
        {Mode::HybridMaskWord, true, true, true, true, true, true, true, true, true},
        {Mode::Generic, false, false, false, true, true, false, false, true, true},
        {Mode::Association, false, false, true, true, true, false, false, true, true},
        {Mode::Hybrid, true, false, true, true, true, true, true, true, true},
    };

    const auto name = [](Mode mode) { return QStringLiteral("attack mode %1").arg(int(mode)); };

    for (const Expectation &expected : expectations) {
        QVERIFY2(CommandBuilder::attackUsesMask(expected.mode) == expected.mask,
                 qPrintable(name(expected.mode)));
        QVERIFY2(CommandBuilder::attackUsesIncrement(expected.mode) == expected.increment,
                 qPrintable(name(expected.mode)));
        QVERIFY2(CommandBuilder::attackUsesWordlists(expected.mode) == expected.wordlists,
                 qPrintable(name(expected.mode)));
        QVERIFY2(CommandBuilder::attackUsesRules(expected.mode) == expected.rules,
                 qPrintable(name(expected.mode)));
        QVERIFY2(CommandBuilder::attackUsesRuleLeft(expected.mode) == expected.ruleLeft,
                 qPrintable(name(expected.mode)));
        QVERIFY2(CommandBuilder::attackUsesRuleRight(expected.mode) == expected.ruleRight,
                 qPrintable(name(expected.mode)));
        QVERIFY2(CommandBuilder::attackUsesCustomCharset(expected.mode) == expected.customCharset,
                 qPrintable(name(expected.mode)));
        QVERIFY2(CommandBuilder::attackUsesHexWordlist(expected.mode) == expected.hexWordlist,
                 qPrintable(name(expected.mode)));
        QVERIFY2(CommandBuilder::attackUsesWordlistEncoding(expected.mode)
                     == expected.wordlistEncoding,
                 qPrintable(name(expected.mode)));
    }
}

int main(int argc, char *argv[])
{
    TestCommandBuilder test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_commandbuilder.moc"
