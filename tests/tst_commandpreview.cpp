/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "commandpreview.h"
#include "testenvironment.h"

// The display formatting of the command line: spacing, the shell quoting of
// the tokens that need it in the configured terminal's dialect, the tooltip's
// 120-column wrap and its markup escaping.
class TestCommandPreview : public QObject
{
    Q_OBJECT

private slots:
    void formatJoinsTheBinaryAndTheArguments();
    void formatQuotesTokensForTheShell_data();
    void formatQuotesTokensForTheShell();
    void formatQuotesTheBinaryName();
    void tooltipHoldsTheWholeCommand();
    void tooltipWrapsAt120Columns();
    void tooltipKeepsQuotedTokensWhole();
    void tooltipKeepsPowerShellTokensWhole();
    void tooltipEscapesMarkup();
};

using Dialect = CommandPreview::Dialect;

void TestCommandPreview::formatJoinsTheBinaryAndTheArguments()
{
    const QStringList arguments{QStringLiteral("--attack-mode"), QStringLiteral("0")};

    QCOMPARE(CommandPreview::format(QStringLiteral("hashcat"), arguments, Dialect::Posix),
             QStringLiteral("hashcat --attack-mode 0"));
    QCOMPARE(CommandPreview::format(QStringLiteral("hashcat"), arguments, Dialect::PowerShell),
             QStringLiteral("& hashcat --attack-mode 0"));

    // Without a configured binary the preview is the bare argument list,
    // which must not start with a blank. PowerShell has nothing to call, so
    // the call operator stays out too.
    QCOMPARE(CommandPreview::format(QString(), arguments, Dialect::Posix),
             QStringLiteral("--attack-mode 0"));
    QCOMPARE(CommandPreview::format(QString(), arguments, Dialect::PowerShell),
             QStringLiteral("--attack-mode 0"));
    QCOMPARE(CommandPreview::format(QStringLiteral("hashcat"), {}, Dialect::Posix),
             QStringLiteral("hashcat "));
    QCOMPARE(CommandPreview::format(QStringLiteral("hashcat"), {}, Dialect::PowerShell),
             QStringLiteral("& hashcat "));
    QCOMPARE(CommandPreview::format(QString(), {}, Dialect::Posix), QString());
    QCOMPARE(CommandPreview::format(QString(), {}, Dialect::PowerShell), QString());
}

// One token per row, quoted the way each shell reads it: literal tokens stay
// bare, the rest are single-quoted (POSIX splice, doubled PowerShell quote,
// base64 for smart quotes). The dialects differ on commas and backslashes;
// both quote the wildcards pwsh expands on Unix, and the recipe follows the
// same rules.
void TestCommandPreview::formatQuotesTokensForTheShell_data()
{
    QTest::addColumn<QString>("token");
    QTest::addColumn<QString>("posix");
    QTest::addColumn<QString>("powerShell");

    QTest::newRow("flag") << QStringLiteral("--hash-recipe") << QStringLiteral("--hash-recipe")
                          << QStringLiteral("--hash-recipe");
    QTest::newRow("number") << QStringLiteral("1400") << QStringLiteral("1400")
                            << QStringLiteral("1400");
    QTest::newRow("windows path") << QStringLiteral("C:\\hashcat\\hashcat.exe")
                                  << QStringLiteral("'C:\\hashcat\\hashcat.exe'")
                                  << QStringLiteral("C:\\hashcat\\hashcat.exe");
    QTest::newRow("recipe") << QStringLiteral("md5($p.$s)") << QStringLiteral("'md5($p.$s)'")
                            << QStringLiteral("'md5($p.$s)'");
    QTest::newRow("recipe with double quotes")
        << QStringLiteral("md5($p.\"<b>\")") << QStringLiteral("'md5($p.\"<b>\")'")
        << QStringLiteral("'md5($p.\"<b>\")'");
    QTest::newRow("recipe with single quotes")
        << QStringLiteral("md5($p.'x')") << QStringLiteral("'md5($p.'\\''x'\\'')'")
        << QStringLiteral("'md5($p.''x'')'");
    QTest::newRow("path with spaces")
        << QStringLiteral("/tmp/my hashes.txt") << QStringLiteral("'/tmp/my hashes.txt'")
        << QStringLiteral("'/tmp/my hashes.txt'");
    QTest::newRow("mask") << QStringLiteral("?d?d") << QStringLiteral("'?d?d'")
                          << QStringLiteral("'?d?d'");
    QTest::newRow("session with spaces")
        << QStringLiteral("my job") << QStringLiteral("'my job'") << QStringLiteral("'my job'");
    QTest::newRow("empty") << QString() << QStringLiteral("''") << QStringLiteral("''");
    QTest::newRow("safe punctuation")
        << QStringLiteral("1,2") << QStringLiteral("1,2") << QStringLiteral("'1,2'");
    QTest::newRow("bcrypt hash") << QStringLiteral("$2a$10$abc") << QStringLiteral("'$2a$10$abc'")
                                 << QStringLiteral("'$2a$10$abc'");
    QTest::newRow("subexpression")
        << QStringLiteral("?d$(Get-Date)") << QStringLiteral("'?d$(Get-Date)'")
        << QStringLiteral("'?d$(Get-Date)'");
    QTest::newRow("semicolon") << QStringLiteral("?d;calc") << QStringLiteral("'?d;calc'")
                               << QStringLiteral("'?d;calc'");
    QTest::newRow("trailing backslash and spaces")
        << QStringLiteral("C:\\dir \\") << QStringLiteral("'C:\\dir \\'")
        << QStringLiteral("'C:\\dir \\'");
    QTest::newRow("smart single quote")
        << QStringLiteral("?d’?d") << QStringLiteral("'?d’?d'")
        << QStringLiteral("([Text.Encoding]::UTF8.GetString([Convert]::FromBase64String('"
                          "P2TigJk/ZA==')))");
    QTest::newRow("mixed quotes and a smart quote")
        << QStringLiteral("it's ’mixed’") << QStringLiteral("'it'\\''s ’mixed’'")
        << QStringLiteral("([Text.Encoding]::UTF8.GetString([Convert]::FromBase64String('"
                          "aXQncyDigJltaXhlZOKAmQ==')))");
    QTest::newRow("newline") << QStringLiteral("line\nbreak") << QStringLiteral("'line\nbreak'")
                             << QStringLiteral("'line\nbreak'");
}

void TestCommandPreview::formatQuotesTokensForTheShell()
{
    QFETCH(QString, token);
    QFETCH(QString, posix);
    QFETCH(QString, powerShell);

    QCOMPARE(CommandPreview::format(QString(), {token}, Dialect::Posix), posix);
    QCOMPARE(CommandPreview::format(QString(), {token}, Dialect::PowerShell), powerShell);
}

void TestCommandPreview::formatQuotesTheBinaryName()
{
    const QStringList arguments{QStringLiteral("--attack-mode"), QStringLiteral("0")};

    QCOMPARE(CommandPreview::format(QStringLiteral("my hashcat"), arguments, Dialect::Posix),
             QStringLiteral("'my hashcat' --attack-mode 0"));
    QCOMPARE(CommandPreview::format(QStringLiteral("my hashcat"), arguments, Dialect::PowerShell),
             QStringLiteral("& 'my hashcat' --attack-mode 0"));
}

void TestCommandPreview::tooltipHoldsTheWholeCommand()
{
    QCOMPARE(CommandPreview::tooltip(QString(), Dialect::Posix), QString());

    const QString command = QStringLiteral("hashcat --attack-mode 0");
    QCOMPARE(CommandPreview::tooltip(command, Dialect::Posix), command);
}

void TestCommandPreview::tooltipWrapsAt120Columns()
{
    // Two long positional wordlists: the break goes between the arguments,
    // and a flag keeps its value on the same line even past the limit.
    const QString pathA = QString(80, QLatin1Char('a'));
    const QString pathB = QString(80, QLatin1Char('b'));
    const QString command =
        QStringLiteral("hashcat --hash-type 0 --attack-mode 0 ") + pathA + QLatin1Char(' ') + pathB;
    QCOMPARE(CommandPreview::tooltip(command, Dialect::Posix),
             QStringLiteral("hashcat --hash-type 0 --attack-mode 0 ") + pathA + QLatin1Char('\n')
                 + pathB);

    // A single long value after a flag is not broken away from it.
    const QString longSession = QString(150, QLatin1Char('x'));
    const QString flagged = QStringLiteral("hashcat --session ") + longSession;
    QCOMPARE(CommandPreview::tooltip(flagged, Dialect::Posix), flagged);
}

// A quoted token that holds spaces is one argument: the wrap goes before or
// after it, never into it, and the tooltip never rewrites the text - the
// POSIX quote splice included.
void TestCommandPreview::tooltipKeepsQuotedTokensWhole()
{
    const QString filler = QString(110, QLatin1Char('a'));
    const QString longDir =
        QStringLiteral("C:\\") + QString(130, QLatin1Char('a')) + QStringLiteral(" with space\\");
    const QString command =
        CommandPreview::format(QStringLiteral("hashcat"),
                               {QStringLiteral("--session"), filler, longDir,
                                QStringLiteral("two words"), QStringLiteral("tail")},
                               Dialect::Posix);
    const QString tooltip = CommandPreview::tooltip(command, Dialect::Posix);
    QVERIFY(tooltip.contains(QStringLiteral("two words")));
    QVERIFY(!tooltip.contains(QStringLiteral("two\nwords")));
    QVERIFY(tooltip.contains(longDir));

    // The tooltip only swaps the spaces between tokens for breaks: putting
    // the spaces back gives the command, character for character.
    QString rejoined = tooltip;
    rejoined.replace(QLatin1Char('\n'), QLatin1Char(' '));
    QCOMPARE(rejoined, command);
}

// The same for the PowerShell dialect, which leaves literal tokens bare and
// single-quotes the rest, and whose backslash is literal: a trailing backslash
// must not break the quote tracking, and the call operator stays with the
// binary.
void TestCommandPreview::tooltipKeepsPowerShellTokensWhole()
{
    const QString filler = QString(110, QLatin1Char('a'));
    const QString longDir =
        QStringLiteral("C:\\") + QString(130, QLatin1Char('a')) + QStringLiteral(" with space\\");
    const QString command =
        CommandPreview::format(QStringLiteral("hashcat"),
                               {QStringLiteral("--session"), filler, longDir,
                                QStringLiteral("two words"), QStringLiteral("tail")},
                               Dialect::PowerShell);
    const QString tooltip = CommandPreview::tooltip(command, Dialect::PowerShell);
    QVERIFY2(tooltip.startsWith(QStringLiteral("& hashcat")), qPrintable(tooltip));
    QVERIFY(tooltip.contains(QStringLiteral("'two words'")));
    QVERIFY(!tooltip.contains(QStringLiteral("'two\nwords'")));
    QVERIFY(tooltip.contains(longDir));

    QString rejoined = tooltip;
    rejoined.replace(QLatin1Char('\n'), QLatin1Char(' '));
    QCOMPARE(rejoined, command);
}

void TestCommandPreview::tooltipEscapesMarkup()
{
    // A path that looks like markup is escaped and its breaks become <br>;
    // plain text is left alone, breaks included.
    const QString command = QStringLiteral("hashcat --outfile /tmp/<b>out</b>.txt");
    QCOMPARE(CommandPreview::tooltip(command, Dialect::Posix),
             QStringLiteral("hashcat --outfile /tmp/&lt;b&gt;out&lt;/b&gt;.txt"));

    const QString richBreak = QStringLiteral("hashcat --session <b>a\nb</b>");
    QCOMPARE(CommandPreview::tooltip(richBreak, Dialect::Posix),
             QStringLiteral("hashcat --session &lt;b&gt;a<br>b&lt;/b&gt;"));

    const QString plainBreak = QStringLiteral("hashcat --session a\nb");
    QCOMPARE(CommandPreview::tooltip(plainBreak, Dialect::Posix), plainBreak);
}

int main(int argc, char *argv[])
{
    TestCommandPreview test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_commandpreview.moc"