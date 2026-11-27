/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef TESTENVIRONMENT_H
#define TESTENVIRONMENT_H

#include "appconstants.h"
#include "examples.h"
#include "hashcatoptions.h"
#include "launchvalidator.h"
#include "settingsmanager.h"

#include <QAbstractButton>
#include <QApplication>
#include <QByteArray>
#include <QCoreApplication>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QStringList>
#include <QTemporaryDir>
#include <QTimer>
#include <QtTest/QTest>

// Scratch state for a test process. QStandardPaths test mode puts every
// writable location below the test root; QSettings is pinned there too
// (IniFormat bypasses QStandardPaths on Windows), and the suite name separates
// parallel ctest suites. Freshness is per test: the fixture removes the
// session and examples, and resetSettings() clears the ini.
namespace TestEnvironment
{

// The data paths a session names, with the <examples>/ token still in place.
inline void collectExamplePaths(const QJsonValue &value, QStringList *paths)
{
    if (value.isString()) {
        const QString text = value.toString();
        if (text.contains(QString(Examples::DataPrefix))) {
            paths->append(text);
        }
        return;
    }
    if (value.isObject()) {
        const QJsonObject object = value.toObject();
        for (auto it = object.constBegin(); it != object.constEnd(); ++it) {
            collectExamplePaths(it.value(), paths);
        }
        return;
    }
    if (value.isArray()) {
        const QJsonArray array = value.toArray();
        for (const auto &entry : array) {
            collectExamplePaths(entry, paths);
        }
    }
}

// A launch context whose non-option fields pass, so option tests can fail on
// the options alone. validate() checks options first; that is the precedence
// these tests pin.
inline LaunchValidator::LaunchProblem validateOptions(const HashcatOptions &options)
{
    LaunchValidator::LaunchContext context;
    context.options = options;
    context.options.hashFile = QStringLiteral("hash.txt");
    context.hashcatPath = QStringLiteral("hashcat");
    context.resolvedHashcatPath = QStringLiteral("/usr/bin/hashcat");
    context.configuredTerminal = QStringLiteral("konsole");
    context.availableTerminals = QStringList{QStringLiteral("konsole")};
    return LaunchValidator::validate(context);
}

// A run-scoped scratch directory for files tests need at a stable path:
// import/export JSON, the binary an import dialog is offered, the shell
// stubs and PATH-lookup executables (step 6). Unique per process and removed
// when the process exits, so parallel or consecutive runs never share one.
inline QTemporaryDir &scratchDir()
{
    static QTemporaryDir dir;
    if (!dir.isValid()) {
        qFatal("TestEnvironment: could not create the scratch directory");
    }
    return dir;
}

// A fresh start for one test: clear the ini instead of writing a list of
// default values. QSettings instances with the same organization/application
// share one QConfFile, so this also resets a SettingsManager constructed
// earlier; absent keys then fall back to SettingsKeys::defaultOf in production.
// Has to run after configureSuite(), so the cleared file is the pinned one.
inline void resetSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       QCoreApplication::applicationName(), QStringLiteral("settings"));
    settings.clear();
    settings.sync();
    // A save that failed leaves values pending for a retry; tests start clean.
    SettingsManager::instance().discardPending();
}

// Writes a file the tests can inspect, replacing its content.
inline bool writeFile(const QString &path,
                      const QByteArray &content = QByteArrayLiteral("password\n"))
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }
    return file.write(content) == content.size();
}

// Appends to a file the tests can inspect.
inline bool appendFile(const QString &path, const QByteArray &content)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        return false;
    }
    return file.write(content) == content.size();
}

// An unresolvable hashcat path. resolve() fails synchronously, so MainWindow
// starts with Auto-detect alone, starts no child and shows no error box.
inline void useMissingHashcatPath()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("hashcat-gui-missing/hashcat"));
}

// A hashcat stand-in the tests can execute. Returns an empty path where a #!
// script cannot run (Windows); callers needing a child QSKIP there, and treat an
// empty path as a hard failure elsewhere. An empty directory uses shared scratch.
inline QString writeShellStub([[maybe_unused]] const QString &name,
                              [[maybe_unused]] const QByteArray &content,
                              [[maybe_unused]] const QString &directory = QString())
{
#ifdef Q_OS_WIN
    // Windows only starts real images: CreateProcess cannot run a #! script,
    // so a stub written here would never answer and the query built on it
    // would fail. Skip the stub tests instead of reporting that failure.
    return {};
#else
    QString path = QDir(directory.isEmpty() ? scratchDir().path() : directory).filePath(name);
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return {};
    }
    file.write(content);
    file.close();
    QFile::setPermissions(path, QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);
    return path;
#endif
}

// A hashcat stand-in that answers --example-hashes with the given JSON map.
// The default has one entry; tests pass their own when a selection needs more.
inline QString
writeHashcatStub([[maybe_unused]] const QString &name,
                 const QByteArray &json = QByteArrayLiteral("{\"0\":{\"name\":\"MD5\"}}"))
{
    QByteArray script = QByteArrayLiteral("#!/bin/sh\nprintf '");
    script += json;
    script += QByteArrayLiteral("\\n'\nexit 0\n");
    return writeShellStub(name, script);
}

// A file named `name` that the platform's executable search accepts, for the
// tests of the PATH lookup. Nothing runs it: on Windows a shell script is not
// an image, so the running test binary is copied instead. An empty directory
// puts it in a scratch directory shared by every caller; a caller that needs
// a clean PATH passes its own.
inline QString writeSearchableExecutable(const QString &name, const QString &directory = QString())
{
#ifdef Q_OS_WIN
    QString path = QDir(directory.isEmpty() ? scratchDir().path() : directory).filePath(name);
    // Several tests ask for the same name; the first copy is good enough.
    if (QFile::exists(path)) {
        return path;
    }
    if (!QFile::copy(QCoreApplication::applicationFilePath(), path)) {
        return {};
    }
    return path;
#else
    return writeShellStub(name, "#!/bin/sh\nexit 0\n", directory);
#endif
}

// Sets PATH for the lifetime of the guard and restores it afterwards, so a
// failed check cannot leak the lookup configuration into the next test.
class PathGuard
{
public:
    explicit PathGuard(const QByteArray &path) : m_previous(qgetenv("PATH"))
    {
        qputenv("PATH", path);
    }

    ~PathGuard()
    {
        qputenv("PATH", m_previous);
    }

    PathGuard(const PathGuard &) = delete;
    PathGuard &operator=(const PathGuard &) = delete;
    PathGuard(PathGuard &&) = delete;
    PathGuard &operator=(PathGuard &&) = delete;

private:
    QByteArray m_previous;
};

// Drives the dialogs a file dialog opens, and records - while closing - any
// message box. A box that should not have appeared must neither hang the
// test nor pass unnoticed: the flag is the evidence.
class ModalRecorder
{
public:
    // Non-empty: the path a file dialog is asked to pick.
    QString pickFile;

    // Saves the settings dialog an import opens instead of closing it, so a
    // test can cover the path that ends in Save.
    bool saveSettingsDialog = false;

    // Clicks Yes on a message box that has one. A box without it - the
    // information that follows a confirmation, say - is still closed.
    bool acceptConfirmation = false;

    bool messageBoxSeen = false;
    QString title;
    QString text;
    Qt::TextFormat textFormat = Qt::AutoText;

    void start()
    {
        waitingForPick = 0;
        timer.setInterval(50);
        QObject::connect(&timer, &QTimer::timeout, &timer, [this] {
            QWidget *modal = QApplication::activeModalWidget();
            if (!modal) {
                return;
            }
            if (auto *box = qobject_cast<QMessageBox *>(modal)) {
                messageBoxSeen = true;
                title = box->windowTitle();
                text = box->text();
                textFormat = box->textFormat();
                if (acceptConfirmation) {
                    if (QAbstractButton *yes = box->button(QMessageBox::Yes)) {
                        yes->click();
                        return;
                    }
                }
                box->close();
            } else if (auto *fileDialog = qobject_cast<QFileDialog *>(modal)) {
                if (pickFile.isEmpty()) {
                    fileDialog->close();
                    return;
                }
                // selectFile() leaves the file name alone while its edit has
                // focus - and the dialog opens with exactly that focus - so
                // let go of it first or the pick never takes.
                if (QWidget *focused = fileDialog->focusWidget()) {
                    focused->clearFocus();
                }
                if (!fileDialog->selectedFiles().contains(pickFile)) {
                    // A native dialog applies the pick asynchronously and may
                    // still offer an earlier selection; keep asking until it
                    // holds our file, but never accept one we did not choose.
                    fileDialog->selectFile(pickFile);
                    if (++waitingForPick > 50) {
                        // ~2.5s without our file: close so the test fails on
                        // its assertions instead of hanging until ctest kills it.
                        fileDialog->close();
                    }
                    return;
                }
                // accept() is protected on QFileDialog; QDialog calls the same
                // virtual implementation.
                static_cast<QDialog *>(fileDialog)->accept();
            } else {
                // Any other dialog - the settings dialog an import opens - is
                // saved when the test asks for it, and closed otherwise so
                // that it cannot block the test. The slot is private, but the
                // meta object exposes it either way.
                if (saveSettingsDialog
                    && modal->metaObject()->indexOfMethod("saveClicked()") >= 0) {
                    QMetaObject::invokeMethod(modal, "saveClicked");
                    return;
                }
                modal->close();
            }
        });
        timer.start();
    }

private:
    QTimer timer;
    int waitingForPick = 0;
};

// The title alignment every group box has to use.
//
// Qt 6 dropped QGroupBox::titlePosition, so the vertical alignment bit is the
// only lever: with it the title breaks the frame's top border, without it the
// Fusion style pushes the frame below the title.
inline Qt::Alignment groupBoxTitleAlignment()
{
    return Qt::AlignLeft | Qt::AlignVCenter;
}

// Checks that every group box below root places its title that way. The
// names of the offenders are part of the message, so a group box that lost
// the property can be found by name.
inline void checkGroupBoxTitleAlignment(QObject *root)
{
    const QList<QGroupBox *> boxes = root->findChildren<QGroupBox *>();
    QVERIFY2(!boxes.isEmpty(), "the window has no group box at all");

    QStringList offenders;
    for (QGroupBox *box : boxes) {
        if (box->alignment() != groupBoxTitleAlignment()) {
            offenders << box->objectName();
        }
    }
    QVERIFY2(offenders.isEmpty(),
             qPrintable(QStringLiteral("group boxes with another title alignment: %1")
                            .arg(offenders.join(QStringLiteral(", ")))));
}

// Runs right after the constructor, before the first QSettings or
// QStandardPaths use. The application name is the suite's directory and the
// QSettings organization, which separates parallel ctest suites, so it needs
// the instance. The organization name stays unset (see main.cpp). QSettings
// needs GenericConfigLocation pinned to reach the same root on Windows.
inline void configureSuite()
{
    QCoreApplication::setApplicationName(
        QStringLiteral("hashcat-gui-tests-%1")
            .arg(QFileInfo(QCoreApplication::applicationFilePath()).baseName()));

    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope,
                       QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation));
}

// Replaces QTEST_MAIN / QTEST_GUILESS_MAIN: same behavior, plus the test-mode
// switch and the suite configuration below. needsGui decides between
// QApplication and QCoreApplication - the command builder and helper utils
// tests never need a display.
inline int run(QObject *test, int argc, char **argv, bool needsGui)
{
    // Before the application object and before any QSettings: QSettings
    // remembers the location it resolves on first construction, and the
    // application constructor may resolve paths of its own.
    QStandardPaths::setTestModeEnabled(true);

    int result = 0;
    if (needsGui) {
        QApplication app(argc, argv);
        configureSuite();
        result = QTest::qExec(test, argc, argv);
    } else {
        QCoreApplication app(argc, argv);
        configureSuite();
        result = QTest::qExec(test, argc, argv);
    }

    return result;
}

} // namespace TestEnvironment

#endif // TESTENVIRONMENT_H
