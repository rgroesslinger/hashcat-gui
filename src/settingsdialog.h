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
    // fit happens when the dialog is shown - once per open, so that a window
    // the user has resized in the meantime is left alone.
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

    // The tests read the widget state through the Ui the same way the main
    // window's tests do: friendship keeps it private instead of widening the
    // dialog's public surface for their sake.
    friend class TestSettingsDialog;

    // Applies one setting to the widget that shows it. Returns a note for the
    // user, empty when there is nothing to say.
    QString applyValue(const QString &key, const QJsonValue &value, ValueSource source);

    // Fills the rows that show where the settings and the session are stored.
    void updatePathLabels();

    // Says what is wrong with the hashcat path or the terminal selection.
    void updateStatusLabels();
    void updateHashcatStatus();
    void updateTerminalStatus();

    // The home directory as a tilde.
    static QString shortenPath(const QString &path);

    Ui::SettingsDialog *ui;

    // The language setting as it was when the dialog opened, so saveClicked()
    // can tell an actual change from "the user pressed Save".
    QString loadedLanguage;

    // The folders the two files are stored in, empty where there is none to open.
    QString m_settingsFolder;
    QString m_sessionFolder;

    // The terminal the last applied value named, so a terminal that is no
    // longer installed can be named instead of just "select one".
    QString m_configuredTerminal;

    // The dialog is fitted to its content once, on the first show.
    bool m_sizedToContent = false;
};

#endif // SETTINGSDIALOG_H
