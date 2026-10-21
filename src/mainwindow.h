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
#include <QShowEvent>

#include "hashcatoptions.h"
#include "helperutils.h"

#include <QJsonObject>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    // The window cannot know its natural size before it is shown: the scroll
    // areas inside the tabs only report the content's final minimum once they
    // are polished. The one-shot fit to the content therefore happens here.
    void showEvent(QShowEvent *event) override;

private slots:
    // menu actions
    void settingsTriggered();
    void importTriggered();
    void exportTriggered();
    void quitTriggered();
    void resetFieldsTriggered();
    void aboutQtTriggered();
    void aboutTriggered();

    // main-tab buttons
    void startClicked();
    void openHashFileClicked();
    void openOutfileClicked();
    void removeWordlistClicked();
    void addWordlistClicked();
    void wordlistSortAscClicked();
    void wordlistSortDescClicked();
    void wordlistItemClicked(QListWidgetItem *item);

    // checkboxes / radio buttons
    void outfileToggled(bool checked);
    void rulesFile1Toggled(bool checked);
    void rulesFile2Toggled(bool checked);
    void rulesFile3Toggled(bool checked);
    void generateRulesToggled(bool checked);
    void useRulesFileToggled(bool checked);
    void customCharset1Toggled(bool checked);
    void customCharset2Toggled(bool checked);
    void customCharset3Toggled(bool checked);
    void customCharset4Toggled(bool checked);

    // line edits
    void hashFileTextChanged(const QString &text);

    // rule-file buttons
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

    // The last outfile name generated from the hash file name, see
    // hashFileTextChanged(). It is not cleared when the user types a name of
    // their own: the field is only overwritten again while it still holds
    // this suggestion.
    QString m_suggestedOutfile;

    void initHashAndAttackModes();
    void updateViewAttackMode();
    void updateOptionDependencies();

    // The attack mode the user selected.
    AttackMode currentAttackMode() const;

    // What the selected attack mode needs from the user.
    QString modeHint(AttackMode mode) const;

    // Shows what a loaded profile has to say for itself. Returns whether a
    // note was shown.
    bool showProfileNotes(const QString &warning, const QString &problem);

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

    // Restores the hash type a profile names. Returns a message when the
    // profile names a hash type this hashcat does not know.
    QString restoreHashType(const QJsonObject &state);

    // Applies the hash type a profile asked for, now that the list of hash
    // types exists. Returns a message when the profile names a hash type
    // this hashcat does not know.
    QString applyPendingHashType();

    // Gives the window the size its current tab actually needs once, on the
    // first show. See showEvent().
    void fitToContent();

    // The fit in showEvent() runs a single time; a later show (hide/restore)
    // must not move or resize a window the user has placed themselves.
    bool m_sizedToContent = false;

    // A profile can name a hash type while comboBox_hash_type still has no
    // items - the list arrives with the asynchronous hashcat query, after the
    // constructor has already loaded the profile. The stored id waits
    // here and is applied once the items exist, see initHashAndAttackModes().
    int m_pendingHashTypeId = -1;
};

#endif // MAINWINDOW_H
