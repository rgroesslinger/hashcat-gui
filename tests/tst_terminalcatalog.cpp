/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appconstants.h"
#include "argumentquoting.h"
#include "terminalcatalog.h"
#include "testenvironment.h"

#include <QCoreApplication>
#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QProcess>
#include <QStandardPaths>
#include <QStringDecoder>
#include <QTemporaryDir>
#include <array>
#include <iterator>

namespace
{
// The script the -EncodedCommand argument carries, back as text.
QString decodeScript(const QString &encoded)
{
    QStringDecoder decoder(QStringDecoder::Utf16LE);
    const QByteArray bytes = QByteArray::fromBase64(encoded.toLatin1());
    return decoder.decode(bytes);
}

// The shell and the helper for the launches that need a real PowerShell;
// empty when absent, so the caller can skip. Windows PowerShell 5.1 is the
// stricter tokenizer, so it is preferred where both shells exist.
QString findPowerShell()
{
    for (const QString &name : {QStringLiteral("powershell.exe"), QStringLiteral("pwsh")}) {
        if (!QStandardPaths::findExecutable(name).isEmpty()) {
            return name;
        }
    }
    return {};
}

QString findArgEcho()
{
#ifdef Q_OS_WIN
    const QString helper =
        QDir(QCoreApplication::applicationDirPath()).filePath(QStringLiteral("argecho.exe"));
#else
    const QString helper =
        QDir(QCoreApplication::applicationDirPath()).filePath(QStringLiteral("argecho"));
#endif
    return QFile::exists(helper) ? helper : QString();
}

// A failed check must not leave the shell behind: -NoExit holds it forever.
struct ShellGuard {
    QProcess *proc;
    explicit ShellGuard(QProcess *p) : proc(p) {}
    ~ShellGuard()
    {
        if (proc->state() != QProcess::NotRunning) {
            proc->kill();
            proc->waitForFinished(5000);
        }
    }
    ShellGuard(const ShellGuard &) = delete;
    ShellGuard &operator=(const ShellGuard &) = delete;
    ShellGuard(ShellGuard &&) = delete;
    ShellGuard &operator=(ShellGuard &&) = delete;
};
} // namespace

// The terminal table, the launch command line and the detached start.
class TestTerminalCatalog : public QObject
{
    Q_OBJECT

private slots:
    void terminalArgumentsArePinned();
    void launchCommandExecsUnixTerminalsDirectly();
    void launchCommandEncodesTheScript_data();
    void launchCommandEncodesTheScript();
    void powerShellScriptIsTheOneThatIsEncoded();
    void launchCommandQuotesEveryToken_data();
    void launchCommandQuotesEveryToken();
    void launchCommandCarriesSmartQuotesAsBase64_data();
    void launchCommandCarriesSmartQuotesAsBase64();
    void launchCommandRefusesAnOverlongCommandLine();
    void launchCommandRefusesAnUnknownTerminal();
    void launchCommandRoundTripsThroughPowerShell_data();
    void launchCommandRoundTripsThroughPowerShell();
    void launchCommandKeepOpenHoldsTheSession_data();
    void launchCommandKeepOpenHoldsTheSession();
    void findHashcatInPathFindsTheExecutable();
    void findHashcatInPathProbesBothUnixNames_data();
    void findHashcatInPathProbesBothUnixNames();
    void findHashcatInPathIsEmptyWithoutOne();
    void availableTerminalsReturnsResolvedPrograms();
    void launchDetachedStartsTheProgram();
    void launchDetachedReportsAFailure();
};

// The mapping startClicked() builds its command line from: pin every
// entry, because a wrong one is a terminal that starts and then never runs hashcat.
void TestTerminalCatalog::terminalArgumentsArePinned()
{
    struct Expected {
        const char *terminal = "";
        QStringList arguments;
    };
    const auto expected = std::to_array<Expected>({
        // PowerShell and pwsh are discovered here but launched through
        // launchCommand(); the table holds no arguments for them.
        {"powershell.exe", {}},
        {"pwsh", {}},
        {"xterm", {"-hold", "-e"}},
        {"gnome-terminal", {"--wait", "--"}},
        {"konsole", {"--hold", "-e"}},
        {"xfce4-terminal", {"--hold", "-x"}},
        {"alacritty", {"--hold", "-e"}},
        {"kitty", {"--hold"}},
        {"foot", {"--hold"}},
        {"wezterm", {"start", "--"}},
        {"ghostty", {"--wait-after-command=true", "-e"}},
    });

    const QMap<QString, QStringList> &table = TerminalCatalog::terminalArguments();
    QCOMPARE(int(table.size()), int(std::size(expected)));
    for (const Expected &entry : expected) {
        QCOMPARE(table.value(QString::fromLatin1(entry.terminal)), entry.arguments);
    }
}

// Every terminal except PowerShell execs hashcat directly: the table's own
// arguments, then the binary, then the collected argv - unchanged by keepOpen.
void TestTerminalCatalog::launchCommandExecsUnixTerminalsDirectly()
{
    const QStringList arguments{QStringLiteral("--attack-mode"), QStringLiteral("0")};
    const QMap<QString, QStringList> &table = TerminalCatalog::terminalArguments();
    for (auto it = table.constBegin(); it != table.constEnd(); ++it) {
        if (it.key() == QStringLiteral("powershell.exe") || it.key() == QStringLiteral("pwsh")) {
            continue;
        }
        const LaunchCommand command = TerminalCatalog::launchCommand(
            it.key(), it.key(), QStringLiteral("hashcat"), arguments, true);
        QCOMPARE(command.program, it.key());
        QStringList expected = it.value();
        expected << QStringLiteral("hashcat") << arguments;
        QCOMPARE(command.arguments, expected);
    }
}

// The PowerShell launch carries its script as UTF-16LE base64; the flags are
// fixed, and -NoExit appears only while the terminal should stay open. The
// script starts hashcat through ProcessStartInfo with a verbatim Arguments
// string - PowerShell's own native argument passing never sees the tokens.
void TestTerminalCatalog::launchCommandEncodesTheScript_data()
{
    QTest::addColumn<QString>("terminal");
    QTest::addColumn<bool>("keepOpen");

    QTest::newRow("powershell.exe, keep open") << QStringLiteral("powershell.exe") << true;
    QTest::newRow("powershell.exe, close") << QStringLiteral("powershell.exe") << false;
    QTest::newRow("pwsh, keep open") << QStringLiteral("pwsh") << true;
    QTest::newRow("pwsh, close") << QStringLiteral("pwsh") << false;
}

void TestTerminalCatalog::launchCommandEncodesTheScript()
{
    QFETCH(QString, terminal);
    QFETCH(bool, keepOpen);

    const QString hashcatPath = QStringLiteral("C:\\Program Files\\hashcat\\hashcat.exe");
    const QStringList arguments{QStringLiteral("--attack-mode"), QStringLiteral("0"),
                                QStringLiteral("--mask"), QStringLiteral("?d?d?d?d")};
    const LaunchCommand command =
        TerminalCatalog::launchCommand(terminal, terminal, hashcatPath, arguments, keepOpen);

    QCOMPARE(command.program, terminal);
    QStringList expectedFlags{QStringLiteral("-NoProfile")};
    if (keepOpen) {
        expectedFlags << QStringLiteral("-NoExit");
    }
    expectedFlags << QStringLiteral("-EncodedCommand");
    QCOMPARE(command.arguments.mid(0, expectedFlags.size()), expectedFlags);
    QCOMPARE(command.arguments.size(), expectedFlags.size() + 1);

    const QString expectedScript =
        QStringLiteral("$si = New-Object System.Diagnostics.ProcessStartInfo\n"
                       "$si.FileName = 'C:\\Program Files\\hashcat\\hashcat.exe'\n"
                       "$si.Arguments = '--attack-mode 0 --mask ?d?d?d?d'\n"
                       "$si.UseShellExecute = $false\n"
                       "$si.WorkingDirectory = '")
        + QFileInfo(hashcatPath).absolutePath()
        + QStringLiteral("'\n"
                         "$p = [System.Diagnostics.Process]::Start($si)\n"
                         "$p.WaitForExit()");
    QCOMPARE(decodeScript(command.arguments.last()), expectedScript);
}

// The Tools action copies the readable script; the launch encodes it. Both
// come from the same builder, so one cannot drift from the other.
void TestTerminalCatalog::powerShellScriptIsTheOneThatIsEncoded()
{
    const QString hashcatPath = QStringLiteral("C:\\Program Files\\hashcat\\hashcat.exe");
    const QStringList arguments{QStringLiteral("--attack-mode"), QStringLiteral("0"),
                                QStringLiteral("$2a$10$abc"), QStringLiteral("?d;calc")};

    const QString script = TerminalCatalog::powerShellScript(hashcatPath, arguments);
    const LaunchCommand command = TerminalCatalog::launchCommand(
        QStringLiteral("pwsh"), QStringLiteral("pwsh"), hashcatPath, arguments, false);

    QCOMPARE(decodeScript(command.arguments.last()), script);
}

// The tokens a hashcat command line really carries: bcrypt hashes, masks with
// shell metacharacters, spaces, quotes and backticks. Each has to reach the
// Arguments string as a literal the MSVCRT parser reads back unchanged -
// never as code, and never rewritten by PowerShell's native binder.
void TestTerminalCatalog::launchCommandQuotesEveryToken_data()
{
    QTest::addColumn<QString>("token");
    QTest::addColumn<QString>("quoted");

    QTest::newRow("bcrypt hash") << QStringLiteral("$2a$10$abcdefghijklmnopqrstuu")
                                 << QStringLiteral("$2a$10$abcdefghijklmnopqrstuu");
    QTest::newRow("mask with subexpression")
        << QStringLiteral("?d?d$(Get-Date)") << QStringLiteral("?d?d$(Get-Date)");
    QTest::newRow("mask with semicolon") << QStringLiteral("?d;calc") << QStringLiteral("?d;calc");
    QTest::newRow("spaces") << QStringLiteral("two words") << QStringLiteral("\"two words\"");
    QTest::newRow("single quotes")
        << QStringLiteral("it's a 'test'") << QStringLiteral("\"it's a 'test'\"");
    QTest::newRow("double quotes")
        << QStringLiteral("say \"hi\"") << QStringLiteral("\"say \\\"hi\\\"\"");
    QTest::newRow("quote without spaces") << QStringLiteral("a\"b") << QStringLiteral("a\\\"b");
    QTest::newRow("backticks") << QStringLiteral("`backtick`") << QStringLiteral("`backtick`");
    QTest::newRow("pipe and ampersand")
        << QStringLiteral("pipe|and&semi") << QStringLiteral("pipe|and&semi");
    QTest::newRow("empty") << QString() << QStringLiteral("\"\"");
    QTest::newRow("newline") << QStringLiteral("line\nbreak") << QStringLiteral("\"line\nbreak\"");
    QTest::newRow("smart double quotes") << QStringLiteral("“”„") << QStringLiteral("“”„");
    QTest::newRow("trailing backslash and spaces")
        << QStringLiteral("C:\\path with space\\") << QStringLiteral("\"C:\\path with space\\\\\"");
}

void TestTerminalCatalog::launchCommandQuotesEveryToken()
{
    QFETCH(QString, token);
    QFETCH(QString, quoted);

    const LaunchCommand command = TerminalCatalog::launchCommand(
        QStringLiteral("pwsh"), QStringLiteral("pwsh"), QStringLiteral("hashcat"), {token}, false);
    // None of these tokens holds a smart single quote, so the Arguments
    // literal is single-quoted with its own quotes doubled.
    QString literal = quoted;
    literal.replace(QLatin1Char('\''), QStringLiteral("''"));
    const QString script = decodeScript(command.arguments.last());
    QVERIFY2(script.contains(QStringLiteral("$si.Arguments = '") + literal + QStringLiteral("'\n")),
             qPrintable(script));
}

// PowerShell reads U+2018-U+201B as single-quote delimiters, so a token that
// holds one cannot be quoted literally; the whole Arguments string travels
// as its own base64 value, decoded inside the script.
void TestTerminalCatalog::launchCommandCarriesSmartQuotesAsBase64_data()
{
    QTest::addColumn<QString>("token");

    QTest::newRow("left single") << QStringLiteral("?d‘?d");
    QTest::newRow("right single") << QStringLiteral("?d’?d");
    QTest::newRow("low single") << QStringLiteral("?d‚?d");
    QTest::newRow("high reversed single") << QStringLiteral("?d‛?d");
    QTest::newRow("mixed with an ASCII quote") << QStringLiteral("it's ’?d’");
}

void TestTerminalCatalog::launchCommandCarriesSmartQuotesAsBase64()
{
    QFETCH(QString, token);

    const LaunchCommand command = TerminalCatalog::launchCommand(
        QStringLiteral("powershell.exe"), QStringLiteral("powershell.exe"),
        QStringLiteral("hashcat"), {token}, false);
    // The base64 carries the token as the MSVCRT quoting left it (pinned in
    // tst_argumentquoting); this case is about the transport.
    const QString expected =
        QStringLiteral("$si.Arguments = "
                       "([Text.Encoding]::UTF8.GetString([Convert]::FromBase64String('")
        + QString::fromLatin1(msvcrtQuoteArgument(token).toUtf8().toBase64())
        + QStringLiteral("')))\n");
    QVERIFY2(decodeScript(command.arguments.last()).contains(expected),
             qPrintable(decodeScript(command.arguments.last())));
}

// A command line beyond the Windows limit is refused, never truncated.
void TestTerminalCatalog::launchCommandRefusesAnOverlongCommandLine()
{
    const QStringList arguments{QString(40000, QLatin1Char('a'))};
    const LaunchCommand command = TerminalCatalog::launchCommand(
        QStringLiteral("pwsh"), QStringLiteral("pwsh"), QStringLiteral("hashcat"), arguments, true);
    QVERIFY(command.program.isEmpty());
    QVERIFY(command.arguments.isEmpty());

    // The same launch well under the limit goes through.
    const LaunchCommand small =
        TerminalCatalog::launchCommand(QStringLiteral("pwsh"), QStringLiteral("pwsh"),
                                       QStringLiteral("hashcat"), {QStringLiteral("?d?d")}, true);
    QVERIFY(!small.program.isEmpty());
}

void TestTerminalCatalog::launchCommandRefusesAnUnknownTerminal()
{
    const LaunchCommand command = TerminalCatalog::launchCommand(
        QStringLiteral("no-such-terminal"), QStringLiteral("no-such-terminal"),
        QStringLiteral("hashcat"), {QStringLiteral("--help")}, true);
    QVERIFY(command.program.isEmpty());
    QVERIFY(command.arguments.isEmpty());
}

// The encoding is only proven once a real PowerShell has parsed it: start the
// argecho helper through the shell and compare its argv with the tokens that
// went in. Windows PowerShell 5.1 is the stricter tokenizer, so it is preferred
// where both shells exist; the Unix runners carry pwsh.
void TestTerminalCatalog::launchCommandRoundTripsThroughPowerShell_data()
{
    QTest::addColumn<QStringList>("tokens");

    QTest::newRow("hashcat argv") << QStringList{QStringLiteral("--attack-mode"),
                                                 QStringLiteral("0"),
                                                 QStringLiteral("-m"),
                                                 QStringLiteral("22000"),
                                                 QStringLiteral("$2a$10$abcdefghijklmnopqrstuu"),
                                                 QStringLiteral("?d?d?d?d")};
    QTest::newRow("shell metacharacters") << QStringList{QStringLiteral("?d?d$(Get-Date)"),
                                                         QStringLiteral("?d;calc"),
                                                         QStringLiteral("a&b"),
                                                         QStringLiteral("a|b"),
                                                         QStringLiteral("`tick`"),
                                                         QStringLiteral("$env:PATH")};
    QTest::newRow("spaces and quotes")
        << QStringList{QStringLiteral("two words"), QStringLiteral("it's"),
                       QStringLiteral("say \"hi\""), QString()};
    QTest::newRow("smart quotes") << QStringList{
        QStringLiteral("?d‘?d"), QStringLiteral("?d’?d"),    QStringLiteral("?d‚?d"),
        QStringLiteral("?d‛?d"), QStringLiteral("“double”"), QStringLiteral("it's ’mixed’")};
    QTest::newRow("line breaks") << QStringList{QStringLiteral("line\nbreak"),
                                                QStringLiteral("tab\there")};
    // The cases PowerShell's own native argument passing used to break on
    // Windows (5.1 and pwsh before 7.3): embedded quotes with and without
    // spaces, an empty token, and a trailing backslash behind spaces.
    QTest::newRow("native passing hazards") << QStringList{QStringLiteral("say \"hi\""),
                                                           QStringLiteral("a\"b"),
                                                           QString(),
                                                           QStringLiteral("C:\\path with space\\"),
                                                           QStringLiteral("trail\\"),
                                                           QStringLiteral("a'b\"c\\d e"),
                                                           QStringLiteral("$2a$10$abc")};
}

void TestTerminalCatalog::launchCommandRoundTripsThroughPowerShell()
{
    const QString shell = findPowerShell();
    if (shell.isEmpty()) {
        QSKIP("no PowerShell on PATH");
    }
    const QString helper = findArgEcho();
    if (helper.isEmpty()) {
        QSKIP("the argecho helper is not built");
    }

    QFETCH(QStringList, tokens);
    const LaunchCommand command =
        TerminalCatalog::launchCommand(shell, shell, helper, tokens, false);
    QVERIFY(!command.program.isEmpty());

    QProcess proc;
    proc.setProgram(command.program);
    proc.setArguments(command.arguments);
    proc.start();
    QVERIFY2(proc.waitForStarted(10000), qPrintable(proc.errorString()));
    QVERIFY2(proc.waitForFinished(30000), "the shell did not finish in time");
    QCOMPARE(proc.exitCode(), 0);

    const QJsonDocument document = QJsonDocument::fromJson(proc.readAllStandardOutput());
    QVERIFY2(document.isArray(), qPrintable(QString::fromUtf8(proc.readAllStandardError())));
    QStringList received;
    const QJsonArray array = document.array();
    for (const auto &value : array) {
        received << value.toString();
    }
    QCOMPARE(received, tokens);
}

// keepOpen has to hold the session after hashcat exits: -NoExit only keeps
// it open while the script itself does not call exit. The helper exits at
// once, so what the shell does afterwards is the whole keepOpen contract.
void TestTerminalCatalog::launchCommandKeepOpenHoldsTheSession_data()
{
    QTest::addColumn<bool>("keepOpen");
    QTest::newRow("keep open") << true;
    QTest::newRow("close") << false;
}

void TestTerminalCatalog::launchCommandKeepOpenHoldsTheSession()
{
    const QString shell = findPowerShell();
    if (shell.isEmpty()) {
        QSKIP("no PowerShell on PATH");
    }
    const QString helper = findArgEcho();
    if (helper.isEmpty()) {
        QSKIP("the argecho helper is not built");
    }

    QFETCH(bool, keepOpen);
    const LaunchCommand command = TerminalCatalog::launchCommand(
        shell, shell, helper, {QStringLiteral("--attack-mode"), QStringLiteral("0")}, keepOpen);
    QVERIFY(!command.program.isEmpty());

    QProcess proc;
    proc.setProgram(command.program);
    proc.setArguments(command.arguments);
    proc.start();
    QVERIFY2(proc.waitForStarted(10000), qPrintable(proc.errorString()));
    const ShellGuard guard{&proc};

    // The helper's JSON arrives when it exits - the moment the script's
    // WaitForExit returns and the keepOpen decision takes effect.
    QByteArray received;
    QElapsedTimer timer;
    timer.start();
    while (!QJsonDocument::fromJson(received).isArray()) {
        QVERIFY2(timer.elapsed() < 30000, "the helper did not run in time");
        proc.waitForReadyRead(100);
        received += proc.readAllStandardOutput();
    }

    if (keepOpen) {
        // Leave the script time to end - and an exit statement in it time to
        // take the session down with it.
        QTest::qWait(1500);
        QVERIFY2(proc.state() == QProcess::Running,
                 "the session ended although -NoExit was asked to hold it");
    } else {
        QVERIFY2(proc.waitForFinished(30000), "the shell did not exit on its own");
    }
}

// The lookup follows PATH and only reports a file this system can execute.
void TestTerminalCatalog::findHashcatInPathFindsTheExecutable()
{
    const QString stub = TestEnvironment::writeSearchableExecutable(
        QString(AppConstants::Hashcat::ExecutableNames.at(0)));
    QVERIFY2(!stub.isEmpty(), "the executable stub could not be written");

    TestEnvironment::PathGuard guard(QFileInfo(stub).absolutePath().toUtf8());

    const QString found = TerminalCatalog::findHashcatInPath();
    QVERIFY2(!found.isEmpty(), "the executable on PATH was not found");
    QCOMPARE(QFileInfo(found).canonicalFilePath(), QFileInfo(stub).canonicalFilePath());
}

// Unix has two names for the same binary: `make install` and the distribution
// packages install "hashcat", the release archive ships "hashcat.bin". Both
// have to be found, and the installed name wins when both are there. Every row
// gets its own directory, so one row cannot see a file another wrote.
void TestTerminalCatalog::findHashcatInPathProbesBothUnixNames_data()
{
    QTest::addColumn<QStringList>("names");
    QTest::addColumn<QString>("expected");

    QTest::newRow("installed name only")
        << QStringList{QStringLiteral("hashcat")} << QStringLiteral("hashcat");
    QTest::newRow("archive name only")
        << QStringList{QStringLiteral("hashcat.bin")} << QStringLiteral("hashcat.bin");
    QTest::newRow("both") << QStringList{QStringLiteral("hashcat.bin"), QStringLiteral("hashcat")}
                          << QStringLiteral("hashcat");
}

void TestTerminalCatalog::findHashcatInPathProbesBothUnixNames()
{
#ifdef Q_OS_WIN
    QSKIP("the Windows build has one executable name");
#else
    QFETCH(QStringList, names);
    QFETCH(QString, expected);

    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    for (const QString &name : names) {
        const QString stub = TestEnvironment::writeSearchableExecutable(name, directory.path());
        QVERIFY2(!stub.isEmpty(),
                 qPrintable(QStringLiteral("the %1 stub could not be written").arg(name)));
    }

    TestEnvironment::PathGuard guard(directory.path().toUtf8());

    const QString found = TerminalCatalog::findHashcatInPath();
    QVERIFY2(!found.isEmpty(), "the executable on PATH was not found");
    QCOMPARE(QFileInfo(found).fileName(), expected);
#endif
}

void TestTerminalCatalog::findHashcatInPathIsEmptyWithoutOne()
{
    QTemporaryDir empty;
    QVERIFY(empty.isValid());

    TestEnvironment::PathGuard guard(empty.path().toUtf8());

    QCOMPARE(TerminalCatalog::findHashcatInPath(), QString());
}

// Discovery answers name -> absolute program, so the launch never hands a
// bare name to a process and a terminal behind PATH is started from the file
// this lookup actually found.
void TestTerminalCatalog::availableTerminalsReturnsResolvedPrograms()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
#ifdef Q_OS_WIN
    const QString fileName = QStringLiteral("xterm.exe");
#else
    const QString fileName = QStringLiteral("xterm");
#endif
    const QString stub = TestEnvironment::writeSearchableExecutable(fileName, directory.path());
    QVERIFY2(!stub.isEmpty(), "the terminal stub could not be written");

    TestEnvironment::PathGuard guard(directory.path().toUtf8());

    const QMap<QString, QString> terminals = TerminalCatalog::availableTerminals();
    QVERIFY2(terminals.contains(QStringLiteral("xterm")), "the terminal on PATH was not found");
    const QString resolved = terminals.value(QStringLiteral("xterm"));
    QVERIFY2(QDir::isAbsolutePath(resolved), "the terminal was not resolved to an absolute path");
    QCOMPARE(QFileInfo(resolved).canonicalFilePath(), QFileInfo(stub).canonicalFilePath());
}

// The detached start runs the program in the hashcat binary's own directory
// and hands it the arguments unchanged. The stub records both.
void TestTerminalCatalog::launchDetachedStartsTheProgram()
{
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString record = directory.filePath(QStringLiteral("record.txt"));
    const QString stub = TestEnvironment::writeShellStub(
        QStringLiteral("stub-terminal"),
        QStringLiteral("#!/bin/sh\npwd > \"%1\"\nprintf '%s\\n' \"$@\" >> \"%1\"\n")
            .arg(record)
            .toUtf8());
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");

    // The hashcat path does not have to exist: only its directory is used.
    const QString hashcatPath = directory.filePath(QStringLiteral("hashcat"));
    LaunchCommand command;
    command.program = stub;
    command.arguments << QStringLiteral("hashcat") << QStringLiteral("--attack-mode")
                      << QStringLiteral("0");
    QString error;
    QVERIFY2(TerminalCatalog::launchDetached(command, hashcatPath, &error), qPrintable(error));

    QTRY_VERIFY_WITH_TIMEOUT(QFile::exists(record), 5000);
    QFile file(record);
    QVERIFY(file.open(QIODevice::ReadOnly));
    const QStringList recorded =
        QString::fromUtf8(file.readAll()).split(QLatin1Char('\n'), Qt::SkipEmptyParts);
    QCOMPARE(recorded.size(), 4);
    QCOMPARE(QDir(recorded.at(0)).canonicalPath(), QDir(directory.path()).canonicalPath());
    QCOMPARE(recorded.mid(1), command.arguments);
#endif
}

void TestTerminalCatalog::launchDetachedReportsAFailure()
{
    LaunchCommand command;
    command.program = QStringLiteral("/nonexistent/hashcat-gui-terminal");
    QString error;
    QVERIFY(!TerminalCatalog::launchDetached(command, QStringLiteral("hashcat"), &error));
    QVERIFY(!error.isEmpty());
}

int main(int argc, char *argv[])
{
    TestTerminalCatalog test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_terminalcatalog.moc"
