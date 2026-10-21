/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "settingsmanager.h"
#include "helperutils.h"
#include "widgetstateserializer.h"
#include "appconstants.h"
#include <QFile>
#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    // The URL is fixed, so only the label around it is translatable.
    const QString downloadLink =
        QStringLiteral("<a href=\"%1\">%1</a>").arg(QString(AppConstants::Urls::Hashcat));
    ui->label_download->setText(tr("Download from %1").arg(downloadLink));
    ui->label_download->setOpenExternalLinks(true);

    readSettings();

    connect(ui->pushButton_select_path, &QPushButton::clicked, this,
            &SettingsDialog::selectPathClicked);
    connect(ui->pushButton_clear_profile, &QPushButton::clicked, this,
            &SettingsDialog::clearProfileClicked);
    connect(ui->pushButton_save, &QPushButton::clicked, this, &SettingsDialog::saveClicked);
    connect(ui->pushButton_cancel, &QPushButton::clicked, this, &SettingsDialog::cancelClicked);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::readSettings()
{
    auto &settings = SettingsManager::instance();

    ui->lineEdit_hc_path->setText(
        settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath));

    QMap<QString, QStringList> availableTerminals = HelperUtils::availableTerminals();
    ui->comboBox_terminal->addItems(availableTerminals.keys());

    ui->comboBox_terminal->setCurrentIndex(ui->comboBox_terminal->findText(
        settings.getKey<QString>(AppConstants::SettingsKeys::Terminal)));

    ui->checkBox_use_short_parameters->setChecked(
        settings.getKey<bool>(AppConstants::SettingsKeys::UseShortParameters));

    ui->checkBox_restore_session->setChecked(
        settings.getKey<bool>(AppConstants::SettingsKeys::RestoreSession, true));

    // Languages: the combo shows each language's own name and carries the
    // locale code as item data - that code is what gets stored. "System
    // default" (an empty code) means "whatever the environment says".
    ui->comboBox_language->addItem(tr("System default"), QString());
    for (const auto &language : AppConstants::Languages::Available) {
        ui->comboBox_language->addItem(QString::fromUtf8(language.name),
                                       QString::fromUtf8(language.code));
    }

    const QString stored = settings.getKey<QString>(
        AppConstants::SettingsKeys::Language, QString::fromUtf8(AppConstants::Languages::Default));
    int index = ui->comboBox_language->findData(stored);
    if (index < 0) {
        // A code we no longer offer (or a hand-edited file): follow the
        // environment again - English if no catalog answers for it.
        index =
            ui->comboBox_language->findData(QString::fromUtf8(AppConstants::Languages::Default));
    }
    ui->comboBox_language->setCurrentIndex(index);

    // What the combo shows is what Save will store, so that is what counts as
    // "unchanged": a stale code that fell back to "System default" must not
    // read as a pending change and trigger a restart.
    loadedLanguage = ui->comboBox_language->currentData().toString();
}

void SettingsDialog::selectPathClicked()
{
    QFileDialog fileDialog(this, tr("Select the hashcat executable"));
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter(tr("Executable Files (*.exe *.bin);;All Files (*)"));

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
    auto &settings = SettingsManager::instance();
    settings.setKey(AppConstants::SettingsKeys::HashcatPath, ui->lineEdit_hc_path->text());
    settings.setKey(AppConstants::SettingsKeys::Terminal, ui->comboBox_terminal->currentText());
    settings.setKey(AppConstants::SettingsKeys::UseShortParameters,
                    ui->checkBox_use_short_parameters->isChecked());
    settings.setKey(AppConstants::SettingsKeys::RestoreSession,
                    ui->checkBox_restore_session->isChecked());
    // The locale code, not the display text - see readSettings().
    const QString language = ui->comboBox_language->currentData().toString();
    settings.setKey(AppConstants::SettingsKeys::Language, language);

    // The catalog is loaded once, in main(), before the first widget exists.
    // Rather than rebuilding every open widget to swap it, tell the user the
    // setting takes effect on the next start.
    if (language != loadedLanguage) {
        QMessageBox::information(this, tr("Restart required"),
                                 tr("Restart hashcat-gui to apply the new language."));
    }

    // accept() signals our parent that settings might have changed
    accept();
    close();
}

void SettingsDialog::cancelClicked()
{
    close();
}

// Removes the profile the application stored at the last run.
//
// The button is not a setting, so it acts right away and not on Save.
void SettingsDialog::clearProfileClicked()
{
    const QString path = WidgetStateSerializer::defaultProfilePath();

    if (!QFile::exists(path)) {
        QMessageBox::information(this, tr("No saved profile"),
                                 tr("There is no saved profile to clear."));
        return;
    }

    if (!QFile::remove(path)) {
        QMessageBox::warning(this, tr("Clear failed"),
                             tr("The saved profile could not be deleted."));
        return;
    }

    QStringList notes;
    notes << tr("The saved profile has been deleted.");
    if (ui->checkBox_restore_session->isChecked()) {
        // The setting is not saved yet, so the profile is written again
        // unless the user turns the setting off before saving.
        notes << tr("It is written again when hashcat-gui exits.");
    }

    QMessageBox::information(this, tr("Profile cleared"), notes.join(QLatin1Char('\n')));
}
