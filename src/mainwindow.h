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
#include "potfilecheck.h"
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
struct LaunchMessage;

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
    void identifyHashTriggered();
    void backendInfoTriggered();
    void potfileCheckTriggered();
    void potfileInspectorTriggered();

    // main-tab buttons
    void startClicked();
    void openHashFileClicked();
    void openOutfileClicked();
    void openPotfilePathClicked();
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
    void tableMoveUpClicked();
    void tableMoveDownClicked();
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
    // Copies the unencoded PowerShell script the launch would encode: the
    // same ProcessStartInfo text, for inspection or reuse outside the GUI.
    void copyPowerShellScript();

private:
    // The tests read the widget state through MainWindowTestAccess, the one
    // test seam; it keeps the private surface private instead of widening the
    // window's public API for the sake of tests.
    friend struct MainWindowTestAccess;

    Ui::MainWindow *ui;

    // The attack mode's face on the form; constructed with the ui, applied
    // on every mode change and once per mode by the envelope measurement.
    AttackModeView m_attackModeView;

    // Destroyed before delete ui; see PathListController.
    std::unique_ptr<PathListController> m_wordlistList;
    std::unique_ptr<PathListController> m_pcfgRulesetList;
    std::unique_ptr<PathListController> m_tableList;
    std::unique_ptr<PathListController> m_feedArgumentList;

    // The last outfile name generated from the hash file name, see
    // hashFileTextChanged(). It is not cleared when the user types a name of
    // their own: the field is only overwritten again while it still holds
    // this suggestion.
    QString m_suggestedOutfile;

    // The constructor's halves: widget setup and the four connection groups.
    void configureWidgets();
    void connectCommandSignals();
    void connectActionSignals();
    void connectListSignals();
    void connectDependencySignals();

    // One checkbox enables (or disables) its targets, applied at bind time.
    void bindEnabled(QAbstractButton *toggle, const QList<QWidget *> &targets);
    void bindDisabled(QAbstractButton *toggle, const QList<QWidget *> &targets);

    // Fills the attack combo once, in the constructor; a settings change
    // must not refill it and reset the selected mode.
    void initAttackModes();
    // Starts the hash-type query for the configured binary; never touches
    // the attack combo.
    void reloadHashTypes();
    // The permanent first entry: a GUI-only sentinel. A negative id omits -m,
    // hashcat's only autodetect trigger.
    void addAutoDetectHashType();
    // Drops the current list, leaving only Auto-detect and no path or retry pending.
    void resetHashTypeCombo();
    // The shared refresh after the settings were saved or imported.
    void refreshAfterSettingsChange();
    // The example submenu is built once, in the constructor; adding the
    // actions on every settings refresh would duplicate them.
    void initExampleSessionMenu();
    void updateViewAttackMode();

    [[nodiscard]] AttackMode currentAttackMode() const;

    // Applies the values of a session file to the widgets and collects the
    // notes about what could not be applied.
    QStringList applySessionFile(const StateFile::Document &document);

    // Reads the current widget state. Everything the command line is built
    // from goes through here, so CommandBuilder never has to know about
    // widgets.
    HashcatOptions collectHashcatOptions();
    QStringList generateArguments();

    // Validates the potfile form state and resolves the input and program.
    // Shows the user-facing error itself and returns false on refusal.
    bool preparePotfileQuery(const QString &title, PotfileQueryInput &input, QString &modeName,
                             QString &hashcatPath);

    // The Identify dialog's apply handler; false keeps the dialog open.
    bool applyIdentifiedHashType(int hashTypeId, const std::optional<QString> &input);

    // Presents what LaunchValidator::validate() reported
    void showLaunchError(const LaunchValidator::LaunchProblem &problem,
                         const LaunchValidator::LaunchContext &context);
    void showLaunchMessage(const LaunchMessage &message);
    // The menu names as shown, for tooltips and messages that point to Settings.
    [[nodiscard]] QString fileMenuLabel() const;
    [[nodiscard]] QString settingsMenuLabel() const;

    [[nodiscard]] QString sessionFile() const;

    // Whether this version understands the session file: one it cannot read
    // at all, or one of a newer format, belongs to another version.
    [[nodiscard]] bool sessionFileIsUnderstood(const QString &file) const;

    // Whether the application keeps the session between two runs.
    [[nodiscard]] bool restoreSessionEnabled() const;

    void loadSession();
    void saveSession();

    // The session as the widgets hold it, with the hash type a pending query
    // still owes corrected to the id the session named, see the definition.
    QJsonObject sessionValues();

    // Whether a restored session named a hash type the list has not been able
    // to apply yet, see m_pendingHashTypeId.
    [[nodiscard]] bool hasPendingHashType() const;

    // Queues the hash type a session names until the list of hash types
    // exists, and reports one this hashcat does not know - which falls back to
    // Auto-detect rather than picking a different real hash type.
    [[nodiscard]] QString restoreHashType(const QJsonObject &values);

    // Applies the hash type a session asked for, or the given fallback when
    // the session named none, now that the list exists. Returns a message when
    // a real id this hashcat does not know had to be replaced by Auto-detect.
    QString applyPendingHashType(int fallbackId, bool fromSession);

    // The largest minimum the attack tab needs across all modes; see the definition for wrapWidth.
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
    // here and is applied once the items exist, see applyPendingHashType().
    int m_pendingHashTypeId = -1;

    // The asynchronous hash-type query: the watcher, the stale-query guard
    // and the parsed list. The window maps its signals onto the combo.
    HashTypeLoader m_hashTypeLoader;

    // The read-only potfile query.
    std::unique_ptr<PotfileCheck> m_potfileCheck;

    // The binary whose list the combo currently shows. Used to tell a
    // same-binary refresh (keep the list) from a changed binary (clear it).
    QString m_hashTypesPath;

    // The combo's explanatory tooltip in its normal state. The loading and
    // error paths override it temporarily; a successful load restores it.
    QString m_hashTypeBaseToolTip;

    // A same-binary unparseable reply kept the old list; accepting Settings
    // again with that path must re-run the query, there is no other retry.
    bool m_hashTypesNeedsRetry = false;
};

#endif // MAINWINDOW_H
