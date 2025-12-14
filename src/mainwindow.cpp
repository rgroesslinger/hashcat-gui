/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2012-2025 Rainer Größlinger
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "settingsdialog.h"
#include "settingsmanager.h"
#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QPixmap>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto& settings = SettingsManager::instance();

    this->init_hash_and_attack_modes();
    this->oclhcplus_update_view_attack_mode();

    connect(ui->listWidget_oclhcplus_wordlist->model(), SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(oclhcplusCommandChanged()));
    connect(ui->listWidget_oclhcplus_wordlist->model(), SIGNAL(rowsRemoved(const QModelIndex &, int, int)), this, SLOT(oclhcplusCommandChanged()));
    connect(ui->listWidget_oclhcplus_wordlist->model(), SIGNAL(rowsMoved(const QModelIndex &, int, int, const QModelIndex &, int)), this, SLOT(oclhcplusCommandChanged()));

    // Show Settings dialog if path to hashcat has not been configured yet
    if (settings.hashcatPath().isEmpty()) {
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

QStringList MainWindow::generate_terminal_env(qint16 hc_type) {

    QStringList arguments;
    QString path;

    switch(hc_type) {
    case 0: path = envInfo.value("dir_current") + envInfo.value("cmd_hc"); break;
    case 1: path = envInfo.value("dir_current") + envInfo.value("cmd_oclhcplus"); break;
    case 2: path = envInfo.value("dir_current") + envInfo.value("cmd_oclhclite"); break;
    }

#if defined(Q_WS_WIN)
    arguments << "/k" << path;
#else
    arguments << "-hold" << "-e" << path;
#endif

    return arguments;
}

void MainWindow::oclhcplusCommandChanged(QString arg) {
    ui->lineEdit_oclhcplus_command->setText(envInfo.value("cmd_oclhcplus") + " " + (arg.length() ? arg : oclhcplus_generate_arguments().join(" ")));
    ui->lineEdit_oclhcplus_command->setCursorPosition(0);
}

void MainWindow::on_actionReset_fields_triggered()
{
//    switch(ui->tabWidget_main->currentIndex()) {
//    case 1:
        ui->lineEdit_oclhcplus_open_hashfile->clear();
        ui->checkBox_oclhcplus_ignoreusername->setChecked(false);
        ui->checkBox_oclhcplus_remove->setChecked(false);
        ui->listWidget_oclhcplus_wordlist->clear();
        ui->comboBox_oclhcplus_attack->setCurrentIndex(0);
        ui->comboBox_oclhcplus_hash->setCurrentIndex(0);
        ui->radioButton_oclhcplus_use_rules_file->setChecked(true);
        ui->checkBox_oclhcplus_rulesfile_1->setChecked(false);
        ui->checkBox_oclhcplus_rulesfile_2->setChecked(false);
        ui->checkBox_oclhcplus_rulesfile_3->setChecked(false);
        ui->lineEdit_oclhcplus_open_rulesfile_1->clear();
        ui->lineEdit_oclhcplus_open_rulesfile_2->clear();
        ui->lineEdit_oclhcplus_open_rulesfile_3->clear();
        ui->spinBox_oclhcplus_generate_rules->setValue(1);
        ui->spinBox_oclhcplus_password_min->setValue(ui->spinBox_oclhcplus_password_min->minimum());
        ui->spinBox_oclhcplus_password_max->setValue(ui->spinBox_oclhcplus_password_max->maximum());
        ui->lineEdit_oclhcplus_mask->clear();
        ui->checkBox_oclhcplus_custom1->setChecked(false);
        ui->lineEdit_oclhcplus_custom1->clear();
        ui->checkBox_oclhcplus_custom2->setChecked(false);
        ui->lineEdit_oclhcplus_custom2->clear();
        ui->checkBox_oclhcplus_custom3->setChecked(false);
        ui->lineEdit_oclhcplus_custom3->clear();
        ui->checkBox_oclhcplus_custom4->setChecked(false);
        ui->lineEdit_oclhcplus_custom4->clear();
        ui->checkBox_oclhcplus_hex_hash->setChecked(false);
        ui->checkBox_oclhcplus_hex_salt->setChecked(false);
        ui->spinBox_oclhcplus_password_min->setValue(1);
        ui->spinBox_oclhcplus_password_max->setValue(16);
        ui->checkBox_oclhcplus_outfile->setChecked(false);
        ui->lineEdit_oclhcplus_outfile->clear();
        ui->comboBox_oclhcplus_outfile_format->setCurrentIndex(2);
        ui->checkBox_oclhcplus_async->setChecked(false);
        ui->lineEdit_oclhcplus_cpu_affinity->clear();
        ui->lineEdit_oclhcplus_devices->setText("0");
        ui->spinBox_oclhcplus_accel->setValue(8);
        ui->spinBox_oclhcplus_loops->setValue(256);
        ui->spinBox_oclhcplus_watchdog->setValue(90);
        ui->spinBox_oclhcplus_segment->setValue(32);
        this->oclhcplusCommandChanged();
//        break;
//    }
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog *settings = new SettingsDialog(this);
    settings->show();
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
    AboutDialog *about = new AboutDialog(this);
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
    oclhcplus_attackModes.insert(0, "Straight");
    oclhcplus_attackModes.insert(1, "Combination");
    oclhcplus_attackModes.insert(3, "Brute-force");
    oclhcplus_attackModes.insert(6, "Hybrid Wordlist + Mask");
    oclhcplus_attackModes.insert(7, "Hybrid Mask + Wordlist");
    oclhcplus_attackModes.insert(9, "Association");

    oclhcplus_hashModes.insert(0, "MD5");
    oclhcplus_hashModes.insert(11, "Joomla");
    oclhcplus_hashModes.insert(21, "osCommerce, xt:Commerce");
    oclhcplus_hashModes.insert(100, "SHA1");
    oclhcplus_hashModes.insert(101, "nsldap, SHA-1(Base64), Netscape LDAP SHA");
    oclhcplus_hashModes.insert(111, "nsldaps, SSHA-1(Base64), Netscape LDAP SSHA");
    oclhcplus_hashModes.insert(112, "Oracle 11g");
    oclhcplus_hashModes.insert(121, "SMF > v1.1");
    oclhcplus_hashModes.insert(122, "OSX v10.4, v10.5, v10.6");
    oclhcplus_hashModes.insert(131, "MSSQL(2000)");
    oclhcplus_hashModes.insert(132, "MSSQL(2005)");
    oclhcplus_hashModes.insert(300, "MySQL > v4.1");
    oclhcplus_hashModes.insert(400, "phpass, MD5(Wordpress), MD5(phpBB3)");
    oclhcplus_hashModes.insert(500, "md5crypt, MD5(Unix), FreeBSD MD5, Cisco-IOS MD5");
    oclhcplus_hashModes.insert(900, "MD4");
    oclhcplus_hashModes.insert(1000, "NTLM");
    oclhcplus_hashModes.insert(1100, "Domain Cached Credentials, mscash");
    oclhcplus_hashModes.insert(1400, "SHA256");
    oclhcplus_hashModes.insert(1500, "descrypt, DES(Unix), Traditional DES");
    oclhcplus_hashModes.insert(1600, "md5apr1, MD5(APR), Apache MD5");
    oclhcplus_hashModes.insert(1700, "SHA512");
    oclhcplus_hashModes.insert(1722, "OS X v10.7");
    oclhcplus_hashModes.insert(2100, "Domain Cached Credentials2, mscash2");
    oclhcplus_hashModes.insert(2400, "Cisco-PIX MD5");
    oclhcplus_hashModes.insert(2500, "WPA/WPA2");
    oclhcplus_hashModes.insert(2600, "Double MD5");
    oclhcplus_hashModes.insert(2611, "vBulletin < v3.8.5");
    oclhcplus_hashModes.insert(2711, "vBulletin > v3.8.5");
    oclhcplus_hashModes.insert(2811, "IPB2+, MyBB 1.2+");
    oclhcplus_hashModes.insert(3000, "LM");
    oclhcplus_hashModes.insert(3100, "Oracle 7-10g, DES(Oracle)");

    this->add_hash_and_attack_modes(ui->comboBox_oclhcplus_attack, oclhcplus_attackModes);
    this->add_hash_and_attack_modes(ui->comboBox_oclhcplus_hash, oclhcplus_hashModes);
}

void MainWindow::add_wordlist_item(QString &wordlist) {
    QListWidget *w;
    bool duplicate = false;

    w = ui->listWidget_oclhcplus_wordlist;

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

    w = ui->listWidget_oclhcplus_wordlist;

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

    hash = ui->lineEdit_oclhcplus_open_hashfile;
    out = ui->lineEdit_oclhcplus_outfile;

    if (hash->text().length()) {
        out->setText(hash->text() + ".out");
    }
}

/********* oclHashcat-plus / cudaHashcat-plus *********************/

void MainWindow::on_comboBox_oclhcplus_attack_currentIndexChanged([[maybe_unused]] int index)
{
    oclhcplus_update_view_attack_mode();
}

void MainWindow::oclhcplus_update_view_attack_mode()
{
    int attackMode = oclhcplus_attackModes.key(ui->comboBox_oclhcplus_attack->currentText());
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

    ui->groupBox_oclhcplus_wordlists->setDisabled(!groupWordlists);
    ui->groupBox_oclhcplus_rules->setDisabled(!groupRules);
    ui->groupBox_oclhcplus_password->setDisabled(!groupPassword);
    ui->groupBox_oclhcplus_custom_charset->setDisabled(!groupMask);
    ui->groupBox_oclhcplus_mask->setDisabled(!groupMask);
    this->oclhcplusCommandChanged();
}

void MainWindow::on_pushButton_oclhcplus_open_hashfile_clicked()
{
    QString hashfile = QFileDialog::getOpenFileName();
    if (!hashfile.isNull()) {
        ui->lineEdit_oclhcplus_open_hashfile->setText(QDir::toNativeSeparators(hashfile));
    }
}

void MainWindow::on_pushButton_oclhcplus_output_clicked()
{
    QString outfile = QFileDialog::getSaveFileName();
    if (!outfile.isNull()) {
        ui->lineEdit_oclhcplus_outfile->setText(QDir::toNativeSeparators(outfile));
    }
}

void MainWindow::on_pushButton_oclhcplus_remove_wordlist_clicked()
{
    qDeleteAll(ui->listWidget_oclhcplus_wordlist->selectedItems());
    ui->pushButton_oclhcplus_remove_wordlist->setEnabled(false);
    ui->toolButton_oclhcplus_wordlist_sort_asc->setEnabled(false);
    ui->toolButton_oclhcplus_wordlist_sort_desc->setEnabled(false);
    ui->listWidget_oclhcplus_wordlist->clearSelection();
}

void MainWindow::on_pushButton_oclhcplus_add_wordlist_clicked()
{
    QStringList wordlist = QFileDialog::getOpenFileNames();
    this->add_wordlist_item(wordlist);
}

void MainWindow::on_pushButton_oclhcplus_add_wordlist_folder_clicked()
{
    QString wordlist = QFileDialog::getExistingDirectory();
    this->add_wordlist_item(wordlist);
}

void MainWindow::on_toolButton_oclhcplus_wordlist_sort_asc_clicked()
{
    int currentRow = ui->listWidget_oclhcplus_wordlist->currentRow();
    if (currentRow == 0) return;
    QListWidgetItem *currentItem = ui->listWidget_oclhcplus_wordlist->takeItem(currentRow);
    ui->listWidget_oclhcplus_wordlist->insertItem(currentRow - 1, currentItem);
    ui->listWidget_oclhcplus_wordlist->setCurrentRow(currentRow - 1);
}

void MainWindow::on_toolButton_oclhcplus_wordlist_sort_desc_clicked()
{
    int currentRow = ui->listWidget_oclhcplus_wordlist->currentRow();
    if (currentRow >= ui->listWidget_oclhcplus_wordlist->count()-1) return;
    QListWidgetItem *currentItem = ui->listWidget_oclhcplus_wordlist->takeItem(currentRow);
    ui->listWidget_oclhcplus_wordlist->insertItem(currentRow + 1, currentItem);
    ui->listWidget_oclhcplus_wordlist->setCurrentRow(currentRow + 1);
}

void MainWindow::on_listWidget_oclhcplus_wordlist_itemClicked([[maybe_unused]] QListWidgetItem* item)
{
    ui->pushButton_oclhcplus_remove_wordlist->setEnabled(true);
    ui->toolButton_oclhcplus_wordlist_sort_asc->setEnabled(true);
    ui->toolButton_oclhcplus_wordlist_sort_desc->setEnabled(true);
}

void MainWindow::on_checkBox_oclhcplus_rulesfile_1_toggled(bool checked)
{
    ui->lineEdit_oclhcplus_open_rulesfile_1->setEnabled(checked);
    ui->pushButton_oclhcplus_open_rulesfile_1->setEnabled(checked);
}

void MainWindow::on_checkBox_oclhcplus_rulesfile_2_toggled(bool checked)
{
    ui->lineEdit_oclhcplus_open_rulesfile_2->setEnabled(checked);
    ui->pushButton_oclhcplus_open_rulesfile_2->setEnabled(checked);
}

void MainWindow::on_checkBox_oclhcplus_rulesfile_3_toggled(bool checked)
{
    ui->lineEdit_oclhcplus_open_rulesfile_3->setEnabled(checked);
    ui->pushButton_oclhcplus_open_rulesfile_3->setEnabled(checked);
}

void MainWindow::on_radioButton_oclhcplus_use_rules_file_toggled(bool checked)
{
    ui->checkBox_oclhcplus_rulesfile_1->setEnabled(checked);
    ui->checkBox_oclhcplus_rulesfile_2->setEnabled(checked);
    ui->checkBox_oclhcplus_rulesfile_3->setEnabled(checked);
    if (checked) {
        this->on_checkBox_oclhcplus_rulesfile_1_toggled(ui->checkBox_oclhcplus_rulesfile_1->isChecked());
        this->on_checkBox_oclhcplus_rulesfile_2_toggled(ui->checkBox_oclhcplus_rulesfile_2->isChecked());
        this->on_checkBox_oclhcplus_rulesfile_3_toggled(ui->checkBox_oclhcplus_rulesfile_3->isChecked());
    }
}

void MainWindow::on_radioButton_oclhcplus_generate_rules_toggled(bool checked)
{
    ui->spinBox_oclhcplus_generate_rules->setEnabled(checked);
    if (checked) {
        this->on_radioButton_oclhcplus_use_rules_file_toggled(false);
        this->on_checkBox_oclhcplus_rulesfile_1_toggled(false);
        this->on_checkBox_oclhcplus_rulesfile_2_toggled(false);
        this->on_checkBox_oclhcplus_rulesfile_3_toggled(false);
    }
}

void MainWindow::on_pushButton_oclhcplus_open_rulesfile_1_clicked()
{
    QString rulesfile = QFileDialog::getOpenFileName();
    if (!rulesfile.isNull()) {
        ui->lineEdit_oclhcplus_open_rulesfile_1->setText(QDir::toNativeSeparators(rulesfile));
    }
}

void MainWindow::on_pushButton_oclhcplus_open_rulesfile_2_clicked()
{
    QString rulesfile = QFileDialog::getOpenFileName();
    if (!rulesfile.isNull()) {
        ui->lineEdit_oclhcplus_open_rulesfile_2->setText(QDir::toNativeSeparators(rulesfile));
    }
}

void MainWindow::on_pushButton_oclhcplus_open_rulesfile_3_clicked()
{
    QString rulesfile = QFileDialog::getOpenFileName();
    if (!rulesfile.isNull()) {
        ui->lineEdit_oclhcplus_open_rulesfile_3->setText(QDir::toNativeSeparators(rulesfile));
    }
}

void MainWindow::on_checkBox_oclhcplus_custom1_toggled(bool checked)
{
    ui->lineEdit_oclhcplus_custom1->setEnabled(checked);
}

void MainWindow::on_checkBox_oclhcplus_custom2_toggled(bool checked)
{
    ui->lineEdit_oclhcplus_custom2->setEnabled(checked);
}

void MainWindow::on_checkBox_oclhcplus_custom3_toggled(bool checked)
{
    ui->lineEdit_oclhcplus_custom3->setEnabled(checked);
}

void MainWindow::on_checkBox_oclhcplus_custom4_toggled(bool checked)
{
    ui->lineEdit_oclhcplus_custom4->setEnabled(checked);
}

void MainWindow::on_checkBox_oclhcplus_outfile_toggled(bool checked)
{
    ui->lineEdit_oclhcplus_outfile->setEnabled(checked);
    ui->pushButton_oclhcplus_output->setEnabled(checked);
}

void MainWindow::on_lineEdit_oclhcplus_open_hashfile_textChanged([[maybe_unused]] const QString &arg1)
{
    this->set_outfile_path();
}

QStringList MainWindow::oclhcplus_generate_arguments()
{
    QStringList arguments;
    QString mask_before_dict = "";
    QString mask_after_dict = "";

    int attackMode = oclhcplus_attackModes.key(ui->comboBox_oclhcplus_attack->currentText());

    if(oclhcplus_hashModes.key(ui->comboBox_oclhcplus_hash->currentText()) != 0) {
        arguments << "--hash-type" << QString::number(oclhcplus_hashModes.key(ui->comboBox_oclhcplus_hash->currentText()));
    }

    if ( attackMode != 0 ) {
        arguments << "--attack-mode" << QString::number(attackMode);
    }

    if (ui->checkBox_oclhcplus_remove->isChecked()) {
        arguments << "--remove";
    }

    if (ui->checkBox_oclhcplus_ignoreusername->isChecked()) {
        arguments << "--username";
    }

    switch(attackMode) {
    case 0:
        if ( ui->radioButton_oclhcplus_use_rules_file->isChecked()) {
            if (ui->checkBox_oclhcplus_rulesfile_1->isChecked() && !ui->lineEdit_oclhcplus_open_rulesfile_1->text().isEmpty()) {
                arguments << "--rules-file" << ui->lineEdit_oclhcplus_open_rulesfile_1->text();
            }

            if (ui->checkBox_oclhcplus_rulesfile_2->isChecked() && !ui->lineEdit_oclhcplus_open_rulesfile_2->text().isEmpty()) {
                arguments << "--rules-file" << ui->lineEdit_oclhcplus_open_rulesfile_2->text();
            }

            if (ui->checkBox_oclhcplus_rulesfile_3->isChecked() && !ui->lineEdit_oclhcplus_open_rulesfile_3->text().isEmpty()) {
                arguments << "--rules-file" << ui->lineEdit_oclhcplus_open_rulesfile_3->text();
            }
        } else if (ui->radioButton_oclhcplus_generate_rules->isChecked() && !ui->spinBox_oclhcplus_generate_rules->cleanText().isEmpty()) {
            arguments << "--generate-rules" << ui->spinBox_oclhcplus_generate_rules->cleanText();
        }
        break;
    case 1:
        break;
    case 3:
        mask_before_dict = ui->lineEdit_oclhcplus_mask->text();
        break;
    case 4:
        arguments << "--perm-min" << ui->spinBox_oclhcplus_password_min->cleanText();
        arguments << "--perm-max" << ui->spinBox_oclhcplus_password_max->cleanText();
        break;
    case 6:
        if (!ui->lineEdit_oclhcplus_mask->text().isEmpty()) {
            mask_after_dict = ui->lineEdit_oclhcplus_mask->text();
        }
        break;
    case 7:
        if (!ui->lineEdit_oclhcplus_mask->text().isEmpty()) {
            mask_before_dict = ui->lineEdit_oclhcplus_mask->text();
        }
        break;
    }

    if (ui->groupBox_oclhcplus_custom_charset->isEnabled()) {
        if (ui->checkBox_oclhcplus_custom1->isChecked() && !ui->lineEdit_oclhcplus_custom1->text().isEmpty()) {
            arguments << "--custom-charset1" << ui->lineEdit_oclhcplus_custom1->text();
        }

        if (ui->checkBox_oclhcplus_custom2->isChecked() && !ui->lineEdit_oclhcplus_custom2->text().isEmpty()) {
            arguments << "--custom-charset2" << ui->lineEdit_oclhcplus_custom2->text();
        }

        if (ui->checkBox_oclhcplus_custom3->isChecked() && !ui->lineEdit_oclhcplus_custom3->text().isEmpty()) {
            arguments << "--custom-charset3" << ui->lineEdit_oclhcplus_custom3->text();
        }

        if (ui->checkBox_oclhcplus_custom4->isChecked() && !ui->lineEdit_oclhcplus_custom4->text().isEmpty()) {
            arguments << "--custom-charset4" << ui->lineEdit_oclhcplus_custom4->text();
        }
    }

    if (ui->checkBox_oclhcplus_hex_hash->isChecked()) {
        arguments << "--hex-charset";
    }

    if (ui->checkBox_oclhcplus_hex_salt->isChecked()) {
        arguments << "--hex-salt";
    }

    if(ui->checkBox_oclhcplus_outfile->isChecked() && !ui->lineEdit_oclhcplus_outfile->text().isEmpty()) {
        QFileInfo hash_fi(ui->lineEdit_oclhcplus_open_hashfile->text());
        QString outfile = ui->lineEdit_oclhcplus_outfile->text();
        outfile.replace("<unixtime>", QString::number(QDateTime::currentMSecsSinceEpoch() / 1000));
        outfile.replace("<hash>", hash_fi.fileName(), Qt::CaseInsensitive);
        arguments << "--outfile" << outfile;
    }

    if (ui->comboBox_oclhcplus_outfile_format->currentIndex() != 2) {
        arguments << "--outfile-format" << QString::number(ui->comboBox_oclhcplus_outfile_format->currentIndex()+1);
    }

    if (ui->checkBox_oclhcplus_async->isChecked()) {
        arguments << "--gpu-async";
    }

    if (!ui->lineEdit_oclhcplus_cpu_affinity->text().isEmpty()) {
        arguments << "--cpu-affinity" << ui->lineEdit_oclhcplus_cpu_affinity->text();
    }

    if (!ui->lineEdit_oclhcplus_devices->text().isEmpty() && ui->lineEdit_oclhcplus_devices->text() != "0") {
        arguments << "--gpu-devices" << ui->lineEdit_oclhcplus_devices->text();
    }

    if (!ui->spinBox_oclhcplus_accel->cleanText().isEmpty() && ui->spinBox_oclhcplus_accel->cleanText() != "8") {
        arguments << "--gpu-accel" << ui->spinBox_oclhcplus_accel->cleanText();
    }

    if (!ui->spinBox_oclhcplus_loops->cleanText().isEmpty() && ui->spinBox_oclhcplus_loops->cleanText() != "256") {
        arguments << "--gpu-loops" << ui->spinBox_oclhcplus_loops->cleanText();
    }

    if (!ui->spinBox_oclhcplus_watchdog->cleanText().isEmpty() && ui->spinBox_oclhcplus_watchdog->cleanText() != "90") {
        arguments << "--gpu-watchdog" << ui->spinBox_oclhcplus_watchdog->cleanText();
    }

    if (!ui->spinBox_oclhcplus_segment->cleanText().isEmpty() && ui->spinBox_oclhcplus_segment->cleanText() != "32") {
        arguments << "--segment-size" << ui->spinBox_oclhcplus_segment->cleanText();
    }

    if (!ui->lineEdit_oclhcplus_open_hashfile->text().isEmpty()) {
        arguments << ui->lineEdit_oclhcplus_open_hashfile->text();
    }

    if (mask_before_dict.length()) {
        arguments << mask_before_dict;
    }

    if (ui->groupBox_oclhcplus_wordlists->isEnabled()) {
        for(int i=0; i<ui->listWidget_oclhcplus_wordlist->count(); i++) {
            if(ui->listWidget_oclhcplus_wordlist->item(i)->checkState() == Qt::Checked) {
                arguments << ui->listWidget_oclhcplus_wordlist->item(i)->text();
            }
        }
    }

    if (mask_after_dict.length()) {
        arguments << mask_after_dict;
    }

    return arguments;
}

void MainWindow::on_pushButton_oclhcplus_execute_clicked()
{
    QStringList env = generate_terminal_env(1);
    QStringList arguments = oclhcplus_generate_arguments();
    oclhcplusCommandChanged(arguments.join(" "));

    if (ui->lineEdit_oclhcplus_open_hashfile->text().isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("Please choose a hash file.");
        msgBox.exec();
        return;
    }

    QProcess proc;
    proc.startDetached(envInfo.value("terminal"), env << arguments, envInfo.value("dir_oclhcplus"));
}

