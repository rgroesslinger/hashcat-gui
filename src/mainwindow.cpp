/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "settingsdialog.h"
#include "settingsmanager.h"
#include "appconstants.h"
#include "commandbuilder.h"
#include "examples.h"
#include "hashcatinfoparser.h"
#include "helperutils.h"
#include "statefile.h"
#include "widgetstateserializer.h"
#include <QAction>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonArray>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QSaveFile>
#include <QAbstractButton>
#include <QAbstractItemModel>
#include <QClipboard>
#include <QCursor>
#include <QFutureWatcher>
#include <QIcon>
#include <QLayout>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QSignalBlocker>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QIntValidator>
#include <QStandardPaths>
#include <QStyle>
#include <QTextDocument>
#include <QToolTip>

#include <utility>

#if defined(Q_OS_WIN)
#include <process.h>
#include <windows.h>
#endif

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

// A tooltip that contains something like "<b>" would be read as markup and
// mangle the display. Escape such text once, and turn its breaks into <br> for
// the rich text renderer. Plain text is left alone: escaping it would show
// entities such as &amp; literally.
QString escapeRichText(QString text)
{
    if (!Qt::mightBeRichText(text)) {
        return text;
    }
    return text.toHtmlEscaped().replace(QLatin1Char('\n'), QStringLiteral("<br>"));
}

// A menu path names the entry, not the action: the ellipsis of an entry that
// opens a dialog is not part of "File → Settings".
QString menuPath(const QString &text)
{
    QString path = text;
    if (path.endsWith(QStringLiteral("…"))) {
        path.chop(1);
    }
    return path.trimmed();
}

// The argument list as the preview and the clipboard show it: one token per
// argument, joined with spaces. The hash recipe is a single argument that
// contains spaces, so it is quoted for display - a copied command must not
// split it. The argv handed to hashcat keeps the token unquoted.
QStringList displayArguments(const QStringList &arguments)
{
    QStringList display = arguments;
#ifdef Q_OS_WIN
    const QChar quote = QLatin1Char('"');
#else
    const QChar quote = QLatin1Char('\'');
#endif
    const qsizetype recipe = display.indexOf(QString(AppConstants::Hashcat::HashRecipe));
    if (recipe >= 0 && recipe + 1 < display.size()) {
        display[recipe + 1] = quote + display.at(recipe + 1) + quote;
    }
    return display;
}

// The tooltip of the command preview: the whole command, because the line
// edit shows only its beginning.
//
// Plain text, not rich text: a rich text tooltip is squeezed into whatever
// width its text control happens to have and grows tall instead, while a
// plain text one keeps the line breaks it is given.
//
// A path that contains something like "<b>" would be read as a tag and
// mangle the display, so a command that looks like markup is escaped and its
// breaks become <br>: the rare case loses the plain-text width, but keeps the
// text.
QString commandTooltip(const QString &command)
{
    if (command.isEmpty()) {
        return {};
    }

    // The breaks go between the arguments, so a flag stays with the value
    // that follows it.
    const QStringList parts = command.split(QLatin1Char(' '));
    QStringList lines;
    QString line;
    for (const QString &part : std::as_const(parts)) {
        const bool endsWithFlag = line.section(QLatin1Char(' '), -1).startsWith(QLatin1Char('-'));
        // The space between the parts counts too: the line is one character
        // longer than the two of them together.
        if (!line.isEmpty() && line.size() + part.size() + 1 > 120 && !endsWithFlag) {
            lines << line;
            line = part;
        } else {
            line += (line.isEmpty() ? QString() : QStringLiteral(" ")) + part;
        }
    }
    if (!line.isEmpty()) {
        lines << line;
    }

    return escapeRichText(lines.join(QLatin1Char('\n')));
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

// Whether a session asks for one of the bundled example data files. Only such
// a session pays the unpacking cost; the brute-force example has no data.
bool containsExamplePrefix(const QJsonValue &value)
{
    if (value.isString()) {
        return value.toString().contains(QString(Examples::DataPrefix));
    }
    if (value.isObject()) {
        const QJsonObject object = value.toObject();
        for (auto it = object.constBegin(); it != object.constEnd(); ++it) {
            if (containsExamplePrefix(it.value())) {
                return true;
            }
        }
    }
    if (value.isArray()) {
        const QJsonArray array = value.toArray();
        for (const auto &entry : array) {
            if (containsExamplePrefix(entry)) {
                return true;
            }
        }
    }
    return false;
}

// Replaces the <examples>/ token in every string of the session, including the
// text of list rows, with the directory the resources were unpacked to.
QJsonValue withExamplePaths(const QJsonValue &value, const QString &directory)
{
    if (value.isString()) {
        QString text = value.toString();
        text.replace(QString(Examples::DataPrefix), directory);
        return text;
    }
    if (value.isObject()) {
        const QJsonObject object = value.toObject();
        QJsonObject result;
        for (auto it = object.constBegin(); it != object.constEnd(); ++it) {
            result.insert(it.key(), withExamplePaths(it.value(), directory));
        }
        return result;
    }
    if (value.isArray()) {
        const QJsonArray array = value.toArray();
        QJsonArray result;
        for (const auto &entry : array) {
            result.append(withExamplePaths(entry, directory));
        }
        return result;
    }
    return value;
}
} // namespace

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // The path rows say what they are at a glance: a file, a folder, or the
    // warning of a path hashcat cannot use. The style owns the pixmaps, and
    // the rows are compared by icon key, so these are asked for once.
    m_fileIcon = style()->standardIcon(QStyle::SP_FileIcon);
    m_folderIcon = style()->standardIcon(QStyle::SP_DirIcon);
    m_warningIcon = style()->standardIcon(QStyle::SP_MessageBoxWarning);

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
    const std::pair<QLineEdit *, QLatin1StringView> feedShapes[] = {
        {ui->lineEdit_pcfg_weights, AppConstants::Validation::WeightsList},
        {ui->lineEdit_pcfg_maxword, AppConstants::Validation::MaxwordValue},
        {ui->lineEdit_pcfg_maxgain, AppConstants::Validation::DecimalNumber},
        {ui->lineEdit_table_identity, AppConstants::Validation::BinaryFlag},
        {ui->lineEdit_table_template, AppConstants::Validation::BinaryFlag},
        {ui->lineEdit_table_single, AppConstants::Validation::BinaryFlag},
    };
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
    const QString charsetTip = tr("Built-in charsets:") + QLatin1String("\n\n")
                               + QString::fromUtf8(AppConstants::HelpText::BuiltinCharsets);
    ui->lineEdit_mask->setToolTip(charsetTip);
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

    auto &settings = SettingsManager::instance();

    initHashAndAttackModes();
    initExampleSessionMenu();
    updateViewAttackMode();
    // The hash type list arrives asynchronously, but the recipe row can only
    // be hidden for the empty combo; every later change comes through the
    // connection set up below.
    updateHashTypeDependencies();

    /* ---------- save the session on quit ---------- */
    connect(qApp, &QCoreApplication::aboutToQuit, this, &MainWindow::saveSession);

    /* ---------- command preview ---------- */
    // Every widget whose value reaches the command line is connected here
    // exactly once. Controls that also need to enable fields, open dialogs or
    // change the attack-mode gating are connected to their own slot below;
    // that second connection is the side effect, not a second preview update.
    // The attack mode combo is the one exception: it refreshes the preview
    // through attackIndexChanged(), see below.
    const auto notifyOnText = [this](QLineEdit *edit) {
        connect(edit, &QLineEdit::textChanged, this, &MainWindow::commandChanged);
    };
    const auto notifyOnToggle = [this](QAbstractButton *button) {
        connect(button, &QAbstractButton::toggled, this, &MainWindow::commandChanged);
    };
    const auto notifyOnValue = [this](QSpinBox *spinBox) {
        connect(spinBox, &QSpinBox::valueChanged, this, &MainWindow::commandChanged);
    };
    const auto notifyOnIndex = [this](QComboBox *comboBox) {
        connect(comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::commandChanged);
    };

    // Attack tab
    notifyOnIndex(ui->comboBox_hash_type);
    // The recipe row belongs to hash mode 4000 alone; the combo decides.
    connect(ui->comboBox_hash_type, &QComboBox::currentIndexChanged, this,
            &MainWindow::updateHashTypeDependencies);
    notifyOnText(ui->lineEdit_hashfile);
    notifyOnText(ui->lineEdit_hash_recipe);
    notifyOnToggle(ui->checkBox_hex_charset);
    notifyOnToggle(ui->checkBox_hex_salt);
    // Word lists
    notifyOnText(ui->lineEdit_encoding_from);
    notifyOnText(ui->lineEdit_encoding_to);
    notifyOnToggle(ui->checkBox_hex_wordlist);
    // Rules
    notifyOnToggle(ui->checkBox_rulesfile_1);
    notifyOnToggle(ui->checkBox_rulesfile_2);
    notifyOnToggle(ui->checkBox_rulesfile_3);
    notifyOnToggle(ui->checkBox_rules_concat);
    notifyOnText(ui->lineEdit_rulesfile_1);
    notifyOnText(ui->lineEdit_rulesfile_2);
    notifyOnText(ui->lineEdit_rulesfile_3);
    notifyOnToggle(ui->radioButton_use_rules_file);
    notifyOnToggle(ui->radioButton_generate_rules);
    notifyOnValue(ui->spinBox_generate_rules);
    notifyOnText(ui->lineEdit_rule_left);
    notifyOnText(ui->lineEdit_rule_right);
    // Mask and charsets
    notifyOnText(ui->lineEdit_mask);
    notifyOnToggle(ui->checkBox_increment);
    notifyOnValue(ui->spinBox_increment_min);
    notifyOnValue(ui->spinBox_increment_max);
    notifyOnToggle(ui->checkBox_custom_charset1);
    notifyOnToggle(ui->checkBox_custom_charset2);
    notifyOnToggle(ui->checkBox_custom_charset3);
    notifyOnToggle(ui->checkBox_custom_charset4);
    notifyOnText(ui->lineEdit_custom_charset1);
    notifyOnText(ui->lineEdit_custom_charset2);
    notifyOnText(ui->lineEdit_custom_charset3);
    notifyOnText(ui->lineEdit_custom_charset4);
    // The feed modes (PCFG, table, generic) and the association settings
    // line. The feed argument field is left out on purpose: it only reaches
    // the command through its list, via the Add button.
    notifyOnText(ui->lineEdit_association_settings);
    notifyOnText(ui->lineEdit_pcfg_scale);
    notifyOnText(ui->lineEdit_pcfg_costmax);
    notifyOnText(ui->lineEdit_pcfg_weights);
    notifyOnText(ui->lineEdit_pcfg_threads);
    notifyOnText(ui->lineEdit_pcfg_kbits);
    notifyOnText(ui->lineEdit_pcfg_maxword);
    notifyOnText(ui->lineEdit_pcfg_maxgain);
    notifyOnText(ui->lineEdit_pcfg_walk);
    notifyOnText(ui->lineEdit_pcfg_mask);
    notifyOnText(ui->lineEdit_pcfg_extra);
    notifyOnText(ui->lineEdit_table_identity);
    notifyOnText(ui->lineEdit_table_template);
    notifyOnText(ui->lineEdit_table_single);
    notifyOnText(ui->lineEdit_table_cap);
    notifyOnText(ui->lineEdit_table_maxperm);
    notifyOnText(ui->lineEdit_table_extra);
    notifyOnText(ui->lineEdit_feed);
    // The two hash file options stay on the Attack tab; everything from the
    // outfile on belongs to the General tab.
    notifyOnToggle(ui->checkBox_remove);
    notifyOnToggle(ui->checkBox_ignoreusername);
    // General tab
    notifyOnToggle(ui->checkBox_outfile);
    notifyOnText(ui->lineEdit_outfile);
    notifyOnText(ui->lineEdit_outfile_format);
    notifyOnToggle(ui->checkBox_outfile_json);
    notifyOnText(ui->lineEdit_cache_path);
    notifyOnText(ui->lineEdit_cpu_affinity);
    notifyOnText(ui->lineEdit_backend_devices);
    notifyOnText(ui->lineEdit_session);
    notifyOnValue(ui->spinBox_runtime);
    notifyOnText(ui->lineEdit_skip);
    notifyOnText(ui->lineEdit_limit);
    notifyOnToggle(ui->checkBox_status);
    notifyOnValue(ui->spinBox_status_timer);
    notifyOnToggle(ui->checkBox_force);
    notifyOnToggle(ui->checkBox_potfile_disable);
    notifyOnToggle(ui->checkBox_optimized_kernel);
    notifyOnToggle(ui->checkBox_speed_only);
    notifyOnText(ui->lineEdit_opencl_device_types);
    notifyOnToggle(ui->checkBox_hwmon_temp_abort);
    notifyOnValue(ui->spinBox_hwmon_temp_abort);

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

    /* ---------- wordlist ---------- */
    connect(ui->listWidget_wordlist->model(), &QAbstractItemModel::rowsInserted, this,
            [this] { commandChanged(); });
    connect(ui->listWidget_wordlist->model(), &QAbstractItemModel::rowsRemoved, this,
            [this] { commandChanged(); });
    connect(ui->listWidget_wordlist->model(), &QAbstractItemModel::rowsMoved, this,
            [this] { commandChanged(); });
    connect(ui->listWidget_wordlist, &QListWidget::itemChanged, this, [this] { commandChanged(); });
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
    connect(ui->listWidget_wordlist->model(), &QAbstractItemModel::rowsInserted, this,
            &MainWindow::refreshWordlistMonitor);
    connect(ui->listWidget_wordlist->model(), &QAbstractItemModel::rowsRemoved, this,
            &MainWindow::refreshWordlistMonitor);
    // clear() - and with it a session restore or a reset - resets the model
    // instead of removing the rows one by one.
    connect(ui->listWidget_wordlist->model(), &QAbstractItemModel::modelReset, this,
            &MainWindow::refreshWordlistMonitor);
    connect(&m_wordlistMonitor, &PathMonitor::availabilityChanged, this,
            &MainWindow::wordlistAvailabilityChanged);

    /* ---------- PCFG, table and feed ---------- */
    // The three editable lists announce every change through their model,
    // which covers the in-place editing as well as the buttons.
    for (QListWidget *list :
         {ui->listWidget_pcfg_rulesets, ui->listWidget_tables, ui->listWidget_feed_arguments}) {
        connect(list->model(), &QAbstractItemModel::rowsInserted, this,
                [this] { commandChanged(); });
        connect(list->model(), &QAbstractItemModel::rowsRemoved, this,
                [this] { commandChanged(); });
        connect(list, &QListWidget::itemChanged, this, [this] { commandChanged(); });
    }

    // The two path lists own a monitor each. A session restore re-inserts
    // their rows instead of editing them, so insertion and the model reset are
    // the events that bring a path under watch.
    connect(ui->listWidget_pcfg_rulesets->model(), &QAbstractItemModel::rowsInserted, this,
            &MainWindow::refreshPcfgMonitor);
    connect(ui->listWidget_pcfg_rulesets->model(), &QAbstractItemModel::rowsRemoved, this,
            &MainWindow::refreshPcfgMonitor);
    connect(ui->listWidget_pcfg_rulesets->model(), &QAbstractItemModel::modelReset, this,
            &MainWindow::refreshPcfgMonitor);
    connect(&m_pcfgMonitor, &PathMonitor::availabilityChanged, this,
            &MainWindow::pcfgAvailabilityChanged);

    connect(ui->listWidget_tables->model(), &QAbstractItemModel::rowsInserted, this,
            &MainWindow::refreshTableMonitor);
    connect(ui->listWidget_tables->model(), &QAbstractItemModel::rowsRemoved, this,
            &MainWindow::refreshTableMonitor);
    connect(ui->listWidget_tables->model(), &QAbstractItemModel::modelReset, this,
            &MainWindow::refreshTableMonitor);
    connect(&m_tableMonitor, &PathMonitor::availabilityChanged, this,
            &MainWindow::tableAvailabilityChanged);

    connect(ui->pushButton_open_pcfg_ruleset, &QPushButton::clicked, this,
            &MainWindow::addPcfgRulesetClicked);
    connect(ui->pushButton_open_pcfg_ruleset_folder, &QPushButton::clicked, this,
            &MainWindow::addPcfgRulesetFolderClicked);
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

    /* ---------- rules ---------- */
    connect(ui->checkBox_rulesfile_1, &QCheckBox::toggled, this, &MainWindow::rulesFile1Toggled);
    connect(ui->checkBox_rulesfile_2, &QCheckBox::toggled, this, &MainWindow::rulesFile2Toggled);
    connect(ui->checkBox_rulesfile_3, &QCheckBox::toggled, this, &MainWindow::rulesFile3Toggled);
    connect(ui->pushButton_open_rulesfile_1, &QPushButton::clicked, this,
            &MainWindow::openRulesFile1Clicked);
    connect(ui->pushButton_open_rulesfile_2, &QPushButton::clicked, this,
            &MainWindow::openRulesFile2Clicked);
    connect(ui->pushButton_open_rulesfile_3, &QPushButton::clicked, this,
            &MainWindow::openRulesFile3Clicked);
    connect(ui->radioButton_generate_rules, &QRadioButton::toggled, this,
            &MainWindow::generateRulesToggled);
    connect(ui->radioButton_use_rules_file, &QRadioButton::toggled, this,
            &MainWindow::useRulesFileToggled);

    /* ---------- custom charset ---------- */
    connect(ui->checkBox_custom_charset1, &QCheckBox::toggled, this,
            &MainWindow::customCharset1Toggled);
    connect(ui->checkBox_custom_charset2, &QCheckBox::toggled, this,
            &MainWindow::customCharset2Toggled);
    connect(ui->checkBox_custom_charset3, &QCheckBox::toggled, this,
            &MainWindow::customCharset3Toggled);
    connect(ui->checkBox_custom_charset4, &QCheckBox::toggled, this,
            &MainWindow::customCharset4Toggled);

    /* ---------- stand-alone widgets ---------- */
    connect(ui->lineEdit_hashfile, &QLineEdit::textChanged, this, &MainWindow::hashFileTextChanged);
    connect(ui->pushButton_open_hashfile, &QPushButton::clicked, this,
            &MainWindow::openHashFileClicked);
    connect(ui->pushButton_open_outfile, &QPushButton::clicked, this,
            &MainWindow::openOutfileClicked);
    connect(ui->pushButton_open_cache_path, &QPushButton::clicked, this,
            &MainWindow::openCachePathClicked);
    connect(ui->pushButton_start, &QPushButton::clicked, this, &MainWindow::startClicked);
    connect(ui->pushButton_copy_clipboard, &QPushButton::clicked, this,
            &MainWindow::copyCommandToClipboard);
    // attackIndexChanged() refreshes the preview through
    // updateViewAttackMode(); connecting it to commandChanged() as well would
    // rebuild the preview twice for every mode change.
    connect(ui->comboBox_attack, &QComboBox::currentIndexChanged, this,
            &MainWindow::attackIndexChanged);
    connect(ui->checkBox_outfile, &QCheckBox::toggled, this, &MainWindow::outfileToggled);
    // JSON and the format field are exclusive; the value stays in the field
    // so switching back restores what was typed.
    connect(ui->checkBox_outfile_json, &QCheckBox::toggled, this,
            [this](bool checked) { ui->lineEdit_outfile_format->setEnabled(!checked); });

    /* ---------- dependent widgets ---------- */
    connect(ui->checkBox_increment, &QCheckBox::toggled, this,
            &MainWindow::updateOptionDependencies);
    connect(ui->checkBox_status, &QCheckBox::toggled, this, &MainWindow::updateOptionDependencies);
    connect(ui->checkBox_hwmon_temp_abort, &QCheckBox::toggled, this,
            &MainWindow::updateOptionDependencies);

    loadSession();

    /* ---------- show Settings if hashcatPath not set ---------- */
    if (settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath).isEmpty()) {
        QMetaObject::invokeMethod(this, &MainWindow::settingsTriggered, Qt::QueuedConnection);
    }
}

MainWindow::~MainWindow()
{
    // QMainWindow destroys its child widgets only after the members are gone.
    // Clearing a list then resets its model, and the handlers read ui and the
    // monitor members, which have to be out of the picture by that point.
    disconnect(ui->listWidget_wordlist->model(), nullptr, this, nullptr);
    disconnect(ui->listWidget_pcfg_rulesets->model(), nullptr, this, nullptr);
    disconnect(ui->listWidget_tables->model(), nullptr, this, nullptr);
    disconnect(ui->listWidget_feed_arguments->model(), nullptr, this, nullptr);
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
        return QSize();
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
        applyAttackModeView(mode);
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
    applyAttackModeView(current);
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
    const QJsonObject values =
        WidgetStateSerializer::toJson(this, {AppConstants::Files::PreviewWidget});
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

    QString error;
    QJsonObject values;
    int version = 0;
    QStringList otherSections;
    if (!StateFile::read(file, AppConstants::FileSections::Session, &values, &version,
                         &otherSections, &error)) {
        QMessageBox::warning(this, tr("Import failed"), error);
        return;
    }

    if (version > AppConstants::Sessions::Version) {
        QMessageBox::warning(this, tr("Import failed"),
                             newerFileNote(version, AppConstants::Sessions::Version));
        return;
    }

    // The mapping applies the values; the hash type is the one it cannot
    // take yet, see restoreHashType().
    const QStringList notes = applySessionFile(values, version, otherSections);

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
QStringList MainWindow::applySessionFile(const QJsonObject &values, int version,
                                         const QStringList &otherSections)
{
    // A retired widget's value cannot be applied, but it is not news: every
    // session written before the removal names it. Dropping the name keeps the
    // note for the values that were never ours.
    QStringList ignoredWidgets{AppConstants::Files::PreviewWidget};
    for (const QLatin1StringView &retired : AppConstants::Sessions::RetiredWidgetNames) {
        ignoredWidgets.append(QString(retired));
    }

    // The hash combo has no items yet - the query that fills it runs
    // asynchronously. Remember the stored hash type; the reply applies it
    // once the items exist.
    const WidgetStateSerializer::ApplyResult notApplied =
        WidgetStateSerializer::applyToWidgets(values, this, ignoredWidgets);
    const QString problem = restoreHashType(values);
    commandChanged();

    QStringList notes = fileNotes(version, AppConstants::Sessions::Version, otherSections);

    // The notes are only there when there is something to say.
    const QString unknownNote = StateFile::unknownValuesNote(notApplied.unknownKeys);
    if (!unknownNote.isEmpty()) {
        notes << unknownNote;
    }

    const QString wrongTypeNote = StateFile::wrongTypeValuesNote(notApplied.wrongTypes);
    if (!wrongTypeNote.isEmpty()) {
        notes << wrongTypeNote;
    }
    if (!problem.isEmpty()) {
        notes << problem;
    }

    return notes;
}

// The notes an import has about the file itself: the version it was made by
// and the sections it holds besides the one that was imported.
QStringList MainWindow::fileNotes(int fileVersion, int currentVersion,
                                  const QStringList &otherSections) const
{
    QStringList notes;

    // A newer format version is refused before it gets here: a name that
    // survived may mean something else there. A file that names no version is
    // hand-edited; its values are applied like an older file's.
    if (fileVersion >= 0 && fileVersion < currentVersion) {
        notes << tr("The file was made by an older version of hashcat-gui (format version %1; "
                    "this version reads format version %2).")
                     .arg(fileVersion)
                     .arg(currentVersion);
    }

    if (!otherSections.isEmpty()) {
        notes << tr("The file holds sections this import does not use: %1.")
                     .arg(StateFile::nameList(otherSections));
    }

    return notes;
}

// Why a file with a newer format version is not applied at all: this version
// does not know what the values it cannot name mean, and a name that survived
// may mean something else there.
QString MainWindow::newerFileNote(int fileVersion, int currentVersion) const
{
    return tr("The file is in format version %1, but this version reads format version %2, so it "
              "was not applied. Update hashcat-gui to read it.")
        .arg(fileVersion)
        .arg(currentVersion);
}

// Why the session of a newer version is not restored.
//
// The file is left as it is as well, so the session of this run is not
// stored either: the session of the newer version wins over the one of this
// run.
QString MainWindow::newerSessionNote(int fileVersion) const
{
    return tr("The session file is in format version %1, but this version reads format version %2, "
              "so the session was not restored. The file was left as it is, so the session of "
              "this run is not stored either. Update hashcat-gui to read it.")
        .arg(fileVersion)
        .arg(AppConstants::Sessions::Version);
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
        QMessageBox::information(this, tr("Exported"), tr("Settings exported to %1.").arg(file));
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

    QString error;
    QJsonObject values;
    int version = 0;
    QStringList otherSections;
    if (!StateFile::read(file, AppConstants::FileSections::Settings, &values, &version,
                         &otherSections, &error)) {
        QMessageBox::warning(this, tr("Import failed"), error);
        return;
    }

    if (version > AppConstants::Settings::Version) {
        QMessageBox::warning(this, tr("Import failed"),
                             newerFileNote(version, AppConstants::Settings::Version));
        return;
    }

    // The dialog opens with the values of the file in it, so the user reviews
    // them before Save applies them.
    SettingsDialog settingsDialog(this);
    QStringList notes = settingsDialog.applyImportedSettings(values);
    notes << fileNotes(version, AppConstants::Settings::Version, otherSections);

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
        // choice too, so the preview and the hash types need the same refresh
        // as after the settings were saved from the menu.
        if (ui->comboBox_hash_type->count() == 0) {
            initHashAndAttackModes();
        }
        commandChanged();
    }
}

// Tools → Load Example Session → <attack mode>
void MainWindow::loadExampleSession(AttackMode mode)
{
    const Examples::Entry *entry = nullptr;
    for (const Examples::Entry &candidate : Examples::All) {
        if (candidate.attackMode == mode) {
            entry = &candidate;
            break;
        }
    }
    if (!entry) {
        return;
    }

    // Loading the example replaces the session the user has set up in the
    // form, so it is only carried out after the user confirmed it.
    const auto answer = QMessageBox::question(
        this, tr("Load example session"),
        tr("Load the %1 example? The current session is lost.\n\n"
           "The example cracks a test hash; hashcat should recover the password %2.")
            .arg(attackModeName(mode), QString(entry->password)),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (answer != QMessageBox::Yes) {
        return;
    }

    // The examples are compiled into this build, so their format version
    // cannot be newer than this one; the test keeps the resources current.
    QString error;
    QJsonObject values;
    int version = 0;
    QStringList otherSections;
    if (!StateFile::read(QString(entry->resource), AppConstants::FileSections::Session, &values,
                         &version, &otherSections, &error)) {
        QMessageBox::warning(this, tr("Load example session"), error);
        return;
    }

    // The wordlists, rules and tables of an example are resources too, and
    // hashcat can only read real files: unpack them first, then point the
    // session at that directory.
    if (containsExamplePrefix(values)) {
        QString directory;
        if (!materializeExampleData(&directory, &error)) {
            QMessageBox::warning(this, tr("Load example session"), error);
            return;
        }
        values = withExamplePaths(values, directory).toObject();
    }

    const QStringList notes = applySessionFile(values, version, otherSections);
    if (!notes.isEmpty()) {
        QMessageBox::warning(this, tr("Load example session"), notes.join(QLatin1Char('\n')));
    }
}

// Writes every bundled data file to the example directory, replacing the
// copy a previous load left there. The whole set is written, not only the
// files the selected session names: the files are tiny, and an example must
// never depend on what another one unpacked before it.
bool MainWindow::materializeExampleData(QString *directory, QString *error)
{
    const QString target = QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))
                               .filePath(QString(Examples::DataDirectory));
    if (!QDir().mkpath(target)) {
        *error = tr("Could not create the example directory “%1”.").arg(target);
        return false;
    }

    for (const QLatin1StringView &resource : Examples::DataFiles) {
        QFile source{QString(resource)};
        if (!source.open(QIODevice::ReadOnly)) {
            *error = tr("Could not read the bundled file “%1”: %2")
                         .arg(QString(resource), source.errorString());
            return false;
        }

        QSaveFile destination{QDir(target).filePath(QFileInfo(QString(resource)).fileName())};
        if (!destination.open(QIODevice::WriteOnly)) {
            *error = tr("Could not write “%1”: %2")
                         .arg(destination.fileName(), destination.errorString());
            return false;
        }

        const QByteArray data = source.readAll();
        if (destination.write(data) != data.size() || !destination.commit()) {
            *error = tr("Could not write “%1”: %2")
                         .arg(destination.fileName(), destination.errorString());
            return false;
        }
    }

    QString prefix = QDir::toNativeSeparators(target);
    if (!prefix.endsWith(QDir::separator())) {
        prefix += QDir::separator();
    }
    *directory = std::move(prefix);
    return true;
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

    // Attack tab
    ui->lineEdit_hashfile->clear();
    ui->lineEdit_hash_recipe->clear();
    ui->checkBox_ignoreusername->setChecked(false);
    ui->checkBox_remove->setChecked(false);
    ui->listWidget_wordlist->clear();
    ui->checkBox_hex_wordlist->setChecked(false);
    ui->lineEdit_encoding_from->clear();
    ui->lineEdit_encoding_to->clear();
    ui->comboBox_attack->setCurrentIndex(0);
    ui->comboBox_hash_type->setCurrentIndex(0);
    // A fresh form must not re-apply a hash type a session is still waiting
    // to deliver.
    m_pendingHashTypeId = -1;
    ui->radioButton_use_rules_file->setChecked(true);
    ui->checkBox_rulesfile_1->setChecked(false);
    ui->checkBox_rulesfile_2->setChecked(false);
    ui->checkBox_rulesfile_3->setChecked(false);
    ui->checkBox_rules_concat->setChecked(false);
    ui->lineEdit_rulesfile_1->clear();
    ui->lineEdit_rulesfile_2->clear();
    ui->lineEdit_rulesfile_3->clear();
    ui->lineEdit_rule_left->clear();
    ui->lineEdit_rule_right->clear();
    ui->lineEdit_association_settings->clear();
    ui->listWidget_pcfg_rulesets->clear();
    ui->lineEdit_pcfg_scale->clear();
    ui->lineEdit_pcfg_costmax->clear();
    ui->lineEdit_pcfg_weights->clear();
    ui->lineEdit_pcfg_threads->clear();
    ui->lineEdit_pcfg_kbits->clear();
    ui->lineEdit_pcfg_maxword->clear();
    ui->lineEdit_pcfg_maxgain->clear();
    ui->lineEdit_pcfg_walk->clear();
    ui->lineEdit_pcfg_mask->clear();
    ui->lineEdit_pcfg_extra->clear();
    ui->listWidget_tables->clear();
    ui->lineEdit_table_identity->clear();
    ui->lineEdit_table_template->clear();
    ui->lineEdit_table_single->clear();
    ui->lineEdit_table_cap->clear();
    ui->lineEdit_table_maxperm->clear();
    ui->lineEdit_table_extra->clear();
    ui->lineEdit_feed->clear();
    ui->lineEdit_feed_argument->clear();
    ui->listWidget_feed_arguments->clear();
    ui->spinBox_generate_rules->setValue(1);
    ui->lineEdit_mask->clear();
    ui->checkBox_increment->setChecked(false);
    ui->spinBox_increment_min->setValue(AppConstants::Defaults::IncrementMin);
    ui->spinBox_increment_max->setValue(AppConstants::Defaults::IncrementMax);
    ui->checkBox_custom_charset1->setChecked(false);
    ui->lineEdit_custom_charset1->clear();
    ui->checkBox_custom_charset2->setChecked(false);
    ui->lineEdit_custom_charset2->clear();
    ui->checkBox_custom_charset3->setChecked(false);
    ui->lineEdit_custom_charset3->clear();
    ui->checkBox_custom_charset4->setChecked(false);
    ui->lineEdit_custom_charset4->clear();
    ui->checkBox_hex_charset->setChecked(false);
    ui->checkBox_hex_salt->setChecked(false);

    // General tab
    ui->checkBox_outfile->setChecked(false);
    ui->lineEdit_outfile->clear();
    ui->lineEdit_outfile_format->setText(AppConstants::Defaults::OutfileFormat);
    ui->checkBox_outfile_json->setChecked(false);
    ui->lineEdit_cache_path->clear();
    ui->lineEdit_cpu_affinity->clear();
    ui->lineEdit_backend_devices->setText(AppConstants::Defaults::BackendDevices);

    ui->lineEdit_session->clear();
    ui->spinBox_runtime->setValue(AppConstants::Defaults::Runtime);
    ui->lineEdit_skip->clear();
    ui->lineEdit_limit->clear();
    ui->checkBox_status->setChecked(false);
    ui->spinBox_status_timer->setValue(AppConstants::Defaults::StatusTimer);
    ui->checkBox_force->setChecked(false);
    ui->checkBox_potfile_disable->setChecked(false);
    ui->checkBox_optimized_kernel->setChecked(false);
    ui->checkBox_speed_only->setChecked(false);
    ui->lineEdit_opencl_device_types->clear();
    ui->checkBox_hwmon_temp_abort->setChecked(false);
    ui->spinBox_hwmon_temp_abort->setValue(AppConstants::Defaults::HwmonTempAbort);

    // The attack combo points at index 0 again, so the mode-specific groups
    // hide; the pass also refreshes the hint and the dependencies. The recipe
    // row is refreshed separately: the combo may already have been at index 0,
    // in which case setCurrentIndex() emits no signal.
    updateHashTypeDependencies();
    updateViewAttackMode();
}

// File → Settings
void MainWindow::settingsTriggered()
{
    SettingsDialog settingsDialog(this);
    if (settingsDialog.exec() == QDialog::Accepted) {
        // Re-run the query when the binary changed: its list replaces the one
        // on screen - which resets the hash type selection, expected because
        // the new binary may not offer the selected id - or when nothing was
        // ever loaded and the path may be usable now.
        const QString hashcatPath =
            SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
        if (hashcatPath != m_queriedHashcatPath || ui->comboBox_hash_type->count() == 0) {
            initHashAndAttackModes();
        }
        // The settings can change the binary name and the short/long parameter
        // choice, both of which are part of the preview.
        commandChanged();
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

// Load the session – called from the constructor
void MainWindow::loadSession()
{
    if (!restoreSessionEnabled()) {
        return;
    }

    const QString file = sessionFile();
    if (!QFile::exists(file)) {
        return;
    }

    QString error;
    QJsonObject values;
    int version = 0;
    QStringList otherSections;
    if (!StateFile::read(file, AppConstants::FileSections::Session, &values, &version,
                         &otherSections, &error)) {
        // A file this version cannot read is not thrown away: it may belong
        // to a version that can read it, or the user may want to look at it.
        // The start goes on without the session.
        QMessageBox::warning(
            this, tr("Session warning"),
            tr("The session file could not be read, so the session was not restored. The file "
               "was left in place.\n\n%1")
                .arg(error));
        return;
    }

    // A session with a newer format version is left in place: it is what that
    // version saved, and the user may go back to it.
    if (version > AppConstants::Sessions::Version) {
        QMessageBox::warning(this, tr("Session warning"), newerSessionNote(version));
        return;
    }

    // The auto-load is not an import: there is no headline, and nothing is
    // shown when the load went well.
    const QStringList notes = applySessionFile(values, version, otherSections);

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
    const QJsonObject values =
        WidgetStateSerializer::toJson(this, {AppConstants::Files::PreviewWidget});
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
    QString error;
    QJsonObject values;
    int version = 0;
    QStringList otherSections;
    if (!StateFile::read(file, AppConstants::FileSections::Session, &values, &version,
                         &otherSections, &error)) {
        return false;
    }

    return version <= AppConstants::Sessions::Version;
}

QString MainWindow::attackModeName(AttackMode mode) const
{
    switch (mode) {
    case AttackMode::Straight:
        return tr("Straight");
    case AttackMode::Combination:
        return tr("Combination");
    case AttackMode::BruteForce:
        return tr("Brute-force");
    case AttackMode::Pcfg:
        return tr("PCFG");
    case AttackMode::Table:
        return tr("Table");
    case AttackMode::HybridWordMask:
        return tr("Hybrid Wordlist + Mask");
    case AttackMode::HybridMaskWord:
        return tr("Hybrid Mask + Wordlist");
    case AttackMode::Generic:
        return tr("Generic (feed)");
    case AttackMode::Association:
        return tr("Association");
    case AttackMode::Hybrid:
        return tr("Hybrid (mask + ?w/?q)");
    }
    return {};
}

void MainWindow::initExampleSessionMenu()
{
    for (const Examples::Entry &entry : Examples::All) {
        QAction *action = ui->menuLoadExampleSession->addAction(attackModeName(entry.attackMode));
        action->setData(static_cast<int>(entry.attackMode));
        connect(action, &QAction::triggered, this,
                [this, mode = entry.attackMode] { loadExampleSession(mode); });
    }
}

void MainWindow::initHashAndAttackModes()
{
    ui->comboBox_attack->clear();
    ui->comboBox_hash_type->clear();

    // Attack modes. The numeric id travels as item data: the displayed text
    // is what a translation changes, the id is what has to end up on the
    // command line.
    for (const AttackMode mode : AllAttackModes) {
        ui->comboBox_attack->addItem(attackModeName(mode), static_cast<int>(mode));
    }

    // Hash types
    auto &settings = SettingsManager::instance();
    const QString hashcatPath = settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath);

    // Remember which binary this list comes from. An empty path records
    // "nothing queried" just as well, so the first configured path counts as
    // a change too - see settingsTriggered().
    m_queriedHashcatPath = hashcatPath;

    if (!hashcatPath.isEmpty()) {
        ui->comboBox_hash_type->setToolTip(tr("Updating…"));
        ui->comboBox_hash_type->setEnabled(false);

        // Create a watcher that will be destroyed once finished
        QFutureWatcher<HashcatResult> *watcher = new QFutureWatcher<HashcatResult>(this);

        // When the future is finished, fill the combo with the parsed types.
        connect(watcher, &QFutureWatcher<HashcatResult>::finished, this, [this, watcher]() {
            const HashcatResult &result = watcher->result();
            watcher->deleteLater();

            QString error;
            QString hashTypeProblem;

            // Check if the command failed. Exit codes changed in hashcat
            // 8.0.0 (-2 is now a wrong attack command, -12 an unusable file,
            // -13 a bridge failure, all of them -1 before), but this only
            // wants "did it work", and the GUI has no per-cause message: a
            // reader must not turn this into per-code handling.
            if (result.exitStatus != QProcess::NormalExit || result.exitCode != 0) {
                error = tr("Failed to obtain supported hash types.\nError: %1")
                            .arg(result.standardError);
            } else {
                QMap<quint32, QString> parsed;
                if (HashcatInfoParser::parseExampleHashes(result.standardOutput, parsed, &error)) {
                    ui->comboBox_hash_type->clear();

                    // fill the combobox, hash type id as item data
                    for (auto it = parsed.constBegin(); it != parsed.constEnd(); ++it) {
                        ui->comboBox_hash_type->addItem(it.value(), it.key());
                    }

                    // A session loaded while the combo was still empty could
                    // not select its hash type; apply the stored id now that
                    // the items exist, and consume it.
                    hashTypeProblem = applyPendingHashType();
                }
            }

            // The combo was disabled while the query ran. Restore it on every
            // path so a failed or malformed reply does not leave it stuck on
            // the "Updating…" tooltip forever.
            ui->comboBox_hash_type->setEnabled(true);
            ui->comboBox_hash_type->setToolTip(error.isEmpty()
                                                   ? QString()
                                                   : tr("The hash types could not be loaded. Check "
                                                        "the hashcat path in Settings."));

            if (!error.isEmpty()) {
                QMessageBox::warning(this, tr("Failed to load hash types"), error);
            } else if (!hashTypeProblem.isEmpty()) {
                QMessageBox::warning(this, tr("Session warning"), hashTypeProblem);
            }
        });

        // Kick off the asynchronous process
        watcher->setFuture(HelperUtils::executeHashcat(QStringList()
                                                       << AppConstants::Hashcat::ExampleHashes
                                                       << AppConstants::Hashcat::MachineReadable));
    } else {
        ui->comboBox_hash_type->setToolTip(
            tr("Set the hashcat path under %1 → %2 to load the available hash types.")
                .arg(menuPath(ui->menuFile->menuAction()->text()),
                     menuPath(ui->actionSettings->text())));
    }
}

void MainWindow::attackIndexChanged([[maybe_unused]] int index)
{
    updateViewAttackMode();
}

void MainWindow::updateViewAttackMode()
{
    applyAttackModeView(currentAttackMode());
    updateOptionDependencies();
    commandChanged();
}

// Shows the mode hint and the groups the mode uses, and hides the rest. The
// very predicates CommandBuilder applies are used, so the widgets show
// exactly the options that end up in the command line and the two can never
// drift apart. Groups are hidden rather than disabled so the form only shows
// the options the mode uses; the window keeps room for the busiest mode
// anyway, see attackModeEnvelope(). Fitting the window measures this once per
// mode, so the hint text has to be here as well - it sets the width the
// window has to hold.
void MainWindow::applyAttackModeView(AttackMode mode)
{
    const bool usesMask = CommandBuilder::attackUsesMask(mode);
    const bool usesRuleLeft = CommandBuilder::attackUsesRuleLeft(mode);
    const bool usesRuleRight = CommandBuilder::attackUsesRuleRight(mode);
    const bool usesIncrement = CommandBuilder::attackUsesIncrement(mode);
    ui->groupBox_wordlists->setVisible(CommandBuilder::attackUsesWordlists(mode));
    ui->groupBox_rules->setVisible(CommandBuilder::attackUsesRules(mode));
    // The group starts out disabled in the .ui file, so it keeps the
    // enabled gate next to the visibility one: it is visible only while one
    // of the two sides applies, and enabled exactly then.
    ui->groupBox_wordlist_rules->setVisible(usesRuleLeft || usesRuleRight);
    ui->groupBox_wordlist_rules->setEnabled(usesRuleLeft || usesRuleRight);
    ui->label_rule_left->setEnabled(usesRuleLeft);
    ui->lineEdit_rule_left->setEnabled(usesRuleLeft);
    ui->label_rule_right->setEnabled(usesRuleRight);
    ui->lineEdit_rule_right->setEnabled(usesRuleRight);
    ui->groupBox_custom_charset->setVisible(CommandBuilder::attackUsesCustomCharset(mode));
    ui->groupBox_mask->setVisible(usesMask);

    // The feed modes get a group each, and mode 9 one extra row: the
    // settings line that bounds a run whose words come from the hash file.
    ui->groupBox_pcfg->setVisible(mode == AttackMode::Pcfg);
    ui->groupBox_table->setVisible(mode == AttackMode::Table);
    ui->groupBox_feed->setVisible(mode == AttackMode::Generic);
    ui->label_association_settings->setVisible(mode == AttackMode::Association);
    ui->lineEdit_association_settings->setVisible(mode == AttackMode::Association);

    // Inside the wordlist group: only the modes that read a wordlist take
    // the wordlist options, and only the ones hashcat lets name an
    // encoding.
    ui->checkBox_hex_wordlist->setVisible(CommandBuilder::attackUsesHexWordlist(mode));
    const bool usesEncoding = CommandBuilder::attackUsesWordlistEncoding(mode);
    ui->label_encoding_from->setVisible(usesEncoding);
    ui->lineEdit_encoding_from->setVisible(usesEncoding);
    ui->label_encoding_to->setVisible(usesEncoding);
    ui->lineEdit_encoding_to->setVisible(usesEncoding);

    // Inside the mask group: hashcat 8.0.0 refuses -i in mode 12, so the
    // increment row only exists where the flag is offered.
    ui->checkBox_increment->setVisible(usesIncrement);
    ui->label_increment_min->setVisible(usesIncrement);
    ui->spinBox_increment_min->setVisible(usesIncrement);
    ui->label_increment_max->setVisible(usesIncrement);
    ui->spinBox_increment_max->setVisible(usesIncrement);

    // Tell the user what the mode expects, so the launch validation does not
    // have to be the first thing that explains it.
    ui->label_mode_hint->setText(modeHint(mode));
}

// The attack mode the user selected.
AttackMode MainWindow::currentAttackMode() const
{
    return static_cast<AttackMode>(ui->comboBox_attack->currentData().toInt());
}

// What the user has to provide for the selected attack mode.
//
// The wording follows the requirements in CommandBuilder::modeRequirements(), so
// the hint and the launch validation cannot tell the user different things.
QString MainWindow::modeHint(AttackMode mode) const
{
    // A sentence per mode. The label wraps at the tab width (wordWrap in the
    // .ui file), so the texts stay free of line breaks and translations can
    // rewrap as they like. The height a wrapped hint needs is not part of a
    // minimum size hint - a wrapping label reports one line - so the fit
    // reserves it through heightForWidth() at the final width,
    // see attackModeEnvelope() and fitToContent().
    switch (mode) {
    case AttackMode::Straight:
        return tr("This mode needs at least one wordlist.");
    case AttackMode::Combination:
        return tr("This mode needs exactly 2 wordlists (left, right).");
    case AttackMode::BruteForce:
        return tr("This mode needs a mask.");
    case AttackMode::Pcfg:
        return tr("This mode runs hashcat's bundled default-passwords ruleset; "
                  "name one or more to use your own.");
    case AttackMode::Table:
        return tr("This mode needs one wordlist and at least one table.");
    case AttackMode::HybridWordMask:
    case AttackMode::HybridMaskWord:
        return tr("This mode needs a wordlist and a mask.");
    case AttackMode::Generic:
        return tr("This mode needs a feed. Arguments depend on the feed — "
                  "random and stdin need none, wordlist needs a path.");
    case AttackMode::Association:
        // The settings grammar (phases, the never-ending default) is the
        // settings line's own tooltip; the hint keeps only what the mode
        // needs, in the wording of modeRequirements().
        return tr("This mode needs at most one wordlist. Without one the words come from the "
                  "hash file and the settings line can bound the run; with one the two files "
                  "are paired by line.");
    case AttackMode::Hybrid:
        return tr("This mode needs a mask with exactly one ?w; a ?q after ?w "
                  "names a second wordlist.");
    }
    return QString();
}

// Options that only carry a value while their parent checkbox is on. The
// group box gating already covers the attack-mode side; this covers the
// intra-group dependencies that do not follow from the mode.
void MainWindow::updateOptionDependencies()
{
    const bool increment = ui->checkBox_increment->isChecked();
    ui->label_increment_min->setEnabled(increment);
    ui->spinBox_increment_min->setEnabled(increment);
    ui->label_increment_max->setEnabled(increment);
    ui->spinBox_increment_max->setEnabled(increment);

    ui->spinBox_status_timer->setEnabled(ui->checkBox_status->isChecked());
    ui->spinBox_hwmon_temp_abort->setEnabled(ui->checkBox_hwmon_temp_abort->isChecked());
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
    removeSelectedRows(ui->listWidget_wordlist);
    ui->pushButton_remove_wordlist->setEnabled(false);
    ui->toolButton_wordlist_move_up->setEnabled(false);
    ui->toolButton_wordlist_move_down->setEnabled(false);
}

void MainWindow::addPathsToList(QListWidget *list, const QStringList &paths)
{
    // The dialog hands out '/' separated paths, which are stored in the
    // platform's own form so the rows look like the paths the user works with
    // everywhere else. A path that is already a row is not added twice.
    for (const QString &path : paths) {
        const QString native = QDir::toNativeSeparators(path);
        if (native.isEmpty() || !list->findItems(native, Qt::MatchExactly).isEmpty()) {
            continue;
        }

        auto *item = new QListWidgetItem(native, list);
        item->setCheckState(Qt::Checked);
    }
}

void MainWindow::addWordlistClicked()
{
    addPathsToList(ui->listWidget_wordlist,
                   QFileDialog::getOpenFileNames(this, tr("Add wordlists")));
}

void MainWindow::addWordlistFolderClicked()
{
    const QString folder = QFileDialog::getExistingDirectory(this, tr("Add wordlist folder"));
    if (!folder.isEmpty()) {
        addPathsToList(ui->listWidget_wordlist, {folder});
    }
}

QStringList MainWindow::checkedItems(const QListWidget *list)
{
    QStringList items;
    items.reserve(list->count());
    for (int i = 0; i < list->count(); ++i) {
        const QListWidgetItem *item = list->item(i);
        if (item->checkState() == Qt::Checked) {
            items << item->text();
        }
    }
    return items;
}

void MainWindow::removeSelectedRows(QListWidget *list)
{
    // The rows know their list and take themselves out of it when deleted.
    const QList<QListWidgetItem *> selected = list->selectedItems();
    for (QListWidgetItem *item : selected) {
        delete item;
    }
    list->clearSelection();
}

void MainWindow::moveSelectedRow(QListWidget *list, int delta)
{
    // currentRow() is -1 when nothing is selected: without the guards below
    // takeItem(-1) returns nullptr and the list ends up with a null entry.
    const int currentRow = list->currentRow();
    const int targetRow = currentRow + delta;
    if (currentRow < 0 || targetRow < 0 || targetRow >= list->count()) {
        return;
    }
    QListWidgetItem *item = list->takeItem(currentRow);
    list->insertItem(targetRow, item);
    list->setCurrentRow(targetRow);
}

void MainWindow::addPcfgRulesetClicked()
{
    addPathsToList(ui->listWidget_pcfg_rulesets,
                   QFileDialog::getOpenFileNames(this, tr("Add rulesets")));
}

void MainWindow::addPcfgRulesetFolderClicked()
{
    const QString folder = QFileDialog::getExistingDirectory(this, tr("Add ruleset folder"));
    if (!folder.isEmpty()) {
        addPathsToList(ui->listWidget_pcfg_rulesets, {folder});
    }
}

void MainWindow::removePcfgRulesetClicked()
{
    removeSelectedRows(ui->listWidget_pcfg_rulesets);
}

void MainWindow::pcfgRulesetMoveUpClicked()
{
    moveSelectedRow(ui->listWidget_pcfg_rulesets, -1);
}

void MainWindow::pcfgRulesetMoveDownClicked()
{
    moveSelectedRow(ui->listWidget_pcfg_rulesets, 1);
}

void MainWindow::addTableClicked()
{
    addPathsToList(ui->listWidget_tables, QFileDialog::getOpenFileNames(this, tr("Add tables")));
}

void MainWindow::removeTableClicked()
{
    removeSelectedRows(ui->listWidget_tables);
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
    removeSelectedRows(ui->listWidget_feed_arguments);
}

void MainWindow::feedArgumentMoveUpClicked()
{
    moveSelectedRow(ui->listWidget_feed_arguments, -1);
}

void MainWindow::feedArgumentMoveDownClicked()
{
    moveSelectedRow(ui->listWidget_feed_arguments, 1);
}

void MainWindow::wordlistMoveUpClicked()
{
    moveSelectedRow(ui->listWidget_wordlist, -1);
}

void MainWindow::wordlistMoveDownClicked()
{
    moveSelectedRow(ui->listWidget_wordlist, 1);
}

void MainWindow::wordlistItemClicked([[maybe_unused]] QListWidgetItem *item)
{
    ui->pushButton_remove_wordlist->setEnabled(true);
    ui->toolButton_wordlist_move_up->setEnabled(true);
    ui->toolButton_wordlist_move_down->setEnabled(true);
}

// The rows are the source of the watched paths: a path arrives through the file
// dialog, a drop, a session or a reset, and all of them change the model.
void MainWindow::refreshMonitor(QListWidget *list, PathMonitor &monitor)
{
    QStringList paths;
    paths.reserve(list->count());
    for (int i = 0; i < list->count(); ++i) {
        paths << list->item(i)->text();
    }
    monitor.setPaths(paths);
}

// Why a row hashcat cannot use is unusable, in the terms of its list. The path
// may contain markup-like text, so the result is escaped for the rich text
// tooltip.
QString MainWindow::unavailableTooltip(PathMonitor::Kind kind, const QString &path)
{
    const bool folder = QFileInfo(path).isDir();

    switch (kind) {
    case PathMonitor::Kind::Wordlist:
        if (folder) {
            return escapeRichText(tr("This folder has no wordlists. hashcat reads the regular "
                                     "files directly inside it, not subfolders."));
        }
        return escapeRichText(tr("This wordlist does not exist:\n%1").arg(path));
    case PathMonitor::Kind::Pcfg:
        if (folder) {
            return escapeRichText(
                tr("This ruleset folder has no Grammar/grammar.txt:\n%1").arg(path));
        }
        return escapeRichText(tr("This ruleset does not exist:\n%1").arg(path));
    case PathMonitor::Kind::Table:
        if (folder) {
            return escapeRichText(
                tr("A table must be a file; hashcat does not read a folder:\n%1").arg(path));
        }
        return escapeRichText(tr("This table does not exist:\n%1").arg(path));
    }

    return {};
}

void MainWindow::setRowAvailability(QListWidget *list, PathMonitor::Kind kind, const QString &path,
                                    bool available)
{
    // Writing an icon or a tooltip is an item change, which for every list
    // rebuilds the preview. The blocker keeps that out: the decoration is not
    // part of the command.
    const QSignalBlocker blocker(list);

    // A usable path shows what it is; an unusable one cannot be trusted with
    // a type, so the warning takes the icon's place. A bundled ruleset name has
    // neither a file nor a folder behind it and stays without one.
    const QFileInfo info(path);
    QIcon icon;
    if (!available) {
        icon = m_warningIcon;
    } else if (info.isDir()) {
        icon = m_folderIcon;
    } else if (info.isFile()) {
        icon = m_fileIcon;
    }

    const QString tooltip = available ? QString() : unavailableTooltip(kind, path);

    for (int i = 0; i < list->count(); ++i) {
        QListWidgetItem *item = list->item(i);
        if (item->text() != path) {
            continue;
        }

        if (item->icon().cacheKey() != icon.cacheKey()) {
            item->setIcon(icon);
        }
        if (item->toolTip() != tooltip) {
            item->setToolTip(tooltip);
        }
    }
}

void MainWindow::refreshWordlistMonitor()
{
    refreshMonitor(ui->listWidget_wordlist, m_wordlistMonitor);
}

void MainWindow::refreshPcfgMonitor()
{
    refreshMonitor(ui->listWidget_pcfg_rulesets, m_pcfgMonitor);
}

void MainWindow::refreshTableMonitor()
{
    refreshMonitor(ui->listWidget_tables, m_tableMonitor);
}

void MainWindow::wordlistAvailabilityChanged(const QString &path, bool available)
{
    setRowAvailability(ui->listWidget_wordlist, PathMonitor::Kind::Wordlist, path, available);
}

void MainWindow::pcfgAvailabilityChanged(const QString &path, bool available)
{
    setRowAvailability(ui->listWidget_pcfg_rulesets, PathMonitor::Kind::Pcfg, path, available);
}

void MainWindow::tableAvailabilityChanged(const QString &path, bool available)
{
    setRowAvailability(ui->listWidget_tables, PathMonitor::Kind::Table, path, available);
}

void MainWindow::rulesFile1Toggled(bool checked)
{
    ui->lineEdit_rulesfile_1->setEnabled(checked);
    ui->pushButton_open_rulesfile_1->setEnabled(checked);
}

void MainWindow::rulesFile2Toggled(bool checked)
{
    ui->lineEdit_rulesfile_2->setEnabled(checked);
    ui->pushButton_open_rulesfile_2->setEnabled(checked);
}

void MainWindow::rulesFile3Toggled(bool checked)
{
    ui->lineEdit_rulesfile_3->setEnabled(checked);
    ui->pushButton_open_rulesfile_3->setEnabled(checked);
}

void MainWindow::useRulesFileToggled(bool checked)
{
    ui->checkBox_rulesfile_1->setEnabled(checked);
    ui->checkBox_rulesfile_2->setEnabled(checked);
    ui->checkBox_rulesfile_3->setEnabled(checked);
    // The join flag is only meaningful next to the files it joins; the
    // generated-rules radio is the other half of the same choice.
    ui->checkBox_rules_concat->setEnabled(checked);
    if (checked) {
        rulesFile1Toggled(ui->checkBox_rulesfile_1->isChecked());
        rulesFile2Toggled(ui->checkBox_rulesfile_2->isChecked());
        rulesFile3Toggled(ui->checkBox_rulesfile_3->isChecked());
    }
}

void MainWindow::generateRulesToggled(bool checked)
{
    ui->spinBox_generate_rules->setEnabled(checked);
    if (checked) {
        useRulesFileToggled(false);
        rulesFile1Toggled(false);
        rulesFile2Toggled(false);
        rulesFile3Toggled(false);
    }
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

void MainWindow::customCharset1Toggled(bool checked)
{
    ui->lineEdit_custom_charset1->setEnabled(checked);
}

void MainWindow::customCharset2Toggled(bool checked)
{
    ui->lineEdit_custom_charset2->setEnabled(checked);
}

void MainWindow::customCharset3Toggled(bool checked)
{
    ui->lineEdit_custom_charset3->setEnabled(checked);
}

void MainWindow::customCharset4Toggled(bool checked)
{
    ui->lineEdit_custom_charset4->setEnabled(checked);
}

void MainWindow::outfileToggled(bool checked)
{
    ui->lineEdit_outfile->setEnabled(checked);
    ui->pushButton_open_outfile->setEnabled(checked);
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
    // What the selected attack mode needs comes first: it is what the user
    // has just configured, and hashcat would only answer it with a usage
    // error after the run has started.
    QString detail;
    const HelperUtils::LaunchError optionError =
        HelperUtils::validateOptions(collectHashcatOptions(), &detail);
    if (optionError != HelperUtils::LaunchError::None) {
        showLaunchError(optionError, detail);
        return;
    }

    auto &settings = SettingsManager::instance();

    const QString hashcatPath = settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    const QString configuredTerminal =
        settings.getKey<QString>(AppConstants::SettingsKeys::Terminal);
    const QMap<QString, QStringList> availableTerminals = HelperUtils::availableTerminals();

    // Nothing is started unless the configuration is known to be complete.
    const HelperUtils::LaunchError error =
        HelperUtils::validateLaunch(ui->lineEdit_hashfile->text(), hashcatPath, configuredTerminal,
                                    availableTerminals.keys(), &detail);
    if (error != HelperUtils::LaunchError::None) {
        showLaunchError(error, detail);
        return;
    }

    QProcess proc;

    /* 1. arguments needed for the selected terminal, 2. the hashcat binary,
     * 3. the arguments collected from the gui elements */
    QStringList arguments = availableTerminals.value(configuredTerminal);
    arguments << hashcatPath << generateArguments();

#if defined(Q_OS_WIN)
    /* Need CREATE_NEW_CONSOLE flag on windows to spawn visible terminal */
    proc.setCreateProcessArgumentsModifier(
        [](QProcess::CreateProcessArguments *args) { args->flags |= CREATE_NEW_CONSOLE; });
#endif

    proc.setProgram(configuredTerminal);
    proc.setArguments(arguments);
    proc.setWorkingDirectory(QFileInfo(hashcatPath).absolutePath());

    if (!proc.startDetached()) {
        QMessageBox::warning(
            this, tr("Launch failed"),
            tr("Could not start “%1”: %2").arg(configuredTerminal, proc.errorString()));
    }
}

void MainWindow::showLaunchError(HelperUtils::LaunchError error, const QString &detail)
{
    const QString menu = menuPath(ui->menuFile->menuAction()->text());
    const QString settingsEntry = menuPath(ui->actionSettings->text());
    const QString configuredTerminal =
        SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::Terminal);

    QMessageBox box(this);
    box.setIcon(QMessageBox::Warning);

    switch (error) {
    case HelperUtils::LaunchError::NoHashFile:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("Please choose a hash file."));
        break;

    case HelperUtils::LaunchError::NoHashcatPath:
        box.setIcon(QMessageBox::Information);
        box.setTextFormat(Qt::RichText);
        box.setText(
            tr("Navigate to <b>%1 → %2</b> to configure the path to the hashcat executable.")
                .arg(menu, settingsEntry));
        break;

    // The two messages below interpolate user supplied strings, so they are
    // set to plain text explicitly: the default is AutoText, which would take
    // a path or a terminal name containing a '<' for markup and display it
    // mangled.
    case HelperUtils::LaunchError::HashcatPathMissing:
        box.setTextFormat(Qt::PlainText);
        box.setText(tr("The configured hashcat executable does not exist:\n%1\n"
                       "Navigate to %2 → %3 to change it.")
                        .arg(detail, menu, settingsEntry));
        break;

    case HelperUtils::LaunchError::NoTerminal:
        box.setIcon(QMessageBox::Information);
        box.setTextFormat(Qt::RichText);
        box.setText(tr("Navigate to <b>%1 → %2</b> to select the terminal used for launching.")
                        .arg(menu, settingsEntry));
        break;

    case HelperUtils::LaunchError::NoTerminals:
        box.setText(HelperUtils::noSupportedTerminalMessage());
        break;

    case HelperUtils::LaunchError::UnknownTerminal:
        box.setTextFormat(Qt::PlainText);
        box.setText(tr("The configured terminal “%1” is not available. Available terminals: %2\n"
                       "Navigate to %3 → %4 to change it.")
                        .arg(configuredTerminal, detail, menu, settingsEntry));
        break;

    // The mode specific requirements. They are not misconfigurations of the
    // installation but of the attack itself, so the message says what the
    // mode needs instead of pointing at the settings.
    case HelperUtils::LaunchError::NoWordlist:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("This mode needs at least one wordlist."));
        break;

    case HelperUtils::LaunchError::WrongWordlistCount: {
        // The wording below uses minWordlists as the exact count. That is only
        // correct while Combination is the sole mode with maxWordlists >= 0,
        // and there min == max; a mode with a range reports TooManyWordlists.
        const CommandBuilder::ModeRequirements requirements =
            CommandBuilder::modeRequirements(currentAttackMode());
        box.setIcon(QMessageBox::Information);
        box.setText(tr("This mode needs exactly %1 wordlists (left, right).\n"
                       "Currently selected: %2.")
                        .arg(requirements.minWordlists)
                        .arg(detail));
        break;
    }

    case HelperUtils::LaunchError::TooManyWordlists: {
        const CommandBuilder::ModeRequirements requirements =
            CommandBuilder::modeRequirements(currentAttackMode());
        box.setIcon(QMessageBox::Information);
        QString text = tr("This mode takes at most %1 wordlists.\nCurrently selected: %2.")
                           .arg(requirements.maxWordlists)
                           .arg(detail);
        if (currentAttackMode() == AttackMode::Association) {
            text += QLatin1Char('\n')
                    + tr("Hashcat pairs the two files by line here; this GUI offers "
                         "only one wordlist for that role.");
        }
        box.setText(text);
        break;
    }

    // The row is user supplied, so plain text: a '<' in the path would be
    // taken for markup by the default AutoText.
    case HelperUtils::LaunchError::WordlistMustBeFile:
        box.setTextFormat(Qt::PlainText);
        box.setText(tr("This wordlist has to be a file: hashcat reads it as the multiplied "
                       "wordlist (the ?q side), and a folder cannot be used there:\n%1")
                        .arg(detail));
        break;

    case HelperUtils::LaunchError::NoMask:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("This mode needs a mask."));
        break;

    case HelperUtils::LaunchError::NoTable:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("This mode needs at least one table."));
        break;

    case HelperUtils::LaunchError::NoFeed:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("This mode needs a feed: a bundled name (wordlist, stdin, random, …) "
                       "or the path of a feed file."));
        break;

    case HelperUtils::LaunchError::MaskNeedsWordMarker:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("This mode pairs the wordlists with the mask; the mask needs a ?w "
                       "for the word."));
        break;

    case HelperUtils::LaunchError::MaskMultipleWordMarkers:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("The mask contains more than one ?w."));
        break;

    case HelperUtils::LaunchError::MaskQBeforeWord:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("The mask contains a ?q before the first ?w."));
        break;

    case HelperUtils::LaunchError::MaskMultipleQMarks:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("The mask contains more than one ?q."));
        break;

    case HelperUtils::LaunchError::MaskQWithoutWordlist:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("The mask names a second word with ?q, but only one wordlist "
                       "is selected."));
        break;

    case HelperUtils::LaunchError::SecondWordlistWithoutQ:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("Two wordlists are selected, but the mask has no ?q for the "
                       "second one."));
        break;

    case HelperUtils::LaunchError::IncrementInMode12:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("The increment is not offered in this hybrid mode."));
        break;

    case HelperUtils::LaunchError::MaskMarkersNotAllowed:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("The ?w and ?q markers are only offered in the hybrid mode "
                       "(mask + wordlists)."));
        break;

    // The three settings errors name a value or token the user typed, so
    // they are set to plain text explicitly: a value containing '<' would
    // be taken for markup by the default AutoText.
    case HelperUtils::LaunchError::InvalidPcfgSetting:
    case HelperUtils::LaunchError::InvalidTableSetting:
        box.setIcon(QMessageBox::Information);
        box.setTextFormat(Qt::PlainText);
        box.setText(tr("The setting “%1” is not valid for this mode.").arg(detail));
        break;

    case HelperUtils::LaunchError::InvalidAssociationSetting:
        box.setIcon(QMessageBox::Information);
        box.setTextFormat(Qt::PlainText);
        box.setText(
            tr("The token “%1” is not a setting; settings are written as key=value.").arg(detail));
        break;

    case HelperUtils::LaunchError::AssociationSettingsWithWordlist:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("The settings line applies only while the words come from the hash "
                       "file. Clear either the settings line or the wordlist list."));
        break;

    case HelperUtils::LaunchError::NoHashRecipe:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("The hash type 4000 describes its hashes as a recipe, for example\n"
                       "md5(md5(salt) . pass). Enter it in the hash recipe field."));
        break;

    case HelperUtils::LaunchError::InvalidIncrement:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("The minimum increment length is larger than the maximum."));
        break;

    // The path below is user supplied, so plain text: a '<' in it would be
    // taken for markup by the default AutoText.
    case HelperUtils::LaunchError::InvalidCachePath:
        box.setTextFormat(Qt::PlainText);
        box.setText(tr("The cache path must be an existing directory:\n%1").arg(detail));
        break;

    case HelperUtils::LaunchError::IncrementWithSpeedOnly:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("The increment cannot be combined with the speed-only option."));
        break;

    case HelperUtils::LaunchError::RemoveInAssociation:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("Removing cracked hashes is not possible in the association attack mode."));
        break;

    case HelperUtils::LaunchError::None:
        return;
    }

    box.exec();
}

/*************** Helper ***************/

void MainWindow::commandChanged()
{
    auto &settings = SettingsManager::instance();
    const QString hashcatPath = settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath);

    // The binary name comes first when one is configured; without it the
    // preview is the bare argument list, which must not start with a blank.
    const QString binary = hashcatPath.isEmpty() ? QString() : QFileInfo(hashcatPath).fileName();
    const QStringList arguments = generateArguments();

    const QString command = (binary.isEmpty() ? QString() : binary + QLatin1Char(' '))
                            + displayArguments(arguments).join(QLatin1Char(' '));
    ui->lineEdit_command->setText(command);
    ui->lineEdit_command->setCursorPosition(0);

    // The line edit shows the beginning of the command only - it is as wide as
    // the window and the command is not. The tooltip carries all of it.
    ui->lineEdit_command->setToolTip(commandTooltip(command));
}

HashcatOptions MainWindow::collectHashcatOptions()
{
    auto &settings = SettingsManager::instance();

    HashcatOptions options;
    options.useShortParameters =
        settings.getKey<bool>(AppConstants::SettingsKeys::UseShortParameters);

    // currentData() is invalid while the hash type list has not been filled -
    // no hashcat configured, or the query failed. CommandBuilder then leaves
    // -m out entirely instead of asking for hash type 0.
    const QVariant hashType = ui->comboBox_hash_type->currentData();
    options.hashType = hashType.isValid() ? hashType.toInt() : -1;
    options.hashRecipe = ui->lineEdit_hash_recipe->text();

    options.attackMode = static_cast<AttackMode>(ui->comboBox_attack->currentData().toInt());

    options.remove = ui->checkBox_remove->isChecked();
    options.ignoreUsername = ui->checkBox_ignoreusername->isChecked();

    options.force = ui->checkBox_force->isChecked();
    options.session = ui->lineEdit_session->text();
    options.runtime = ui->spinBox_runtime->value();
    options.skip = ui->lineEdit_skip->text();
    options.limit = ui->lineEdit_limit->text();
    options.status = ui->checkBox_status->isChecked();
    options.statusTimer = ui->spinBox_status_timer->value();
    options.potfileDisable = ui->checkBox_potfile_disable->isChecked();

    if (ui->radioButton_use_rules_file->isChecked()) {
        QCheckBox *boxes[3] = {ui->checkBox_rulesfile_1, ui->checkBox_rulesfile_2,
                               ui->checkBox_rulesfile_3};
        QLineEdit *edits[3] = {ui->lineEdit_rulesfile_1, ui->lineEdit_rulesfile_2,
                               ui->lineEdit_rulesfile_3};
        for (int i = 0; i < 3; ++i) {
            if (boxes[i]->isChecked() && !edits[i]->text().isEmpty()) {
                options.rulesFiles << edits[i]->text();
            }
        }
        // Stored as a plain bool: the builder only emits it next to the -r
        // files, so a ticked box with generated rules selected stays silent.
        options.rulesConcat = ui->checkBox_rules_concat->isChecked();
    } else if (ui->radioButton_generate_rules->isChecked()) {
        options.generateRules = ui->spinBox_generate_rules->value();
    }

    options.ruleLeft = ui->lineEdit_rule_left->text();
    options.ruleRight = ui->lineEdit_rule_right->text();

    options.mask = ui->lineEdit_mask->text();
    // The increment widgets are hidden in the modes that refuse the flag
    // (mode 12), so a session that stored a checked box there must not put
    // -i on the command line.
    options.increment = CommandBuilder::attackUsesIncrement(options.attackMode)
                        && ui->checkBox_increment->isChecked();
    options.incrementMin = ui->spinBox_increment_min->value();
    options.incrementMax = ui->spinBox_increment_max->value();
    options.speedOnly = ui->checkBox_speed_only->isChecked();
    options.optimizedKernel = ui->checkBox_optimized_kernel->isChecked();

    QCheckBox *charsetBoxes[4] = {
        ui->checkBox_custom_charset1,
        ui->checkBox_custom_charset2,
        ui->checkBox_custom_charset3,
        ui->checkBox_custom_charset4,
    };
    QLineEdit *charsetEdits[4] = {
        ui->lineEdit_custom_charset1,
        ui->lineEdit_custom_charset2,
        ui->lineEdit_custom_charset3,
        ui->lineEdit_custom_charset4,
    };
    for (int i = 0; i < 4; ++i) {
        options.customCharsets[i].enabled = charsetBoxes[i]->isChecked();
        options.customCharsets[i].value = charsetEdits[i]->text();
    }

    options.hexCharset = ui->checkBox_hex_charset->isChecked();
    options.hexSalt = ui->checkBox_hex_salt->isChecked();
    options.hexWordlist = ui->checkBox_hex_wordlist->isChecked();
    options.encodingFrom = ui->lineEdit_encoding_from->text();
    options.encodingTo = ui->lineEdit_encoding_to->text();

    options.outfileEnabled = ui->checkBox_outfile->isChecked();
    options.outfile = ui->lineEdit_outfile->text();
    options.outfileFormat = ui->lineEdit_outfile_format->text();
    options.outfileJson = ui->checkBox_outfile_json->isChecked();
    options.cachePath = ui->lineEdit_cache_path->text();
    options.cpuAffinity = ui->lineEdit_cpu_affinity->text();
    options.backendDevices = ui->lineEdit_backend_devices->text();
    options.openclDeviceTypes = ui->lineEdit_opencl_device_types->text();
    options.hwmonTempAbort = ui->checkBox_hwmon_temp_abort->isChecked();
    options.hwmonTempAbortCelsius = ui->spinBox_hwmon_temp_abort->value();
    options.hashFile = ui->lineEdit_hashfile->text();

    // Every argument list - the wordlists included - passes only its checked
    // rows: an unchecked row stays in the list and in the session, but not in
    // the command.
    options.wordlists = checkedItems(ui->listWidget_wordlist);

    // The feed modes. Every value is read unconditionally: a disabled or
    // hidden field can still hold a value a session stored, and the launch
    // validation is where a combination that cannot work is refused.
    const auto splitSettings = [](const QString &text) {
        static const QRegularExpression separator{QStringLiteral("\\s+")};
        return text.split(separator, Qt::SkipEmptyParts);
    };

    options.pcfgRulesets = checkedItems(ui->listWidget_pcfg_rulesets);
    options.pcfgScale = ui->lineEdit_pcfg_scale->text().trimmed();
    options.pcfgCostmax = ui->lineEdit_pcfg_costmax->text().trimmed();
    options.pcfgWeights = ui->lineEdit_pcfg_weights->text().trimmed();
    options.pcfgThreads = ui->lineEdit_pcfg_threads->text().trimmed();
    options.pcfgKbits = ui->lineEdit_pcfg_kbits->text().trimmed();
    options.pcfgMaxword = ui->lineEdit_pcfg_maxword->text().trimmed();
    options.pcfgMaxgain = ui->lineEdit_pcfg_maxgain->text().trimmed();
    options.pcfgWalk = ui->lineEdit_pcfg_walk->text().trimmed();
    options.pcfgMask = ui->lineEdit_pcfg_mask->text().trimmed();
    options.pcfgExtraSettings = splitSettings(ui->lineEdit_pcfg_extra->text());

    options.tableFiles = checkedItems(ui->listWidget_tables);
    options.tableIdentity = ui->lineEdit_table_identity->text().trimmed();
    options.tableTemplate = ui->lineEdit_table_template->text().trimmed();
    options.tableSingle = ui->lineEdit_table_single->text().trimmed();
    options.tableCap = ui->lineEdit_table_cap->text().trimmed();
    options.tableMaxperm = ui->lineEdit_table_maxperm->text().trimmed();
    options.tableExtraSettings = splitSettings(ui->lineEdit_table_extra->text());

    options.feedName = ui->lineEdit_feed->text().trimmed();
    options.feedArguments = checkedItems(ui->listWidget_feed_arguments);

    // The settings line of mode 9 stays as typed even next to a wordlist:
    // the launch validation names both controls instead of silently dropping
    // what the user typed, and the preview keeps showing it.
    options.associationSettings = splitSettings(ui->lineEdit_association_settings->text());

    return options;
}

QStringList MainWindow::generateArguments()
{
    return CommandBuilder::build(collectHashcatOptions());
}
