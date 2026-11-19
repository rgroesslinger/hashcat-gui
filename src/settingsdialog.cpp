/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "settingsdialog.h"

#include "appconstants.h"
#include "appearance.h"
#include "executable.h"
#include "settingsmanager.h"
#include "statefile.h"
#include "terminalcatalog.h"
#include "ui_settingsdialog.h"

#include <QDesktopServices>
#include <QDialogButtonBox>
#include <QDir>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>
#include <QPushButton>
#include <QShowEvent>
#include <QStandardItemModel>
#include <QUrl>
#include <algorithm>

namespace
{
// The type a setting takes. A value of another type cannot be used: the
// setting would keep what it has, and the file is reported.
bool hasExpectedType(QLatin1StringView key, const QJsonValue &value)
{
    for (const AppConstants::SettingsKeys::Entry &entry : AppConstants::SettingsKeys::All) {
        if (entry.key != key) {
            continue;
        }

        switch (entry.type) {
        case AppConstants::SettingsKeys::Type::Boolean:
            return value.isBool();
        case AppConstants::SettingsKeys::Type::String:
            return value.isString();
        }
    }

    return false;
}

// The terminal combo lists every supported terminal and disables the ones
// that are not installed. True when at least one entry can be chosen.
bool hasSelectableTerminal(const QComboBox &combo)
{
    const auto *model = qobject_cast<const QStandardItemModel *>(combo.model());
    if (!model) {
        return combo.count() > 0;
    }

    for (int row = 0; row < combo.count(); ++row) {
        const QStandardItem *item = model->item(row);
        if (item && item->isEnabled()) {
            return true;
        }
    }
    return false;
}

// The value the dialog shows for a setting, in the type the setting takes.
// configuredTerminal is the terminal the last applied value named: the combo
// shows no selection for one that is not installed, and Save keeps it.
QJsonValue widgetValue(const Ui::SettingsDialog &ui, QLatin1StringView key,
                       const QString &configuredTerminal)
{
    if (key == AppConstants::SettingsKeys::HashcatPath) {
        return ui.lineEdit_hc_path->text();
    }
    if (key == AppConstants::SettingsKeys::Terminal) {
        // A configured terminal that is not installed shows no selection, but
        // it stays the configured one: saving another setting must not clear
        // it. An empty configured value is the intentional "none" and saves
        // as itself.
        if (ui.comboBox_terminal->currentIndex() < 0 && !configuredTerminal.isEmpty()) {
            return configuredTerminal;
        }
        return ui.comboBox_terminal->currentText();
    }
    if (key == AppConstants::SettingsKeys::UseShortParameters) {
        return ui.checkBox_use_short_parameters->isChecked();
    }
    if (key == AppConstants::SettingsKeys::RestoreSession) {
        return ui.checkBox_restore_session->isChecked();
    }
    if (key == AppConstants::SettingsKeys::ColorScheme) {
        return ui.comboBox_color_scheme->currentData().toString();
    }
    if (key == AppConstants::SettingsKeys::Language) {
        return ui.comboBox_language->currentData().toString();
    }

    return {};
}
} // namespace

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    // The URL is fixed, so only the label around it is translatable.
    const QString downloadLink =
        QStringLiteral("<a href=\"%1\">%1</a>").arg(QString(AppConstants::Urls::Hashcat));
    ui->label_download->setText(tr("Download from %1").arg(downloadLink));
    ui->label_download->setOpenExternalLinks(true);

    ui->label_terminal->setBuddy(ui->comboBox_terminal);
    ui->label_language->setBuddy(ui->comboBox_language);
    ui->label_color_scheme->setBuddy(ui->comboBox_color_scheme);
    ui->label_parameters->setBuddy(ui->checkBox_use_short_parameters);
    ui->label_session_path_text->setBuddy(ui->lineEdit_session_path);
    ui->label_settings_path_text->setBuddy(ui->lineEdit_settings_path);

    ui->comboBox_color_scheme->setToolTip(
        tr("Not all color schemes are available on all platforms."));

    ui->comboBox_terminal->setToolTip(tr("The terminal hashcat is started in."));

    ui->checkBox_use_short_parameters->setToolTip(
        tr("Use the short option names (-m) instead of the long ones (--hash-type) in the "
           "command."));

    // The buttons of the dialog: Save is what Enter triggers and Cancel what
    // Esc triggers.
    QPushButton *saveButton = ui->buttonBox->addButton(tr("Save"), QDialogButtonBox::AcceptRole);
    saveButton->setObjectName(QStringLiteral("pushButton_save"));
    saveButton->setDefault(true);
    QPushButton *cancelButton =
        ui->buttonBox->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);
    cancelButton->setObjectName(QStringLiteral("pushButton_cancel"));

    readSettings();

    connect(ui->pushButton_select_path, &QPushButton::clicked, this,
            &SettingsDialog::selectPathClicked);
    connect(ui->pushButton_open_folder, &QPushButton::clicked, this,
            &SettingsDialog::openFolderClicked);
    connect(ui->pushButton_open_session_folder, &QPushButton::clicked, this,
            &SettingsDialog::openSessionFolderClicked);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::saveClicked);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::cancelClicked);

    // Both rows say what is missing as the user edits them, not only at launch.
    connect(ui->lineEdit_hc_path, &QLineEdit::textChanged, this,
            &SettingsDialog::updateHashcatStatus);
    connect(ui->comboBox_terminal, qOverload<int>(&QComboBox::currentIndexChanged), this,
            &SettingsDialog::updateTerminalStatus);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    if (m_sizedToContent) {
        return;
    }
    m_sizedToContent = true;

    resize(sizeHint());
}

void SettingsDialog::readSettings()
{
    auto &settings = SettingsManager::instance();

    // The combos are filled first: a stored value can only find its entry
    // once the entries exist.
    //
    // Every terminal this platform offers is listed, installed or not: a
    // machine without one still sees what hashcat-gui can use, and the
    // entries that cannot be started are disabled instead of hidden.
    const QStringList offeredTerminals = TerminalCatalog::offeredTerminals();
    ui->comboBox_terminal->addItems(offeredTerminals);
    if (const auto *model = qobject_cast<QStandardItemModel *>(ui->comboBox_terminal->model())) {
        for (int row = 0; row < ui->comboBox_terminal->count(); ++row) {
            QStandardItem *item = model->item(row);
            if (item && !TerminalCatalog::isInstalled(offeredTerminals.at(row))) {
                item->setEnabled(false);
            }
        }
    }
    ui->comboBox_terminal->setPlaceholderText(tr("Select a terminal"));

    // The color scheme: the combo carries the value that gets stored, so
    // that the display text can be translated freely.
    ui->comboBox_color_scheme->addItem(tr("System default"), QString());
    ui->comboBox_color_scheme->addItem(tr("Light"),
                                       QString(AppConstants::Appearance::ColorSchemeLight));
    ui->comboBox_color_scheme->addItem(tr("Dark"),
                                       QString(AppConstants::Appearance::ColorSchemeDark));

    // Languages: the combo shows each language's own name and carries the
    // locale code as item data - that code is what gets stored. "System
    // default" (an empty code) means "whatever the environment says".
    ui->comboBox_language->addItem(tr("System default"), QString());
    for (const auto &language : AppConstants::Languages::Available) {
        ui->comboBox_language->addItem(QString::fromUtf8(language.name),
                                       QString::fromUtf8(language.code));
    }

    // One way in for every stored value: it is the same code the import uses.
    for (const AppConstants::SettingsKeys::Entry &entry : AppConstants::SettingsKeys::All) {
        const QString key = QString(entry.key);
        const QJsonValue value =
            entry.type == AppConstants::SettingsKeys::Type::Boolean
                ? QJsonValue(settings.getKey<bool>(
                      key, AppConstants::SettingsKeys::defaultOf<bool>(entry.key)))
                : QJsonValue(settings.getKey<QString>(
                      key, AppConstants::SettingsKeys::defaultOf<QString>(entry.key)));
        applyValue(key, value, ValueSource::Stored);
    }

    // Nothing configured: offer the hashcat PATH already finds, so a fresh
    // install does not have to point at a binary it could start as it is. Only
    // an empty field is filled - a path the user chose is never replaced - and
    // nothing is stored until Save.
    if (ui->lineEdit_hc_path->text().isEmpty()) {
        const QString onPath = TerminalCatalog::findHashcatInPath();
        if (!onPath.isEmpty()) {
            ui->lineEdit_hc_path->setText(QDir::toNativeSeparators(onPath));
        }
    }

    // What the combo shows is what Save will store, so that is what counts as
    // "unchanged": a stale code that fell back to "System default" must not
    // read as a pending change and trigger a restart. The terminal is the one
    // exception: a configured one that is not installed shows no selection,
    // and Save keeps it instead of clearing it (widgetValue()).
    loadedLanguage = ui->comboBox_language->currentData().toString();

    updatePathLabels();
    updateStatusLabels();
}

// Shows where the application keeps its settings and its session.
//
// The home directory is shortened to a tilde, the way the documentation
// writes it, so that the paths stay readable.
void SettingsDialog::updatePathLabels()
{
    const QString settingsFile = SettingsManager::instance().fileName();
    const QString sessionFile = StateFile::sessionFilePath();

    // The settings live in a file on every platform now, so there is always
    // a folder to open next to them.
    m_settingsFolder =
        QFileInfo(settingsFile).isAbsolute() ? QFileInfo(settingsFile).absolutePath() : QString();
    ui->pushButton_open_folder->setEnabled(!m_settingsFolder.isEmpty());

    m_sessionFolder =
        QFileInfo(sessionFile).isAbsolute() ? QFileInfo(sessionFile).absolutePath() : QString();
    ui->pushButton_open_session_folder->setEnabled(!m_sessionFolder.isEmpty());

    // Qt keeps paths with "/" internally; show the native separator so a
    // path copied out of the field is valid in another application.
    ui->lineEdit_settings_path->setText(QDir::toNativeSeparators(shortenPath(settingsFile)));
    ui->lineEdit_session_path->setText(QDir::toNativeSeparators(shortenPath(sessionFile)));

    // setText() leaves the cursor at the end, and a read-only field scrolls
    // to it: the path would show its last characters instead of its folder.
    ui->lineEdit_settings_path->setCursorPosition(0);
    ui->lineEdit_session_path->setCursorPosition(0);
}

// The home directory as a tilde.
//
// Only a path inside the home directory is one: /home/alice-other is not
// /home/alice, and shortening it would make it look as if it were.
QString SettingsDialog::shortenPath(const QString &path)
{
#ifdef Q_OS_WIN
    // A leading "~" is not understood by Explorer, so keep the full path.
    return path;
#endif

    const QString home = QDir::homePath();
    if (home.isEmpty() || !path.startsWith(home)) {
        return path;
    }
    if (path.size() > home.size() && path.at(home.size()) != QLatin1Char('/')) {
        return path;
    }

    return QLatin1Char('~') + path.mid(home.size());
}

void SettingsDialog::openFolderClicked()
{
    if (m_settingsFolder.isEmpty()) {
        return;
    }

    // A file-backed location opens in the file manager.
    QDesktopServices::openUrl(QUrl::fromLocalFile(m_settingsFolder));
}

void SettingsDialog::openSessionFolderClicked()
{
    if (m_sessionFolder.isEmpty()) {
        return;
    }

    QDesktopServices::openUrl(QUrl::fromLocalFile(m_sessionFolder));
}

QJsonObject SettingsDialog::savedSettings()
{
    auto &settings = SettingsManager::instance();

    QJsonObject stored;
    for (const AppConstants::SettingsKeys::Entry &entry : AppConstants::SettingsKeys::All) {
        const QString key = QString(entry.key);
        switch (entry.type) {
        case AppConstants::SettingsKeys::Type::Boolean:
            stored.insert(key, settings.getKey<bool>(
                                   key, AppConstants::SettingsKeys::defaultOf<bool>(entry.key)));
            break;
        case AppConstants::SettingsKeys::Type::String:
            stored.insert(key, settings.getKey<QString>(
                                   key, AppConstants::SettingsKeys::defaultOf<QString>(entry.key)));
            break;
        }
    }

    return stored;
}

QStringList SettingsDialog::applyImportedSettings(const QJsonObject &settings)
{
    QStringList notes;

    // The values that could not be applied: the keys this version does not
    // know, and the values that are of a type the setting does not take.
    QStringList wrongTypes;

    // A key the file mentions is a value the user chose, even when it is
    // empty: an empty language or color scheme is the system default, and an
    // empty path says that hashcat is not configured.
    for (const AppConstants::SettingsKeys::Entry &entry : AppConstants::SettingsKeys::All) {
        const QString key = QString(entry.key);
        if (!settings.contains(key)) {
            continue;
        }

        const QJsonValue value = settings.value(key);
        if (!hasExpectedType(entry.key, value)) {
            wrongTypes << key;
            continue;
        }

        const QString note = applyValue(key, value, ValueSource::Imported);
        if (!note.isEmpty()) {
            notes << note;
        }
    }

    // The keys this version does not know: the file was made by another
    // version, or it was edited by hand. They are named in one sentence, so
    // that a file with many of them stays readable.
    const QStringList keys = settings.keys();
    QStringList unknownKeys;
    for (const QString &key : keys) {
        const bool known =
            std::ranges::any_of(AppConstants::SettingsKeys::All,
                                [&key](const AppConstants::SettingsKeys::Entry &entry) {
                                    return QString(entry.key) == key;
                                });
        if (!known) {
            unknownKeys << key;
        }
    }

    const QString unknownNote = StateFile::unknownValuesNote(unknownKeys);
    if (!unknownNote.isEmpty()) {
        notes << unknownNote;
    }

    const QString wrongTypeNote = StateFile::wrongTypeValuesNote(wrongTypes);
    if (!wrongTypeNote.isEmpty()) {
        notes << wrongTypeNote;
    }

    updateStatusLabels();
    return notes;
}

QString SettingsDialog::applyValue(const QString &key, const QJsonValue &value, ValueSource source)
{
    if (key == AppConstants::SettingsKeys::HashcatPath) {
        const QString path = value.toString();
        ui->lineEdit_hc_path->setText(path);

        // A path of another machine is the normal case of an import. It is
        // applied - the user reviews the dialog before saving - and named, so
        // that the missing file is not a surprise later.
        if (source == ValueSource::Imported && !path.isEmpty()
            && Executable::resolve(path).isEmpty()) {
            return tr("The hashcat path “%1” was not found on this computer.").arg(path);
        }
        return {};
    }

    if (key == AppConstants::SettingsKeys::Terminal) {
        const QString terminal = value.toString();
        m_configuredTerminal = terminal;
        const int index = ui->comboBox_terminal->findText(terminal);
        // The combo lists terminals that are not installed too, but a
        // disabled entry cannot be chosen.
        if (index >= 0 && TerminalCatalog::isInstalled(terminal)) {
            ui->comboBox_terminal->setCurrentIndex(index);
            return {};
        }

        // No entry at all, or one that cannot be used: the combo has no
        // empty item, but no selection is how it shows "none". The status
        // below the combo names a terminal that is gone; the import says so
        // in its notes on top of that.
        if (terminal.isEmpty() || source == ValueSource::Stored) {
            ui->comboBox_terminal->setCurrentIndex(-1);
            return {};
        }
        return tr("The terminal “%1” is not installed and was not applied.").arg(terminal);
    }

    if (key == AppConstants::SettingsKeys::UseShortParameters) {
        ui->checkBox_use_short_parameters->setChecked(value.toBool());
        return {};
    }

    if (key == AppConstants::SettingsKeys::RestoreSession) {
        ui->checkBox_restore_session->setChecked(value.toBool());
        return {};
    }

    if (key == AppConstants::SettingsKeys::ColorScheme) {
        const QString scheme = value.toString();
        const int index = ui->comboBox_color_scheme->findData(scheme);
        if (index >= 0) {
            ui->comboBox_color_scheme->setCurrentIndex(index);
            return {};
        }
        if (source == ValueSource::Stored) {
            // A value we no longer offer: follow the system again.
            ui->comboBox_color_scheme->setCurrentIndex(
                ui->comboBox_color_scheme->findData(QString()));
            return {};
        }
        return tr("The color scheme “%1” is not offered and was not applied.").arg(scheme);
    }

    if (key == AppConstants::SettingsKeys::Language) {
        const QString language = value.toString();
        const int index = ui->comboBox_language->findData(language);
        if (index >= 0) {
            ui->comboBox_language->setCurrentIndex(index);
            return {};
        }
        if (source == ValueSource::Stored) {
            // A code we no longer offer (or a hand-edited file): follow the
            // environment again - English if no catalog answers for it.
            ui->comboBox_language->setCurrentIndex(
                ui->comboBox_language->findData(AppConstants::SettingsKeys::defaultOf<QString>(
                    AppConstants::SettingsKeys::Language)));
            return {};
        }
        return tr("The language “%1” is not offered and was not applied.").arg(language);
    }

    return {};
}

void SettingsDialog::updateStatusLabels()
{
    updateHashcatStatus();
    updateTerminalStatus();
}

void SettingsDialog::updateHashcatStatus()
{
    const QString path = ui->lineEdit_hc_path->text();
    const bool executable = !Executable::resolve(path).isEmpty();

    // The download link belongs to the same help block as the path status: it
    // is only useful while no executable has been selected.
    ui->label_hashcat_status->setVisible(!executable);
    ui->label_download->setVisible(!executable);

    if (executable) {
        return;
    }

    ui->label_hashcat_status->setText(
        path.isEmpty() ? tr("No hashcat executable is configured; hashcat cannot be started.")
                       : tr("The configured hashcat executable was not found on this computer."));
}

void SettingsDialog::updateTerminalStatus()
{
    if (!hasSelectableTerminal(*ui->comboBox_terminal)) {
        ui->label_terminal_status->setText(TerminalCatalog::noSupportedTerminalMessage());
        ui->label_terminal_status->setVisible(true);
        return;
    }

    if (ui->comboBox_terminal->currentIndex() < 0) {
        ui->label_terminal_status->setText(
            m_configuredTerminal.isEmpty()
                ? tr("Select the terminal hashcat is started in.")
                : tr("The configured terminal “%1” is not installed.").arg(m_configuredTerminal));
        ui->label_terminal_status->setVisible(true);
        return;
    }

    ui->label_terminal_status->setVisible(false);
}

void SettingsDialog::selectPathClicked()
{
    QFileDialog fileDialog(this, tr("Select the hashcat executable"));
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter(tr("Executable Files (*.exe *.bin hashcat);;All Files (*)"));

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

    // One way out for every setting: the table names the key and its type,
    // the widget provides the value.
    for (const AppConstants::SettingsKeys::Entry &entry : AppConstants::SettingsKeys::All) {
        const QJsonValue value = widgetValue(*ui, entry.key, m_configuredTerminal);
        switch (entry.type) {
        case AppConstants::SettingsKeys::Type::Boolean:
            settings.setKey(QString(entry.key), value.toBool());
            break;
        case AppConstants::SettingsKeys::Type::String:
            settings.setKey(QString(entry.key), value.toString());
            break;
        }
    }

    // The color scheme takes effect right away, no restart needed.
    const QString colorScheme = ui->comboBox_color_scheme->currentData().toString();
    Appearance::applyColorScheme(colorScheme);

    // The catalog is loaded once, in main(), before the first widget exists.
    // Rather than rebuilding every open widget to swap it, tell the user the
    // setting takes effect on the next start.
    const QString language = ui->comboBox_language->currentData().toString();
    if (language != loadedLanguage) {
        QMessageBox::information(this, tr("Restart required"),
                                 tr("Restart hashcat-gui to apply the new language."));
    }

    // accept() signals our parent that settings might have changed
    accept();
}

void SettingsDialog::cancelClicked()
{
    reject();
}
