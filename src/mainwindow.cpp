/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "settingsdialog.h"
#include "settingsmanager.h"
#include "helperutils.h"
#include "widgetstateserializer.h"
#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonObject>
#include <QAbstractItemModel>
#include <QClipboard>

#if defined(Q_OS_WIN)
#include <process.h>
#include <windows.h>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto &settings = SettingsManager::instance();

    initHashAndAttackModes();
    updateViewAttackMode();

    /* ---------- wordlist ---------- */
    connect(ui->listWidget_wordlist->model(), &QAbstractItemModel::rowsInserted, this, [this] { commandChanged(); });
    connect(ui->listWidget_wordlist->model(), &QAbstractItemModel::rowsRemoved, this, [this] { commandChanged(); });
    connect(ui->listWidget_wordlist->model(), &QAbstractItemModel::rowsMoved, this, [this] { commandChanged(); });
    connect(ui->listWidget_wordlist, &QListWidget::itemChanged, this, [this] { commandChanged(); });

    /* ---------- copy to clipboard ---------- */
    connect(ui->pushButton_copy_clipboard, &QPushButton::clicked, this, &MainWindow::copyCommandToClipboard);

    /* ---------- workload tuning ---------- */
    connect(ui->checkBox_override_workload_profile, &QCheckBox::toggled, ui->comboBox_workload_profile, &QComboBox::setEnabled);

    /* ---------- menu actions ---------- */
    connect(ui->actionHelp_About, &QAction::triggered, this, &MainWindow::aboutTriggered);
    connect(ui->actionReset_fields, &QAction::triggered, this, &MainWindow::resetFieldsTriggered);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quitTriggered);
    connect(ui->actionExport, &QAction::triggered, this, &MainWindow::exportTriggered);
    connect(ui->actionImport, &QAction::triggered, this, &MainWindow::importTriggered);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::settingsTriggered);
    connect(ui->actionAbout_Qt, &QAction::triggered, this, &MainWindow::aboutQtTriggered);

    /* ---------- main‑tab buttons ---------- */
    connect(ui->pushButton_execute, &QPushButton::clicked, this, &MainWindow::executeClicked);
    connect(ui->pushButton_open_hashfile, &QPushButton::clicked, this, &MainWindow::openHashFileClicked);
    connect(ui->pushButton_output, &QPushButton::clicked, this, &MainWindow::outputClicked);
    connect(ui->pushButton_remove_wordlist, &QPushButton::clicked, this, &MainWindow::removeWordlistClicked);
    connect(ui->pushButton_add_wordlist, &QPushButton::clicked, this, &MainWindow::addWordlistClicked);
    connect(ui->toolButton_wordlist_sort_asc, &QToolButton::clicked, this, &MainWindow::wordlistSortAscClicked);
    connect(ui->toolButton_wordlist_sort_desc, &QToolButton::clicked, this, &MainWindow::wordlistSortDescClicked);
    connect(ui->listWidget_wordlist, &QListWidget::itemClicked, this, &MainWindow::wordlistItemClicked);

    /* ---------- checkboxes ---------- */
    connect(ui->checkBox_outfile, &QCheckBox::toggled, this, &MainWindow::outfileToggled);
    connect(ui->checkBox_rulesfile_1, &QCheckBox::toggled, this, &MainWindow::rulesfile1Toggled);
    connect(ui->checkBox_rulesfile_2, &QCheckBox::toggled, this, &MainWindow::rulesfile2Toggled);
    connect(ui->checkBox_rulesfile_3, &QCheckBox::toggled, this, &MainWindow::rulesfile3Toggled);
    connect(ui->radioButton_generate_rules, &QRadioButton::toggled, this, &MainWindow::generateRulesToggled);
    connect(ui->radioButton_use_rules_file, &QRadioButton::toggled, this, &MainWindow::useRulesFileToggled);
    connect(ui->checkBox_custom1, &QCheckBox::toggled, this, &MainWindow::custom1Toggled);
    connect(ui->checkBox_custom2, &QCheckBox::toggled, this, &MainWindow::custom2Toggled);
    connect(ui->checkBox_custom3, &QCheckBox::toggled, this, &MainWindow::custom3Toggled);
    connect(ui->checkBox_custom4, &QCheckBox::toggled, this, &MainWindow::custom4Toggled);

    /* ---------- line edits ---------- */
    connect(ui->lineEdit_hashfile, &QLineEdit::textChanged, this, &MainWindow::hashFileTextChanged);

    /* ---------- rule‑file buttons ---------- */
    connect(ui->pushButton_open_rulesfile_1, &QPushButton::clicked, this, &MainWindow::openRulesFile1Clicked);
    connect(ui->pushButton_open_rulesfile_2, &QPushButton::clicked, this, &MainWindow::openRulesFile2Clicked);
    connect(ui->pushButton_open_rulesfile_3, &QPushButton::clicked, this, &MainWindow::openRulesFile3Clicked);

    /* ---------- combobox ---------- */
    connect(ui->comboBox_attack, &QComboBox::currentIndexChanged, this, &MainWindow::attackIndexChanged);

    /* ---------- show Settings if hashcatPath not set ---------- */
    if (settings.getKey<QString>("hashcatPath").isEmpty()) {
        QMetaObject::invokeMethod(this, &MainWindow::settingsTriggered, Qt::QueuedConnection);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*************** Menu Bar ***************/

// File → Export
void MainWindow::exportTriggered()
{
    QStringList ignoreWidgets = { "lineEdit_command" };

    QString file = QFileDialog::getSaveFileName(
        this, tr("Save Profile"),
        QString(),
        tr("JSON Files (*.json)"));

    if (!file.isEmpty()) {
        WidgetStateSerializer s;
        s.saveStateToFile(QString::fromUtf8(metaObject()->className()), this, file, ignoreWidgets);
        QMessageBox::information(this, tr("Saved"), tr("Profile saved to %1.").arg(file));
    }
}

// File → Import
void MainWindow::importTriggered()
{
    QString file = QFileDialog::getOpenFileName(
        this, tr("Load Profile"),
        QString(),
        tr("JSON Files (*.json)"));

    if (!file.isEmpty()) {
        WidgetStateSerializer s;
        if (s.loadStateFromFile(QString::fromUtf8(metaObject()->className()), this, file)) {
            commandChanged();
            QMessageBox::information(this, tr("Loaded"), tr("Profile loaded from %1.").arg(file));
        }
    }
}

// Tools → Reset field
void MainWindow::resetFieldsTriggered()
{
    // Main tab
    ui->lineEdit_hashfile->clear();
    ui->checkBox_ignoreusername->setChecked(false);
    ui->checkBox_remove->setChecked(false);
    ui->listWidget_wordlist->clear();
    ui->comboBox_attack->setCurrentIndex(0);
    ui->comboBox_hash->setCurrentIndex(0);
    ui->radioButton_use_rules_file->setChecked(true);
    ui->checkBox_rulesfile_1->setChecked(false);
    ui->checkBox_rulesfile_2->setChecked(false);
    ui->checkBox_rulesfile_3->setChecked(false);
    ui->lineEdit_open_rulesfile_1->clear();
    ui->lineEdit_open_rulesfile_2->clear();
    ui->lineEdit_open_rulesfile_3->clear();
    ui->spinBox_generate_rules->setValue(1);
    ui->lineEdit_mask->clear();
    ui->checkBox_custom1->setChecked(false);
    ui->lineEdit_custom1->clear();
    ui->checkBox_custom2->setChecked(false);
    ui->lineEdit_custom2->clear();
    ui->checkBox_custom3->setChecked(false);
    ui->lineEdit_custom3->clear();
    ui->checkBox_custom4->setChecked(false);
    ui->lineEdit_custom4->clear();
    ui->checkBox_hex_hash->setChecked(false);
    ui->checkBox_hex_salt->setChecked(false);
    ui->checkBox_outfile->setChecked(false);
    ui->lineEdit_outfile->clear();
    ui->lineEdit_outfile_format->setText("1,2");
    ui->lineEdit_cpu_affinity->clear();
    ui->lineEdit_devices->setText("0");
    ui->spinBox_segment->setValue(32);

    // Advanced tab
    ui->checkBox_override_workload_profile->setChecked(false);
    ui->comboBox_workload_profile->setCurrentIndex(0);

    commandChanged();
}

// File → Settings
void MainWindow::settingsTriggered()
{
    SettingsDialog settingsDialog(this);
    if (settingsDialog.exec() == QDialog::Accepted) {
        // If SettingsDialog was saved and there are no hash types yet maybe we can populate them now
        if (ui->comboBox_hash->count() == 0) {
            initHashAndAttackModes();
        }
    }
}

// File → Quit
void MainWindow::quitTriggered()
{
    qApp->quit();
}

// Help → About Qt
void MainWindow::aboutQtTriggered()
{
    QApplication::aboutQt();
}

// Help → About
void MainWindow::aboutTriggered()
{
    AboutDialog about(this);
    about.exec();
}

/*************** MainWindow ***************/

void MainWindow::initHashAndAttackModes()
{
    ui->comboBox_attack->clear();
    ui->comboBox_hash->clear();
    attackModes.clear();
    hashModes.clear();

    // Attack modes
    attackModes.insert(AttackMode::Straight, "Straight");
    attackModes.insert(AttackMode::Combination, "Combination");
    attackModes.insert(AttackMode::BruteForce, "Brute-force");
    attackModes.insert(AttackMode::HybridWordMask, "Hybrid Wordlist + Mask");
    attackModes.insert(AttackMode::HybridMaskWord, "Hybrid Mask + Wordlist");
    attackModes.insert(AttackMode::Association, "Association");

    // Hash types
    auto &settings = SettingsManager::instance();
    QString hashTypes;

    // Read list of example hashes, returns JSON
    if (!settings.getKey<QString>("hashcatPath").isEmpty()) {
        hashTypes = HelperUtils::executeHashcat(QStringList() << "--example-hashes" << "--machine-readable")
                        .remove('\n')
                        .remove('\r');
    }

    // Fill QComboBox with available hash types
    QJsonDocument doc = QJsonDocument::fromJson(hashTypes.toUtf8());
    if (doc.isObject()) {
        QJsonObject rootObj = doc.object();

        for (auto it = rootObj.constBegin(); it != rootObj.constEnd(); ++it) {
            hashModes.insert(it.key().toInt(),
                             QString(it.key() + " | " + it.value().toObject().value("name").toString()));
        }
    }

    // Fill combo boxes
    for (const auto &value : std::as_const(attackModes)) {
        ui->comboBox_attack->addItem(value);
    }

    for (const auto &value : std::as_const(hashModes)) {
        ui->comboBox_hash->addItem(value);
    }
}

void MainWindow::attackIndexChanged([[maybe_unused]] int index)
{
    updateViewAttackMode();
}

void MainWindow::updateViewAttackMode()
{
    int attackMode = attackModes.key(ui->comboBox_attack->currentText());
    bool groupWordlists = false, groupRules = false, groupMask = false;

    switch (attackMode) {
    case AttackMode::Straight:
        groupWordlists = true;
        groupRules = true;
        groupMask = false;
        break;
    case AttackMode::Combination:
        groupWordlists = true;
        groupRules = false;
        groupMask = false;
        break;
    case AttackMode::BruteForce:
        groupWordlists = false;
        groupRules = false;
        groupMask = true;
        break;
    case AttackMode::HybridWordMask:
    case AttackMode::HybridMaskWord:
        groupWordlists = true;
        groupRules = false;
        groupMask = true;
        break;
    case AttackMode::Association:
        groupWordlists = true;
        groupRules = true;
        groupMask = false;
        break;
    }

    ui->groupBox_wordlists->setEnabled(groupWordlists);
    ui->groupBox_rules->setEnabled(groupRules);
    ui->groupBox_custom_charset->setEnabled(groupMask);
    ui->groupBox_mask->setEnabled(groupMask);
    commandChanged();
}

void MainWindow::openHashFileClicked()
{
    QString hashfile = QFileDialog::getOpenFileName();
    if (!hashfile.isEmpty()) {
        ui->lineEdit_hashfile->setText(QDir::toNativeSeparators(hashfile));
    }
}

void MainWindow::outputClicked()
{
    QString outfile = QFileDialog::getSaveFileName();
    if (!outfile.isEmpty()) {
        ui->lineEdit_outfile->setText(QDir::toNativeSeparators(outfile));
    }
}

void MainWindow::removeWordlistClicked()
{
    qDeleteAll(ui->listWidget_wordlist->selectedItems());
    ui->pushButton_remove_wordlist->setEnabled(false);
    ui->toolButton_wordlist_sort_asc->setEnabled(false);
    ui->toolButton_wordlist_sort_desc->setEnabled(false);
    ui->listWidget_wordlist->clearSelection();
}

void MainWindow::addWordlistClicked()
{
    QStringList files = QFileDialog::getOpenFileNames();
    QListWidget *w = ui->listWidget_wordlist;

    for (const QString &wordlist : std::as_const(files)) {
        // A file has been selected and it is not already in the list
        if (!wordlist.isEmpty() && w->findItems(wordlist, Qt::MatchExactly).isEmpty()) {
            QListWidgetItem *newItem = new QListWidgetItem(wordlist, w);
            newItem->setCheckState(Qt::Checked);
        }
    }
}

void MainWindow::wordlistSortAscClicked()
{
    int currentRow = ui->listWidget_wordlist->currentRow();
    if (currentRow == 0) return;
    QListWidgetItem *currentItem = ui->listWidget_wordlist->takeItem(currentRow);
    ui->listWidget_wordlist->insertItem(currentRow - 1, currentItem);
    ui->listWidget_wordlist->setCurrentRow(currentRow - 1);
}

void MainWindow::wordlistSortDescClicked()
{
    int currentRow = ui->listWidget_wordlist->currentRow();
    if (currentRow >= ui->listWidget_wordlist->count() - 1) return;
    QListWidgetItem *currentItem = ui->listWidget_wordlist->takeItem(currentRow);
    ui->listWidget_wordlist->insertItem(currentRow + 1, currentItem);
    ui->listWidget_wordlist->setCurrentRow(currentRow + 1);
}

void MainWindow::wordlistItemClicked([[maybe_unused]] QListWidgetItem *item)
{
    ui->pushButton_remove_wordlist->setEnabled(true);
    ui->toolButton_wordlist_sort_asc->setEnabled(true);
    ui->toolButton_wordlist_sort_desc->setEnabled(true);
}

void MainWindow::rulesfile1Toggled(bool checked)
{
    ui->lineEdit_open_rulesfile_1->setEnabled(checked);
    ui->pushButton_open_rulesfile_1->setEnabled(checked);
}

void MainWindow::rulesfile2Toggled(bool checked)
{
    ui->lineEdit_open_rulesfile_2->setEnabled(checked);
    ui->pushButton_open_rulesfile_2->setEnabled(checked);
}

void MainWindow::rulesfile3Toggled(bool checked)
{
    ui->lineEdit_open_rulesfile_3->setEnabled(checked);
    ui->pushButton_open_rulesfile_3->setEnabled(checked);
}

void MainWindow::useRulesFileToggled(bool checked)
{
    ui->checkBox_rulesfile_1->setEnabled(checked);
    ui->checkBox_rulesfile_2->setEnabled(checked);
    ui->checkBox_rulesfile_3->setEnabled(checked);
    if (checked) {
        rulesfile1Toggled(ui->checkBox_rulesfile_1->isChecked());
        rulesfile2Toggled(ui->checkBox_rulesfile_2->isChecked());
        rulesfile3Toggled(ui->checkBox_rulesfile_3->isChecked());
    }
}

void MainWindow::generateRulesToggled(bool checked)
{
    ui->spinBox_generate_rules->setEnabled(checked);
    if (checked) {
        useRulesFileToggled(false);
        rulesfile1Toggled(false);
        rulesfile2Toggled(false);
        rulesfile3Toggled(false);
    }
}

void MainWindow::openRulesFile1Clicked()
{
    QString rulesfile = QFileDialog::getOpenFileName();
    if (!rulesfile.isEmpty()) {
        ui->lineEdit_open_rulesfile_1->setText(QDir::toNativeSeparators(rulesfile));
    }
}

void MainWindow::openRulesFile2Clicked()
{
    QString rulesfile = QFileDialog::getOpenFileName();
    if (!rulesfile.isEmpty()) {
        ui->lineEdit_open_rulesfile_2->setText(QDir::toNativeSeparators(rulesfile));
    }
}

void MainWindow::openRulesFile3Clicked()
{
    QString rulesfile = QFileDialog::getOpenFileName();
    if (!rulesfile.isEmpty()) {
        ui->lineEdit_open_rulesfile_3->setText(QDir::toNativeSeparators(rulesfile));
    }
}

void MainWindow::custom1Toggled(bool checked)
{
    ui->lineEdit_custom1->setEnabled(checked);
}

void MainWindow::custom2Toggled(bool checked)
{
    ui->lineEdit_custom2->setEnabled(checked);
}

void MainWindow::custom3Toggled(bool checked)
{
    ui->lineEdit_custom3->setEnabled(checked);
}

void MainWindow::custom4Toggled(bool checked)
{
    ui->lineEdit_custom4->setEnabled(checked);
}

void MainWindow::outfileToggled(bool checked)
{
    ui->lineEdit_outfile->setEnabled(checked);
    ui->pushButton_output->setEnabled(checked);
}

void MainWindow::hashFileTextChanged(const QString &text)
{
    if (!text.isEmpty()) {
        ui->lineEdit_outfile->setText(text + ".out");
    }
}

void MainWindow::copyCommandToClipboard()
{
    QString text = ui->lineEdit_command->text();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(text);
}

void MainWindow::executeClicked()
{
    auto &settings = SettingsManager::instance();
    QProcess proc;
    QString terminal;
    QStringList arguments;

    if (ui->lineEdit_hashfile->text().isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("Please choose a hash file.");
        msgBox.exec();
        return;
    }

    if (settings.getKey<QString>("hashcatPath").isEmpty()) {
        QMessageBox msgBox(this);
        QString message = tr("Navigate to <b>%1 → %2</b> to configure the path to the hashcat executable.")
                              .arg(ui->menuFile->menuAction()->text(), ui->actionSettings->text());
        msgBox.setText(message);
        msgBox.setTextFormat(Qt::RichText);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        return;
    }

    if (settings.getKey<QString>("terminal").isEmpty()) {
        QMessageBox msgBox(this);
        QString message = tr("Navigate to <b>%1 → %2</b> to select the terminal used for launching.")
                              .arg(ui->menuFile->menuAction()->text(), ui->actionSettings->text());
        msgBox.setText(message);
        msgBox.setTextFormat(Qt::RichText);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        return;
    }

    /* 1. Get arguments needed for the selected terminal */
    QMap<QString, QStringList> availableTerminals = HelperUtils::getAvailableTerminals();

    // The configured terminal has a known configuration
    if (availableTerminals.contains(settings.getKey<QString>("terminal"))) {
        terminal = settings.getKey<QString>("terminal");
        arguments << availableTerminals.value(terminal);
    }

    /* 2. append hashcat binary to launch command */
    arguments << settings.getKey<QString>("hashcatPath");

    /* 3. append arguments set in gui elements */
    arguments << generateArguments();

#if defined(Q_OS_WIN)
    /* Need CREATE_NEW_CONSOLE flag on windows to spawn visible terminal */
    proc.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *args) {
        args->flags |= CREATE_NEW_CONSOLE;
    });
#endif

    proc.setProgram(terminal);
    proc.setArguments(arguments);
    proc.setWorkingDirectory(QFileInfo(settings.getKey<QString>("hashcatPath")).absolutePath());
    proc.startDetached();
}

/*************** Helper ***************/

void MainWindow::commandChanged()
{
    auto &settings = SettingsManager::instance();
    QFileInfo fileInfo(settings.getKey<QString>("hashcatPath"));

    ui->lineEdit_command->clear();

    // prepend hashcat binary name if it has already been configured in settings
    if (!settings.getKey<QString>("hashcatPath").isEmpty()) {
        ui->lineEdit_command->setText(fileInfo.fileName());
    }

    // command line arguments for hashcat
    ui->lineEdit_command->insert(" " + generateArguments().join(" "));
    ui->lineEdit_command->setCursorPosition(0);
}

QStringList MainWindow::generateArguments()
{
    auto &settings = SettingsManager::instance();
    QStringList arguments;
    QString mask_before_dict = "";
    QString mask_after_dict = "";

    bool useShort = settings.getKey<bool>("useShortParameters");
    int attackMode = attackModes.key(ui->comboBox_attack->currentText());

    arguments << HelperUtils::getParameter(HelperUtils::Parameter::HashType, useShort) << QString::number(hashModes.key(ui->comboBox_hash->currentText()));
    arguments << HelperUtils::getParameter(HelperUtils::Parameter::AttackMode, useShort) << QString::number(attackMode);

    if (ui->checkBox_remove->isChecked()) {
        arguments << HelperUtils::getParameter(HelperUtils::Parameter::Remove, useShort);
    }

    if (ui->checkBox_ignoreusername->isChecked()) {
        arguments << HelperUtils::getParameter(HelperUtils::Parameter::Username, useShort);
    }

    switch (attackMode) {
    case AttackMode::Straight:
    case AttackMode::Association:
        if (ui->radioButton_use_rules_file->isChecked()) {
            if (ui->checkBox_rulesfile_1->isChecked() && !ui->lineEdit_open_rulesfile_1->text().isEmpty()) {
                arguments << HelperUtils::getParameter(HelperUtils::Parameter::RulesFile, useShort) << ui->lineEdit_open_rulesfile_1->text();
            }
            if (ui->checkBox_rulesfile_2->isChecked() && !ui->lineEdit_open_rulesfile_2->text().isEmpty()) {
                arguments << HelperUtils::getParameter(HelperUtils::Parameter::RulesFile, useShort) << ui->lineEdit_open_rulesfile_2->text();
            }
            if (ui->checkBox_rulesfile_3->isChecked() && !ui->lineEdit_open_rulesfile_3->text().isEmpty()) {
                arguments << HelperUtils::getParameter(HelperUtils::Parameter::RulesFile, useShort) << ui->lineEdit_open_rulesfile_3->text();
            }
        } else if (ui->radioButton_generate_rules->isChecked() && !ui->spinBox_generate_rules->cleanText().isEmpty()) {
            arguments << HelperUtils::getParameter(HelperUtils::Parameter::GenerateRules, useShort) << ui->spinBox_generate_rules->cleanText();
        }
        break;
    case AttackMode::Combination:
        break;
    case AttackMode::BruteForce:
        mask_before_dict = ui->lineEdit_mask->text();
        break;
    case AttackMode::HybridWordMask:
        if (!ui->lineEdit_mask->text().isEmpty()) {
            mask_after_dict = ui->lineEdit_mask->text();
        }
        break;
    case AttackMode::HybridMaskWord:
        if (!ui->lineEdit_mask->text().isEmpty()) {
            mask_before_dict = ui->lineEdit_mask->text();
        }
        break;
    }

    if (ui->checkBox_override_workload_profile->isChecked()) {
        arguments << HelperUtils::getParameter(HelperUtils::Parameter::WorkloadProfile, useShort) << ui->comboBox_workload_profile->currentText();
    }

    if (ui->groupBox_custom_charset->isEnabled()) {
        if (ui->checkBox_custom1->isChecked() && !ui->lineEdit_custom1->text().isEmpty()) {
            arguments << HelperUtils::getParameter(HelperUtils::Parameter::CustomCharset1, useShort) << ui->lineEdit_custom1->text();
        }
        if (ui->checkBox_custom2->isChecked() && !ui->lineEdit_custom2->text().isEmpty()) {
            arguments << HelperUtils::getParameter(HelperUtils::Parameter::CustomCharset2, useShort) << ui->lineEdit_custom2->text();
        }
        if (ui->checkBox_custom3->isChecked() && !ui->lineEdit_custom3->text().isEmpty()) {
            arguments << HelperUtils::getParameter(HelperUtils::Parameter::CustomCharset3, useShort) << ui->lineEdit_custom3->text();
        }
        if (ui->checkBox_custom4->isChecked() && !ui->lineEdit_custom4->text().isEmpty()) {
            arguments << HelperUtils::getParameter(HelperUtils::Parameter::CustomCharset4, useShort) << ui->lineEdit_custom4->text();
        }
    }

    if (ui->checkBox_hex_hash->isChecked()) {
        arguments << HelperUtils::getParameter(HelperUtils::Parameter::HexCharset, useShort);
    }

    if (ui->checkBox_hex_salt->isChecked()) {
        arguments << HelperUtils::getParameter(HelperUtils::Parameter::HexSalt, useShort);
    }

    if (ui->checkBox_outfile->isChecked() && !ui->lineEdit_outfile->text().isEmpty()) {
        QFileInfo hash_fi(ui->lineEdit_hashfile->text());
        QString outfile = ui->lineEdit_outfile->text();
        outfile.replace("<unixtime>", QString::number(QDateTime::currentMSecsSinceEpoch() / 1000));
        outfile.replace("<hash>", hash_fi.fileName(), Qt::CaseInsensitive);
        arguments << HelperUtils::getParameter(HelperUtils::Parameter::Outfile, useShort) << outfile;
    }

    if (ui->lineEdit_outfile_format->text() != "1,2") {
        arguments << HelperUtils::getParameter(HelperUtils::Parameter::OutfileFormat, useShort) << ui->lineEdit_outfile_format->text();
    }

    if (!ui->lineEdit_cpu_affinity->text().isEmpty()) {
        arguments << HelperUtils::getParameter(HelperUtils::Parameter::CpuAffinity, useShort) << ui->lineEdit_cpu_affinity->text();
    }

    if (!ui->lineEdit_devices->text().isEmpty() && ui->lineEdit_devices->text() != "0") {
        arguments << HelperUtils::getParameter(HelperUtils::Parameter::BackendDevices, useShort) << ui->lineEdit_devices->text();
    }

    if (!ui->spinBox_segment->cleanText().isEmpty() && ui->spinBox_segment->cleanText() != "32") {
        arguments << HelperUtils::getParameter(HelperUtils::Parameter::SegmentSize, useShort) << ui->spinBox_segment->cleanText();
    }

    if (!ui->lineEdit_hashfile->text().isEmpty()) {
        arguments << ui->lineEdit_hashfile->text();
    }

    if (!mask_before_dict.isEmpty()) {
        arguments << mask_before_dict;
    }

    if (ui->groupBox_wordlists->isEnabled()) {
        auto const wordlists = ui->listWidget_wordlist->findItems(QString("*"), Qt::MatchWildcard);
        for (QListWidgetItem *item : wordlists) {
            if (item->checkState() == Qt::Checked) {
                arguments << item->text();
            }
        }
    }

    if (!mask_after_dict.isEmpty()) {
        arguments << mask_after_dict;
    }

    return arguments;
}
