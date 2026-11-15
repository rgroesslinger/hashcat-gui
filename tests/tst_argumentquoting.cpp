/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "argumentquoting.h"
#include "testenvironment.h"

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

int main(int argc, char *argv[])
{
    TestArgumentQuoting test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_argumentquoting.moc"
