/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QListWidgetItem>
#include <QStandardPaths>
#include <QTimer>
#include <QWidget>

#include "appconstants.h"
#include "commandbuilder.h"
#include "hashcatoptions.h"
#include "mainwindow.h"
#include "settingsmanager.h"
#include "ui_mainwindow.h"

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
        // Safety net for ctest, for the tests that spin the event loop: a
        // reply the stub cannot produce opens a modal warning, and a modal
        // dialog must never be able to hang the run. Repeating with a short
        // interval, because a warning can only appear while an event loop
        // is turning - which is when this timer ticks - and the failure-path
        // tests below want it gone the moment it is there. Living as a
        // member lets cleanup() stop it, so it cannot fire into a later
        // test and close a modal that test legitimately opened.
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
    void attackModeIdsTravelAsItemData();
    void groupBoxesFollowTheBuilderPredicates();
    void widgetsFlowIntoTheOptions();
    void checkedWordlistsEndUpInTheCommand();
    void hashTypeComesFromTheItemData();
    void hashTypesAreFilledFromHashcat();
    void failedQueryReenablesTheHashTypeCombo_data();
    void failedQueryReenablesTheHashTypeCombo();
    void outfileSuggestionFollowsUntilTheUserTakesOver();
    void wordlistSortSurvivesAMissingSelection();
    void commandPreviewCarriesTheBinaryName();
    void defaultProfileRoundTrips();
    void profileHashTypeSurvivesTheAsynchronousQuery();

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

    // What each mode owes the widgets, written down independently of the very
    // predicates updateViewAttackMode() switches on: the same ground truth
    // tst_commandbuilder::predicates pins for the builder, asserted here
    // against the widgets. A wrong predicate now disagrees with this table.
    struct Expectation {
        AttackMode mode;
        bool wordlists;
        bool rules;
        bool mask;
    };
    const QList<Expectation> expectations{
        {AttackMode::Straight, true, true, false},
        {AttackMode::Combination, true, false, false},
        {AttackMode::BruteForce, false, false, true},
        {AttackMode::HybridWordMask, true, false, true},
        {AttackMode::HybridMaskWord, true, false, true},
        {AttackMode::Association, true, true, false},
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
        QCOMPARE(window.ui->groupBox_mask->isEnabled(), expected->mask);
        QCOMPARE(window.ui->groupBox_custom_charset->isEnabled(), expected->mask);
    }
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

    const HashcatOptions options = window.collectHashcatOptions();
    QCOMPARE(options.hashFile, QStringLiteral("/tmp/example.hash"));
    QCOMPARE(options.mask, QStringLiteral("?d?d"));
    QVERIFY(options.remove);
    QVERIFY(options.ignoreUsername);
    QVERIFY(options.speedOnly);
    QCOMPARE(options.workloadProfile, QStringLiteral("2"));

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
    QVERIFY(!arguments.contains(QStringLiteral("--segment-size")));
    QVERIFY(!arguments.contains(QStringLiteral("--outfile")));

    // ... and a segment size that is no longer the default does show up
    window.ui->spinBox_segment->setValue(64);
    QCOMPARE(window.collectHashcatOptions().segmentSize, 64);
    QVERIFY(window.generateArguments().contains(QStringLiteral("--segment-size")));
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
    window.ui->comboBox_hash->clear();
    window.ui->comboBox_hash->addItem(QStringLiteral("1000 | NTLM"), 1000);
    window.ui->comboBox_hash->addItem(QStringLiteral("0 | MD5"), 0);
    window.ui->comboBox_hash->setCurrentIndex(0);

    QCOMPARE(window.collectHashcatOptions().hashType, 1000);
    const QStringList withHashType{"--hash-type", "1000", "--attack-mode", "0"};
    QCOMPARE(window.generateArguments(), withHashType);

    // Nothing selected - the state before the query finished - must leave -m
    // out instead of silently asking for hash type 0
    window.ui->comboBox_hash->clear();
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
    QVERIFY(!window.ui->comboBox_hash->isEnabled());
    QCOMPARE(window.ui->comboBox_hash->toolTip(), QStringLiteral("Updating..."));

    // Wait at least as long as the query's own deadline: giving up earlier
    // would fail the test while hashcat could still have answered
    QTRY_VERIFY_WITH_TIMEOUT(window.ui->comboBox_hash->count() > 0,
                             AppConstants::Hashcat::QueryTimeoutMs);

    QCOMPARE(window.ui->comboBox_hash->count(), 2);
    QVERIFY(window.ui->comboBox_hash->isEnabled());
    QVERIFY(window.ui->comboBox_hash->toolTip().isEmpty());

    // 0 sorts before 1000, and the ids travel as item data
    QCOMPARE(window.ui->comboBox_hash->itemData(0).toUInt(), 0u);
    QCOMPARE(window.ui->comboBox_hash->itemData(1).toUInt(), 1000u);

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
    QVERIFY(!window.ui->comboBox_hash->isEnabled());
    QCOMPARE(window.ui->comboBox_hash->toolTip(), QStringLiteral("Updating..."));

    // ... and when the reply turns out to be unusable, the combo comes back
    QTRY_VERIFY_WITH_TIMEOUT(window.ui->comboBox_hash->isEnabled(),
                             AppConstants::Hashcat::QueryTimeoutMs);
    QVERIFY(window.ui->comboBox_hash->toolTip().isEmpty());
    QCOMPARE(window.ui->comboBox_hash->count(), 0);
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

// The profile machinery as the application uses it: saveDefaultProfile()
// (the aboutToQuit handler) and the loadDefaultProfile() every constructor
// runs. Two things are pinned here that no other test reaches: the command
// preview stays out of the file, and a new window starts from the stored
// state instead of the widget defaults.
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
    writer.ui->spinBox_segment->setValue(64);       // not the default 32
    writer.saveDefaultProfile();

    const QString profile = writer.defaultProfileFile();
    QVERIFY(QFile::exists(profile));

    // What gets stored: the window under its class name, and without the
    // command preview - that one is regenerated from the state on load.
    QFile file(profile);
    QVERIFY(file.open(QIODevice::ReadOnly));
    const QJsonObject root = QJsonDocument::fromJson(file.readAll()).object();
    file.close();

    const QJsonObject state =
        root.value(QString::fromUtf8(writer.metaObject()->className())).toObject();
    QVERIFY(!state.isEmpty());
    QVERIFY(!state.contains(QLatin1String(AppConstants::Files::PreviewWidget)));
    QCOMPARE(state.value(QStringLiteral("comboBox_attack")).toInt(), 2);

    // A new window runs loadDefaultProfile() in its constructor...
    MainWindow reloaded;
    QCOMPARE(reloaded.ui->comboBox_attack->currentIndex(), 2);
    QCOMPARE(reloaded.ui->spinBox_segment->value(), 64);

    // ...and the preview reflects the restored state, not the defaults the
    // widgets held before the profile arrived.
    QCOMPARE(reloaded.ui->lineEdit_command->text(),
             QStringLiteral("hashcat --attack-mode 3 --segment-size 64"));
}

// The constructor loads the profile while the hashcat query that fills
// comboBox_hash is still in flight: the combo has no items, so
// setCurrentIndex() is a silent no-op and the stored index can only be
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

    // A profile that picked the second entry: the ids sort 0, 1000, so the
    // selection must land on NTLM and not on the first item the fill leaves.
    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QVERIFY(QDir().mkpath(dir));
    QFile profile(QDir(dir).filePath(AppConstants::Files::DefaultProfile));
    QVERIFY(profile.open(QIODevice::WriteOnly | QIODevice::Truncate));
    profile.write(R"({"MainWindow": {"comboBox_hash": 1}})");
    profile.close();

    m_watchdog.start();

    MainWindow window;

    // The query runs off-thread; wait at least as long as its own deadline
    // so the test never gives up while hashcat could still have answered.
    QTRY_VERIFY_WITH_TIMEOUT(window.ui->comboBox_hash->count() > 0,
                             AppConstants::Hashcat::QueryTimeoutMs);

    QCOMPARE(window.ui->comboBox_hash->currentIndex(), 1);
    QCOMPARE(window.ui->comboBox_hash->currentData().toUInt(), 1000u);
    QCOMPARE(window.m_pendingHashTypeIndex, -1); // applied and consumed
}

int main(int argc, char *argv[])
{
    TestMainWindow test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_mainwindow.moc"
