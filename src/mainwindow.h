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
    Ui::MainWindow *ui;

    QMap<quint32, QString> hashModes;
    QMap<quint32, QString> attackModes;

    void initHashAndAttackModes();
    void updateViewAttackMode();

    QStringList generateArguments();

    void addWordlistItem(QStringList &);
    void setOutfilePath();

    QString defaultProfileFile() const;
    void loadDefaultProfile();
    void saveDefaultProfile();

    // Supported attack modes
    enum AttackMode
    {
        Straight       = 0,
        Combination    = 1,
        BruteForce     = 3,
        HybridWordMask = 6,
        HybridMaskWord = 7,
        Association    = 9
    };
};

#endif // MAINWINDOW_H
