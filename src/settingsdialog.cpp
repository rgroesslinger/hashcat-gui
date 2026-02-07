/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "settingsmanager.h"
#include "helperutils.h"
#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    readSettings();

    connect(ui->pushButton_settings_select_path, &QPushButton::clicked, this, &SettingsDialog::selectPathClicked);
    connect(ui->pushButton_save, &QPushButton::clicked, this, &SettingsDialog::saveClicked);
    connect(ui->pushButton_cancel, &QPushButton::clicked, this, &SettingsDialog::cancelClicked);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::readSettings() {
    auto &settings = SettingsManager::instance();

    // hashcat path from saved settings
    ui->lineEdit_hc_path->setText(settings.getKey<QString>("hashcatPath"));

    // available terminals
    QMap<QString, QStringList> availableTermins = HelperUtils::getAvailableTerminals();
    ui->comboBox_terminal->addItems(availableTermins.keys());

    // terminal from saved settings
    ui->comboBox_terminal->setCurrentIndex(ui->comboBox_terminal->findText(settings.getKey<QString>("terminal")));

    // use short parameters
    ui->checkBox_use_short_parameters->setChecked(settings.getKey<bool>("useShortParameters"));
}

// Configure path to hashcat binary
void SettingsDialog::selectPathClicked()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter("Executable Files (*.exe *.bin);;All Files (*)");

    if (fileDialog.exec() == QDialog::Accepted) {
        QString fileName = fileDialog.selectedFiles().constFirst();
        QFileInfo file(fileName);
        if (!file.isFile() || !file.isExecutable()) {
            QMessageBox::warning(this, tr("Invalid file"),
                                 tr("The selected file is not an executable."));
            return;
        }
        ui->lineEdit_hc_path->setText(QDir::toNativeSeparators(fileName));
    }
}

void SettingsDialog::saveClicked()
{
    // Save values in persistent settings
    auto &settings = SettingsManager::instance();
    settings.setKey("hashcatPath", ui->lineEdit_hc_path->text());
    settings.setKey("terminal", ui->comboBox_terminal->currentText());
    settings.setKey("useShortParameters", ui->checkBox_use_short_parameters->isChecked());

    // accept() signals our parent that settings might have changed
    accept();
    close();
}


void SettingsDialog::cancelClicked()
{
    close();
}

