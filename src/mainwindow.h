/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QComboBox>
#include <QIcon>
#include <QProcess>
#include <QShowEvent>

#include "hashcatoptions.h"
#include "helperutils.h"
#include "pathmonitor.h"

#include <QJsonObject>

namespace Ui
{
class MainWindow;
}

class QListWidget;

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
    void importSettingsTriggered();
    void exportSettingsTriggered();
    void quitTriggered();
    void loadExampleSessionTriggered();
    void resetSessionTriggered();
    void aboutQtTriggered();
    void aboutTriggered();

    // main-tab buttons
    void startClicked();
    void openHashFileClicked();
    void openOutfileClicked();
    void openCachePathClicked();
    void removeWordlistClicked();
    void addWordlistClicked();
    void addWordlistFolderClicked();
    void wordlistMoveUpClicked();
    void wordlistMoveDownClicked();
    void wordlistItemClicked(QListWidgetItem *item);
    void refreshWordlistMonitor();
    void wordlistAvailabilityChanged(const QString &path, bool available);

    // the PCFG, table and feed groups
    void addPcfgRulesetClicked();
    void addPcfgRulesetFolderClicked();
    void removePcfgRulesetClicked();
    void pcfgRulesetMoveUpClicked();
    void pcfgRulesetMoveDownClicked();
    void refreshPcfgMonitor();
    void pcfgAvailabilityChanged(const QString &path, bool available);
    void addTableClicked();
    void removeTableClicked();
    void refreshTableMonitor();
    void tableAvailabilityChanged(const QString &path, bool available);
    void addFeedClicked();
    void addFeedArgumentClicked();
    void removeFeedArgumentClicked();
    void feedArgumentMoveUpClicked();
    void feedArgumentMoveDownClicked();

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
    // The recipe row belongs to hash mode 4000 alone; the hash type combo
    // decides whether it exists.
    void updateHashTypeDependencies();
    void copyCommandToClipboard();

private:
    // The tests read the widget state through collectHashcatOptions() and
    // generateArguments(); making them a friend keeps those two private
    // instead of widening the window's public surface for the sake of tests.
    friend class TestMainWindow;

    Ui::MainWindow *ui;

    // Tell the rows of each list which of their paths hashcat can use.
    PathMonitor m_wordlistMonitor{PathMonitor::Kind::Wordlist};
    PathMonitor m_pcfgMonitor{PathMonitor::Kind::Pcfg};
    PathMonitor m_tableMonitor{PathMonitor::Kind::Table};

    // What a path row is decorated with: the type of the path while it is
    // usable, the warning otherwise. Built once, because a refresh compares
    // against them and does not want to rebuild the style's icons.
    QIcon m_fileIcon;
    QIcon m_folderIcon;
    QIcon m_warningIcon;

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

    // The notes an import has about the file itself: the version it was made
    // by and the sections it holds besides the one that was imported.
    QStringList fileNotes(int fileVersion, int currentVersion,
                          const QStringList &otherSections) const;

    // Why a file a newer version wrote is not applied at all.
    QString newerFileNote(int fileVersion, int currentVersion) const;

    // Why the session of a newer version is not restored - and not stored
    // either.
    QString newerSessionNote(int fileVersion) const;

    // Applies the values of a session file to the widgets and collects the
    // notes about what could not be applied.
    QStringList applySessionFile(const QJsonObject &values, int version,
                                 const QStringList &otherSections);

    // Reads the current widget state. Everything the command line is built
    // from goes through here, so CommandBuilder never has to know about
    // widgets.
    HashcatOptions collectHashcatOptions();
    QStringList generateArguments();

    // Row handling shared by every argument list. The wordlist slots are thin
    // wrappers around it: their buttons follow the selection, so they update
    // that too.
    //
    // checkedItems() is what reaches the command: an unchecked row stays in
    // the list - it keeps its place, its path and its text - but is left out.
    static QStringList checkedItems(const QListWidget *list);
    static void removeSelectedRows(QListWidget *list);
    static void moveSelectedRow(QListWidget *list, int delta);

    // The file and folder dialogs end here: the paths that are not rows yet,
    // once each, in the platform's own separator form. Every row starts
    // checked; unchecking it later keeps the row in the list but leaves it out
    // of the command.
    static void addPathsToList(QListWidget *list, const QStringList &paths);

    // Rebuilds a monitor's watched set from the rows of its list, and marks or
    // clears one row. The row write blocks the list's signals: an icon is not
    // a change to the command, so it must not rebuild the preview through
    // itemChanged().
    static void refreshMonitor(QListWidget *list, PathMonitor &monitor);
    void setRowAvailability(QListWidget *list, PathMonitor::Kind kind, const QString &path,
                            bool available);
    static QString unavailableTooltip(PathMonitor::Kind kind, const QString &path);

    // Presents what HelperUtils::validateLaunch() reported
    void showLaunchError(HelperUtils::LaunchError error, const QString &detail);

    QString sessionFile() const;

    // Whether this version understands the session file: one it cannot read
    // at all, or one of a newer format, belongs to another version.
    bool sessionFileIsUnderstood(const QString &file) const;

    // Whether the application keeps the session between two runs.
    bool restoreSessionEnabled() const;

    void loadSession();
    void saveSession();

    // Queues the hash type a session names until the list of hash types
    // exists, and reports one this hashcat does not know - which leaves the
    // combo without a selection.
    QString restoreHashType(const QJsonObject &state);

    // Applies the hash type a session asked for, now that the list of hash
    // types exists. Returns a message when the session names a hash type
    // this hashcat does not know.
    QString applyPendingHashType();

    // Shows the mode hint and the groups the mode uses, and hides the rest.
    // Shared by the normal refresh and the envelope measurement, which walks
    // every mode before the window is sized.
    void applyAttackModeView(AttackMode mode);

    // The largest minimum the attack tab can need: every mode's groups and
    // hint in one size. Measured before the visible mode is restored. A
    // wrapping widget announces one line in its minimum size hint, so
    // wrapWidth - the content width it wraps at - adds the height it really
    // needs; zero measures without that correction.
    QSize attackModeEnvelope(int wrapWidth);

    // Gives the window the size its tabs need once, on the first show. The
    // attack tab contributes the envelope of all its modes, so changing modes
    // cannot introduce a scrollbar. See showEvent().
    void fitToContent();

    // The fit in showEvent() runs a single time; a later show (hide/restore)
    // must not move or resize a window the user has placed themselves.
    bool m_sizedToContent = false;

    // A session can name a hash type while comboBox_hash_type still has no
    // items - the list arrives with the asynchronous hashcat query, after the
    // constructor has already loaded the session. The stored id waits
    // here and is applied once the items exist, see initHashAndAttackModes().
    int m_pendingHashTypeId = -1;

    // The binary the current hash-type list came from. Settings re-runs the
    // query when the configured path no longer matches, so a changed binary
    // replaces the list instead of leaving the old one on screen.
    QString m_queriedHashcatPath;
};

#endif // MAINWINDOW_H
