/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QComboBox>
#include <QProcess>

#include "hashcatoptions.h"
#include "helperutils.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // menu actions
    void settingsTriggered();
    void importTriggered();
    void exportTriggered();
    void quitTriggered();
    void resetFieldsTriggered();
    void aboutQtTriggered();
    void aboutTriggered();

    // main‑tab buttons
    void executeClicked();
    void openHashFileClicked();
    void outputClicked();
    void removeWordlistClicked();
    void addWordlistClicked();
    void wordlistSortAscClicked();
    void wordlistSortDescClicked();
    void wordlistItemClicked(QListWidgetItem *item);

    // checkboxes / radio buttons
    void outfileToggled(bool checked);
    void rulesfile1Toggled(bool checked);
    void rulesfile2Toggled(bool checked);
    void rulesfile3Toggled(bool checked);
    void generateRulesToggled(bool checked);
    void useRulesFileToggled(bool checked);
    void customCharset1Toggled(bool checked);
    void customCharset2Toggled(bool checked);
    void customCharset3Toggled(bool checked);
    void customCharset4Toggled(bool checked);

    // line edits
    void hashFileTextChanged(const QString &text);

    // rule‑file buttons
    void openRulesFile1Clicked();
    void openRulesFile2Clicked();
    void openRulesFile3Clicked();

    // combobox
    void attackIndexChanged(int index);

    void commandChanged();
    void copyCommandToClipboard();

private:
    // The tests read the widget state through collectHashcatOptions() and
    // generateArguments(); making them a friend keeps those two private
    // instead of widening the window's public surface for the sake of tests.
    friend class TestMainWindow;

    Ui::MainWindow *ui;

    // Last outfile name generated from the hash file name, see
    // hashFileTextChanged(). Empty when the field holds a user-chosen value.
    QString suggestedOutfile;

    QMap<quint32, QString> hashModes;
    QMap<AttackMode, QString> attackModes;

    void initHashAndAttackModes();
    void updateViewAttackMode();

    // Reads the current widget state. Everything the command line is built
    // from goes through here, so CommandBuilder never has to know about
    // widgets.
    HashcatOptions collectHashcatOptions();
    QStringList generateArguments();

    // Presents what HelperUtils::validateLaunch() reported
    void showLaunchError(HelperUtils::LaunchError error, const QString &detail);

    QString defaultProfileFile() const;
    void loadDefaultProfile();
    void saveDefaultProfile();

    // A profile can name a hash type while comboBox_hash still has no items
    // - the list arrives with the asynchronous hashcat query, after the
    // constructor has already loaded the profile. The stored index waits
    // here and is applied once the items exist, see initHashAndAttackModes().
    int m_pendingHashTypeIndex = -1;
};

#endif // MAINWINDOW_H
