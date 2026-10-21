/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QJsonObject>
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

    // The settings as they are stored right now.
    //
    // The export is a backup of the configuration the application runs with,
    // so it holds what is saved and not what the dialog shows but has not
    // saved.
    QJsonObject savedSettings() const;

    // Fills the dialog with the settings of an imported file. Returns the
    // notes for the user.
    QStringList applyImportedSettings(const QJsonObject &settings);

private slots:
    void readSettings();
    void selectPathClicked();
    void clearProfileClicked();
    void openFolderClicked();
    void saveClicked();
    void cancelClicked();

private:
    // The tests read the widget state through the Ui the same way the main
    // window's tests do: friendship keeps it private instead of widening the
    // dialog's public surface for their sake.
    friend class TestSettingsDialog;

    // Fills the rows that show where the settings and the session are stored.
    void updatePathLabels();

    // The home directory as a tilde.
    static QString shortenPath(const QString &path);

    Ui::SettingsDialog *ui;

    // The language setting as it was when the dialog opened, so saveClicked()
    // can tell an actual change from "the user pressed Save".
    QString loadedLanguage;

    // The folder the settings are stored in, empty where there is none - the
    // registry on Windows has no folder to open.
    QString m_settingsFolder;
};

#endif // SETTINGSDIALOG_H
