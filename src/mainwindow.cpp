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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto& settings = SettingsManager::instance();

    this->init_hash_and_attack_modes();
    this->update_view_attack_mode();

    /***** Signals/Slots *****/

    // wordlist
    connect(ui->listWidget_wordlist->model(), &QAbstractItemModel::rowsInserted, this, [this] { CommandChanged(); });
    connect(ui->listWidget_wordlist->model(), &QAbstractItemModel::rowsRemoved, this, [this] { CommandChanged(); });
    connect(ui->listWidget_wordlist->model(), &QAbstractItemModel::rowsMoved, this, [this] { CommandChanged(); });
    connect(ui->listWidget_wordlist, &QListWidget::itemChanged, this, [this] { CommandChanged(); });

    // copy to clipboard
    connect(ui->pushButton_copy_clipboard, &QPushButton::clicked, this, [this] { copyCommandToClipboard(); });

    // workload tuning
    connect(ui->checkBox_override_workload_profile, &QCheckBox::toggled, ui->comboBox_workload_profile, &QComboBox::setEnabled);

    // Show Settings dialog if path to hashcat has not been configured yet
    if (settings.getKey("hashcatPath").isEmpty()) {
        QMetaObject::invokeMethod(this, [this]() {
            ui->actionSettings->triggered();
        }, Qt::QueuedConnection);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*************** Menu Bar ***************/

// File -> Export
// Export QWidget state to a JSON file
void MainWindow::on_actionExport_triggered()
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

// File -> Import
// Import QWidget state from a JSON file
void MainWindow::on_actionImport_triggered()
{
    QString file = QFileDialog::getOpenFileName(
        this, tr("Load Profile"),
        QString(),
        tr("JSON Files (*.json)"));

    if (!file.isEmpty()) {
        WidgetStateSerializer s;
        if (s.loadStateFromFile(QString::fromUtf8(metaObject()->className()), this, file)) {
            this->CommandChanged();
            QMessageBox::information(this, tr("Loaded"), tr("Profile loaded from %1.").arg(file));
        }
    }
}


// Tools -> Reset field
void MainWindow::on_actionReset_fields_triggered()
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

    this->CommandChanged();
}

// File -> Settings
// Open Settings dialog
void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog settingsDialog(this);
    if (settingsDialog.exec() == QDialog::Accepted) {
        // If SettingsDialog was saved and there are no hash types yet maybe we can populate them now
        if (ui->comboBox_hash->count() == 0) {
            this->init_hash_and_attack_modes();
        }
    }
}

// File -> Quit
void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}

// Help -> About Qt
void MainWindow::on_actionAbout_Qt_triggered()
{
    QApplication::aboutQt();
}

// Help -> About
void MainWindow::on_actionHelp_About_triggered()
{
    AboutDialog about(this);
    about.exec();
}

/*************** MainWindow ***************/

void MainWindow::init_hash_and_attack_modes() {

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
    auto& settings = SettingsManager::instance();
    QString hashTypes;

    // Read list of example hashes, returns JSON
    if (!settings.getKey("hashcatPath").isEmpty()) {
        hashTypes = HelperUtils::executeHashcat(QStringList() << "--example-hashes" << "--machine-readable").remove('\n').remove('\r');
    }

    // Fill QComboBox with available hash types
    QJsonDocument doc = QJsonDocument::fromJson(hashTypes.toUtf8());
    if (doc.isObject()) {
        QJsonObject rootObj = doc.object();

        for (auto it = rootObj.constBegin(); it != rootObj.constEnd(); ++it) {
            hashModes.insert(it.key().toInt(), QString(it.key() + " | " + it.value().toObject().value("name").toString()));
        }
    }

    // Fill combo boxes
    for (const QString &value : attackModes.values()) {
        ui->comboBox_attack->addItem(value);
    }

    for (const QString &value : hashModes.values()) {
        ui->comboBox_hash->addItem(value);
    }
}

void MainWindow::on_comboBox_attack_currentIndexChanged([[maybe_unused]] int index)
{
    update_view_attack_mode();
}

void MainWindow::update_view_attack_mode()
{
    int attackMode = attackModes.key(ui->comboBox_attack->currentText());
    bool groupWordlists = false, groupRules = false, groupMask = false;

    switch(attackMode) {
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
    this->CommandChanged();
}

void MainWindow::on_pushButton_open_hashfile_clicked()
{
    QString hashfile = QFileDialog::getOpenFileName();
    if (!hashfile.isNull()) {
        ui->lineEdit_hashfile->setText(QDir::toNativeSeparators(hashfile));
    }
}

void MainWindow::on_pushButton_output_clicked()
{
    QString outfile = QFileDialog::getSaveFileName();
    if (!outfile.isNull()) {
        ui->lineEdit_outfile->setText(QDir::toNativeSeparators(outfile));
    }
}

void MainWindow::on_pushButton_remove_wordlist_clicked()
{
    qDeleteAll(ui->listWidget_wordlist->selectedItems());
    ui->pushButton_remove_wordlist->setEnabled(false);
    ui->toolButton_wordlist_sort_asc->setEnabled(false);
    ui->toolButton_wordlist_sort_desc->setEnabled(false);
    ui->listWidget_wordlist->clearSelection();
}

void MainWindow::on_pushButton_add_wordlist_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames();
    QListWidget *w = ui->listWidget_wordlist;

    for (const QString &wordlist : std::as_const(files)) {
        // A file has been selected and it is not already in the list
        if (!wordlist.isNull() && w->findItems(wordlist, Qt::MatchExactly).isEmpty()) {
            QListWidgetItem *newItem = new QListWidgetItem(wordlist, w);
            newItem->setCheckState(Qt::Checked);
        }
    }
}

void MainWindow::on_toolButton_wordlist_sort_asc_clicked()
{
    int currentRow = ui->listWidget_wordlist->currentRow();
    if (currentRow == 0) return;
    QListWidgetItem *currentItem = ui->listWidget_wordlist->takeItem(currentRow);
    ui->listWidget_wordlist->insertItem(currentRow - 1, currentItem);
    ui->listWidget_wordlist->setCurrentRow(currentRow - 1);
}

void MainWindow::on_toolButton_wordlist_sort_desc_clicked()
{
    int currentRow = ui->listWidget_wordlist->currentRow();
    if (currentRow >= ui->listWidget_wordlist->count()-1) return;
    QListWidgetItem *currentItem = ui->listWidget_wordlist->takeItem(currentRow);
    ui->listWidget_wordlist->insertItem(currentRow + 1, currentItem);
    ui->listWidget_wordlist->setCurrentRow(currentRow + 1);
}

void MainWindow::on_listWidget_wordlist_itemClicked([[maybe_unused]] QListWidgetItem* item)
{
    ui->pushButton_remove_wordlist->setEnabled(true);
    ui->toolButton_wordlist_sort_asc->setEnabled(true);
    ui->toolButton_wordlist_sort_desc->setEnabled(true);
}

void MainWindow::on_checkBox_rulesfile_1_toggled(bool checked)
{
    ui->lineEdit_open_rulesfile_1->setEnabled(checked);
    ui->pushButton_open_rulesfile_1->setEnabled(checked);
}

void MainWindow::on_checkBox_rulesfile_2_toggled(bool checked)
{
    ui->lineEdit_open_rulesfile_2->setEnabled(checked);
    ui->pushButton_open_rulesfile_2->setEnabled(checked);
}

void MainWindow::on_checkBox_rulesfile_3_toggled(bool checked)
{
    ui->lineEdit_open_rulesfile_3->setEnabled(checked);
    ui->pushButton_open_rulesfile_3->setEnabled(checked);
}

void MainWindow::on_radioButton_use_rules_file_toggled(bool checked)
{
    ui->checkBox_rulesfile_1->setEnabled(checked);
    ui->checkBox_rulesfile_2->setEnabled(checked);
    ui->checkBox_rulesfile_3->setEnabled(checked);
    if (checked) {
        this->on_checkBox_rulesfile_1_toggled(ui->checkBox_rulesfile_1->isChecked());
        this->on_checkBox_rulesfile_2_toggled(ui->checkBox_rulesfile_2->isChecked());
        this->on_checkBox_rulesfile_3_toggled(ui->checkBox_rulesfile_3->isChecked());
    }
}

void MainWindow::on_radioButton_generate_rules_toggled(bool checked)
{
    ui->spinBox_generate_rules->setEnabled(checked);
    if (checked) {
        this->on_radioButton_use_rules_file_toggled(false);
        this->on_checkBox_rulesfile_1_toggled(false);
        this->on_checkBox_rulesfile_2_toggled(false);
        this->on_checkBox_rulesfile_3_toggled(false);
    }
}

void MainWindow::on_pushButton_open_rulesfile_1_clicked()
{
    QString rulesfile = QFileDialog::getOpenFileName();
    if (!rulesfile.isNull()) {
        ui->lineEdit_open_rulesfile_1->setText(QDir::toNativeSeparators(rulesfile));
    }
}

void MainWindow::on_pushButton_open_rulesfile_2_clicked()
{
    QString rulesfile = QFileDialog::getOpenFileName();
    if (!rulesfile.isNull()) {
        ui->lineEdit_open_rulesfile_2->setText(QDir::toNativeSeparators(rulesfile));
    }
}

void MainWindow::on_pushButton_open_rulesfile_3_clicked()
{
    QString rulesfile = QFileDialog::getOpenFileName();
    if (!rulesfile.isNull()) {
        ui->lineEdit_open_rulesfile_3->setText(QDir::toNativeSeparators(rulesfile));
    }
}

void MainWindow::on_checkBox_custom1_toggled(bool checked)
{
    ui->lineEdit_custom1->setEnabled(checked);
}

void MainWindow::on_checkBox_custom2_toggled(bool checked)
{
    ui->lineEdit_custom2->setEnabled(checked);
}

void MainWindow::on_checkBox_custom3_toggled(bool checked)
{
    ui->lineEdit_custom3->setEnabled(checked);
}

void MainWindow::on_checkBox_custom4_toggled(bool checked)
{
    ui->lineEdit_custom4->setEnabled(checked);
}

void MainWindow::on_checkBox_outfile_toggled(bool checked)
{
    ui->lineEdit_outfile->setEnabled(checked);
    ui->pushButton_output->setEnabled(checked);
}

void MainWindow::on_lineEdit_hashfile_textChanged(const QString &text)
{
    if (!text.isEmpty()) {
        ui->lineEdit_outfile->setText(text + ".out");
    }
}

void MainWindow::copyCommandToClipboard() {
    QString text = ui->lineEdit_command->text();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(text);
}

void MainWindow::on_pushButton_execute_clicked()
{
    auto& settings = SettingsManager::instance();
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

    if (settings.getKey("hashcatPath").isEmpty()) {
        QMessageBox msgBox(this);
        QString message = tr("Navigate to <b>%1 → %2</b> to configure the path to the hashcat executable.")
                              .arg(ui->menuFile->menuAction()->text(), ui->actionSettings->text());
        msgBox.setText(message);
        msgBox.setTextFormat(Qt::RichText);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        return;
    }

    if (settings.getKey("terminal").isEmpty()) {
        QMessageBox msgBox(this);
        QString message = tr("Navigate to <b>%1 → %2</b> to select the terminal used for launching.")
                              .arg(ui->menuFile->menuAction()->text(), ui->actionSettings->text());
        msgBox.setText(message);
        msgBox.setTextFormat(Qt::RichText);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        return;
    }

    // 1. Get arguments needed for the selected terminal
    QMap <QString, QStringList> availableTerminals = HelperUtils::getAvailableTerminals();

    // The configured terminal has a known configuration
    if (availableTerminals.contains(settings.getKey("terminal"))) {
        terminal = settings.getKey("terminal");
        arguments << availableTerminals.value(terminal);
    }

    // 2. append hashcat binary to launch comand
    arguments << settings.getKey("hashcatPath");

    // 3. append arguments set in gui elements
    arguments << generate_arguments();

#if defined(Q_OS_WIN)
    // Need CREATE_NEW_CONSOLE flag on windows to spawn visible terminal
    proc.setCreateProcessArgumentsModifier([] (QProcess::CreateProcessArguments *args)
    {
        args->flags |= CREATE_NEW_CONSOLE;
    });
#endif

    proc.setProgram(terminal);
    proc.setArguments(arguments);
    proc.setWorkingDirectory(QFileInfo(settings.getKey("hashcatPath")).absolutePath());
    proc.startDetached();
}

/*************** Helper ***************/

void MainWindow::CommandChanged(QString arg) {
    auto& settings = SettingsManager::instance();
    QFileInfo fileInfo(settings.getKey("hashcatPath"));

    ui->lineEdit_command->clear();

    // prepend hashcat binary name if it has already been configured in settings
    if (!settings.getKey("hashcatPath").isEmpty()) {
        ui->lineEdit_command->setText(fileInfo.fileName());
    }

    // command line arguments for hashcat
    ui->lineEdit_command->insert(" " + (arg.length() ? arg : generate_arguments().join(" ")));
    ui->lineEdit_command->setCursorPosition(0);
}

QStringList MainWindow::generate_arguments()
{
    QStringList arguments;
    QString mask_before_dict = "";
    QString mask_after_dict = "";

    int attackMode = attackModes.key(ui->comboBox_attack->currentText());

    arguments << "--hash-type" << QString::number(hashModes.key(ui->comboBox_hash->currentText()));
    arguments << "--attack-mode" << QString::number(attackMode);

    if (ui->checkBox_remove->isChecked()) {
        arguments << "--remove";
    }

    if (ui->checkBox_ignoreusername->isChecked()) {
        arguments << "--username";
    }

    switch(attackMode) {
    case AttackMode::Straight:
        if ( ui->radioButton_use_rules_file->isChecked()) {
            if (ui->checkBox_rulesfile_1->isChecked() && !ui->lineEdit_open_rulesfile_1->text().isEmpty()) {
                arguments << "--rules-file" << ui->lineEdit_open_rulesfile_1->text();
            }

            if (ui->checkBox_rulesfile_2->isChecked() && !ui->lineEdit_open_rulesfile_2->text().isEmpty()) {
                arguments << "--rules-file" << ui->lineEdit_open_rulesfile_2->text();
            }

            if (ui->checkBox_rulesfile_3->isChecked() && !ui->lineEdit_open_rulesfile_3->text().isEmpty()) {
                arguments << "--rules-file" << ui->lineEdit_open_rulesfile_3->text();
            }
        } else if (ui->radioButton_generate_rules->isChecked() && !ui->spinBox_generate_rules->cleanText().isEmpty()) {
            arguments << "--generate-rules" << ui->spinBox_generate_rules->cleanText();
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
    case AttackMode::Association:
        break;
    }

    if (ui->checkBox_override_workload_profile->isChecked()) {
        arguments << "-w" << ui->comboBox_workload_profile->currentText();
    }

    if (ui->groupBox_custom_charset->isEnabled()) {
        if (ui->checkBox_custom1->isChecked() && !ui->lineEdit_custom1->text().isEmpty()) {
            arguments << "--custom-charset1" << ui->lineEdit_custom1->text();
        }

        if (ui->checkBox_custom2->isChecked() && !ui->lineEdit_custom2->text().isEmpty()) {
            arguments << "--custom-charset2" << ui->lineEdit_custom2->text();
        }

        if (ui->checkBox_custom3->isChecked() && !ui->lineEdit_custom3->text().isEmpty()) {
            arguments << "--custom-charset3" << ui->lineEdit_custom3->text();
        }

        if (ui->checkBox_custom4->isChecked() && !ui->lineEdit_custom4->text().isEmpty()) {
            arguments << "--custom-charset4" << ui->lineEdit_custom4->text();
        }
    }

    if (ui->checkBox_hex_hash->isChecked()) {
        arguments << "--hex-charset";
    }

    if (ui->checkBox_hex_salt->isChecked()) {
        arguments << "--hex-salt";
    }

    if(ui->checkBox_outfile->isChecked() && !ui->lineEdit_outfile->text().isEmpty()) {
        QFileInfo hash_fi(ui->lineEdit_hashfile->text());
        QString outfile = ui->lineEdit_outfile->text();
        outfile.replace("<unixtime>", QString::number(QDateTime::currentMSecsSinceEpoch() / 1000));
        outfile.replace("<hash>", hash_fi.fileName(), Qt::CaseInsensitive);
        arguments << "--outfile" << outfile;
    }

    if (ui->lineEdit_outfile_format->text() != "1,2") {
        arguments << "--outfile-format" << ui->lineEdit_outfile_format->text();
    }

    if (!ui->lineEdit_cpu_affinity->text().isEmpty()) {
        arguments << "--cpu-affinity" << ui->lineEdit_cpu_affinity->text();
    }

    if (!ui->lineEdit_devices->text().isEmpty() && ui->lineEdit_devices->text() != "0") {
        arguments << "--backend-devices" << ui->lineEdit_devices->text();
    }

    if (!ui->spinBox_segment->cleanText().isEmpty() && ui->spinBox_segment->cleanText() != "32") {
        arguments << "--segment-size" << ui->spinBox_segment->cleanText();
    }

    if (!ui->lineEdit_hashfile->text().isEmpty()) {
        arguments << ui->lineEdit_hashfile->text();
    }

    if (mask_before_dict.length()) {
        arguments << mask_before_dict;
    }

    if (ui->groupBox_wordlists->isEnabled()) {
        for(int i=0; i<ui->listWidget_wordlist->count(); i++) {
            if(ui->listWidget_wordlist->item(i)->checkState() == Qt::Checked) {
                arguments << ui->listWidget_wordlist->item(i)->text();
            }
        }
    }

    if (mask_after_dict.length()) {
        arguments << mask_after_dict;
    }

    return arguments;
}
