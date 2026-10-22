/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QGroupBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>

#include "appconstants.h"
#include "helperutils.h"
#include "settingsdialog.h"
#include "settingsmanager.h"
#include "statefile.h"
#include "ui_settingsdialog.h"

#include <iterator>

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
    void restoreSessionFollowsTheSetting();
    void clearSessionDeletesTheStoredSession();
    void clearSessionKeepsTheFileWhenDeclined();
    void shortenPathOnlyShortensTheHomeDirectory();
    void showsWhereTheFilesAreStored();
    void savedSettingsHoldTheKnownSettings();
    void savedSettingsCoverEveryKnownKey();
    void everyKnownKeyIsTypeChecked();
    void importSettingsFillsTheDialog();
    void importSettingsAppliesEmptyValues();
    void importSettingsNamesTheUnknownKeys();
    void importSettingsDoesNotApplyAValueOfTheWrongType();
    void colorSchemeFollowsTheSetting();
    void fitsItsContent();
    void groupBoxesShareOneTitleAlignment();

private:
    QString m_savedHashcatPath;
    QString m_savedTerminal;
    QString m_savedLanguage;
    QString m_savedColorScheme;
    bool m_savedShortParameters = false;
    bool m_savedRestoreSession = true;
};

// Every test starts from the same settings, and cleanup() puts back whatever
// it found, so the order the slots run in stops mattering.
void TestSettingsDialog::init()
{
    auto &settings = SettingsManager::instance();
    m_savedHashcatPath = settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    m_savedTerminal = settings.getKey<QString>(AppConstants::SettingsKeys::Terminal);
    m_savedLanguage = settings.getKey<QString>(AppConstants::SettingsKeys::Language);
    m_savedColorScheme = settings.getKey<QString>(AppConstants::SettingsKeys::ColorScheme);
    m_savedShortParameters = settings.getKey<bool>(AppConstants::SettingsKeys::UseShortParameters);
    m_savedRestoreSession = settings.getKey<bool>(AppConstants::SettingsKeys::RestoreSession, true);

    // A fresh install: nothing configured, system default language, the
    // system color scheme, the session restored. Each test then stores what
    // it needs on top of this.
    settings.setKey(AppConstants::SettingsKeys::HashcatPath, QString());
    settings.setKey(AppConstants::SettingsKeys::Terminal, QString());
    settings.setKey(AppConstants::SettingsKeys::Language,
                    QString::fromUtf8(AppConstants::Languages::Default));
    settings.setKey(AppConstants::SettingsKeys::ColorScheme, QString());
    settings.setKey(AppConstants::SettingsKeys::UseShortParameters, false);
    settings.setKey(AppConstants::SettingsKeys::RestoreSession, true);
}

void TestSettingsDialog::cleanup()
{
    auto &settings = SettingsManager::instance();
    settings.setKey(AppConstants::SettingsKeys::HashcatPath, m_savedHashcatPath);
    settings.setKey(AppConstants::SettingsKeys::Terminal, m_savedTerminal);
    settings.setKey(AppConstants::SettingsKeys::Language, m_savedLanguage);
    settings.setKey(AppConstants::SettingsKeys::UseShortParameters, m_savedShortParameters);
    settings.setKey(AppConstants::SettingsKeys::RestoreSession, m_savedRestoreSession);
    settings.setKey(AppConstants::SettingsKeys::ColorScheme, m_savedColorScheme);
    // The scheme is a property of the whole application, so it returns to
    // what the user had as well.
    HelperUtils::applyColorScheme(m_savedColorScheme);
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
    // change is what the recorder catches instead of blocking on. Selecting
    // by code keeps this independent of the order the languages are listed in.
    const int deIndex = dialog.ui->comboBox_language->findData(QStringLiteral("de"));
    QVERIFY(deIndex >= 0);
    dialog.ui->comboBox_language->setCurrentIndex(deIndex);
    QCOMPARE(dialog.ui->comboBox_language->currentData().toString(), QStringLiteral("de"));

    TestEnvironment::ModalRecorder recorder;
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
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::Language, QStringLiteral("xx"));

    SettingsDialog dialog;
    QCOMPARE(dialog.ui->comboBox_language->currentIndex(), 0);
    QCOMPARE(dialog.ui->comboBox_language->currentData().toString(), QString());

    QCOMPARE(SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::Language),
             QStringLiteral("xx"));
}

// The regression the stale-code fallback used to set off: with a code no
// longer offered stored and the combo showing "System default", saving after
// changing only the hashcat path is no language change at all - no restart
// dialog, no rewrite of the key to something the user never picked.
void TestSettingsDialog::staleLanguageCodeDoesNotDemandARestart()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::Language, QStringLiteral("xx"));

    SettingsDialog dialog;
    dialog.ui->lineEdit_hc_path->setText(QStringLiteral("/opt/somewhere/hashcat"));

    TestEnvironment::ModalRecorder recorder;
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

    TestEnvironment::ModalRecorder recorder;
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

    TestEnvironment::ModalRecorder recorder;
    recorder.pickFile = plain;
    recorder.start();
    QVERIFY(QMetaObject::invokeMethod(&dialog, "selectPathClicked"));

    QVERIFY(recorder.messageBoxSeen);
    QCOMPARE(recorder.title, QStringLiteral("Invalid file"));
    QCOMPARE(dialog.ui->lineEdit_hc_path->text(), QStringLiteral("/preset"));
#endif
}

// Whether the last session is restored at the next start is a setting, so
// the checkbox follows it and Save stores what it shows.
void TestSettingsDialog::restoreSessionFollowsTheSetting()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::RestoreSession, false);

    SettingsDialog dialog;
    QCOMPARE(dialog.ui->checkBox_restore_session->isChecked(), false);

    dialog.ui->checkBox_restore_session->setChecked(true);

    TestEnvironment::ModalRecorder recorder;
    recorder.start();
    QVERIFY(QMetaObject::invokeMethod(&dialog, "saveClicked"));

    QCOMPARE(SettingsManager::instance().getKey<bool>(AppConstants::SettingsKeys::RestoreSession),
             true);
}

// The button that clears the saved session asks first, then removes the file
// the application stores its state in, and says that it did.
void TestSettingsDialog::clearSessionDeletesTheStoredSession()
{
    const QString path = StateFile::sessionFilePath();
    QDir().mkpath(QFileInfo(path).absolutePath());

    QFile file(path);
    QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Truncate));
    file.write("{}");
    file.close();
    QVERIFY(QFile::exists(path));

    SettingsDialog dialog;

    TestEnvironment::ModalRecorder recorder;
    recorder.acceptConfirmation = true;
    recorder.start();
    QVERIFY(QMetaObject::invokeMethod(&dialog, "clearSessionClicked"));

    QVERIFY2(!QFile::exists(path), "the saved session was not deleted");
    QVERIFY(recorder.messageBoxSeen);
    QCOMPARE(recorder.title, QStringLiteral("Session cleared"));
}

// The question is what stands between the button and the file: No leaves the
// session where it is.
void TestSettingsDialog::clearSessionKeepsTheFileWhenDeclined()
{
    const QString path = StateFile::sessionFilePath();
    QDir().mkpath(QFileInfo(path).absolutePath());

    QFile file(path);
    QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Truncate));
    file.write("{}");
    file.close();

    SettingsDialog dialog;

    // The recorder closes the box, which answers with No.
    TestEnvironment::ModalRecorder recorder;
    recorder.start();
    QVERIFY(QMetaObject::invokeMethod(&dialog, "clearSessionClicked"));

    QCOMPARE(recorder.title, QStringLiteral("Clear session"));
    QVERIFY(QFile::exists(path));
    QFile::remove(path);
}

// Only a path inside the home directory gets the tilde: a sibling folder
// whose name begins with the same letters is not in it.
void TestSettingsDialog::shortenPathOnlyShortensTheHomeDirectory()
{
    const QString home = QDir::homePath();

    QCOMPARE(SettingsDialog::shortenPath(home), QStringLiteral("~"));
    QCOMPARE(SettingsDialog::shortenPath(home + QStringLiteral("/.config/hashcat-gui")),
             QStringLiteral("~/.config/hashcat-gui"));
    QCOMPARE(SettingsDialog::shortenPath(home + QStringLiteral("Other/x")),
             home + QStringLiteral("Other/x"));
}

// The dialog says where it keeps the settings and the session, so the user
// does not have to look them up in the documentation.
void TestSettingsDialog::showsWhereTheFilesAreStored()
{
    SettingsDialog dialog;

    const QString settingsFile = SettingsManager::instance().fileName();
    QVERIFY(!settingsFile.isEmpty());
    QVERIFY(dialog.ui->lineEdit_settings_path->text().contains(QFileInfo(settingsFile).fileName()));

    const QString sessionFile = StateFile::sessionFilePath();
    QVERIFY(dialog.ui->lineEdit_session_path->text().contains(QFileInfo(sessionFile).fileName()));

    // Where the settings live in a file, there is a folder to open. The
    // registry on Windows has none.
    const bool inFile = QFileInfo(settingsFile).isAbsolute();
    QCOMPARE(dialog.ui->pushButton_open_folder->isEnabled(), inFile);
}

// The export holds the settings the application runs with, so that they can
// be moved to another machine.
void TestSettingsDialog::savedSettingsHoldTheKnownSettings()
{
    auto &settings = SettingsManager::instance();
    settings.setKey(AppConstants::SettingsKeys::HashcatPath,
                    QStringLiteral("/opt/somewhere/hashcat"));
    settings.setKey(AppConstants::SettingsKeys::Terminal, QStringLiteral("kitty"));

    const QJsonObject stored = SettingsDialog::savedSettings();

    QCOMPARE(stored.value(QStringLiteral("hashcatPath")).toString(),
             QStringLiteral("/opt/somewhere/hashcat"));
    QCOMPARE(stored.value(QStringLiteral("terminal")).toString(), QStringLiteral("kitty"));
    QVERIFY(stored.contains(QStringLiteral("useShortParameters")));
    QVERIFY(stored.contains(QStringLiteral("restoreSession")));
    QVERIFY(stored.contains(QStringLiteral("language")));
    QVERIFY(stored.contains(QStringLiteral("colorScheme")));
}

// Every key of the table reaches the export. Adding a setting means adding an
// entry there; the export has to follow it, not a second list.
void TestSettingsDialog::savedSettingsCoverEveryKnownKey()
{
    QStringList exportedKeys = SettingsDialog::savedSettings().keys();
    exportedKeys.sort();

    QStringList knownKeys;
    for (const AppConstants::SettingsKeys::Entry &entry : AppConstants::SettingsKeys::All) {
        knownKeys << QString(entry.key);
    }
    knownKeys.sort();

    QCOMPARE(exportedKeys, knownKeys);
}

// Every key of the table is checked against its type before it is applied: a
// file that gives all of them a value of the wrong type names all of them.
void TestSettingsDialog::everyKnownKeyIsTypeChecked()
{
    QJsonObject stored;
    for (const AppConstants::SettingsKeys::Entry &entry : AppConstants::SettingsKeys::All) {
        const QJsonValue wrong = entry.type == AppConstants::SettingsKeys::Type::Boolean
                                     ? QJsonValue(QStringLiteral("true"))
                                     : QJsonValue(true);
        stored.insert(QString(entry.key), wrong);
    }

    SettingsDialog dialog;
    const QStringList notes = dialog.applyImportedSettings(stored);

    QCOMPARE(notes.size(), 1);
    for (const AppConstants::SettingsKeys::Entry &entry : AppConstants::SettingsKeys::All) {
        QVERIFY2(notes.first().contains(QString(entry.key)), qPrintable(notes.first()));
    }
}

// The import fills the dialog with the settings of a file. They are not
// applied before the user saves them.
void TestSettingsDialog::importSettingsFillsTheDialog()
{
    QJsonObject stored;
    stored.insert(QStringLiteral("hashcatPath"), QStringLiteral("/imported/hashcat"));
    stored.insert(QStringLiteral("useShortParameters"), true);
    stored.insert(QStringLiteral("restoreSession"), false);
    stored.insert(QStringLiteral("language"), QStringLiteral("de"));

    SettingsDialog dialog;
    dialog.applyImportedSettings(stored);

    QCOMPARE(dialog.ui->lineEdit_hc_path->text(), QStringLiteral("/imported/hashcat"));
    QCOMPARE(dialog.ui->checkBox_use_short_parameters->isChecked(), true);
    QCOMPARE(dialog.ui->checkBox_restore_session->isChecked(), false);
    QCOMPARE(dialog.ui->comboBox_language->currentData().toString(), QStringLiteral("de"));
    // The imported values are not applied before the user saves them.
    QCOMPARE(SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath),
             QString());
}

// An empty value is a value too: the system default for the language and the
// color scheme, and "no terminal" for the terminal. A file that says so has to
// be taken at its word.
void TestSettingsDialog::importSettingsAppliesEmptyValues()
{
    QJsonObject stored;
    stored.insert(QStringLiteral("hashcatPath"), QString());
    stored.insert(QStringLiteral("terminal"), QString());
    stored.insert(QStringLiteral("language"), QString());
    stored.insert(QStringLiteral("colorScheme"), QString());

    SettingsDialog dialog;
    // The dialog shows something else, so that the import has to change it.
    dialog.ui->lineEdit_hc_path->setText(QStringLiteral("/before/hashcat"));
    if (dialog.ui->comboBox_terminal->count() > 0) {
        dialog.ui->comboBox_terminal->setCurrentIndex(0);
    }
    dialog.ui->comboBox_language->setCurrentIndex(
        dialog.ui->comboBox_language->findData(QStringLiteral("de")));
    dialog.ui->comboBox_color_scheme->setCurrentIndex(
        dialog.ui->comboBox_color_scheme->findData(QStringLiteral("dark")));

    dialog.applyImportedSettings(stored);

    // The empty values took the place of the ones that were there. The
    // terminal combo has no empty entry; no selection is how it shows "none".
    QCOMPARE(dialog.ui->lineEdit_hc_path->text(), QString());
    QCOMPARE(dialog.ui->comboBox_terminal->currentIndex(), -1);
    QCOMPARE(dialog.ui->comboBox_language->currentData().toString(), QString());
    QCOMPARE(dialog.ui->comboBox_color_scheme->currentData().toString(), QString());
}

// The color scheme is stored as the value of the combo entry, and the
// dialog follows the setting.
void TestSettingsDialog::colorSchemeFollowsTheSetting()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::ColorScheme,
                                       QStringLiteral("dark"));

    SettingsDialog dialog;
    QCOMPARE(dialog.ui->comboBox_color_scheme->currentData().toString(), QStringLiteral("dark"));

    const int lightIndex = dialog.ui->comboBox_color_scheme->findData(QStringLiteral("light"));
    QVERIFY(lightIndex >= 0);
    dialog.ui->comboBox_color_scheme->setCurrentIndex(lightIndex);

    TestEnvironment::ModalRecorder recorder;
    recorder.start();
    QVERIFY(QMetaObject::invokeMethod(&dialog, "saveClicked"));

    QCOMPARE(SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::ColorScheme),
             QStringLiteral("light"));

    // The dialog says that not every platform offers both appearances.
    QLabel *note = dialog.findChild<QLabel *>(QStringLiteral("label_color_scheme_note"));
    QVERIFY(note);
    QVERIFY(!note->text().isEmpty());
}

// The dialog opens as large as its content: the size in the .ui would clip
// the content under a large font or a long translation and leave an empty
// margin under a small one.
void TestSettingsDialog::fitsItsContent()
{
    SettingsDialog dialog;
    dialog.show();
    QCoreApplication::processEvents();

    QCOMPARE(dialog.size(), dialog.sizeHint());

    // The user may make the dialog whatever size they like afterwards.
    QVERIFY(dialog.minimumSize() != dialog.maximumSize());
}

// The titles of the group boxes have to look alike. The alignment is the only
// thing that decides it, and a group box without it would draw its title
// above the frame while its neighbors break the border with it.
void TestSettingsDialog::groupBoxesShareOneTitleAlignment()
{
    SettingsDialog dialog;

    TestEnvironment::checkGroupBoxTitleAlignment(&dialog);
}

// A key this version does not know - the file was made by another version, or
// it was edited by hand - is named in the notes, and the keys that are known
// are applied all the same.
void TestSettingsDialog::importSettingsNamesTheUnknownKeys()
{
    QJsonObject stored;
    stored.insert(QStringLiteral("hashcatPath"), QStringLiteral("/imported/hashcat"));
    stored.insert(QStringLiteral("aKeyFromTheFuture"), QStringLiteral("something"));
    stored.insert(QStringLiteral("anotherOne"), QStringLiteral("something else"));

    SettingsDialog dialog;
    const QStringList notes = dialog.applyImportedSettings(stored);

    // The known key was applied...
    QCOMPARE(dialog.ui->lineEdit_hc_path->text(), QStringLiteral("/imported/hashcat"));
    // ... and the unknown ones are named in one sentence.
    QCOMPARE(notes.size(), 1);
    QVERIFY(notes.first().contains(QStringLiteral("aKeyFromTheFuture")));
    QVERIFY(notes.first().contains(QStringLiteral("anotherOne")));
}

// A value of the wrong type cannot be applied: converting it would change a
// setting the user did not ask for. The setting keeps what it has, and the
// value is reported.
void TestSettingsDialog::importSettingsDoesNotApplyAValueOfTheWrongType()
{
    QJsonObject stored;
    // A check box wants a boolean, a text field and a combo a string.
    stored.insert(QStringLiteral("useShortParameters"), QStringLiteral("invalidString"));
    stored.insert(QStringLiteral("hashcatPath"), 42);
    stored.insert(QStringLiteral("terminal"), 42);

    SettingsDialog dialog;
    dialog.ui->checkBox_use_short_parameters->setChecked(true);
    dialog.ui->lineEdit_hc_path->setText(QStringLiteral("/before/hashcat"));

    const QStringList notes = dialog.applyImportedSettings(stored);

    // The settings keep the values they had.
    QCOMPARE(dialog.ui->checkBox_use_short_parameters->isChecked(), true);
    QCOMPARE(dialog.ui->lineEdit_hc_path->text(), QStringLiteral("/before/hashcat"));

    // ... and the values that were not applied are named.
    QCOMPARE(notes.size(), 1);
    QVERIFY(notes.first().contains(QStringLiteral("useShortParameters")));
    QVERIFY(notes.first().contains(QStringLiteral("hashcatPath")));
    QVERIFY(notes.first().contains(QStringLiteral("terminal")));
}

int main(int argc, char *argv[])
{
    TestSettingsDialog test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_settingsdialog.moc"
