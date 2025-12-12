/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2012-2025 Rainer Größlinger
 */

#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "helperutils.h"
#include "settingsmanager.h"
#include <QMessageBox>
#include <QFileInfo>

aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutdialog)
{
    ui->setupUi(this);
}

aboutDialog::~aboutDialog()
{
    delete ui;
}

void aboutDialog::on_pushButton_about_ok_clicked()
{
    this->close();
}

void aboutDialog::get_versions() {
    auto& settings = SettingsManager::instance();
    QFileInfo fileInfo(settings.hashcatPath());

    if (!settings.hashcatPath().isEmpty()) {
        ui->label_hc_version_text->setText(fileInfo.fileName());
        ui->label_hc_version->setText(HelperUtils::executeHashcat(QStringList() << "--version").remove('\n').remove('\r'));
    }
    ui->label_hc_gui_version->setText(QApplication::applicationVersion());
}
