/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "helperutils.h"
#include "settingsmanager.h"
#include "appconstants.h"
#include <QApplication>
#include <QFileInfo>
#include <QFont>
#include <QFutureWatcher>
#include <QPixmap>
#include <QSysInfo>

namespace
{
// The artwork the window and the taskbar show. It is part of the resources
// already, so the dialog does not need a second copy.
const QString &logoPath()
{
    static const QString path = QStringLiteral(":/icons/hicolor/128x128/apps/hashcat-gui.png");
    return path;
}
} // namespace

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    updateVersionLabel();
    updateSystemLabels();

    // The logo is scaled to the size the .ui gives the label, so the artwork
    // stays sharp on a high-dpi display without the .ui having to know about
    // the device pixel ratio.
    const QSize logoSize = ui->label_logo->minimumSize();
    const QPixmap logo(logoPath());
    if (!logo.isNull()) {
        ui->label_logo->setPixmap(
            logo.scaled(logoSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    ui->label_logo->setAlignment(Qt::AlignCenter);

    // The application name is the headline of the dialog.
    QFont nameFont = ui->label_app_name->font();
    nameFont.setBold(true);
    ui->label_app_name->setFont(nameFont);

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

// What the application runs on: the toolkit it is built with and the system
// underneath it. Both are asked for when the dialog opens, because neither
// of them can change while it is on screen.
void AboutDialog::updateSystemLabels()
{
    ui->label_qt_version->setText(QString::fromLatin1(qVersion()));
    ui->label_os->setText(QSysInfo::prettyProductName());
}

void AboutDialog::updateVersionLabel()
{
    auto &settings = SettingsManager::instance();
    QFileInfo fileInfo(settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath));

    if (!settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath).isEmpty()) {
        ui->label_hc_version_text->setText(fileInfo.fileName());

        QFutureWatcher<HashcatResult> *watcher = new QFutureWatcher<HashcatResult>(this);
        connect(watcher, &QFutureWatcher<HashcatResult>::finished, this, [this, watcher]() {
            const HashcatResult &result = watcher->result();

            if (result.exitStatus != QProcess::NormalExit || result.exitCode != 0) {
                ui->label_hc_version->setText(
                    tr("Error: %1").arg(result.standardError.simplified()));
            } else {
                ui->label_hc_version->setText(result.standardOutput.simplified());
            }

            watcher->deleteLater();
        });

        watcher->setFuture(
            HelperUtils::executeHashcat(QStringList() << AppConstants::Hashcat::Version));
    }

    ui->label_hc_gui_version->setText(QApplication::applicationVersion());
}
