/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2012-2025 Rainer Größlinger
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "settingsdialog.h"
#include "settingsmanager.h"
#include "helperutils.h"
#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QPixmap>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto& settings = SettingsManager::instance();

    this->init_hash_and_attack_modes();
    this->update_view_attack_mode();

    connect(ui->listWidget_wordlist->model(), SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(CommandChanged()));
    connect(ui->listWidget_wordlist->model(), SIGNAL(rowsRemoved(const QModelIndex &, int, int)), this, SLOT(CommandChanged()));
    connect(ui->listWidget_wordlist->model(), SIGNAL(rowsMoved(const QModelIndex &, int, int, const QModelIndex &, int)), this, SLOT(CommandChanged()));

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

/********* MainWindow *************************************/

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

void MainWindow::on_actionReset_fields_triggered()
{
    ui->lineEdit_open_hashfile->clear();
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
    ui->spinBox_password_min->setValue(ui->spinBox_password_min->minimum());
    ui->spinBox_password_max->setValue(ui->spinBox_password_max->maximum());
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
    ui->spinBox_password_min->setValue(1);
    ui->spinBox_password_max->setValue(16);
    ui->checkBox_outfile->setChecked(false);
    ui->lineEdit_outfile->clear();
    ui->comboBox_outfile_format->setCurrentIndex(2);
    ui->checkBox_async->setChecked(false);
    ui->lineEdit_cpu_affinity->clear();
    ui->lineEdit_devices->setText("0");
    ui->spinBox_accel->setValue(8);
    ui->spinBox_loops->setValue(256);
    ui->spinBox_watchdog->setValue(90);
    ui->spinBox_segment->setValue(32);
    this->CommandChanged();
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog* settings = new SettingsDialog(this);
    int result = settings->exec();

    if (result == QDialog::Accepted) {
        // If SettingsDialog was saved and there are no hash types yet maybe we can populate them now
        if (ui->comboBox_hash->count() == 0) {
            this->init_hash_and_attack_modes();
        }
    }
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::on_actionHelp_About_triggered()
{
    AboutDialog* about = new AboutDialog(this);
    about->get_versions();
    about->show();
}

void MainWindow::add_hash_and_attack_modes(QComboBox *&combobox, QMap <quint32, QString> &map) {
    QMapIterator<quint32, QString> iter(map);
    while (iter.hasNext()) {
        iter.next();
        combobox->addItem(iter.value());
    }
}

void MainWindow::init_hash_and_attack_modes() {

    ui->comboBox_attack->clear();
    ui->comboBox_hash->clear();
    attackModes.clear();
    hashModes.clear();

    // Attack modes
    attackModes.insert(0, "Straight");
    attackModes.insert(1, "Combination");
    attackModes.insert(3, "Brute-force");
    attackModes.insert(6, "Hybrid Wordlist + Mask");
    attackModes.insert(7, "Hybrid Mask + Wordlist");
    attackModes.insert(9, "Association");

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

        for (auto it = rootObj.begin(); it != rootObj.end(); ++it) {
            hashModes.insert(it.key().toInt(), QString(it.key() + " | " + it.value().toObject().value("name").toString()));
        }
    }

    this->add_hash_and_attack_modes(ui->comboBox_attack, attackModes);
    this->add_hash_and_attack_modes(ui->comboBox_hash, hashModes);
}

void MainWindow::add_wordlist_item(QString &wordlist) {
    QListWidget *w;
    bool duplicate = false;

    w = ui->listWidget_wordlist;

    for (int j=0; j<w->count(); ++j) {
        if (w->item(j)->text() == wordlist) {
            duplicate = true;
        }
    }

    if (!wordlist.isNull() && !duplicate) {
        QListWidgetItem *newItem = new QListWidgetItem(QIcon(":/images/icon_dir.png"), wordlist, w);
        newItem->setCheckState(Qt::Checked);
        w->addItem(newItem);
    }

}

void MainWindow::add_wordlist_item(QStringList &wordlist) {
    QListWidget *w;
    bool duplicate = false;

    w = ui->listWidget_wordlist;

    for (int i=0; i<wordlist.length(); i++) {
        duplicate = false;
        for (int j=0; j<w->count(); ++j) {
            if (w->item(j)->text() == wordlist.at(i)) {
                duplicate = true;
            }
        }

        if (!wordlist.at(i).isNull() && !duplicate) {
            QListWidgetItem *newItem = new QListWidgetItem(QIcon(":/images/icon_file.png"), wordlist.at(i), w);
            newItem->setCheckState(Qt::Checked);
            w->addItem(newItem);
        }
    }
}

void MainWindow::set_outfile_path() {
    QLineEdit *hash, *out;

    hash = ui->lineEdit_open_hashfile;
    out = ui->lineEdit_outfile;

    if (hash->text().length()) {
        out->setText(hash->text() + ".out");
    }
}

void MainWindow::on_comboBox_attack_currentIndexChanged([[maybe_unused]] int index)
{
    update_view_attack_mode();
}

void MainWindow::update_view_attack_mode()
{
    int attackMode = attackModes.key(ui->comboBox_attack->currentText());
    bool groupWordlists = false, groupRules = false, groupPassword = false, groupMask = false;

    switch(attackMode) {
    case 0:
        // Straight
        groupWordlists = true;
        groupRules = true;
        groupPassword = false;
        groupMask = false;
        break;
    case 1:
        // Combination
        groupWordlists = true;
        groupRules = false;
        groupPassword = false;
        groupMask = false;
        break;
    case 3:
        // Brute-force
        groupWordlists = false;
        groupRules = false;
        groupPassword = false;
        groupMask = true;
        break;
    case 4:
        // Permutation
        groupWordlists = true;
        groupRules = false;
        groupPassword = true;
        groupMask = false;
        break;
    case 6:
    case 7:
        // Hybrid dict + mask
        groupWordlists = true;
        groupRules = false;
        groupPassword = false;
        groupMask = true;
        break;
    }

    ui->groupBox_wordlists->setDisabled(!groupWordlists);
    ui->groupBox_rules->setDisabled(!groupRules);
    ui->groupBox_password->setDisabled(!groupPassword);
    ui->groupBox_custom_charset->setDisabled(!groupMask);
    ui->groupBox_mask->setDisabled(!groupMask);
    this->CommandChanged();
}

void MainWindow::on_pushButton_open_hashfile_clicked()
{
    QString hashfile = QFileDialog::getOpenFileName();
    if (!hashfile.isNull()) {
        ui->lineEdit_open_hashfile->setText(QDir::toNativeSeparators(hashfile));
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
    QStringList wordlist = QFileDialog::getOpenFileNames();
    this->add_wordlist_item(wordlist);
}

void MainWindow::on_pushButton_add_wordlist_folder_clicked()
{
    QString wordlist = QFileDialog::getExistingDirectory();
    this->add_wordlist_item(wordlist);
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

void MainWindow::on_lineEdit_open_hashfile_textChanged([[maybe_unused]] const QString &arg1)
{
    this->set_outfile_path();
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
    case 0:
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
    case 1:
        break;
    case 3:
        mask_before_dict = ui->lineEdit_mask->text();
        break;
    case 4:
        arguments << "--perm-min" << ui->spinBox_password_min->cleanText();
        arguments << "--perm-max" << ui->spinBox_password_max->cleanText();
        break;
    case 6:
        if (!ui->lineEdit_mask->text().isEmpty()) {
            mask_after_dict = ui->lineEdit_mask->text();
        }
        break;
    case 7:
        if (!ui->lineEdit_mask->text().isEmpty()) {
            mask_before_dict = ui->lineEdit_mask->text();
        }
        break;
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
        QFileInfo hash_fi(ui->lineEdit_open_hashfile->text());
        QString outfile = ui->lineEdit_outfile->text();
        outfile.replace("<unixtime>", QString::number(QDateTime::currentMSecsSinceEpoch() / 1000));
        outfile.replace("<hash>", hash_fi.fileName(), Qt::CaseInsensitive);
        arguments << "--outfile" << outfile;
    }

    if (ui->comboBox_outfile_format->currentIndex() != 2) {
        arguments << "--outfile-format" << QString::number(ui->comboBox_outfile_format->currentIndex()+1);
    }

    if (ui->checkBox_async->isChecked()) {
        arguments << "--gpu-async";
    }

    if (!ui->lineEdit_cpu_affinity->text().isEmpty()) {
        arguments << "--cpu-affinity" << ui->lineEdit_cpu_affinity->text();
    }

    if (!ui->lineEdit_devices->text().isEmpty() && ui->lineEdit_devices->text() != "0") {
        arguments << "--gpu-devices" << ui->lineEdit_devices->text();
    }

    if (!ui->spinBox_accel->cleanText().isEmpty() && ui->spinBox_accel->cleanText() != "8") {
        arguments << "--gpu-accel" << ui->spinBox_accel->cleanText();
    }

    if (!ui->spinBox_loops->cleanText().isEmpty() && ui->spinBox_loops->cleanText() != "256") {
        arguments << "--gpu-loops" << ui->spinBox_loops->cleanText();
    }

    if (!ui->spinBox_watchdog->cleanText().isEmpty() && ui->spinBox_watchdog->cleanText() != "90") {
        arguments << "--gpu-watchdog" << ui->spinBox_watchdog->cleanText();
    }

    if (!ui->spinBox_segment->cleanText().isEmpty() && ui->spinBox_segment->cleanText() != "32") {
        arguments << "--segment-size" << ui->spinBox_segment->cleanText();
    }

    if (!ui->lineEdit_open_hashfile->text().isEmpty()) {
        arguments << ui->lineEdit_open_hashfile->text();
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

void MainWindow::on_pushButton_execute_clicked()
{
    auto& settings = SettingsManager::instance();
    QProcess proc;
    QString terminal;
    QStringList arguments;

    if (ui->lineEdit_open_hashfile->text().isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("Please choose a hash file.");
        msgBox.exec();
        return;
    }

    if (settings.getKey("hashcatPath").isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("Please configure the path to the hashcat binary in settings first.");
        msgBox.exec();
        return;
    }

    if (settings.getKey("terminal").isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("Please configure a terminal in settings first.");
        msgBox.exec();
        return;
    }

    // 1. Get arguments needed for the selected terminal
    QMap <QString, QStringList> availableTerminals = HelperUtils::getAvailableTerminals();
    for (const QString& key : availableTerminals.keys()) {
        // The configured terminal has a known configuration
        if (key == settings.getKey("terminal")) {
            terminal = key;
            arguments << availableTerminals.value(key);
        }
    }

    // 2. append hashcat binary to launch comand
    arguments << settings.getKey("hashcatPath");

    // 3. append arguments set in gui elements
    arguments << generate_arguments();

    qInfo() << terminal;
    qInfo() << arguments;

    proc.startDetached(terminal, arguments, QFileInfo(settings.getKey("hascatPath")).absolutePath());
}

