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

#ifndef Q_OS_WIN
#include <csignal>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace
{
// The script the -EncodedCommand argument carries, back as text.
QString decodeScript(const QString &encoded)
{
    QStringDecoder decoder(QStringDecoder::Utf16LE);
    const QByteArray bytes = QByteArray::fromBase64(encoded.toLatin1());
    return decoder.decode(bytes);
}

// Every PowerShell on PATH: Windows PowerShell 5.1 and pwsh take different
// paths through native argument passing, so the round trip runs both where
// both exist.
QStringList findPowerShells()
{
    QStringList shells;
    for (const QString &name : {QStringLiteral("powershell.exe"), QStringLiteral("pwsh")}) {
        if (!QStandardPaths::findExecutable(name).isEmpty()) {
            shells << name;
        }
    }
    return shells;
}

// The shell the keep-open test uses: the stricter tokenizer first.
QString findPowerShell()
{
    const QStringList shells = findPowerShells();
    return shells.isEmpty() ? QString() : shells.first();
}

// A helper next to the test binary; empty when it was not built.
QString findArgumentEcho(const QString &name)
{
#ifdef Q_OS_WIN
    const QString helper =
        QDir(QCoreApplication::applicationDirPath()).filePath(name + QStringLiteral(".exe"));
#else
    const QString helper = QDir(QCoreApplication::applicationDirPath()).filePath(name);
#endif
    return QFile::exists(helper) ? helper : QString();
}

// CI can require the shell tests: a skip then means the runner changed under
// the job, not that the test does not apply. Callers return after this.
bool shellTestsRequired()
{
    const QByteArray value = qgetenv("HASHCAT_GUI_REQUIRE_SHELL_TESTS");
    return !value.isEmpty() && value != QByteArrayLiteral("0");
}

void skipOrFail(const char *why)
{
    if (shellTestsRequired()) {
        QFAIL(why);
    }
    QSKIP(why);
}

// The arguments in an argecho_crt dump: one "A <hex>" line each, an empty
// argument being "A " with no hex.
QList<QByteArray> parseArgumentDump(const QByteArray &output)
{
    QList<QByteArray> arguments;
    const QList<QByteArray> lines = output.split('\n');
    for (const QByteArray &line : lines) {
        if (line.isEmpty()) {
            continue;
        }
        if (!line.startsWith("A ")) {
            return {};
        }
        arguments << QByteArray::fromHex(line.mid(2));
    }
    return arguments;
}

// The bytes a process receives for a token: QProcess encodes on Unix with the
// local 8-bit codec, and the Windows C runtime converts the UTF-16 command
// line to its own code page for main(). toLocal8Bit() is the same conversion.
QList<QByteArray> localBytes(const QStringList &tokens)
{
    QList<QByteArray> bytes;
    bytes.reserve(tokens.size());
    for (const QString &token : tokens) {
        bytes << token.toLocal8Bit();
    }
    return bytes;
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
    void launchCommandWrapsUnixTerminals();
    void launchCommandEncodesTheScript_data();
    void launchCommandEncodesTheScript();
    void powerShellScriptIsTheOneThatIsEncoded();
    void launchCommandQuotesEveryToken_data();
    void launchCommandQuotesEveryToken();
    void launchCommandCarriesSmartQuotesAsBase64_data();
    void launchCommandCarriesSmartQuotesAsBase64();
    void launchCommandRefusesAnOverlongCommandLine();
    void launchCommandRefusesAnUnknownTerminal();
    void commandLengthLimitIsDocumented();
    void launchCommandRoundTripsThroughPowerShell_data();
    void launchCommandRoundTripsThroughPowerShell();
    void launchCommandKeepOpenHoldsTheSession_data();
    void launchCommandKeepOpenHoldsTheSession();
    void posixQuotingRoundTripsThroughTheShell_data();
    void posixQuotingRoundTripsThroughTheShell();
    void findHashcatInPathFindsTheExecutable();
    void findHashcatInPathProbesBothUnixNames_data();
    void findHashcatInPathProbesBothUnixNames();
    void findHashcatInPathIsEmptyWithoutOne();
    void availableTerminalsReturnsResolvedPrograms();
    void availableTerminalsOfferPowerShellOnlyOnWindows();
    void noSupportedTerminalMessageNamesPtyxis();
    void programDirectoryFollowsSymlinks();
    void launchDetachedStartsTheProgram();
    void unixWrapperRunsHashcatInItsDirectoryWithTheArguments();
    void unixWrapperSurvivesInterrupt();
    void unixWrapperReportsAFailedDirectory();
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
        {"xterm", {"-e"}},
        {"gnome-terminal", {"--"}},
        {"konsole", {"-e"}},
        {"xfce4-terminal", {"-x"}},
        {"alacritty", {"-e"}},
        {"kitty", {}},
        {"foot", {}},
        {"wezterm", {"start", "--"}},
        {"ghostty", {"-e"}},
    });

    const QMap<QString, QStringList> &table = TerminalCatalog::terminalArguments();
    QCOMPARE(int(table.size()), int(std::size(expected)));
    for (const Expected &entry : expected) {
        QCOMPARE(table.value(QString::fromLatin1(entry.terminal)), entry.arguments);
    }
}

// Every terminal except PowerShell runs the same sh wrapper: the terminal's
// own mode argument, then sh -c with the script, then the hashcat command as
// positional parameters. Pinning the exact script keeps the working
// directory, the exit code and the hold-open guarantee from drifting.
void TestTerminalCatalog::launchCommandWrapsUnixTerminals()
{
    const QString wrapper = QStringLiteral(R"(trap : INT
cd "$1" || {
    printf 'hashcat-gui: cannot enter %s\n' "$1" >&2
    printf '[press Enter to close]\n' >&2
    read -r _
    exit 1
}
shift
"$@"
rc=$?
printf '\n[hashcat exited with %s - press Enter to close]\n' "$rc"
read -r _)");

    const QStringList arguments{QStringLiteral("--attack-mode"), QStringLiteral("0")};
    const QMap<QString, QStringList> &table = TerminalCatalog::terminalArguments();
    for (auto it = table.constBegin(); it != table.constEnd(); ++it) {
        if (it.key() == QStringLiteral("powershell.exe") || it.key() == QStringLiteral("pwsh")) {
            continue;
        }
        const QString hashcatPath = QStringLiteral("/opt/hashcat");
        const LaunchCommand command =
            TerminalCatalog::launchCommand(it.key(), it.key(), hashcatPath, arguments, true);
        QCOMPARE(command.program, it.key());
        QCOMPARE(int(command.refusal), int(LaunchCommand::Refusal::None));

        QStringList expected = it.value();
        expected << QStringLiteral("sh") << QStringLiteral("-c") << wrapper
                 << QStringLiteral("hashcat-gui") << QFileInfo(hashcatPath).absolutePath()
                 << hashcatPath << arguments;
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

// A command line beyond the Windows limit is refused, never truncated, and
// the refusal says which cause it was.
void TestTerminalCatalog::launchCommandRefusesAnOverlongCommandLine()
{
    const QStringList arguments{QString(40000, QLatin1Char('a'))};
    const LaunchCommand command = TerminalCatalog::launchCommand(
        QStringLiteral("pwsh"), QStringLiteral("pwsh"), QStringLiteral("hashcat"), arguments, true);
    QVERIFY(command.program.isEmpty());
    QVERIFY(command.arguments.isEmpty());
    QCOMPARE(int(command.refusal), int(LaunchCommand::Refusal::CommandTooLong));

    // The same launch well under the limit goes through.
    const LaunchCommand small =
        TerminalCatalog::launchCommand(QStringLiteral("pwsh"), QStringLiteral("pwsh"),
                                       QStringLiteral("hashcat"), {QStringLiteral("?d?d")}, true);
    QVERIFY(!small.program.isEmpty());
    QCOMPARE(int(small.refusal), int(LaunchCommand::Refusal::None));
}

void TestTerminalCatalog::launchCommandRefusesAnUnknownTerminal()
{
    const LaunchCommand command = TerminalCatalog::launchCommand(
        QStringLiteral("no-such-terminal"), QStringLiteral("no-such-terminal"),
        QStringLiteral("hashcat"), {QStringLiteral("--help")}, true);
    QVERIFY(command.program.isEmpty());
    QVERIFY(command.arguments.isEmpty());
    QCOMPARE(int(command.refusal), int(LaunchCommand::Refusal::UnknownTerminal));
}

// The refusal message says "about 12,000 ASCII characters"; that number is
// the measured one-token limit next to an ordinary program name. If the
// script overhead or the CreateProcess margin changes, this has to move with
// the message.
void TestTerminalCatalog::commandLengthLimitIsDocumented()
{
    const QString hashcatPath = QStringLiteral("/opt/hashcat");
    const auto fits = [&hashcatPath](qsizetype length) {
        const LaunchCommand command = TerminalCatalog::launchCommand(
            QStringLiteral("pwsh"), QStringLiteral("pwsh"), hashcatPath,
            QStringList{QString(length, QLatin1Char('a'))}, false);
        return command.refusal == LaunchCommand::Refusal::None;
    };

    qsizetype low = 0;
    qsizetype high = 20000;
    while (low < high) {
        const qsizetype mid = low + (high - low + 1) / 2;
        if (fits(mid)) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    QCOMPARE(low, qsizetype(12038));
    QVERIFY(fits(low));
    QVERIFY(!fits(low + 1));
}

// The encoding is only proven once a real PowerShell has parsed it: start the
// argecho helpers through the shell and compare their argv with the tokens
// that went in. Every PowerShell on PATH runs the round trip - 5.1 and pwsh
// take different paths through native argument passing - and both helpers
// are used: Qt's argv and the C runtime's argv that hashcat's main() gets.
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
    const QStringList shells = findPowerShells();
    const QString qtHelper = findArgumentEcho(QStringLiteral("argecho"));
    const QString crtHelper = findArgumentEcho(QStringLiteral("argecho_crt"));
    if (shells.isEmpty()) {
        skipOrFail("no PowerShell on PATH");
        return;
    }
    if (qtHelper.isEmpty() || crtHelper.isEmpty()) {
        skipOrFail("an argecho helper is not built");
        return;
    }

    QFETCH(QStringList, tokens);
    // Every shell on the runner, and both parsers: Qt's and the C runtime's
    // that hashcat's main() gets.
    for (const QString &shell : shells) {
        for (const QString &helper : {qtHelper, crtHelper}) {
            const LaunchCommand command =
                TerminalCatalog::launchCommand(shell, shell, helper, tokens, false);
            QVERIFY2(!command.program.isEmpty(), qPrintable(shell));

            QProcess proc;
            proc.setProgram(command.program);
            proc.setArguments(command.arguments);
            proc.start();
            QVERIFY2(proc.waitForStarted(10000), qPrintable(proc.errorString()));
            QVERIFY2(proc.waitForFinished(30000), "the shell did not finish in time");
            QCOMPARE(proc.exitCode(), 0);

            const QByteArray output = proc.readAllStandardOutput();
            if (helper == crtHelper) {
                QCOMPARE(parseArgumentDump(output), localBytes(tokens));
                continue;
            }

            const QJsonDocument document = QJsonDocument::fromJson(output);
            QVERIFY2(document.isArray(),
                     qPrintable(QString::fromUtf8(proc.readAllStandardError())));
            QStringList received;
            const QJsonArray array = document.array();
            for (const auto &value : array) {
                received << value.toString();
            }
            QCOMPARE(received, tokens);
        }
    }
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
        skipOrFail("no PowerShell on PATH");
        return;
    }
    const QString helper = findArgumentEcho(QStringLiteral("argecho"));
    if (helper.isEmpty()) {
        skipOrFail("the argecho helper is not built");
        return;
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

// The POSIX clipboard promise: the tokens posixQuoteArgument() produces are
// read back by a real /bin/sh -c as the same argv, metacharacters and all.
// The C-runtime helper is used, so the check is on the bytes a process gets.
void TestTerminalCatalog::posixQuotingRoundTripsThroughTheShell_data()
{
    QTest::addColumn<QStringList>("tokens");

    QTest::newRow("hashcat argv") << QStringList{QStringLiteral("--attack-mode"),
                                                 QStringLiteral("0"),
                                                 QStringLiteral("-m"),
                                                 QStringLiteral("22000"),
                                                 QStringLiteral("$2a$10$abcdefghijklmnopqrstuu"),
                                                 QStringLiteral("?d?d?d?d")};
    QTest::newRow("shell metacharacters") << QStringList{QStringLiteral("?d$(touch /tmp/x)"),
                                                         QStringLiteral("?d;calc"),
                                                         QStringLiteral("a&b"),
                                                         QStringLiteral("a|b"),
                                                         QStringLiteral("`tick`"),
                                                         QStringLiteral("$HOME")};
    QTest::newRow("spaces and quotes")
        << QStringList{QStringLiteral("two words"), QStringLiteral("it's"),
                       QStringLiteral("say \"hi\""), QString()};
    QTest::newRow("backslashes") << QStringList{QStringLiteral("back\\slash"),
                                                QStringLiteral("C:\\dir with space\\"),
                                                QStringLiteral("trail\\")};
    QTest::newRow("line breaks") << QStringList{QStringLiteral("line\nbreak"),
                                                QStringLiteral("tab\there")};
}

void TestTerminalCatalog::posixQuotingRoundTripsThroughTheShell()
{
#ifdef Q_OS_WIN
    QSKIP("the POSIX shell round trip does not apply on Windows");
#else
    const QString shell = QStandardPaths::findExecutable(QStringLiteral("sh"));
    const QString helper = findArgumentEcho(QStringLiteral("argecho_crt"));
    if (shell.isEmpty() || helper.isEmpty()) {
        skipOrFail("no sh or argecho_crt to round-trip through");
        return;
    }

    QFETCH(QStringList, tokens);
    QString command = posixQuoteArgument(helper);
    for (const QString &token : tokens) {
        command += QLatin1Char(' ') + posixQuoteArgument(token);
    }

    QProcess proc;
    proc.setProgram(shell);
    proc.setArguments({QStringLiteral("-c"), command});
    proc.start();
    QVERIFY2(proc.waitForStarted(10000), qPrintable(proc.errorString()));
    QVERIFY2(proc.waitForFinished(30000), "the shell did not finish in time");
    QCOMPARE(proc.exitCode(), 0);
    QCOMPARE(parseArgumentDump(proc.readAllStandardOutput()), localBytes(tokens));
#endif
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

// A PowerShell shell is not a terminal: on Linux and macOS it opens no window
// when started by itself, so discovery must not offer it there. The other way
// round, Windows has no sh for the Unix wrapper, so only the PowerShell path
// is offered there.
void TestTerminalCatalog::availableTerminalsOfferPowerShellOnlyOnWindows()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
#ifdef Q_OS_WIN
    const QString pwshName = QStringLiteral("pwsh.exe");
    const QString weztermName = QStringLiteral("wezterm.exe");
#else
    const QString pwshName = QStringLiteral("pwsh");
    const QString weztermName = QStringLiteral("wezterm");
#endif
    const QString pwsh = TestEnvironment::writeSearchableExecutable(pwshName, directory.path());
    const QString wezterm =
        TestEnvironment::writeSearchableExecutable(weztermName, directory.path());
    QVERIFY2(!pwsh.isEmpty() && !wezterm.isEmpty(), "the terminal stubs could not be written");

    TestEnvironment::PathGuard guard(directory.path().toUtf8());

    const QMap<QString, QString> terminals = TerminalCatalog::availableTerminals();
#ifdef Q_OS_WIN
    QVERIFY2(terminals.contains(QStringLiteral("pwsh")),
             "Windows did not offer the installed pwsh");
    QVERIFY2(!terminals.contains(QStringLiteral("wezterm")),
             "Windows offered a terminal that needs the Unix sh wrapper");
#else
    QVERIFY2(!terminals.contains(QStringLiteral("pwsh")),
             "Unix offered pwsh although it opens no window");
    QVERIFY2(terminals.contains(QStringLiteral("wezterm")),
             "Unix did not offer the installed wezterm");
#endif
    QVERIFY(TerminalCatalog::isPowerShell(QStringLiteral("pwsh")));
}

// ptyxis is broken for hashcat but may be the only terminal installed. The
// message then names it and the reason instead of leaving the user with "no
// supported terminal".
void TestTerminalCatalog::noSupportedTerminalMessageNamesPtyxis()
{
#ifdef Q_OS_WIN
    QSKIP("ptyxis is a Unix terminal");
#else
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString stub =
        TestEnvironment::writeSearchableExecutable(QStringLiteral("ptyxis"), directory.path());
    QVERIFY2(!stub.isEmpty(), "the ptyxis stub could not be written");
    TestEnvironment::PathGuard guard(directory.path().toUtf8());

    const QString message = TerminalCatalog::noSupportedTerminalMessage();
    QVERIFY2(message.contains(QStringLiteral("ptyxis")), qPrintable(message));
    QVERIFY2(message.contains(QStringLiteral("$")), qPrintable(message));
#endif
}

// A hashcat binary reached through a symlink runs next to the real file: its
// kernels are there, not in the directory the link sits in. The program path
// itself stays the resolved link.
void TestTerminalCatalog::programDirectoryFollowsSymlinks()
{
#ifdef Q_OS_WIN
    QSKIP("symlinks need privileges on Windows");
#else
    QTemporaryDir target;
    QTemporaryDir links;
    QVERIFY(target.isValid() && links.isValid());
    const QString hashcat = TestEnvironment::writeShellStub(QStringLiteral("hashcat-linked"),
                                                            "#!/bin/sh\nexit 0\n", target.path());
    QVERIFY2(!hashcat.isEmpty(), "the hashcat stub could not be written");
    const QString link = links.filePath(QStringLiteral("hashcat"));
    QVERIFY2(QFile::link(hashcat, link), "the symlink could not be created");

    const QString expected = QFileInfo(hashcat).canonicalPath();
    const LaunchCommand command = TerminalCatalog::launchCommand(
        QStringLiteral("xterm"), QStringLiteral("xterm"), link, {QStringLiteral("--help")}, true);
    const qsizetype marker = command.arguments.indexOf(QStringLiteral("hashcat-gui"));
    QVERIFY(marker >= 0);
    QCOMPARE(command.arguments.at(marker + 1), expected);

    const QString script = TerminalCatalog::powerShellScript(link, {QStringLiteral("--help")});
    QVERIFY2(script.contains(QStringLiteral("$si.WorkingDirectory = ")
                             + powerShellQuoteArgument(expected)),
             qPrintable(script));
#endif
}

// The detached start runs the program in the hashcat binary's own directory
// and hands it the arguments unchanged. The stub records both. The binary is
// reached through a symlink in another directory: the working directory has
// to be the real file's, not the link's.
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

    const QString hashcat = TestEnvironment::writeShellStub(
        QStringLiteral("hashcat-detached"), "#!/bin/sh\nexit 0\n", directory.path());
    QVERIFY2(!hashcat.isEmpty(), "the hashcat stub could not be written");
    QTemporaryDir links;
    QVERIFY(links.isValid());
    const QString hashcatPath = links.filePath(QStringLiteral("hashcat"));
    QVERIFY2(QFile::link(hashcat, hashcatPath), "the symlink could not be created");

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

// The wrapper sets the working directory and hands the hashcat command to
// the shell as positional parameters: the recorded cwd and argv are exactly
// what the GUI built, even for values a shell would expand or split.
void TestTerminalCatalog::unixWrapperRunsHashcatInItsDirectoryWithTheArguments()
{
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString record = directory.filePath(QStringLiteral("record.txt"));
    const QString hashcat = TestEnvironment::writeShellStub(
        QStringLiteral("stub-hashcat-wrapper"),
        QStringLiteral("#!/bin/sh\npwd > \"%1\"\nprintf '%s\\n' \"$@\" >> \"%1\"\n")
            .arg(record)
            .toUtf8(),
        directory.path());
    QVERIFY2(!hashcat.isEmpty(), "the hashcat stub could not be written");

    // A symlink in another directory: the wrapper's cd has to reach the real
    // file's directory, not the link's.
    QTemporaryDir links;
    QVERIFY(links.isValid());
    const QString hashcatPath = links.filePath(QStringLiteral("hashcat"));
    QVERIFY2(QFile::link(hashcat, hashcatPath), "the symlink could not be created");

    // A stand-in terminal: consume xterm's -e and run the wrapper, with stdin
    // closed so the wrapper's read does not wait for a keypress.
    const QString terminal = TestEnvironment::writeShellStub(
        QStringLiteral("stub-terminal-xterm"), "#!/bin/sh\nshift\nexec \"$@\" </dev/null\n",
        directory.path());
    QVERIFY2(!terminal.isEmpty(), "the terminal stub could not be written");

    const QStringList arguments{QStringLiteral("--attack-mode"), QStringLiteral("0"),
                                QStringLiteral("$2a$10$abc"), QStringLiteral("?d;calc")};
    const LaunchCommand command = TerminalCatalog::launchCommand(QStringLiteral("xterm"), terminal,
                                                                 hashcatPath, arguments, true);
    QCOMPARE(int(command.refusal), int(LaunchCommand::Refusal::None));

    QString error;
    QVERIFY2(TerminalCatalog::launchDetached(command, hashcatPath, &error), qPrintable(error));

    QTRY_VERIFY_WITH_TIMEOUT(QFile::exists(record), 5000);
    QFile file(record);
    QVERIFY(file.open(QIODevice::ReadOnly));
    const QStringList recorded =
        QString::fromUtf8(file.readAll()).split(QLatin1Char('\n'), Qt::SkipEmptyParts);
    QCOMPARE(recorded.size(), 1 + arguments.size());
    QCOMPARE(QDir(recorded.at(0)).canonicalPath(), QDir(directory.path()).canonicalPath());
    QCOMPARE(recorded.mid(1), arguments);
#endif
}

// Ctrl-C goes to the whole foreground process group. hashcat handles it and
// exits, but dash has no handler and would die with it, closing the terminal
// before the status line. The wrapper traps INT with a handler (never an
// ignored signal, which the child would inherit), so the status and the
// prompt always appear. Run under every sh on the machine, with the wrapper's
// `sh` resolved through a one-entry PATH.
void TestTerminalCatalog::unixWrapperSurvivesInterrupt()
{
#ifdef Q_OS_WIN
    QSKIP("the Unix wrapper signal test does not apply on Windows");
#else
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString hashcat =
        TestEnvironment::writeShellStub(QStringLiteral("stub-hashcat-interrupt"),
                                        "#!/bin/sh\n"
                                        "trap 'echo HASH_INT; exit 2' INT\n"
                                        "echo HASH_STARTED\n"
                                        "sleep 10\n",
                                        directory.path());
    const QString terminal =
        TestEnvironment::writeShellStub(QStringLiteral("stub-terminal-interrupt"),
                                        "#!/bin/sh\nshift\nexec \"$@\"\n", directory.path());
    QVERIFY2(!hashcat.isEmpty() && !terminal.isEmpty(), "the stubs could not be written");

    const LaunchCommand command = TerminalCatalog::launchCommand(
        QStringLiteral("xterm"), terminal, hashcat,
        {QStringLiteral("--attack-mode"), QStringLiteral("0")}, true);
    QCOMPARE(int(command.refusal), int(LaunchCommand::Refusal::None));

    QStringList shells{QStringLiteral("/bin/sh")};
    for (const QString &name : {QStringLiteral("dash"), QStringLiteral("bash")}) {
        const QString path = QStandardPaths::findExecutable(name);
        if (!path.isEmpty() && !shells.contains(path)) {
            shells << path;
        }
    }

    for (const QString &shell : shells) {
        // The wrapper calls `sh`; the shim comes first so that name picks
        // this shell, while the original PATH keeps the stub's own tools
        // (sleep) available.
        QTemporaryDir shim;
        QVERIFY(shim.isValid());
        const QString sh = shim.filePath(QStringLiteral("sh"));
        QVERIFY2(QFile::link(shell, sh), qPrintable(shell));
        TestEnvironment::PathGuard guard(shim.path().toUtf8() + ':' + qgetenv("PATH"));

        QProcess process;
        process.setProgram(command.program);
        process.setArguments(command.arguments);
        process.setChildProcessModifier([] { ::setsid(); });
        process.start();
        QVERIFY2(process.waitForStarted(10000), qPrintable(process.errorString()));

        QByteArray output;
        QElapsedTimer timer;
        timer.start();
        while (!output.contains("HASH_STARTED")) {
            QVERIFY2(timer.elapsed() < 10000,
                     qPrintable(QStringLiteral("%1: %2").arg(shell, QString::fromUtf8(output))));
            process.waitForReadyRead(100);
            output += process.readAllStandardOutput();
        }

        // The stub prints HASH_STARTED before it starts its foreground sleep;
        // give it that moment so the signal hits the running child, as Ctrl-C
        // does in a real terminal.
        QTest::qWait(250);
        QVERIFY2(::kill(-static_cast<pid_t>(process.processId()), SIGINT) == 0,
                 qPrintable(QStringLiteral("could not signal %1").arg(shell)));

        // The wrapper has to survive the interrupt; the child's HASH_INT
        // proves the signal reached the group. The status line may still sit
        // in the pipe buffer (stdout is not a TTY here), so close stdin first
        // and assert it after the shell has flushed and exited. An old wrapper
        // is already gone at this point and cannot print it.
        while (!output.contains("HASH_INT")) {
            if (process.state() == QProcess::NotRunning) {
                break;
            }
            QVERIFY2(timer.elapsed() < 10000,
                     qPrintable(QStringLiteral("%1: %2").arg(shell, QString::fromUtf8(output))));
            process.waitForReadyRead(100);
            output += process.readAllStandardOutput();
        }
        if (process.state() != QProcess::NotRunning) {
            process.write("\n");
            process.closeWriteChannel();
        }
        QVERIFY2(process.waitForFinished(10000),
                 qPrintable(QStringLiteral("%1: %2").arg(shell, QString::fromUtf8(output))));
        output += process.readAllStandardOutput();

        QVERIFY2(output.contains("HASH_INT"),
                 qPrintable(QStringLiteral("%1: %2").arg(shell, QString::fromUtf8(output))));
        QVERIFY2(output.contains("exited with 2"),
                 qPrintable(QStringLiteral("%1: %2").arg(shell, QString::fromUtf8(output))));
        QCOMPARE(process.exitStatus(), QProcess::NormalExit);
        QCOMPARE(process.exitCode(), 0);
    }
#endif
}

// A hashcat directory that is gone must say so and wait, not close the window
// without a word.
void TestTerminalCatalog::unixWrapperReportsAFailedDirectory()
{
#ifdef Q_OS_WIN
    QSKIP("the Unix wrapper test does not apply on Windows");
#else
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString terminal = TestEnvironment::writeShellStub(
        QStringLiteral("stub-terminal-failed-cd"), "#!/bin/sh\nshift\nexec \"$@\" </dev/null\n",
        directory.path());
    QVERIFY2(!terminal.isEmpty(), "the terminal stub could not be written");

    const QString hashcat = directory.filePath(QStringLiteral("gone/hashcat"));
    const LaunchCommand command = TerminalCatalog::launchCommand(
        QStringLiteral("xterm"), terminal, hashcat, {QStringLiteral("--help")}, true);
    QCOMPARE(int(command.refusal), int(LaunchCommand::Refusal::None));

    QProcess process;
    process.setProgram(command.program);
    process.setArguments(command.arguments);
    process.start();
    QVERIFY2(process.waitForStarted(10000), qPrintable(process.errorString()));
    QVERIFY2(process.waitForFinished(10000), "the wrapper did not finish");

    const QByteArray output = process.readAllStandardOutput() + process.readAllStandardError();
    QVERIFY2(output.contains("cannot enter"), qPrintable(QString::fromUtf8(output)));
    QVERIFY2(output.contains("[press Enter to close]"), qPrintable(QString::fromUtf8(output)));
    QCOMPARE(process.exitStatus(), QProcess::NormalExit);
    QCOMPARE(process.exitCode(), 1);
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
