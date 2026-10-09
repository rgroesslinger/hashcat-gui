/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QDir>
#include <QMessageBox>
#include <QTimer>

#include "appconstants.h"
#include "settingsdialog.h"
#include "settingsmanager.h"
#include "ui_settingsdialog.h"

#include <iterator>

// Drives the dialogs the settings dialog opens by itself, and records -
// while closing - any message box. A box that should not have appeared must
// neither hang the test nor pass unnoticed: the flag is the evidence.
class ModalRecorder
{
public:
    // Non-empty: the path a file dialog is asked to pick.
    QString pickFile;

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
                    // A native file dialog applies the pick asynchronously and
                    // may still be offering what an earlier dialog in this
                    // process selected; keep asking until it holds our file,
                    // but never accept on a file we did not choose.
                    fileDialog->selectFile(pickFile);
                    if (++waitingForPick > 50) {
                        // ~2.5s without our file: close so the test fails on
                        // its own assertions instead of hanging until the
                        // ctest timeout kills it.
                        fileDialog->close();
                    }
                    return;
                }
                // accept() is protected on QFileDialog; going through QDialog
                // calls the very same virtual implementation
                static_cast<QDialog *>(fileDialog)->accept();
            }
        });
        timer.start();
    }

private:
    QTimer timer;
    int waitingForPick = 0;
};

// The language setting and the executable check: does the combo carry locale
// codes all the way into the settings, does a stale code stay quiet until
// the user actually picks a language, and does selectPathClicked() turn away
// a file that cannot be executed?
class TestSettingsDialog : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void languageComboStoresLocaleCodes();
    void unknownLanguageCodeFallsBackToSystemDefault();
    void staleLanguageCodeDoesNotDemandARestart();
    void selectPathClickedAcceptsAnExecutable();
    void selectPathClickedRejectsAPlainFile();

private:
    QString m_savedHashcatPath;
    QString m_savedTerminal;
    QString m_savedLanguage;
    bool m_savedShortParameters = false;
};

// Every test starts from the same settings, and cleanup() puts back whatever
// it found, so the order the slots run in stops mattering.
void TestSettingsDialog::init()
{
    auto &settings = SettingsManager::instance();
    m_savedHashcatPath = settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    m_savedTerminal = settings.getKey<QString>(AppConstants::SettingsKeys::Terminal);
    m_savedLanguage = settings.getKey<QString>(AppConstants::SettingsKeys::Language);
    m_savedShortParameters = settings.getKey<bool>(AppConstants::SettingsKeys::UseShortParameters);

    // A fresh install: nothing configured, system default language. Each
    // test then stores what it needs on top of this.
    settings.setKey(AppConstants::SettingsKeys::HashcatPath, QString());
    settings.setKey(AppConstants::SettingsKeys::Terminal, QString());
    settings.setKey(AppConstants::SettingsKeys::Language,
                    QString::fromUtf8(AppConstants::Languages::Default));
    settings.setKey(AppConstants::SettingsKeys::UseShortParameters, false);
}

void TestSettingsDialog::cleanup()
{
    auto &settings = SettingsManager::instance();
    settings.setKey(AppConstants::SettingsKeys::HashcatPath, m_savedHashcatPath);
    settings.setKey(AppConstants::SettingsKeys::Terminal, m_savedTerminal);
    settings.setKey(AppConstants::SettingsKeys::Language, m_savedLanguage);
    settings.setKey(AppConstants::SettingsKeys::UseShortParameters, m_savedShortParameters);
}

// System default plus one row per catalog, each shown in its own language
// and carrying the locale code as item data - and the code is what reaches
// the settings, never the display text.
void TestSettingsDialog::languageComboStoresLocaleCodes()
{
    SettingsDialog dialog;

    const int languages = int(std::size(AppConstants::Languages::Available));
    QCOMPARE(dialog.ui->comboBox_language->count(), languages + 1);
    QCOMPARE(dialog.ui->comboBox_language->itemData(0).toString(), QString());

    for (int i = 0; i < languages; ++i) {
        const auto &language = AppConstants::Languages::Available[i];
        QCOMPARE(dialog.ui->comboBox_language->itemData(i + 1).toString(),
                 QString::fromUtf8(language.code));
        // shown in the language's own name, not an English word for it
        QCOMPARE(dialog.ui->comboBox_language->itemText(i + 1), QString::fromUtf8(language.name));
    }

    // Move to "Deutsch" and save: the stored value is the code, not the
    // name and not the display text. The restart dialog for this real
    // change is what the recorder catches instead of blocking on.
    dialog.ui->comboBox_language->setCurrentIndex(2);
    QCOMPARE(dialog.ui->comboBox_language->currentData().toString(), QStringLiteral("de"));

    ModalRecorder recorder;
    recorder.start();
    QVERIFY(QMetaObject::invokeMethod(&dialog, "saveClicked"));

    QCOMPARE(SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::Language),
             QStringLiteral("de"));

    QVERIFY(recorder.messageBoxSeen);
    QCOMPARE(recorder.title, QStringLiteral("Restart required"));
}

// A code we no longer offer (a hand-edited file, a removed catalog): the
// combo falls back to what a fresh install would show, and opening the
// dialog alone must not rewrite the stored value behind the user's back.
void TestSettingsDialog::unknownLanguageCodeFallsBackToSystemDefault()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::Language, QStringLiteral("fr"));

    SettingsDialog dialog;
    QCOMPARE(dialog.ui->comboBox_language->currentIndex(), 0);
    QCOMPARE(dialog.ui->comboBox_language->currentData().toString(), QString());

    QCOMPARE(SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::Language),
             QStringLiteral("fr"));
}

// The regression the stale-code fallback used to set off: with "fr" stored
// and the combo showing "System default", saving after changing only the
// hashcat path is no language change at all - no restart dialog, no rewrite
// of the key to something the user never picked.
void TestSettingsDialog::staleLanguageCodeDoesNotDemandARestart()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::Language, QStringLiteral("fr"));

    SettingsDialog dialog;
    dialog.ui->lineEdit_hc_path->setText(QStringLiteral("/opt/somewhere/hashcat"));

    ModalRecorder recorder;
    recorder.start();
    QVERIFY(QMetaObject::invokeMethod(&dialog, "saveClicked"));

    QVERIFY2(!recorder.messageBoxSeen,
             "a stale language code must not read as a pending language change");
    QCOMPARE(SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath),
             QStringLiteral("/opt/somewhere/hashcat"));
    // what Save stores is what the combo showed
    QCOMPARE(SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::Language),
             QString());
    QCOMPARE(dialog.result(), int(QDialog::Accepted));
}

// The happy path of selectPathClicked(): a file that exists and runs fills
// the field, with the platform's own separator style.
void TestSettingsDialog::selectPathClickedAcceptsAnExecutable()
{
#ifdef Q_OS_WIN
    QSKIP("Windows opens a real file dialog that nothing here can drive");
#else
    const QString stub =
        TestEnvironment::writeShellStub(QStringLiteral("stub-hashcat.bin"), "#!/bin/sh\nexit 0\n");
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");

    SettingsDialog dialog;
    dialog.ui->lineEdit_hc_path->setText(QStringLiteral("/preset"));

    ModalRecorder recorder;
    recorder.pickFile = stub;
    recorder.start();
    QVERIFY(QMetaObject::invokeMethod(&dialog, "selectPathClicked"));

    QCOMPARE(dialog.ui->lineEdit_hc_path->text(), QDir::toNativeSeparators(stub));
    QVERIFY2(!recorder.messageBoxSeen, "an executable was wrongly refused");
#endif
}

// The other half of the check: a file that exists but cannot be executed
// gets the warning, and the field keeps what it held.
void TestSettingsDialog::selectPathClickedRejectsAPlainFile()
{
#ifdef Q_OS_WIN
    QSKIP("Windows opens a real file dialog that nothing here can drive");
#else
    const QString plain = TestEnvironment::configHome().filePath(QStringLiteral("plain.bin"));
    QFile file(plain);
    QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Truncate));
    file.write("not a program\n");
    file.close();

    SettingsDialog dialog;
    dialog.ui->lineEdit_hc_path->setText(QStringLiteral("/preset"));

    ModalRecorder recorder;
    recorder.pickFile = plain;
    recorder.start();
    QVERIFY(QMetaObject::invokeMethod(&dialog, "selectPathClicked"));

    QVERIFY(recorder.messageBoxSeen);
    QCOMPARE(recorder.title, QStringLiteral("Invalid file"));
    QCOMPARE(dialog.ui->lineEdit_hc_path->text(), QStringLiteral("/preset"));
#endif
}

int main(int argc, char *argv[])
{
    TestSettingsDialog test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_settingsdialog.moc"
