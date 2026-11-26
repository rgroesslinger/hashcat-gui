/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonValue>
#include <QStringList>

namespace Ui
{
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog() override;

    SettingsDialog(const SettingsDialog &) = delete;
    SettingsDialog &operator=(const SettingsDialog &) = delete;
    SettingsDialog(SettingsDialog &&) = delete;
    SettingsDialog &operator=(SettingsDialog &&) = delete;

    // The settings as they are stored right now.
    //
    // The export is a backup of the configuration the application runs with,
    // so it holds what is saved and not what the dialog shows but has not
    // saved. It reads the settings, not the dialog, so it needs no instance.
    static QJsonObject savedSettings();

    // Fills the dialog with the settings of an imported file. Returns the
    // notes for the user.
    QStringList applyImportedSettings(const QJsonObject &settings);

protected:
    // Fits the dialog once, on its first show.
    void showEvent(QShowEvent *event) override;

private slots:
    void readSettings();
    void selectPathClicked();
    void openFolderClicked();
    void openSessionFolderClicked();
    void saveClicked();
    void cancelClicked();

private:
    // Where a value comes from. A stored value the dialog no longer offers
    // falls back to the default, because the dialog has to show something; an
    // imported one is left alone and named, because the user chose it.
    enum class ValueSource : quint8 {
        Stored,
        Imported,
    };

    friend class TestSettingsDialog;

    // Applies one setting to its widget. Returns a note when the value cannot
    // be used; stored and imported values differ only in the fallback.
    QString applyValue(const QString &key, const QJsonValue &value, ValueSource source);

    void updatePathLabels();

    void updateStatusLabels();
    void updateHashcatStatus();
    void updateTerminalStatus();

    // Renames a malformed settings file aside and retries the save.
    [[nodiscard]] bool recoverMalformedSettingsFile();

    static QString shortenPath(const QString &path);

    Ui::SettingsDialog *ui;

    // The language setting as it was when the dialog opened, so saveClicked()
    // can tell an actual change from "the user pressed Save".
    QString loadedLanguage;

    // The folders the two files are stored in, empty where there is none to open.
    QString m_settingsFolder;
    QString m_sessionFolder;

    // The terminal the last applied value named: one that is no longer
    // installed is named instead of just "select one", and Save keeps it
    // rather than storing the combo's empty selection (widgetValue()).
    QString m_configuredTerminal;

    bool m_sizedToContent = false;
};

#endif // SETTINGSDIALOG_H
