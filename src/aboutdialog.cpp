/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "aboutdialog.h"

#include "appconstants.h"
#include "executable.h"
#include "hashcatrunner.h"
#include "settingsmanager.h"
#include "ui_aboutdialog.h"
#include "updatechecker.h"

#include <QApplication>
#include <QClipboard>
#include <QCursor>
#include <QDialogButtonBox>
#include <QFileInfo>
#include <QFont>
#include <QFutureWatcher>
#include <QIcon>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QShowEvent>
#include <QStringList>
#include <QStyle>
#include <QSysInfo>
#include <QTextLayout>
#include <QTextOption>
#include <QToolTip>

namespace
{
// The artwork the window and the taskbar show. It is part of the resources
// already, so the dialog does not need a second copy.
const QString &logoPath()
{
    static const QString path = QStringLiteral(":/icons/hicolor/128x128/apps/hashcat-gui.png");
    return path;
}

// An anchor around a label. The address is a constant, the label around it
// is what gets translated.
QString link(QLatin1StringView url, const QString &text)
{
    return QStringLiteral("<a href=\"%1\">%2</a>").arg(QString(url), text);
}

// A sentence with a link in it: the anchor sits where the text says it does.
//
// The tooltip says where the link leads, so a user does not have to click it
// to find out.
void setLinkText(QLabel *label, QLatin1StringView url, const QString &richText)
{
    label->setText(richText);
    label->setOpenExternalLinks(true);
    label->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse
                                   | Qt::TextInteractionFlag::LinksAccessibleByKeyboard);
    label->setToolTip(QString(url));
}

// A link that is nothing but the anchor around its text.
void setLink(QLabel *label, QLatin1StringView url, const QString &text)
{
    setLinkText(label, url, link(url, text));
}

// Wraps the text at word boundaries, breaking a word longer than the line.
// The inserted breaks let the label report its height, so the dialog grows
// with the text instead of cutting it off.
QString wrapText(const QString &text, const QFont &font, int width)
{
    QTextOption option;
    option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

    QTextLayout layout(text, font);
    layout.setTextOption(option);
    layout.beginLayout();

    QStringList lines;
    while (true) {
        QTextLine line = layout.createLine();
        if (!line.isValid()) {
            break;
        }
        line.setLineWidth(width);
        lines << text.mid(line.textStart(), line.textLength());
    }
    layout.endLayout();

    return lines.join(QLatin1Char('\n'));
}
} // namespace

AboutDialog::AboutDialog(QWidget *parent) : AboutDialog(nullptr, parent) {}

AboutDialog::AboutDialog(UpdateChecker *checker, QWidget *parent)
    : QDialog(parent), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    // hashcat's answer is not markup: a path or an error with "<" in it would
    // be read as a tag and disappear from the label.
    ui->label_hc_version->setTextFormat(Qt::PlainText);
    ui->label_update_result->setTextFormat(Qt::PlainText);

    updateVersionLabel();
    updateSystemLabels();
    updateLinks();
    updateLicense();
    setupUpdateCheck(checker);

    // The logo is scaled to the size the .ui gives the label, in device
    // pixels, so the artwork stays sharp on a high-dpi display without the
    // .ui having to know about the device pixel ratio.
    const QSize logoSize = ui->label_logo->minimumSize();
    const qreal ratio = devicePixelRatioF();
    const QPixmap logo(logoPath());
    if (!logo.isNull()) {
        QPixmap scaled =
            logo.scaled(logoSize * ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        scaled.setDevicePixelRatio(ratio);
        ui->label_logo->setPixmap(scaled);
    }
    ui->label_logo->setAlignment(Qt::AlignCenter);

    // The application name is the headline of the dialog.
    QFont nameFont = ui->label_app_name->font();
    nameFont.setBold(true);
    ui->label_app_name->setFont(nameFont);

    // OK is the response of the dialog, so it gets the standard role: Enter
    // presses it and the dialog accepts. Copy details stays a plain button on
    // the other side - it acts on the dialog, it does not close it.
    QPushButton *okButton = ui->buttonBox->addButton(tr("OK"), QDialogButtonBox::AcceptRole);
    okButton->setObjectName(QStringLiteral("pushButton_ok"));
    okButton->setDefault(true);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->pushButton_copy_details, &QPushButton::clicked, this,
            &AboutDialog::copyDetailsClicked);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    if (m_sizedToContent) {
        return;
    }
    m_sizedToContent = true;

    resize(sizeHint());
}

void AboutDialog::copyDetailsClicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    if (!clipboard) {
        // No clipboard on this platform.
        return;
    }

    clipboard->setText(detailsText());
    // A one-shot tooltip confirms the copy. The button's own tooltip would
    // not re-render while the pointer stays on the button.
    QToolTip::showText(QCursor::pos(), tr("Copied to the clipboard"), ui->pushButton_copy_details,
                       {}, 2000);
}

// The versions for a bug report. Not translated: a report that mixes
// languages is harder to read.
QString AboutDialog::detailsText() const
{
    QStringList details;
    details << QStringLiteral("hashcat-gui %1").arg(QApplication::applicationVersion());
    details << QStringLiteral("Qt %1").arg(QString::fromLatin1(qVersion()));
    details << QStringLiteral("OS: %1").arg(QSysInfo::prettyProductName());

    const QString hashcatPath =
        SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    if (hashcatPath.isEmpty()) {
        details << QStringLiteral("hashcat: not configured");
    } else if (m_hashcatVersion.isEmpty()) {
        // The query is still on its way, or it failed: there is no version to
        // report, and the error is not one.
        details << QStringLiteral("hashcat: version unknown (%1)").arg(hashcatPath);
    } else {
        details << QStringLiteral("hashcat %1 (%2)").arg(m_hashcatVersion, hashcatPath);
    }

    return details.join(QLatin1Char('\n'));
}

// What the application runs on: the toolkit it is built with and the system
// underneath it. Both are asked for when the dialog opens, because neither
// of them can change while it is on screen.
void AboutDialog::updateSystemLabels()
{
    ui->label_qt_version->setText(QString::fromLatin1(qVersion()));
    ui->label_os->setText(QSysInfo::prettyProductName());
}

// The places the user is most likely to want next: the source of the
// application, the place to report a problem and the pages of hashcat
// itself. The URLs are constants, only the labels around them are
// translated.
void AboutDialog::updateLinks()
{
    setLink(ui->label_link_project, AppConstants::Urls::Project, tr("Project page"));
    setLink(ui->label_link_issue, AppConstants::Urls::Issues, tr("Report an issue"));
    setLink(ui->label_link_hashcat, AppConstants::Urls::Hashcat, tr("hashcat"));
    setLink(ui->label_link_hashcat_faq, AppConstants::Urls::HashcatFaq, tr("hashcat FAQ"));
}

// hashcat has its own license; the dialog does not distribute it, so it does
// not state its terms.
void AboutDialog::updateLicense()
{
    setLinkText(ui->label_license_gui, AppConstants::Urls::License,
                tr("hashcat-gui is licensed under the %1.")
                    .arg(link(AppConstants::Urls::License, tr("GNU GPL v3.0 or later"))));
}

void AboutDialog::updateVersionLabel()
{
    auto &settings = SettingsManager::instance();
    const QString path = settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    const QString resolved = Executable::resolve(path);

    if (path.isEmpty()) {
        // No hashcat is configured, so there is no version to show. The
        // details of a bug report say the same.
        setVersionLabel(tr("Not configured"));
    } else if (resolved.isEmpty()) {
        // The query would only fail to start; the path is what a bug report
        // needs to name.
        ui->label_hc_version_text->setText(QFileInfo(path).fileName());
        setVersionError(tr("The configured hashcat executable is not found."));
    } else {
        ui->label_hc_version_text->setText(QFileInfo(path).fileName());

        if (m_versionWatcher == nullptr) {
            m_versionWatcher = new QFutureWatcher<HashcatResult>(this);
            connect(m_versionWatcher, &QFutureWatcher<HashcatResult>::finished, this, [this] {
                const HashcatResult &result = m_versionWatcher->result();

                if (result.exitStatus != QProcess::NormalExit || result.exitCode != 0) {
                    setVersionError(result.standardError.simplified());
                } else {
                    setVersionText(result.standardOutput.simplified());
                }
            });
        }

        m_versionWatcher->setFuture(
            HashcatRunner::execute(resolved, QStringList() << AppConstants::Hashcat::Version));
    }

    ui->label_hc_gui_version->setText(QApplication::applicationVersion());
}

void AboutDialog::setVersionText(const QString &text)
{
    m_hashcatVersion = text;
    setVersionLabel(text);
}

// The error shown in place of the version, kept out of the reported string.
void AboutDialog::setVersionError(const QString &error)
{
    m_hashcatVersion.clear();
    setVersionLabel(tr("Error: %1").arg(error));
}

void AboutDialog::setVersionLabel(const QString &text)
{
    const int width = ui->label_hc_version->minimumWidth();
    ui->label_hc_version->setText(wrapText(text, ui->label_hc_version->font(), width));
}

// The update row starts idle: only the button is visible. Opening the dialog
// must not contact the network - the user decides when to check.
void AboutDialog::setupUpdateCheck(UpdateChecker *checker)
{
    ui->label_update_checking->hide();
    ui->progressBar_update->hide();
    ui->label_update_result->hide();
    ui->label_update_link->hide();

    // Qt has no dedicated update icon among its standard pixmaps. Use the
    // platform's refresh symbol where there is an icon theme, and Qt's reload
    // arrows everywhere else.
    ui->pushButton_check_updates->setIcon(QIcon::fromTheme(
        QStringLiteral("view-refresh"), style()->standardIcon(QStyle::SP_BrowserReload)));

    connect(ui->pushButton_check_updates, &QPushButton::clicked, this,
            &AboutDialog::checkUpdatesClicked);

    // A checker passed in belongs to its caller - the tests keep it on the
    // stack. The checker this dialog creates itself is a child and is
    // destroyed along with the dialog, together with its network reply.
    m_updateChecker = checker ? checker : new UpdateChecker(this);
    connect(m_updateChecker, &UpdateChecker::finished, this, &AboutDialog::showUpdateResult);
}

void AboutDialog::checkUpdatesClicked()
{
    // The button is disabled while a request is in flight and hidden once a
    // successful answer has arrived, so neither state can start a second
    // request.
    if (!ui->pushButton_check_updates->isEnabled()) {
        return;
    }

    ui->pushButton_check_updates->setEnabled(false);
    ui->label_update_checking->show();
    ui->progressBar_update->show();
    ui->label_update_result->hide();
    ui->label_update_link->hide();

    growToFitContent();
    m_updateChecker->check();
}

// Grows the dialog when the update row needs more room. A window the user
// has resized is left at its size; only the missing space is added.
void AboutDialog::growToFitContent()
{
    ensurePolished();
    layout()->activate();
    resize(size().expandedTo(sizeHint()));
}

// Replaces the progress indicator with the answer. The result label is plain
// text because the error comes from the network and could contain markup; the
// link is a constant anchor built here.
void AboutDialog::showUpdateResult(const UpdateCheckResult &result)
{
    ui->label_update_checking->hide();
    ui->progressBar_update->hide();
    ui->label_update_result->show();
    ui->label_update_link->hide();

    switch (result.status) {
    case UpdateCheckResult::Status::UpdateAvailable:
        ui->label_update_result->setText(
            tr("A newer version is available: %1.").arg(result.latestVersion));
        setLink(ui->label_update_link, AppConstants::Urls::LatestReleasePage, tr("Release page"));
        ui->label_update_link->show();
        break;
    case UpdateCheckResult::Status::UpToDate:
        ui->label_update_result->setText(tr("No updates available."));
        break;
    case UpdateCheckResult::Status::Failed:
    default:
        ui->label_update_result->setText(
            result.detail.isEmpty() ? tr("Could not check for updates.")
                                    : tr("Could not check for updates: %1").arg(result.detail));
        break;
    }

    // A successful answer is final: the button gives way to it. A failed
    // check is not an answer, so the button stays and the user can try again
    // without reopening the dialog.
    const bool answered = result.status != UpdateCheckResult::Status::Failed;
    ui->pushButton_check_updates->setVisible(!answered);
    ui->pushButton_check_updates->setEnabled(!answered);

    // The answer can be taller than the indicator it replaces.
    growToFitContent();
}
