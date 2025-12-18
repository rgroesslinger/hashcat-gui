/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2025 Rainer Größlinger
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

    // hashcat path
    ui->lineEdit_hc_path->setText(settings.getKey("hashcatPath"));

    // available terminals
    QMap <QString, QStringList> availableTermins = HelperUtils::getAvailableTerminals();

    for (const QString& key : availableTermins.keys()) {
        ui->comboBox_terminal->addItem(key);
        // Pre-select a terminal if we have one in the config
        if (key == settings.getKey("terminal")) {
            int lastIndex = ui->comboBox_terminal->count() - 1;
            ui->comboBox_terminal->setCurrentIndex(lastIndex);
        }
    }
}

// Configure path to hashcat binary
void SettingsDialog::on_pushButton_settings_select_path_clicked()
{
    QString hashcatPath = QFileDialog::getOpenFileName();
    if (!hashcatPath.isNull()) {
        ui->lineEdit_hc_path->setText(QDir::toNativeSeparators(hashcatPath));
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

