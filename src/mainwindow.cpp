/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "mainwindow.h"

#include "aboutdialog.h"
#include "appconstants.h"
#include "attackmodeview.h"
#include "checkeditemlist.h"
#include "commandbuilder.h"
#include "commandpreview.h"
#include "examples.h"
#include "examplesession.h"
#include "executable.h"
#include "hashtypeloader.h"
#include "launchmessage.h"
#include "launchvalidator.h"
#include "optioncollector.h"
#include "pathlistcontroller.h"
#include "pathmonitor.h"
#include "sessiontransfer.h"
#include "settingsdialog.h"
#include "settingsmanager.h"
#include "statefile.h"
#include "statewidgets.h"
#include "terminalcatalog.h"
#include "ui_mainwindow.h"
#include "widgetstateserializer.h"

#include <QAbstractButton>
#include <QAction>
#include <QClipboard>
#include <QCompleter>
#include <QCursor>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QIcon>
#include <QIntValidator>
#include <QLayout>
#include <QMap>
#include <QMenu>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QStandardPaths>
#include <QStyle>
#include <QToolTip>

#include <array>
#include <utility>

namespace
{
// A hidden tab page keeps stale layout minima until its layouts have run at
// least once. Walk the page bottom-up so every nested group box and layout
// resolves its minimum before the page layout sums them up.
void activateLayouts(QWidget *widget)
{
    widget->ensurePolished();
    const QList<QWidget *> children =
        widget->findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly);
    for (QWidget *child : children) {
        activateLayouts(child);
    }
    if (QLayout *layout = widget->layout()) {
        layout->invalidate();
        layout->activate();
    }
}

// A menu path names the entry, not the action: the ellipsis of an entry that
// opens a dialog is not part of "File → Settings", and neither is the
// mnemonic marker (the titles read "&File" so Alt+F can reach them).
QString menuPath(const QString &text)
{
    QString path = text;
    if (path.endsWith(QStringLiteral("…"))) {
        path.chop(1);
    }
    return path.remove(QLatin1Char('&')).trimmed();
}

// Where a file dialog starts: a folder the user knows. A bare file name
// would open the dialog in the application's working directory, wherever it
// happened to be started from.
QString suggestedFolder()
{
    QString folder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if (!QFileInfo(folder).isDir()) {
        folder = QDir::homePath();
    }
    return folder;
}

// Where a file dialog for an export starts: that folder, with the name the
// application suggests.
QString suggestedExportPath(QLatin1StringView fileName)
{
    return QDir(suggestedFolder()).filePath(QString(fileName));
}

} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_attackModeView(*ui), m_hashTypeLoader(this)
{
    ui->setupUi(this);

    configureWidgets();

    auto &settings = SettingsManager::instance();

    // The loader's replies fill the hash combo. The connections come before
    // the first reload, so the loading state is not missed.
    connect(&m_hashTypeLoader, &HashTypeLoader::loadingChanged, this, [this](bool loading) {
        ui->comboBox_hash_type->setEnabled(!loading);
        if (loading) {
            ui->comboBox_hash_type->setToolTip(tr("Updating…"));
        }
    });
    connect(&m_hashTypeLoader, &HashTypeLoader::loaded, this,
            [this](const QMap<quint32, QString> &hashTypes) {
                // The refill is a bulk write: one preview rebuild at the end,
                // not one per combo event.
                QString problem;
                {
                    const CommandSilence silence(*this);
                    ui->comboBox_hash_type->clear();
                    // fill the combobox, hash type id as item data
                    for (auto it = hashTypes.constBegin(); it != hashTypes.constEnd(); ++it) {
                        ui->comboBox_hash_type->addItem(it.value(), it.key());
                    }
                    ui->comboBox_hash_type->setToolTip(QString());

                    // A session loaded while the combo was still empty could
                    // not select its hash type; apply the stored id now that
                    // the items exist, and consume it.
                    problem = applyPendingHashType();
                }
                commandChanged();
                if (!problem.isEmpty()) {
                    QMessageBox::warning(this, tr("Session warning"), problem);
                }
            });
    connect(&m_hashTypeLoader, &HashTypeLoader::failed, this,
            [this](const HashTypeFailure &failure) {
                // The previous binary's list must not survive a failed reload:
                // a stale id would reach the preview and the launch validation,
                // and the empty combo is what lets a same-path retry re-query
                // once the binary is repaired.
                ui->comboBox_hash_type->clear();
                // The combo was disabled while the query ran. Restore its
                // tooltip on every failure path so it does not stay stuck on
                // "Updating…" forever.
                ui->comboBox_hash_type->setToolTip(
                    tr("The hash types could not be loaded. Check the hashcat path in Settings."));
                // Exit codes changed in hashcat 8.0.0, but this only wants
                // "did it work"; the GUI has no per-cause message, and a
                // reader must not turn this into per-code handling.
                QString error =
                    tr("Failed to obtain supported hash types.\nError: %1").arg(failure.detail);
#ifdef Q_OS_WIN
                if (failure.kind == HashTypeFailure::Kind::Timeout) {
                    // The FAQ names Windows Defender as the usual cause of a
                    // query that runs into its deadline; on other platforms a
                    // timeout has no such explanation to offer.
                    error += tr("\n\nThis can be caused by Windows Defender or other "
                                "antivirus software. See the <a href=\"%1\">FAQ entry</a> "
                                "for a remedy.")
                                 .arg(QString(AppConstants::Urls::FaqHashTypesTimeout));
                }
#endif
                QMessageBox::warning(this, tr("Failed to load hash types"), error);
            });

    initAttackModes();
    // The default snapshot, captured exactly once - constructor only. The
    // attack combo already carries its item data (or the snapshot would
    // store a position), and no session has been restored yet. A capture in
    // a settings refresh would replace the defaults with the user's form.
    m_defaultSnapshot = SessionTransfer::toDocument(this);
    initExampleSessionMenu();
    updateViewAttackMode();
    // The hash type list arrives asynchronously, but the recipe row can only
    // be hidden for the empty combo; every later change comes through the
    // connection set up below.
    updateHashTypeDependencies();
    reloadHashTypes();

    /* ---------- save the session on quit ---------- */
    connect(qApp, &QCoreApplication::aboutToQuit, this, &MainWindow::saveSession);

    connectCommandSignals();
    connectActionSignals();
    connectListSignals();
    connectDependencySignals();

    /* ---------- show Settings if hashcatPath not set ---------- */
    if (settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath).isEmpty()) {
        QMetaObject::invokeMethod(this, &MainWindow::settingsTriggered, Qt::QueuedConnection);
    }
}

// The widgets' own setup: the completer, the list controllers, the input
// validators and the help tooltips. No command wiring here.
void MainWindow::configureWidgets()
{
    // The hash type list is searchable through a completer. It must be editable
    // before the completer is set - a completer on a non-editable combo is
    // refused - and NoInsert keeps a name the completer does not match from
    // becoming the selection, so the current id is always a real one.
    ui->comboBox_hash_type->setEditable(true);
    ui->comboBox_hash_type->setInsertPolicy(QComboBox::NoInsert);
    QCompleter *hashTypeCompleter = ui->comboBox_hash_type->completer();
    hashTypeCompleter->setCompletionMode(QCompleter::PopupCompletion);
    hashTypeCompleter->setFilterMode(Qt::MatchContains);
    hashTypeCompleter->setCaseSensitivity(Qt::CaseInsensitive);

    // The path rows say what they are at a glance: a file, a folder, or the
    // warning of a path hashcat cannot use. The style owns the pixmaps, and
    // the rows are compared by icon key, so these are asked for once.
    const QIcon fileIcon = style()->standardIcon(QStyle::SP_FileIcon);
    const QIcon folderIcon = style()->standardIcon(QStyle::SP_DirIcon);
    const QIcon warningIcon = style()->standardIcon(QStyle::SP_MessageBoxWarning);

    // One controller per argument list: the rows, their checked state and
    // the monitor decoration. They are destroyed before delete ui, see the
    // destructor.
    m_wordlistList =
        std::make_unique<PathListController>(ui->listWidget_wordlist, PathMonitor::Kind::Wordlist,
                                             fileIcon, folderIcon, warningIcon, nullptr);
    m_pcfgRulesetList =
        std::make_unique<PathListController>(ui->listWidget_pcfg_rulesets, PathMonitor::Kind::Pcfg,
                                             fileIcon, folderIcon, warningIcon, nullptr);
    m_tableList =
        std::make_unique<PathListController>(ui->listWidget_tables, PathMonitor::Kind::Table,
                                             fileIcon, folderIcon, warningIcon, nullptr);
    m_feedArgumentList =
        std::make_unique<PathListController>(ui->listWidget_feed_arguments, nullptr);

    // -s and -l take a plain number; a digit-only validator keeps a value
    // hashcat would refuse to start from ever reaching the command line.
    const QRegularExpression digits{QString(AppConstants::Validation::Digits)};
    ui->lineEdit_skip->setValidator(new QRegularExpressionValidator(digits, ui->lineEdit_skip));
    ui->lineEdit_limit->setValidator(new QRegularExpressionValidator(digits, ui->lineEdit_limit));

    // -D 3 (the accelerators) is gone with hashcat 8.0.0. The field was free
    // text, so a 3 typed here only failed after the launch, in the terminal;
    // the validator keeps the accepted device types where the user sees it.
    const QRegularExpression deviceTypes{QString(AppConstants::Validation::DeviceTypes)};
    ui->lineEdit_opencl_device_types->setValidator(
        new QRegularExpressionValidator(deviceTypes, ui->lineEdit_opencl_device_types));

    // The settings of the PCFG and table feeds, which hashcat parses itself
    // - and only after it has set up the backend devices, long after the
    // preview is gone. The ranges a spin box would hold stay line edits
    // (the feed also takes its defaults, which no spin box can show), so
    // they get validators of their own.
    ui->lineEdit_pcfg_scale->setValidator(new QIntValidator(1, 64, ui->lineEdit_pcfg_scale));
    ui->lineEdit_pcfg_costmax->setValidator(new QIntValidator(1, 64, ui->lineEdit_pcfg_costmax));
    ui->lineEdit_pcfg_threads->setValidator(new QIntValidator(0, 16, ui->lineEdit_pcfg_threads));
    ui->lineEdit_pcfg_kbits->setValidator(new QIntValidator(0, 27, ui->lineEdit_pcfg_kbits));
    ui->lineEdit_pcfg_walk->setValidator(new QIntValidator(0, 1, ui->lineEdit_pcfg_walk));

    // The shapes that are not a plain integer range: the colon separated
    // weights list, maxword's multiple of 16, the dot separated decimal and
    // the 0/1 flags of the table feed.
    const auto feedShapes = std::to_array<std::pair<QLineEdit *, QLatin1StringView>>({
        {ui->lineEdit_pcfg_weights, AppConstants::Validation::WeightsList},
        {ui->lineEdit_pcfg_maxword, AppConstants::Validation::MaxwordValue},
        {ui->lineEdit_pcfg_maxgain, AppConstants::Validation::DecimalNumber},
        {ui->lineEdit_table_identity, AppConstants::Validation::BinaryFlag},
        {ui->lineEdit_table_template, AppConstants::Validation::BinaryFlag},
        {ui->lineEdit_table_single, AppConstants::Validation::BinaryFlag},
    });
    for (const auto &[edit, shape] : feedShapes) {
        edit->setValidator(
            new QRegularExpressionValidator(QRegularExpression(QString(shape)), edit));
    }
    // cap and maxperm reach beyond every Qt integer type (0…0xffffffff and a
    // full u64); the digits are filtered here, the bounds in validateOptions.
    ui->lineEdit_table_cap->setValidator(
        new QRegularExpressionValidator(digits, ui->lineEdit_table_cap));
    ui->lineEdit_table_maxperm->setValidator(
        new QRegularExpressionValidator(digits, ui->lineEdit_table_maxperm));

    /* ---------- help tooltips: translatable label + fixed reference tables -- */
    const QString charsetTip =
        tr("Use ?1–?4 in a mask to insert a custom charset. A mask is used by "
           "Brute-force, the hybrid modes and PCFG’s mask= setting; a charset that no "
           "mask names is refused.")
        + QLatin1String("\n\n") + tr("Built-in charsets:") + QLatin1String("\n\n")
        + QString::fromUtf8(AppConstants::HelpText::BuiltinCharsets);
    ui->lineEdit_mask->setToolTip(tr("Password candidate mask. ?1–?4 insert a custom charset, "
                                     "?w inserts the wordlist word and ?q a second wordlist word "
                                     "in the Hybrid (mask + ?w/?q) mode, and ?? is a literal ?.")
                                  + QLatin1String("\n\n") + charsetTip);
    ui->lineEdit_custom_charset1->setToolTip(charsetTip);
    ui->lineEdit_custom_charset2->setToolTip(charsetTip);
    ui->lineEdit_custom_charset3->setToolTip(charsetTip);
    ui->lineEdit_custom_charset4->setToolTip(charsetTip);

    const QString outfileFormatTip = tr("Outfile format to use, separated by commas.\nDefault: 1,2")
                                     + QLatin1String("\n\n")
                                     + QString::fromUtf8(AppConstants::HelpText::OutfileFormats);
    ui->lineEdit_outfile_format->setToolTip(outfileFormatTip);

    const QString encodingTip = tr("Common encodings:") + QLatin1String("\n\n")
                                + QString::fromUtf8(AppConstants::HelpText::CommonEncodings);
    ui->lineEdit_encoding_from->setToolTip(tr("Force the internal wordlist encoding from this "
                                              "code. Leave empty to let hashcat decide.")
                                           + QLatin1String("\n\n") + encodingTip);
    ui->lineEdit_encoding_to->setToolTip(
        tr("Force the internal wordlist encoding to this code. Leave empty to let hashcat decide.")
        + QLatin1String("\n\n") + encodingTip);

    const QString deviceTypeTip = tr("OpenCL device types to use, separated by commas.")
                                  + QLatin1String("\n\n")
                                  + QString::fromUtf8(AppConstants::HelpText::OpenClDeviceTypes);
    ui->lineEdit_opencl_device_types->setToolTip(deviceTypeTip);
}

// Every widget whose value reaches the command line is connected here
// exactly once, from the same walk the serializer uses. Controls that also
// enable fields, open dialogs or change the gating get their own connection
// elsewhere; that second connection is the side effect, not a second preview
// update. The attack mode combo is the one exception: it refreshes the
// preview through attackIndexChanged().
void MainWindow::connectCommandSignals()
{
    const StateWidgetLists widgets = collectStateWidgets(this);

    for (QLineEdit *edit : widgets.lineEdits) {
        // The preview is generated, not entered; the two staging fields
        // reach the command only through their Add buttons.
        if (edit == ui->lineEdit_command || edit == ui->lineEdit_feed_argument
            || edit == ui->lineEdit_pcfg_ruleset_name) {
            continue;
        }
        connect(edit, &QLineEdit::textChanged, this, &MainWindow::commandChanged);
    }
    for (QCheckBox *checkBox : widgets.checkBoxes) {
        connect(checkBox, &QAbstractButton::toggled, this, &MainWindow::commandChanged);
    }
    for (QRadioButton *radioButton : widgets.radioButtons) {
        connect(radioButton, &QAbstractButton::toggled, this, &MainWindow::commandChanged);
    }
    for (QSpinBox *spinBox : widgets.spinBoxes) {
        connect(spinBox, &QSpinBox::valueChanged, this, &MainWindow::commandChanged);
    }
    for (QDoubleSpinBox *spinBox : widgets.doubleSpinBoxes) {
        connect(spinBox, &QDoubleSpinBox::valueChanged, this, &MainWindow::commandChanged);
    }
    for (QComboBox *comboBox : widgets.comboBoxes) {
        if (comboBox == ui->comboBox_attack) {
            continue;
        }
        connect(comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::commandChanged);
    }

    // The hash file also drives the outfile suggestion.
    connect(ui->lineEdit_hashfile, &QLineEdit::textChanged, this, &MainWindow::hashFileTextChanged);
    // The recipe row belongs to hash mode 4000 alone; the combo decides.
    connect(ui->comboBox_hash_type, &QComboBox::currentIndexChanged, this,
            &MainWindow::updateHashTypeDependencies);
}

void MainWindow::connectActionSignals()
{
    /* ---------- menu actions ---------- */
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::aboutTriggered);
    connect(ui->actionResetSession, &QAction::triggered, this, &MainWindow::resetSessionTriggered);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quitTriggered);
    connect(ui->actionExportSession, &QAction::triggered, this, &MainWindow::exportTriggered);
    connect(ui->actionImportSession, &QAction::triggered, this, &MainWindow::importTriggered);
    connect(ui->actionImportSettings, &QAction::triggered, this,
            &MainWindow::importSettingsTriggered);
    connect(ui->actionExportSettings, &QAction::triggered, this,
            &MainWindow::exportSettingsTriggered);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::settingsTriggered);
    connect(ui->actionAboutQt, &QAction::triggered, this, &MainWindow::aboutQtTriggered);

    /* ---------- stand-alone widgets ---------- */
    connect(ui->pushButton_open_hashfile, &QPushButton::clicked, this,
            &MainWindow::openHashFileClicked);
    connect(ui->pushButton_open_outfile, &QPushButton::clicked, this,
            &MainWindow::openOutfileClicked);
    connect(ui->pushButton_open_cache_path, &QPushButton::clicked, this,
            &MainWindow::openCachePathClicked);
    connect(ui->pushButton_start, &QPushButton::clicked, this, &MainWindow::startClicked);
    connect(ui->pushButton_copy_clipboard, &QPushButton::clicked, this,
            &MainWindow::copyCommandToClipboard);
}

void MainWindow::connectListSignals()
{
    // The controllers announce every command-relevant row change.
    for (PathListController *controller : {m_wordlistList.get(), m_pcfgRulesetList.get(),
                                           m_tableList.get(), m_feedArgumentList.get()}) {
        connect(controller, &PathListController::changed, this, &MainWindow::commandChanged);
    }

    /* ---------- wordlist ---------- */
    connect(ui->listWidget_wordlist, &QListWidget::itemClicked, this,
            &MainWindow::wordlistItemClicked);
    connect(ui->pushButton_remove_wordlist, &QPushButton::clicked, this,
            &MainWindow::removeWordlistClicked);
    connect(ui->pushButton_add_wordlist, &QPushButton::clicked, this,
            &MainWindow::addWordlistClicked);
    connect(ui->pushButton_add_wordlist_folder, &QPushButton::clicked, this,
            &MainWindow::addWordlistFolderClicked);
    connect(ui->toolButton_wordlist_move_up, &QToolButton::clicked, this,
            &MainWindow::wordlistMoveUpClicked);
    connect(ui->toolButton_wordlist_move_down, &QToolButton::clicked, this,
            &MainWindow::wordlistMoveDownClicked);
    /* ---------- PCFG, table and feed ---------- */
    connect(ui->pushButton_open_pcfg_ruleset, &QPushButton::clicked, this,
            &MainWindow::addPcfgRulesetClicked);
    connect(ui->pushButton_open_pcfg_ruleset_folder, &QPushButton::clicked, this,
            &MainWindow::addPcfgRulesetFolderClicked);
    connect(ui->pushButton_add_pcfg_ruleset_name, &QPushButton::clicked, this,
            &MainWindow::addPcfgRulesetNameClicked);
    // Typing a bundled name and pressing Enter is the natural sequence; without
    // this connection the button would be the only way to add one.
    connect(ui->lineEdit_pcfg_ruleset_name, &QLineEdit::returnPressed, this,
            &MainWindow::addPcfgRulesetNameClicked);
    connect(ui->pushButton_remove_pcfg_ruleset, &QPushButton::clicked, this,
            &MainWindow::removePcfgRulesetClicked);
    connect(ui->toolButton_pcfg_ruleset_up, &QToolButton::clicked, this,
            &MainWindow::pcfgRulesetMoveUpClicked);
    connect(ui->toolButton_pcfg_ruleset_down, &QToolButton::clicked, this,
            &MainWindow::pcfgRulesetMoveDownClicked);
    connect(ui->pushButton_open_table, &QPushButton::clicked, this, &MainWindow::addTableClicked);
    connect(ui->pushButton_remove_table, &QPushButton::clicked, this,
            &MainWindow::removeTableClicked);
    connect(ui->pushButton_open_feed, &QPushButton::clicked, this, &MainWindow::addFeedClicked);
    connect(ui->pushButton_add_feed_argument, &QPushButton::clicked, this,
            &MainWindow::addFeedArgumentClicked);
    // Typing an argument and pressing Enter is the natural sequence; without
    // this connection the button would be the only way to add one.
    connect(ui->lineEdit_feed_argument, &QLineEdit::returnPressed, this,
            &MainWindow::addFeedArgumentClicked);
    connect(ui->pushButton_remove_feed_argument, &QPushButton::clicked, this,
            &MainWindow::removeFeedArgumentClicked);
    connect(ui->toolButton_feed_argument_up, &QToolButton::clicked, this,
            &MainWindow::feedArgumentMoveUpClicked);
    connect(ui->toolButton_feed_argument_down, &QToolButton::clicked, this,
            &MainWindow::feedArgumentMoveDownClicked);
}

void MainWindow::connectDependencySignals()
{
    // The radios and the file boxes all feed the same predicate, so one slot
    // owns the whole rules choice - see updateRulesDependencies().
    connect(ui->checkBox_rulesfile_1, &QCheckBox::toggled, this,
            &MainWindow::updateRulesDependencies);
    connect(ui->checkBox_rulesfile_2, &QCheckBox::toggled, this,
            &MainWindow::updateRulesDependencies);
    connect(ui->checkBox_rulesfile_3, &QCheckBox::toggled, this,
            &MainWindow::updateRulesDependencies);
    bindEnabled(ui->radioButton_generate_rules, {ui->spinBox_generate_rules});
    connect(ui->radioButton_use_rules_file, &QRadioButton::toggled, this,
            &MainWindow::updateRulesDependencies);
    connect(ui->radioButton_generate_rules, &QRadioButton::toggled, this,
            &MainWindow::updateRulesDependencies);
    updateRulesDependencies();

    connect(ui->pushButton_open_rulesfile_1, &QPushButton::clicked, this,
            &MainWindow::openRulesFile1Clicked);
    connect(ui->pushButton_open_rulesfile_2, &QPushButton::clicked, this,
            &MainWindow::openRulesFile2Clicked);
    connect(ui->pushButton_open_rulesfile_3, &QPushButton::clicked, this,
            &MainWindow::openRulesFile3Clicked);

    // A custom charset field is only editable while its box is ticked.
    bindEnabled(ui->checkBox_custom_charset1, {ui->lineEdit_custom_charset1});
    bindEnabled(ui->checkBox_custom_charset2, {ui->lineEdit_custom_charset2});
    bindEnabled(ui->checkBox_custom_charset3, {ui->lineEdit_custom_charset3});
    bindEnabled(ui->checkBox_custom_charset4, {ui->lineEdit_custom_charset4});

    // The outfile is opt-in; JSON and the format field are exclusive, and the
    // value stays in the field so switching back restores what was typed.
    bindEnabled(ui->checkBox_outfile, {ui->lineEdit_outfile, ui->pushButton_open_outfile});
    bindDisabled(ui->checkBox_outfile_json, {ui->lineEdit_outfile_format});

    // The option dependencies that follow a checkbox, not the attack mode.
    bindEnabled(ui->checkBox_increment, {ui->label_increment_min, ui->spinBox_increment_min,
                                         ui->label_increment_max, ui->spinBox_increment_max});
    bindEnabled(ui->checkBox_status, {ui->spinBox_status_timer});
    bindEnabled(ui->checkBox_hwmon_temp_abort, {ui->spinBox_hwmon_temp_abort});

    // attackIndexChanged() refreshes the preview through
    // updateViewAttackMode(); connecting it to commandChanged() as well would
    // rebuild the preview twice for every mode change.
    connect(ui->comboBox_attack, &QComboBox::currentIndexChanged, this,
            &MainWindow::attackIndexChanged);
}

// One checkbox enables its targets while it is checked; the state is applied
// at bind time, so the .ui defaults and the connections cannot drift apart.
void MainWindow::bindEnabled(QAbstractButton *toggle, const QList<QWidget *> &targets)
{
    connect(toggle, &QAbstractButton::toggled, this, [targets](bool checked) {
        for (QWidget *target : targets) {
            target->setEnabled(checked);
        }
    });
    for (QWidget *target : targets) {
        target->setEnabled(toggle->isChecked());
    }
}

// The inverted pair: the target is enabled while the checkbox is NOT checked.
void MainWindow::bindDisabled(QAbstractButton *toggle, const QList<QWidget *> &targets)
{
    connect(toggle, &QAbstractButton::toggled, this, [targets](bool checked) {
        for (QWidget *target : targets) {
            target->setEnabled(!checked);
        }
    });
    for (QWidget *target : targets) {
        target->setEnabled(!toggle->isChecked());
    }
}

MainWindow::~MainWindow()
{
    // The controllers own connections into the list models. ~QListWidget
    // resets its model while the connections still run, so they have to be
    // gone before delete ui - being a child of the list would not help,
    // rowsRemoved fires before ~QObject disconnects.
    m_wordlistList.reset();
    m_pcfgRulesetList.reset();
    m_tableList.reset();
    m_feedArgumentList.reset();
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    // Fit once, on the first show. Guarded because hiding and restoring the
    // window must not override a size the user has chosen in the meantime.
    if (!m_sizedToContent) {
        m_sizedToContent = true;
        fitToContent();
    }
}

// Give the window the size the tabs need: the tallest tab decides the height,
// the widest one the width. A shorter tab keeps an empty strip; a taller one
// gets a scrollbar only when the screen is too small. The attack tab is
// measured across every mode, not only the selected one: its groups are
// hidden per mode, and a mode the user switches to later must not turn the
// window into a scroll area.
void MainWindow::fitToContent()
{
    QWidget *currentTab = ui->tabWidget_main->currentWidget();
    if (!currentTab) {
        return;
    }
    QScrollArea *currentArea = currentTab->findChild<QScrollArea *>();
    if (!currentArea || !currentArea->widget()) {
        return;
    }

    // The scroll area's own sizeHint is capped and does not describe the tab
    // content. Derive the window chrome from the geometry it has on first
    // show: the difference between the window and the scroll area that fills
    // it holds the menu bar, margins, tab bar and frame, and is the same for
    // every tab.
    const QSize chrome = size() - currentArea->size();

    // A screen too small for the content keeps the window on-screen; the
    // scroll areas stay in place and show their scrollbars as the fallback.
    // The margin leaves room for the window decorations.
    const QScreen *scr = screen();
    QSize cap;
    if (scr) {
        cap = scr->availableGeometry().size() - QSize(40, 80);
    }

    // Hidden pages report the minima they had before their layouts first ran.
    // Resolve every page first, then take the largest minimum in each
    // direction independently: one tab can be the height reference while
    // another is the width reference.
    struct TabMinimum {
        QScrollArea *area;
        QSize minimum;
    };

    // The width decides how far a wrapping widget - the mode hint - rewraps,
    // and its wrapped height in turn decides whether a tab needs room for a
    // vertical scrollbar, which adds to the width. Measure once without
    // wrapping, then again at the width the first pass chose, and repeat
    // while the width is still moving.
    QSize target;
    int contentWidth = 0;
    for (int pass = 0; pass < 3; ++pass) {
        // The attack tab's own minimum describes the selected mode alone.
        const QSize attackEnvelope = attackModeEnvelope(contentWidth);

        QList<TabMinimum> tabMinima;
        QSize contentMinimum(0, 0);
        for (int i = 0; i < ui->tabWidget_main->count(); ++i) {
            QWidget *tab = ui->tabWidget_main->widget(i);
            if (!tab) {
                continue;
            }
            QScrollArea *area = tab->findChild<QScrollArea *>();
            if (!area || !area->widget()) {
                continue;
            }
            activateLayouts(area->widget());
            QSize minimum = area->widget()->minimumSizeHint();
            if (contentWidth > 0) {
                // A wrapping widget announces a single line in its minimum
                // size hint; the height it needs at the tab width is a
                // heightForWidth() question, which the layouts pass up.
                const int wrapped = area->widget()->heightForWidth(contentWidth);
                if (wrapped > minimum.height()) {
                    minimum.setHeight(wrapped);
                }
            }
            if (area == ui->scrollArea_attack && !attackEnvelope.isEmpty()) {
                minimum = attackEnvelope;
            }
            tabMinima.append({area, minimum});
            contentMinimum = contentMinimum.expandedTo(minimum);
        }
        if (tabMinima.isEmpty()) {
            return;
        }

        target = chrome + contentMinimum;
        if (scr) {
            target = target.boundedTo(cap);
        }

        // A tab taller than the final viewport shows a vertical scrollbar.
        // Reserve its width so that content does not need a horizontal
        // scrollbar as well.
        const int viewportHeight = target.height() - chrome.height();
        int requiredWidth = 0;
        for (const TabMinimum &tab : std::as_const(tabMinima)) {
            int width = tab.minimum.width();
            if (tab.minimum.height() > viewportHeight) {
                width += tab.area->verticalScrollBar()->sizeHint().width();
            }
            requiredWidth = qMax(requiredWidth, width);
        }
        if (requiredWidth == contentWidth) {
            break;
        }
        contentWidth = requiredWidth;
    }

    target.setWidth(chrome.width() + contentWidth);
    if (scr) {
        target.setWidth(qMin(target.width(), cap.width()));
    }

    resize(target);
}

// The window must hold every attack mode without a scrollbar, but the tab
// only reports the selected mode's minimum: the groups the other modes use are
// hidden. Walk every mode once, with updates off, and take the largest minimum
// in each direction. The widest and the tallest mode need not be the same one.
//
// A wrapping widget (the mode hint) announces one line in its minimum size
// hint, no matter how long its text is: wrapWidth is the content width it
// wraps at, so that the height it really needs enters the envelope. Zero
// measures without that correction, for the caller that has no width yet.
QSize MainWindow::attackModeEnvelope(int wrapWidth)
{
    QWidget *content = ui->scrollArea_attack->widget();
    if (!content) {
        return {};
    }
    // The combo is filled in the constructor; keep a defensive fallback for a
    // window whose modes are not set up yet.
    if (ui->comboBox_attack->count() == 0) {
        return content->minimumSizeHint();
    }

    const AttackMode current = currentAttackMode();
    const bool updates = content->updatesEnabled();
    content->setUpdatesEnabled(false);

    QSize envelope(0, 0);
    for (int i = 0; i < ui->comboBox_attack->count(); ++i) {
        const auto mode = static_cast<AttackMode>(ui->comboBox_attack->itemData(i).toInt());
        m_attackModeView.apply(mode);
        activateLayouts(content);
        QSize minimum = content->minimumSizeHint();
        if (wrapWidth > 0) {
            const int wrapped = content->heightForWidth(wrapWidth);
            if (wrapped > minimum.height()) {
                minimum.setHeight(wrapped);
            }
        }
        envelope = envelope.expandedTo(minimum);
    }

    // Leave the tab showing the mode the user (or a restored session) chose.
    m_attackModeView.apply(current);
    activateLayouts(content);
    content->setUpdatesEnabled(updates);
    return envelope;
}

/*************** Menu Bar ***************/

// File → Export
void MainWindow::exportTriggered()
{
    const QString file = QFileDialog::getSaveFileName(
        this, tr("Export session"), suggestedExportPath(AppConstants::Files::SessionExport),
        tr("JSON Files (*.json)"));

    if (file.isEmpty()) {
        return;
    }

    QString error;
    const QJsonObject values = SessionTransfer::toDocument(this);
    if (StateFile::write(file, AppConstants::FileSections::Session, AppConstants::Sessions::Version,
                         values, &error)) {
        QMessageBox::information(this, tr("Exported"), tr("Session exported to “%1”.").arg(file));
    } else {
        QMessageBox::warning(this, tr("Export failed"), error);
    }
}

// File → Import
void MainWindow::importTriggered()
{
    const QString file = QFileDialog::getOpenFileName(this, tr("Import session"), suggestedFolder(),
                                                      tr("JSON Files (*.json)"));

    if (file.isEmpty()) {
        return;
    }

    const StateFile::Document document = StateFile::readDocument(
        file, AppConstants::FileSections::Session, AppConstants::Sessions::Version);
    if (document.status == StateFile::Status::Unreadable) {
        QMessageBox::warning(this, tr("Import failed"), document.error);
        return;
    }

    if (document.status == StateFile::Status::Newer) {
        QMessageBox::warning(
            this, tr("Import failed"),
            SessionTransfer::newerFileNote(document.version, AppConstants::Sessions::Version));
        return;
    }

    // The mapping applies the values; the hash type is the one it cannot
    // take yet, see restoreHashType().
    const QStringList notes = applySessionFile(document);

    if (notes.isEmpty()) {
        QMessageBox::information(this, tr("Imported"), tr("Session imported from “%1”.").arg(file));
    } else {
        QMessageBox::warning(
            this, tr("Imported"),
            tr("Session imported from “%1”.\n\n%2").arg(file, notes.join(QLatin1Char('\n'))));
    }
}

// Applies the values of a session file to the widgets and collects the notes
// about what could not be applied.
//
// The caller decides what to do with the notes: an import has a headline, the
// auto-load stays quiet when there is nothing to say.
QStringList MainWindow::applySessionFile(const StateFile::Document &document)
{
    // A retired widget's value cannot be applied, but it is not news: every
    // session written before the removal names it. Dropping the name keeps the
    // note for the values that were never ours.
    QStringList ignoredWidgets{AppConstants::Files::PreviewWidget};
    for (const QLatin1StringView &retired : AppConstants::Sessions::RetiredWidgetNames) {
        ignoredWidgets.append(QString(retired));
    }

    // The bulk write is silenced: one preview rebuild below, not one per
    // widget. The hash-type note comes last, after the transfer's notes.
    QStringList notes;
    {
        const CommandSilence silence(*this);
        notes = SessionTransfer::applyToWidgets(document, this, ignoredWidgets);
        const QString problem = restoreHashType(document.values);
        if (!problem.isEmpty()) {
            notes << problem;
        }
    }
    commandChanged();

    return notes;
}

// File → Export Settings
void MainWindow::exportSettingsTriggered()
{
    const QString file = QFileDialog::getSaveFileName(
        this, tr("Export settings"), suggestedExportPath(AppConstants::Files::SettingsExport),
        tr("JSON Files (*.json)"));

    if (file.isEmpty()) {
        return;
    }

    QString error;
    if (StateFile::write(file, AppConstants::FileSections::Settings,
                         AppConstants::Settings::Version, SettingsDialog::savedSettings(),
                         &error)) {
        QMessageBox::information(this, tr("Exported"), tr("Settings exported to “%1”.").arg(file));
    } else {
        QMessageBox::warning(this, tr("Export failed"), error);
    }
}

// File → Import Settings
void MainWindow::importSettingsTriggered()
{
    const QString file = QFileDialog::getOpenFileName(this, tr("Import settings"),
                                                      suggestedFolder(), tr("JSON Files (*.json)"));

    if (file.isEmpty()) {
        return;
    }

    const StateFile::Document document = StateFile::readDocument(
        file, AppConstants::FileSections::Settings, AppConstants::Settings::Version);
    if (document.status == StateFile::Status::Unreadable) {
        QMessageBox::warning(this, tr("Import failed"), document.error);
        return;
    }

    if (document.status == StateFile::Status::Newer) {
        QMessageBox::warning(
            this, tr("Import failed"),
            SessionTransfer::newerFileNote(document.version, AppConstants::Settings::Version));
        return;
    }

    // The dialog opens with the values of the file in it, so the user reviews
    // them before Save applies them.
    SettingsDialog settingsDialog(this);
    QStringList notes = settingsDialog.applyImportedSettings(document.values);
    notes << SessionTransfer::fileNotes(document.version, AppConstants::Settings::Version,
                                        document.otherSections);

    QStringList message;
    message << tr("Settings loaded from “%1”.").arg(file);
    message << tr("Review them and save to apply them.");
    message << notes;

    if (notes.isEmpty()) {
        QMessageBox::information(this, tr("Imported"), message.join(QLatin1Char('\n')));
    } else {
        QMessageBox::warning(this, tr("Imported"), message.join(QLatin1Char('\n')));
    }

    if (settingsDialog.exec() == QDialog::Accepted) {
        // An import can change the binary name and the short/long parameter
        // choice too, so it gets the same refresh as a save from the menu.
        refreshAfterSettingsChange();
    }
}

// Tools → Load Example Session → <attack mode>
void MainWindow::loadExampleSession(AttackMode mode)
{
    const Examples::Entry *entry = ExampleSession::find(mode);
    if (!entry) {
        return;
    }

    // Loading the example replaces the session the user has set up in the
    // form, so it is only carried out after the user confirmed it.
    const auto answer = QMessageBox::question(
        this, tr("Load example session"),
        tr("Load the %1 example? The current session is lost.\n\n"
           "The example cracks a test hash; hashcat should recover the password %2.")
            .arg(AttackModeView::name(mode), QString(entry->password)),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (answer != QMessageBox::Yes) {
        return;
    }

    // The examples are compiled into this build, so their format version
    // cannot be newer than this one; the test keeps the resources current.
    StateFile::Document document =
        StateFile::readDocument(QString(entry->resource), AppConstants::FileSections::Session,
                                AppConstants::Sessions::Version);
    if (document.status == StateFile::Status::Unreadable) {
        QMessageBox::warning(this, tr("Load example session"), document.error);
        return;
    }

    // The wordlists, rules and tables of an example are resources too, and
    // hashcat can only read real files: unpack them first, then point the
    // session at that directory.
    QString error;
    if (ExampleSession::containsExamplePrefix(document.values)) {
        QString directory;
        if (!ExampleSession::materialize(&directory, &error)) {
            QMessageBox::warning(this, tr("Load example session"), error);
            return;
        }
        document.values = ExampleSession::withExamplePaths(document.values, directory).toObject();
    }

    const QStringList notes = applySessionFile(document);
    if (!notes.isEmpty()) {
        QMessageBox::warning(this, tr("Load example session"), notes.join(QLatin1Char('\n')));
    }
}

// Tools → Reset Session
void MainWindow::resetSessionTriggered()
{
    // Resetting throws away the session the user has set up in the form,
    // so it is only carried out after the user confirmed it.
    const auto answer =
        QMessageBox::question(this, tr("Reset session"),
                              tr("Reset every field to its default? The current session is lost."),
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (answer != QMessageBox::Yes) {
        return;
    }

    // The constructor captured the snapshot; without it there is nothing to
    // reset to (and no way to get here).
    if (!m_defaultSnapshot.has_value()) {
        return;
    }

    // One bulk write from the constructor's snapshot: one preview rebuild
    // at the end, not one per setter.
    {
        const CommandSilence silence(*this);
        WidgetStateSerializer::applyToWidgets(m_defaultSnapshot.value(), this,
                                              {AppConstants::Files::PreviewWidget});
        // A fresh form must not re-apply a hash type a session is still
        // waiting to deliver.
        m_pendingHashTypeId = -1;
        // The snapshot stores -1 for the empty hash combo; index 0 is the
        // fresh selection whether or not the query has filled it since.
        ui->comboBox_hash_type->setCurrentIndex(0);
        // The recipe row is refreshed separately: the combo may already have
        // been at index 0, in which case setCurrentIndex() emits no signal.
        updateHashTypeDependencies();
        updateViewAttackMode();
    }
    commandChanged();
}

// File → Settings
void MainWindow::settingsTriggered()
{
    SettingsDialog settingsDialog(this);
    if (settingsDialog.exec() == QDialog::Accepted) {
        refreshAfterSettingsChange();
    }
}

// File → Quit
void MainWindow::quitTriggered()
{
    qApp->quit();
}

// Help → About Qt
void MainWindow::aboutQtTriggered()
{
    QApplication::aboutQt();
}

// Help → About
void MainWindow::aboutTriggered()
{
    AboutDialog about(this);
    about.exec();
}

// Path to the session JSON
QString MainWindow::sessionFile() const
{
    return StateFile::sessionFilePath();
}

// Whether the application keeps the session between two runs.
//
// The user can ask for a fresh form at every start, and then there is
// nothing to restore and nothing to store.
bool MainWindow::restoreSessionEnabled() const
{
    return SettingsManager::instance().getKey<bool>(
        AppConstants::SettingsKeys::RestoreSession,
        AppConstants::SettingsKeys::defaultOf<bool>(AppConstants::SettingsKeys::RestoreSession));
}

// Restores the session once, between construction and show(): the window
// must not paint the default state first, and the first fitToContent() has
// to measure the restored content. The flag means "the restore pass ran",
// not "a file was applied" - it is set before every early return, so the
// aboutToQuit save still stores the "no file yet" case. A second call is a
// no-op: applying twice would consume the pending hash type again.
void MainWindow::restoreSession()
{
    if (m_sessionRestored) {
        return;
    }
    m_sessionRestored = true;
    loadSession();
}

// Load the session - called from restoreSession()
void MainWindow::loadSession()
{
    if (!restoreSessionEnabled()) {
        return;
    }

    const QString file = sessionFile();
    if (!QFile::exists(file)) {
        return;
    }

    const StateFile::Document document = StateFile::readDocument(
        file, AppConstants::FileSections::Session, AppConstants::Sessions::Version);
    if (document.status == StateFile::Status::Unreadable) {
        // A file this version cannot read is not thrown away: it may belong
        // to a version that can read it, or the user may want to look at it.
        // The start goes on without the session.
        QMessageBox::warning(
            this, tr("Session warning"),
            tr("The session file could not be read, so the session was not restored. The file "
               "was left in place.\n\n%1")
                .arg(document.error));
        return;
    }

    // A session with a newer format version is left in place: it is what that
    // version saved, and the user may go back to it.
    if (document.status == StateFile::Status::Newer) {
        QMessageBox::warning(this, tr("Session warning"),
                             SessionTransfer::newerSessionNote(document.version));
        return;
    }

    // The auto-load is not an import: there is no headline, and nothing is
    // shown when the load went well.
    const QStringList notes = applySessionFile(document);

    if (!notes.isEmpty()) {
        QMessageBox::warning(this, tr("Session warning"), notes.join(QLatin1Char('\n')));
    }
}

// The hash type of a session.
//
// The mapping applies it when the list of hash types is there; this only
// checks whether this hashcat knows the one the file names, and it waits for
// the list while the query that fills it is still on its way.
//
// A hash type this hashcat does not know leaves the combo without a
// selection: the position would point at a different hash type, and cracking
// with the wrong one is worse than not cracking at all.
QString MainWindow::restoreHashType(const QJsonObject &values)
{
    const int id = values.value(QStringLiteral("comboBox_hash_type")).toInt(-1);

    if (ui->comboBox_hash_type->count() == 0) {
        // The list is still on its way; the value waits for it.
        m_pendingHashTypeId = id;
        return {};
    }

    if (id >= 0 && ui->comboBox_hash_type->findData(id) < 0) {
        // The hash type is gone. An empty selection is the honest answer: the
        // position would point at a different hash type, and cracking with the
        // wrong one is worse than not cracking at all.
        ui->comboBox_hash_type->setCurrentIndex(-1);
        m_pendingHashTypeId = -1;
        return tr("The session names the hash type %1, which this hashcat version does not "
                  "support.")
            .arg(id);
    }

    m_pendingHashTypeId = -1;
    return {};
}

// The recipe belongs to hash mode 4000 alone, so its row only exists where
// hashcat reads it. The value itself is left in the field: the builder drops
// a stale recipe for any other type, so switching away and back keeps it.
void MainWindow::updateHashTypeDependencies()
{
    const bool recipe =
        ui->comboBox_hash_type->currentData().toInt() == AppConstants::Hashcat::RecipeHashType;
    ui->label_hash_recipe->setVisible(recipe);
    ui->lineEdit_hash_recipe->setVisible(recipe);
}

// Applies the hash type a session asked for, now that the list of hash
// types exists. Returns a message when the session names a hash type this
// hashcat does not know.
QString MainWindow::applyPendingHashType()
{
    QString problem;

    if (m_pendingHashTypeId >= 0) {
        const int index = ui->comboBox_hash_type->findData(m_pendingHashTypeId);
        if (index >= 0) {
            ui->comboBox_hash_type->setCurrentIndex(index);
        } else {
            // The hash type is gone. An empty selection is the honest
            // answer: the position would point at a different hash type, and
            // cracking with the wrong one is worse than not cracking at
            // all.
            ui->comboBox_hash_type->setCurrentIndex(-1);
            problem = tr("The session names the hash type %1, which this hashcat version does "
                         "not support.")
                          .arg(m_pendingHashTypeId);
        }
    }

    m_pendingHashTypeId = -1;

    return problem;
}

// Save the session – called by aboutToQuit signal
void MainWindow::saveSession()
{
    // A session that was never restored must not be overwritten by a blank
    // form: a path that forgets restoreSession() would destroy the user's
    // stored session on quit. With the setting off there is nothing to
    // restore, and the removal below still applies.
    if (restoreSessionEnabled() && !m_sessionRestored) {
        return;
    }

    const QString file = sessionFile();

    // A file this version does not understand is neither written over nor
    // deleted: it belongs to another version, and the values in it may mean
    // something there.
    if (QFile::exists(file) && !sessionFileIsUnderstood(file)) {
        return;
    }

    // Without the setting there is nothing to restore, so nothing is stored
    // either. The session of an earlier run goes with it: it would come back
    // as stale state once the setting is switched on again.
    if (!restoreSessionEnabled()) {
        QFile::remove(file);
        return;
    }

    QDir().mkpath(QFileInfo(file).absolutePath());

    QString error;
    const QJsonObject values = SessionTransfer::toDocument(this);
    if (!StateFile::write(file, AppConstants::FileSections::Session,
                          AppConstants::Sessions::Version, values, &error)) {
        QMessageBox::warning(this, tr("Save failed"), error);
    }
}

// Whether this version understands the session file.
//
// A file it can read and whose format version it knows may be written over:
// it is the session of an earlier run, or of this one. A file it cannot read
// at all, or one that holds a format version this build does not know, was
// written by another version and is left alone.
bool MainWindow::sessionFileIsUnderstood(const QString &file) const
{
    const StateFile::Document document = StateFile::readDocument(
        file, AppConstants::FileSections::Session, AppConstants::Sessions::Version);
    return document.status != StateFile::Status::Unreadable
           && document.status != StateFile::Status::Newer;
}

void MainWindow::initExampleSessionMenu()
{
    for (const Examples::Entry &entry : Examples::All) {
        QAction *action =
            ui->menuLoadExampleSession->addAction(AttackModeView::name(entry.attackMode));
        action->setData(static_cast<int>(entry.attackMode));
        connect(action, &QAction::triggered, this,
                [this, mode = entry.attackMode] { loadExampleSession(mode); });
    }
}

// Fills the attack combo once, in the constructor. The numeric id travels
// as item data: the displayed text is what a translation changes, the id is
// what has to end up on the command line. A settings change must not refill
// it: that would silently reset the selected mode to Straight.
void MainWindow::initAttackModes()
{
    for (const AttackMode mode : AllAttackModes) {
        ui->comboBox_attack->addItem(AttackModeView::name(mode), static_cast<int>(mode));
    }
}

// Starts the hash-type query for the configured binary. Never touches the
// attack combo - the mode the user selected survives a settings change.
void MainWindow::reloadHashTypes()
{
    const QString hashcatPath =
        SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    const QString resolved = Executable::resolve(hashcatPath);
    if (resolved.isEmpty()) {
        // The list belongs to the binary that answered; a path that cannot
        // run must not leave another binary's types selectable. The loader
        // call below ends any query still on its way.
        ui->comboBox_hash_type->clear();
        // A query an earlier call started disabled the combo. Without a new
        // one there is nothing to wait for, so it goes back to the user.
        ui->comboBox_hash_type->setToolTip(
            hashcatPath.isEmpty()
                ? tr("Set the hashcat path under %1 → %2 to load the available hash types.")
                      .arg(menuPath(ui->menuFile->menuAction()->text()),
                           menuPath(ui->actionSettings->text()))
                : tr("The configured hashcat executable is not found: %1").arg(hashcatPath));
    }
    m_hashTypeLoader.reload(resolved);
}

// The refresh after the settings were saved or imported: re-run the query
// when the binary changed - or when nothing was ever loaded and the path may
// be usable now - and rebuild the preview, which carries the binary name and
// the short/long parameter choice. The comparison uses the resolved program,
// the one the loader records.
void MainWindow::refreshAfterSettingsChange()
{
    const QString hashcatPath =
        SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    if (Executable::resolve(hashcatPath) != m_hashTypeLoader.queriedPath()
        || (ui->comboBox_hash_type->count() == 0 && !m_hashTypeLoader.isLoading())) {
        reloadHashTypes();
    }
    commandChanged();
}

void MainWindow::attackIndexChanged([[maybe_unused]] int index)
{
    updateViewAttackMode();
}

void MainWindow::updateViewAttackMode()
{
    m_attackModeView.apply(currentAttackMode());
    commandChanged();
}

// The attack mode the user selected.
AttackMode MainWindow::currentAttackMode() const
{
    return static_cast<AttackMode>(ui->comboBox_attack->currentData().toInt());
}

void MainWindow::openHashFileClicked()
{
    QString hashfile = QFileDialog::getOpenFileName(this, tr("Open hash file"));
    if (!hashfile.isEmpty()) {
        ui->lineEdit_hashfile->setText(QDir::toNativeSeparators(hashfile));
    }
}

void MainWindow::openOutfileClicked()
{
    QString outfile = QFileDialog::getSaveFileName(this, tr("Save output file"));
    if (!outfile.isEmpty()) {
        ui->lineEdit_outfile->setText(QDir::toNativeSeparators(outfile));
    }
}

void MainWindow::openCachePathClicked()
{
    // hashcat only accepts an existing directory here, so the dialog offers
    // no file name to invent.
    const QString path = QFileDialog::getExistingDirectory(this, tr("Open cache directory"));
    if (!path.isEmpty()) {
        ui->lineEdit_cache_path->setText(QDir::toNativeSeparators(path));
    }
}

void MainWindow::removeWordlistClicked()
{
    m_wordlistList->removeSelected();
    ui->pushButton_remove_wordlist->setEnabled(false);
    ui->toolButton_wordlist_move_up->setEnabled(false);
    ui->toolButton_wordlist_move_down->setEnabled(false);
}

void MainWindow::addWordlistClicked()
{
    m_wordlistList->addPaths(QFileDialog::getOpenFileNames(this, tr("Add wordlists")));
}

void MainWindow::addWordlistFolderClicked()
{
    const QString folder = QFileDialog::getExistingDirectory(this, tr("Add wordlist folder"));
    if (!folder.isEmpty()) {
        m_wordlistList->addPaths({folder});
    }
}

void MainWindow::addPcfgRulesetClicked()
{
    m_pcfgRulesetList->addPaths(QFileDialog::getOpenFileNames(this, tr("Add rulesets")));
}

void MainWindow::addPcfgRulesetFolderClicked()
{
    const QString folder = QFileDialog::getExistingDirectory(this, tr("Add ruleset folder"));
    if (!folder.isEmpty()) {
        m_pcfgRulesetList->addPaths({folder});
    }
}

void MainWindow::addPcfgRulesetNameClicked()
{
    CheckedItemList::addName(ui->listWidget_pcfg_rulesets, ui->lineEdit_pcfg_ruleset_name->text());
    ui->lineEdit_pcfg_ruleset_name->clear();
}

void MainWindow::removePcfgRulesetClicked()
{
    m_pcfgRulesetList->removeSelected();
}

void MainWindow::pcfgRulesetMoveUpClicked()
{
    m_pcfgRulesetList->moveSelected(-1);
}

void MainWindow::pcfgRulesetMoveDownClicked()
{
    m_pcfgRulesetList->moveSelected(1);
}

void MainWindow::addTableClicked()
{
    m_tableList->addPaths(QFileDialog::getOpenFileNames(this, tr("Add tables")));
}

void MainWindow::removeTableClicked()
{
    m_tableList->removeSelected();
}

void MainWindow::addFeedClicked()
{
    // Only a file path goes through the dialog: the bundled feeds (random,
    // wordlist, …) are names the user types.
    const QString file = QFileDialog::getOpenFileName(this, tr("Open feed file"));
    if (!file.isEmpty()) {
        ui->lineEdit_feed->setText(QDir::toNativeSeparators(file));
    }
}

void MainWindow::addFeedArgumentClicked()
{
    // The argument field is a staging area: what reaches the command line is
    // the list, one argv token per row - checked, like a path row, so it can
    // be taken out of one run without being lost.
    const QString argument = ui->lineEdit_feed_argument->text().trimmed();
    if (!argument.isEmpty()) {
        auto *item = new QListWidgetItem(argument, ui->listWidget_feed_arguments);
        item->setCheckState(Qt::Checked);
        ui->lineEdit_feed_argument->clear();
    }
}

void MainWindow::removeFeedArgumentClicked()
{
    m_feedArgumentList->removeSelected();
}

void MainWindow::feedArgumentMoveUpClicked()
{
    m_feedArgumentList->moveSelected(-1);
}

void MainWindow::feedArgumentMoveDownClicked()
{
    m_feedArgumentList->moveSelected(1);
}

void MainWindow::wordlistMoveUpClicked()
{
    m_wordlistList->moveSelected(-1);
}

void MainWindow::wordlistMoveDownClicked()
{
    m_wordlistList->moveSelected(1);
}

void MainWindow::wordlistItemClicked([[maybe_unused]] QListWidgetItem *item)
{
    ui->pushButton_remove_wordlist->setEnabled(true);
    ui->toolButton_wordlist_move_up->setEnabled(true);
    ui->toolButton_wordlist_move_down->setEnabled(true);
}

// The one owner of the rules choice's enable state: the radio pair decides
// which side applies at all, each file box whether its field is live. Every
// widget feeding the predicate is connected here, so a session that ticks a
// file box under the generated side cannot enable its field.
void MainWindow::updateRulesDependencies()
{
    const bool useFiles = ui->radioButton_use_rules_file->isChecked();
    ui->checkBox_rulesfile_1->setEnabled(useFiles);
    ui->checkBox_rulesfile_2->setEnabled(useFiles);
    ui->checkBox_rulesfile_3->setEnabled(useFiles);
    // The join flag is only meaningful next to the files it joins; the
    // generated-rules radio is the other half of the same choice.
    ui->checkBox_rules_concat->setEnabled(useFiles);

    const auto applySide = [useFiles](QCheckBox *box, QLineEdit *field, QPushButton *button) {
        const bool enabled = useFiles && box->isChecked();
        field->setEnabled(enabled);
        button->setEnabled(enabled);
    };
    applySide(ui->checkBox_rulesfile_1, ui->lineEdit_rulesfile_1, ui->pushButton_open_rulesfile_1);
    applySide(ui->checkBox_rulesfile_2, ui->lineEdit_rulesfile_2, ui->pushButton_open_rulesfile_2);
    applySide(ui->checkBox_rulesfile_3, ui->lineEdit_rulesfile_3, ui->pushButton_open_rulesfile_3);
}

void MainWindow::openRulesFile1Clicked()
{
    QString rulesfile = QFileDialog::getOpenFileName(this, tr("Open rules file"));
    if (!rulesfile.isEmpty()) {
        ui->lineEdit_rulesfile_1->setText(QDir::toNativeSeparators(rulesfile));
    }
}

void MainWindow::openRulesFile2Clicked()
{
    QString rulesfile = QFileDialog::getOpenFileName(this, tr("Open rules file"));
    if (!rulesfile.isEmpty()) {
        ui->lineEdit_rulesfile_2->setText(QDir::toNativeSeparators(rulesfile));
    }
}

void MainWindow::openRulesFile3Clicked()
{
    QString rulesfile = QFileDialog::getOpenFileName(this, tr("Open rules file"));
    if (!rulesfile.isEmpty()) {
        ui->lineEdit_rulesfile_3->setText(QDir::toNativeSeparators(rulesfile));
    }
}

void MainWindow::hashFileTextChanged(const QString &text)
{
    if (text.isEmpty()) {
        return;
    }

    // Suggest "<hash file>.out", but never overwrite a value that is no longer
    // ours: the suggestion only follows the hash file name while the field
    // still holds the suggestion generated for the previous one.
    const QString suggestion = text + AppConstants::Defaults::OutfileSuffix;
    QLineEdit *outfile = ui->lineEdit_outfile;
    if (outfile->text().isEmpty() || outfile->text() == m_suggestedOutfile) {
        m_suggestedOutfile = suggestion;
        outfile->setText(suggestion);
    }
}

void MainWindow::copyCommandToClipboard()
{
    QString text = ui->lineEdit_command->text();
    // QApplication::clipboard() is null on platforms without clipboard support
    QClipboard *clipboard = QApplication::clipboard();
    if (clipboard) {
        clipboard->setText(text);
        // A one-shot tooltip confirms the copy. The button's own tooltip
        // would not re-render while the pointer stays on the button.
        QToolTip::showText(QCursor::pos(), tr("Copied to the clipboard"), nullptr, {}, 2000);
    }
}

void MainWindow::startClicked()
{
    // Everything the validation reads is collected here, on the GUI thread;
    // the validator itself looks at no settings. The option checks run first:
    // they are what the user has just configured, and hashcat would only
    // answer them with a usage error after the run has started.
    LaunchValidator::LaunchContext context;
    context.options = collectHashcatOptions();

    auto &settings = SettingsManager::instance();
    context.hashcatPath = settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    context.resolvedHashcatPath = Executable::resolve(context.hashcatPath);
    context.configuredTerminal = settings.getKey<QString>(AppConstants::SettingsKeys::Terminal);
    const QMap<QString, QString> terminals = TerminalCatalog::availableTerminals();
    context.availableTerminals = terminals.keys();

    // Nothing is started unless the configuration is known to be complete.
    const LaunchValidator::LaunchProblem problem = LaunchValidator::validate(context);
    if (problem) {
        showLaunchError(problem, context);
        return;
    }

    // The terminal command line is built in one place: PowerShell gets an
    // encoded script, so no hash, mask, path or session value is parsed as
    // shell code. The terminal stays open after hashcat exits, as before.
    // Every program is the resolved absolute one, never the stored text.
    const LaunchCommand command = TerminalCatalog::launchCommand(
        context.configuredTerminal, terminals.value(context.configuredTerminal),
        context.resolvedHashcatPath, CommandBuilder::build(context.options), true);
    if (command.program.isEmpty()) {
        QMessageBox::warning(this, tr("Launch failed"),
                             tr("The command line is too long for the terminal. Shorten the "
                                "argument lists or their values and try again."));
        return;
    }

    QString launchError;
    if (!TerminalCatalog::launchDetached(command, context.resolvedHashcatPath, &launchError)) {
        QMessageBox::warning(
            this, tr("Launch failed"),
            tr("Could not start “%1”: %2").arg(context.configuredTerminal, launchError));
    }
}

void MainWindow::showLaunchError(const LaunchValidator::LaunchProblem &problem,
                                 const LaunchValidator::LaunchContext &context)
{
    // The menu labels are computed here: menuPath also serves the hash-type
    // tooltip, and the presenter must not know the menu widgets.
    const QString menu = menuPath(ui->menuFile->menuAction()->text());
    const QString settingsEntry = menuPath(ui->actionSettings->text());
    const LaunchMessage message = messageFor(problem, context, menu, settingsEntry);
    if (message.text.isEmpty()) {
        return;
    }

    QMessageBox box(this);
    box.setIcon(message.icon);
    box.setTextFormat(message.format);
    box.setText(message.text);
    box.exec();
}

/*************** Helper ***************/

void MainWindow::commandChanged()
{
    // A bulk write is in progress: it rebuilds the preview once, at its end.
    if (m_commandSilenceDepth > 0) {
        return;
    }
    ++m_previewRebuilds;

    auto &settings = SettingsManager::instance();
    const QString hashcatPath = settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    const QString binary = hashcatPath.isEmpty() ? QString() : QFileInfo(hashcatPath).fileName();

    const QString command = CommandPreview::format(binary, generateArguments());
    ui->lineEdit_command->setText(command);
    ui->lineEdit_command->setCursorPosition(0);

    // The line edit shows the beginning of the command only - it is as wide as
    // the window and the command is not. The tooltip carries all of it.
    ui->lineEdit_command->setToolTip(CommandPreview::tooltip(command));
}

HashcatOptions MainWindow::collectHashcatOptions()
{
    return OptionCollector::collect(*ui, SettingsManager::instance().getKey<bool>(
                                             AppConstants::SettingsKeys::UseShortParameters));
}

QStringList MainWindow::generateArguments()
{
    return CommandBuilder::build(collectHashcatOptions());
}
