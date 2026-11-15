/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "commandpreview.h"
#include "testenvironment.h"

// The display formatting of the command line: spacing, the recipe's display
// quotes, the tooltip's 120-column wrap and its markup escaping.
class TestCommandPreview : public QObject
{
    Q_OBJECT

private slots:
    void formatJoinsTheBinaryAndTheArguments();
    void formatQuotesTheRecipeForDisplay();
    void tooltipHoldsTheWholeCommand();
    void tooltipWrapsAt120Columns();
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

void TestCommandPreview::formatQuotesTheRecipeForDisplay()
{
#ifdef Q_OS_WIN
    const QChar quote = QLatin1Char('"');
#else
    const QChar quote = QLatin1Char('\'');
#endif
    const QStringList arguments{QStringLiteral("--hash-type"),   QStringLiteral("4000"),
                                QStringLiteral("--hash-recipe"), QStringLiteral("md5($p.$s)"),
                                QStringLiteral("--attack-mode"), QStringLiteral("0")};
    QCOMPARE(CommandPreview::format(QStringLiteral("hashcat"), arguments),
             QStringLiteral("hashcat --hash-type 4000 --hash-recipe ") + quote
                 + QStringLiteral("md5($p.$s)") + quote + QStringLiteral(" --attack-mode 0"));

    // A trailing flag with no value has nothing to quote.
    const QStringList trailing{QStringLiteral("--hash-recipe")};
    QCOMPARE(CommandPreview::format(QString(), trailing), QStringLiteral("--hash-recipe"));
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