/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "helperutils.h"
#include "settingsmanager.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QFutureWatcher>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    updateVersionLabel();

    connect(ui->pushButton_ok, &QPushButton::clicked, this, &AboutDialog::okClicked);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::okClicked()
{
    close();
}

void AboutDialog::updateVersionLabel()
{
    auto &settings = SettingsManager::instance();
    QFileInfo fileInfo(settings.getKey<QString>("hashcatPath"));

    if (!settings.getKey<QString>("hashcatPath").isEmpty()) {
        ui->label_hc_version_text->setText(fileInfo.fileName());

        QFutureWatcher<HashcatResult> *watcher = new QFutureWatcher<HashcatResult>(this);
        connect(watcher, &QFutureWatcher<HashcatResult>::finished, this, [this, watcher]() {
            const HashcatResult &result = watcher->result();

            if (result.exitStatus != QProcess::NormalExit || result.exitCode != 0) {
                ui->label_hc_version->setText(tr("Error: %1").arg(result.standardError.simplified()));
            } else {
                ui->label_hc_version->setText(result.standardOutput.simplified());
            }

            watcher->deleteLater();
        });

        watcher->setFuture(HelperUtils::executeHashcat(QStringList() << "--version"));
    }

    ui->label_hc_gui_version->setText(QApplication::applicationVersion());
}
