/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QDateTime>

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
    void shortFallsBackToLong();
    void outfileIsOptInAndExpanded();
    void expandOutfileTemplateSubstitutesUnixTimeAndHash();
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

    const QStringList arguments = CommandBuilder::build(options);
    QVERIFY(!arguments.contains(QStringLiteral("--hash-type")));
    // the short form of --hash-type is -m, so check that exact token too
    QVERIFY(!arguments.contains(QStringLiteral("-m")));
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
        << QStringList{"--attack-mode", "0", "--rules-file", "r.rule", "hash.txt", "dict.txt"};
    QTest::newRow("combination") << int(AttackMode::Combination)
        << QStringList{"--attack-mode", "1", "hash.txt", "dict.txt"};
    QTest::newRow("brute force") << int(AttackMode::BruteForce)
        << QStringList{"--attack-mode", "3", "--custom-charset1", "abc", "hash.txt", "?d?d"};
    QTest::newRow("hybrid wordlist + mask") << int(AttackMode::HybridWordMask)
        << QStringList{"--attack-mode", "6", "--custom-charset1", "abc", "hash.txt", "dict.txt", "?d?d"};
    QTest::newRow("hybrid mask + wordlist") << int(AttackMode::HybridMaskWord)
        << QStringList{"--attack-mode", "7", "--custom-charset1", "abc", "hash.txt", "?d?d", "dict.txt"};
    QTest::newRow("association") << int(AttackMode::Association)
        << QStringList{"--attack-mode", "9", "--rules-file", "r.rule", "hash.txt", "dict.txt"};
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
    options.backendDevices = QStringLiteral("0,1");
    options.segmentSize = 64;

    const QStringList full{"--attack-mode", "0",
                           "--outfile-format", "1",
                           "--backend-devices", "0,1",
                           "--segment-size", "64"};
    QCOMPARE(CommandBuilder::build(options), full);
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
    const QStringList expected{"-m", "1000", "-a", "0",
                               "--remove", "--username",
                               "-w", "2",
                               "--cpu-affinity", "1-4"};
    QCOMPARE(CommandBuilder::build(options), expected);

    for (const QString &argument : CommandBuilder::build(options)) {
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
    const QString stamped = CommandBuilder::expandOutfileTemplate(
        QStringLiteral("<unixtime>"), QStringLiteral("/tmp/whatever"));
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
}

int main(int argc, char *argv[])
{
    TestCommandBuilder test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_commandbuilder.moc"
