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
    // saved. It reads the settings, not the dialog, so it needs no instance.
    static QJsonObject savedSettings();

    // Fills the dialog with the settings of an imported file. Returns the
    // notes for the user.
    QStringList applyImportedSettings(const QJsonObject &settings);

protected:
    // The dialog opens as large as its content. The height of the
    // word-wrapped labels is only known once they have been polished, so the
    // fit happens when the dialog is shown - and only then, so that a window
    // the user has resized is left alone.
    void showEvent(QShowEvent *event) override;

private slots:
    void readSettings();
    void selectPathClicked();
    void clearSessionClicked();
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

    // The dialog is fitted to its content once, on the first show.
    bool m_sizedToContent = false;
};

#endif // SETTINGSDIALOG_H
