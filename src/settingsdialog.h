/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

private slots:
    void readSettings();
    void selectPathClicked();
    void saveClicked();
    void cancelClicked();
private:
    Ui::SettingsDialog *ui;

    // The language setting as it was when the dialog opened, so saveClicked()
    // can tell an actual change from "the user pressed Save".
    QString loadedLanguage;
};

#endif // SETTINGSDIALOG_H
