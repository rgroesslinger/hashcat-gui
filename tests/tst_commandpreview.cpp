/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "commandpreview.h"
#include "testenvironment.h"

// The display formatting of the command line: spacing, the shell quoting of
// every token the shell would split or expand, the tooltip's 120-column wrap
// and its markup escaping.
class TestCommandPreview : public QObject
{
    Q_OBJECT

private slots:
    void formatJoinsTheBinaryAndTheArguments();
    void formatQuotesEveryTokenForTheShell_data();
    void formatQuotesEveryTokenForTheShell();
    void formatQuotesTheBinaryName();
    void tooltipHoldsTheWholeCommand();
    void tooltipWrapsAt120Columns();
    void tooltipKeepsQuotedTokensWhole();
    void tooltipEscapesMarkup();
};

void TestCommandPreview::formatJoinsTheBinaryAndTheArguments()
{
    const QStringList arguments{QStringLiteral("--attack-mode"), QStringLiteral("0")};

    QCOMPARE(CommandPreview::format(QStringLiteral("hashcat"), arguments),
             QStringLiteral("hashcat --attack-mode 0"));

    // Without a configured binary the preview is the bare argument list,
    // which must not start with a blank.
    QCOMPARE(CommandPreview::format(QString(), arguments), QStringLiteral("--attack-mode 0"));
    QCOMPARE(CommandPreview::format(QStringLiteral("hashcat"), {}), QStringLiteral("hashcat "));
    QCOMPARE(CommandPreview::format(QString(), {}), QString());
}

// One token per row, quoted the way the platform's shell reads it: POSIX
// single quotes on Linux for everything outside the literal character set,
// MSVCRT double quotes on Windows only where the token is empty or holds
// whitespace (embedded quotes are escaped either way). The recipe is no
// special case - it is quoted by the same rules as every other token.
void TestCommandPreview::formatQuotesEveryTokenForTheShell_data()
{
    QTest::addColumn<QString>("token");
    QTest::addColumn<QString>("posix");
    QTest::addColumn<QString>("windows");

    QTest::newRow("recipe") << QStringLiteral("md5($p.$s)") << QStringLiteral("'md5($p.$s)'")
                            << QStringLiteral("md5($p.$s)");
    QTest::newRow("recipe with double quotes")
        << QStringLiteral("md5($p.\"<b>\")") << QStringLiteral("'md5($p.\"<b>\")'")
        << QStringLiteral("md5($p.\\\"<b>\\\")");
    QTest::newRow("recipe with single quotes")
        << QStringLiteral("md5($p.'x')") << QStringLiteral("'md5($p.'\\''x'\\'')'")
        << QStringLiteral("md5($p.'x')");
    QTest::newRow("path with spaces")
        << QStringLiteral("/tmp/my hashes.txt") << QStringLiteral("'/tmp/my hashes.txt'")
        << QStringLiteral("\"/tmp/my hashes.txt\"");
    QTest::newRow("mask") << QStringLiteral("?d?d") << QStringLiteral("'?d?d'")
                          << QStringLiteral("?d?d");
    QTest::newRow("session with spaces")
        << QStringLiteral("my job") << QStringLiteral("'my job'") << QStringLiteral("\"my job\"");
    QTest::newRow("empty") << QString() << QStringLiteral("''") << QStringLiteral("\"\"");
    QTest::newRow("safe punctuation")
        << QStringLiteral("1,2") << QStringLiteral("1,2") << QStringLiteral("1,2");
    QTest::newRow("bcrypt hash") << QStringLiteral("$2a$10$abc") << QStringLiteral("'$2a$10$abc'")
                                 << QStringLiteral("$2a$10$abc");
    QTest::newRow("trailing backslash and spaces")
        << QStringLiteral("C:\\dir \\") << QStringLiteral("'C:\\dir \\'")
        << QStringLiteral("\"C:\\dir \\\\\"");
}

void TestCommandPreview::formatQuotesEveryTokenForTheShell()
{
    QFETCH(QString, token);
#ifdef Q_OS_WIN
    QFETCH(QString, windows);
    const QString &quoted = windows;
#else
    QFETCH(QString, posix);
    const QString &quoted = posix;
#endif
    QCOMPARE(CommandPreview::format(QString(), {token}), quoted);

    // A trailing flag with no value has nothing to quote.
    QCOMPARE(CommandPreview::format(QString(), {QStringLiteral("--hash-recipe")}),
             QStringLiteral("--hash-recipe"));
}

void TestCommandPreview::formatQuotesTheBinaryName()
{
    const QStringList arguments{QStringLiteral("--attack-mode"), QStringLiteral("0")};
#ifdef Q_OS_WIN
    const QString expected = QStringLiteral("\"my hashcat\" --attack-mode 0");
#else
    const QString expected = QStringLiteral("'my hashcat' --attack-mode 0");
#endif
    QCOMPARE(CommandPreview::format(QStringLiteral("my hashcat"), arguments), expected);
}

void TestCommandPreview::tooltipHoldsTheWholeCommand()
{
    QCOMPARE(CommandPreview::tooltip(QString()), QString());

    const QString command = QStringLiteral("hashcat --attack-mode 0");
    QCOMPARE(CommandPreview::tooltip(command), command);
}

void TestCommandPreview::tooltipWrapsAt120Columns()
{
    // Two long positional wordlists: the break goes between the arguments,
    // and a flag keeps its value on the same line even past the limit.
    const QString pathA = QString(80, QLatin1Char('a'));
    const QString pathB = QString(80, QLatin1Char('b'));
    const QString command =
        QStringLiteral("hashcat --hash-type 0 --attack-mode 0 ") + pathA + QLatin1Char(' ') + pathB;
    QCOMPARE(CommandPreview::tooltip(command),
             QStringLiteral("hashcat --hash-type 0 --attack-mode 0 ") + pathA + QLatin1Char('\n')
                 + pathB);

    // A single long value after a flag is not broken away from it.
    const QString longSession = QString(150, QLatin1Char('x'));
    const QString flagged = QStringLiteral("hashcat --session ") + longSession;
    QCOMPARE(CommandPreview::tooltip(flagged), flagged);
}

// A quoted token that holds spaces is one argument: the wrap goes before or
// after it, never into it, and the tooltip never rewrites the text - the
// doubled trailing backslashes before a Windows closing quote included.
void TestCommandPreview::tooltipKeepsQuotedTokensWhole()
{
    const QString filler = QString(110, QLatin1Char('a'));
    const QString longDir =
        QStringLiteral("C:\\") + QString(130, QLatin1Char('a')) + QStringLiteral(" with space\\");
    const QString command = CommandPreview::format(
        QStringLiteral("hashcat"), {QStringLiteral("--session"), filler, longDir,
                                    QStringLiteral("two words"), QStringLiteral("tail")});
    const QString tooltip = CommandPreview::tooltip(command);
    QVERIFY(tooltip.contains(QStringLiteral("two words")));
    QVERIFY(!tooltip.contains(QStringLiteral("two\nwords")));
    QVERIFY(tooltip.contains(longDir));

    // The tooltip only swaps the spaces between tokens for breaks: putting
    // the spaces back gives the command, character for character.
    QString rejoined = tooltip;
    rejoined.replace(QLatin1Char('\n'), QLatin1Char(' '));
    QCOMPARE(rejoined, command);
}

void TestCommandPreview::tooltipEscapesMarkup()
{
    // A path that looks like markup is escaped and its breaks become <br>;
    // plain text is left alone, breaks included.
    const QString command = QStringLiteral("hashcat --outfile /tmp/<b>out</b>.txt");
    QCOMPARE(CommandPreview::tooltip(command),
             QStringLiteral("hashcat --outfile /tmp/&lt;b&gt;out&lt;/b&gt;.txt"));

    const QString richBreak = QStringLiteral("hashcat --session <b>a\nb</b>");
    QCOMPARE(CommandPreview::tooltip(richBreak),
             QStringLiteral("hashcat --session &lt;b&gt;a<br>b&lt;/b&gt;"));

    const QString plainBreak = QStringLiteral("hashcat --session a\nb");
    QCOMPARE(CommandPreview::tooltip(plainBreak), plainBreak);
}

int main(int argc, char *argv[])
{
    TestCommandPreview test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_commandpreview.moc"
