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
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonObject>
#include <QMessageBox>
#include <QUrl>

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
    connect(ui->pushButton_open_folder, &QPushButton::clicked, this,
            &SettingsDialog::openFolderClicked);
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

    updatePathLabels();

    QMap<QString, QStringList> availableTerminals = HelperUtils::availableTerminals();
    ui->comboBox_terminal->addItems(availableTerminals.keys());

    ui->comboBox_terminal->setCurrentIndex(ui->comboBox_terminal->findText(
        settings.getKey<QString>(AppConstants::SettingsKeys::Terminal)));

    ui->checkBox_use_short_parameters->setChecked(
        settings.getKey<bool>(AppConstants::SettingsKeys::UseShortParameters));

    ui->checkBox_restore_session->setChecked(
        settings.getKey<bool>(AppConstants::SettingsKeys::RestoreSession, true));

    // The color scheme: the combo carries the value that gets stored, so
    // that the display text can be translated freely.
    ui->comboBox_color_scheme->addItem(tr("System default"), QString());
    ui->comboBox_color_scheme->addItem(tr("Light"),
                                       QString(AppConstants::Appearance::ColorSchemeLight));
    ui->comboBox_color_scheme->addItem(tr("Dark"),
                                       QString(AppConstants::Appearance::ColorSchemeDark));

    const QString storedScheme =
        settings.getKey<QString>(AppConstants::SettingsKeys::ColorScheme, QString());
    int schemeIndex = ui->comboBox_color_scheme->findData(storedScheme);
    if (schemeIndex < 0) {
        // A value we no longer offer: follow the system again.
        schemeIndex = ui->comboBox_color_scheme->findData(QString());
    }
    ui->comboBox_color_scheme->setCurrentIndex(schemeIndex);

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

// Shows where the application keeps its settings and its session.
//
// The home directory is shortened to a tilde, the way the documentation
// writes it, so that the paths stay readable.
void SettingsDialog::updatePathLabels()
{
    const QString settingsFile = SettingsManager::instance().fileName();
    const QString profileFile = WidgetStateSerializer::defaultProfilePath();

    // The settings live in a file on most systems and in the registry on
    // Windows - where there is no folder to open.
    m_settingsFolder =
        QFileInfo(settingsFile).isAbsolute() ? QFileInfo(settingsFile).absolutePath() : QString();
    ui->pushButton_open_folder->setEnabled(!m_settingsFolder.isEmpty());

    ui->lineEdit_settings_path->setText(shortenPath(settingsFile));
    ui->lineEdit_profile_path->setText(shortenPath(profileFile));
}

// The home directory as a tilde.
QString SettingsDialog::shortenPath(const QString &path)
{
    const QString home = QDir::homePath();
    if (!home.isEmpty() && path.startsWith(home)) {
        return QLatin1Char('~') + path.mid(home.length());
    }
    return path;
}

// Opens the folder the settings are stored in.
void SettingsDialog::openFolderClicked()
{
    if (m_settingsFolder.isEmpty()) {
        return;
    }

    QDesktopServices::openUrl(QUrl::fromLocalFile(m_settingsFolder));
}

// The settings as they are stored right now.
//
// The export is a backup of the configuration the application runs with, so
// it holds what is saved and not what the dialog shows but has not saved.
QJsonObject SettingsDialog::savedSettings() const
{
    auto &settings = SettingsManager::instance();

    QJsonObject stored;
    stored.insert(AppConstants::SettingsKeys::HashcatPath,
                  settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath));
    stored.insert(AppConstants::SettingsKeys::Terminal,
                  settings.getKey<QString>(AppConstants::SettingsKeys::Terminal));
    stored.insert(AppConstants::SettingsKeys::UseShortParameters,
                  settings.getKey<bool>(AppConstants::SettingsKeys::UseShortParameters));
    stored.insert(AppConstants::SettingsKeys::RestoreSession,
                  settings.getKey<bool>(AppConstants::SettingsKeys::RestoreSession, true));
    stored.insert(AppConstants::SettingsKeys::Language,
                  settings.getKey<QString>(AppConstants::SettingsKeys::Language));
    stored.insert(AppConstants::SettingsKeys::ColorScheme,
                  settings.getKey<QString>(AppConstants::SettingsKeys::ColorScheme));

    return stored;
}

// Fills the dialog with the settings of an imported file.
//
// Returns the notes for the user: a value the dialog cannot take - a
// terminal that is not installed - is worth a word.
QStringList SettingsDialog::applyImportedSettings(const QJsonObject &settings)
{
    QStringList notes;

    // A key the file mentions is a value the user chose, even when it is
    // empty: an empty language or color scheme is the system default, and an
    // empty path says that hashcat is not configured.
    if (settings.contains(AppConstants::SettingsKeys::HashcatPath)) {
        ui->lineEdit_hc_path->setText(
            settings.value(AppConstants::SettingsKeys::HashcatPath).toString());
    }

    // The terminal is the one exception: its combo has no empty entry, so
    // there is nothing an empty value could select.
    const QString terminal = settings.value(AppConstants::SettingsKeys::Terminal).toString();
    if (!terminal.isEmpty()) {
        const int index = ui->comboBox_terminal->findText(terminal);
        if (index >= 0) {
            ui->comboBox_terminal->setCurrentIndex(index);
        } else {
            notes << tr("The terminal %1 is not installed and was not applied.").arg(terminal);
        }
    }

    if (settings.contains(AppConstants::SettingsKeys::UseShortParameters)) {
        ui->checkBox_use_short_parameters->setChecked(
            settings.value(AppConstants::SettingsKeys::UseShortParameters).toBool());
    }

    if (settings.contains(AppConstants::SettingsKeys::RestoreSession)) {
        ui->checkBox_restore_session->setChecked(
            settings.value(AppConstants::SettingsKeys::RestoreSession).toBool());
    }

    if (settings.contains(AppConstants::SettingsKeys::Language)) {
        const QString language = settings.value(AppConstants::SettingsKeys::Language).toString();
        const int index = ui->comboBox_language->findData(language);
        if (index >= 0) {
            ui->comboBox_language->setCurrentIndex(index);
        } else {
            notes << tr("The language %1 is not offered and was not applied.").arg(language);
        }
    }

    if (settings.contains(AppConstants::SettingsKeys::ColorScheme)) {
        const QString colorScheme =
            settings.value(AppConstants::SettingsKeys::ColorScheme).toString();
        const int index = ui->comboBox_color_scheme->findData(colorScheme);
        if (index >= 0) {
            ui->comboBox_color_scheme->setCurrentIndex(index);
        } else {
            notes << tr("The color scheme %1 is not offered and was not applied.").arg(colorScheme);
        }
    }

    return notes;
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

    // The color scheme, like the locale code, is stored as the value of the
    // combo entry and not as its text - see readSettings().
    const QString colorScheme = ui->comboBox_color_scheme->currentData().toString();
    settings.setKey(AppConstants::SettingsKeys::ColorScheme, colorScheme);
    // The color scheme takes effect right away, no restart needed.
    HelperUtils::applyColorScheme(colorScheme);
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
