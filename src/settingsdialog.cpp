/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2025 Rainer Größlinger
 */

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "settingsmanager.h"


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

    ui->lineEdit_hc_path->setText(settings.hashcatPath());
}

// Configure path to hashcat binary
void SettingsDialog::on_pushButton_settings_select_path_clicked()
{
    QString hashcatPath = QFileDialog::getOpenFileName();
    if (!hashcatPath.isNull()) {
        ui->lineEdit_hc_path->setText(QDir::toNativeSeparators(hashcatPath));
    }
}

// Save selected path in persistent settings
void SettingsDialog::on_pushButton_save_clicked()
{
    auto& settings = SettingsManager::instance();
    settings.hashcatPath(ui->lineEdit_hc_path->text());

    this->close();
}


void SettingsDialog::on_pushButton_cancel_clicked()
{
    this->close();
}

