/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef TESTENVIRONMENT_H
#define TESTENVIRONMENT_H

#include <QApplication>
#include <QAbstractButton>
#include <QCoreApplication>
#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QGroupBox>
#include <QMessageBox>
#include <QStringList>
#include <QTemporaryDir>
#include <QTimer>
#include <QtTest/QTest>

// Scratch base directories for a test process.
//
// QSettings and QStandardPaths resolve XDG_CONFIG_HOME / XDG_DATA_HOME on
// first use; redirecting them here means no test can read or overwrite the
// settings, session file or window geometry of whoever runs ctest.
//
// The three statics are first touched from main(), so they outlive
// SettingsManager's singleton and are destroyed last, after it.
namespace TestEnvironment
{

inline QTemporaryDir &configHome()
{
    static QTemporaryDir dir;
    return dir;
}

inline QTemporaryDir &dataHome()
{
    static QTemporaryDir dir;
    return dir;
}

inline QTemporaryDir &cacheHome()
{
    static QTemporaryDir dir;
    return dir;
}

// Has to run before the first QSettings or QStandardPaths call.
inline void redirectBaseDirectories()
{
    // A scratch directory that could not be created would send QSettings and
    // QStandardPaths back at the real ones - the exact leak this redirect
    // exists to stop. Stop the run instead of testing against somebody's own
    // configuration.
    if (!configHome().isValid() || !dataHome().isValid() || !cacheHome().isValid()) {
        qFatal("TestEnvironment: could not create the scratch base directories");
    }

    qputenv("XDG_CONFIG_HOME", configHome().path().toUtf8());
    qputenv("XDG_DATA_HOME", dataHome().path().toUtf8());
    qputenv("XDG_CACHE_HOME", cacheHome().path().toUtf8());
}

// A hashcat stand-in the tests can execute. Returns an empty path where a #!
// script cannot run (Windows) or be written; callers QSKIP on Windows and
// treat an empty path as a hard failure elsewhere.
inline QString writeShellStub([[maybe_unused]] const QString &name,
                              [[maybe_unused]] const QByteArray &content)
{
#ifdef Q_OS_WIN
    // Windows only starts real images: CreateProcess cannot run a #! script,
    // so a stub written here would never answer and the query built on it
    // would fail. Skip the stub tests instead of reporting that failure.
    return {};
#else
    static QTemporaryDir dir;
    if (!dir.isValid()) {
        return {};
    }

    QString path = dir.filePath(name);
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
// Qt 6 dropped QGroupBox::titlePosition, so the vertical bit of the title
// alignment is the only lever left: with it the title breaks the top border
// of the frame, without it the Fusion style pushes the frame below the title
// and the title ends up above the box. A group box that forgets the property
// therefore looks different from its neighbors.
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

// Replaces QTEST_MAIN / QTEST_GUILESS_MAIN: same behavior, plus the redirect
// above. needsGui decides between QApplication and QCoreApplication - the
// command builder and helper utils tests never need a display.
inline int run(QObject *test, int argc, char *argv[], bool needsGui)
{
    redirectBaseDirectories();

    int result = 0;
    if (needsGui) {
        QApplication app(argc, argv);
        // The application name is the organization QSettings writes under, so
        // this is a second, independent guard on top of the XDG redirect - and
        // on Windows, where XDG plays no part, the only one.
        app.setApplicationName(QStringLiteral("hashcat-gui-tests"));
        result = QTest::qExec(test, argc, argv);
    } else {
        QCoreApplication app(argc, argv);
        app.setApplicationName(QStringLiteral("hashcat-gui-tests"));
        result = QTest::qExec(test, argc, argv);
    }

    return result;
}

} // namespace TestEnvironment

#endif // TESTENVIRONMENT_H
