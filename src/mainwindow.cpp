/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "mainwindow.h"

#include "aboutdialog.h"
#include "appconstants.h"
#include "attackmodeview.h"
#include "backendinfodialog.h"
#include "checkeditemlist.h"
#include "commandbuilder.h"
#include "commandpreview.h"
#include "examples.h"
#include "examplesession.h"
#include "executable.h"
#include "hashtypeloader.h"
#include "identifydialog.h"
#include "launchmessage.h"
#include "launchvalidator.h"
#include "optioncollector.h"
#include "pathlistcontroller.h"
#include "pathmonitor.h"
#include "potfileinspector.h"
#include "potfileresultsdialog.h"
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
#include <QKeySequence>
#include <QLayout>
#include <QMap>
#include <QMenu>
#include <QMessageBox>
#include <QProgressDialog>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QStandardPaths>
#include <QStyle>
#include <QTimer>
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

// The shell the copied command has to fit: the configured terminal decides,
// not the platform this build happens to run on.
CommandPreview::Dialect previewDialect(const QString &terminal)
{
    return TerminalCatalog::isPowerShell(terminal) ? CommandPreview::Dialect::PowerShell
                                                   : CommandPreview::Dialect::Posix;
}

// The default comes from the setting table, like every other setting.
bool showLaunchCommand()
{
    return SettingsManager::instance().getKey<bool>(
        AppConstants::SettingsKeys::ShowLaunchCommand,
        AppConstants::SettingsKeys::defaultOf<bool>(AppConstants::SettingsKeys::ShowLaunchCommand));
}

// Shared construction for the repeated option shapes.
QRegularExpressionValidator *makeValidator(QLatin1StringView shape, QWidget *parent)
{
    return new QRegularExpressionValidator(QRegularExpression(QString(shape)), parent);
}

// Messages that carry a path or hashcat output are plain text: AutoText would
// read a leading '<' as markup.
void showPlainMessage(QWidget *parent, QMessageBox::Icon icon, const QString &title,
                      const QString &text)
{
    QMessageBox box(icon, title, text, QMessageBox::Ok, parent);
    box.setTextFormat(Qt::PlainText);
    box.exec();
}

} // namespace

QString MainWindow::fileMenuLabel() const
{
    return menuPath(ui->menuFile->menuAction()->text());
}

QString MainWindow::settingsMenuLabel() const
{
    return menuPath(ui->actionSettings->text());
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_attackModeView(*ui), m_hashTypeLoader(this)
{
    ui->setupUi(this);
    m_potfileCheck = std::make_unique<PotfileCheck>(this);

    configureWidgets();

    auto &settings = SettingsManager::instance();

    // The loader's replies fill the hash combo. The connections come before
    // the first reload, so the loading state is not missed.
    connect(&m_hashTypeLoader, &HashTypeLoader::loadingChanged, this, [this](bool loading) {
        ui->comboBox_hash_type->setEnabled(!loading);
        setHashTypeActionsBlocked(loading);
        if (loading) {
            ui->comboBox_hash_type->setToolTip(tr("Updating…"));
        }
    });
    connect(
        &m_hashTypeLoader, &HashTypeLoader::loaded, this,
        [this](const QMap<quint32, QString> &hashTypes) {
            // The refill is a bulk write: one preview rebuild at the end,
            // not one per combo event.
            QString problem;
            bool fromSession = false;
            {
                const CommandSilence silence(*this);
                // Remember what was selected before the refill: a settings
                // change that points at another binary must not silently
                // drop a hash type the new binary also knows.
                const QVariant previous = ui->comboBox_hash_type->currentData();
                const int previousId = previous.isValid() ? previous.toInt() : -1;
                const bool hasPrevious =
                    ui->comboBox_hash_type->currentIndex() >= 0
                    && (previousId >= 0 || previousId == AppConstants::Hashcat::AutoDetectHashType);

                ui->comboBox_hash_type->clear();
                addAutoDetectHashType();
                // Fill the combobox, hash type id as item data. QMap iterates
                // in key order, so real modes stay sorted after Auto-detect.
                for (auto it = hashTypes.constBegin(); it != hashTypes.constEnd(); ++it) {
                    ui->comboBox_hash_type->addItem(it.value(), it.key());
                }
                ui->comboBox_hash_type->setToolTip(m_hashTypeBaseToolTip);
                m_hashTypesPath = m_hashTypeLoader.queriedPath();
                m_hashTypesNeedsRetry = false;

                // A session loaded while the combo was empty still has its
                // id pending; otherwise keep the previous selection. With
                // neither, Auto-detect is the working default.
                fromSession = hasPendingHashType();
                problem = applyPendingHashType(
                    hasPrevious ? previousId : AppConstants::Hashcat::AutoDetectHashType,
                    fromSession);
            }
            commandChanged();
            if (!problem.isEmpty()) {
                QMessageBox::warning(
                    this, fromSession ? tr("Session warning") : tr("Hash type changed"), problem);
            }
        });
    connect(
        &m_hashTypeLoader, &HashTypeLoader::failed, this, [this](const HashTypeFailure &failure) {
            // An unparseable reply from the same binary only means the
            // refresh failed; the list on screen is still valid. A reply
            // from another binary is not, and must be cleared.
            const bool keepList = failure.kind == HashTypeFailure::Kind::Unparseable
                                  && !m_hashTypesPath.isEmpty()
                                  && m_hashTypesPath == m_hashTypeLoader.queriedPath()
                                  && ui->comboBox_hash_type->count() > 0;
            if (keepList) {
                // The list stays valid, but a later Settings save with the
                // same path must retry - there is no separate retry button.
                m_hashTypesNeedsRetry = true;
            } else {
                resetHashTypeCombo();
            }

            // The combo was disabled while the query ran. Restore its
            // tooltip on every failure path so it does not stay stuck on
            // "Updating…" forever.
            ui->comboBox_hash_type->setToolTip(
                keepList ? tr("The hash types could not be refreshed; showing the previous list.")
                         : tr("The hash types could not be loaded. Check the hashcat path in "
                              "Settings."));
            // Exit codes changed in hashcat 8.0.0, but this only wants
            // "did it work"; the GUI has no per-cause message, and a
            // reader must not turn this into per-code handling.
            const QString error =
                tr("Failed to obtain supported hash types.\nError: %1").arg(failure.detail);
#ifdef Q_OS_WIN
            if (failure.kind == HashTypeFailure::Kind::Timeout) {
                // Escape stderr: the FAQ link is the only intended markup.
                // <br> keeps line breaks that rich text would collapse.
                QString rich = tr("Failed to obtain supported hash types.\nError: %1")
                                   .arg(failure.detail.toHtmlEscaped());
                rich += tr("\n\nThis can be caused by Windows Defender or other "
                           "antivirus software. See the <a href=\"%1\">FAQ entry</a> "
                           "for a remedy.")
                            .arg(QString(AppConstants::Urls::FaqHashTypesTimeout));
                rich.replace(QLatin1Char('\n'), QStringLiteral("<br>"));
                QMessageBox box(QMessageBox::Warning, tr("Failed to load hash types"), rich,
                                QMessageBox::Ok, this);
                box.setTextFormat(Qt::RichText);
                box.exec();
                return;
            }
#endif
            showPlainMessage(this, QMessageBox::Warning, tr("Failed to load hash types"), error);
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

    // NoInsert leaves an unmatched name in the editor but not as a selection.
    // Reset it so the visible mode matches the id the command uses.
    connect(ui->comboBox_hash_type->lineEdit(), &QLineEdit::editingFinished, this,
            &MainWindow::hashTypeEditingFinished);

    // The combo's normal-state tooltip: what Auto-detect does and that the
    // list is searchable. The loading and error paths override it temporarily;
    // a successful load restores this text.
    m_hashTypeBaseToolTip =
        tr("The hash algorithm (-m). Auto-detect omits -m and lets hashcat identify the type. "
           "Type to search the list.");
    ui->comboBox_hash_type->setToolTip(m_hashTypeBaseToolTip);

    // Keep the .ui tooltips for the restore after loading.
    m_startToolTip = ui->pushButton_start->toolTip();
    m_copyToolTip = ui->pushButton_copy_clipboard->toolTip();
    m_copyPowerShellToolTip = ui->actionCopyPowerShellScript->toolTip();
    m_potfileCheckToolTip = ui->actionPotfileCheck->toolTip();

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
    ui->lineEdit_skip->setValidator(
        makeValidator(AppConstants::Validation::Digits, ui->lineEdit_skip));
    ui->lineEdit_limit->setValidator(
        makeValidator(AppConstants::Validation::Digits, ui->lineEdit_limit));

    // -D 3 (the accelerators) is gone with hashcat 8.0.0. The field was free
    // text, so a 3 typed here only failed after the launch, in the terminal;
    // the validator keeps the accepted device types where the user sees it.
    ui->lineEdit_opencl_device_types->setValidator(
        makeValidator(AppConstants::Validation::DeviceTypes, ui->lineEdit_opencl_device_types));

    // --outfile-format is free text, but hashcat takes only 1..6 in a comma
    // separated list. The launch check repeats this for values a session set.
    ui->lineEdit_outfile_format->setValidator(
        makeValidator(AppConstants::Validation::OutfileFormatList, ui->lineEdit_outfile_format));

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
    // the boolean flags of the table feed.
    const auto feedShapes = std::to_array<std::pair<QLineEdit *, QLatin1StringView>>({
        {ui->lineEdit_pcfg_weights, AppConstants::Validation::WeightsList},
        {ui->lineEdit_pcfg_maxword, AppConstants::Validation::MaxwordValue},
        {ui->lineEdit_pcfg_maxgain, AppConstants::Validation::DecimalNumber},
        {ui->lineEdit_table_identity, AppConstants::Validation::BinaryFlag},
        {ui->lineEdit_table_template, AppConstants::Validation::BinaryFlag},
        {ui->lineEdit_table_single, AppConstants::Validation::BinaryFlag},
    });
    for (const auto &[edit, shape] : feedShapes) {
        edit->setValidator(makeValidator(shape, edit));
    }
    // cap and maxperm reach beyond every Qt integer type (0…0xffffffff and a
    // full u64); the digits are filtered here, the bounds in validateOptions.
    ui->lineEdit_table_cap->setValidator(
        makeValidator(AppConstants::Validation::Digits, ui->lineEdit_table_cap));
    ui->lineEdit_table_maxperm->setValidator(
        makeValidator(AppConstants::Validation::Digits, ui->lineEdit_table_maxperm));

    /* ---------- help tooltips: translatable label + fixed reference tables -- */
    const QString charsetTip =
        tr("Use ?1–?4 in a mask to insert a custom charset. A mask is used by "
           "Brute-force, the hybrid modes and PCFG’s mask= setting. In PCFG a custom "
           "charset needs mask=; a charset the mask does not reference is ignored.")
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

    // Associate each caption with its field: clicking the label focuses the
    // field and screen readers announce the pair, as in the Settings dialog.
    const auto buddies = std::to_array<std::pair<QLabel *, QWidget *>>({
        {ui->label_hashfile, ui->lineEdit_hashfile},
        {ui->label_attack_mode, ui->comboBox_attack},
        {ui->label_hash_type, ui->comboBox_hash_type},
        {ui->label_hash_recipe, ui->lineEdit_hash_recipe},
        {ui->label_encoding_from, ui->lineEdit_encoding_from},
        {ui->label_encoding_to, ui->lineEdit_encoding_to},
        {ui->label_association_settings, ui->lineEdit_association_settings},
        {ui->label_pcfg_ruleset_name, ui->lineEdit_pcfg_ruleset_name},
        {ui->label_pcfg_scale, ui->lineEdit_pcfg_scale},
        {ui->label_pcfg_costmax, ui->lineEdit_pcfg_costmax},
        {ui->label_pcfg_weights, ui->lineEdit_pcfg_weights},
        {ui->label_pcfg_threads, ui->lineEdit_pcfg_threads},
        {ui->label_pcfg_kbits, ui->lineEdit_pcfg_kbits},
        {ui->label_pcfg_maxword, ui->lineEdit_pcfg_maxword},
        {ui->label_pcfg_maxgain, ui->lineEdit_pcfg_maxgain},
        {ui->label_pcfg_walk, ui->lineEdit_pcfg_walk},
        {ui->label_pcfg_mask, ui->lineEdit_pcfg_mask},
        {ui->label_pcfg_extra, ui->lineEdit_pcfg_extra},
        {ui->label_table_identity, ui->lineEdit_table_identity},
        {ui->label_table_template, ui->lineEdit_table_template},
        {ui->label_table_single, ui->lineEdit_table_single},
        {ui->label_table_cap, ui->lineEdit_table_cap},
        {ui->label_table_maxperm, ui->lineEdit_table_maxperm},
        {ui->label_table_extra, ui->lineEdit_table_extra},
        {ui->label_feed, ui->lineEdit_feed},
        {ui->label_feed_argument, ui->lineEdit_feed_argument},
        {ui->label_rule_left, ui->lineEdit_rule_left},
        {ui->label_rule_right, ui->lineEdit_rule_right},
        {ui->label_increment_min, ui->spinBox_increment_min},
        {ui->label_increment_max, ui->spinBox_increment_max},
        {ui->label_session, ui->lineEdit_session},
        {ui->label_runtime, ui->spinBox_runtime},
        {ui->label_skip, ui->lineEdit_skip},
        {ui->label_limit, ui->lineEdit_limit},
        {ui->label_cache_path, ui->lineEdit_cache_path},
        {ui->label_cpu_affinity, ui->lineEdit_cpu_affinity},
        {ui->label_backend_devices, ui->lineEdit_backend_devices},
        {ui->label_opencl_device_types, ui->lineEdit_opencl_device_types},
        {ui->label_outfile_format, ui->lineEdit_outfile_format},
        {ui->label_potfile_path, ui->lineEdit_potfile_path},
    });
    for (const auto &[label, field] : buddies) {
        label->setBuddy(field);
    }

    // Qt has no copy icon among its standard pixmaps. Use the platform's copy
    // symbol where there is an icon theme, and a text document everywhere else.
    ui->pushButton_copy_clipboard->setIcon(QIcon::fromTheme(
        QStringLiteral("edit-copy"), style()->standardIcon(QStyle::SP_FileDialogDetailedView)));

    // An icon-only button collapses below the icon on some styles; keep a
    // square that holds the fallback document icon.
    const int copySide = style()->pixelMetric(QStyle::PM_SmallIconSize)
                         + (2 * style()->pixelMetric(QStyle::PM_ButtonMargin));
    ui->pushButton_copy_clipboard->setMinimumSize(copySide, copySide);

    ui->pushButton_start->setShortcut(QKeySequence(QStringLiteral("Ctrl+Return")));
    ui->pushButton_copy_clipboard->setShortcut(QKeySequence(QStringLiteral("Ctrl+Shift+C")));

    updatePathMonitorBasePaths();
}

void MainWindow::updatePathMonitorBasePaths()
{
    const QString hashcatPath = Executable::resolve(
        SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath));
    m_wordlistList->setHashcatPath(hashcatPath);
    m_pcfgRulesetList->setHashcatPath(hashcatPath);
    m_tableList->setHashcatPath(hashcatPath);
}

// Every widget whose value reaches the command line is connected here, once.
// Controls that also enable fields or change the gating get their own
// connection; the attack combo refreshes the preview via attackIndexChanged().
void MainWindow::connectCommandSignals()
{
    m_commandTimer.setSingleShot(true);
    m_commandTimer.setInterval(AppConstants::Hashcat::CommandPreviewDebounceMs);
    connect(&m_commandTimer, &QTimer::timeout, this, &MainWindow::commandChanged);

    const StateWidgetLists widgets = collectStateWidgets(this);

    for (QLineEdit *edit : widgets.lineEdits) {
        // The preview is generated, not entered; the two staging fields
        // reach the command only through their Add buttons.
        if (edit == ui->lineEdit_command || edit == ui->lineEdit_feed_argument
            || edit == ui->lineEdit_pcfg_ruleset_name) {
            continue;
        }
        connect(edit, &QLineEdit::textChanged, this, &MainWindow::scheduleCommandChanged);
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
    connect(ui->actionCopyPowerShellScript, &QAction::triggered, this,
            &MainWindow::copyPowerShellScript);
    connect(ui->actionIdentifyHash, &QAction::triggered, this, &MainWindow::identifyHashTriggered);
    connect(ui->actionBackendInfo, &QAction::triggered, this, &MainWindow::backendInfoTriggered);
    connect(ui->actionPotfileCheck, &QAction::triggered, this, &MainWindow::potfileCheckTriggered);
    connect(ui->actionPotfileInspector, &QAction::triggered, this,
            &MainWindow::potfileInspectorTriggered);

    /* ---------- stand-alone widgets ---------- */
    connect(ui->pushButton_open_hashfile, &QPushButton::clicked, this,
            &MainWindow::openHashFileClicked);
    connect(ui->pushButton_open_outfile, &QPushButton::clicked, this,
            &MainWindow::openOutfileClicked);
    connect(ui->pushButton_open_potfile_path, &QPushButton::clicked, this,
            &MainWindow::openPotfilePathClicked);
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
    connect(ui->toolButton_table_up, &QToolButton::clicked, this, &MainWindow::tableMoveUpClicked);
    connect(ui->toolButton_table_down, &QToolButton::clicked, this,
            &MainWindow::tableMoveDownClicked);
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

    // JSON and the format field depend on the outfile itself.
    const auto updateOutfileFields = [this] {
        const bool outfile = ui->checkBox_outfile->isChecked();
        const bool json = ui->checkBox_outfile_json->isChecked();
        ui->lineEdit_outfile->setEnabled(outfile);
        ui->pushButton_open_outfile->setEnabled(outfile);
        ui->checkBox_outfile_json->setEnabled(outfile);
        ui->lineEdit_outfile_format->setEnabled(outfile && !json);
    };
    connect(ui->checkBox_outfile, &QAbstractButton::toggled, this, updateOutfileFields);
    connect(ui->checkBox_outfile_json, &QAbstractButton::toggled, this, updateOutfileFields);
    updateOutfileFields();
    // Exclusive like JSON and the format; the builder drops the path.
    bindDisabled(ui->checkBox_potfile_disable, {ui->label_potfile_path, ui->lineEdit_potfile_path,
                                                ui->pushButton_open_potfile_path});

    // hashcat refuses --remove with --keep-guessing. Disable the other box; a
    // restored pair stays enabled so one can be cleared. Validation is the guard.
    const auto updateRemoveKeepGuessing = [this] {
        const bool remove = ui->checkBox_remove->isChecked();
        const bool keep = ui->checkBox_keep_guessing->isChecked();
        const bool both = remove && keep;
        ui->checkBox_remove->setEnabled(!keep || both);
        ui->checkBox_keep_guessing->setEnabled(!remove || both);
    };
    connect(ui->checkBox_remove, &QCheckBox::toggled, this, updateRemoveKeepGuessing);
    connect(ui->checkBox_keep_guessing, &QCheckBox::toggled, this, updateRemoveKeepGuessing);
    updateRemoveKeepGuessing();

    // The option dependencies that follow a checkbox, not the attack mode.
    bindEnabled(ui->checkBox_increment,
                {ui->label_increment_min, ui->spinBox_increment_min, ui->label_increment_max,
                 ui->spinBox_increment_max, ui->checkBox_increment_inverse});
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
    m_wordlistList.reset();
    m_pcfgRulesetList.reset();
    m_tableList.reset();
    m_feedArgumentList.reset();
    // ~QWidget emits editingFinished after delete ui; disconnect first.
    disconnect(ui->comboBox_hash_type->lineEdit(), &QLineEdit::editingFinished, this,
               &MainWindow::hashTypeEditingFinished);
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

// Size the window for every tab: the tallest sets the height, the widest the
// width, and the attack tab is measured across all modes so switching modes
// cannot introduce a scrollbar.
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

    // The width decides how far the mode hint rewraps, its wrapped height
    // whether a scrollbar is needed; iterate until the width settles.
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
                // Wrapping labels report one line; ask for the real height.
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

// The largest minimum over every mode, because the tab reports only the
// selected one's. wrapWidth is the content width the mode hint wraps at; zero
// means the caller has no width yet and no wrapping correction is applied.
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

void MainWindow::exportTriggered()
{
    const QString file = QFileDialog::getSaveFileName(
        this, tr("Export session"), suggestedExportPath(AppConstants::Files::SessionExport),
        tr("JSON Files (*.json)"));

    if (file.isEmpty()) {
        return;
    }

    QString error;
    const QJsonObject values = sessionValues();
    if (StateFile::write(file, AppConstants::FileSections::Session, AppConstants::Sessions::Version,
                         values, &error)) {
        showPlainMessage(this, QMessageBox::Information, tr("Exported"),
                         tr("Session exported to “%1”.").arg(file));
    } else {
        showPlainMessage(this, QMessageBox::Warning, tr("Export failed"), error);
    }
}

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
        showPlainMessage(this, QMessageBox::Warning, tr("Import failed"), document.error);
        return;
    }

    if (document.status == StateFile::Status::Newer) {
        showPlainMessage(
            this, QMessageBox::Warning, tr("Import failed"),
            SessionTransfer::newerFileNote(document.version, AppConstants::Sessions::Version));
        return;
    }

    // The mapping applies the values; the hash type is the one it cannot
    // take yet, see restoreHashType().
    const QStringList notes = applySessionFile(document);

    if (notes.isEmpty()) {
        showPlainMessage(this, QMessageBox::Information, tr("Imported"),
                         tr("Session imported from “%1”.").arg(file));
    } else {
        showPlainMessage(
            this, QMessageBox::Warning, tr("Imported"),
            tr("Session imported from “%1”.\n\n%2").arg(file, notes.join(QLatin1Char('\n'))));
    }
}

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
        showPlainMessage(this, QMessageBox::Information, tr("Exported"),
                         tr("Settings exported to “%1”.").arg(file));
    } else {
        showPlainMessage(this, QMessageBox::Warning, tr("Export failed"), error);
    }
}

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
        showPlainMessage(this, QMessageBox::Warning, tr("Import failed"), document.error);
        return;
    }

    if (document.status == StateFile::Status::Newer) {
        showPlainMessage(
            this, QMessageBox::Warning, tr("Import failed"),
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
        showPlainMessage(this, QMessageBox::Information, tr("Imported"),
                         message.join(QLatin1Char('\n')));
    } else {
        showPlainMessage(this, QMessageBox::Warning, tr("Imported"),
                         message.join(QLatin1Char('\n')));
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

    // Loading replaces the form, so confirm first.
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
        showPlainMessage(this, QMessageBox::Warning, tr("Load example session"), document.error);
        return;
    }

    // The wordlists, rules and tables of an example are resources too, and
    // hashcat can only read real files: unpack them first, then point the
    // session at that directory.
    QString error;
    if (ExampleSession::containsExamplePrefix(document.values)) {
        QString directory;
        if (!ExampleSession::materialize(&directory, &error)) {
            showPlainMessage(this, QMessageBox::Warning, tr("Load example session"), error);
            return;
        }
        document.values = ExampleSession::withExamplePaths(document.values, directory).toObject();
    }

    const QStringList notes = applySessionFile(document);
    if (!notes.isEmpty()) {
        showPlainMessage(this, QMessageBox::Warning, tr("Load example session"),
                         notes.join(QLatin1Char('\n')));
    }
}

// Tools → Reset Session
void MainWindow::resetSessionTriggered()
{
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
        // fresh selection - Auto-detect once the query has filled the combo.
        ui->comboBox_hash_type->setCurrentIndex(0);
        // The recipe row is refreshed separately: the combo may already have
        // been at index 0, in which case setCurrentIndex() emits no signal.
        updateHashTypeDependencies();
        updateViewAttackMode();
    }
    commandChanged();
}

void MainWindow::settingsTriggered()
{
    SettingsDialog settingsDialog(this);
    if (settingsDialog.exec() == QDialog::Accepted) {
        refreshAfterSettingsChange();
    }
}

void MainWindow::quitTriggered()
{
    qApp->quit();
}

void MainWindow::aboutQtTriggered()
{
    QApplication::aboutQt();
}

void MainWindow::aboutTriggered()
{
    AboutDialog about(this);
    about.exec();
}

void MainWindow::identifyHashTriggered()
{
    // Stack-local like AboutDialog: persistent widgets enter session serialization.
    IdentifyDialog dialog(this);
    dialog.setInput(ui->lineEdit_hashfile->text());
    dialog.setApplyHandler([this](int hashTypeId, const std::optional<QString> &input) {
        return applyIdentifiedHashType(hashTypeId, input);
    });
    dialog.exec();
}

void MainWindow::backendInfoTriggered()
{
    BackendInfoDialog dialog(this);
    dialog.exec();
}

bool MainWindow::preparePotfileQuery(const QString &title, PotfileQueryInput &input,
                                     QString &modeName, QString &hashcatPath)
{
    const QString entered = ui->lineEdit_hashfile->text().trimmed();
    if (entered.isEmpty()) {
        showPlainMessage(this, QMessageBox::Information, title,
                         tr("Enter a hash or select a hash file first."));
        return false;
    }

    const QString configured =
        SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    if (configured.isEmpty()) {
        showPlainMessage(this, QMessageBox::Information, title,
                         tr("No hashcat executable is configured. Set one in Settings."));
        return false;
    }
    hashcatPath = Executable::resolve(configured);
    if (hashcatPath.isEmpty()) {
        showPlainMessage(this, QMessageBox::Warning, title,
                         tr("The configured hashcat executable is not found:\n%1").arg(configured));
        return false;
    }

    const QVariant hashTypeData = ui->comboBox_hash_type->currentData();
    input.hashType =
        hashTypeData.isValid() ? hashTypeData.toInt() : AppConstants::Hashcat::AutoDetectHashType;
    // Auto-detect is best-effort: a wrong mode silently miscounts the potfile.
    if (input.hashType < 0) {
        showPlainMessage(this, QMessageBox::Information, title,
                         PotfileCheck::missingHashTypeMessage());
        ui->comboBox_hash_type->setFocus();
        return false;
    }
    input.hashInput = PotfileQuery::resolveHashInput(entered, hashcatPath);
    // Resolve against the directory hashcat runs in, so the query reads the
    // same potfile a launch would.
    input.potfilePath = PotfileQuery::resolvePath(ui->lineEdit_potfile_path->text(), hashcatPath);
    input.username = ui->checkBox_ignoreusername->isChecked();
    // The editor may hold a name that was never selected; the results title
    // must name the mode the query uses.
    const int typeIndex = ui->comboBox_hash_type->currentIndex();
    modeName = typeIndex >= 0 ? ui->comboBox_hash_type->itemText(typeIndex) : QString();
    return true;
}

void MainWindow::potfileInspectorTriggered()
{
    PotfileInspectorDialog dialog(this);
    dialog.setPotfilePath(ui->lineEdit_potfile_path->text());
    dialog.exec();
}

void MainWindow::potfileCheckTriggered()
{
    if (refuseWhileHashTypesLoad(tr("Check Against Potfile"))) {
        return;
    }
    PotfileQueryInput input;
    QString modeName;
    QString hashcatPath;
    if (!preparePotfileQuery(tr("Check Against Potfile"), input, modeName, hashcatPath)) {
        return;
    }

    ui->actionPotfileCheck->setEnabled(false);

    std::optional<PotfileCheckResult> result;
    QProgressDialog progress(tr("Checking potfile…"), tr("Cancel"), 0, 0, this);
    progress.setWindowTitle(tr("Check Against Potfile"));
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setAutoClose(false);
    progress.setAutoReset(false);

    const QMetaObject::Connection finishedConnection =
        connect(m_potfileCheck.get(), &PotfileCheck::finished, &progress,
                [&result, &progress](const PotfileCheckResult &checkResult) {
                    result = checkResult;
                    progress.accept();
                });
    auto cancelled = std::make_shared<bool>(false);
    const auto markCancelled = [cancelled] { *cancelled = true; };
    connect(&progress, &QProgressDialog::canceled, m_potfileCheck.get(), &PotfileCheck::cancel);
    connect(&progress, &QDialog::rejected, m_potfileCheck.get(), &PotfileCheck::cancel);
    connect(&progress, &QProgressDialog::canceled, &progress, markCancelled);
    connect(&progress, &QDialog::rejected, &progress, markCancelled);

    // Start after exec() has entered its event loop: a reply fast enough to
    // finish first would otherwise close the dialog before it is shown. A
    // dialog cancelled before this fires must not start a query at all.
    QTimer::singleShot(0, m_potfileCheck.get(), [this, hashcatPath, input, cancelled] {
        if (*cancelled) {
            return;
        }
        m_potfileCheck->start(hashcatPath, input);
    });

    progress.exec();
    disconnect(finishedConnection);
    ui->actionPotfileCheck->setEnabled(true);

    if (!result) {
        // The dialog was closed while the query was still running.
        m_potfileCheck->cancel();
        showPlainMessage(this, QMessageBox::Information, tr("Check Against Potfile"),
                         tr("Check cancelled."));
        return;
    }

    switch (result->status) {
    case PotfileCheckResult::Status::Ok:
    case PotfileCheckResult::Status::LeftUnknown: {
        PotfileResultsDialog dialog(this);
        dialog.setResult(*result, modeName, input.potfilePath);
        dialog.exec();
        break;
    }
    case PotfileCheckResult::Status::Cancelled:
        showPlainMessage(this, QMessageBox::Information, tr("Check Against Potfile"),
                         tr("Check cancelled."));
        break;
    case PotfileCheckResult::Status::ShowFailed: {
        // The combined output can be long: keep it expandable and copyable.
        const QString summary = result->diagnostic.section(QLatin1Char('\n'), 0, 0).trimmed();
        QMessageBox box(QMessageBox::Warning, tr("Check Against Potfile"),
                        summary.isEmpty() ? tr("hashcat could not query the potfile.") : summary,
                        QMessageBox::Ok, this);
        box.setTextFormat(Qt::PlainText);
        if (result->diagnostic != summary) {
            box.setDetailedText(result->diagnostic);
        }
        box.exec();
        break;
    }
    }
}

bool MainWindow::applyIdentifiedHashType(int hashTypeId, const std::optional<QString> &input)
{
    // An empty or in-flight list is not the final word from this binary: a
    // reload still shows the previous list, so don't call the id unsupported.
    if (m_hashTypeLoader.isLoading()
        || (m_hashTypesPath.isEmpty() && ui->comboBox_hash_type->count() <= 1)) {
        QMessageBox::information(this, tr("Identify hash"),
                                 tr("The hash type list is unavailable or still loading. Nothing "
                                    "was changed. Check the hashcat path in Settings or try again "
                                    "once the list is available."));
        return false;
    }

    const int index = ui->comboBox_hash_type->findData(hashTypeId);
    if (index < 0) {
        QMessageBox::warning(this, tr("Identify hash"),
                             tr("Hash mode %1 is not in this hashcat’s hash-type list. "
                                "Nothing was changed.")
                                 .arg(hashTypeId));
        return false;
    }
    ui->comboBox_hash_type->setCurrentIndex(index);
    // An unchanged input keeps the form's own text; an edited one is written
    // back raw, never as a resolved absolute path.
    if (input.has_value()) {
        ui->lineEdit_hashfile->setText(input.value());
    }
    return true;
}

QString MainWindow::sessionFile() const
{
    return StateFile::sessionFilePath();
}

bool MainWindow::restoreSessionEnabled() const
{
    return SettingsManager::instance().getKey<bool>(
        AppConstants::SettingsKeys::RestoreSession,
        AppConstants::SettingsKeys::defaultOf<bool>(AppConstants::SettingsKeys::RestoreSession));
}

// Restores the session once, before show(): the window must not paint the
// default state first, and the first fit has to measure the restored content.
// The flag means the pass ran, not that a file was applied; a second call is
// a no-op.
void MainWindow::restoreSession()
{
    if (m_sessionRestored) {
        return;
    }
    m_sessionRestored = true;
    loadSession();
}

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
        // Keep an unreadable file; the user may move it aside.
        offerSessionFileRecovery(
            file, tr("The session file could not be read, so the session was not restored.\n\n%1")
                      .arg(document.error));
        return;
    }

    // A session with a newer format version is left in place: it is what that
    // version saved, and the user may go back to it.
    if (document.status == StateFile::Status::Newer) {
        offerSessionFileRecovery(file, SessionTransfer::newerSessionNote(document.version));
        return;
    }

    // The auto-load is not an import: there is no headline, and nothing is
    // shown when the load went well.
    const QStringList notes = applySessionFile(document);

    if (!notes.isEmpty()) {
        showPlainMessage(this, QMessageBox::Warning, tr("Session warning"),
                         notes.join(QLatin1Char('\n')));
    }
}

void MainWindow::offerSessionFileRecovery(const QString &file, const QString &message)
{
    QMessageBox box(QMessageBox::Warning, tr("Session warning"), message, QMessageBox::NoButton,
                    this);
    box.setTextFormat(Qt::PlainText);
    box.setInformativeText(
        tr("Move the file aside to start with a fresh session? It is renamed, never deleted."));
    QPushButton *move = box.addButton(tr("Move File Aside"), QMessageBox::ActionRole);
    QPushButton *keep = box.addButton(tr("Keep File"), QMessageBox::RejectRole);
    box.setDefaultButton(keep);
    box.setEscapeButton(keep);
    box.exec();
    if (box.clickedButton() != move) {
        return;
    }

    // Bump the suffix until the backup name is free.
    QString backup = file + QStringLiteral(".bak");
    for (int suffix = 1; QFile::exists(backup); ++suffix) {
        backup = file + QStringLiteral(".bak.%1").arg(suffix);
    }
    if (!QFile::rename(file, backup)) {
        QMessageBox warning(QMessageBox::Warning, tr("Session warning"),
                            tr("The session file could not be moved to:\n%1").arg(backup),
                            QMessageBox::Ok, this);
        warning.setTextFormat(Qt::PlainText);
        warning.exec();
        return;
    }
    QMessageBox moved(QMessageBox::Information, tr("Session warning"),
                      tr("The session file was moved to:\n%1").arg(backup), QMessageBox::Ok, this);
    moved.setTextFormat(Qt::PlainText);
    moved.exec();
}

// Whether the hash type a restored session named is still waiting for the
// list of hash types. Auto-detect is a value a session can name, so it counts
// as pending; every other negative id means the session named none.
bool MainWindow::hasPendingHashType() const
{
    return m_pendingHashTypeId >= 0
           || m_pendingHashTypeId == AppConstants::Hashcat::AutoDetectHashType;
}

// The hash type a session names. It waits for the list while the query is in
// flight; an id this hashcat does not know falls back to Auto-detect rather
// than picking another real type that would crack with the wrong algorithm.
QString MainWindow::restoreHashType(const QJsonObject &values)
{
    const int id = values.value(QStringLiteral("comboBox_hash_type")).toInt(-1);
    const bool namesType = id >= 0 || id == AppConstants::Hashcat::AutoDetectHashType;

    // "List available" means a successful query has answered, not that the
    // combo holds items: it always holds the permanent Auto-detect entry, even
    // while no hashcat path is configured. With no answered query the id must
    // wait, or a configuration problem would be reported as an unsupported
    // hash type and the value would be lost.
    if (m_hashTypesPath.isEmpty()) {
        m_pendingHashTypeId = namesType ? id : -1;
        return {};
    }

    if (namesType && ui->comboBox_hash_type->findData(id) < 0) {
        const int autoIndex =
            ui->comboBox_hash_type->findData(AppConstants::Hashcat::AutoDetectHashType);
        if (autoIndex >= 0) {
            ui->comboBox_hash_type->setCurrentIndex(autoIndex);
        }
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

void MainWindow::hashTypeEditingFinished()
{
    QComboBox *combo = ui->comboBox_hash_type;
    QLineEdit *editor = combo->lineEdit();

    // A complete typed name selects its id, case-insensitively like the
    // completer; anything else falls back to the selection.
    const int typed = combo->findText(editor->text(), Qt::MatchFixedString);
    if (typed >= 0) {
        combo->setCurrentIndex(typed);
    }

    const int current = combo->currentIndex();
    editor->setText(current >= 0 ? combo->itemText(current) : QString());
}

QString MainWindow::applyPendingHashType(int fallbackId, bool fromSession)
{
    const int pending = m_pendingHashTypeId;
    const bool hasPending = hasPendingHashType();
    const int requested = hasPending ? pending : fallbackId;

    QString problem;
    const int index = ui->comboBox_hash_type->findData(requested);
    if (index >= 0) {
        ui->comboBox_hash_type->setCurrentIndex(index);
    } else {
        const int autoIndex =
            ui->comboBox_hash_type->findData(AppConstants::Hashcat::AutoDetectHashType);
        if (autoIndex >= 0) {
            ui->comboBox_hash_type->setCurrentIndex(autoIndex);
        }
        if (requested >= 0) {
            problem = fromSession
                          ? tr("The session names the hash type %1, which this hashcat version "
                               "does not support. Auto-detect was selected.")
                                .arg(requested)
                          : tr("The selected hash type %1 is not offered by this hashcat "
                               "version. Auto-detect was selected.")
                                .arg(requested);
        }
    }

    m_pendingHashTypeId = -1;
    return problem;
}

// The values a save writes. While the hash type query has not answered, the
// session's id is still pending and not in the combo, so store the pending id
// instead of the empty selection.
QJsonObject MainWindow::sessionValues()
{
    QJsonObject values = SessionTransfer::toDocument(this);
    const int pending = m_pendingHashTypeId;
    if (hasPendingHashType()) {
        values.insert(QStringLiteral("comboBox_hash_type"), pending);
    }
    return values;
}

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
    const QJsonObject values = sessionValues();
    if (!StateFile::write(file, AppConstants::FileSections::Session,
                          AppConstants::Sessions::Version, values, &error)) {
        showPlainMessage(this, QMessageBox::Warning, tr("Save failed"), error);
    }
}

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

void MainWindow::reloadHashTypes()
{
    const QString hashcatPath =
        SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    const QString resolved = Executable::resolve(hashcatPath);
    if (resolved.isEmpty()) {
        // The list belongs to the binary that answered; a path that cannot
        // run must not leave another binary's types selectable. The loader
        // call below ends any query still on its way.
        resetHashTypeCombo();
        // A query an earlier call started disabled the combo. Without a new
        // one there is nothing to wait for, so it goes back to the user.
        ui->comboBox_hash_type->setToolTip(
            hashcatPath.isEmpty()
                ? tr("Set the hashcat path under %1 → %2 to load the available hash types.")
                      .arg(fileMenuLabel(), settingsMenuLabel())
                : tr("The configured hashcat executable is not found: %1").arg(hashcatPath));
    }
    m_hashTypeLoader.reload(resolved);
}

void MainWindow::addAutoDetectHashType()
{
    ui->comboBox_hash_type->addItem(tr("Auto-detect"), AppConstants::Hashcat::AutoDetectHashType);
}

void MainWindow::resetHashTypeCombo()
{
    ui->comboBox_hash_type->clear();
    addAutoDetectHashType();
    m_hashTypesPath.clear();
    m_hashTypesNeedsRetry = false;
}

// The refresh after the settings were saved or imported: re-run the query
// when the binary changed - or when nothing was ever loaded and the path may
// be usable now - and rebuild the preview, which carries the binary name and
// the short/long parameter choice. The comparison uses the resolved program,
// the one the loader records.
void MainWindow::refreshAfterSettingsChange()
{
    // The refresh after the settings were saved or imported: ...
    const QString hashcatPath =
        SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    if (Executable::resolve(hashcatPath) != m_hashTypeLoader.queriedPath() || m_hashTypesNeedsRetry
        || (m_hashTypesPath.isEmpty() && !m_hashTypeLoader.isLoading())) {
        reloadHashTypes();
    }
    // Relative wordlists, rulesets, tables and the cache path follow the
    // program directory, so a new binary changes what they point at.
    updatePathMonitorBasePaths();
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

void MainWindow::openPotfilePathClicked()
{
    // The potfile may not exist yet, so offer a new name too.
    const QString path = QFileDialog::getSaveFileName(this, tr("Choose potfile"));
    if (!path.isEmpty()) {
        ui->lineEdit_potfile_path->setText(QDir::toNativeSeparators(path));
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

void MainWindow::tableMoveUpClicked()
{
    m_tableList->moveSelected(-1);
}

void MainWindow::tableMoveDownClicked()
{
    m_tableList->moveSelected(1);
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
    if (refuseWhileHashTypesLoad(tr("Copy command"))) {
        return;
    }
    // A pending text edit must not leave the clipboard with the previous command.
    const QString text = commandPreviewText();
    // QApplication::clipboard() is null on platforms without clipboard support
    QClipboard *clipboard = QApplication::clipboard();
    if (clipboard) {
        clipboard->setText(text);
        QToolTip::showText(QCursor::pos(), tr("Copied to the clipboard"),
                           ui->pushButton_copy_clipboard, {}, 2000);
    }
}

// The copy and the launch share one script builder, so what this puts on the
// clipboard is exactly what a PowerShell terminal would run, only readable.
void MainWindow::copyPowerShellScript()
{
    if (refuseWhileHashTypesLoad(tr("Copy as PowerShell Script"))) {
        return;
    }
    auto &settings = SettingsManager::instance();
    const QString hashcatPath = settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    const QString resolved = Executable::resolve(hashcatPath);

    if (resolved.isEmpty()) {
        // The path is user supplied, so the message is plain text: a '<' in
        // it would be read as markup by the default AutoText.
        QMessageBox box(
            QMessageBox::Information, tr("Copy as PowerShell Script"),
            hashcatPath.isEmpty()
                ? tr("Select the hashcat executable under %1 → %2 first.")
                      .arg(fileMenuLabel(), settingsMenuLabel())
                : tr("The configured hashcat executable is not found:\n%1").arg(hashcatPath),
            QMessageBox::Ok, this);
        box.setTextFormat(Qt::PlainText);
        box.exec();
        return;
    }

    const QString script = TerminalCatalog::powerShellScript(
        resolved, CommandBuilder::build(collectHashcatOptions()), showLaunchCommand());
    if (QClipboard *clipboard = QApplication::clipboard()) {
        clipboard->setText(script);
        QToolTip::showText(QCursor::pos(), tr("PowerShell script copied to the clipboard"), this,
                           {}, 2000);
    }
}

void MainWindow::startClicked()
{
    if (refuseWhileHashTypesLoad(tr("Start"))) {
        return;
    }
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
        context.resolvedHashcatPath, CommandBuilder::build(context.options), true,
        showLaunchCommand());
    if (command.refusal != LaunchCommand::Refusal::None) {
        showLaunchMessage(messageFor(command, context.configuredTerminal));
        return;
    }

    QString launchError;
    if (!TerminalCatalog::launchDetached(command, context.resolvedHashcatPath, &launchError)) {
        showPlainMessage(
            this, QMessageBox::Warning, tr("Launch failed"),
            tr("Could not start “%1”: %2").arg(context.configuredTerminal, launchError));
    }
}

void MainWindow::showLaunchError(const LaunchValidator::LaunchProblem &problem,
                                 const LaunchValidator::LaunchContext &context)
{
    // The menu labels are computed here: the presenter must not know the menu widgets.
    const LaunchMessage message =
        messageFor(problem, context, fileMenuLabel(), settingsMenuLabel());
    if (message.text.isEmpty()) {
        return;
    }
    showLaunchMessage(message);
    focusProblemField(problem);
}

void MainWindow::focusProblemField(const LaunchValidator::LaunchProblem &problem)
{
    using Error = LaunchValidator::LaunchError;

    QWidget *field = nullptr;
    switch (problem.code) {
    case Error::NoHashFile:
        field = ui->lineEdit_hashfile;
        break;
    case Error::NoWordlist:
    case Error::TooManyWordlists:
    case Error::WrongWordlistCount:
    case Error::WordlistMustBeFile:
        field = ui->listWidget_wordlist;
        break;
    case Error::NoTable:
    case Error::InvalidTablePath:
        field = ui->listWidget_tables;
        break;
    case Error::NoMask:
    case Error::MaskNeedsWordMarker:
    case Error::MaskMultipleWordMarkers:
    case Error::MaskQBeforeWord:
    case Error::MaskMultipleQMarks:
    case Error::MaskQWithoutWordlist:
    case Error::SecondWordlistWithoutQ:
    case Error::MaskMarkersNotAllowed:
    case Error::PcfgCharsetWithoutMask:
        field = ui->lineEdit_mask;
        break;
    case Error::NoFeed:
        field = ui->lineEdit_feed;
        break;
    case Error::NoHashRecipe:
        field = ui->lineEdit_hash_recipe;
        break;
    case Error::InvalidOutfileFormat:
        field = ui->lineEdit_outfile_format;
        break;
    case Error::InvalidCachePath:
        field = ui->lineEdit_cache_path;
        break;
    case Error::InvalidPotfilePath:
        field = ui->lineEdit_potfile_path;
        break;
    case Error::InvalidSkipLimit:
        // One error covers two fields; the detail is the offending value.
        field =
            problem.detail == ui->lineEdit_skip->text() ? ui->lineEdit_skip : ui->lineEdit_limit;
        break;
    case Error::InvalidOpenclDeviceTypes:
        field = ui->lineEdit_opencl_device_types;
        break;
    case Error::InvalidCpuAffinity:
        field = ui->lineEdit_cpu_affinity;
        break;
    case Error::InvalidBackendDevices:
        field = ui->lineEdit_backend_devices;
        break;
    case Error::InvalidSessionName:
        field = ui->lineEdit_session;
        break;
    case Error::InvalidRulesetPath:
        for (QLineEdit *rules :
             {ui->lineEdit_rulesfile_1, ui->lineEdit_rulesfile_2, ui->lineEdit_rulesfile_3}) {
            if (rules->text() == problem.detail) {
                field = rules;
                break;
            }
        }
        break;
    // The rest point into Settings or name fields left unfocused here.
    default:
        break;
    }

    if (field == nullptr) {
        return;
    }
    // Focus cannot reach a widget on another tab; show its page first.
    for (int i = 0; i < ui->tabWidget_main->count(); ++i) {
        QWidget *page = ui->tabWidget_main->widget(i);
        if (page != nullptr && page->isAncestorOf(field)) {
            ui->tabWidget_main->setCurrentIndex(i);
            break;
        }
    }
    field->setFocus();
    if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(field)) {
        lineEdit->selectAll();
    }
}

void MainWindow::showLaunchMessage(const LaunchMessage &message)
{
    QMessageBox box(this);
    box.setIcon(message.icon);
    box.setTextFormat(message.format);
    box.setText(message.text);
    box.exec();
}

void MainWindow::setHashTypeActionsBlocked(bool blocked)
{
    const QString reason = tr("Wait until the hash type list has loaded.");
    const QList<QPair<QWidget *, QString>> widgets{
        {ui->pushButton_start, m_startToolTip},
        {ui->pushButton_copy_clipboard, m_copyToolTip},
    };
    for (const auto &[widget, toolTip] : widgets) {
        widget->setEnabled(!blocked);
        widget->setToolTip(blocked ? reason : toolTip);
    }
    const QList<QPair<QAction *, QString>> actions{
        {ui->actionCopyPowerShellScript, m_copyPowerShellToolTip},
        {ui->actionPotfileCheck, m_potfileCheckToolTip},
    };
    for (const auto &[action, toolTip] : actions) {
        action->setEnabled(!blocked);
        action->setToolTip(blocked ? reason : toolTip);
    }
}

bool MainWindow::refuseWhileHashTypesLoad(const QString &title)
{
    if (!m_hashTypeLoader.isLoading()) {
        return false;
    }
    showPlainMessage(this, QMessageBox::Information, title,
                     tr("The hash type list is still loading. Wait until it has loaded and try "
                        "again."));
    return true;
}

/*************** Helper ***************/

void MainWindow::scheduleCommandChanged()
{
    // A bulk write is in progress: it rebuilds the preview once, at its end.
    if (m_commandSilenceDepth > 0) {
        return;
    }
    m_commandTimer.start();
}

void MainWindow::commandChanged()
{
    // An explicit rebuild supersedes a queued text edit.
    m_commandTimer.stop();
    // A bulk write is in progress: it rebuilds the preview once, at its end.
    if (m_commandSilenceDepth > 0) {
        return;
    }
    ++m_previewRebuilds;

    const QString command = commandPreviewText();
    ui->lineEdit_command->setText(command);
    ui->lineEdit_command->setCursorPosition(0);

    // The line edit shows the beginning of the command only - it is as wide as
    // the window and the command is not. The tooltip carries all of it.
    const CommandPreview::Dialect dialect = previewDialect(
        SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::Terminal));
    ui->lineEdit_command->setToolTip(CommandPreview::tooltip(command, dialect));
}

QString MainWindow::commandPreviewText()
{
    auto &settings = SettingsManager::instance();
    const QString hashcatPath = settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    const QString binary = hashcatPath.isEmpty() ? QString() : QFileInfo(hashcatPath).fileName();
    const CommandPreview::Dialect dialect =
        previewDialect(settings.getKey<QString>(AppConstants::SettingsKeys::Terminal));
    return CommandPreview::format(binary, generateArguments(), dialect);
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
