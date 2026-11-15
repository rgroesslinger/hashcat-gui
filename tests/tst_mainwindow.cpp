/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appconstants.h"
#include "checkeditemlist.h"
#include "commandbuilder.h"
#include "examples.h"
#include "hashcatoptions.h"
#include "mainwindow.h"
#include "mainwindowtestaccess.h"
#include "mainwindowtesthelpers.h"
#include "settingsmanager.h"
#include "statefile.h"
#include "statewidgets.h"
#include "testenvironment.h"
#include "ui_mainwindow.h"
#include "widgetstateserializer.h"

#include <QAction>
#include <QButtonGroup>
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
#include <utility>

// The widget half of the main window: defaults, gating, lists, examples
// and the hash-type query.
class TestMainWindow : public QObject
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
    void freshWindowMatchesTheDefaultOptions();
    void hashTypeComboIsSearchable();
    void attackModeIdsTravelAsItemData();
    void widgetsFollowTheBuilderPredicates();
    void exampleSessionsCoverEveryAttackMode();
    void exampleSessionFilesAreValid();
    void loadExampleSessionNeedsAConfirmation();
    void loadExampleSessionAppliesTheBundledSession();
    void loadExampleSessionUnpacksItsDataFiles();
    void optionDependenciesFollowTheirCheckboxes();
    void outfileJsonTakesOverFromTheFormatField();
    void modeHintFollowsTheAttackMode();
    void hashRecipeRowFollowsTheHashType();
    void widgetsFlowIntoTheOptions();
    void statusTimerReachesTheCommandLine();
    void skipAndLimitAcceptOnlyNumbers();
    void deviceTypesAcceptOnlyTheSupportedOnes();
    void checkedWordlistsEndUpInTheCommand();
    void missingWordlistsAreMarked();
    void addingPathsKeepsTheListClean();
    void pcfgRulesetNameIsAddedFromTheField();
    void folderPathsAreMarked();
    void hashTypeComesFromTheItemData();
    void hashTypesAreFilledFromHashcat();
    void failedQueryReenablesTheHashTypeCombo_data();
    void failedQueryReenablesTheHashTypeCombo();
    void changingTheBinaryReRunsTheQuery();
    void staleQueryDoesNotReplaceTheNewerList_data();
    void staleQueryDoesNotReplaceTheNewerList();
    void outfileSuggestionFollowsUntilTheUserTakesOver();
    void wordlistMoveSurvivesAMissingSelection();
    void theDefaultsMatchTheUiFiles();
    void switchingTheAttackModeRebuildsThePreviewOnce();
    void destroyingTheWindowWithListRowsIsSafe();
    void everyCommandWidgetReachesThePreview();
    void listRowEventsRebuildThePreviewOnce();
    void changingTheBinaryKeepsTheAttackMode();
    void importingSettingsReRunsTheQueryWhenTheBinaryChanged();
    void aFailedReloadClearsTheHashTypes();
    void retryingTheSamePathAfterAFailureReQueries();
    void aReloadRebuildsThePreviewOnce();

private:
    MainWindowFixture m_fixture;
};

void TestMainWindow::freshWindowMatchesTheDefaultOptions()
{
    MainWindow window;

    // Ground truth spelled out: comparing build() against build() would let a
    // builder that emits nothing pass every run
    const QStringList expected{"--attack-mode", "0"};

    QCOMPARE(CommandBuilder::build(HashcatOptions{}), expected);
    QCOMPARE(MainWindowTestAccess::generateArguments(window), expected);

    // ... and that is what the preview shows, character for character
    QCOMPARE(MainWindowTestAccess::ui(window)->lineEdit_command->text(),
             expected.join(QLatin1Char(' ')));
}

void TestMainWindow::hashTypeComboIsSearchable()
{
    MainWindow window;

    // The constructor makes the field editable and gives it a completer.
    QVERIFY(MainWindowTestAccess::ui(window)->comboBox_hash_type->isEditable());
    QCompleter *completer = MainWindowTestAccess::ui(window)->comboBox_hash_type->completer();
    if (completer == nullptr) {
        QFAIL("the hash type combo has no completer");
    }

    // The list arrives asynchronously and is refilled as hashcat changes, so
    // the completer is bound to the combo's own model, not a snapshot taken
    // at construction: it has to follow every later clear()/addItem().
    QVERIFY(completer->model() == MainWindowTestAccess::ui(window)->comboBox_hash_type->model());

    // A hundred entries that differ only in case make case-insensitivity the
    // whole feature.
    QCOMPARE(completer->caseSensitivity(), Qt::CaseInsensitive);

    // The completer's popup does not realize its filter under the offscreen
    // platform used by the tests, so the filtered match itself cannot be
    // observed here; the configuration above is what the feature depends on.
}

void TestMainWindow::attackModeIdsTravelAsItemData()
{
    MainWindow window;

    const QList<int> expected{0, 1, 3, 4, 5, 6, 7, 8, 9, 12};
    QList<int> actual;
    for (int i = 0; i < MainWindowTestAccess::ui(window)->comboBox_attack->count(); ++i) {
        actual << MainWindowTestAccess::ui(window)->comboBox_attack->itemData(i).toInt();
    }
    QCOMPARE(actual, expected);

    // The displayed text is what a translation replaces; the id is what has
    // to reach -a.
    for (int i = 0; i < MainWindowTestAccess::ui(window)->comboBox_attack->count(); ++i) {
        MainWindowTestAccess::ui(window)->comboBox_attack->setCurrentIndex(i);
        QCOMPARE(int(MainWindowTestAccess::collectHashcatOptions(window).attackMode),
                 expected.at(i));
        QVERIFY(MainWindowTestAccess::ui(window)->comboBox_attack->itemData(i).isValid());
    }
}

// Which widgets each mode shows. The predicates are the same ones the
// builder emits with, so what the user sees and what reaches the command
// line can never drift apart; the same table tst_commandbuilder::predicates
// pins for the builder.
//
// The mode-specific groups are hidden, not disabled: a disabled group still
// takes its share of the tab, so the window would keep the height of its
// busiest mode in every other one. A hidden widget reports isHidden(); the
// window itself is never shown here, so isVisible() would answer for the
// window, not for the gating.
void TestMainWindow::widgetsFollowTheBuilderPredicates()
{
    MainWindow window;

    struct Expectation {
        AttackMode mode;
        bool mask;
        bool increment;
        bool wordlists;
        bool rules;
        bool ruleLeft;
        bool ruleRight;
        bool customCharset;
        bool hexWordlist;
        bool wordlistEncoding;
    };
    const QList<Expectation> expectations{
        // mode                mask   incr   words  rules  left   right  charset hexwl  enc
        {AttackMode::Straight, false, false, true, true, true, false, false, true, true},
        {AttackMode::Combination, false, false, true, true, true, true, false, true, true},
        {AttackMode::BruteForce, true, true, false, true, false, false, true, false, false},
        {AttackMode::Pcfg, false, false, false, true, false, false, true, false, false},
        {AttackMode::Table, false, false, true, true, false, false, false, true, true},
        {AttackMode::HybridWordMask, true, true, true, true, true, false, true, true, true},
        {AttackMode::HybridMaskWord, true, true, true, true, true, true, true, true, true},
        {AttackMode::Generic, false, false, false, true, true, false, false, true, true},
        {AttackMode::Association, false, false, true, true, true, false, false, true, true},
        {AttackMode::Hybrid, true, false, true, true, true, true, true, true, true},
    };

    const auto shown = [](QWidget *widget) { return !widget->isHidden(); };

    for (int i = 0; i < MainWindowTestAccess::ui(window)->comboBox_attack->count(); ++i) {
        // -1 first, so that every iteration really changes the index and runs
        // attackIndexChanged()
        MainWindowTestAccess::ui(window)->comboBox_attack->setCurrentIndex(-1);
        MainWindowTestAccess::ui(window)->comboBox_attack->setCurrentIndex(i);

        const AttackMode mode = static_cast<AttackMode>(
            MainWindowTestAccess::ui(window)->comboBox_attack->currentData().toInt());

        const Expectation *expected = nullptr;
        for (const Expectation &candidate : expectations) {
            if (candidate.mode == mode) {
                expected = &candidate;
            }
        }
        QVERIFY2(expected, "no expectation written down for this attack mode");

        QCOMPARE(shown(MainWindowTestAccess::ui(window)->groupBox_wordlists), expected->wordlists);
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->groupBox_rules), expected->rules);
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->groupBox_wordlist_rules),
                 expected->ruleLeft || expected->ruleRight);
        // The rule fields stay enabled-gated: they live inside a group that
        // the mode may share with the other rule.
        QCOMPARE(MainWindowTestAccess::ui(window)->label_rule_left->isEnabled(),
                 expected->ruleLeft);
        QCOMPARE(MainWindowTestAccess::ui(window)->lineEdit_rule_left->isEnabled(),
                 expected->ruleLeft);
        QCOMPARE(MainWindowTestAccess::ui(window)->label_rule_right->isEnabled(),
                 expected->ruleRight);
        QCOMPARE(MainWindowTestAccess::ui(window)->lineEdit_rule_right->isEnabled(),
                 expected->ruleRight);
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->groupBox_mask), expected->mask);
        // The hex-charset checkbox lives inside the charset group and takes
        // its visibility from it: same predicate, so the group carries it.
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->groupBox_custom_charset),
                 expected->customCharset);

        // The feed modes get a group each; mode 9 gets one extra row: the
        // settings line that bounds a run whose words come from the hash file.
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->groupBox_pcfg), mode == AttackMode::Pcfg);
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->groupBox_table),
                 mode == AttackMode::Table);
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->groupBox_feed),
                 mode == AttackMode::Generic);
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->label_association_settings),
                 mode == AttackMode::Association);
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->lineEdit_association_settings),
                 mode == AttackMode::Association);

        // Inside the wordlist group: the options only the modes that read a
        // wordlist (and that hashcat lets name an encoding) take.
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->checkBox_hex_wordlist),
                 expected->hexWordlist);
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->label_encoding_from),
                 expected->wordlistEncoding);
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->lineEdit_encoding_from),
                 expected->wordlistEncoding);
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->label_encoding_to),
                 expected->wordlistEncoding);
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->lineEdit_encoding_to),
                 expected->wordlistEncoding);

        // Inside the mask group: hashcat 8.0.0 refuses -i in mode 12.
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->checkBox_increment), expected->increment);
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->label_increment_min), expected->increment);
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->spinBox_increment_min),
                 expected->increment);
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->label_increment_max), expected->increment);
        QCOMPARE(shown(MainWindowTestAccess::ui(window)->spinBox_increment_max),
                 expected->increment);
    }
}

// Every supported attack mode has exactly one example, and the examples do not
// repeat a hash type or a password: the set is meant to exercise the modes,
// not to test the same hash ten times.
void TestMainWindow::exampleSessionsCoverEveryAttackMode()
{
    MainWindow window;

    int modeCount = 0;
    for (const AttackMode mode : AllAttackModes) {
        ++modeCount;
        int examples = 0;
        for (const Examples::Entry &entry : Examples::All) {
            if (entry.attackMode == mode) {
                ++examples;
            }
        }
        QVERIFY2(examples == 1, qPrintable(QStringLiteral("mode %1 has %2 examples")
                                               .arg(static_cast<int>(mode))
                                               .arg(examples)));

        QAction *action = exampleAction(window, mode);
        QVERIFY2(
            action,
            qPrintable(QStringLiteral("mode %1 has no menu entry").arg(static_cast<int>(mode))));
        QCOMPARE(action->data().toInt(), static_cast<int>(mode));
    }

    QCOMPARE(MainWindowTestAccess::ui(window)->menuLoadExampleSession->actions().size(), modeCount);

    QSet<int> hashTypes;
    QSet<QString> passwords;
    for (const Examples::Entry &entry : Examples::All) {
        QVERIFY2(QFile::exists(QString(entry.resource)), qPrintable(QString(entry.resource)));
        QVERIFY2(!hashTypes.contains(entry.hashType),
                 qPrintable(QStringLiteral("hash type %1 is used twice").arg(entry.hashType)));
        hashTypes.insert(entry.hashType);
        QVERIFY(!entry.password.isEmpty());
        const QString password = QString(entry.password);
        QVERIFY2(!passwords.contains(password),
                 qPrintable(QStringLiteral("password %1 is used twice").arg(password)));
        passwords.insert(password);
    }

    for (const QLatin1StringView &resource : Examples::DataFiles) {
        QVERIFY2(QFile::exists(QString(resource)), qPrintable(QString(resource)));
    }
}

// The examples that ship in the binary have to stay valid sessions: every
// widget a session stores is named, and applying one leaves no value behind.
void TestMainWindow::exampleSessionFilesAreValid()
{
    MainWindow window;

    for (const Examples::Entry &entry : Examples::All) {
        const QString file = QString(entry.resource);
        QVERIFY2(QFile::exists(file), qPrintable(file));

        const StateFile::Document document = StateFile::readDocument(
            file, AppConstants::FileSections::Session, AppConstants::Sessions::Version);
        QVERIFY2(document.status != StateFile::Status::Unreadable, qPrintable(document.error));
        QCOMPARE(document.version, AppConstants::Sessions::Version);
        QVERIFY(document.otherSections.isEmpty());

        // The hash type list normally arrives from hashcat in the background;
        // the tests only need the entry the example names.
        MainWindowTestAccess::ui(window)->comboBox_hash_type->clear();
        MainWindowTestAccess::ui(window)->comboBox_hash_type->addItem(
            QString::number(entry.hashType), entry.hashType);

        const QStringList notes = MainWindowTestAccess::applySessionFile(window, document);
        QVERIFY2(notes.isEmpty(),
                 qPrintable(QStringLiteral("%1: %2").arg(file, notes.join(QLatin1Char('\n')))));

        QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_attack->currentData().toInt(),
                 static_cast<int>(entry.attackMode));
        QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->currentData().toInt(),
                 entry.hashType);

        // A widget added later must be added to every example as well, or
        // loading one would leave that widget at whatever the user had in the
        // form.
        const QJsonObject state =
            WidgetStateSerializer::toJson(&window, {AppConstants::Files::PreviewWidget});
        QStringList missing;
        for (auto it = state.constBegin(); it != state.constEnd(); ++it) {
            if (!document.values.contains(it.key())) {
                missing << it.key();
            }
        }
        QVERIFY2(missing.isEmpty(), qPrintable(QStringLiteral("%1 does not name: %2")
                                                   .arg(file, missing.join(QStringLiteral(", ")))));

        // A data path that no resource is bundled under would fail only when
        // the example is loaded, with a missing wordlist or rules file.
        QStringList paths;
        collectExamplePaths(document.values, &paths);
        for (const QString &path : std::as_const(paths)) {
            const QString name = QFileInfo(path).fileName();
            bool found = false;
            for (const QLatin1StringView &resource : Examples::DataFiles) {
                if (QFileInfo(QString(resource)).fileName() == name) {
                    found = true;
                    break;
                }
            }
            QVERIFY2(found,
                     qPrintable(
                         QStringLiteral("%1 names %2, which has no bundled file").arg(file, path)));
        }
    }
}

// Like a reset, loading an example throws away the session in the form, so
// declining the question has to leave it untouched.
void TestMainWindow::loadExampleSessionNeedsAConfirmation()
{
    MainWindow window;
    MainWindowTestAccess::ui(window)->lineEdit_session->setText(QStringLiteral("keepMe"));

    QAction *action = exampleAction(window, AttackMode::BruteForce);
    QVERIFY(action);
    QCOMPARE(action->text(), QStringLiteral("Brute-force"));

    TestEnvironment::ModalRecorder recorder;
    recorder.start();

    action->trigger();

    QVERIFY(recorder.messageBoxSeen);
    QCOMPARE(recorder.title, QStringLiteral("Load example session"));
    QVERIFY(recorder.text.contains(QStringLiteral("lost")));
    QVERIFY(recorder.text.contains(QStringLiteral("should recover the password Nice!98")));
    // The recorder closed the box without confirming: nothing was loaded.
    QCOMPARE(MainWindowTestAccess::ui(window)->lineEdit_session->text(), QStringLiteral("keepMe"));
}

// Confirming the question loads the bundled example through the same path an
// imported session takes.
void TestMainWindow::loadExampleSessionAppliesTheBundledSession()
{
    MainWindow window;
    MainWindowTestAccess::ui(window)->comboBox_hash_type->clear();
    MainWindowTestAccess::ui(window)->comboBox_hash_type->addItem(QStringLiteral("1400 | SHA2-256"),
                                                                  1400);

    QAction *action = exampleAction(window, AttackMode::BruteForce);
    QVERIFY(action);

    TestEnvironment::ModalRecorder recorder;
    recorder.acceptConfirmation = true;
    recorder.start();

    action->trigger();

    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_attack->currentData().toInt(), 3);
    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->currentData().toInt(), 1400);
    QCOMPARE(MainWindowTestAccess::ui(window)->lineEdit_hashfile->text(),
             QStringLiteral("ee3d96862a7b0806e6e0ad7861c46ea2d0b25a5eb9607cf5c837eb9906ca53b6"));
    QCOMPARE(MainWindowTestAccess::ui(window)->lineEdit_mask->text(),
             QStringLiteral("?u?1?1?1?2?d?d"));
    QCOMPARE(MainWindowTestAccess::ui(window)->lineEdit_custom_charset1->text(),
             QStringLiteral("?l?u"));
    QCOMPARE(MainWindowTestAccess::ui(window)->lineEdit_custom_charset2->text(),
             QStringLiteral("!@#$"));
    QVERIFY(MainWindowTestAccess::ui(window)->checkBox_custom_charset1->isChecked());
    QVERIFY(MainWindowTestAccess::ui(window)->checkBox_custom_charset2->isChecked());
    QVERIFY(MainWindowTestAccess::ui(window)->lineEdit_command->text().contains(
        QStringLiteral("--attack-mode 3")));
    QVERIFY(MainWindowTestAccess::ui(window)->lineEdit_command->text().contains(
        QStringLiteral("--hash-type 1400")));
}

// The data an example names lives in the resources, so loading it has to
// unpack every file and replace the token before the session is applied.
void TestMainWindow::loadExampleSessionUnpacksItsDataFiles()
{
    MainWindow window;
    MainWindowTestAccess::ui(window)->comboBox_hash_type->clear();
    MainWindowTestAccess::ui(window)->comboBox_hash_type->addItem(QStringLiteral("0 | MD5"), 0);

    QAction *action = exampleAction(window, AttackMode::Straight);
    QVERIFY(action);

    TestEnvironment::ModalRecorder recorder;
    recorder.acceptConfirmation = true;
    recorder.start();

    action->trigger();

    const QString directory =
        QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))
            .filePath(QString(Examples::DataDirectory));
    for (const QLatin1StringView &resource : Examples::DataFiles) {
        const QString path = QDir(directory).filePath(QFileInfo(QString(resource)).fileName());
        QVERIFY2(QFile::exists(path), qPrintable(path));
    }

    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_attack->currentData().toInt(), 0);
    QVERIFY(MainWindowTestAccess::ui(window)->lineEdit_command->text().contains(
        QDir::toNativeSeparators(directory)));
    QVERIFY(!MainWindowTestAccess::ui(window)->lineEdit_command->text().contains(
        QString(Examples::DataPrefix)));
    QVERIFY(MainWindowTestAccess::ui(window)->lineEdit_command->text().contains(
        QDir::toNativeSeparators(QDir(directory).filePath(QStringLiteral("straight.rule")))));
}

void TestMainWindow::optionDependenciesFollowTheirCheckboxes()
{
    MainWindow window;
    // A mask mode keeps the increment widgets out of the disabled mask group,
    // so the test observes their own dependency and not the group's.
    MainWindowTestAccess::ui(window)->comboBox_attack->setCurrentIndex(2); // brute-force

    // A fresh window: every dependent value starts switched off.
    QVERIFY(!MainWindowTestAccess::ui(window)->spinBox_increment_min->isEnabled());
    QVERIFY(!MainWindowTestAccess::ui(window)->spinBox_increment_max->isEnabled());
    QVERIFY(!MainWindowTestAccess::ui(window)->spinBox_status_timer->isEnabled());
    QVERIFY(!MainWindowTestAccess::ui(window)->spinBox_hwmon_temp_abort->isEnabled());

    MainWindowTestAccess::ui(window)->checkBox_increment->setChecked(true);
    QVERIFY(MainWindowTestAccess::ui(window)->spinBox_increment_min->isEnabled());
    QVERIFY(MainWindowTestAccess::ui(window)->spinBox_increment_max->isEnabled());
    QVERIFY(MainWindowTestAccess::ui(window)->label_increment_min->isEnabled());
    QVERIFY(MainWindowTestAccess::ui(window)->label_increment_max->isEnabled());
    MainWindowTestAccess::ui(window)->checkBox_increment->setChecked(false);
    QVERIFY(!MainWindowTestAccess::ui(window)->spinBox_increment_min->isEnabled());
    QVERIFY(!MainWindowTestAccess::ui(window)->spinBox_increment_max->isEnabled());

    MainWindowTestAccess::ui(window)->checkBox_status->setChecked(true);
    QVERIFY(MainWindowTestAccess::ui(window)->spinBox_status_timer->isEnabled());
    MainWindowTestAccess::ui(window)->checkBox_status->setChecked(false);
    QVERIFY(!MainWindowTestAccess::ui(window)->spinBox_status_timer->isEnabled());

    MainWindowTestAccess::ui(window)->checkBox_hwmon_temp_abort->setChecked(true);
    QVERIFY(MainWindowTestAccess::ui(window)->spinBox_hwmon_temp_abort->isEnabled());
    MainWindowTestAccess::ui(window)->checkBox_hwmon_temp_abort->setChecked(false);
    QVERIFY(!MainWindowTestAccess::ui(window)->spinBox_hwmon_temp_abort->isEnabled());
}

// JSON and the format field are exclusive: the box takes the field over,
// keeps its value, and the builder suppresses --outfile-format.
void TestMainWindow::outfileJsonTakesOverFromTheFormatField()
{
    MainWindow window;

    MainWindowTestAccess::ui(window)->lineEdit_outfile_format->setText(QStringLiteral("1,2,3"));
    QVERIFY(MainWindowTestAccess::ui(window)->lineEdit_outfile_format->isEnabled());

    MainWindowTestAccess::ui(window)->checkBox_outfile_json->setChecked(true);
    QVERIFY(!MainWindowTestAccess::ui(window)->lineEdit_outfile_format->isEnabled());
    // The value survives, so switching back restores what was typed.
    QCOMPARE(MainWindowTestAccess::ui(window)->lineEdit_outfile_format->text(),
             QStringLiteral("1,2,3"));
    QVERIFY(!MainWindowTestAccess::generateArguments(window).contains(
        QStringLiteral("--outfile-format")));

    MainWindowTestAccess::ui(window)->checkBox_outfile_json->setChecked(false);
    QVERIFY(MainWindowTestAccess::ui(window)->lineEdit_outfile_format->isEnabled());
    QCOMPARE(MainWindowTestAccess::generateArguments(window).value(
                 MainWindowTestAccess::generateArguments(window).indexOf(
                     QStringLiteral("--outfile-format"))
                 + 1),
             QStringLiteral("1,2,3"));

    // The rules-concat box only exists next to the files it joins; the
    // generated-rules radio is the other half of the same choice.
    QVERIFY(MainWindowTestAccess::ui(window)->checkBox_rules_concat->isEnabled());
    MainWindowTestAccess::ui(window)->radioButton_generate_rules->setChecked(true);
    QVERIFY(!MainWindowTestAccess::ui(window)->checkBox_rules_concat->isEnabled());
    MainWindowTestAccess::ui(window)->radioButton_use_rules_file->setChecked(true);
    QVERIFY(MainWindowTestAccess::ui(window)->checkBox_rules_concat->isEnabled());
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
    // The hints wrap at the tab width (wordWrap in the .ui file), so the
    // texts carry no breaks - see the comment in modeHint().
    const QList<Expectation> expectations{
        {AttackMode::Straight, QStringLiteral("This mode needs at least one wordlist.")},
        {AttackMode::Combination,
         QStringLiteral("This mode needs exactly 2 wordlists (left, right).")},
        {AttackMode::BruteForce, QStringLiteral("This mode needs a mask.")},
        {AttackMode::Pcfg,
         QStringLiteral("This mode runs hashcat’s bundled default-passwords ruleset; "
                        "name one or more to use your own.")},
        {AttackMode::Table, QStringLiteral("This mode needs one wordlist and at least one table.")},
        {AttackMode::HybridWordMask, QStringLiteral("This mode needs a wordlist and a mask.")},
        {AttackMode::HybridMaskWord, QStringLiteral("This mode needs a wordlist and a mask.")},
        {AttackMode::Generic,
         QStringLiteral("This mode needs a feed. Arguments depend on the feed: "
                        "random and stdin need none, wordlist needs a path.")},
        {AttackMode::Association,
         QStringLiteral("This mode needs at most one wordlist. Without one the words come from "
                        "the hash file and the settings line can bound the run; with one the "
                        "two files are paired by line.")},
        {AttackMode::Hybrid,
         QStringLiteral("This mode needs a mask with exactly one ?w; a ?q after ?w "
                        "names a second wordlist.")},
    };

    for (int i = 0; i < MainWindowTestAccess::ui(window)->comboBox_attack->count(); ++i) {
        // -1 first, so that every iteration really changes the index and runs
        // attackIndexChanged()
        MainWindowTestAccess::ui(window)->comboBox_attack->setCurrentIndex(-1);
        MainWindowTestAccess::ui(window)->comboBox_attack->setCurrentIndex(i);

        const AttackMode mode = static_cast<AttackMode>(
            MainWindowTestAccess::ui(window)->comboBox_attack->currentData().toInt());

        const Expectation *expected = nullptr;
        for (const Expectation &candidate : expectations) {
            if (candidate.mode == mode) {
                expected = &candidate;
            }
        }
        if (expected == nullptr) {
            QFAIL("no expectation written down for this attack mode");
        }

        QCOMPARE(MainWindowTestAccess::ui(window)->label_mode_hint->text(), expected->hint);
    }
}

// The launch validation works on what the window collects, so the two have to
// agree: what the widgets show is what the validation complains about.
// The recipe row belongs to hash mode 4000 alone, and the recipe is one
// argument - the preview quotes it so a copied command does not split at
// its spaces.
void TestMainWindow::hashRecipeRowFollowsTheHashType()
{
    MainWindow window;

    // The combo stays empty until the hashcat query answers; a fresh window
    // therefore shows no recipe row.
    QVERIFY(MainWindowTestAccess::ui(window)->lineEdit_hash_recipe->isHidden());

    MainWindowTestAccess::ui(window)->comboBox_hash_type->addItem(QStringLiteral("4000 | Hashcat"),
                                                                  4000);
    MainWindowTestAccess::ui(window)->comboBox_hash_type->addItem(QStringLiteral("1000 | NTLM"),
                                                                  1000);

    MainWindowTestAccess::ui(window)->comboBox_hash_type->setCurrentIndex(1); // NTLM
    QVERIFY(MainWindowTestAccess::ui(window)->label_hash_recipe->isHidden());
    QVERIFY(MainWindowTestAccess::ui(window)->lineEdit_hash_recipe->isHidden());

    MainWindowTestAccess::ui(window)->comboBox_hash_type->setCurrentIndex(0); // 4000
    QVERIFY(!MainWindowTestAccess::ui(window)->label_hash_recipe->isHidden());
    QVERIFY(!MainWindowTestAccess::ui(window)->lineEdit_hash_recipe->isHidden());

    // The recipe reaches the command as exactly one token, spaces included,
    // and the preview quotes it for display and for the clipboard.
    MainWindowTestAccess::ui(window)->lineEdit_hash_recipe->setText(
        QStringLiteral("md5(md5(salt) . pass)"));

    const QStringList arguments = MainWindowTestAccess::generateArguments(window);
    const qsizetype flag = arguments.indexOf(QStringLiteral("--hash-recipe"));
    QVERIFY(flag >= 0);
    QCOMPARE(arguments.at(flag + 1), QStringLiteral("md5(md5(salt) . pass)"));

    const QString preview = MainWindowTestAccess::ui(window)->lineEdit_command->text();
#ifdef Q_OS_WIN
    QVERIFY(preview.contains(QStringLiteral("\"md5(md5(salt) . pass)\"")));
#else
    QVERIFY(preview.contains(QStringLiteral("'md5(md5(salt) . pass)'")));
#endif

    // Switching the type away drops the flag even though the field still
    // holds the recipe: hashcat refuses the flag for any other type.
    MainWindowTestAccess::ui(window)->comboBox_hash_type->setCurrentIndex(1);
    QVERIFY(
        !MainWindowTestAccess::generateArguments(window).contains(QStringLiteral("--hash-recipe")));
}

void TestMainWindow::widgetsFlowIntoTheOptions()
{
    MainWindow window;

    MainWindowTestAccess::ui(window)->lineEdit_hashfile->setText(
        QStringLiteral("/tmp/example.hash"));
    MainWindowTestAccess::ui(window)->lineEdit_mask->setText(QStringLiteral("?d?d"));
    MainWindowTestAccess::ui(window)->checkBox_remove->setChecked(true);
    MainWindowTestAccess::ui(window)->checkBox_ignoreusername->setChecked(true);
    MainWindowTestAccess::ui(window)->checkBox_speed_only->setChecked(true);
    MainWindowTestAccess::ui(window)->checkBox_hex_wordlist->setChecked(true);
    MainWindowTestAccess::ui(window)->lineEdit_encoding_from->setText(
        QStringLiteral("iso-8859-15"));
    MainWindowTestAccess::ui(window)->lineEdit_encoding_to->setText(QStringLiteral("utf-8"));
    MainWindowTestAccess::ui(window)->lineEdit_rule_left->setText(QStringLiteral("c"));
    MainWindowTestAccess::ui(window)->lineEdit_rule_right->setText(QStringLiteral("^x"));
    MainWindowTestAccess::ui(window)->checkBox_increment->setChecked(true);
    MainWindowTestAccess::ui(window)->spinBox_increment_min->setValue(2);
    MainWindowTestAccess::ui(window)->spinBox_increment_max->setValue(8);
    MainWindowTestAccess::ui(window)->lineEdit_session->setText(QStringLiteral("job"));
    MainWindowTestAccess::ui(window)->spinBox_runtime->setValue(60);
    MainWindowTestAccess::ui(window)->lineEdit_skip->setText(QStringLiteral("100"));
    MainWindowTestAccess::ui(window)->lineEdit_limit->setText(QStringLiteral("200"));
    MainWindowTestAccess::ui(window)->checkBox_status->setChecked(true);
    MainWindowTestAccess::ui(window)->spinBox_status_timer->setValue(5);
    MainWindowTestAccess::ui(window)->checkBox_force->setChecked(true);
    MainWindowTestAccess::ui(window)->checkBox_potfile_disable->setChecked(true);
    MainWindowTestAccess::ui(window)->lineEdit_opencl_device_types->setText(QStringLiteral("1,2"));
    MainWindowTestAccess::ui(window)->checkBox_hwmon_temp_abort->setChecked(true);
    MainWindowTestAccess::ui(window)->spinBox_hwmon_temp_abort->setValue(85);

    // The feed modes and the association line: read unconditionally, because
    // a hidden field can still hold a value a session stored; a combination
    // that cannot work is the launch validation's answer, not a dropped
    // value here.
    const auto addRow = [](QListWidget *list, const QString &text) {
        auto *item = new QListWidgetItem(text, list);
        item->setCheckState(Qt::Checked);
    };
    addRow(MainWindowTestAccess::ui(window)->listWidget_pcfg_rulesets,
           QStringLiteral("/tmp/one.pcfg"));
    addRow(MainWindowTestAccess::ui(window)->listWidget_pcfg_rulesets,
           QStringLiteral("/tmp/two.pcfg"));
    MainWindowTestAccess::ui(window)->lineEdit_pcfg_scale->setText(QStringLiteral("2"));
    MainWindowTestAccess::ui(window)->lineEdit_pcfg_weights->setText(QStringLiteral("2:1"));
    MainWindowTestAccess::ui(window)->lineEdit_pcfg_extra->setText(
        QStringLiteral("omen=1  pwmin=8"));
    addRow(MainWindowTestAccess::ui(window)->listWidget_tables, QStringLiteral("/tmp/table.txt"));
    MainWindowTestAccess::ui(window)->lineEdit_table_identity->setText(QStringLiteral("1"));
    MainWindowTestAccess::ui(window)->lineEdit_table_cap->setText(QStringLiteral("4294967295"));
    MainWindowTestAccess::ui(window)->lineEdit_feed->setText(QStringLiteral("wordlist"));
    addRow(MainWindowTestAccess::ui(window)->listWidget_feed_arguments, QStringLiteral("dict.txt"));
    MainWindowTestAccess::ui(window)->lineEdit_association_settings->setText(
        QStringLiteral("phases=words,rules rules.rulemax=300"));

    // The tier 2 options.
    MainWindowTestAccess::ui(window)->checkBox_rules_concat->setChecked(true);
    MainWindowTestAccess::ui(window)->checkBox_outfile_json->setChecked(true);
    MainWindowTestAccess::ui(window)->lineEdit_cache_path->setText(QStringLiteral("/tmp/hc-cache"));

    const HashcatOptions options = MainWindowTestAccess::collectHashcatOptions(window);
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
    // The increment box is checked, but straight mode offers no -i: the
    // collect gating drops the flag while the bounds still arrive - a stale
    // pair must not block the other modes.
    QVERIFY(!options.increment);
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
    QCOMPARE(options.pcfgRulesets,
             (QStringList{QStringLiteral("/tmp/one.pcfg"), QStringLiteral("/tmp/two.pcfg")}));
    QCOMPARE(options.pcfgScale, QStringLiteral("2"));
    QCOMPARE(options.pcfgWeights, QStringLiteral("2:1"));
    // The extra line is a settings run: whitespace separates the tokens, and
    // a run of spaces must not leave an empty one behind.
    QCOMPARE(options.pcfgExtraSettings,
             (QStringList{QStringLiteral("omen=1"), QStringLiteral("pwmin=8")}));
    QCOMPARE(options.tableFiles, QStringList{QStringLiteral("/tmp/table.txt")});
    QCOMPARE(options.tableIdentity, QStringLiteral("1"));
    QCOMPARE(options.tableCap, QStringLiteral("4294967295"));
    QCOMPARE(options.feedName, QStringLiteral("wordlist"));
    QCOMPARE(options.feedArguments, QStringList{QStringLiteral("dict.txt")});
    QCOMPARE(options.associationSettings, (QStringList{QStringLiteral("phases=words,rules"),
                                                       QStringLiteral("rules.rulemax=300")}));
    QVERIFY(options.rulesConcat);
    QVERIFY(options.outfileJson);
    QCOMPARE(options.cachePath, QStringLiteral("/tmp/hc-cache"));

    // Untouched fields keep the values the .ui file starts with
    QCOMPARE(options.outfileFormat, AppConstants::Defaults::OutfileFormat);
    QCOMPARE(options.backendDevices, AppConstants::Defaults::BackendDevices);

    // The suggested outfile never switches output on by itself
    QVERIFY(!options.outfileEnabled);

    const QStringList arguments = MainWindowTestAccess::generateArguments(window);
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

    // A row that is unchecked stays in the list and keeps its place, but is
    // not passed to hashcat - in every list, not only the wordlists.
    MainWindowTestAccess::ui(window)->listWidget_pcfg_rulesets->item(1)->setCheckState(
        Qt::Unchecked);
    MainWindowTestAccess::ui(window)->listWidget_tables->item(0)->setCheckState(Qt::Unchecked);
    MainWindowTestAccess::ui(window)->listWidget_feed_arguments->item(0)->setCheckState(
        Qt::Unchecked);
    const HashcatOptions reduced = MainWindowTestAccess::collectHashcatOptions(window);
    QCOMPARE(reduced.pcfgRulesets, QStringList{QStringLiteral("/tmp/one.pcfg")});
    QVERIFY(reduced.tableFiles.isEmpty());
    QVERIFY(reduced.feedArguments.isEmpty());
    QCOMPARE(MainWindowTestAccess::ui(window)->listWidget_pcfg_rulesets->count(), 2);
    QCOMPARE(MainWindowTestAccess::ui(window)->listWidget_tables->count(), 1);
    QCOMPARE(MainWindowTestAccess::ui(window)->listWidget_feed_arguments->count(), 1);

    // The increment widgets are hidden in the hybrid mode (12), which
    // refuses -i: a checked box a session stored there must not reach the
    // options.
    MainWindowTestAccess::ui(window)->comboBox_attack->setCurrentIndex(9); // hybrid
    QCOMPARE(int(MainWindowTestAccess::collectHashcatOptions(window).attackMode),
             int(AttackMode::Hybrid));
    QVERIFY(MainWindowTestAccess::ui(window)->checkBox_increment->isChecked());
    QVERIFY(!MainWindowTestAccess::collectHashcatOptions(window).increment);
}

void TestMainWindow::statusTimerReachesTheCommandLine()
{
    MainWindow window;

    // The spin box starts at hashcat's own interval, and the value it shows is
    // the value the command line carries.
    QCOMPARE(MainWindowTestAccess::ui(window)->spinBox_status_timer->value(),
             AppConstants::Defaults::StatusTimer);

    MainWindowTestAccess::ui(window)->checkBox_status->setChecked(true);

    QStringList arguments = MainWindowTestAccess::generateArguments(window);
    QVERIFY(arguments.contains(QStringLiteral("--status")));
    QVERIFY(arguments.contains(QStringLiteral("--status-timer")));
    QCOMPARE(arguments.value(arguments.indexOf(QStringLiteral("--status-timer")) + 1),
             QString::number(AppConstants::Defaults::StatusTimer));

    // ... and the command line follows the spin box.
    MainWindowTestAccess::ui(window)->spinBox_status_timer->setValue(1);
    arguments = MainWindowTestAccess::generateArguments(window);
    QCOMPARE(arguments.value(arguments.indexOf(QStringLiteral("--status-timer")) + 1),
             QStringLiteral("1"));
}

void TestMainWindow::skipAndLimitAcceptOnlyNumbers()
{
    MainWindow window;

    QLineEdit *skip = MainWindowTestAccess::ui(window)->lineEdit_skip;
    QLineEdit *limit = MainWindowTestAccess::ui(window)->lineEdit_limit;
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

// -D 3, the accelerators, is gone with hashcat 8.0.0. The field only lets the
// device types 1 (CPU) and 2 (GPU) through, with the spaces around the
// numbers that hashcat's own number list accepts.
void TestMainWindow::deviceTypesAcceptOnlyTheSupportedOnes()
{
    MainWindow window;

    QLineEdit *deviceTypes = MainWindowTestAccess::ui(window)->lineEdit_opencl_device_types;
    QVERIFY(deviceTypes->validator() != nullptr);

    const auto accepts = [&deviceTypes](const QString &text) {
        deviceTypes->setText(text);
        return deviceTypes->hasAcceptableInput();
    };

    // Empty means "let hashcat decide" and stays allowed.
    QVERIFY(accepts(QString()));
    QVERIFY(accepts(QStringLiteral("1")));
    QVERIFY(accepts(QStringLiteral("2")));
    QVERIFY(accepts(QStringLiteral("1,2")));
    QVERIFY(accepts(QStringLiteral("1, 2")));

    QVERIFY(!accepts(QStringLiteral("3")));
    QVERIFY(!accepts(QStringLiteral("1,3")));
    QVERIFY(!accepts(QStringLiteral("0")));
}

void TestMainWindow::checkedWordlistsEndUpInTheCommand()
{
    MainWindow window;

    const auto add = [&window](const QString &path, Qt::CheckState state) {
        auto *item =
            new QListWidgetItem(path, MainWindowTestAccess::ui(window)->listWidget_wordlist);
        item->setCheckState(state);
    };
    add(QStringLiteral("/tmp/a.txt"), Qt::Checked);
    add(QStringLiteral("/tmp/b.txt"), Qt::Unchecked);
    add(QStringLiteral("/tmp/c.txt"), Qt::Checked);

    const HashcatOptions options = MainWindowTestAccess::collectHashcatOptions(window);
    const QStringList expected{QStringLiteral("/tmp/a.txt"), QStringLiteral("/tmp/c.txt")};
    QCOMPARE(options.wordlists, expected);

    const QStringList arguments = MainWindowTestAccess::generateArguments(window);
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

    // A directory is a valid wordlist input since hashcat 8.0.0, so a row
    // naming one with a wordlist in it must not be marked as missing.
    const QString directoryPath = directory.filePath(QStringLiteral("wordlists"));
    QVERIFY(QDir().mkpath(directoryPath));
    QVERIFY(writeFile(QDir(directoryPath).filePath(QStringLiteral("words.txt"))));

    // A configured path keeps the constructor from queueing the settings
    // dialog; the test spins the event loop below. The watchdog closes the
    // query's failure warning, whose modal loop would otherwise swallow it.
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("/nonexistent/hashcat"));
    m_fixture.startWatchdog();

    MainWindow window;
    const auto add = [&window](const QString &path) {
        auto *item =
            new QListWidgetItem(path, MainWindowTestAccess::ui(window)->listWidget_wordlist);
        item->setCheckState(Qt::Checked);
        return item;
    };
    QListWidgetItem *present = add(presentPath);
    QListWidgetItem *missing = add(missingPath);
    QListWidgetItem *wordlistDirectory = add(directoryPath);

    // A usable path shows its type: a file for the file, a folder for the
    // folder. The warning takes the icon's place once the path is gone.
    QVERIFY(!present->icon().isNull());
    QVERIFY(!wordlistDirectory->icon().isNull());
    QVERIFY(present->icon().cacheKey() != wordlistDirectory->icon().cacheKey());
    QVERIFY(present->toolTip().isEmpty());
    QVERIFY(wordlistDirectory->toolTip().isEmpty());
    QVERIFY(!missing->icon().isNull());
    QVERIFY(missing->icon().cacheKey() != present->icon().cacheKey());
    QVERIFY(missing->toolTip().contains(missingPath));

    const qint64 fileIconKey = present->icon().cacheKey();
    const qint64 warningIconKey = missing->icon().cacheKey();

    // The indicator is presentation only: text, command and session keep the
    // path.
    QCOMPARE(missing->text(), missingPath);
    QCOMPARE(MainWindowTestAccess::collectHashcatOptions(window).wordlists,
             (QStringList{presentPath, missingPath, directoryPath}));
    const QJsonObject values =
        WidgetStateSerializer::toJson(&window, {AppConstants::Files::PreviewWidget});
    const QJsonArray items = values.value(QStringLiteral("listWidget_wordlist")).toArray();
    QCOMPARE(items.size(), 3);
    QCOMPARE(items.at(1).toObject().value(QStringLiteral("text")).toString(), missingPath);

    // A file that disappears after it was added is marked; one that comes back
    // is not.
    QVERIFY(QFile::remove(presentPath));
    QTRY_COMPARE(present->icon().cacheKey(), warningIconKey);
    QVERIFY(present->toolTip().contains(presentPath));

    QVERIFY(writeFile(presentPath));
    QTRY_COMPARE(present->icon().cacheKey(), fileIconKey);
    QVERIFY(present->toolTip().isEmpty());

    // A restored session clears the model and inserts the rows again; the
    // monitor has to attach to those rows as well.
    QJsonArray restoredWordlists;
    QJsonObject restoredWordlist;
    restoredWordlist.insert(QStringLiteral("text"), missingPath);
    restoredWordlist.insert(QStringLiteral("checked"), true);
    restoredWordlists.append(restoredWordlist);
    QJsonObject sessionValues;
    sessionValues.insert(QStringLiteral("listWidget_wordlist"), restoredWordlists);

    StateFile::Document document;
    document.status = StateFile::Status::Read;
    document.version = AppConstants::Sessions::Version;
    document.values = sessionValues;
    QVERIFY(MainWindowTestAccess::applySessionFile(window, document).isEmpty());

    QListWidget *list = MainWindowTestAccess::ui(window)->listWidget_wordlist;
    QCOMPARE(list->count(), 1);
    QListWidgetItem *restored = list->item(0);
    QCOMPARE(restored->text(), missingPath);
    QCOMPARE(restored->icon().cacheKey(), warningIconKey);
    QVERIFY(restored->toolTip().contains(missingPath));
}

// Both dialogs end in the same helper, so what it does to the list is what a
// selection does - including the check state the wordlist needs and the other
// lists must not store.
void TestMainWindow::addingPathsKeepsTheListClean()
{
    MainWindow window;

    QListWidget *wordlists = MainWindowTestAccess::ui(window)->listWidget_wordlist;
    CheckedItemList::addPaths(wordlists,
                              {QStringLiteral("/tmp/a.txt"), QString(),
                               QStringLiteral("/tmp/a.txt"), QStringLiteral("/tmp/b.txt")});
    QCOMPARE(wordlists->count(), 2);
    QCOMPARE(wordlists->item(0)->text(), QDir::toNativeSeparators(QStringLiteral("/tmp/a.txt")));
    QCOMPARE(wordlists->item(0)->checkState(), Qt::Checked);
    QCOMPARE(wordlists->item(1)->checkState(), Qt::Checked);

    // A path that is already a row is not added twice.
    CheckedItemList::addPaths(wordlists, {QStringLiteral("/tmp/a.txt")});
    QCOMPARE(wordlists->count(), 2);

    // Every list takes part in the command the same way: a new row starts
    // checked, and unchecking it keeps the row without passing it on.
    QListWidget *rulesets = MainWindowTestAccess::ui(window)->listWidget_pcfg_rulesets;
    CheckedItemList::addPaths(rulesets, {QStringLiteral("/tmp/one.pcfg")});
    QCOMPARE(rulesets->count(), 1);
    QCOMPARE(rulesets->item(0)->checkState(), Qt::Checked);

    QListWidget *tables = MainWindowTestAccess::ui(window)->listWidget_tables;
    CheckedItemList::addPaths(tables, {QStringLiteral("/tmp/table.txt")});
    QCOMPARE(tables->item(0)->checkState(), Qt::Checked);
}

// A bundled or installed ruleset is a name, not a file the dialog could
// return: the field adds it verbatim, once, and the command carries it.
void TestMainWindow::pcfgRulesetNameIsAddedFromTheField()
{
    MainWindow window;

    QLineEdit *field = MainWindowTestAccess::ui(window)->lineEdit_pcfg_ruleset_name;
    QListWidget *rulesets = MainWindowTestAccess::ui(window)->listWidget_pcfg_rulesets;

    // The field trims, but nothing else about the name is changed.
    field->setText(QStringLiteral("  hints  "));
    MainWindowTestAccess::addPcfgRulesetNameClicked(window);
    QCOMPARE(rulesets->count(), 1);
    QCOMPARE(rulesets->item(0)->text(), QStringLiteral("hints"));
    QCOMPARE(rulesets->item(0)->checkState(), Qt::Checked);
    QVERIFY(field->text().isEmpty());

    // A name that is already a row is not added twice.
    field->setText(QStringLiteral("hints"));
    MainWindowTestAccess::addPcfgRulesetNameClicked(window);
    QCOMPARE(rulesets->count(), 1);

    // An empty field adds nothing.
    field->setText(QStringLiteral("   "));
    MainWindowTestAccess::addPcfgRulesetNameClicked(window);
    QCOMPARE(rulesets->count(), 1);

    // PCFG passes the rows on unchanged; a bare name is what makes hashcat
    // look in its own pcfg folders rather than in the working directory.
    MainWindowTestAccess::ui(window)->comboBox_attack->setCurrentIndex(3); // PCFG
    QVERIFY(MainWindowTestAccess::generateArguments(window).contains(QStringLiteral("hints")));
}

// A wordlist folder, a ruleset folder and a table are judged by different
// rules; the rows have to show which of them hashcat can use, and have to
// recover when the missing piece arrives.
void TestMainWindow::folderPathsAreMarked()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());

    const QString trained = directory.filePath(QStringLiteral("trained"));
    QVERIFY(QDir().mkpath(trained + QStringLiteral("/Grammar")));
    QVERIFY(writeFile(trained + QStringLiteral("/Grammar/grammar.txt")));

    const QString untrained = directory.filePath(QStringLiteral("untrained"));
    QVERIFY(QDir().mkpath(untrained));

    const QString tableFile = directory.filePath(QStringLiteral("table.txt"));
    QVERIFY(writeFile(tableFile));

    // A folder with a table file in it is still not a table.
    const QString tableFolder = directory.filePath(QStringLiteral("tables"));
    QVERIFY(QDir().mkpath(tableFolder));
    QVERIFY(writeFile(tableFolder + QStringLiteral("/inside.txt")));

    const QString emptyFolder = directory.filePath(QStringLiteral("empty"));
    QVERIFY(QDir().mkpath(emptyFolder));

    // A configured path keeps the constructor from queueing the settings
    // dialog; the test spins the event loop below. The watchdog closes the
    // query's failure warning, whose modal loop would otherwise swallow it.
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("/nonexistent/hashcat"));
    m_fixture.startWatchdog();

    MainWindow window;

    MainWindowTestAccess::ui(window)->listWidget_pcfg_rulesets->addItem(trained);
    MainWindowTestAccess::ui(window)->listWidget_pcfg_rulesets->addItem(untrained);
    MainWindowTestAccess::ui(window)->listWidget_pcfg_rulesets->addItem(
        QStringLiteral("default-passwords"));
    MainWindowTestAccess::ui(window)->listWidget_tables->addItem(tableFile);
    MainWindowTestAccess::ui(window)->listWidget_tables->addItem(tableFolder);
    auto *empty =
        new QListWidgetItem(emptyFolder, MainWindowTestAccess::ui(window)->listWidget_wordlist);
    empty->setCheckState(Qt::Checked);

    const auto row = [](QListWidget *list, const QString &text) -> QListWidgetItem * {
        const QList<QListWidgetItem *> matches = list->findItems(text, Qt::MatchExactly);
        return matches.isEmpty() ? nullptr : matches.first();
    };

    QListWidget *pcfg = MainWindowTestAccess::ui(window)->listWidget_pcfg_rulesets;
    QListWidget *tables = MainWindowTestAccess::ui(window)->listWidget_tables;

    QVERIFY(row(pcfg, trained));
    QVERIFY(!row(pcfg, trained)->icon().isNull());
    QVERIFY(row(pcfg, trained)->toolTip().isEmpty());
    const qint64 folderIconKey = row(pcfg, trained)->icon().cacheKey();

    QVERIFY(!row(pcfg, untrained)->icon().isNull());
    QVERIFY(row(pcfg, untrained)->icon().cacheKey() != folderIconKey);
    QVERIFY(row(pcfg, untrained)->toolTip().contains(QStringLiteral("Grammar/grammar.txt")));
    const qint64 warningIconKey = row(pcfg, untrained)->icon().cacheKey();

    // A bundled name is resolved by hashcat, so the GUI must not call it
    // missing - and it has no file or folder to show a type for.
    QVERIFY(row(pcfg, QStringLiteral("default-passwords"))->icon().isNull());
    QVERIFY(row(pcfg, QStringLiteral("default-passwords"))->toolTip().isEmpty());

    // A table is a file: it shows the file icon, and a folder is marked as
    // unusable whatever it holds.
    QVERIFY(!row(tables, tableFile)->icon().isNull());
    QVERIFY(row(tables, tableFile)->icon().cacheKey() != folderIconKey);
    QVERIFY(row(tables, tableFile)->toolTip().isEmpty());
    QVERIFY(!row(tables, tableFolder)->icon().isNull());
    QCOMPARE(row(tables, tableFolder)->icon().cacheKey(), warningIconKey);
    QVERIFY(row(tables, tableFolder)->toolTip().contains(QStringLiteral("must be a file")));

    QVERIFY(!empty->icon().isNull());
    QCOMPARE(empty->icon().cacheKey(), warningIconKey);
    QVERIFY(empty->toolTip().contains(QStringLiteral("no wordlists")));

    // The grammar arriving later turns the warning back into the folder the
    // row names.
    QVERIFY(QDir().mkpath(untrained + QStringLiteral("/Grammar")));
    QVERIFY(writeFile(untrained + QStringLiteral("/Grammar/grammar.txt")));
    QTRY_COMPARE(row(pcfg, untrained)->icon().cacheKey(), folderIconKey);
    QVERIFY(row(pcfg, untrained)->toolTip().isEmpty());
}

void TestMainWindow::hashTypeComesFromTheItemData()
{
    MainWindow window;

    // The real list comes from hashcat at runtime; inserting it by hand keeps
    // this test independent of the stub below.
    MainWindowTestAccess::ui(window)->comboBox_hash_type->clear();
    MainWindowTestAccess::ui(window)->comboBox_hash_type->addItem(QStringLiteral("1000 | NTLM"),
                                                                  1000);
    MainWindowTestAccess::ui(window)->comboBox_hash_type->addItem(QStringLiteral("0 | MD5"), 0);
    MainWindowTestAccess::ui(window)->comboBox_hash_type->setCurrentIndex(0);

    QCOMPARE(MainWindowTestAccess::collectHashcatOptions(window).hashType, 1000);
    const QStringList withHashType{"--hash-type", "1000", "--attack-mode", "0"};
    QCOMPARE(MainWindowTestAccess::generateArguments(window), withHashType);

    // Nothing selected - the state before the query finished - must leave -m
    // out instead of silently asking for hash type 0
    MainWindowTestAccess::ui(window)->comboBox_hash_type->clear();
    QCOMPARE(MainWindowTestAccess::collectHashcatOptions(window).hashType, -1);
    const QStringList withoutHashType{"--attack-mode", "0"};
    QCOMPARE(MainWindowTestAccess::generateArguments(window), withoutHashType);
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

    m_fixture.startWatchdog();

    MainWindow window;

    // The query runs off-thread; until the event loop delivers its result the
    // combo stays parked in the state the first reload put it in.
    QVERIFY(!MainWindowTestAccess::ui(window)->comboBox_hash_type->isEnabled());
    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->toolTip(),
             QStringLiteral("Updating…"));

    // Wait at least as long as the query's own deadline: giving up earlier
    // would fail the test while hashcat could still have answered
    QTRY_VERIFY_WITH_TIMEOUT(MainWindowTestAccess::ui(window)->comboBox_hash_type->count() > 0,
                             AppConstants::Hashcat::QueryTimeoutMs);

    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->count(), 2);
    QVERIFY(MainWindowTestAccess::ui(window)->comboBox_hash_type->isEnabled());
    QVERIFY(MainWindowTestAccess::ui(window)->comboBox_hash_type->toolTip().isEmpty());

    // 0 sorts before 1000, and the ids travel as item data
    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->itemData(0).toUInt(), 0U);
    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->itemData(1).toUInt(), 1000U);

    const QStringList arguments = MainWindowTestAccess::generateArguments(window);
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
    m_fixture.startWatchdog();

    MainWindow window;

    // The query starts out exactly like the successful one: parked on the
    // reply, with the tooltip that says so.
    QVERIFY(!MainWindowTestAccess::ui(window)->comboBox_hash_type->isEnabled());
    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->toolTip(),
             QStringLiteral("Updating…"));

    // ... and when the reply turns out to be unusable, the combo comes back
    QTRY_VERIFY_WITH_TIMEOUT(MainWindowTestAccess::ui(window)->comboBox_hash_type->isEnabled(),
                             AppConstants::Hashcat::QueryTimeoutMs);
    QCOMPARE(
        MainWindowTestAccess::ui(window)->comboBox_hash_type->toolTip(),
        QStringLiteral("The hash types could not be loaded. Check the hashcat path in Settings."));
    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->count(), 0);
}

// Accepting Settings again with the same binary leaves the list alone; a
// changed binary re-runs the query and replaces it with the modes the new
// one reports (which resets the selection - the old id may not exist there).
void TestMainWindow::changingTheBinaryReRunsTheQuery()
{
    const QString stubA = TestEnvironment::writeShellStub(
        QStringLiteral("stub-hashcat-settings-a"),
        "#!/bin/sh\n"
        "printf '{\"0\":{\"name\":\"MD5\"},\"1000\":{\"name\":\"NTLM\"}}\\n'\n"
        "exit 0\n");
    const QString stubB =
        TestEnvironment::writeShellStub(QStringLiteral("stub-hashcat-settings-b"),
                                        "#!/bin/sh\n"
                                        "printf '{\"1400\":{\"name\":\"SHA2-256\"}}\\n'\n"
                                        "exit 0\n");
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stubA.isEmpty() && !stubB.isEmpty(), "the shell stubs could not be written");
#endif

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stubA);

    MainWindow window;
    QTRY_VERIFY_WITH_TIMEOUT(MainWindowTestAccess::ui(window)->comboBox_hash_type->count() == 2,
                             AppConstants::Hashcat::QueryTimeoutMs);
    MainWindowTestAccess::ui(window)->comboBox_hash_type->setCurrentIndex(1); // NTLM

    // The dialog takes its values from the settings, so the path is set
    // before it opens; the recorder saves it, which is what a user pressing
    // Save does.
    TestEnvironment::ModalRecorder recorder;
    recorder.saveSettingsDialog = true;
    recorder.start();

    // Same path: no new query, so the list and the selection stay as they
    // are. A re-query would clear the combo and disable it synchronously.
    QVERIFY(QMetaObject::invokeMethod(&window, "settingsTriggered"));
    QVERIFY(MainWindowTestAccess::ui(window)->comboBox_hash_type->isEnabled());
    QVERIFY(MainWindowTestAccess::ui(window)->comboBox_hash_type->toolTip().isEmpty());
    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->count(), 2);
    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->currentIndex(), 1);

    // Changed path: the query runs again and the new list replaces the old.
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stubB);
    QVERIFY(QMetaObject::invokeMethod(&window, "settingsTriggered"));
    QTRY_VERIFY_WITH_TIMEOUT(MainWindowTestAccess::ui(window)->comboBox_hash_type->count() == 1,
                             AppConstants::Hashcat::QueryTimeoutMs);
    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->itemData(0).toUInt(), 1400U);
}

// The reply of a query an earlier init started describes a binary the list no
// longer comes from. It must not replace the newer list - and must not fill the
// combo after the path was cleared. The first stub holds its answer until the
// test lets it go, so the two replies cannot race by accident.
void TestMainWindow::staleQueryDoesNotReplaceTheNewerList_data()
{
    QTest::addColumn<bool>("clearPath");

    QTest::newRow("changed binary") << false;
    QTest::newRow("cleared path") << true;
}

void TestMainWindow::staleQueryDoesNotReplaceTheNewerList()
{
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QFETCH(bool, clearPath);

    QTemporaryDir release;
    QVERIFY(release.isValid());
    const QString releaseFile = release.filePath(QStringLiteral("release"));
    const QString markerFile = release.filePath(QStringLiteral("answered"));

    QByteArray slowScript = "#!/bin/sh\n"
                            "while [ ! -e '";
    slowScript += releaseFile.toUtf8();
    slowScript += "' ]; do sleep 0.05; done\n"
                  "printf '{\"1000\":{\"name\":\"NTLM\"}}\\n'\n"
                  "printf done > '";
    slowScript += markerFile.toUtf8();
    slowScript += "'\n"
                  "exit 0\n";

    const QString slowStub =
        TestEnvironment::writeShellStub(QStringLiteral("stub-hashcat-stale"), slowScript);
    const QString fastStub = TestEnvironment::writeShellStub(
        QStringLiteral("stub-hashcat-current"), "#!/bin/sh\n"
                                                "printf '{\"1400\":{\"name\":\"SHA2-256\"}}\\n'\n"
                                                "exit 0\n");
    QVERIFY2(!slowStub.isEmpty() && !fastStub.isEmpty(), "the shell stubs could not be written");

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, slowStub);

    MainWindow window;

    // The first query is still on its way; it is the one that will answer
    // last. The stub writes a marker once it has printed its reply, so the
    // test can wait for the stale answer without a watcher to observe.
    QVERIFY(!MainWindowTestAccess::ui(window)->comboBox_hash_type->isEnabled());

    // A second reload supersedes it before the stub has answered.
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       clearPath ? QString() : fastStub);
    MainWindowTestAccess::reloadHashTypes(window);

    if (clearPath) {
        QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->count(), 0);
        QVERIFY(MainWindowTestAccess::ui(window)->comboBox_hash_type->isEnabled());
        QVERIFY(MainWindowTestAccess::ui(window)->comboBox_hash_type->toolTip().contains(
            QStringLiteral("Settings")));
    } else {
        QTRY_COMPARE_WITH_TIMEOUT(MainWindowTestAccess::ui(window)->comboBox_hash_type->count(), 1,
                                  AppConstants::Hashcat::QueryTimeoutMs);
        QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->itemData(0).toUInt(), 1400U);
    }

    // Let the first query answer; the marker proves the stale reply was
    // produced. The loader dropped that watcher when the second reload
    // replaced it, so the reply cannot reach the combo.
    QFile releaseFlag(releaseFile);
    QVERIFY(releaseFlag.open(QIODevice::WriteOnly));
    releaseFlag.close();
    QTRY_VERIFY_WITH_TIMEOUT(QFile::exists(markerFile), AppConstants::Hashcat::QueryTimeoutMs);
    QTest::qWait(100);

    // The stale reply changed nothing: the current list is still the newer
    // one - or still empty after the path was cleared.
    if (clearPath) {
        QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->count(), 0);
        QVERIFY(MainWindowTestAccess::ui(window)->comboBox_hash_type->isEnabled());
        QVERIFY(MainWindowTestAccess::ui(window)->comboBox_hash_type->toolTip().contains(
            QStringLiteral("Settings")));
    } else {
        QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->count(), 1);
        QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->itemData(0).toUInt(), 1400U);
        QVERIFY(MainWindowTestAccess::ui(window)->comboBox_hash_type->isEnabled());
    }
#endif
}

// The suggestion may follow the hash file name only as long as nobody else
// has put something in the outfile field: what the user typed has to survive
// every later hash file edit, and an empty field is nobody's value at all.
void TestMainWindow::outfileSuggestionFollowsUntilTheUserTakesOver()
{
    MainWindow window;
    QLineEdit *hashFile = MainWindowTestAccess::ui(window)->lineEdit_hashfile;
    QLineEdit *outfile = MainWindowTestAccess::ui(window)->lineEdit_outfile;
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
    QListWidget *list = MainWindowTestAccess::ui(window)->listWidget_wordlist;

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

// The defaults the code documents and the defaults the .ui files bake in are
// two sources of the same truth; a widget that drifts from its constant makes
// the reset and the fresh window disagree.
void TestMainWindow::theDefaultsMatchTheUiFiles()
{
    MainWindow window;
    QCOMPARE(MainWindowTestAccess::ui(window)->lineEdit_outfile_format->text(),
             QString(AppConstants::Defaults::OutfileFormat));
    QCOMPARE(MainWindowTestAccess::ui(window)->lineEdit_backend_devices->text(),
             QString(AppConstants::Defaults::BackendDevices));
    QCOMPARE(MainWindowTestAccess::ui(window)->spinBox_runtime->value(),
             AppConstants::Defaults::Runtime);
    QCOMPARE(MainWindowTestAccess::ui(window)->spinBox_status_timer->value(),
             AppConstants::Defaults::StatusTimer);
    QCOMPARE(MainWindowTestAccess::ui(window)->spinBox_hwmon_temp_abort->value(),
             AppConstants::Defaults::HwmonTempAbort);
    QCOMPARE(MainWindowTestAccess::ui(window)->spinBox_increment_min->value(),
             AppConstants::Defaults::IncrementMin);
    QCOMPARE(MainWindowTestAccess::ui(window)->spinBox_increment_max->value(),
             AppConstants::Defaults::IncrementMax);
    QCOMPARE(MainWindowTestAccess::ui(window)->spinBox_generate_rules->value(),
             AppConstants::Defaults::GenerateRules);
}

// A mode switch is not a bulk write: applyAttackModeView only shows, hides
// and enables, so the explicit refresh in updateViewAttackMode is the one
// rebuild.
void TestMainWindow::switchingTheAttackModeRebuildsThePreviewOnce()
{
    MainWindow window;
    const int before = MainWindowTestAccess::previewRebuilds(window);
    MainWindowTestAccess::ui(window)->comboBox_attack->setCurrentIndex(1);
    QCOMPARE(MainWindowTestAccess::previewRebuilds(window) - before, 1);
}

// The list owners must be gone before delete ui: ~QListWidget resets its
// model while the handlers still run, and a handler reading a dead member is
// a crash. Rows in every list make the reset fire; the run under ASan is the
// real proof.
void TestMainWindow::destroyingTheWindowWithListRowsIsSafe()
{
    auto window = std::make_unique<MainWindow>();
    Ui::MainWindow *ui = MainWindowTestAccess::ui(*window);
    CheckedItemList::addPaths(ui->listWidget_wordlist, {QStringLiteral("/tmp/a.txt")});
    CheckedItemList::addPaths(ui->listWidget_pcfg_rulesets, {QStringLiteral("default-passwords")});
    CheckedItemList::addPaths(ui->listWidget_tables, {QStringLiteral("/tmp/t.table")});
    CheckedItemList::addPaths(ui->listWidget_feed_arguments, {QStringLiteral("arg")});

    window.reset();
    QVERIFY(true);
}

// A settings change that reloads the hash types must not touch the attack
// combo: refilling it would silently reset the mode to Straight.
void TestMainWindow::changingTheBinaryKeepsTheAttackMode()
{
    const QString stubA = TestEnvironment::writeShellStub(QStringLiteral("stub-hashcat-mode-a"),
                                                          "#!/bin/sh\n"
                                                          "printf '{\"0\":{\"name\":\"MD5\"}}\\n'\n"
                                                          "exit 0\n");
    const QString stubB = TestEnvironment::writeShellStub(
        QStringLiteral("stub-hashcat-mode-b"), "#!/bin/sh\n"
                                               "printf '{\"1400\":{\"name\":\"SHA2-256\"}}\\n'\n"
                                               "exit 0\n");
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stubA.isEmpty() && !stubB.isEmpty(), "the shell stubs could not be written");
#endif

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stubA);

    MainWindow window;
    QTRY_VERIFY_WITH_TIMEOUT(MainWindowTestAccess::ui(window)->comboBox_hash_type->count() == 1,
                             AppConstants::Hashcat::QueryTimeoutMs);
    MainWindowTestAccess::ui(window)->comboBox_attack->setCurrentIndex(2); // brute-force
    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_attack->currentData().toInt(),
             static_cast<int>(AttackMode::BruteForce));

    TestEnvironment::ModalRecorder recorder;
    recorder.saveSettingsDialog = true;
    recorder.start();

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stubB);
    QVERIFY(QMetaObject::invokeMethod(&window, "settingsTriggered"));

    // The hash-type list is replaced - the selection resets, expected because
    // the new binary may not offer the selected id - but the mode stays. Wait
    // for the new id, not the count: both lists hold one entry.
    QTRY_VERIFY_WITH_TIMEOUT(
        MainWindowTestAccess::ui(window)->comboBox_hash_type->itemData(0).toUInt() == 1400U,
        AppConstants::Hashcat::QueryTimeoutMs);
    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_attack->currentData().toInt(),
             static_cast<int>(AttackMode::BruteForce));
}

// An import that changes the binary re-runs the hash-type query, exactly as
// a save from the settings dialog does; the old list must not stay on screen.
void TestMainWindow::importingSettingsReRunsTheQueryWhenTheBinaryChanged()
{
    const QString stubA = TestEnvironment::writeShellStub(
        QStringLiteral("stub-hashcat-import-a"),
        "#!/bin/sh\n"
        "printf '{\"0\":{\"name\":\"MD5\"},\"1000\":{\"name\":\"NTLM\"}}\\n'\n"
        "exit 0\n");
    const QString stubB = TestEnvironment::writeShellStub(
        QStringLiteral("stub-hashcat-import-b"), "#!/bin/sh\n"
                                                 "printf '{\"1400\":{\"name\":\"SHA2-256\"}}\\n'\n"
                                                 "exit 0\n");
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stubA.isEmpty() && !stubB.isEmpty(), "the shell stubs could not be written");
#endif

    const QString file =
        TestEnvironment::configHome().filePath(QStringLiteral("import-binary.json"));
    QJsonObject values;
    values.insert(QStringLiteral("hashcatPath"), stubB);
    QVERIFY(StateFile::write(file, AppConstants::FileSections::Settings,
                             AppConstants::Settings::Version, values, nullptr));

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stubA);

    MainWindow window;
    QTRY_VERIFY_WITH_TIMEOUT(MainWindowTestAccess::ui(window)->comboBox_hash_type->count() == 2,
                             AppConstants::Hashcat::QueryTimeoutMs);

    TestEnvironment::ModalRecorder recorder;
    recorder.pickFile = file;
    recorder.saveSettingsDialog = true;
    recorder.start();

    QVERIFY(QMetaObject::invokeMethod(&window, "importSettingsTriggered"));

    QTRY_VERIFY_WITH_TIMEOUT(MainWindowTestAccess::ui(window)->comboBox_hash_type->count() == 1,
                             AppConstants::Hashcat::QueryTimeoutMs);
    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->itemData(0).toUInt(), 1400U);
}

// The wiring loop and the serializer share one walk: every command widget
// it connects has to reach the preview, and the excluded ones are the
// preview itself, the two staging fields and the attack combo's own slot.
void TestMainWindow::everyCommandWidgetReachesThePreview()
{
    MainWindow window;
    Ui::MainWindow *ui = MainWindowTestAccess::ui(window);
    // The hash combo is filled asynchronously in the application; fill it by
    // hand so its index connection is testable without a query.
    ui->comboBox_hash_type->addItem(QStringLiteral("0 | MD5"), 0);
    ui->comboBox_hash_type->addItem(QStringLiteral("1000 | NTLM"), 1000);

    const StateWidgetLists widgets = collectStateWidgets(&window);
    const auto rebuilds = [&window] { return MainWindowTestAccess::previewRebuilds(window); };
    int touched = 0;

    for (QLineEdit *edit : widgets.lineEdits) {
        if (edit == ui->lineEdit_command || edit == ui->lineEdit_feed_argument
            || edit == ui->lineEdit_pcfg_ruleset_name) {
            continue;
        }
        const int before = rebuilds();
        edit->setText(edit->text() + QStringLiteral("x"));
        QVERIFY2(rebuilds() > before, qPrintable(edit->objectName()));
        ++touched;
    }
    for (QCheckBox *checkBox : widgets.checkBoxes) {
        const int before = rebuilds();
        checkBox->setChecked(!checkBox->isChecked());
        QVERIFY2(rebuilds() > before, qPrintable(checkBox->objectName()));
        ++touched;
    }
    for (QRadioButton *radioButton : widgets.radioButtons) {
        const int before = rebuilds();
        if (radioButton->isChecked()) {
            // An auto-exclusive radio cannot be unchecked programmatically:
            // check the other side of its group instead.
            const auto buttons = radioButton->group()->buttons();
            for (QAbstractButton *other : buttons) {
                if (other != radioButton) {
                    other->setChecked(true);
                    break;
                }
            }
        } else {
            radioButton->setChecked(true);
        }
        QVERIFY2(rebuilds() > before, qPrintable(radioButton->objectName()));
        ++touched;
    }
    for (QSpinBox *spinBox : widgets.spinBoxes) {
        const int before = rebuilds();
        spinBox->setValue(spinBox->value() == spinBox->maximum() ? spinBox->value() - 1
                                                                 : spinBox->value() + 1);
        QVERIFY2(rebuilds() > before, qPrintable(spinBox->objectName()));
        ++touched;
    }
    for (QComboBox *comboBox : widgets.comboBoxes) {
        const int before = rebuilds();
        comboBox->setCurrentIndex(comboBox->currentIndex() == 0 ? 1 : 0);
        if (comboBox == ui->comboBox_attack) {
            // Its own slot refreshes the preview exactly once.
            QCOMPARE(rebuilds() - before, 1);
        } else {
            QVERIFY2(rebuilds() > before, qPrintable(comboBox->objectName()));
        }
        ++touched;
    }

    // 41 line edits, 22 checkboxes, 2 radios, 6 spin boxes, 2 combos.
    QCOMPARE(touched, 73);
}

// One owner per connection: a single list row event rebuilds the preview
// exactly once. The controller's changed() signal must not reach
// commandChanged() through two connections.
void TestMainWindow::listRowEventsRebuildThePreviewOnce()
{
    MainWindow window;
    QListWidget *list = MainWindowTestAccess::ui(window)->listWidget_wordlist;

    // A bare insert: one rowsInserted event.
    const int beforeInsert = MainWindowTestAccess::previewRebuilds(window);
    list->addItem(QStringLiteral("/tmp/a.txt"));
    QCOMPARE(MainWindowTestAccess::previewRebuilds(window) - beforeInsert, 1);

    // A check-state change: one itemChanged event.
    const int beforeCheck = MainWindowTestAccess::previewRebuilds(window);
    list->item(0)->setCheckState(Qt::Checked);
    QCOMPARE(MainWindowTestAccess::previewRebuilds(window) - beforeCheck, 1);

    // A removal: one rowsRemoved event.
    const int beforeRemove = MainWindowTestAccess::previewRebuilds(window);
    delete list->takeItem(0);
    QCOMPARE(MainWindowTestAccess::previewRebuilds(window) - beforeRemove, 1);
}

// A failed reload must not keep the previous binary's list: a stale id in
// the preview would reach the launch validation, and the empty combo is what
// lets the same-path retry re-query.
void TestMainWindow::aFailedReloadClearsTheHashTypes()
{
    const QString stub = TestEnvironment::writeShellStub(QStringLiteral("stub-hashcat-fail-clear"),
                                                         "#!/bin/sh\n"
                                                         "printf '{\"0\":{\"name\":\"MD5\"}}\\n'\n"
                                                         "exit 0\n");
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stub.isEmpty(), "the shell stub could not be written");
#endif
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stub);

    m_fixture.startWatchdog();
    MainWindow window;
    QTRY_VERIFY_WITH_TIMEOUT(MainWindowTestAccess::ui(window)->comboBox_hash_type->count() == 1,
                             AppConstants::Hashcat::QueryTimeoutMs);

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("/nonexistent/hashcat-broken"));
    MainWindowTestAccess::reloadHashTypes(window);
    QTRY_VERIFY_WITH_TIMEOUT(!MainWindowTestAccess::hashTypeLoader(window).isLoading(),
                             AppConstants::Hashcat::QueryTimeoutMs);

    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->count(), 0);
    QVERIFY(!MainWindowTestAccess::ui(window)->lineEdit_command->text().contains(
        QStringLiteral("--hash-type")));
}

// The recovery path: the binary at the same path is repaired, and accepting
// Settings again re-queries because the failed reload left the combo empty.
void TestMainWindow::retryingTheSamePathAfterAFailureReQueries()
{
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString binary = directory.filePath(QStringLiteral("hashcat-repaired"));
    QFile script(binary);
    QVERIFY(script.open(QIODevice::WriteOnly));
    script.write("#!/bin/sh\nexit 1\n");
    script.close();
    QVERIFY(script.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner));

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, binary);

    m_fixture.startWatchdog();
    MainWindow window;
    QTRY_VERIFY_WITH_TIMEOUT(!MainWindowTestAccess::hashTypeLoader(window).isLoading(),
                             AppConstants::Hashcat::QueryTimeoutMs);
    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->count(), 0);

    // Repair the binary at the same path, then accept Settings again. The
    // watchdog has done its job (the failure box); stop it so it cannot race
    // the recorder for the settings dialog.
    m_fixture.stopWatchdog();
    QVERIFY(script.open(QIODevice::WriteOnly | QIODevice::Truncate));
    script.write("#!/bin/sh\nprintf '{\"1400\":{\"name\":\"SHA2-256\"}}\\n'\nexit 0\n");
    script.close();

    TestEnvironment::ModalRecorder recorder;
    recorder.saveSettingsDialog = true;
    recorder.start();
    QVERIFY(QMetaObject::invokeMethod(&window, "settingsTriggered"));

    QTRY_VERIFY_WITH_TIMEOUT(MainWindowTestAccess::ui(window)->comboBox_hash_type->count() == 1,
                             AppConstants::Hashcat::QueryTimeoutMs);
    QCOMPARE(MainWindowTestAccess::ui(window)->comboBox_hash_type->itemData(0).toUInt(), 1400U);
#endif
}

// The refill is a bulk write: a reload that replaces the list costs exactly
// one preview rebuild, not one per combo event.
void TestMainWindow::aReloadRebuildsThePreviewOnce()
{
    const QString stubA = TestEnvironment::writeShellStub(QStringLiteral("stub-hashcat-reload-a"),
                                                          "#!/bin/sh\n"
                                                          "printf '{\"0\":{\"name\":\"MD5\"}}\\n'\n"
                                                          "exit 0\n");
    const QString stubB = TestEnvironment::writeShellStub(
        QStringLiteral("stub-hashcat-reload-b"), "#!/bin/sh\n"
                                                 "printf '{\"1000\":{\"name\":\"NTLM\"}}\\n'\n"
                                                 "exit 0\n");
#ifdef Q_OS_WIN
    QSKIP("no shell stub on this platform");
#else
    QVERIFY2(!stubA.isEmpty() && !stubB.isEmpty(), "the shell stubs could not be written");
#endif
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stubA);

    MainWindow window;
    QTRY_VERIFY_WITH_TIMEOUT(MainWindowTestAccess::ui(window)->comboBox_hash_type->count() == 1,
                             AppConstants::Hashcat::QueryTimeoutMs);

    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, stubB);
    const int before = MainWindowTestAccess::previewRebuilds(window);
    MainWindowTestAccess::reloadHashTypes(window);
    QTRY_VERIFY_WITH_TIMEOUT(
        MainWindowTestAccess::ui(window)->comboBox_hash_type->itemData(0).toUInt() == 1000U,
        AppConstants::Hashcat::QueryTimeoutMs);
    QCOMPARE(MainWindowTestAccess::previewRebuilds(window) - before, 1);
}

int main(int argc, char *argv[])
{
    TestMainWindow test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_mainwindow.moc"
