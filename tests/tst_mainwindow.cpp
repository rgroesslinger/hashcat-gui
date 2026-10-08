/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QListWidgetItem>
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
        // Safety net for ctest, for the test that spins the event loop: a
        // reply the stub cannot produce would open a modal warning, and a
        // modal dialog must never be able to hang the run. Living as a member
        // lets cleanup() stop it, so it cannot fire into a later test and
        // close a modal that test legitimately opened.
        m_watchdog.setSingleShot(true);
        m_watchdog.setInterval(10000);
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

    auto &settings = SettingsManager::instance();
    settings.setKey(AppConstants::SettingsKeys::HashcatPath, m_savedHashcatPath);
    settings.setKey(AppConstants::SettingsKeys::Terminal, m_savedTerminal);
    settings.setKey(AppConstants::SettingsKeys::UseShortParameters, m_savedShortParameters);
}

void TestMainWindow::freshWindowMatchesTheDefaultOptions()
{
    MainWindow window;

    const HashcatOptions expected;
    const QStringList arguments = CommandBuilder::build(expected);

    QCOMPARE(window.generateArguments(), arguments);

    // ... and that is what the preview shows, character for character
    QCOMPARE(window.ui->lineEdit_command->text(), arguments.join(QLatin1Char(' ')));
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

    for (int i = 0; i < window.ui->comboBox_attack->count(); ++i) {
        // -1 first, so that every iteration really changes the index and runs
        // attackIndexChanged()
        window.ui->comboBox_attack->setCurrentIndex(-1);
        window.ui->comboBox_attack->setCurrentIndex(i);

        const AttackMode mode =
            static_cast<AttackMode>(window.ui->comboBox_attack->currentData().toInt());

        QCOMPARE(window.ui->groupBox_wordlists->isEnabled(),
                 CommandBuilder::attackUsesWordlists(mode));
        QCOMPARE(window.ui->groupBox_rules->isEnabled(), CommandBuilder::attackUsesRules(mode));
        QCOMPARE(window.ui->groupBox_mask->isEnabled(), CommandBuilder::attackUsesMask(mode));
        QCOMPARE(window.ui->groupBox_custom_charset->isEnabled(),
                 CommandBuilder::attackUsesMask(mode));
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

    QTRY_VERIFY_WITH_TIMEOUT(window.ui->comboBox_hash->count() > 0, 15000);

    QCOMPARE(window.ui->comboBox_hash->count(), 2);
    QVERIFY(window.ui->comboBox_hash->isEnabled());
    QVERIFY(window.ui->comboBox_hash->toolTip().isEmpty());

    // 0 sorts before 1000, and the ids travel as item data
    QCOMPARE(window.ui->comboBox_hash->itemData(0).toUInt(), 0u);
    QCOMPARE(window.ui->comboBox_hash->itemData(1).toUInt(), 1000u);

    const QStringList arguments = window.generateArguments();
    const int hashType = arguments.indexOf(QStringLiteral("--hash-type"));
    QVERIFY(hashType >= 0);
    QCOMPARE(arguments.value(hashType + 1), QStringLiteral("0"));
}

int main(int argc, char *argv[])
{
    TestMainWindow test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_mainwindow.moc"
