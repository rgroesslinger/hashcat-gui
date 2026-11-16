/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "argumentquoting.h"
#include "testenvironment.h"

namespace
{
// The C runtime's command-line rules from Appendix A: whitespace outside
// quotes is space or tab; 2n backslashes before a quote yield n backslashes
// and toggle quote mode, 2n+1 yield n and a literal quote; backslashes not
// before a quote are literal; "" inside quote mode is one literal quote.
QStringList splitCommandLine(const QString &line)
{
    QStringList arguments;
    QString token;
    bool tokenStarted = false;
    bool inQuotes = false;
    qsizetype i = 0;
    while (i < line.size()) {
        const QChar c = line.at(i);
        if (!inQuotes && (c == QLatin1Char(' ') || c == QLatin1Char('\t'))) {
            if (tokenStarted) {
                arguments << token;
                token.clear();
                tokenStarted = false;
            }
            ++i;
            continue;
        }
        tokenStarted = true;
        if (c == QLatin1Char('\\')) {
            qsizetype run = 0;
            while (i + run < line.size() && line.at(i + run) == QLatin1Char('\\')) {
                ++run;
            }
            if (i + run < line.size() && line.at(i + run) == QLatin1Char('"')) {
                token += QString(run / 2, QLatin1Char('\\'));
                if (run % 2 == 1) {
                    token += QLatin1Char('"');
                } else {
                    inQuotes = !inQuotes;
                }
                i += run + 1;
            } else {
                token += QString(run, QLatin1Char('\\'));
                i += run;
            }
            continue;
        }
        if (c == QLatin1Char('"')) {
            if (inQuotes && i + 1 < line.size() && line.at(i + 1) == QLatin1Char('"')) {
                token += QLatin1Char('"');
                i += 2;
            } else {
                inQuotes = !inQuotes;
                ++i;
            }
            continue;
        }
        token += c;
        ++i;
    }
    if (tokenStarted) {
        arguments << token;
    }
    return arguments;
}
} // namespace

// The two command-line quoters: the MSVCRT rules the PowerShell launch
// script and the Windows preview share, and the POSIX rules of the Unix
// preview. The expected strings follow the parsing rules of the target
// (CommandLineToArgvW / a POSIX shell), not the implementation.
class TestArgumentQuoting : public QObject
{
    Q_OBJECT

private slots:
    void msvcrtQuotesForTheCommandLineParser_data();
    void msvcrtQuotesForTheCommandLineParser();
    void msvcrtJoinsTheArgumentList();
    void posixQuotesForTheShell_data();
    void posixQuotesForTheShell();
    void powerShellQuotesForTheShell_data();
    void powerShellQuotesForTheShell();
    void msvcrtQuotingRoundTripsThroughAReferenceParser();
};

void TestArgumentQuoting::msvcrtQuotesForTheCommandLineParser_data()
{
    QTest::addColumn<QString>("token");
    QTest::addColumn<QString>("quoted");

    QTest::newRow("empty") << QString() << QStringLiteral("\"\"");
    QTest::newRow("plain") << QStringLiteral("plain") << QStringLiteral("plain");
    QTest::newRow("flag") << QStringLiteral("--attack-mode") << QStringLiteral("--attack-mode");
    QTest::newRow("spaces") << QStringLiteral("two words") << QStringLiteral("\"two words\"");
    QTest::newRow("tab") << QStringLiteral("tab\there") << QStringLiteral("\"tab\there\"");
    QTest::newRow("newline") << QStringLiteral("line\nbreak") << QStringLiteral("\"line\nbreak\"");
    QTest::newRow("embedded quote") << QStringLiteral("a\"b") << QStringLiteral("a\\\"b");
    QTest::newRow("spaces and quotes")
        << QStringLiteral("say \"hi\"") << QStringLiteral("\"say \\\"hi\\\"\"");
    QTest::newRow("backslash before quote")
        << QStringLiteral("a\\\"b") << QStringLiteral("a\\\\\\\"b");
    QTest::newRow("two backslashes before quote")
        << QStringLiteral("a\\\\\"b") << QStringLiteral("a\\\\\\\\\\\"b");
    QTest::newRow("only quotes") << QStringLiteral("\"\"") << QStringLiteral("\\\"\\\"");
    QTest::newRow("trailing backslash, no spaces")
        << QStringLiteral("C:\\dir\\") << QStringLiteral("C:\\dir\\");
    QTest::newRow("trailing backslash and spaces")
        << QStringLiteral("C:\\dir with space\\") << QStringLiteral("\"C:\\dir with space\\\\\"");
    QTest::newRow("two trailing backslashes and spaces")
        << QStringLiteral("dir \\\\") << QStringLiteral("\"dir \\\\\\\\\"");
    QTest::newRow("backslashes away from quotes")
        << QStringLiteral("back\\slash") << QStringLiteral("back\\slash");
    QTest::newRow("dollar and parens stay literal")
        << QStringLiteral("$2a$10$(x)") << QStringLiteral("$2a$10$(x)");
}

void TestArgumentQuoting::msvcrtQuotesForTheCommandLineParser()
{
    QFETCH(QString, token);
    QFETCH(QString, quoted);
    QCOMPARE(msvcrtQuoteArgument(token), quoted);
}

void TestArgumentQuoting::msvcrtJoinsTheArgumentList()
{
    const QStringList arguments{QStringLiteral("--attack-mode"), QStringLiteral("0"),
                                QStringLiteral("two words"), QString(),
                                QStringLiteral("say \"hi\"")};
    QCOMPARE(msvcrtJoinArguments(arguments),
             QStringLiteral("--attack-mode 0 \"two words\" \"\" \"say \\\"hi\\\"\""));
    QCOMPARE(msvcrtJoinArguments({}), QString());
}

void TestArgumentQuoting::posixQuotesForTheShell_data()
{
    QTest::addColumn<QString>("token");
    QTest::addColumn<QString>("quoted");

    QTest::newRow("empty") << QString() << QStringLiteral("''");
    QTest::newRow("plain") << QStringLiteral("plain") << QStringLiteral("plain");
    QTest::newRow("flag") << QStringLiteral("--attack-mode") << QStringLiteral("--attack-mode");
    QTest::newRow("safe punctuation") << QStringLiteral("1,2") << QStringLiteral("1,2");
    QTest::newRow("assignment") << QStringLiteral("scale=2") << QStringLiteral("scale=2");
    QTest::newRow("path") << QStringLiteral("/path/to.x") << QStringLiteral("/path/to.x");
    QTest::newRow("spaces") << QStringLiteral("two words") << QStringLiteral("'two words'");
    QTest::newRow("single quote") << QStringLiteral("it's") << QStringLiteral("'it'\\''s'");
    QTest::newRow("double quotes")
        << QStringLiteral("say \"hi\"") << QStringLiteral("'say \"hi\"'");
    QTest::newRow("dollar") << QStringLiteral("$HOME") << QStringLiteral("'$HOME'");
    QTest::newRow("mask") << QStringLiteral("?d?d") << QStringLiteral("'?d?d'");
    QTest::newRow("glob") << QStringLiteral("*glob*") << QStringLiteral("'*glob*'");
    QTest::newRow("backslash") << QStringLiteral("back\\slash") << QStringLiteral("'back\\slash'");
    QTest::newRow("backtick") << QStringLiteral("`tick`") << QStringLiteral("'`tick`'");
    QTest::newRow("non-ascii") << QStringLiteral("ünïcödé") << QStringLiteral("'ünïcödé'");
    QTest::newRow("mixed quotes") << QStringLiteral("a'b\"c") << QStringLiteral("'a'\\''b\"c'");
    QTest::newRow("recipe") << QStringLiteral("md5($p.'x')")
                            << QStringLiteral("'md5($p.'\\''x'\\'')'");
}

void TestArgumentQuoting::posixQuotesForTheShell()
{
    QFETCH(QString, token);
    QFETCH(QString, quoted);
    QCOMPARE(posixQuoteArgument(token), quoted);
}

// PowerShell's single-quoted string: every token is wrapped, an embedded
// quote is doubled, and the smart single quotes it would read as delimiters
// travel as the token's UTF-8 bytes decoded from base64 inside the script.
void TestArgumentQuoting::powerShellQuotesForTheShell_data()
{
    QTest::addColumn<QString>("token");
    QTest::addColumn<QString>("quoted");

    QTest::newRow("empty") << QString() << QStringLiteral("''");
    QTest::newRow("plain") << QStringLiteral("plain") << QStringLiteral("'plain'");
    QTest::newRow("flag") << QStringLiteral("--attack-mode") << QStringLiteral("'--attack-mode'");
    QTest::newRow("spaces") << QStringLiteral("two words") << QStringLiteral("'two words'");
    QTest::newRow("single quote") << QStringLiteral("it's") << QStringLiteral("'it''s'");
    QTest::newRow("double quotes")
        << QStringLiteral("say \"hi\"") << QStringLiteral("'say \"hi\"'");
    QTest::newRow("dollar") << QStringLiteral("$2a$10$abc") << QStringLiteral("'$2a$10$abc'");
    QTest::newRow("subexpression")
        << QStringLiteral("?d$(Get-Date)") << QStringLiteral("'?d$(Get-Date)'");
    QTest::newRow("semicolon") << QStringLiteral("?d;calc") << QStringLiteral("'?d;calc'");
    QTest::newRow("backtick") << QStringLiteral("`tick`") << QStringLiteral("'`tick`'");
    QTest::newRow("backslash") << QStringLiteral("C:\\dir \\") << QStringLiteral("'C:\\dir \\'");
    QTest::newRow("newline") << QStringLiteral("line\nbreak") << QStringLiteral("'line\nbreak'");
    QTest::newRow("smart single quote")
        << QStringLiteral("?d’?d")
        << QStringLiteral("([Text.Encoding]::UTF8.GetString([Convert]::FromBase64String('"
                          "P2TigJk/ZA==')))");
    QTest::newRow("mixed with a smart quote")
        << QStringLiteral("it's ’mixed’")
        << QStringLiteral("([Text.Encoding]::UTF8.GetString([Convert]::FromBase64String('"
                          "aXQncyDigJltaXhlZOKAmQ==')))");
}

void TestArgumentQuoting::powerShellQuotesForTheShell()
{
    QFETCH(QString, token);
    QFETCH(QString, quoted);
    QCOMPARE(powerShellQuoteArgument(token), quoted);
}

// The quoter and an independent parser: any token list that survives this
// round trip is passed by the C runtime exactly as it went in. The generator
// is seeded and the count fixed, so a failure is reproducible.
void TestArgumentQuoting::msvcrtQuotingRoundTripsThroughAReferenceParser()
{
    // A few fixed lines anchor the parser before the property runs.
    const QStringList spaced{QStringLiteral("a"), QStringLiteral("b c"), QStringLiteral("d")};
    QCOMPARE(splitCommandLine(QStringLiteral("a \"b c\" d")), spaced);
    QCOMPARE(splitCommandLine(QStringLiteral("\"\"")), QStringList{QString()});

    const QString alphabet = QStringLiteral("ab \\\"\t\n$;&|'");
    quint32 state = 0x9e3779b9u;
    const auto next = [&state] {
        state = state * 1664525u + 1013904223u;
        return state;
    };

    for (int iteration = 0; iteration < 2000; ++iteration) {
        QStringList tokens;
        const int count = int(next() % 5u);
        for (int t = 0; t < count; ++t) {
            QString token;
            const int length = int(next() % 9u);
            for (int c = 0; c < length; ++c) {
                token += alphabet.at(int(next() % quint32(alphabet.size())));
            }
            tokens << token;
        }

        const QString line = msvcrtJoinArguments(tokens);
        QVERIFY2(splitCommandLine(line) == tokens,
                 qPrintable(QStringLiteral("iteration %1, line: %2").arg(iteration).arg(line)));
    }
}

int main(int argc, char *argv[])
{
    TestArgumentQuoting test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_argumentquoting.moc"
