/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QClipboard>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QRadioButton>
#include <QSpinBox>
#include <QStandardPaths>
#include <QScrollArea>
#include <QScrollBar>
#include <QScreen>
#include <QTimer>
#include <QToolTip>
#include <QWidget>

#include "appconstants.h"
#include "commandbuilder.h"
#include "hashcatoptions.h"
#include "mainwindow.h"
#include "settingsmanager.h"
#include "ui_mainwindow.h"
#include "statefile.h"
#include "widgetstateserializer.h"

namespace
{

// Puts every widget below root into a state the defaults do not have, so a
// reset that forgets one of them shows up in the comparison with a fresh
// window. The command preview is left alone: it is generated, not entered.
void changeEveryValue(QWidget *root)
{
    const auto lineEdits = root->findChildren<QLineEdit *>();
    for (QLineEdit *lineEdit : lineEdits) {
        if (lineEdit->objectName() == AppConstants::Files::PreviewWidget) {
            continue;
        }
        lineEdit->setText(lineEdit->text().isEmpty() ? QStringLiteral("changed") : QString());
    }

    const auto checkBoxes = root->findChildren<QCheckBox *>();
    for (QCheckBox *checkBox : checkBoxes) {
        checkBox->setChecked(!checkBox->isChecked());
    }

    // The radio buttons of a group exclude each other, so the reset has to
    // name the one that is checked.
    const auto radioButtons = root->findChildren<QRadioButton *>();
    for (QRadioButton *radioButton : radioButtons) {
        radioButton->setChecked(!radioButton->isChecked());
    }

    const auto comboBoxes = root->findChildren<QComboBox *>();
    for (QComboBox *comboBox : comboBoxes) {
        if (comboBox->count() > 1) {
            comboBox->setCurrentIndex(comboBox->currentIndex() == 0 ? 1 : 0);
        }
    }

    const auto spinBoxes = root->findChildren<QSpinBox *>();
    for (QSpinBox *spinBox : spinBoxes) {
        if (spinBox->minimum() != spinBox->maximum()) {
            spinBox->setValue(spinBox->value() == spinBox->maximum() ? spinBox->value() - 1
                                                                     : spinBox->value() + 1);
        }
    }

    const auto doubleSpinBoxes = root->findChildren<QDoubleSpinBox *>();
    for (QDoubleSpinBox *doubleSpinBox : doubleSpinBoxes) {
        if (doubleSpinBox->minimum() != doubleSpinBox->maximum()) {
            doubleSpinBox->setValue(doubleSpinBox->value() == doubleSpinBox->maximum()
                                        ? doubleSpinBox->value() - 1
                                        : doubleSpinBox->value() + 1);
        }
    }

    const auto listWidgets = root->findChildren<QListWidget *>();
    for (QListWidget *listWidget : listWidgets) {
        listWidget->addItem(QStringLiteral("changed"));
    }
}

// The session file a serializer wrote, as the object it holds.
QJsonObject readJsonObject(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return {};
    }
    const QByteArray data = file.readAll();
    file.close();
    return QJsonDocument::fromJson(data).object();
}

// The widgets whose values in filename differ from the ones in reference, by
// name. A widget the reset forgot is named here, not just counted.
QStringList differingWidgets(const QString &filename, const QJsonObject &reference)
{
    const QJsonObject state = readJsonObject(filename)
                                  .value(QStringLiteral("sections"))
                                  .toObject()
                                  .value(QString(AppConstants::FileSections::Session))
                                  .toObject()
                                  .value(QStringLiteral("values"))
                                  .toObject();

    QStringList names;
    for (auto it = state.begin(); it != state.end(); ++it) {
        if (reference.value(it.key()) != it.value()) {
            names << it.key();
        }
    }
    for (auto it = reference.begin(); it != reference.end(); ++it) {
        if (!state.contains(it.key())) {
            names << it.key();
        }
    }
    return names;
}

// A wordlist the monitor can see. The handle is closed at the end of the
// scope, so the file can be removed again while the window watches it.
bool writeFile(const QString &path)
{
    const QByteArray content = QByteArrayLiteral("password\n");
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }
    return file.write(content) == content.size();
}

} // namespace

// A hashcat that answers the query for the hash types.
//
// A binary that is not there opens a "hashcat error" box while the test
// runs, and the recorder keeps only the last box it saw - so a test that
// wants a box of its own needs a hashcat that answers.
QString hashcatStub(const QString &name)
{
    return TestEnvironment::writeShellStub(name, "#!/bin/sh\n"
                                                 "printf '{\"0\":{\"name\":\"MD5\"}}\\n'\n"
                                                 "exit 0\n");
}

// The half of the command line rules that lives in widgets: does a freshly
// opened window really start at the values HashcatOptions documents, does the
// item data survive into the options, and do the group boxes agree with the
// builder?
class TestMainWindow : public QObject
{
    Q_OBJECT

public:
    TestMainWindow()
    {
        // Safety net for the tests that spin the event loop: a reply the stub
        // cannot produce opens a modal warning, and a modal must never be able
        // to hang the run. A member, so cleanup() stops it before it can fire
        // into a later test.
        m_watchdog.setSingleShot(false);
        m_watchdog.setInterval(100);
        connect(&m_watchdog, &QTimer::timeout, this, [this] {
            if (QWidget *modal = QApplication::activeModalWidget()) {
                ++m_closedModals;
                modal->close();
            }
        });
    }

private slots:
    void init();
    void cleanup();
    void freshWindowMatchesTheDefaultOptions();
    void commandPreviewFollowsWidgetChanges();
    void attackModeIdsTravelAsItemData();
    void groupBoxesFollowTheBuilderPredicates();
    void groupBoxesShareOneTitleAlignment();
    void generalTabGroupBoxesAlignTheirContentToTheTop();
    void resetSessionRestoresEveryWidget();
    void resettingTheSessionNeedsAConfirmation();
    void exampleSessionFileIsValid();
    void loadExampleSessionNeedsAConfirmation();
    void loadExampleSessionAppliesTheBundledSession();
    void optionDependenciesFollowTheirCheckboxes();
    void modeHintFollowsTheAttackMode();
    void launchValidationFollowsTheWidgets();
    void widgetsFlowIntoTheOptions();
    void statusTimerReachesTheCommandLine();
    void skipAndLimitAcceptOnlyNumbers();
    void checkedWordlistsEndUpInTheCommand();
    void missingWordlistsAreMarked();
    void hashTypeComesFromTheItemData();
    void hashTypesAreFilledFromHashcat();
    void failedQueryReenablesTheHashTypeCombo_data();
    void failedQueryReenablesTheHashTypeCombo();
    void outfileSuggestionFollowsUntilTheUserTakesOver();
    void wordlistMoveSurvivesAMissingSelection();
    void commandPreviewCarriesTheBinaryName();
    void thePreviewTooltipHoldsTheWholeCommand();
    void copyingTheCommandPutsItOnTheClipboard();
    void theSessionRoundTrips();
    void turningTheSessionOffRemovesTheStoredSession();
    void sessionHashTypeSurvivesTheAsynchronousQuery();
    void sessionWithAnUnknownHashTypeSelectsNothing();
    void windowFitsItsContent();
    void commandBarStaysVisibleOnEveryTab();
    void theSettingsAreExportedThroughTheFileMenu();
    void theSettingsAreImportedThroughTheFileMenu();
    void theImportNamesTheValuesTheSessionDoesNotKnow();
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

private:
    QString m_savedHashcatPath;
    QString m_savedTerminal;
    bool m_savedShortParameters = false;
    bool m_savedRestoreSession = true;
    // The modals the watchdog had to close: a test that expects none.
    int m_closedModals = 0;
    QTimer m_watchdog;
};

// Every test starts from the same settings, and cleanup() puts back whatever
// it found, so the order the slots run in stops mattering.
void TestMainWindow::init()
{
    auto &settings = SettingsManager::instance();
    m_savedHashcatPath = settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    m_savedTerminal = settings.getKey<QString>(AppConstants::SettingsKeys::Terminal);
    m_savedShortParameters = settings.getKey<bool>(AppConstants::SettingsKeys::UseShortParameters);
    m_savedRestoreSession = settings.getKey<bool>(AppConstants::SettingsKeys::RestoreSession, true);

    // A deterministic starting point: no configured hashcat, no configured
    // terminal, long parameters, the session restored. Everything else comes
    // from the fresh XDG directories, which hold no session to load.
    settings.setKey(AppConstants::SettingsKeys::HashcatPath, QString());
    settings.setKey(AppConstants::SettingsKeys::Terminal, QString());
    settings.setKey(AppConstants::SettingsKeys::UseShortParameters, false);
    settings.setKey(AppConstants::SettingsKeys::RestoreSession, true);

    m_closedModals = 0;
}

void TestMainWindow::cleanup()
{
    m_watchdog.stop();

    // The round-trip test writes a session into the redirected data
    // directory, and that directory lives for the whole run: every later
    // window would silently load whatever was left there.
    QFile::remove(QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))
                      .filePath(AppConstants::Files::SessionFile));

    auto &settings = SettingsManager::instance();
    settings.setKey(AppConstants::SettingsKeys::HashcatPath, m_savedHashcatPath);
    settings.setKey(AppConstants::SettingsKeys::Terminal, m_savedTerminal);
    settings.setKey(AppConstants::SettingsKeys::UseShortParameters, m_savedShortParameters);
    settings.setKey(AppConstants::SettingsKeys::RestoreSession, m_savedRestoreSession);
}

void TestMainWindow::freshWindowMatchesTheDefaultOptions()
{
    MainWindow window;

    // Ground truth spelled out: comparing build() against build() would let a
    // builder that emits nothing pass every run
    const QStringList expected{"--attack-mode", "0"};

    QCOMPARE(CommandBuilder::build(HashcatOptions{}), expected);
    QCOMPARE(window.generateArguments(), expected);

    // ... and that is what the preview shows, character for character
    QCOMPARE(window.ui->lineEdit_command->text(), expected.join(QLatin1Char(' ')));
}

void TestMainWindow::commandPreviewFollowsWidgetChanges()
{
    MainWindow window;
    QLineEdit *preview = window.ui->lineEdit_command;

    QCOMPARE(preview->text(), QStringLiteral("--attack-mode 0"));

    // One representative for each connection helper.
    window.ui->checkBox_remove->setChecked(true);
    QVERIFY(preview->text().contains(QStringLiteral("--remove")));

    window.ui->checkBox_ignoreusername->setChecked(true);
    QVERIFY(preview->text().contains(QStringLiteral("--username")));

    window.ui->lineEdit_backend_devices->setText(QStringLiteral("1,2"));
    QVERIFY(preview->text().contains(QStringLiteral("--backend-devices 1,2")));

    window.ui->checkBox_hwmon_temp_abort->setChecked(true);
    window.ui->spinBox_hwmon_temp_abort->setValue(85);
    QVERIFY(preview->text().contains(QStringLiteral("--hwmon-temp-abort 85")));

    window.ui->lineEdit_session->setText(QStringLiteral("job"));
    QVERIFY(preview->text().contains(QStringLiteral("--session job")));

    window.ui->checkBox_force->setChecked(true);
    QVERIFY(preview->text().contains(QStringLiteral("--force")));

    window.ui->spinBox_runtime->setValue(60);
    QVERIFY(preview->text().contains(QStringLiteral("--runtime 60")));

    // Radio buttons share the toggle helper and keep their side-effect slot.
    window.ui->radioButton_generate_rules->setChecked(true);
    QVERIFY(preview->text().contains(QStringLiteral("--generate-rules 1")));

    // The hash combo is filled asynchronously in the application; fill it by
    // hand here to test the index connection without a hashcat query.
    window.ui->comboBox_hash_type->clear();
    window.ui->comboBox_hash_type->addItem(QStringLiteral("1000 | NTLM"), 1000);
    QVERIFY(preview->text().contains(QStringLiteral("--hash-type 1000")));

    // The attack mode refreshes the preview through updateViewAttackMode(),
    // not through a second commandChanged connection.
    window.ui->comboBox_attack->setCurrentIndex(2); // brute-force
    QVERIFY(preview->text().contains(QStringLiteral("--attack-mode 3")));

    window.ui->lineEdit_mask->setText(QStringLiteral("?d?d"));
    QVERIFY(preview->text().contains(QStringLiteral("?d?d")));

    window.ui->checkBox_increment->setChecked(true);
    QVERIFY(preview->text().contains(QStringLiteral("--increment")));
}

void TestMainWindow::attackModeIdsTravelAsItemData()
{
    MainWindow window;

    const QList<int> expected{0, 1, 3, 6, 7, 9};
    QList<int> actual;
    for (int i = 0; i < window.ui->comboBox_attack->count(); ++i) {
        actual << window.ui->comboBox_attack->itemData(i).toInt();
    }
    QCOMPARE(actual, expected);

    // The displayed text is what a translation replaces; the id is what has
    // to reach -a.
    for (int i = 0; i < window.ui->comboBox_attack->count(); ++i) {
        window.ui->comboBox_attack->setCurrentIndex(i);
        QCOMPARE(int(window.collectHashcatOptions().attackMode), expected.at(i));
        QVERIFY(window.ui->comboBox_attack->itemData(i).isValid());
    }
}

void TestMainWindow::groupBoxesFollowTheBuilderPredicates()
{
    MainWindow window;

    // What each mode owes the widgets, independent of the predicates
    // updateViewAttackMode() switches on: the same table
    // tst_commandbuilder::predicates pins for the builder.
    struct Expectation {
        AttackMode mode;
        bool wordlists;
        bool rules;
        bool ruleLeft;
        bool ruleRight;
        bool mask;
    };
    const QList<Expectation> expectations{
        {AttackMode::Straight, true, true, true, false, false},
        {AttackMode::Combination, true, false, true, true, false},
        {AttackMode::BruteForce, false, false, false, false, true},
        {AttackMode::HybridWordMask, true, false, true, false, true},
        {AttackMode::HybridMaskWord, true, false, false, true, true},
        {AttackMode::Association, true, true, true, false, false},
    };

    for (int i = 0; i < window.ui->comboBox_attack->count(); ++i) {
        // -1 first, so that every iteration really changes the index and runs
        // attackIndexChanged()
        window.ui->comboBox_attack->setCurrentIndex(-1);
        window.ui->comboBox_attack->setCurrentIndex(i);

        const AttackMode mode =
            static_cast<AttackMode>(window.ui->comboBox_attack->currentData().toInt());

        const Expectation *expected = nullptr;
        for (const Expectation &candidate : expectations) {
            if (candidate.mode == mode) {
                expected = &candidate;
            }
        }
        QVERIFY2(expected, "no expectation written down for this attack mode");

        QCOMPARE(window.ui->groupBox_wordlists->isEnabled(), expected->wordlists);
        QCOMPARE(window.ui->groupBox_rules->isEnabled(), expected->rules);
        QCOMPARE(window.ui->groupBox_wordlist_rules->isEnabled(),
                 expected->ruleLeft || expected->ruleRight);
        QCOMPARE(window.ui->label_rule_left->isEnabled(), expected->ruleLeft);
        QCOMPARE(window.ui->lineEdit_rule_left->isEnabled(), expected->ruleLeft);
        QCOMPARE(window.ui->label_rule_right->isEnabled(), expected->ruleRight);
        QCOMPARE(window.ui->lineEdit_rule_right->isEnabled(), expected->ruleRight);
        QCOMPARE(window.ui->groupBox_mask->isEnabled(), expected->mask);
        QCOMPARE(window.ui->groupBox_custom_charset->isEnabled(), expected->mask);
        // --hex-charset belongs to the mask modes for the same reason.
        QCOMPARE(window.ui->checkBox_hex_charset->isEnabled(), expected->mask);
    }
}

// The titles of the group boxes have to look alike. The alignment is the only
// thing that decides it, and a group box without it would draw its title
// above the frame while its neighbors break the border with it.
void TestMainWindow::groupBoxesShareOneTitleAlignment()
{
    MainWindow window;

    TestEnvironment::checkGroupBoxTitleAlignment(&window);
}

// A group box in a row is as tall as its tallest neighbor. Without the top
// alignment Qt spreads the children of the shorter boxes over that height or
// centers them, so two boxes next to each other would start their content at
// different heights.
void TestMainWindow::generalTabGroupBoxesAlignTheirContentToTheTop()
{
    MainWindow window;

    const QList<QGroupBox *> boxes = window.ui->tab_general->findChildren<QGroupBox *>();
    QVERIFY(!boxes.isEmpty());

    QStringList offenders;
    for (QGroupBox *box : boxes) {
        if (!(box->layout()->alignment() & Qt::AlignTop)) {
            offenders << box->objectName();
        }
    }
    QVERIFY2(offenders.isEmpty(),
             qPrintable(QStringLiteral("group boxes whose content is not top aligned: %1")
                            .arg(offenders.join(QStringLiteral(", ")))));
}

// The reset has to bring every widget that holds a value back to its
// default - and only after the user confirmed it, so the box is answered
// here.
//
// The test does not know the widgets: it changes every one of them, resets
// the form and compares the whole state with a fresh window. A widget the
// reset forgets - now or after it is added - shows up as a difference, and
// the list of widgets does not have to be kept in two places.
void TestMainWindow::resetSessionRestoresEveryWidget()
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
void TestMainWindow::resettingTheSessionNeedsAConfirmation()
{
    MainWindow window;
    window.ui->lineEdit_session->setText(QStringLiteral("keepMe"));

    TestEnvironment::ModalRecorder recorder;
    recorder.start();

    QVERIFY(QMetaObject::invokeMethod(&window, "resetSessionTriggered"));

    QVERIFY(recorder.messageBoxSeen);
    QCOMPARE(recorder.title, QStringLiteral("Reset session"));
    QVERIFY(recorder.text.contains(QStringLiteral("lost")));
    // The recorder closed the box without confirming: nothing was reset.
    QCOMPARE(window.ui->lineEdit_session->text(), QStringLiteral("keepMe"));
}

// The example that ships in the binary has to stay a valid session: every
// widget a session stores is named, and applying it leaves no value behind.
void TestMainWindow::exampleSessionFileIsValid()
{
    const QString file = QString(AppConstants::Files::ExampleSession);
    QVERIFY2(QFile::exists(file), qPrintable(file));

    QString error;
    QJsonObject values;
    int version = -1;
    QStringList otherSections;
    QVERIFY2(StateFile::read(file, AppConstants::FileSections::Session, &values, &version,
                             &otherSections, &error),
             qPrintable(error));
    QCOMPARE(version, AppConstants::Sessions::Version);
    QVERIFY(otherSections.isEmpty());

    MainWindow window;
    // The hash type list normally arrives from hashcat in the background;
    // the test only needs the entry the example names.
    window.ui->comboBox_hash_type->clear();
    window.ui->comboBox_hash_type->addItem(QStringLiteral("1400 | SHA2-256"), 1400);

    const QStringList notes = window.applySessionFile(values, version, otherSections);
    QVERIFY2(notes.isEmpty(), qPrintable(notes.join(QLatin1Char('\n'))));

    // A widget added later must be added to the example as well, or loading
    // it would leave that widget at whatever the user had in the form.
    const QJsonObject state =
        WidgetStateSerializer::toJson(&window, {AppConstants::Files::PreviewWidget});
    QStringList missing;
    for (auto it = state.constBegin(); it != state.constEnd(); ++it) {
        if (!values.contains(it.key())) {
            missing << it.key();
        }
    }
    QVERIFY2(missing.isEmpty(), qPrintable(QStringLiteral("the example does not name: %1")
                                               .arg(missing.join(QStringLiteral(", ")))));
}

// Like a reset, loading the example throws away the session in the form, so
// declining the question has to leave it untouched.
void TestMainWindow::loadExampleSessionNeedsAConfirmation()
{
    MainWindow window;
    window.ui->lineEdit_session->setText(QStringLiteral("keepMe"));

    TestEnvironment::ModalRecorder recorder;
    recorder.start();

    QVERIFY(QMetaObject::invokeMethod(&window, "loadExampleSessionTriggered"));

    QVERIFY(recorder.messageBoxSeen);
    QCOMPARE(recorder.title, QStringLiteral("Load example session"));
    QVERIFY(recorder.text.contains(QStringLiteral("lost")));
    QVERIFY(recorder.text.contains(QStringLiteral("should recover the password Nice!98")));
    // The recorder closed the box without confirming: nothing was loaded.
    QCOMPARE(window.ui->lineEdit_session->text(), QStringLiteral("keepMe"));
}

// Confirming the question loads the bundled example through the same path an
// imported session takes.
void TestMainWindow::loadExampleSessionAppliesTheBundledSession()
{
    MainWindow window;
    window.ui->comboBox_hash_type->clear();
    window.ui->comboBox_hash_type->addItem(QStringLiteral("1400 | SHA2-256"), 1400);

    TestEnvironment::ModalRecorder recorder;
    recorder.acceptConfirmation = true;
    recorder.start();

    QVERIFY(QMetaObject::invokeMethod(&window, "loadExampleSessionTriggered"));

    QCOMPARE(window.ui->comboBox_attack->currentData().toInt(), 3);
    QCOMPARE(window.ui->comboBox_hash_type->currentData().toInt(), 1400);
    QCOMPARE(window.ui->lineEdit_hashfile->text(),
             QStringLiteral("ee3d96862a7b0806e6e0ad7861c46ea2d0b25a5eb9607cf5c837eb9906ca53b6"));
    QCOMPARE(window.ui->lineEdit_mask->text(), QStringLiteral("?u?1?1?1?2?d?d"));
    QCOMPARE(window.ui->lineEdit_custom_charset1->text(), QStringLiteral("?l?u"));
    QCOMPARE(window.ui->lineEdit_custom_charset2->text(), QStringLiteral("!@#$"));
    QVERIFY(window.ui->checkBox_custom_charset1->isChecked());
    QVERIFY(window.ui->checkBox_custom_charset2->isChecked());
    QVERIFY(window.ui->lineEdit_command->text().contains(QStringLiteral("--attack-mode 3")));
    QVERIFY(window.ui->lineEdit_command->text().contains(QStringLiteral("--hash-type 1400")));
}

void TestMainWindow::optionDependenciesFollowTheirCheckboxes()
{
    MainWindow window;
    // A mask mode keeps the increment widgets out of the disabled mask group,
    // so the test observes their own dependency and not the group's.
    window.ui->comboBox_attack->setCurrentIndex(2); // brute-force

    // A fresh window: every dependent value starts switched off.
    QVERIFY(!window.ui->spinBox_increment_min->isEnabled());
    QVERIFY(!window.ui->spinBox_increment_max->isEnabled());
    QVERIFY(!window.ui->spinBox_status_timer->isEnabled());
    QVERIFY(!window.ui->spinBox_hwmon_temp_abort->isEnabled());

    window.ui->checkBox_increment->setChecked(true);
    QVERIFY(window.ui->spinBox_increment_min->isEnabled());
    QVERIFY(window.ui->spinBox_increment_max->isEnabled());
    QVERIFY(window.ui->label_increment_min->isEnabled());
    QVERIFY(window.ui->label_increment_max->isEnabled());
    window.ui->checkBox_increment->setChecked(false);
    QVERIFY(!window.ui->spinBox_increment_min->isEnabled());
    QVERIFY(!window.ui->spinBox_increment_max->isEnabled());

    window.ui->checkBox_status->setChecked(true);
    QVERIFY(window.ui->spinBox_status_timer->isEnabled());
    window.ui->checkBox_status->setChecked(false);
    QVERIFY(!window.ui->spinBox_status_timer->isEnabled());

    window.ui->checkBox_hwmon_temp_abort->setChecked(true);
    QVERIFY(window.ui->spinBox_hwmon_temp_abort->isEnabled());
    window.ui->checkBox_hwmon_temp_abort->setChecked(false);
    QVERIFY(!window.ui->spinBox_hwmon_temp_abort->isEnabled());
}

// The hint under the mode selector has to tell the user what the mode needs,
// otherwise the launch validation is the first thing that explains it.
void TestMainWindow::modeHintFollowsTheAttackMode()
{
    MainWindow window;

    struct Expectation {
        AttackMode mode;
        QString hint;
    };
    const QList<Expectation> expectations{
        {AttackMode::Straight, QStringLiteral("This mode needs at least one wordlist.")},
        {AttackMode::Combination,
         QStringLiteral("This mode needs exactly 2 wordlists (left, right).")},
        {AttackMode::BruteForce, QStringLiteral("This mode needs a mask.")},
        {AttackMode::HybridWordMask, QStringLiteral("This mode needs a wordlist and a mask.")},
        {AttackMode::HybridMaskWord, QStringLiteral("This mode needs a wordlist and a mask.")},
        {AttackMode::Association, QStringLiteral("This mode needs at least one wordlist.")},
    };

    for (int i = 0; i < window.ui->comboBox_attack->count(); ++i) {
        // -1 first, so that every iteration really changes the index and runs
        // attackIndexChanged()
        window.ui->comboBox_attack->setCurrentIndex(-1);
        window.ui->comboBox_attack->setCurrentIndex(i);

        const AttackMode mode =
            static_cast<AttackMode>(window.ui->comboBox_attack->currentData().toInt());

        const Expectation *expected = nullptr;
        for (const Expectation &candidate : expectations) {
            if (candidate.mode == mode) {
                expected = &candidate;
            }
        }
        QVERIFY2(expected, "no expectation written down for this attack mode");

        QCOMPARE(window.ui->label_mode_hint->text(), expected->hint);
    }
}

// The launch validation works on what the window collects, so the two have to
// agree: what the widgets show is what the validation complains about.
void TestMainWindow::launchValidationFollowsTheWidgets()
{
    MainWindow window;

    const auto addWordlist = [&window](const QString &path) {
        auto *item = new QListWidgetItem(path, window.ui->listWidget_wordlist);
        item->setCheckState(Qt::Checked);
    };

    // Straight mode without a word list is what the hint warns about.
    QCOMPARE(int(HelperUtils::validateOptions(window.collectHashcatOptions())),
             int(HelperUtils::LaunchError::NoWordlist));

    addWordlist(QStringLiteral("/tmp/a.txt"));
    QCOMPARE(int(HelperUtils::validateOptions(window.collectHashcatOptions())),
             int(HelperUtils::LaunchError::None));

    // Combination mode wants exactly two word lists.
    window.ui->comboBox_attack->setCurrentIndex(1); // combination
    QCOMPARE(int(window.ui->comboBox_attack->currentData().toInt()), int(AttackMode::Combination));
    QCOMPARE(int(HelperUtils::validateOptions(window.collectHashcatOptions())),
             int(HelperUtils::LaunchError::WrongWordlistCount));

    addWordlist(QStringLiteral("/tmp/b.txt"));
    QCOMPARE(int(HelperUtils::validateOptions(window.collectHashcatOptions())),
             int(HelperUtils::LaunchError::None));

    addWordlist(QStringLiteral("/tmp/c.txt"));
    QCOMPARE(int(HelperUtils::validateOptions(window.collectHashcatOptions())),
             int(HelperUtils::LaunchError::WrongWordlistCount));

    // A mask mode needs a mask, and the bounds of the increment have to fit.
    window.ui->comboBox_attack->setCurrentIndex(2); // brute-force
    QCOMPARE(int(HelperUtils::validateOptions(window.collectHashcatOptions())),
             int(HelperUtils::LaunchError::NoMask));

    window.ui->lineEdit_mask->setText(QStringLiteral("?d?d?d?d"));
    QCOMPARE(int(HelperUtils::validateOptions(window.collectHashcatOptions())),
             int(HelperUtils::LaunchError::None));

    window.ui->checkBox_increment->setChecked(true);
    window.ui->spinBox_increment_min->setValue(8);
    window.ui->spinBox_increment_max->setValue(4);
    QCOMPARE(int(HelperUtils::validateOptions(window.collectHashcatOptions())),
             int(HelperUtils::LaunchError::InvalidIncrement));

    // A valid pair of bounds, but incrementing and speed-only are mutually
    // exclusive in hashcat. Both controls are reachable in a mask mode.
    window.ui->spinBox_increment_min->setValue(4);
    window.ui->spinBox_increment_max->setValue(8);
    window.ui->checkBox_speed_only->setChecked(true);
    QCOMPARE(int(HelperUtils::validateOptions(window.collectHashcatOptions())),
             int(HelperUtils::LaunchError::IncrementWithSpeedOnly));
    window.ui->checkBox_speed_only->setChecked(false);
    QCOMPARE(int(HelperUtils::validateOptions(window.collectHashcatOptions())),
             int(HelperUtils::LaunchError::None));

    // Association keeps the checked increment from the brute-force step (its
    // group box is disabled), but the command line carries no -i, so the
    // launch validation has to ignore it. --remove is refused, and its box
    // stays enabled so the user can clear it.
    window.ui->comboBox_attack->setCurrentIndex(5); // association
    QCOMPARE(int(window.ui->comboBox_attack->currentData().toInt()), int(AttackMode::Association));
    QCOMPARE(int(HelperUtils::validateOptions(window.collectHashcatOptions())),
             int(HelperUtils::LaunchError::None));

    window.ui->checkBox_remove->setChecked(true);
    QCOMPARE(int(HelperUtils::validateOptions(window.collectHashcatOptions())),
             int(HelperUtils::LaunchError::RemoveInAssociation));
    window.ui->checkBox_remove->setChecked(false);
    QCOMPARE(int(HelperUtils::validateOptions(window.collectHashcatOptions())),
             int(HelperUtils::LaunchError::None));
}

void TestMainWindow::widgetsFlowIntoTheOptions()
{
    MainWindow window;

    window.ui->lineEdit_hashfile->setText(QStringLiteral("/tmp/example.hash"));
    window.ui->lineEdit_mask->setText(QStringLiteral("?d?d"));
    window.ui->checkBox_remove->setChecked(true);
    window.ui->checkBox_ignoreusername->setChecked(true);
    window.ui->checkBox_speed_only->setChecked(true);
    window.ui->checkBox_hex_wordlist->setChecked(true);
    window.ui->lineEdit_encoding_from->setText(QStringLiteral("iso-8859-15"));
    window.ui->lineEdit_encoding_to->setText(QStringLiteral("utf-8"));
    window.ui->lineEdit_rule_left->setText(QStringLiteral("c"));
    window.ui->lineEdit_rule_right->setText(QStringLiteral("^x"));
    window.ui->checkBox_increment->setChecked(true);
    window.ui->spinBox_increment_min->setValue(2);
    window.ui->spinBox_increment_max->setValue(8);
    window.ui->lineEdit_session->setText(QStringLiteral("job"));
    window.ui->spinBox_runtime->setValue(60);
    window.ui->lineEdit_skip->setText(QStringLiteral("100"));
    window.ui->lineEdit_limit->setText(QStringLiteral("200"));
    window.ui->checkBox_status->setChecked(true);
    window.ui->spinBox_status_timer->setValue(5);
    window.ui->checkBox_force->setChecked(true);
    window.ui->checkBox_potfile_disable->setChecked(true);
    window.ui->lineEdit_opencl_device_types->setText(QStringLiteral("1,2"));
    window.ui->checkBox_hwmon_temp_abort->setChecked(true);
    window.ui->spinBox_hwmon_temp_abort->setValue(85);

    const HashcatOptions options = window.collectHashcatOptions();
    QCOMPARE(options.hashFile, QStringLiteral("/tmp/example.hash"));
    QCOMPARE(options.mask, QStringLiteral("?d?d"));
    QVERIFY(options.remove);
    QVERIFY(options.ignoreUsername);
    QVERIFY(options.speedOnly);
    QVERIFY(options.hexWordlist);
    QCOMPARE(options.encodingFrom, QStringLiteral("iso-8859-15"));
    QCOMPARE(options.encodingTo, QStringLiteral("utf-8"));
    QCOMPARE(options.ruleLeft, QStringLiteral("c"));
    QCOMPARE(options.ruleRight, QStringLiteral("^x"));
    QVERIFY(options.increment);
    QCOMPARE(options.incrementMin, 2);
    QCOMPARE(options.incrementMax, 8);
    QCOMPARE(options.session, QStringLiteral("job"));
    QCOMPARE(options.runtime, 60);
    QCOMPARE(options.skip, QStringLiteral("100"));
    QCOMPARE(options.limit, QStringLiteral("200"));
    QVERIFY(options.status);
    QCOMPARE(options.statusTimer, 5);
    QVERIFY(options.force);
    QVERIFY(options.potfileDisable);
    QCOMPARE(options.openclDeviceTypes, QStringLiteral("1,2"));
    QVERIFY(options.hwmonTempAbort);
    QCOMPARE(options.hwmonTempAbortCelsius, 85);

    // Untouched fields keep the values the .ui file starts with
    QCOMPARE(options.outfileFormat, AppConstants::Defaults::OutfileFormat);
    QCOMPARE(options.backendDevices, AppConstants::Defaults::BackendDevices);

    // The suggested outfile never switches output on by itself
    QVERIFY(!options.outfileEnabled);

    const QStringList arguments = window.generateArguments();
    QVERIFY(arguments.contains(QStringLiteral("--remove")));
    QVERIFY(arguments.contains(QStringLiteral("--username")));
    QVERIFY(arguments.contains(QStringLiteral("--speed-only")));
    QVERIFY(arguments.contains(QStringLiteral("--force")));
    QVERIFY(arguments.contains(QStringLiteral("--session")));
    QVERIFY(arguments.contains(QStringLiteral("--runtime")));
    QVERIFY(arguments.contains(QStringLiteral("--skip")));
    QVERIFY(arguments.contains(QStringLiteral("--limit")));
    QVERIFY(arguments.contains(QStringLiteral("--status")));
    QVERIFY(arguments.contains(QStringLiteral("--status-timer")));
    QVERIFY(arguments.contains(QStringLiteral("--potfile-disable")));
    QVERIFY(arguments.contains(QStringLiteral("--hex-wordlist")));
    QVERIFY(arguments.contains(QStringLiteral("--encoding-from")));
    QVERIFY(arguments.contains(QStringLiteral("--encoding-to")));
    QVERIFY(arguments.contains(QStringLiteral("--opencl-device-types")));
    QVERIFY(arguments.contains(QStringLiteral("--hwmon-temp-abort")));
    // Straight mode has no mask; it applies the left rule but has no right
    // wordlist, so the right rule is left out.
    QVERIFY(!arguments.contains(QStringLiteral("--increment")));
    QVERIFY(arguments.contains(QStringLiteral("--rule-left")));
    QVERIFY(!arguments.contains(QStringLiteral("--rule-right")));
    QVERIFY(!arguments.contains(QStringLiteral("--outfile")));
}

void TestMainWindow::statusTimerReachesTheCommandLine()
{
    MainWindow window;

    // The spin box starts at hashcat's own interval, and the value it shows is
    // the value the command line carries.
    QCOMPARE(window.ui->spinBox_status_timer->value(), AppConstants::Defaults::StatusTimer);

    window.ui->checkBox_status->setChecked(true);

    QStringList arguments = window.generateArguments();
    QVERIFY(arguments.contains(QStringLiteral("--status")));
    QVERIFY(arguments.contains(QStringLiteral("--status-timer")));
    QCOMPARE(arguments.value(arguments.indexOf(QStringLiteral("--status-timer")) + 1),
             QString::number(AppConstants::Defaults::StatusTimer));

    // ... and the command line follows the spin box.
    window.ui->spinBox_status_timer->setValue(1);
    arguments = window.generateArguments();
    QCOMPARE(arguments.value(arguments.indexOf(QStringLiteral("--status-timer")) + 1),
             QStringLiteral("1"));
}

void TestMainWindow::skipAndLimitAcceptOnlyNumbers()
{
    MainWindow window;

    QLineEdit *skip = window.ui->lineEdit_skip;
    QLineEdit *limit = window.ui->lineEdit_limit;
    QVERIFY(skip->validator() != nullptr);
    QVERIFY(limit->validator() != nullptr);

    // Typing is what the validator guards: an invalid character is dropped.
    QTest::keyClicks(skip, QStringLiteral("12a"));
    QCOMPARE(skip->text(), QStringLiteral("12"));

    QTest::keyClicks(limit, QStringLiteral("1b2"));
    QCOMPARE(limit->text(), QStringLiteral("12"));

    // Empty stays allowed - it means "not set".
    skip->clear();
    limit->clear();
    QVERIFY(skip->text().isEmpty());
    QVERIFY(limit->text().isEmpty());
}

void TestMainWindow::checkedWordlistsEndUpInTheCommand()
{
    MainWindow window;

    const auto add = [&window](const QString &path, Qt::CheckState state) {
        auto *item = new QListWidgetItem(path, window.ui->listWidget_wordlist);
        item->setCheckState(state);
    };
    add(QStringLiteral("/tmp/a.txt"), Qt::Checked);
    add(QStringLiteral("/tmp/b.txt"), Qt::Unchecked);
    add(QStringLiteral("/tmp/c.txt"), Qt::Checked);

    const HashcatOptions options = window.collectHashcatOptions();
    const QStringList expected{QStringLiteral("/tmp/a.txt"), QStringLiteral("/tmp/c.txt")};
    QCOMPARE(options.wordlists, expected);

    const QStringList arguments = window.generateArguments();
    QVERIFY(arguments.contains(QStringLiteral("/tmp/a.txt")));
    QVERIFY(arguments.contains(QStringLiteral("/tmp/c.txt")));
    QVERIFY(!arguments.contains(QStringLiteral("/tmp/b.txt")));
}

// A wordlist can be gone without the row changing: it was deleted, or a
// restored session names a file that is no longer there. The row has to say
// so, and the path itself must survive untouched - it is what hashcat is given
// and what a session stores.
void TestMainWindow::missingWordlistsAreMarked()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString presentPath = directory.filePath(QStringLiteral("present.txt"));
    QVERIFY(writeFile(presentPath));

    const QString missingPath = directory.filePath(QStringLiteral("missing.txt"));

    // A configured path keeps the constructor from queueing the settings
    // dialog; the test spins the event loop below. The watchdog closes the
    // query's failure warning, whose modal loop would otherwise swallow it.
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("/nonexistent/hashcat"));
    m_watchdog.start();

    MainWindow window;
    const auto add = [&window](const QString &path) {
        auto *item = new QListWidgetItem(path, window.ui->listWidget_wordlist);
        item->setCheckState(Qt::Checked);
        return item;
    };
    QListWidgetItem *present = add(presentPath);
    QListWidgetItem *missing = add(missingPath);

    QVERIFY(present->icon().isNull());
    QVERIFY(present->toolTip().isEmpty());
    QVERIFY(!missing->icon().isNull());
    QVERIFY(missing->toolTip().contains(missingPath));

    // The indicator is presentation only: text, command and session keep the
    // path.
    QCOMPARE(missing->text(), missingPath);
    QCOMPARE(window.collectHashcatOptions().wordlists, (QStringList{presentPath, missingPath}));
    const QJsonObject values =
        WidgetStateSerializer::toJson(&window, {AppConstants::Files::PreviewWidget});
    const QJsonArray items = values.value(QStringLiteral("listWidget_wordlist")).toArray();
    QCOMPARE(items.size(), 2);
    QCOMPARE(items.at(1).toObject().value(QStringLiteral("text")).toString(), missingPath);

    // A file that disappears after it was added is marked; one that comes back
    // is not.
    QVERIFY(QFile::remove(presentPath));
    QTRY_VERIFY(!present->icon().isNull());
    QVERIFY(present->toolTip().contains(presentPath));

    QVERIFY(writeFile(presentPath));
    QTRY_VERIFY(present->icon().isNull());
    QVERIFY(present->toolTip().isEmpty());

    // A restored session clears the model and inserts the rows again; the
    // monitor has to attach to those rows as well.
    QJsonArray restoredWordlists;
    QJsonObject restoredWordlist;
    restoredWordlist[QStringLiteral("text")] = missingPath;
    restoredWordlist[QStringLiteral("checked")] = true;
    restoredWordlists.append(restoredWordlist);
    QJsonObject sessionValues;
    sessionValues[QStringLiteral("listWidget_wordlist")] = restoredWordlists;

    QVERIFY(window.applySessionFile(sessionValues, AppConstants::Sessions::Version, {}).isEmpty());

    QListWidget *list = window.ui->listWidget_wordlist;
    QCOMPARE(list->count(), 1);
    QListWidgetItem *restored = list->item(0);
    QCOMPARE(restored->text(), missingPath);
    QVERIFY(!restored->icon().isNull());
    QVERIFY(restored->toolTip().contains(missingPath));
}

void TestMainWindow::hashTypeComesFromTheItemData()
{
    MainWindow window;

    // The real list comes from hashcat at runtime; inserting it by hand keeps
    // this test independent of the stub below.
    window.ui->comboBox_hash_type->clear();
    window.ui->comboBox_hash_type->addItem(QStringLiteral("1000 | NTLM"), 1000);
    window.ui->comboBox_hash_type->addItem(QStringLiteral("0 | MD5"), 0);
    window.ui->comboBox_hash_type->setCurrentIndex(0);

    QCOMPARE(window.collectHashcatOptions().hashType, 1000);
    const QStringList withHashType{"--hash-type", "1000", "--attack-mode", "0"};
    QCOMPARE(window.generateArguments(), withHashType);

    // Nothing selected - the state before the query finished - must leave -m
    // out instead of silently asking for hash type 0
    window.ui->comboBox_hash_type->clear();
    QCOMPARE(window.collectHashcatOptions().hashType, -1);
    const QStringList withoutHashType{"--attack-mode", "0"};
    QCOMPARE(window.generateArguments(), withoutHashType);
}

// Unlike the tests above it spins the event loop while the async query runs;
// init()/cleanup() have taken care of the settings, so it is free to run in
// any position.
void TestMainWindow::hashTypesAreFilledFromHashcat()
{
    const QString stub = TestEnvironment::writeShellStub(
        QStringLiteral("stub-hashcat"),
        "#!/bin/sh\n"
        "printf '{\"0\":{\"name\":\"MD5\"},\"1000\":{\"name\":\"NTLM\"}}\\n'\n"
        "exit 0\n");
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    // Same contract as the helper utils tests: on Linux the stub has to exist
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    m_watchdog.start();

    MainWindow window;

    // The query runs off-thread; until the event loop delivers its result the
    // combo stays parked in the state initHashAndAttackModes() put it in.
    QVERIFY(!window.ui->comboBox_hash_type->isEnabled());
    QCOMPARE(window.ui->comboBox_hash_type->toolTip(), QStringLiteral("Updating…"));

    // Wait at least as long as the query's own deadline: giving up earlier
    // would fail the test while hashcat could still have answered
    QTRY_VERIFY_WITH_TIMEOUT(window.ui->comboBox_hash_type->count() > 0,
                             AppConstants::Hashcat::QueryTimeoutMs);

    QCOMPARE(window.ui->comboBox_hash_type->count(), 2);
    QVERIFY(window.ui->comboBox_hash_type->isEnabled());
    QVERIFY(window.ui->comboBox_hash_type->toolTip().isEmpty());

    // 0 sorts before 1000, and the ids travel as item data
    QCOMPARE(window.ui->comboBox_hash_type->itemData(0).toUInt(), 0u);
    QCOMPARE(window.ui->comboBox_hash_type->itemData(1).toUInt(), 1000u);

    const QStringList arguments = window.generateArguments();
    const qsizetype hashType = arguments.indexOf(QStringLiteral("--hash-type"));
    QVERIFY(hashType >= 0);
    QCOMPARE(arguments.value(hashType + 1), QStringLiteral("0"));
}

// Every failure path owes the user the state the success path leaves behind:
// the combo back under their control, no "Updating…" left on it, a tooltip
// that says why it is empty, and no hash type in it. Two ways for hashcat to
// not answer, both real.
void TestMainWindow::failedQueryReenablesTheHashTypeCombo_data()
{
    QTest::addColumn<QByteArray>("script");

    QTest::newRow("non-zero exit") << QByteArray("#!/bin/sh\n"
                                                 "printf 'no such option\\n' >&2\n"
                                                 "exit 1\n");
    QTest::newRow("garbage output") << QByteArray("#!/bin/sh\n"
                                                  "printf 'this is not json at all\\n'\n"
                                                  "exit 0\n");
}

void TestMainWindow::failedQueryReenablesTheHashTypeCombo()
{
    QFETCH(QByteArray, script);

    const QString stub =
        TestEnvironment::writeShellStub(QStringLiteral("stub-hashcat-fails"), script);
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    // The failure raises a modal warning; the watchdog closes it so the
    // assertions below get their turn (and the run cannot hang either way).
    m_watchdog.start();

    MainWindow window;

    // The query starts out exactly like the successful one: parked on the
    // reply, with the tooltip that says so.
    QVERIFY(!window.ui->comboBox_hash_type->isEnabled());
    QCOMPARE(window.ui->comboBox_hash_type->toolTip(), QStringLiteral("Updating…"));

    // ... and when the reply turns out to be unusable, the combo comes back
    QTRY_VERIFY_WITH_TIMEOUT(window.ui->comboBox_hash_type->isEnabled(),
                             AppConstants::Hashcat::QueryTimeoutMs);
    QCOMPARE(
        window.ui->comboBox_hash_type->toolTip(),
        QStringLiteral("The hash types could not be loaded. Check the hashcat path in Settings."));
    QCOMPARE(window.ui->comboBox_hash_type->count(), 0);
}

// The suggestion may follow the hash file name only as long as nobody else
// has put something in the outfile field: what the user typed has to survive
// every later hash file edit, and an empty field is nobody's value at all.
void TestMainWindow::outfileSuggestionFollowsUntilTheUserTakesOver()
{
    MainWindow window;
    QLineEdit *hashFile = window.ui->lineEdit_hashfile;
    QLineEdit *outfile = window.ui->lineEdit_outfile;
    QVERIFY(outfile->text().isEmpty());

    hashFile->setText(QStringLiteral("/tmp/first.hash"));
    QCOMPARE(outfile->text(), QStringLiteral("/tmp/first.hash.out"));

    // still our own suggestion, so it keeps following
    hashFile->setText(QStringLiteral("/tmp/second.hash"));
    QCOMPARE(outfile->text(), QStringLiteral("/tmp/second.hash.out"));

    // the user takes ownership - from here on the field is left alone
    outfile->setText(QStringLiteral("/tmp/mine.txt"));
    hashFile->setText(QStringLiteral("/tmp/third.hash"));
    QCOMPARE(outfile->text(), QStringLiteral("/tmp/mine.txt"));

    // an empty field is nobody's value, so suggesting starts afresh
    outfile->clear();
    hashFile->setText(QStringLiteral("/tmp/fourth.hash"));
    QCOMPARE(outfile->text(), QStringLiteral("/tmp/fourth.hash.out"));
}

// The move buttons only appear after an item was clicked, but the slots are
// still slots: called with nothing current - or with nothing there at all -
// they have to be a no-op instead of a walk through takeItem(-1).
void TestMainWindow::wordlistMoveSurvivesAMissingSelection()
{
    MainWindow window;
    QListWidget *list = window.ui->listWidget_wordlist;

    new QListWidgetItem(QStringLiteral("b.txt"), list);
    new QListWidgetItem(QStringLiteral("a.txt"), list);
    QCOMPARE(list->currentRow(), -1);

    const auto texts = [list] {
        QStringList result;
        for (int i = 0; i < list->count(); ++i) {
            result << list->item(i)->text();
        }
        return result;
    };
    const QStringList before{QStringLiteral("b.txt"), QStringLiteral("a.txt")};

    QVERIFY(QMetaObject::invokeMethod(&window, "wordlistMoveUpClicked"));
    QCOMPARE(texts(), before);
    QCOMPARE(list->currentRow(), -1);

    QVERIFY(QMetaObject::invokeMethod(&window, "wordlistMoveDownClicked"));
    QCOMPARE(texts(), before);
    QCOMPARE(list->currentRow(), -1);

    // an empty list is the same no-op
    list->clear();
    QVERIFY(QMetaObject::invokeMethod(&window, "wordlistMoveUpClicked"));
    QVERIFY(QMetaObject::invokeMethod(&window, "wordlistMoveDownClicked"));
    QCOMPARE(list->count(), 0);
}

// With a hashcat binary configured the preview is [binary][separator][args]:
// the file name, exactly one blank, the arguments - and nothing in front of
// them, a leading blank used to ride into the clipboard with the copy.
void TestMainWindow::commandPreviewCarriesTheBinaryName()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("/opt/hashcat/hashcat"));

    MainWindow window;
    QCOMPARE(window.ui->lineEdit_command->text(), QStringLiteral("hashcat --attack-mode 0"));
}

// The line edit shows the beginning of the command only; the tooltip has to
// carry all of it.
void TestMainWindow::thePreviewTooltipHoldsTheWholeCommand()
{
    MainWindow window;

    // More word lists than the window can show, and a path that looks like a
    // tag.
    for (int i = 0; i < 12; ++i) {
        auto *item = new QListWidgetItem(QStringLiteral("/tmp/wordlist-number-%1.txt").arg(i),
                                         window.ui->listWidget_wordlist);
        item->setCheckState(Qt::Checked);
    }
    window.ui->checkBox_outfile->setChecked(true);
    window.ui->lineEdit_outfile->setText(QStringLiteral("/tmp/<b>out</b>.txt"));

    const QString command = window.ui->lineEdit_command->text();
    const QString tooltip = window.ui->lineEdit_command->toolTip();

    // Every part of the command is in the tooltip. The path that looks like a
    // tag is escaped, so that it is shown as typed.
    const QStringList parts = command.split(QLatin1Char(' '));
    for (const QString &part : std::as_const(parts)) {
        QVERIFY2(tooltip.contains(part.toHtmlEscaped()),
                 qPrintable(QStringLiteral("the tooltip does not hold \"%1\"").arg(part)));
    }

    QVERIFY(tooltip.contains(QStringLiteral("&lt;b&gt;out&lt;/b&gt;.txt")));

    // A command that looks like markup becomes rich text, where a line break
    // has to be explicit.
    QVERIFY(tooltip.contains(QStringLiteral("<br>")));

    // Without markup the tooltip stays plain text and keeps its line breaks.
    window.ui->lineEdit_outfile->setText(QStringLiteral("/tmp/out.txt"));
    const QString plainTooltip = window.ui->lineEdit_command->toolTip();
    QVERIFY(plainTooltip.contains(QStringLiteral("/tmp/out.txt")));
    QVERIFY(plainTooltip.contains(QLatin1Char('\n')));
    QVERIFY(!plainTooltip.contains(QStringLiteral("<br>")));
}

// Copying puts the command on the clipboard and says that it did.
void TestMainWindow::copyingTheCommandPutsItOnTheClipboard()
{
    MainWindow window;
    window.ui->checkBox_remove->setChecked(true);

    const QString command = window.ui->lineEdit_command->text();
    QVERIFY(!command.isEmpty());

    QVERIFY(QMetaObject::invokeMethod(&window, "copyCommandToClipboard"));

    QCOMPARE(QApplication::clipboard()->text(), command);

    // The confirmation is a one-shot tooltip at the pointer.
    QVERIFY(QToolTip::text().contains(QStringLiteral("Copied")));
    QToolTip::hideText();
}

// The session machinery as the application uses it: saveSession() and
// the loadSession() every constructor runs. Pinned here, and nowhere
// else: the command preview stays out of the file, and a new window starts
// from the stored state.
void TestMainWindow::theSessionRoundTrips()
{
    // A failed save or load opens a modal warning; the watchdog must be
    // able to close it so the test fails instead of hanging until ctest's
    // timeout.
    m_watchdog.start();

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("/opt/hashcat/hashcat"));

    MainWindow writer;
    writer.ui->comboBox_attack->setCurrentIndex(2); // brute-force, not the default straight
    writer.ui->spinBox_runtime->setValue(64);       // not the default 0
    writer.saveSession();

    const QString session = writer.sessionFile();
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
    QCOMPARE(reloaded.ui->comboBox_attack->currentIndex(), 2);
    QCOMPARE(reloaded.ui->spinBox_runtime->value(), 64);

    // ...and the preview reflects the restored state, not the defaults the
    // widgets held before the session arrived.
    QCOMPARE(reloaded.ui->lineEdit_command->text(),
             QStringLiteral("hashcat --attack-mode 3 --runtime 64"));
}

// The session is stored only while it is wanted: the session of an earlier
// run goes when the setting is off, so that it cannot come back as stale
// state once the setting returns.
void TestMainWindow::turningTheSessionOffRemovesTheStoredSession()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::RestoreSession, true);

    MainWindow writer;
    writer.saveSession();
    const QString session = writer.sessionFile();
    QVERIFY(QFile::exists(session));

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::RestoreSession, false);
    writer.saveSession();

    QVERIFY(!QFile::exists(session));
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::RestoreSession, true);
}

// The constructor loads the session while the hashcat query that fills
// comboBox_hash_type is still in flight: the combo has no items, so
// setCurrentIndex() is a silent no-op and the stored id can only be
// applied when the reply delivers the items.
void TestMainWindow::sessionHashTypeSurvivesTheAsynchronousQuery()
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

    m_watchdog.start();

    MainWindow window;

    // The query runs off-thread; wait at least as long as its own deadline
    // so the test never gives up while hashcat could still have answered.
    QTRY_VERIFY_WITH_TIMEOUT(window.ui->comboBox_hash_type->count() > 0,
                             AppConstants::Hashcat::QueryTimeoutMs);

    QCOMPARE(window.ui->comboBox_hash_type->currentIndex(), 1);
    QCOMPARE(window.ui->comboBox_hash_type->currentData().toUInt(), 1000u);
    QCOMPARE(window.m_pendingHashTypeId, -1); // applied and consumed
}

// A session that names a hash type this hashcat does not know. Selecting
// another one would crack with the wrong algorithm, so nothing is selected
// instead - and the command line asks for no hash type at all.
void TestMainWindow::sessionWithAnUnknownHashTypeSelectsNothing()
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

    m_watchdog.start();

    MainWindow window;

    QTRY_VERIFY_WITH_TIMEOUT(window.ui->comboBox_hash_type->count() > 0,
                             AppConstants::Hashcat::QueryTimeoutMs);

    QCOMPARE(window.ui->comboBox_hash_type->currentIndex(), -1);
    QVERIFY(!window.generateArguments().contains(QStringLiteral("-m")));
    QVERIFY(!window.generateArguments().contains(QStringLiteral("--hash-type")));
}

// No single size fits every font, DPI scaling, style and translation: a fixed
// geometry either clips the content or leaves an empty strip. The window
// measures the content on its first show instead.
void TestMainWindow::windowFitsItsContent()
{
    // A configured path keeps the constructor from queueing the settings
    // dialog, whose modal loop would want the event loop this test spins. The
    // path need not exist: the query it starts only ever fills the hash type
    // combo, which stays empty and keeps the measured size stable.
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("/nonexistent/hashcat"));
    m_watchdog.start();

    MainWindow window;
    window.show();
    QCoreApplication::processEvents();

    QScrollArea *attackArea = window.ui->scrollArea_attack;
    QWidget *attackContent = attackArea->widget();
    QScrollArea *generalArea = window.ui->scrollArea_general;
    QVERIFY(attackContent);
    QVERIFY(generalArea->widget());
    QVERIFY(window.screen());

    // Regression: the General tab needs more width than the Attack tab, so the
    // window must take its width from the tab that is not in front at startup.
    QVERIFY(generalArea->widget()->minimumSizeHint().width()
            > attackContent->minimumSizeHint().width());

    // The size fitToContent() should have produced: the common window chrome
    // plus the largest content minimum in each direction. A tab whose content
    // is taller than the final viewport also needs room for its vertical
    // scrollbar, otherwise it would show a horizontal one as well.
    const QSize chrome = window.size() - attackArea->size();
    const QSize cap = window.screen()->availableGeometry().size() - QSize(40, 80);
    QSize contentMinimum(0, 0);
    for (int i = 0; i < window.ui->tabWidget_main->count(); ++i) {
        QScrollArea *tabArea = window.ui->tabWidget_main->widget(i)->findChild<QScrollArea *>();
        QVERIFY(tabArea);
        QVERIFY(tabArea->widget());
        contentMinimum = contentMinimum.expandedTo(tabArea->widget()->minimumSizeHint());
    }
    QSize natural = chrome + contentMinimum;
    const int viewportHeight = natural.boundedTo(cap).height() - chrome.height();
    int requiredWidth = 0;
    for (int i = 0; i < window.ui->tabWidget_main->count(); ++i) {
        QScrollArea *tabArea = window.ui->tabWidget_main->widget(i)->findChild<QScrollArea *>();
        const QSize minimum = tabArea->widget()->minimumSizeHint();
        int width = minimum.width();
        if (minimum.height() > viewportHeight) {
            width += tabArea->verticalScrollBar()->sizeHint().width();
        }
        requiredWidth = qMax(requiredWidth, width);
    }
    natural.setWidth(chrome.width() + requiredWidth);
    const QSize target = natural.boundedTo(cap);

    // The window has to be big enough for the content: a scrollbar for
    // content that fits is the bug this guards. Checked before the exact
    // size, so that a failure says what went wrong.
    if (natural.height() <= cap.height()) {
        QTRY_VERIFY(!attackArea->verticalScrollBar()->isVisible());
    }

    // QTRY because a resize inside showEvent() needs the event loop to settle
    // the child layouts before the final scrollbar state is visible.
    QTRY_COMPARE(window.size(), target);

    // Both tabs fit horizontally. Only the height may fall back to a scrollbar
    // when the screen is too small for the tallest tab.
    if (target.width() == natural.width()) {
        for (int i = 0; i < window.ui->tabWidget_main->count(); ++i) {
            window.ui->tabWidget_main->setCurrentIndex(i);
            QCoreApplication::processEvents();
            QScrollArea *tabArea = window.ui->tabWidget_main->widget(i)->findChild<QScrollArea *>();
            QTRY_VERIFY(!tabArea->horizontalScrollBar()->isVisible());
        }
    }

    // The fit uses the same chrome for every tab and must not depend on which
    // one is in front.
    window.ui->tabWidget_main->setCurrentIndex(1);
    QCoreApplication::processEvents();
    window.fitToContent();
    QCoreApplication::processEvents();
    QCOMPARE(window.size(), target);
    window.ui->tabWidget_main->setCurrentIndex(0);
    QCoreApplication::processEvents();

    if (natural.height() <= cap.height()) {
        // The content fits the screen: no scrollbar, and no empty strip below
        // it - the content is exactly as tall as it needs to be.
        QTRY_VERIFY(!attackArea->verticalScrollBar()->isVisible());
        QVERIFY(attackContent->height() - attackContent->minimumSizeHint().height() < 30);
    } else {
        // The content is taller than the screen: the window stays on-screen
        // and the scroll area has to show its scrollbar.
        QTRY_VERIFY(attackArea->verticalScrollBar()->isVisible());
    }

    // Regression: the hint under the mode selector must not wrap. A wrapped
    // label asks its layout for more height than its size hint announced, so
    // the window was sized for less than the content needed and the scroll
    // area showed a scrollbar for content that fitted.
    QVERIFY(!window.ui->label_mode_hint->wordWrap());
}

// The command bar belongs to the window, not to one tab: switching tabs must
// not take Start, Copy or the preview away.
void TestMainWindow::commandBarStaysVisibleOnEveryTab()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("/nonexistent/hashcat"));
    m_watchdog.start();

    MainWindow window;
    window.show();
    QCoreApplication::processEvents();

    QPushButton *execute = window.ui->pushButton_start;
    QPushButton *copy = window.ui->pushButton_copy_clipboard;
    QLineEdit *preview = window.ui->lineEdit_command;

    // The row lives below the tabs, not in the first tab's scroll content.
    QVERIFY(window.centralWidget()->isAncestorOf(execute));
    QVERIFY(!window.ui->tabWidget_main->isAncestorOf(execute));
    QVERIFY(!window.ui->scrollArea_attack->isAncestorOf(execute));
    QVERIFY(!window.ui->tabWidget_main->isAncestorOf(copy));
    QVERIFY(!window.ui->tabWidget_main->isAncestorOf(preview));

    QVERIFY(execute->isVisible());
    QVERIFY(copy->isVisible());
    QVERIFY(preview->isVisible());

    // ... and the row stays on screen on the second tab as well.
    window.ui->tabWidget_main->setCurrentWidget(window.ui->tab_general);
    QCoreApplication::processEvents();

    QVERIFY(execute->isVisible());
    QVERIFY(copy->isVisible());
    QVERIFY(preview->isVisible());
}

// The settings leave the application through the File menu, like the session:
// the file holds the settings that are in effect, in the shared format.
void TestMainWindow::theSettingsAreExportedThroughTheFileMenu()
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
void TestMainWindow::theSettingsAreImportedThroughTheFileMenu()
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
    const QString preview = window.ui->lineEdit_command->text();
    QVERIFY2(preview.startsWith(QStringLiteral("hashcat-from-the-file")), qPrintable(preview));
    QVERIFY2(preview.contains(QStringLiteral(" -a 0")), qPrintable(preview));
}

// A session file that names a widget this version does not know - it was
// made by another version, or it was edited by hand. The values that are
// known are applied all the same, and the ones that are not are named.
void TestMainWindow::theImportNamesTheValuesTheSessionDoesNotKnow()
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
    QCOMPARE(window.ui->spinBox_runtime->value(), 64);
    // ... and the one that named nothing is named in the notes.
    QVERIFY(recorder.messageBoxSeen);
    QVERIFY(recorder.text.contains(QStringLiteral("aWidgetFromTheFuture")));
}

// A file with a newer format version is not applied at all: a value whose
// name survived may mean something else there. The note names the version.
void TestMainWindow::aNewerSessionIsRefused()
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
    QVERIFY(window.ui->spinBox_runtime->value() != 64);
}

// A file an older version wrote is applied, and the note says why a value it
// named may be missing.
void TestMainWindow::anOlderSessionIsAppliedWithANote()
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

    QCOMPARE(window.ui->spinBox_runtime->value(), 64);
    QVERIFY(recorder.messageBoxSeen);
    QCOMPARE(recorder.title, QStringLiteral("Imported"));
    QVERIFY(recorder.text.contains(QStringLiteral("older version")));
}

// A settings file of a newer format version is refused, like a session file:
// the keys it holds may mean something else there.
void TestMainWindow::aNewerSettingsFileIsRefused()
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
void TestMainWindow::anOlderStartupSessionIsAppliedWithANote()
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
    QCOMPARE(window.ui->spinBox_runtime->value(), 64);
    // ... and the note says that the file was made by an older version.
    QVERIFY(recorder.messageBoxSeen);
    QCOMPARE(recorder.title, QStringLiteral("Session warning"));
    QVERIFY(recorder.text.contains(QStringLiteral("older version")));

    // The exit writes the file with the current version.
    window.saveSession();

    QJsonObject written;
    int version = 0;
    QStringList otherSections;
    QVERIFY(StateFile::read(session, AppConstants::FileSections::Session, &written, &version,
                            &otherSections, nullptr));
    QCOMPARE(version, AppConstants::Sessions::Version);
}

// The session the application restores at startup follows the same rule, and
// the file of the newer format version is left in place for that version.
void TestMainWindow::aNewerStartupSessionIsLeftAlone()
{
    const QString session = StateFile::sessionFilePath();
    QVERIFY(QFileInfo(session).dir().mkpath(QStringLiteral(".")));

    QJsonObject values;
    values.insert(QStringLiteral("spinBox_runtime"), 64);
    QVERIFY(StateFile::write(session, AppConstants::FileSections::Session,
                             AppConstants::Sessions::Version + 1, values, nullptr));

    m_watchdog.start();
    MainWindow window;

    QCOMPARE(m_closedModals, 1);
    QVERIFY(window.ui->spinBox_runtime->value() != 64);
    QVERIFY(QFile::exists(session));
}

// The session of a newer version survives the exit: it is not written over,
// and it is not deleted - not even when the session is switched off, which
// is the case where the file is never read at the startup.
void TestMainWindow::aNewerSessionSurvivesTheExit()
{
    const QString session = StateFile::sessionFilePath();
    QVERIFY(QFileInfo(session).dir().mkpath(QStringLiteral(".")));

    QJsonObject values;
    values.insert(QStringLiteral("spinBox_runtime"), 64);
    QVERIFY(StateFile::write(session, AppConstants::FileSections::Session,
                             AppConstants::Sessions::Version + 1, values, nullptr));
    const QJsonObject before = readJsonObject(session);

    m_watchdog.start();
    MainWindow window;
    QCOMPARE(m_closedModals, 1);

    // The session is on: the file of the newer version is not written over.
    window.saveSession();
    QCOMPARE(readJsonObject(session), before);

    // The session is off: the file is not deleted either.
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::RestoreSession, false);
    window.saveSession();
    QCOMPARE(readJsonObject(session), before);
    QVERIFY(QFile::exists(session));
}

// The warning of a newer session says what the user loses: the session is not
// restored, and the session of this run is not stored either.
void TestMainWindow::theNewerSessionWarningSaysWhatIsLost()
{
    MainWindow window;

    const QString note = window.newerSessionNote(AppConstants::Sessions::Version + 1);
    QVERIFY(note.contains(QStringLiteral("not restored")));
    QVERIFY(note.contains(QStringLiteral("not stored")));
}

// A session file this version cannot read is not thrown away: it may belong
// to a version that can read it, or the user may want to look at it.
void TestMainWindow::aSessionFileThatCannotBeReadIsLeftAlone()
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

    m_watchdog.start();
    MainWindow window;

    // The startup says why the session did not come back...
    QCOMPARE(m_closedModals, 1);
    // ... and the file is still there.
    QVERIFY(QFile::exists(session));

    // The exit neither writes over it nor deletes it - not with the session
    // on, and not with it off.
    window.saveSession();
    QCOMPARE(readJsonObject(session), before);

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::RestoreSession, false);
    window.saveSession();
    QCOMPARE(readJsonObject(session), before);
    QVERIFY(QFile::exists(session));
}

// A file of a shape this version does not know is left alone as well: it may
// be a file of a version that comes later.
void TestMainWindow::aSessionFileOfAnotherShapeIsLeftAlone()
{
    const QString session = StateFile::sessionFilePath();
    QVERIFY(QFileInfo(session).dir().mkpath(QStringLiteral(".")));

    QFile file(session);
    QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Truncate));
    file.write(R"({"sections": {"somethingElse": {"version": 1, "values": {}}}})");
    file.close();
    const QJsonObject before = readJsonObject(session);

    m_watchdog.start();
    MainWindow window;

    QCOMPARE(m_closedModals, 1);
    QVERIFY(QFile::exists(session));

    window.saveSession();
    QCOMPARE(readJsonObject(session), before);
    QVERIFY(QFile::exists(session));
}

// The startup must be quiet. A session the application wrote itself holds
// nothing it does not know, so no box may open - an empty one would be the
// worst kind of noise.
void TestMainWindow::theStartupShowsNoBox()
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
    writer.ui->spinBox_runtime->setValue(64);
    writer.saveSession();

    m_watchdog.start();
    MainWindow reloaded;

    // The window opened without a box...
    QCOMPARE(m_closedModals, 0);
    // ... and the session was restored.
    QCOMPARE(reloaded.ui->spinBox_runtime->value(), 64);
}

int main(int argc, char *argv[])
{
    TestMainWindow test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_mainwindow.moc"
