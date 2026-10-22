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
#include "hashcatinfoparser.h"
#include "helperutils.h"
#include "statefile.h"
#include "widgetstateserializer.h"
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QProcess>
#include <QAbstractButton>
#include <QAbstractItemModel>
#include <QClipboard>
#include <QCursor>
#include <QFutureWatcher>
#include <QLayout>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QStandardPaths>
#include <QTextDocument>
#include <QToolTip>

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
        if (!line.isEmpty() && line.size() + part.size() > 120 && !endsWithFlag) {
            lines << line;
            line = part;
        } else {
            line += (line.isEmpty() ? QString() : QStringLiteral(" ")) + part;
        }
    }
    if (!line.isEmpty()) {
        lines << line;
    }

    QString text = lines.join(QLatin1Char('\n'));
    if (!Qt::mightBeRichText(text)) {
        return text;
    }

    return text.toHtmlEscaped().replace(QLatin1Char('\n'), QStringLiteral("<br>"));
}

// Where a file dialog for an export starts: a folder the user knows, with the
// name the application suggests. A bare file name would open the dialog in the
// application's working directory, wherever it happened to be started from.
QString suggestedExportPath(QLatin1StringView fileName)
{
    QString folder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if (!QFileInfo(folder).isDir()) {
        folder = QDir::homePath();
    }
    return QDir(folder).filePath(QString(fileName));
}
} // namespace

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // -s and -l take a plain number; a digit-only validator keeps a value
    // hashcat would refuse to start from ever reaching the command line.
    const QRegularExpression digits{QString(AppConstants::Validation::Digits)};
    ui->lineEdit_skip->setValidator(new QRegularExpressionValidator(digits, ui->lineEdit_skip));
    ui->lineEdit_limit->setValidator(new QRegularExpressionValidator(digits, ui->lineEdit_limit));

    /* ---------- help tooltips: translatable label + fixed reference tables -- */
    const QString charsetTip =
        tr("Built-in charsets:") + QLatin1String("\n\n") + AppConstants::HelpText::BuiltinCharsets;
    ui->lineEdit_mask->setToolTip(charsetTip);
    ui->lineEdit_custom_charset1->setToolTip(charsetTip);
    ui->lineEdit_custom_charset2->setToolTip(charsetTip);
    ui->lineEdit_custom_charset3->setToolTip(charsetTip);
    ui->lineEdit_custom_charset4->setToolTip(charsetTip);

    const QString outfileFormatTip =
        tr("Outfile format to use, separated with commas.\ndefault: 1,2") + QLatin1String("\n\n")
        + AppConstants::HelpText::OutfileFormats;
    ui->lineEdit_outfile_format->setToolTip(outfileFormatTip);

    const QString encodingTip =
        tr("Common encodings:") + QLatin1String("\n\n") + AppConstants::HelpText::CommonEncodings;
    ui->lineEdit_encoding_from->setToolTip(tr("Force the internal wordlist encoding from this "
                                              "code. Leave empty to let hashcat decide.")
                                           + QLatin1String("\n\n") + encodingTip);
    ui->lineEdit_encoding_to->setToolTip(
        tr("Force the internal wordlist encoding to this code. Leave empty to let hashcat decide.")
        + QLatin1String("\n\n") + encodingTip);

    const QString deviceTypeTip = tr("OpenCL device types to use, separated with commas")
                                  + QLatin1String("\n\n")
                                  + AppConstants::HelpText::OpenClDeviceTypes;
    ui->lineEdit_opencl_device_types->setToolTip(deviceTypeTip);

    auto &settings = SettingsManager::instance();

    // The workload levels carry only display text in the .ui file; give each
    // one an id so a session stores the level and not its position.
    for (int i = 0; i < ui->comboBox_workload_profile->count(); ++i) {
        ui->comboBox_workload_profile->setItemData(i, i + 1);
    }

    initHashAndAttackModes();
    updateViewAttackMode();

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
    notifyOnText(ui->lineEdit_hashfile);
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
    // The two hash file options stay on the Attack tab; everything from the
    // outfile on belongs to the General tab.
    notifyOnToggle(ui->checkBox_remove);
    notifyOnToggle(ui->checkBox_ignoreusername);
    // General tab
    notifyOnToggle(ui->checkBox_outfile);
    notifyOnText(ui->lineEdit_outfile);
    notifyOnText(ui->lineEdit_outfile_format);
    notifyOnText(ui->lineEdit_cpu_affinity);
    notifyOnText(ui->lineEdit_backend_devices);
    notifyOnValue(ui->spinBox_segment_size);
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
    notifyOnToggle(ui->checkBox_override_workload_profile);
    notifyOnIndex(ui->comboBox_workload_profile);
    notifyOnText(ui->lineEdit_opencl_device_types);
    notifyOnToggle(ui->checkBox_hwmon_temp_abort);
    notifyOnValue(ui->spinBox_hwmon_temp_abort);

    /* ---------- menu actions ---------- */
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::aboutTriggered);
    connect(ui->actionResetFields, &QAction::triggered, this, &MainWindow::resetFieldsTriggered);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quitTriggered);
    connect(ui->actionExport, &QAction::triggered, this, &MainWindow::exportTriggered);
    connect(ui->actionImport, &QAction::triggered, this, &MainWindow::importTriggered);
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
    connect(ui->toolButton_wordlist_sort_asc, &QToolButton::clicked, this,
            &MainWindow::wordlistSortAscClicked);
    connect(ui->toolButton_wordlist_sort_desc, &QToolButton::clicked, this,
            &MainWindow::wordlistSortDescClicked);

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
    connect(ui->pushButton_start, &QPushButton::clicked, this, &MainWindow::startClicked);
    connect(ui->pushButton_copy_clipboard, &QPushButton::clicked, this,
            &MainWindow::copyCommandToClipboard);
    connect(ui->checkBox_override_workload_profile, &QCheckBox::toggled,
            ui->comboBox_workload_profile, &QComboBox::setEnabled);
    // attackIndexChanged() refreshes the preview through
    // updateViewAttackMode(); connecting it to commandChanged() as well would
    // rebuild the preview twice for every mode change.
    connect(ui->comboBox_attack, &QComboBox::currentIndexChanged, this,
            &MainWindow::attackIndexChanged);
    connect(ui->checkBox_outfile, &QCheckBox::toggled, this, &MainWindow::outfileToggled);

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
// the widest one the width. No scrollbar when the content fits, and no empty
// strip when it is shorter.
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

    // Hidden pages report the minima they had before their layouts first ran.
    // Resolve every page first, then take the largest minimum in each
    // direction independently: one tab can be the height reference while
    // another is the width reference.
    QList<QScrollArea *> areas;
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
        areas.append(area);
        contentMinimum = contentMinimum.expandedTo(area->widget()->minimumSizeHint());
    }
    if (areas.isEmpty()) {
        return;
    }

    // The scroll area's own sizeHint is capped and does not describe the tab
    // content. Derive the window chrome from the geometry it has on first
    // show: the difference between the window and the scroll area that fills
    // it holds the menu bar, margins, tab bar and frame, and is the same for
    // every tab.
    const QSize chrome = size() - currentArea->size();
    QSize target = chrome + contentMinimum;

    // A screen too small for the content keeps the window on-screen; the
    // scroll areas stay in place and show their scrollbars as the fallback.
    // The margin leaves room for the window decorations.
    const QScreen *scr = screen();
    QSize cap;
    if (scr) {
        cap = scr->availableGeometry().size() - QSize(40, 80);
        target = target.boundedTo(cap);
    }

    // A tab taller than the final viewport shows a vertical scrollbar. Reserve
    // its width so that content does not need a horizontal scrollbar as well.
    const int viewportHeight = target.height() - chrome.height();
    int requiredWidth = 0;
    for (const QScrollArea *area : areas) {
        const QSize minimum = area->widget()->minimumSizeHint();
        int width = minimum.width();
        if (minimum.height() > viewportHeight) {
            width += area->verticalScrollBar()->sizeHint().width();
        }
        requiredWidth = qMax(requiredWidth, width);
    }
    target.setWidth(chrome.width() + requiredWidth);
    if (scr) {
        target.setWidth(qMin(target.width(), cap.width()));
    }

    resize(target);
}

/*************** Menu Bar ***************/

// File → Export
void MainWindow::exportTriggered()
{
    const QString file = QFileDialog::getSaveFileName(
        this, tr("Export Session"), suggestedExportPath(AppConstants::Files::SessionExport),
        tr("JSON Files (*.json)"));

    if (file.isEmpty()) {
        return;
    }

    QString error;
    const QJsonObject values =
        WidgetStateSerializer::toJson(this, {AppConstants::Files::PreviewWidget});
    if (StateFile::write(file, AppConstants::FileSections::Session, AppConstants::Sessions::Version,
                         values, &error)) {
        QMessageBox::information(this, tr("Exported"), tr("Session exported to %1.").arg(file));
    } else {
        QMessageBox::warning(this, tr("Export failed"), error);
    }
}

// File → Import
void MainWindow::importTriggered()
{
    const QString file = QFileDialog::getOpenFileName(this, tr("Import Session"), QString(),
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
    const WidgetStateSerializer::ApplyResult notApplied =
        WidgetStateSerializer::applyToWidgets(values, this);
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

    if (notes.isEmpty()) {
        QMessageBox::information(this, tr("Imported"), tr("Session imported from %1.").arg(file));
    } else {
        QMessageBox::warning(
            this, tr("Imported"),
            tr("Session imported from %1.\n\n%2").arg(file, notes.join(QLatin1Char('\n'))));
    }
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

// File → Export Settings
void MainWindow::exportSettingsTriggered()
{
    const QString file = QFileDialog::getSaveFileName(
        this, tr("Export Settings"), suggestedExportPath(AppConstants::Files::SettingsExport),
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
    const QString file = QFileDialog::getOpenFileName(this, tr("Import Settings"), QString(),
                                                      tr("JSON Files (*.json)"));

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
    message << tr("Settings loaded from %1.").arg(file);
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

// Tools → Reset field
void MainWindow::resetFieldsTriggered()
{
    // Attack tab
    ui->lineEdit_hashfile->clear();
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
    ui->lineEdit_rulesfile_1->clear();
    ui->lineEdit_rulesfile_2->clear();
    ui->lineEdit_rulesfile_3->clear();
    ui->lineEdit_rule_left->clear();
    ui->lineEdit_rule_right->clear();
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
    ui->lineEdit_cpu_affinity->clear();
    ui->lineEdit_backend_devices->setText(AppConstants::Defaults::BackendDevices);
    ui->spinBox_segment_size->setValue(AppConstants::Defaults::SegmentSize);

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
    ui->checkBox_override_workload_profile->setChecked(false);
    ui->comboBox_workload_profile->setCurrentIndex(0);
    ui->lineEdit_opencl_device_types->clear();
    ui->checkBox_hwmon_temp_abort->setChecked(false);
    ui->spinBox_hwmon_temp_abort->setValue(AppConstants::Defaults::HwmonTempAbort);

    updateOptionDependencies();
    commandChanged();
}

// File → Settings
void MainWindow::settingsTriggered()
{
    SettingsDialog settingsDialog(this);
    if (settingsDialog.exec() == QDialog::Accepted) {
        // If SettingsDialog was saved and there are no hash types yet maybe we can populate them
        // now
        if (ui->comboBox_hash_type->count() == 0) {
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
    return SettingsManager::instance().getKey<bool>(AppConstants::SettingsKeys::RestoreSession,
                                                    true);
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
               "was left in place; \"Clear the saved session\" in the settings deletes it.\n\n%1")
                .arg(error));
        return;
    }

    // A session with a newer format version is left in place: it is what that
    // version saved, and the user may go back to it.
    if (version > AppConstants::Sessions::Version) {
        QMessageBox::warning(this, tr("Session warning"),
                             newerFileNote(version, AppConstants::Sessions::Version));
        return;
    }

    // The hash combo has no items yet - the query that fills it runs
    // asynchronously. Remember the stored hash type; the reply applies it
    // once the items exist.
    const WidgetStateSerializer::ApplyResult notApplied =
        WidgetStateSerializer::applyToWidgets(values, this);
    const QString problem = restoreHashType(values);
    commandChanged();

    // The auto-load is not an import: there is no headline, and nothing is
    // shown when the load went well.
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

    if (!notes.isEmpty()) {
        QMessageBox::warning(this, tr("Session warning"), notes.join(QLatin1Char('\n')));
    }
}

// The hash type of a session.
//
// The mapping applies it when the list of hash types is there; this only
// checks whether this hashcat knows the one the file names, and it waits for
// the list while the query that fills it is still on its way.
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

void MainWindow::initHashAndAttackModes()
{
    ui->comboBox_attack->clear();
    ui->comboBox_hash_type->clear();

    // Attack modes
    QMap<AttackMode, QString> attackModes;
    attackModes.insert(AttackMode::Straight, tr("Straight"));
    attackModes.insert(AttackMode::Combination, tr("Combination"));
    attackModes.insert(AttackMode::BruteForce, tr("Brute-force"));
    attackModes.insert(AttackMode::HybridWordMask, tr("Hybrid Wordlist + Mask"));
    attackModes.insert(AttackMode::HybridMaskWord, tr("Hybrid Mask + Wordlist"));
    attackModes.insert(AttackMode::Association, tr("Association"));

    // The numeric id travels as item data: the displayed text is what a
    // translation changes, the id is what has to end up on the command line.
    for (auto it = attackModes.constBegin(); it != attackModes.constEnd(); ++it) {
        ui->comboBox_attack->addItem(it.value(), static_cast<int>(it.key()));
    }

    // Hash types
    auto &settings = SettingsManager::instance();

    if (!settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath).isEmpty()) {
        ui->comboBox_hash_type->setToolTip(tr("Updating..."));
        ui->comboBox_hash_type->setEnabled(false);

        // Create a watcher that will be destroyed once finished
        QFutureWatcher<HashcatResult> *watcher = new QFutureWatcher<HashcatResult>(this);

        // When the future is finished, fill the combo with the parsed types.
        connect(watcher, &QFutureWatcher<HashcatResult>::finished, this, [this, watcher]() {
            const HashcatResult &result = watcher->result();
            watcher->deleteLater();

            QString error;
            QString hashTypeProblem;

            // Check if the command failed
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
            // the "Updating..." tooltip forever.
            ui->comboBox_hash_type->setEnabled(true);
            ui->comboBox_hash_type->setToolTip(QString());

            if (!error.isEmpty()) {
                QMessageBox::warning(this, tr("hashcat error"), error);
            } else if (!hashTypeProblem.isEmpty()) {
                QMessageBox::warning(this, tr("Session warning"), hashTypeProblem);
            }
        });

        // Kick off the asynchronous process
        watcher->setFuture(HelperUtils::executeHashcat(QStringList()
                                                       << AppConstants::Hashcat::ExampleHashes
                                                       << AppConstants::Hashcat::MachineReadable));
    }
}

void MainWindow::attackIndexChanged([[maybe_unused]] int index)
{
    updateViewAttackMode();
}

void MainWindow::updateViewAttackMode()
{
    const AttackMode mode = static_cast<AttackMode>(ui->comboBox_attack->currentData().toInt());

    // Use the very predicates CommandBuilder applies, so the widgets show
    // exactly the options that end up in the command line and the two can
    // never drift apart.
    const bool usesMask = CommandBuilder::attackUsesMask(mode);
    const bool usesRuleLeft = CommandBuilder::attackUsesRuleLeft(mode);
    const bool usesRuleRight = CommandBuilder::attackUsesRuleRight(mode);
    ui->groupBox_wordlists->setEnabled(CommandBuilder::attackUsesWordlists(mode));
    ui->groupBox_rules->setEnabled(CommandBuilder::attackUsesRules(mode));
    ui->groupBox_wordlist_rules->setEnabled(usesRuleLeft || usesRuleRight);
    ui->label_rule_left->setEnabled(usesRuleLeft);
    ui->lineEdit_rule_left->setEnabled(usesRuleLeft);
    ui->label_rule_right->setEnabled(usesRuleRight);
    ui->lineEdit_rule_right->setEnabled(usesRuleRight);
    ui->groupBox_custom_charset->setEnabled(usesMask);
    ui->groupBox_mask->setEnabled(usesMask);
    ui->checkBox_hex_charset->setEnabled(usesMask);

    // Tell the user what the mode expects, so the launch validation does not
    // have to be the first thing that explains it.
    ui->label_mode_hint->setText(modeHint(mode));

    updateOptionDependencies();
    commandChanged();
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
    const CommandBuilder::ModeRequirements requirements = CommandBuilder::modeRequirements(mode);

    // Combination mode is the only one that wants an exact number of word
    // lists, and its two lists are the left and the right side.
    if (requirements.minWordlists > 0 && requirements.maxWordlists == requirements.minWordlists) {
        return tr("This mode needs exactly %1 word lists (left, right).")
            .arg(requirements.minWordlists);
    }
    if (requirements.needsMask && requirements.minWordlists > 0) {
        return tr("This mode needs a word list and a mask.");
    }
    if (requirements.needsMask) {
        return tr("This mode needs a mask.");
    }
    if (requirements.minWordlists > 0) {
        return tr("This mode needs at least one word list.");
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
    QString hashfile = QFileDialog::getOpenFileName(this, tr("Open Hash File"));
    if (!hashfile.isEmpty()) {
        ui->lineEdit_hashfile->setText(QDir::toNativeSeparators(hashfile));
    }
}

void MainWindow::openOutfileClicked()
{
    QString outfile = QFileDialog::getSaveFileName(this, tr("Save Output File"));
    if (!outfile.isEmpty()) {
        ui->lineEdit_outfile->setText(QDir::toNativeSeparators(outfile));
    }
}

void MainWindow::removeWordlistClicked()
{
    qDeleteAll(ui->listWidget_wordlist->selectedItems());
    ui->pushButton_remove_wordlist->setEnabled(false);
    ui->toolButton_wordlist_sort_asc->setEnabled(false);
    ui->toolButton_wordlist_sort_desc->setEnabled(false);
    ui->listWidget_wordlist->clearSelection();
}

void MainWindow::addWordlistClicked()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Add Wordlists"));
    QListWidget *w = ui->listWidget_wordlist;

    for (const QString &wordlist : std::as_const(files)) {
        // A file has been selected and it is not already in the list
        if (!wordlist.isEmpty() && w->findItems(wordlist, Qt::MatchExactly).isEmpty()) {
            QListWidgetItem *newItem = new QListWidgetItem(wordlist, w);
            newItem->setCheckState(Qt::Checked);
        }
    }
}

void MainWindow::wordlistSortAscClicked()
{
    // currentRow() is -1 when nothing is selected: without the guard below
    // takeItem(-1) returns nullptr and the list ends up with a null entry.
    int currentRow = ui->listWidget_wordlist->currentRow();
    if (currentRow <= 0) {
        return;
    }
    QListWidgetItem *currentItem = ui->listWidget_wordlist->takeItem(currentRow);
    ui->listWidget_wordlist->insertItem(currentRow - 1, currentItem);
    ui->listWidget_wordlist->setCurrentRow(currentRow - 1);
}

void MainWindow::wordlistSortDescClicked()
{
    int currentRow = ui->listWidget_wordlist->currentRow();
    if (currentRow < 0 || currentRow >= ui->listWidget_wordlist->count() - 1) {
        return;
    }
    QListWidgetItem *currentItem = ui->listWidget_wordlist->takeItem(currentRow);
    ui->listWidget_wordlist->insertItem(currentRow + 1, currentItem);
    ui->listWidget_wordlist->setCurrentRow(currentRow + 1);
}

void MainWindow::wordlistItemClicked([[maybe_unused]] QListWidgetItem *item)
{
    ui->pushButton_remove_wordlist->setEnabled(true);
    ui->toolButton_wordlist_sort_asc->setEnabled(true);
    ui->toolButton_wordlist_sort_desc->setEnabled(true);
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
    QString rulesfile = QFileDialog::getOpenFileName(this, tr("Open Rules File"));
    if (!rulesfile.isEmpty()) {
        ui->lineEdit_rulesfile_1->setText(QDir::toNativeSeparators(rulesfile));
    }
}

void MainWindow::openRulesFile2Clicked()
{
    QString rulesfile = QFileDialog::getOpenFileName(this, tr("Open Rules File"));
    if (!rulesfile.isEmpty()) {
        ui->lineEdit_rulesfile_2->setText(QDir::toNativeSeparators(rulesfile));
    }
}

void MainWindow::openRulesFile3Clicked()
{
    QString rulesfile = QFileDialog::getOpenFileName(this, tr("Open Rules File"));
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
            tr("Could not start %1: %2").arg(configuredTerminal, proc.errorString()));
    }
}

void MainWindow::showLaunchError(HelperUtils::LaunchError error, const QString &detail)
{
    const QString menu = ui->menuFile->menuAction()->text();
    const QString settingsEntry = ui->actionSettings->text();
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
        box.setText(tr("No supported terminal was found on this system. "
                       "hashcat-gui needs one to show the hashcat output."));
        break;

    case HelperUtils::LaunchError::UnknownTerminal:
        box.setTextFormat(Qt::PlainText);
        box.setText(tr("The configured terminal \"%1\" is not available. Available terminals: %2\n"
                       "Navigate to %3 → %4 to change it.")
                        .arg(configuredTerminal, detail, menu, settingsEntry));
        break;

    // The mode specific requirements. They are not misconfigurations of the
    // installation but of the attack itself, so the message says what the
    // mode needs instead of pointing at the settings.
    case HelperUtils::LaunchError::NoWordlist:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("This attack mode needs at least one word list."));
        break;

    case HelperUtils::LaunchError::WrongWordlistCount: {
        // The wording below uses minWordlists as the exact count. That is only
        // correct while Combination is the sole mode with maxWordlists >= 0,
        // and there min == max; a mode with a range needs different wording.
        const CommandBuilder::ModeRequirements requirements =
            CommandBuilder::modeRequirements(currentAttackMode());
        box.setIcon(QMessageBox::Information);
        box.setText(tr("This attack mode needs exactly %1 word lists.\nCurrently selected: %2.")
                        .arg(requirements.minWordlists)
                        .arg(detail));
        break;
    }

    case HelperUtils::LaunchError::NoMask:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("This attack mode needs a mask."));
        break;

    case HelperUtils::LaunchError::InvalidIncrement:
        box.setIcon(QMessageBox::Information);
        box.setText(tr("The minimum increment length is larger than the maximum."));
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
                            + arguments.join(QLatin1Char(' '));
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
    } else if (ui->radioButton_generate_rules->isChecked()) {
        options.generateRules = ui->spinBox_generate_rules->value();
    }

    options.ruleLeft = ui->lineEdit_rule_left->text();
    options.ruleRight = ui->lineEdit_rule_right->text();

    options.mask = ui->lineEdit_mask->text();
    options.increment = ui->checkBox_increment->isChecked();
    options.incrementMin = ui->spinBox_increment_min->value();
    options.incrementMax = ui->spinBox_increment_max->value();
    options.speedOnly = ui->checkBox_speed_only->isChecked();
    // The combo carries the workload level as item data; read that instead of
    // the display text, which is only a label.
    if (ui->checkBox_override_workload_profile->isChecked()) {
        options.workloadProfile = ui->comboBox_workload_profile->currentData().toString();
    }
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
    options.cpuAffinity = ui->lineEdit_cpu_affinity->text();
    options.backendDevices = ui->lineEdit_backend_devices->text();
    options.openclDeviceTypes = ui->lineEdit_opencl_device_types->text();
    options.hwmonTempAbort = ui->checkBox_hwmon_temp_abort->isChecked();
    options.hwmonTempAbortCelsius = ui->spinBox_hwmon_temp_abort->value();
    options.segmentSize = ui->spinBox_segment_size->value();
    options.hashFile = ui->lineEdit_hashfile->text();

    const auto wordlists =
        ui->listWidget_wordlist->findItems(QStringLiteral("*"), Qt::MatchWildcard);
    for (const QListWidgetItem *item : wordlists) {
        if (item->checkState() == Qt::Checked) {
            options.wordlists << item->text();
        }
    }

    return options;
}

QStringList MainWindow::generateArguments()
{
    return CommandBuilder::build(collectHashcatOptions());
}
