/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2012-2026 Rainer Größlinger
 */

#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "helperutils.h"
#include "settingsmanager.h"
#include <QMessageBox>
#include <QFileInfo>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->updateVersionLabel();
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_pushButton_about_ok_clicked()
{
    this->close();
}

void AboutDialog::updateVersionLabel() {
    auto& settings = SettingsManager::instance();
    QFileInfo fileInfo(settings.getKey("hashcatPath"));

    if (!settings.getKey("hashcatPath").isEmpty()) {
        ui->label_hc_version_text->setText(fileInfo.fileName());
        ui->label_hc_version->setText(HelperUtils::executeHashcat(QStringList() << "--version").remove('\n').remove('\r'));
    }
    ui->label_hc_gui_version->setText(QApplication::applicationVersion());
}
