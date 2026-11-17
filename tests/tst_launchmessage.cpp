/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "launchmessage.h"
#include "launchvalidator.h"
#include "terminalcatalog.h"
#include "testenvironment.h"

#include <QMessageBox>

// The mapping from a launch problem to its message box data: every code,
// the icon and text format it needs, and the value interpolation.
class TestLaunchMessage : public QObject
{
    Q_OBJECT

private slots:
    void messageFor_data();
    void messageFor();
    void refusalMessage_data();
    void refusalMessage();
    void noProblemHasNoMessage();
};

void TestLaunchMessage::messageFor_data()
{
    using Error = LaunchValidator::LaunchError;

    QTest::addColumn<int>("code");
    QTest::addColumn<QString>("detail");
    QTest::addColumn<int>("mode");
    QTest::addColumn<int>("icon");
    QTest::addColumn<int>("format");
    QTest::addColumn<QString>("text");

    const int info = int(QMessageBox::Information);
    const int warning = int(QMessageBox::Warning);
    const int autoText = int(Qt::AutoText);
    const int plain = int(Qt::PlainText);
    const int rich = int(Qt::RichText);
    const int straight = int(AttackMode::Straight);

    QTest::newRow("NoHashFile") << int(Error::NoHashFile) << QString() << straight << info
                                << autoText << QStringLiteral("Please choose a hash file.");

    QTest::newRow("NoHashcatPath")
        << int(Error::NoHashcatPath) << QString() << straight << info << rich
        << QStringLiteral("Navigate to <b>File → Settings</b> to configure the path to the "
                          "hashcat executable.");

    QString missing =
        QStringLiteral("The configured hashcat executable does not exist:\n/opt/hashcat\n"
                       "Navigate to File → Settings to change it.");
    QString notExecutable =
        QStringLiteral("The configured hashcat path is not an executable file:\n/opt/hashcat\n"
                       "Navigate to File → Settings to change it.");
#ifdef Q_OS_WIN
    const QString suffixNote =
        QStringLiteral("\nWindows executable names include their extension, for example "
                       "hashcat.exe.");
    missing += suffixNote;
    notExecutable += suffixNote;
#endif
    QTest::newRow("HashcatPathMissing")
        << int(Error::HashcatPathMissing) << QStringLiteral("/opt/hashcat") << straight << warning
        << plain << missing;

    QTest::newRow("HashcatNotExecutable")
        << int(Error::HashcatNotExecutable) << QStringLiteral("/opt/hashcat") << straight << warning
        << plain << notExecutable;

    QTest::newRow("NoTerminal")
        << int(Error::NoTerminal) << QString() << straight << info << rich
        << QStringLiteral("Navigate to <b>File → Settings</b> to select the terminal used for "
                          "launching.");

    QTest::newRow("NoTerminals") << int(Error::NoTerminals) << QString() << straight << warning
                                 << autoText << TerminalCatalog::noSupportedTerminalMessage();

    QTest::newRow("UnknownTerminal")
        << int(Error::UnknownTerminal) << QStringLiteral("konsole, kitty") << straight << warning
        << plain
        << QStringLiteral("The configured terminal “yakuake” is not available. Available "
                          "terminals: konsole, kitty\nNavigate to File → Settings to change it.");

    QTest::newRow("NoWordlist") << int(Error::NoWordlist) << QString() << straight << info
                                << autoText
                                << QStringLiteral("This mode needs at least one wordlist.");

    QTest::newRow("WrongWordlistCount")
        << int(Error::WrongWordlistCount) << QStringLiteral("1") << int(AttackMode::Combination)
        << info << autoText
        << QStringLiteral("This mode needs exactly 2 wordlists (left, right).\n"
                          "Currently selected: 1.");

    QTest::newRow("TooManyWordlists")
        << int(Error::TooManyWordlists) << QStringLiteral("3") << int(AttackMode::Hybrid) << info
        << autoText
        << QStringLiteral("Maximum number of wordlists for this mode: 2.\nCurrently selected: 3.");

    QTest::newRow("TooManyWordlists in association")
        << int(Error::TooManyWordlists) << QStringLiteral("2") << int(AttackMode::Association)
        << info << autoText
        << QStringLiteral("Maximum number of wordlists for this mode: 1.\nCurrently selected: 2.\n"
                          "hashcat pairs the two files by line here; this GUI offers only one "
                          "wordlist for that role.");

    QTest::newRow("WordlistMustBeFile")
        << int(Error::WordlistMustBeFile) << QStringLiteral("/tmp/folder") << straight << warning
        << plain
        << QStringLiteral("This wordlist has to be a file: hashcat reads it as the multiplied "
                          "wordlist (the ?q side), and a folder cannot be used there:\n"
                          "/tmp/folder");

    QTest::newRow("NoMask") << int(Error::NoMask) << QString() << straight << info << autoText
                            << QStringLiteral("This mode needs a mask.");

    QTest::newRow("NoTable") << int(Error::NoTable) << QString() << straight << info << autoText
                             << QStringLiteral("This mode needs at least one table.");

    QTest::newRow("NoFeed")
        << int(Error::NoFeed) << QString() << straight << info << autoText
        << QStringLiteral("This mode needs a feed: a bundled name (wordlist, stdin, random, …) "
                          "or the path of a feed file.");

    QTest::newRow("MaskNeedsWordMarker")
        << int(Error::MaskNeedsWordMarker) << QString() << straight << info << autoText
        << QStringLiteral("This mode pairs the wordlists with the mask; the mask needs a ?w "
                          "for the word.");

    QTest::newRow("MaskMultipleWordMarkers")
        << int(Error::MaskMultipleWordMarkers) << QString() << straight << info << autoText
        << QStringLiteral("The mask contains more than one ?w.");

    QTest::newRow("MaskQBeforeWord")
        << int(Error::MaskQBeforeWord) << QString() << straight << info << autoText
        << QStringLiteral("The mask contains a ?q before the first ?w.");

    QTest::newRow("MaskMultipleQMarks")
        << int(Error::MaskMultipleQMarks) << QString() << straight << info << autoText
        << QStringLiteral("The mask contains more than one ?q.");

    QTest::newRow("MaskQWithoutWordlist")
        << int(Error::MaskQWithoutWordlist) << QString() << straight << info << autoText
        << QStringLiteral("The mask names a second word with ?q, but only one wordlist "
                          "is selected.");

    QTest::newRow("SecondWordlistWithoutQ")
        << int(Error::SecondWordlistWithoutQ) << QString() << straight << info << autoText
        << QStringLiteral("Two wordlists are selected, but the mask has no ?q for the "
                          "second one.");

    QTest::newRow("IncrementInMode12")
        << int(Error::IncrementInMode12) << QString() << straight << info << autoText
        << QStringLiteral("The increment is not offered in this hybrid mode.");

    QTest::newRow("MaskMarkersNotAllowed")
        << int(Error::MaskMarkersNotAllowed) << QString() << straight << info << autoText
        << QStringLiteral("The ?w and ?q markers are only offered in the hybrid mode "
                          "(mask + wordlists).");

    QTest::newRow("PcfgCharsetWithoutMask")
        << int(Error::PcfgCharsetWithoutMask) << QString() << straight << info << autoText
        << QStringLiteral("A custom charset is used only inside a mask, and this run named no "
                          "mask. Add a mask= setting that references ?1–?4, for example "
                          "mask=?1?1?1?1, or clear the custom charset.");

    QTest::newRow("InvalidPcfgSetting")
        << int(Error::InvalidPcfgSetting) << QStringLiteral("scale=99") << straight << info << plain
        << QStringLiteral("The setting “scale=99” is not valid for this mode.");

    QTest::newRow("InvalidTableSetting")
        << int(Error::InvalidTableSetting) << QStringLiteral("cap=x") << straight << info << plain
        << QStringLiteral("The setting “cap=x” is not valid for this mode.");

    QTest::newRow("InvalidAssociationSetting")
        << int(Error::InvalidAssociationSetting) << QStringLiteral("phases") << straight << info
        << plain
        << QStringLiteral("The token “phases” is not a setting; settings are written as "
                          "key=value.");

    QTest::newRow("AssociationSettingsWithWordlist")
        << int(Error::AssociationSettingsWithWordlist) << QString() << straight << info << autoText
        << QStringLiteral("The settings line applies only while the words come from the hash "
                          "file. Clear either the settings line or the wordlist list.");

    QTest::newRow("NoHashRecipe")
        << int(Error::NoHashRecipe) << QString() << straight << info << autoText
        << QStringLiteral("The hash type 4000 describes its hashes as a recipe, for example\n"
                          "md5(md5(salt) . pass). Enter it in the hash recipe field.");

    QTest::newRow("InvalidIncrement")
        << int(Error::InvalidIncrement) << QString() << straight << info << autoText
        << QStringLiteral("The minimum increment length is larger than the maximum.");

    QTest::newRow("InvalidOutfileFormat")
        << int(Error::InvalidOutfileFormat) << QStringLiteral("1,7") << straight << warning << plain
        << QStringLiteral("The outfile format must be one or more comma-separated "
                          "numbers from 1 to 6:\n1,7");

    QTest::newRow("EmptyOutfileFormat")
        << int(Error::InvalidOutfileFormat) << QString() << straight << warning << plain
        << QStringLiteral("Enter an outfile format: one or more comma-separated numbers from "
                          "1 to 6.");

    QTest::newRow("InvalidCachePath")
        << int(Error::InvalidCachePath) << QStringLiteral("/tmp/gone") << straight << warning
        << plain << QStringLiteral("The cache path must be an existing directory:\n/tmp/gone");

    QTest::newRow("InvalidCharacter")
        << int(Error::InvalidCharacter) << QStringLiteral("hash\\x1b.txt") << straight << info
        << plain
        << QStringLiteral("This value contains a control character that cannot be passed to "
                          "hashcat:\nhash\\x1b.txt\nRemove the character and try again.");

    QTest::newRow("IncrementWithSpeedOnly")
        << int(Error::IncrementWithSpeedOnly) << QString() << straight << info << autoText
        << QStringLiteral("The increment cannot be combined with the speed-only option.");

    QTest::newRow("RemoveInAssociation")
        << int(Error::RemoveInAssociation) << QString() << straight << info << autoText
        << QStringLiteral("Removing cracked hashes is not possible in the association attack "
                          "mode.");
}

void TestLaunchMessage::messageFor()
{
    QFETCH(int, code);
    QFETCH(QString, detail);
    QFETCH(int, mode);
    QFETCH(int, icon);
    QFETCH(int, format);
    QFETCH(QString, text);

    LaunchValidator::LaunchContext context;
    context.configuredTerminal = QStringLiteral("yakuake");
    context.options.attackMode = static_cast<AttackMode>(mode);

    LaunchValidator::LaunchProblem problem;
    problem.code = static_cast<LaunchValidator::LaunchError>(code);
    problem.detail = detail;

    const LaunchMessage message =
        ::messageFor(problem, context, QStringLiteral("File"), QStringLiteral("Settings"));
    QCOMPARE(int(message.icon), icon);
    QCOMPARE(int(message.format), format);
    QCOMPARE(message.text, text);
}

// A refusal from the command builder gets its own message: an unknown
// terminal is a configuration problem, while a too-long command is the fixed
// CreateProcess limit of the encoded PowerShell script.
void TestLaunchMessage::refusalMessage_data()
{
    using Refusal = LaunchCommand::Refusal;

    QTest::addColumn<int>("refusal");
    QTest::addColumn<QString>("terminal");
    QTest::addColumn<int>("icon");
    QTest::addColumn<int>("format");
    QTest::addColumn<QString>("text");

    const int warning = int(QMessageBox::Warning);
    const int autoText = int(Qt::AutoText);
    const int plain = int(Qt::PlainText);

    QTest::newRow("built") << int(Refusal::None) << QStringLiteral("konsole") << warning << autoText
                           << QString();

    QTest::newRow("unknown terminal")
        << int(Refusal::UnknownTerminal) << QStringLiteral("yakuake") << warning << plain
        << QStringLiteral("The configured terminal “yakuake” is not available.");

    QTest::newRow("too long") << int(Refusal::CommandTooLong) << QStringLiteral("pwsh") << warning
                              << plain
                              << QStringLiteral(
                                     "The command line is too long for a PowerShell terminal. The "
                                     "encoded script must fit into 32,767 characters, which "
                                     "leaves room for about 12,000 ASCII characters of hashcat "
                                     "arguments. Shorten the argument lists or their values and "
                                     "try again.");
}

void TestLaunchMessage::refusalMessage()
{
    QFETCH(int, refusal);
    QFETCH(QString, terminal);
    QFETCH(int, icon);
    QFETCH(int, format);
    QFETCH(QString, text);

    LaunchCommand command;
    command.refusal = static_cast<LaunchCommand::Refusal>(refusal);
    const LaunchMessage message = ::messageFor(command, terminal);

    if (command.refusal == LaunchCommand::Refusal::None) {
        QVERIFY(message.text.isEmpty());
        return;
    }
    QCOMPARE(int(message.icon), icon);
    QCOMPARE(int(message.format), format);
    QCOMPARE(message.text, text);
}

void TestLaunchMessage::noProblemHasNoMessage()
{
    LaunchValidator::LaunchContext context;
    LaunchValidator::LaunchProblem problem;
    const LaunchMessage message =
        ::messageFor(problem, context, QStringLiteral("File"), QStringLiteral("Settings"));
    QVERIFY(message.text.isEmpty());
}

int main(int argc, char *argv[])
{
    TestLaunchMessage test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_launchmessage.moc"