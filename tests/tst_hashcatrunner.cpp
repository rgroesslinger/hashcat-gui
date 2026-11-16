/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appconstants.h"
#include "hashcatrunner.h"
#include "testenvironment.h"

#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QTemporaryDir>

// The asynchronous hashcat query: the path is a parameter, the output is
// captured, and a hung child is killed rather than waited out.
class TestHashcatRunner : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void executeWithoutAPath();
    void executeCapturesOutput();
    void executeGivesUpOnTimeout();
    void executeAppendsQuietAndUsesBinaryDirectory();
    void executeFollowsASymlinkedBinaryToItsRealDirectory();
    void executeDecodesUtf8Output();

private:
    // The stubs only make sense where a #! line does
    [[nodiscard]] bool stubsAvailable() const
    {
        return !m_okScript.isEmpty();
    }

    QString m_okScript;
    QString m_sleepingScript;
    QString m_echoScript;
};

void TestHashcatRunner::initTestCase()
{
    m_okScript = TestEnvironment::writeShellStub(
        QStringLiteral("stub-hashcat"),
        "#!/bin/sh\n"
        "printf '{\"0\":{\"name\":\"MD5\"},\"1000\":{\"name\":\"NTLM\"}}\\n'\n"
        "exit 0\n");
    m_sleepingScript = TestEnvironment::writeShellStub(
        QStringLiteral("stub-hashcat-slow"),
        // exec replaces the shell, so the process QProcess kills is the
        // sleeper itself instead of a shell whose sleep child is orphaned
        "#!/bin/sh\n"
        "exec sleep 30\n");
    m_echoScript = TestEnvironment::writeShellStub(
        QStringLiteral("stub-hashcat-echo"),
        // reports where it ran and what it was given, one item per line
        "#!/bin/sh\n"
        "pwd\n"
        "printf '%s\\n' \"$@\"\n");
}

void TestHashcatRunner::executeWithoutAPath()
{
    const HashcatResult result =
        HashcatRunner::execute(QString(), {QStringLiteral("--help")}).result();

    // "no process ran" has to look like a failure to every consumer
    QCOMPARE(int(result.exitStatus), int(QProcess::CrashExit));
    QCOMPARE(result.exitCode, -1);
    QVERIFY(result.standardError.contains(QStringLiteral("not configured")));
}

void TestHashcatRunner::executeCapturesOutput()
{
#ifdef Q_OS_WIN
    QSKIP("no shell stubs on this platform");
#else
    // A stub that never got written is a broken test environment, not a
    // reason to report success without having run anything
    QVERIFY2(stubsAvailable(), "the shell stub could not be written");
#endif

    const HashcatResult result =
        HashcatRunner::execute(m_okScript, {QStringLiteral("--example-hashes")}).result();

    QCOMPARE(int(result.exitStatus), int(QProcess::NormalExit));
    QCOMPARE(result.exitCode, 0);
    QVERIFY(!result.timedOut);
    QVERIFY(result.standardOutput.contains(QStringLiteral("NTLM")));
}

void TestHashcatRunner::executeGivesUpOnTimeout()
{
#ifdef Q_OS_WIN
    QSKIP("no shell stubs on this platform");
#else
    QVERIFY2(stubsAvailable(), "the shell stub could not be written");
#endif

    QElapsedTimer elapsed;
    elapsed.start();
    const HashcatResult result =
        HashcatRunner::execute(m_sleepingScript, {QStringLiteral("--help")}, 300).result();

    // The child is killed and reaped rather than waited out: a query that can
    // hang the "Updating…" state forever is what this guards against.
    QVERIFY2(elapsed.elapsed() < 10000, qPrintable(QStringLiteral("%1 ms").arg(elapsed.elapsed())));
    QCOMPARE(int(result.exitStatus), int(QProcess::CrashExit));
    QCOMPARE(result.exitCode, -1);
    QVERIFY(result.timedOut);
    QVERIFY(result.standardError.contains(QStringLiteral("timed out")));
}

// The contract with every caller: the caller's arguments go first, --quiet
// is appended so the output can be read, and the child runs next to its own
// binary - that is where hashcat looks for its modules.
void TestHashcatRunner::executeAppendsQuietAndUsesBinaryDirectory()
{
#ifdef Q_OS_WIN
    QSKIP("no shell stubs on this platform");
#else
    QVERIFY2(!m_echoScript.isEmpty(), "the shell stub could not be written");
#endif

    const HashcatResult result =
        HashcatRunner::execute(m_echoScript, {QStringLiteral("--version")}).result();

    QCOMPARE(int(result.exitStatus), int(QProcess::NormalExit));
    QCOMPARE(result.exitCode, 0);

    const QStringList output = result.standardOutput.split(QLatin1Char('\n'), Qt::SkipEmptyParts);
    QCOMPARE(output.size(), 3);

    // working directory = the directory of the binary (canonical, so a
    // symlinked temp path cannot tell the two apart)
    QCOMPARE(QDir(output.value(0)).canonicalPath(),
             QDir(QFileInfo(m_echoScript).absolutePath()).canonicalPath());

    // the caller's arguments, then --quiet - in that order
    const QStringList expectedArguments{QStringLiteral("--version"),
                                        QString(AppConstants::Hashcat::Quiet)};
    QCOMPARE(output.mid(1), expectedArguments);
}

// A symlink in another folder must not make the query run in that folder:
// hashcat looks for its modules next to the real binary.
void TestHashcatRunner::executeFollowsASymlinkedBinaryToItsRealDirectory()
{
#ifdef Q_OS_WIN
    QSKIP("no shell stubs or symlinks on this platform");
#else
    QVERIFY2(!m_echoScript.isEmpty(), "the shell stub could not be written");
    QTemporaryDir links;
    QVERIFY(links.isValid());
    const QString link = links.filePath(QStringLiteral("hashcat-link"));
    QVERIFY(QFile::link(m_echoScript, link));

    const HashcatResult result =
        HashcatRunner::execute(link, {QStringLiteral("--version")}).result();
    QCOMPARE(int(result.exitStatus), int(QProcess::NormalExit));
    const QStringList output = result.standardOutput.split(QLatin1Char('\n'), Qt::SkipEmptyParts);
    QVERIFY(!output.isEmpty());
    QCOMPARE(QDir(output.value(0)).canonicalPath(),
             QDir(QFileInfo(m_echoScript).absolutePath()).canonicalPath());
#endif
}

// The bytes QProcess hands back are decoded as UTF-8, so a non-ASCII reply
// arrives intact. The literals are \u escapes.
void TestHashcatRunner::executeDecodesUtf8Output()
{
#ifdef Q_OS_WIN
    QSKIP("no shell stubs on this platform");
#else
    QVERIFY2(stubsAvailable(), "the shell stub could not be written");
#endif

    const QString text =
        QStringLiteral("Gr\u00fc\u00dfe \u2013 \u00e4\u00f6\u00fc\u00df \u65e5\u672c\u8a9e");

    // The stub prints the UTF-8 bytes of the text, exactly as hashcat would.
    const QByteArray script =
        QStringLiteral("#!/bin/sh\nprintf '%s\\n' \"%1\"\n").arg(text).toUtf8();
    const QString stub =
        TestEnvironment::writeShellStub(QStringLiteral("stub-hashcat-utf8"), script);
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");

    const HashcatResult result =
        HashcatRunner::execute(stub, {QStringLiteral("--version")}).result();

    QCOMPARE(int(result.exitStatus), int(QProcess::NormalExit));
    QCOMPARE(result.exitCode, 0);
    QCOMPARE(result.standardOutput.trimmed(), text);
}

int main(int argc, char *argv[])
{
    TestHashcatRunner test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_hashcatrunner.moc"