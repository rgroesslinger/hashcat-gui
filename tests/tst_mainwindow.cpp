/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QDir>
#include <QFile>
#include <QClipboard>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
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

// The profile file a serializer wrote, as the object it holds.
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

} // namespace

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
        connect(&m_watchdog, &QTimer::timeout, this, [] {
            if (QWidget *modal = QApplication::activeModalWidget()) {
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
    void resetFieldsRestoresEveryWidget();
    void optionDependenciesFollowTheirCheckboxes();
    void modeHintFollowsTheAttackMode();
    void launchValidationFollowsTheWidgets();
    void widgetsFlowIntoTheOptions();
    void statusTimerReachesTheCommandLine();
    void skipAndLimitAcceptOnlyNumbers();
    void checkedWordlistsEndUpInTheCommand();
    void hashTypeComesFromTheItemData();
    void hashTypesAreFilledFromHashcat();
    void failedQueryReenablesTheHashTypeCombo_data();
    void failedQueryReenablesTheHashTypeCombo();
    void outfileSuggestionFollowsUntilTheUserTakesOver();
    void wordlistSortSurvivesAMissingSelection();
    void commandPreviewCarriesTheBinaryName();
    void thePreviewTooltipHoldsTheWholeCommand();
    void copyingTheCommandPutsItOnTheClipboard();
    void defaultProfileRoundTrips();
    void profileHashTypeSurvivesTheAsynchronousQuery();
    void profileWithAnUnknownHashTypeSelectsNothing();
    void windowFitsItsContent();
    void commandBarStaysVisibleOnEveryTab();
    void theSettingsAreExportedThroughTheFileMenu();
    void theSettingsAreImportedThroughTheFileMenu();
    void theImportNamesTheValuesTheSessionDoesNotKnow();

private:
    QString m_savedHashcatPath;
    QString m_savedTerminal;
    bool m_savedShortParameters = false;
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

    // A deterministic starting point: no configured hashcat, no configured
    // terminal, long parameters. Everything else comes from the fresh XDG
    // directories, which hold no profile to load.
    settings.setKey(AppConstants::SettingsKeys::HashcatPath, QString());
    settings.setKey(AppConstants::SettingsKeys::Terminal, QString());
    settings.setKey(AppConstants::SettingsKeys::UseShortParameters, false);
}

void TestMainWindow::cleanup()
{
    m_watchdog.stop();

    // The round-trip test writes a profile into the redirected data
    // directory, and that directory lives for the whole run: every later
    // window would silently load whatever was left there.
    QFile::remove(QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))
                      .filePath(AppConstants::Files::DefaultProfile));

    auto &settings = SettingsManager::instance();
    settings.setKey(AppConstants::SettingsKeys::HashcatPath, m_savedHashcatPath);
    settings.setKey(AppConstants::SettingsKeys::Terminal, m_savedTerminal);
    settings.setKey(AppConstants::SettingsKeys::UseShortParameters, m_savedShortParameters);
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

    window.ui->spinBox_segment_size->setValue(64);
    QVERIFY(preview->text().contains(QStringLiteral("--segment-size 64")));

    window.ui->lineEdit_session->setText(QStringLiteral("job"));
    QVERIFY(preview->text().contains(QStringLiteral("--session job")));

    window.ui->checkBox_force->setChecked(true);
    QVERIFY(preview->text().contains(QStringLiteral("--force")));

    window.ui->spinBox_runtime->setValue(60);
    QVERIFY(preview->text().contains(QStringLiteral("--runtime 60")));

    window.ui->checkBox_override_workload_profile->setChecked(true);
    window.ui->comboBox_workload_profile->setCurrentText(QStringLiteral("2"));
    QVERIFY(preview->text().contains(QStringLiteral("--workload-profile 2")));

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
// default.
//
// The test does not know the widgets: it changes every one of them, resets
// the form and compares the whole state with a fresh window. A widget the
// reset forgets - now or after it is added - shows up as a difference, and
// the list of widgets does not have to be kept in two places.
void TestMainWindow::resetFieldsRestoresEveryWidget()
{
    MainWindow fresh;
    MainWindow window;

    changeEveryValue(&window);

    QVERIFY(QMetaObject::invokeMethod(&window, "resetFieldsTriggered"));

    QString error;
    const QString freshFile = TestEnvironment::configHome().filePath(QStringLiteral("fresh.json"));
    const QString resetFile = TestEnvironment::configHome().filePath(QStringLiteral("reset.json"));
    const QStringList ignored{AppConstants::Files::PreviewWidget};
    QVERIFY2(StateFile::write(freshFile, AppConstants::FileSections::Session,
                              AppConstants::Profiles::Version,
                              WidgetStateSerializer::toJson(&fresh, ignored), &error),
             qPrintable(error));
    QVERIFY2(StateFile::write(resetFile, AppConstants::FileSections::Session,
                              AppConstants::Profiles::Version,
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
        {AttackMode::Straight, QStringLiteral("This mode needs at least one word list.")},
        {AttackMode::Combination,
         QStringLiteral("This mode needs exactly 2 word lists (left, right).")},
        {AttackMode::BruteForce, QStringLiteral("This mode needs a mask.")},
        {AttackMode::HybridWordMask, QStringLiteral("This mode needs a word list and a mask.")},
        {AttackMode::HybridMaskWord, QStringLiteral("This mode needs a word list and a mask.")},
        {AttackMode::Association, QStringLiteral("This mode needs at least one word list.")},
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
    window.ui->checkBox_override_workload_profile->setChecked(true);
    window.ui->comboBox_workload_profile->setCurrentText(QStringLiteral("2"));
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
    QCOMPARE(options.workloadProfile, QStringLiteral("2"));
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
    QCOMPARE(options.segmentSize, int(AppConstants::Defaults::SegmentSize));

    // The suggested outfile never switches output on by itself
    QVERIFY(!options.outfileEnabled);

    const QStringList arguments = window.generateArguments();
    QVERIFY(arguments.contains(QStringLiteral("--remove")));
    QVERIFY(arguments.contains(QStringLiteral("--username")));
    QVERIFY(arguments.contains(QStringLiteral("--speed-only")));
    QVERIFY(arguments.contains(QStringLiteral("--workload-profile")));
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
    QVERIFY(!arguments.contains(QStringLiteral("--segment-size")));
    QVERIFY(!arguments.contains(QStringLiteral("--outfile")));

    // ... and a segment size that is no longer the default does show up
    window.ui->spinBox_segment_size->setValue(64);
    QCOMPARE(window.collectHashcatOptions().segmentSize, 64);
    QVERIFY(window.generateArguments().contains(QStringLiteral("--segment-size")));
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
    QCOMPARE(window.ui->comboBox_hash_type->toolTip(), QStringLiteral("Updating..."));

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
// the combo back under their control, no "Updating..." left on it, and no
// hash type in it. Two ways for hashcat to not answer, both real.
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
    QCOMPARE(window.ui->comboBox_hash_type->toolTip(), QStringLiteral("Updating..."));

    // ... and when the reply turns out to be unusable, the combo comes back
    QTRY_VERIFY_WITH_TIMEOUT(window.ui->comboBox_hash_type->isEnabled(),
                             AppConstants::Hashcat::QueryTimeoutMs);
    QVERIFY(window.ui->comboBox_hash_type->toolTip().isEmpty());
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

// The sort buttons only appear after an item was clicked, but the slots are
// still slots: called with nothing current - or with nothing there at all -
// they have to be a no-op instead of a walk through takeItem(-1).
void TestMainWindow::wordlistSortSurvivesAMissingSelection()
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

    QVERIFY(QMetaObject::invokeMethod(&window, "wordlistSortAscClicked"));
    QCOMPARE(texts(), before);
    QCOMPARE(list->currentRow(), -1);

    QVERIFY(QMetaObject::invokeMethod(&window, "wordlistSortDescClicked"));
    QCOMPARE(texts(), before);
    QCOMPARE(list->currentRow(), -1);

    // an empty list is the same no-op
    list->clear();
    QVERIFY(QMetaObject::invokeMethod(&window, "wordlistSortAscClicked"));
    QVERIFY(QMetaObject::invokeMethod(&window, "wordlistSortDescClicked"));
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

    // Every part of the command is in the tooltip.
    const QStringList parts = command.split(QLatin1Char(' '));
    for (const QString &part : std::as_const(parts)) {
        QVERIFY2(tooltip.contains(part),
                 qPrintable(QStringLiteral("the tooltip does not hold \"%1\"").arg(part)));
    }

    // The tooltip is plain text: the command is in it as it is.
    QVERIFY(tooltip.contains(QStringLiteral("/tmp/<b>out</b>.txt")));

    // A command this long is broken into several lines.
    QVERIFY(tooltip.contains(QLatin1Char('\n')));
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

// The profile machinery as the application uses it: saveDefaultProfile() and
// the loadDefaultProfile() every constructor runs. Pinned here, and nowhere
// else: the command preview stays out of the file, and a new window starts
// from the stored state.
void TestMainWindow::defaultProfileRoundTrips()
{
    // A failed save or load opens a modal warning; the watchdog must be
    // able to close it so the test fails instead of hanging until ctest's
    // timeout.
    m_watchdog.start();

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("/opt/hashcat/hashcat"));

    MainWindow writer;
    writer.ui->comboBox_attack->setCurrentIndex(2); // brute-force, not the default straight
    writer.ui->spinBox_segment_size->setValue(64);  // not the default 32
    writer.saveDefaultProfile();

    const QString profile = writer.defaultProfileFile();
    QVERIFY(QFile::exists(profile));

    // What gets stored: the values of the session, and without the command
    // preview - that one is regenerated from the state on load.
    QFile file(profile);
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
    // The attack mode is stored by its id, not by its position: the ids are
    // 0, 1, 3, 6, 7 and 9, and brute-force - the third entry - is id 3.
    QCOMPARE(state.value(QStringLiteral("comboBox_attack")).toInt(), 3);

    // A new window runs loadDefaultProfile() in its constructor...
    MainWindow reloaded;
    QCOMPARE(reloaded.ui->comboBox_attack->currentIndex(), 2);
    QCOMPARE(reloaded.ui->spinBox_segment_size->value(), 64);

    // ...and the preview reflects the restored state, not the defaults the
    // widgets held before the profile arrived.
    QCOMPARE(reloaded.ui->lineEdit_command->text(),
             QStringLiteral("hashcat --attack-mode 3 --segment-size 64"));
}

// The constructor loads the profile while the hashcat query that fills
// comboBox_hash_type is still in flight: the combo has no items, so
// setCurrentIndex() is a silent no-op and the stored id can only be
// applied when the reply delivers the items.
void TestMainWindow::profileHashTypeSurvivesTheAsynchronousQuery()
{
    const QString stub = TestEnvironment::writeShellStub(
        QStringLiteral("stub-hashcat-profile"),
        "#!/bin/sh\n"
        "printf '{\"0\":{\"name\":\"MD5\"},\"1000\":{\"name\":\"NTLM\"}}\\n'\n"
        "exit 0\n");
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    // A profile that names the hash type 1000: the ids sort 0, 1000, so the
    // selection must land on NTLM and not on the first item the fill leaves.
    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QVERIFY(QDir().mkpath(dir));
    QFile profile(QDir(dir).filePath(AppConstants::Files::DefaultProfile));
    QVERIFY(profile.open(QIODevice::WriteOnly | QIODevice::Truncate));
    profile.write(R"({"createdWith": "hashcat-gui 0.8.0", "sections": {"session": )"
                  R"({"version": 1, "values": {"comboBox_hash_type": 1000}}}})");
    profile.close();

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

// A profile that names a hash type this hashcat does not know. Selecting
// another one would crack with the wrong algorithm, so nothing is selected
// instead - and the command line asks for no hash type at all.
void TestMainWindow::profileWithAnUnknownHashTypeSelectsNothing()
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
    QFile profile(QDir(dir).filePath(AppConstants::Files::DefaultProfile));
    QVERIFY(profile.open(QIODevice::WriteOnly | QIODevice::Truncate));
    profile.write(R"({"createdWith": "hashcat-gui 0.8.0", "sections": {"session": )"
                  R"({"version": 1, "values": {"comboBox_hash_type": 9999}}}})");
    profile.close();

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

// The settings leave the application through the File menu, like the profile:
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
    values.insert(QStringLiteral("hashcatPath"), QStringLiteral("/imported/hashcat"));
    QVERIFY(StateFile::write(file, AppConstants::FileSections::Settings,
                             AppConstants::Settings::Version, values, nullptr));

    // A configured hashcat, so that the window does not queue the settings
    // dialog of its own.
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("/opt/somewhere/hashcat"));

    MainWindow window;
    TestEnvironment::ModalRecorder recorder;
    recorder.pickFile = file;
    recorder.start();

    // The dialog the import opens is closed by the recorder; the test is
    // about the file that was read and the notes that came with it.
    QVERIFY(QMetaObject::invokeMethod(&window, "importSettingsTriggered"));
    QVERIFY(recorder.messageBoxSeen);
    QCOMPARE(recorder.title, QStringLiteral("Imported"));
}

// A session file that names a widget this version does not know - it was
// made by another version, or it was edited by hand. The values that are
// known are applied all the same, and the ones that are not are named.
void TestMainWindow::theImportNamesTheValuesTheSessionDoesNotKnow()
{
    const QString file = TestEnvironment::configHome().filePath(QStringLiteral("session.json"));

    QJsonObject values;
    values.insert(QStringLiteral("spinBox_segment_size"), 64);
    values.insert(QStringLiteral("aWidgetFromTheFuture"), QStringLiteral("something"));
    QVERIFY(StateFile::write(file, AppConstants::FileSections::Session,
                             AppConstants::Profiles::Version, values, nullptr));

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("/opt/somewhere/hashcat"));

    MainWindow window;
    TestEnvironment::ModalRecorder recorder;
    recorder.pickFile = file;
    recorder.start();

    QVERIFY(QMetaObject::invokeMethod(&window, "importTriggered"));

    // The value that named a widget was applied...
    QCOMPARE(window.ui->spinBox_segment_size->value(), 64);
    // ... and the one that named nothing is named in the notes.
    QVERIFY(recorder.messageBoxSeen);
    QVERIFY(recorder.text.contains(QStringLiteral("aWidgetFromTheFuture")));
}

int main(int argc, char *argv[])
{
    TestMainWindow test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_mainwindow.moc"
