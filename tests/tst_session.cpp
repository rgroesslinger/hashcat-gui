/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appconstants.h"
#include "mainwindow.h"
#include "mainwindowtestaccess.h"
#include "mainwindowtesthelpers.h"
#include "settingsmanager.h"
#include "statefile.h"
#include "testenvironment.h"
#include "ui_mainwindow.h"
#include "widgetstateserializer.h"

#include <QAction>
#include <QClipboard>
#include <QComboBox>
#include <QCompleter>
#include <QDir>
#include <QDoubleSpinBox>
#include <QFile>
#include <QFileInfo>
#include <QGridLayout>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>
#include <QPointer>
#include <QRadioButton>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QSet>
#include <QSpinBox>
#include <QStandardPaths>
#include <QTimer>
#include <QToolTip>
#include <QWidget>
#include <array>

// The session half of the main window: persistence, import/export, version
// notes, retired names and the frozen format guards.
class TestSession : public QObject
{
    Q_OBJECT

private slots:
    void init()
    {
        m_fixture.init();
    }
    void cleanup()
    {
        m_fixture.cleanup();
    }
    void resetSessionRestoresEveryWidget();
    void resettingTheSessionNeedsAConfirmation();
    void theSessionRoundTrips();
    void turningTheSessionOffRemovesTheStoredSession();
    void sessionHashTypeSurvivesTheAsynchronousQuery();
    void sessionWithAnUnknownHashTypeSelectsNothing();
    void theSettingsAreExportedThroughTheFileMenu();
    void theSettingsAreImportedThroughTheFileMenu();
    void theImportNamesTheValuesTheSessionDoesNotKnow();
    void theImportIgnoresRetiredWidgetNames();
    void aStartupSessionWithRetiredNamesStaysQuiet();
    void retiredWidgetNamesAreNotReused();
    void aNewerSessionIsRefused();
    void anOlderSessionIsAppliedWithANote();
    void aNewerSettingsFileIsRefused();
    void anOlderStartupSessionIsAppliedWithANote();
    void aNewerStartupSessionIsLeftAlone();
    void aNewerSessionSurvivesTheExit();
    void theNewerSessionWarningSaysWhatIsLost();
    void aSessionFileThatCannotBeReadIsLeftAlone();
    void aSessionFileOfAnotherShapeIsLeftAlone();
    void theStartupShowsNoBox();
    void theSerializedKeySetIsFrozen();
    void theGoldenSessionRoundTrips();
    void applyingASessionRebuildsThePreviewOnce();
    void aNoOpApplyWithRowsRebuildsThePreviewOnce();
    void resettingTheSessionRebuildsThePreviewOnce();

private:
    MainWindowFixture m_fixture;
};

// The reset has to bring every widget that holds a value back to its
// default - and only after the user confirmed it, so the box is answered
// here.
//
// The test does not know the widgets: it changes every one of them, resets
// the form and compares the whole state with a fresh window. A widget the
// reset forgets - now or after it is added - shows up as a difference, and
// the list of widgets does not have to be kept in two places.
void TestSession::resetSessionRestoresEveryWidget()
{
    MainWindow fresh;
    MainWindow window;

    changeEveryValue(&window);

    TestEnvironment::ModalRecorder recorder;
    recorder.acceptConfirmation = true;
    recorder.start();

    QVERIFY(QMetaObject::invokeMethod(&window, "resetSessionTriggered"));
    QVERIFY(recorder.messageBoxSeen);

    QString error;
    const QString freshFile = TestEnvironment::configHome().filePath(QStringLiteral("fresh.json"));
    const QString resetFile = TestEnvironment::configHome().filePath(QStringLiteral("reset.json"));
    const QStringList ignored{AppConstants::Files::PreviewWidget};
    QVERIFY2(StateFile::write(freshFile, AppConstants::FileSections::Session,
                              AppConstants::Sessions::Version,
                              WidgetStateSerializer::toJson(&fresh, ignored), &error),
             qPrintable(error));
    QVERIFY2(StateFile::write(resetFile, AppConstants::FileSections::Session,
                              AppConstants::Sessions::Version,
                              WidgetStateSerializer::toJson(&window, ignored), &error),
             qPrintable(error));

    const QJsonObject defaults = readJsonObject(freshFile)
                                     .value(QStringLiteral("sections"))
                                     .toObject()
                                     .value(QString(AppConstants::FileSections::Session))
                                     .toObject()
                                     .value(QStringLiteral("values"))
                                     .toObject();
    const QStringList differing = differingWidgets(resetFile, defaults);
    QVERIFY2(differing.isEmpty(), qPrintable(QStringLiteral("these widgets were not reset: %1")
                                                 .arg(differing.join(QStringLiteral(", ")))));
}

// The reset is confirmed first, and a box that is closed without a Yes has
// to leave the session alone: the values the user entered are what the
// confirmation protects.
void TestSession::resettingTheSessionNeedsAConfirmation()
{
    MainWindow window;
    MainWindowTestAccess::ui(window)->lineEdit_session->setText(QStringLiteral("keepMe"));

    TestEnvironment::ModalRecorder recorder;
    recorder.start();

    QVERIFY(QMetaObject::invokeMethod(&window, "resetSessionTriggered"));

    QVERIFY(recorder.messageBoxSeen);
    QCOMPARE(recorder.title, QStringLiteral("Reset session"));
    QVERIFY(recorder.text.contains(QStringLiteral("lost")));
    // The recorder closed the box without confirming: nothing was reset.
    QCOMPARE(MainWindowTestAccess::ui(window)->lineEdit_session->text(), QStringLiteral("keepMe"));
}

// The session machinery as the application uses it: saveSession() and
// the loadSession() every constructor runs. Pinned here, and nowhere
// else: the command preview stays out of the file, and a new window starts
// from the stored state.
void TestSession::theSessionRoundTrips()
{
    // A failed save or load opens a modal warning; the watchdog must be
    // able to close it so the test fails instead of hanging until ctest's
    // timeout.
    m_fixture.startWatchdog();

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("/opt/hashcat/hashcat"));

    MainWindow writer;
    MainWindowTestAccess::ui(writer)->comboBox_attack->setCurrentIndex(
        2); // brute-force, not the default straight
    MainWindowTestAccess::ui(writer)->spinBox_runtime->setValue(64); // not the default 0
    MainWindowTestAccess::saveSession(writer);

    const QString session = MainWindowTestAccess::sessionFile(writer);
    QVERIFY(QFile::exists(session));

    // What gets stored: the values of the session, and without the command
    // preview - that one is regenerated from the state on load.
    QFile file(session);
    QVERIFY(file.open(QIODevice::ReadOnly));
    const QJsonObject root = QJsonDocument::fromJson(file.readAll()).object();
    file.close();

    const QJsonObject state = root.value(QStringLiteral("sections"))
                                  .toObject()
                                  .value(QString(AppConstants::FileSections::Session))
                                  .toObject()
                                  .value(QStringLiteral("values"))
                                  .toObject();
    QVERIFY(!state.isEmpty());
    QVERIFY(!state.contains(QLatin1String(AppConstants::Files::PreviewWidget)));
    // The editor a spin box builds for itself is not a field of the form: it
    // must not become a name of the session format.
    for (auto it = state.constBegin(); it != state.constEnd(); ++it) {
        QVERIFY2(
            !it.key().startsWith(QStringLiteral("qt_")),
            qPrintable(QStringLiteral("the session holds the internal name %1").arg(it.key())));
        QVERIFY2(!it.key().isEmpty(), "the session holds a value without a name");
    }
    // The attack mode is stored by its id, not by its position: the ids are
    // 0, 1, 3, 6, 7 and 9, and brute-force - the third entry - is id 3.
    QCOMPARE(state.value(QStringLiteral("comboBox_attack")).toInt(), 3);

    // A new window runs loadSession() in its constructor...
    MainWindow reloaded;
    QCOMPARE(MainWindowTestAccess::ui(reloaded)->comboBox_attack->currentIndex(), 2);
    QCOMPARE(MainWindowTestAccess::ui(reloaded)->spinBox_runtime->value(), 64);

    // ...and the preview reflects the restored state, not the defaults the
    // widgets held before the session arrived.
    QCOMPARE(MainWindowTestAccess::ui(reloaded)->lineEdit_command->text(),
             QStringLiteral("hashcat --attack-mode 3 --runtime 64"));
}

// The session is stored only while it is wanted: the session of an earlier
// run goes when the setting is off, so that it cannot come back as stale
// state once the setting returns.
void TestSession::turningTheSessionOffRemovesTheStoredSession()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::RestoreSession, true);

    MainWindow writer;
    MainWindowTestAccess::saveSession(writer);
    const QString session = MainWindowTestAccess::sessionFile(writer);
    QVERIFY(QFile::exists(session));

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::RestoreSession, false);
    MainWindowTestAccess::saveSession(writer);

    QVERIFY(!QFile::exists(session));
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::RestoreSession, true);
}

// The constructor loads the session while the hashcat query that fills
// comboBox_hash_type is still in flight: the combo has no items, so
// setCurrentIndex() is a silent no-op and the stored id can only be
// applied when the reply delivers the items.
void TestSession::sessionHashTypeSurvivesTheAsynchronousQuery()
{
    const QString stub = TestEnvironment::writeShellStub(
        QStringLiteral("stub-hashcat-session"),
        "#!/bin/sh\n"
        "printf '{\"0\":{\"name\":\"MD5\"},\"1000\":{\"name\":\"NTLM\"}}\\n'\n"
        "exit 0\n");
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    // A session that names the hash type 1000: the ids sort 0, 1000, so the
    // selection must land on NTLM and not on the first item the fill leaves.
    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QVERIFY(QDir().mkpath(dir));
    QFile session(QDir(dir).filePath(AppConstants::Files::SessionFile));
    QVERIFY(session.open(QIODevice::WriteOnly | QIODevice::Truncate));
    session.write(R"({"sections": {"session": )"
                  R"({"version": 1, "values": {"comboBox_hash_type": 1000}}}})");
    session.close();

    m_fixture.startWatchdog();

    MainWindow window;

    // The query runs off-thread; wait at least as long as its own deadline
    // so the test never gives up while hashcat could still have answered.
    QTRY_VERIFY_WITH_TIMEOUT(MainWindowTestAccess::ui(window)->comboBox_hash_type->count() > 0,
                             AppConstants::Hashcat::QueryTimeoutMs);

    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->currentIndex(), 1);
    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->currentData().toUInt(), 1000U);
    QCOMPARE(MainWindowTestAccess::pendingHashTypeId(window), -1); // applied and consumed
}

// A session that names a hash type this hashcat does not know. Selecting
// another one would crack with the wrong algorithm, so nothing is selected
// instead - and the command line asks for no hash type at all.
void TestSession::sessionWithAnUnknownHashTypeSelectsNothing()
{
    const QString stub = TestEnvironment::writeShellStub(
        QStringLiteral("stub-hashcat-unknown"),
        "#!/bin/sh\n"
        "printf '{\"0\":{\"name\":\"MD5\"},\"1000\":{\"name\":\"NTLM\"}}\\n'\n"
        "exit 0\n");
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QVERIFY(QDir().mkpath(dir));
    QFile session(QDir(dir).filePath(AppConstants::Files::SessionFile));
    QVERIFY(session.open(QIODevice::WriteOnly | QIODevice::Truncate));
    session.write(R"({"sections": {"session": )"
                  R"({"version": 1, "values": {"comboBox_hash_type": 9999}}}})");
    session.close();

    m_fixture.startWatchdog();

    MainWindow window;

    QTRY_VERIFY_WITH_TIMEOUT(MainWindowTestAccess::ui(window)->comboBox_hash_type->count() > 0,
                             AppConstants::Hashcat::QueryTimeoutMs);

    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->currentIndex(), -1);
    QVERIFY(!MainWindowTestAccess::generateArguments(window).contains(QStringLiteral("-m")));
    QVERIFY(
        !MainWindowTestAccess::generateArguments(window).contains(QStringLiteral("--hash-type")));
}

// The settings leave the application through the File menu, like the session:
// the file holds the settings that are in effect, in the shared format.
void TestSession::theSettingsAreExportedThroughTheFileMenu()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("/opt/somewhere/hashcat"));

    const QString file = TestEnvironment::configHome().filePath(QStringLiteral("settings.json"));

    MainWindow window;
    TestEnvironment::ModalRecorder recorder;
    recorder.pickFile = file;
    recorder.start();

    QVERIFY(QMetaObject::invokeMethod(&window, "exportSettingsTriggered"));

    QVERIFY(QFile::exists(file));

    QJsonObject values;
    int version = 0;
    QStringList otherSections;
    QVERIFY2(StateFile::read(file, AppConstants::FileSections::Settings, &values, &version,
                             &otherSections, nullptr),
             "the exported settings cannot be read again");
    QCOMPARE(values.value(QStringLiteral("hashcatPath")).toString(),
             QStringLiteral("/opt/somewhere/hashcat"));
}

// The import opens the settings dialog with the values of the file in it, so
// the user reviews them before they are applied.
void TestSession::theSettingsAreImportedThroughTheFileMenu()
{
    const QString file = TestEnvironment::configHome().filePath(QStringLiteral("import.json"));

    QJsonObject values;
    values.insert(QStringLiteral("hashcatPath"), QStringLiteral("/imported/hashcat-from-the-file"));
    values.insert(QStringLiteral("useShortParameters"), true);
    QVERIFY(StateFile::write(file, AppConstants::FileSections::Settings,
                             AppConstants::Settings::Version, values, nullptr));

    // A configured hashcat, so that the window does not queue the settings
    // dialog of its own - and one that answers, so that no error box opens
    // while the test runs.
    const QString stub = hashcatStub(QStringLiteral("stub-hashcat-settings-import"));
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    MainWindow window;
    TestEnvironment::ModalRecorder recorder;
    recorder.pickFile = file;
    recorder.saveSettingsDialog = true;
    recorder.start();

    // The dialog the import opens is saved, so the test also covers what has
    // to follow a save: the notes that came with the file, and the refresh of
    // everything that depends on the settings.
    QVERIFY(QMetaObject::invokeMethod(&window, "importSettingsTriggered"));
    QVERIFY(recorder.messageBoxSeen);
    QCOMPARE(recorder.title, QStringLiteral("Imported"));

    QCOMPARE(SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath),
             QStringLiteral("/imported/hashcat-from-the-file"));
    // The preview follows the new binary and the new parameter style without
    // the user touching a widget.
    // The preview follows the new binary and the new parameter style without
    // the user touching a widget. The hash type the stub reports is in it
    // as well.
    const QString preview = MainWindowTestAccess::ui(window)->lineEdit_command->text();
    QVERIFY2(preview.startsWith(QStringLiteral("hashcat-from-the-file")), qPrintable(preview));
    QVERIFY2(preview.contains(QStringLiteral(" -a 0")), qPrintable(preview));
}

// A session file that names a widget this version does not know - it was
// made by another version, or it was edited by hand. The values that are
// known are applied all the same, and the ones that are not are named.
void TestSession::theImportNamesTheValuesTheSessionDoesNotKnow()
{
    const QString file = TestEnvironment::configHome().filePath(QStringLiteral("session.json"));

    QJsonObject values;
    values.insert(QStringLiteral("spinBox_runtime"), 64);
    values.insert(QStringLiteral("aWidgetFromTheFuture"), QStringLiteral("something"));
    QVERIFY(StateFile::write(file, AppConstants::FileSections::Session,
                             AppConstants::Sessions::Version, values, nullptr));

    const QString stub = hashcatStub(QStringLiteral("stub-hashcat-import"));
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    MainWindow window;
    TestEnvironment::ModalRecorder recorder;
    recorder.pickFile = file;
    recorder.start();

    QVERIFY(QMetaObject::invokeMethod(&window, "importTriggered"));

    // The value that named a widget was applied...
    QCOMPARE(MainWindowTestAccess::ui(window)->spinBox_runtime->value(), 64);
    // ... and the one that named nothing is named in the notes.
    QVERIFY(recorder.messageBoxSeen);
    QVERIFY(recorder.text.contains(QStringLiteral("aWidgetFromTheFuture")));
}

// A session written before a widget was removed still names it. The retired
// name is dropped without a note - warning about it would hit every session of
// that version - while a name that was never ours is still reported.
void TestSession::theImportIgnoresRetiredWidgetNames()
{
    const QString file = TestEnvironment::configHome().filePath(QStringLiteral("retired.json"));

    QJsonObject values;
    values.insert(QStringLiteral("spinBox_runtime"), 64);
    values.insert(QStringLiteral("spinBox_segment_size"), 32);
    values.insert(QStringLiteral("comboBox_workload_profile"), 2);
    values.insert(QStringLiteral("checkBox_override_workload_profile"), true);
    values.insert(QStringLiteral("aWidgetFromTheFuture"), QStringLiteral("something"));
    QVERIFY(StateFile::write(file, AppConstants::FileSections::Session,
                             AppConstants::Sessions::Version, values, nullptr));

    const QString stub = hashcatStub(QStringLiteral("stub-hashcat-retired"));
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    MainWindow window;
    TestEnvironment::ModalRecorder recorder;
    recorder.pickFile = file;
    recorder.start();

    QVERIFY(QMetaObject::invokeMethod(&window, "importTriggered"));

    // The value that named a widget was applied...
    QCOMPARE(MainWindowTestAccess::ui(window)->spinBox_runtime->value(), 64);
    // ... the name that was never ours is still named...
    QVERIFY(recorder.messageBoxSeen);
    QVERIFY(recorder.text.contains(QStringLiteral("aWidgetFromTheFuture")));
    // ... and the retired ones are not.
    for (const QLatin1StringView &retired : AppConstants::Sessions::RetiredWidgetNames) {
        QVERIFY2(!recorder.text.contains(QString(retired)), qPrintable(recorder.text));
    }
}

// The startup stays quiet for a session that names a retired widget: the file
// was written by this application, and the value it holds cannot come back.
void TestSession::aStartupSessionWithRetiredNamesStaysQuiet()
{
    const QString session = StateFile::sessionFilePath();
    QVERIFY(QFileInfo(session).dir().mkpath(QStringLiteral(".")));

    QJsonObject values;
    values.insert(QStringLiteral("spinBox_runtime"), 64);
    values.insert(QStringLiteral("spinBox_segment_size"), 32);
    values.insert(QStringLiteral("comboBox_workload_profile"), 2);
    values.insert(QStringLiteral("checkBox_override_workload_profile"), true);
    QVERIFY(StateFile::write(session, AppConstants::FileSections::Session,
                             AppConstants::Sessions::Version, values, nullptr));

    const QString stub = hashcatStub(QStringLiteral("stub-hashcat-retired-startup"));
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    m_fixture.startWatchdog();
    MainWindow window;

    QCOMPARE(m_fixture.closedModals(), 0);
    QCOMPARE(MainWindowTestAccess::ui(window)->spinBox_runtime->value(), 64);
}

// A retired name is never reused: a widget that took one again would have its
// session value dropped silently on load. The list only holds names no widget
// has anymore.
void TestSession::retiredWidgetNamesAreNotReused()
{
    MainWindow window;
    const QJsonObject state =
        WidgetStateSerializer::toJson(&window, {AppConstants::Files::PreviewWidget});

    for (const QLatin1StringView &retired : AppConstants::Sessions::RetiredWidgetNames) {
        QVERIFY2(!state.contains(QString(retired)),
                 qPrintable(QStringLiteral("a widget still carries the retired name %1")
                                .arg(QString(retired))));
    }
}

// A file with a newer format version is not applied at all: a value whose
// name survived may mean something else there. The note names the version.
void TestSession::aNewerSessionIsRefused()
{
    const QString file = TestEnvironment::configHome().filePath(QStringLiteral("newer.json"));

    QJsonObject values;
    values.insert(QStringLiteral("spinBox_runtime"), 64);
    QVERIFY(StateFile::write(file, AppConstants::FileSections::Session,
                             AppConstants::Sessions::Version + 1, values, nullptr));

    const QString stub = hashcatStub(QStringLiteral("stub-hashcat-newer"));
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    MainWindow window;
    TestEnvironment::ModalRecorder recorder;
    recorder.pickFile = file;
    recorder.start();

    QVERIFY(QMetaObject::invokeMethod(&window, "importTriggered"));

    QVERIFY(recorder.messageBoxSeen);
    QCOMPARE(recorder.title, QStringLiteral("Import failed"));
    QVERIFY(recorder.text.contains(
        QStringLiteral("format version %1").arg(AppConstants::Sessions::Version + 1)));
    QVERIFY(MainWindowTestAccess::ui(window)->spinBox_runtime->value() != 64);
}

// A file an older version wrote is applied, and the note says why a value it
// named may be missing.
void TestSession::anOlderSessionIsAppliedWithANote()
{
    const QString file = TestEnvironment::configHome().filePath(QStringLiteral("older.json"));

    QJsonObject values;
    values.insert(QStringLiteral("spinBox_runtime"), 64);
    QVERIFY(StateFile::write(file, AppConstants::FileSections::Session,
                             AppConstants::Sessions::Version - 1, values, nullptr));

    const QString stub = hashcatStub(QStringLiteral("stub-hashcat-older-import"));
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    MainWindow window;
    TestEnvironment::ModalRecorder recorder;
    recorder.pickFile = file;
    recorder.start();

    QVERIFY(QMetaObject::invokeMethod(&window, "importTriggered"));

    QCOMPARE(MainWindowTestAccess::ui(window)->spinBox_runtime->value(), 64);
    QVERIFY(recorder.messageBoxSeen);
    QCOMPARE(recorder.title, QStringLiteral("Imported"));
    QVERIFY(recorder.text.contains(QStringLiteral("older version")));
}

// A settings file of a newer format version is refused, like a session file:
// the keys it holds may mean something else there.
void TestSession::aNewerSettingsFileIsRefused()
{
    const QString file =
        TestEnvironment::configHome().filePath(QStringLiteral("newer-settings.json"));

    QJsonObject values;
    values.insert(QStringLiteral("useShortParameters"), true);
    QVERIFY(StateFile::write(file, AppConstants::FileSections::Settings,
                             AppConstants::Settings::Version + 1, values, nullptr));

    const QString stub = hashcatStub(QStringLiteral("stub-hashcat-settings"));
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    MainWindow window;
    TestEnvironment::ModalRecorder recorder;
    recorder.pickFile = file;
    recorder.start();

    QVERIFY(QMetaObject::invokeMethod(&window, "importSettingsTriggered"));

    QVERIFY(recorder.messageBoxSeen);
    QCOMPARE(recorder.title, QStringLiteral("Import failed"));
    QVERIFY(recorder.text.contains(
        QStringLiteral("format version %1").arg(AppConstants::Settings::Version + 1)));
    // The settings of the file were not applied.
    QCOMPARE(
        SettingsManager::instance().getKey<bool>(AppConstants::SettingsKeys::UseShortParameters),
        false);
}

// A session file of an older version is applied at the startup as well: the
// values it still names are restored, and the file is rewritten with the
// current version at the exit.
void TestSession::anOlderStartupSessionIsAppliedWithANote()
{
    const QString session = StateFile::sessionFilePath();
    QVERIFY(QFileInfo(session).dir().mkpath(QStringLiteral(".")));

    QJsonObject values;
    values.insert(QStringLiteral("spinBox_runtime"), 64);
    QVERIFY(StateFile::write(session, AppConstants::FileSections::Session,
                             AppConstants::Sessions::Version - 1, values, nullptr));

    const QString stub = hashcatStub(QStringLiteral("stub-hashcat-older"));
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    TestEnvironment::ModalRecorder recorder;
    recorder.start();
    MainWindow window;

    // The value that named a widget was applied...
    QCOMPARE(MainWindowTestAccess::ui(window)->spinBox_runtime->value(), 64);
    // ... and the note says that the file was made by an older version.
    QVERIFY(recorder.messageBoxSeen);
    QCOMPARE(recorder.title, QStringLiteral("Session warning"));
    QVERIFY(recorder.text.contains(QStringLiteral("older version")));

    // The exit writes the file with the current version.
    MainWindowTestAccess::saveSession(window);

    QJsonObject written;
    int version = 0;
    QStringList otherSections;
    QVERIFY(StateFile::read(session, AppConstants::FileSections::Session, &written, &version,
                            &otherSections, nullptr));
    QCOMPARE(version, AppConstants::Sessions::Version);
}

// The session the application restores at startup follows the same rule, and
// the file of the newer format version is left in place for that version.
void TestSession::aNewerStartupSessionIsLeftAlone()
{
    const QString session = StateFile::sessionFilePath();
    QVERIFY(QFileInfo(session).dir().mkpath(QStringLiteral(".")));

    QJsonObject values;
    values.insert(QStringLiteral("spinBox_runtime"), 64);
    QVERIFY(StateFile::write(session, AppConstants::FileSections::Session,
                             AppConstants::Sessions::Version + 1, values, nullptr));

    m_fixture.startWatchdog();
    MainWindow window;

    QCOMPARE(m_fixture.closedModals(), 1);
    QVERIFY(MainWindowTestAccess::ui(window)->spinBox_runtime->value() != 64);
    QVERIFY(QFile::exists(session));
}

// The session of a newer version survives the exit: it is not written over,
// and it is not deleted - not even when the session is switched off, which
// is the case where the file is never read at the startup.
void TestSession::aNewerSessionSurvivesTheExit()
{
    const QString session = StateFile::sessionFilePath();
    QVERIFY(QFileInfo(session).dir().mkpath(QStringLiteral(".")));

    QJsonObject values;
    values.insert(QStringLiteral("spinBox_runtime"), 64);
    QVERIFY(StateFile::write(session, AppConstants::FileSections::Session,
                             AppConstants::Sessions::Version + 1, values, nullptr));
    const QJsonObject before = readJsonObject(session);

    m_fixture.startWatchdog();
    MainWindow window;
    QCOMPARE(m_fixture.closedModals(), 1);

    // The session is on: the file of the newer version is not written over.
    MainWindowTestAccess::saveSession(window);
    QCOMPARE(readJsonObject(session), before);

    // The session is off: the file is not deleted either.
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::RestoreSession, false);
    MainWindowTestAccess::saveSession(window);
    QCOMPARE(readJsonObject(session), before);
    QVERIFY(QFile::exists(session));
}

// The warning of a newer session says what the user loses: the session is not
// restored, and the session of this run is not stored either.
void TestSession::theNewerSessionWarningSaysWhatIsLost()
{
    MainWindow window;

    const QString note =
        MainWindowTestAccess::newerSessionNote(window, AppConstants::Sessions::Version + 1);
    QVERIFY(note.contains(QStringLiteral("not restored")));
    QVERIFY(note.contains(QStringLiteral("not stored")));
}

// A session file this version cannot read is not thrown away: it may belong
// to a version that can read it, or the user may want to look at it.
void TestSession::aSessionFileThatCannotBeReadIsLeftAlone()
{
    const QString session = StateFile::sessionFilePath();
    QVERIFY(QFileInfo(session).dir().mkpath(QStringLiteral(".")));

    // The format of the versions before the sections: this one finds no
    // section in it.
    QFile file(session);
    QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Truncate));
    file.write(R"({"version": 1, "MainWindow": {}})");
    file.close();
    const QJsonObject before = readJsonObject(session);

    m_fixture.startWatchdog();
    MainWindow window;

    // The startup says why the session did not come back...
    QCOMPARE(m_fixture.closedModals(), 1);
    // ... and the file is still there.
    QVERIFY(QFile::exists(session));

    // The exit neither writes over it nor deletes it - not with the session
    // on, and not with it off.
    MainWindowTestAccess::saveSession(window);
    QCOMPARE(readJsonObject(session), before);

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::RestoreSession, false);
    MainWindowTestAccess::saveSession(window);
    QCOMPARE(readJsonObject(session), before);
    QVERIFY(QFile::exists(session));
}

// A file of a shape this version does not know is left alone as well: it may
// be a file of a version that comes later.
void TestSession::aSessionFileOfAnotherShapeIsLeftAlone()
{
    const QString session = StateFile::sessionFilePath();
    QVERIFY(QFileInfo(session).dir().mkpath(QStringLiteral(".")));

    QFile file(session);
    QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Truncate));
    file.write(R"({"sections": {"somethingElse": {"version": 1, "values": {}}}})");
    file.close();
    const QJsonObject before = readJsonObject(session);

    m_fixture.startWatchdog();
    MainWindow window;

    QCOMPARE(m_fixture.closedModals(), 1);
    QVERIFY(QFile::exists(session));

    MainWindowTestAccess::saveSession(window);
    QCOMPARE(readJsonObject(session), before);
    QVERIFY(QFile::exists(session));
}

// The startup must be quiet. A session the application wrote itself holds
// nothing it does not know, so no box may open - an empty one would be the
// worst kind of noise.
void TestSession::theStartupShowsNoBox()
{
    const QString stub = TestEnvironment::writeShellStub(QStringLiteral("stub-hashcat-startup"),
                                                         "#!/bin/sh\n"
                                                         "printf '{\"0\":{\"name\":\"MD5\"}}\\n'\n"
                                                         "exit 0\n");
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    // A session the application wrote itself.
    MainWindow writer;
    MainWindowTestAccess::ui(writer)->spinBox_runtime->setValue(64);
    MainWindowTestAccess::saveSession(writer);

    m_fixture.startWatchdog();
    MainWindow reloaded;

    // The window opened without a box...
    QCOMPARE(m_fixture.closedModals(), 0);
    // ... and the session was restored.
    QCOMPARE(MainWindowTestAccess::ui(reloaded)->spinBox_runtime->value(), 64);
}

// The session format is the set of widget names: a new named widget - even a
// Designer auto-name - enters the frozen format and can never be renamed.
// The key set of a fresh window is pinned against the checked-in list.
void TestSession::theSerializedKeySetIsFrozen()
{
    MainWindow window;
    const QJsonObject values =
        WidgetStateSerializer::toJson(&window, {AppConstants::Files::PreviewWidget});

    const auto expected = std::to_array<QLatin1StringView>({
        QLatin1StringView{"checkBox_custom_charset1"},
        QLatin1StringView{"checkBox_custom_charset2"},
        QLatin1StringView{"checkBox_custom_charset3"},
        QLatin1StringView{"checkBox_custom_charset4"},
        QLatin1StringView{"checkBox_force"},
        QLatin1StringView{"checkBox_hex_charset"},
        QLatin1StringView{"checkBox_hex_salt"},
        QLatin1StringView{"checkBox_hex_wordlist"},
        QLatin1StringView{"checkBox_hwmon_temp_abort"},
        QLatin1StringView{"checkBox_ignoreusername"},
        QLatin1StringView{"checkBox_increment"},
        QLatin1StringView{"checkBox_optimized_kernel"},
        QLatin1StringView{"checkBox_outfile"},
        QLatin1StringView{"checkBox_outfile_json"},
        QLatin1StringView{"checkBox_potfile_disable"},
        QLatin1StringView{"checkBox_remove"},
        QLatin1StringView{"checkBox_rules_concat"},
        QLatin1StringView{"checkBox_rulesfile_1"},
        QLatin1StringView{"checkBox_rulesfile_2"},
        QLatin1StringView{"checkBox_rulesfile_3"},
        QLatin1StringView{"checkBox_speed_only"},
        QLatin1StringView{"checkBox_status"},
        QLatin1StringView{"comboBox_attack"},
        QLatin1StringView{"comboBox_hash_type"},
        QLatin1StringView{"lineEdit_association_settings"},
        QLatin1StringView{"lineEdit_backend_devices"},
        QLatin1StringView{"lineEdit_cache_path"},
        QLatin1StringView{"lineEdit_cpu_affinity"},
        QLatin1StringView{"lineEdit_custom_charset1"},
        QLatin1StringView{"lineEdit_custom_charset2"},
        QLatin1StringView{"lineEdit_custom_charset3"},
        QLatin1StringView{"lineEdit_custom_charset4"},
        QLatin1StringView{"lineEdit_encoding_from"},
        QLatin1StringView{"lineEdit_encoding_to"},
        QLatin1StringView{"lineEdit_feed"},
        QLatin1StringView{"lineEdit_feed_argument"},
        QLatin1StringView{"lineEdit_hash_recipe"},
        QLatin1StringView{"lineEdit_hashfile"},
        QLatin1StringView{"lineEdit_limit"},
        QLatin1StringView{"lineEdit_mask"},
        QLatin1StringView{"lineEdit_opencl_device_types"},
        QLatin1StringView{"lineEdit_outfile"},
        QLatin1StringView{"lineEdit_outfile_format"},
        QLatin1StringView{"lineEdit_pcfg_costmax"},
        QLatin1StringView{"lineEdit_pcfg_extra"},
        QLatin1StringView{"lineEdit_pcfg_kbits"},
        QLatin1StringView{"lineEdit_pcfg_mask"},
        QLatin1StringView{"lineEdit_pcfg_maxgain"},
        QLatin1StringView{"lineEdit_pcfg_maxword"},
        QLatin1StringView{"lineEdit_pcfg_ruleset_name"},
        QLatin1StringView{"lineEdit_pcfg_scale"},
        QLatin1StringView{"lineEdit_pcfg_threads"},
        QLatin1StringView{"lineEdit_pcfg_walk"},
        QLatin1StringView{"lineEdit_pcfg_weights"},
        QLatin1StringView{"lineEdit_rule_left"},
        QLatin1StringView{"lineEdit_rule_right"},
        QLatin1StringView{"lineEdit_rulesfile_1"},
        QLatin1StringView{"lineEdit_rulesfile_2"},
        QLatin1StringView{"lineEdit_rulesfile_3"},
        QLatin1StringView{"lineEdit_session"},
        QLatin1StringView{"lineEdit_skip"},
        QLatin1StringView{"lineEdit_table_cap"},
        QLatin1StringView{"lineEdit_table_extra"},
        QLatin1StringView{"lineEdit_table_identity"},
        QLatin1StringView{"lineEdit_table_maxperm"},
        QLatin1StringView{"lineEdit_table_single"},
        QLatin1StringView{"lineEdit_table_template"},
        QLatin1StringView{"listWidget_feed_arguments"},
        QLatin1StringView{"listWidget_pcfg_rulesets"},
        QLatin1StringView{"listWidget_tables"},
        QLatin1StringView{"listWidget_wordlist"},
        QLatin1StringView{"radioButton_generate_rules"},
        QLatin1StringView{"radioButton_use_rules_file"},
        QLatin1StringView{"spinBox_generate_rules"},
        QLatin1StringView{"spinBox_hwmon_temp_abort"},
        QLatin1StringView{"spinBox_increment_max"},
        QLatin1StringView{"spinBox_increment_min"},
        QLatin1StringView{"spinBox_runtime"},
        QLatin1StringView{"spinBox_status_timer"},
    });

    QStringList expectedNames;
    for (const QLatin1StringView &name : expected) {
        expectedNames << QString(name);
    }
    QCOMPARE(values.keys(), expectedNames);
}

// One canonical session with every serialized widget non-default: applying it
// and saving again has to be byte-identical, and the argv it produces is
// pinned. Together with the key-set test this guards the frozen session
// format and the argv order through every extraction step.
void TestSession::theGoldenSessionRoundTrips()
{
    const QString stub = TestEnvironment::writeShellStub(QStringLiteral("stub-hashcat-golden"),
                                                         "#!/bin/sh\n"
                                                         "printf '{\"0\":{\"name\":\"MD5\"},"
                                                         "\"1000\":{\"name\":\"NTLM\"},"
                                                         "\"4000\":{\"name\":\"recipe\"}}\\n'\n"
                                                         "exit 0\n");
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    const QString goldenPath =
        QStringLiteral(QT_TESTCASE_SOURCEDIR "/tests/data/golden_session.json");
    QFile goldenFile(goldenPath);
    QVERIFY2(goldenFile.open(QIODevice::ReadOnly), qPrintable(goldenPath));
    const QByteArray goldenBytes = goldenFile.readAll();
    goldenFile.close();

    MainWindow window;
    QTRY_VERIFY_WITH_TIMEOUT(MainWindowTestAccess::ui(window)->comboBox_hash_type->count() == 3,
                             5000);

    QString error;
    QJsonObject values;
    int version = 0;
    QStringList otherSections;
    QVERIFY(StateFile::read(goldenPath, AppConstants::FileSections::Session, &values, &version,
                            &otherSections, &error));
    const QStringList notes =
        MainWindowTestAccess::applySessionFile(window, values, version, otherSections);
    QVERIFY2(notes.isEmpty(), qPrintable(notes.join(QStringLiteral("\n"))));

    MainWindowTestAccess::saveSession(window);
    QFile saved(StateFile::sessionFilePath());
    QVERIFY(saved.open(QIODevice::ReadOnly));
    QCOMPARE(saved.readAll(), goldenBytes);

    const QStringList expectedArguments{
        QStringLiteral("--hash-type"),
        QStringLiteral("4000"),
        QStringLiteral("--hash-recipe"),
        QStringLiteral("md5(md5($p).$s)"),
        QStringLiteral("--attack-mode"),
        QStringLiteral("1"),
        QStringLiteral("--force"),
        QStringLiteral("--remove"),
        QStringLiteral("--username"),
        QStringLiteral("--session"),
        QStringLiteral("golden"),
        QStringLiteral("--runtime"),
        QStringLiteral("60"),
        QStringLiteral("--status"),
        QStringLiteral("--status-timer"),
        QStringLiteral("30"),
        QStringLiteral("--potfile-disable"),
        QStringLiteral("--generate-rules"),
        QStringLiteral("500"),
        QStringLiteral("--rule-left"),
        QStringLiteral("^a"),
        QStringLiteral("--rule-right"),
        QStringLiteral("$b"),
        QStringLiteral("--speed-only"),
        QStringLiteral("--optimized-kernel-enable"),
        QStringLiteral("--skip"),
        QStringLiteral("10"),
        QStringLiteral("--limit"),
        QStringLiteral("100"),
        QStringLiteral("--hex-salt"),
        QStringLiteral("--hex-wordlist"),
        QStringLiteral("--encoding-from"),
        QStringLiteral("utf-8"),
        QStringLiteral("--encoding-to"),
        QStringLiteral("iso-8859-1"),
        QStringLiteral("--outfile"),
        QStringLiteral("/tmp/golden-out.txt"),
        QStringLiteral("--outfile-json"),
        QStringLiteral("--cache-path"),
        QStringLiteral("/tmp/golden-cache"),
        QStringLiteral("--cpu-affinity"),
        QStringLiteral("0,1"),
        QStringLiteral("--backend-devices"),
        QStringLiteral("1"),
        QStringLiteral("--opencl-device-types"),
        QStringLiteral("1,2"),
        QStringLiteral("--hwmon-temp-abort"),
        QStringLiteral("85"),
        QStringLiteral("/tmp/golden-hashes.txt"),
        QStringLiteral("/tmp/golden-left.txt"),
        QStringLiteral("/tmp/golden-right.txt"),
    };
    QCOMPARE(MainWindowTestAccess::generateArguments(window), expectedArguments);
}

// A bulk apply rebuilds the preview exactly once, not once per widget: the
// golden session changes every serialized widget and still costs one rebuild.
void TestSession::applyingASessionRebuildsThePreviewOnce()
{
    const QString stub = TestEnvironment::writeShellStub(QStringLiteral("stub-hashcat-once"),
                                                         "#!/bin/sh\n"
                                                         "printf '{\"0\":{\"name\":\"MD5\"},"
                                                         "\"1000\":{\"name\":\"NTLM\"},"
                                                         "\"4000\":{\"name\":\"recipe\"}}\\n'\n"
                                                         "exit 0\n");
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    MainWindow window;
    QTRY_VERIFY_WITH_TIMEOUT(MainWindowTestAccess::ui(window)->comboBox_hash_type->count() == 3,
                             5000);

    const QString goldenPath =
        QStringLiteral(QT_TESTCASE_SOURCEDIR "/tests/data/golden_session.json");
    QString error;
    QJsonObject values;
    int version = 0;
    QStringList otherSections;
    QVERIFY(StateFile::read(goldenPath, AppConstants::FileSections::Session, &values, &version,
                            &otherSections, &error));

    const int before = MainWindowTestAccess::previewRebuilds(window);
    const QStringList notes =
        MainWindowTestAccess::applySessionFile(window, values, version, otherSections);
    QVERIFY2(notes.isEmpty(), qPrintable(notes.join(QStringLiteral("\n"))));
    QCOMPARE(MainWindowTestAccess::previewRebuilds(window) - before, 1);

    // The one rebuild really produced the session's command, not the default.
    QVERIFY(MainWindowTestAccess::ui(window)->lineEdit_command->text()
            != QStringLiteral("--attack-mode 0"));
}

// A no-op apply is a bulk write too: the serializer clears and re-inserts
// every list even when the values are identical, and the guard has to cover
// that churn as well.
void TestSession::aNoOpApplyWithRowsRebuildsThePreviewOnce()
{
    const QString stub = hashcatStub(QStringLiteral("stub-hashcat-noop"));
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    MainWindow window;
    QTRY_VERIFY_WITH_TIMEOUT(MainWindowTestAccess::ui(window)->comboBox_hash_type->count() == 1,
                             5000);
    MainWindowTestAccess::addPathsToList(MainWindowTestAccess::ui(window)->listWidget_wordlist,
                                         {QStringLiteral("/tmp/one.txt")});
    MainWindowTestAccess::addPathsToList(
        MainWindowTestAccess::ui(window)->listWidget_feed_arguments,
        {QStringLiteral("arg one"), QStringLiteral("arg two")});

    const QJsonObject current =
        WidgetStateSerializer::toJson(&window, {AppConstants::Files::PreviewWidget});
    const int before = MainWindowTestAccess::previewRebuilds(window);
    const QStringList notes = MainWindowTestAccess::applySessionFile(
        window, current, AppConstants::Sessions::Version, {});
    QVERIFY2(notes.isEmpty(), qPrintable(notes.join(QStringLiteral("\n"))));
    QCOMPARE(MainWindowTestAccess::previewRebuilds(window) - before, 1);
}

// The reset writes every widget back to its default; that is one bulk write
// and one preview rebuild.
void TestSession::resettingTheSessionRebuildsThePreviewOnce()
{
    MainWindow window;
    changeEveryValue(&window);

    TestEnvironment::ModalRecorder recorder;
    recorder.acceptConfirmation = true;
    recorder.start();

    const int before = MainWindowTestAccess::previewRebuilds(window);
    QVERIFY(QMetaObject::invokeMethod(&window, "resetSessionTriggered"));
    QVERIFY(recorder.messageBoxSeen);
    QCOMPARE(MainWindowTestAccess::previewRebuilds(window) - before, 1);
}

int main(int argc, char *argv[])
{
    TestSession test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_session.moc"
