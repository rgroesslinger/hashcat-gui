/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "attackmodeview.h"
#include "hashcatoptions.h"
#include "hashcatrunner.h"
#include "hashtypeloader.h"
#include "launchvalidator.h"
#include "pathlistcontroller.h"
#include "statefile.h"

#include <QAbstractButton>
#include <QComboBox>
#include <QJsonObject>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QShowEvent>
#include <memory>
#include <optional>

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

    // Restores the session once; call between construction and show().
    void restoreSession();

    MainWindow(const MainWindow &) = delete;
    MainWindow &operator=(const MainWindow &) = delete;
    MainWindow(MainWindow &&) = delete;
    MainWindow &operator=(MainWindow &&) = delete;

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
    void loadExampleSession(AttackMode mode);
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

    // the PCFG, table and feed groups
    void addPcfgRulesetClicked();
    void addPcfgRulesetFolderClicked();
    void addPcfgRulesetNameClicked();
    void removePcfgRulesetClicked();
    void pcfgRulesetMoveUpClicked();
    void pcfgRulesetMoveDownClicked();
    void addTableClicked();
    void removeTableClicked();
    void addFeedClicked();
    void addFeedArgumentClicked();
    void removeFeedArgumentClicked();
    void feedArgumentMoveUpClicked();
    void feedArgumentMoveDownClicked();

    // checkboxes / radio buttons
    // The cross-state half of the rules choice, on the radio pair.
    void updateRulesDependencies();

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
    // The tests read the widget state through MainWindowTestAccess, the one
    // test seam; it keeps the private surface private instead of widening the
    // window's public API for the sake of tests.
    friend struct MainWindowTestAccess;

    Ui::MainWindow *ui;

    // The attack mode's face on the form; constructed with the ui, applied
    // on every mode change and once per mode by the envelope measurement.
    AttackModeView m_attackModeView;

    // One controller per argument list: the rows, their checked state and
    // the monitor decoration. Destroyed before delete ui, see the destructor.
    std::unique_ptr<PathListController> m_wordlistList;
    std::unique_ptr<PathListController> m_pcfgRulesetList;
    std::unique_ptr<PathListController> m_tableList;
    std::unique_ptr<PathListController> m_feedArgumentList;

    // The last outfile name generated from the hash file name, see
    // hashFileTextChanged(). It is not cleared when the user types a name of
    // their own: the field is only overwritten again while it still holds
    // this suggestion.
    QString m_suggestedOutfile;

    // Fills the attack combo once, in the constructor; a settings change
    // must not refill it and reset the selected mode.
    // The constructor's halves: widget setup and the four connection groups.
    void configureWidgets();
    void connectCommandSignals();
    void connectActionSignals();
    void connectListSignals();
    void connectDependencySignals();

    // One checkbox enables (or disables) its targets, applied at bind time.
    void bindEnabled(QAbstractButton *toggle, const QList<QWidget *> &targets);
    void bindDisabled(QAbstractButton *toggle, const QList<QWidget *> &targets);

    void initAttackModes();
    // Starts the hash-type query for the configured binary; never touches
    // the attack combo.
    void reloadHashTypes();
    // The shared refresh after the settings were saved or imported.
    void refreshAfterSettingsChange();
    // The example submenu is built once: initHashAndAttackModes() runs again
    // on settings changes, and adding the actions there would duplicate them.
    void initExampleSessionMenu();
    void updateViewAttackMode();

    // The attack mode the user selected.
    [[nodiscard]] AttackMode currentAttackMode() const;

    // Applies the values of a session file to the widgets and collects the
    // notes about what could not be applied.
    QStringList applySessionFile(const StateFile::Document &document);

    // Reads the current widget state. Everything the command line is built
    // from goes through here, so CommandBuilder never has to know about
    // widgets.
    HashcatOptions collectHashcatOptions();
    QStringList generateArguments();

    // Presents what LaunchValidator::validate() reported
    void showLaunchError(const LaunchValidator::LaunchProblem &problem,
                         const LaunchValidator::LaunchContext &context);

    [[nodiscard]] QString sessionFile() const;

    // Whether this version understands the session file: one it cannot read
    // at all, or one of a newer format, belongs to another version.
    [[nodiscard]] bool sessionFileIsUnderstood(const QString &file) const;

    // Whether the application keeps the session between two runs.
    [[nodiscard]] bool restoreSessionEnabled() const;

    void loadSession();
    void saveSession();

    // Queues the hash type a session names until the list of hash types
    // exists, and reports one this hashcat does not know - which leaves the
    // combo without a selection.
    [[nodiscard]] QString restoreHashType(const QJsonObject &values);

    // Applies the hash type a session asked for, now that the list of hash
    // types exists. Returns a message when the session names a hash type
    // this hashcat does not know.
    QString applyPendingHashType();

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

    // Bulk writes (session apply, reset, combo refill) silence the preview
    // while they run: one rebuild at the end instead of one per widget. A
    // depth counter, not a bool, keeps nested guards reentrant; the RAII guard
    // clears the depth on every early return.
    class CommandSilence
    {
    public:
        explicit CommandSilence(MainWindow &window) : m_window(window)
        {
            ++m_window.m_commandSilenceDepth;
        }
        ~CommandSilence()
        {
            --m_window.m_commandSilenceDepth;
        }
        CommandSilence(const CommandSilence &) = delete;
        CommandSilence &operator=(const CommandSilence &) = delete;
        CommandSilence(CommandSilence &&) = delete;
        CommandSilence &operator=(CommandSilence &&) = delete;

    private:
        MainWindow &m_window;
    };

    // The widget defaults, captured once in the constructor after
    // initAttackModes() and before the startup restore; the reset applies
    // them. Constructor only - a later capture would adopt the user's form.
    std::optional<QJsonObject> m_defaultSnapshot;

    // Whether the restore pass ran - not whether a file was applied.
    bool m_sessionRestored = false;

    int m_commandSilenceDepth = 0;
    // The rebuilds commandChanged() actually performed; the tests pin the
    // bulk writes to exactly one.
    int m_previewRebuilds = 0;

    // A session can name a hash type while comboBox_hash_type still has no
    // items - the list arrives with the asynchronous hashcat query, after the
    // constructor has already loaded the session. The stored id waits
    // here and is applied once the items exist, see initHashAndAttackModes().
    int m_pendingHashTypeId = -1;

    // The asynchronous hash-type query: the watcher, the stale-query guard
    // and the parsed list. The window maps its signals onto the combo.
    HashTypeLoader m_hashTypeLoader;
};

#endif // MAINWINDOW_H
