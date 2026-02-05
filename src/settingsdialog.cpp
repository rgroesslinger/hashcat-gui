/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "settingsmanager.h"
#include "helperutils.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    this->readSettings();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::readSettings() {
    auto& settings = SettingsManager::instance();

    // hashcat path from saved settings
    ui->lineEdit_hc_path->setText(settings.getKey("hashcatPath"));

    // available terminals
    QMap <QString, QStringList> availableTermins = HelperUtils::getAvailableTerminals();
    ui->comboBox_terminal->addItems(availableTermins.keys());

    // terminal from saved settings
    ui->comboBox_terminal->setCurrentIndex(ui->comboBox_terminal->findText(settings.getKey("terminal")));
}

// Configure path to hashcat binary
void SettingsDialog::on_pushButton_settings_select_path_clicked()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter("Executable Files (*.exe *.bin);;All Files (*)");

    if (fileDialog.exec() == QDialog::Accepted) {
        QString fileName = fileDialog.selectedFiles().constFirst();
        ui->lineEdit_hc_path->setText(QDir::toNativeSeparators(fileName));
    }
}

void SettingsDialog::on_pushButton_save_clicked()
{
    // Save values in persistent settings
    auto& settings = SettingsManager::instance();
    settings.setKey("hashcatPath", ui->lineEdit_hc_path->text());
    settings.setKey("terminal", ui->comboBox_terminal->currentText());

    // accept() signals our parent that settings might have changed
    this->accept();
    this->close();
}


void SettingsDialog::on_pushButton_cancel_clicked()
{
    this->close();
}

