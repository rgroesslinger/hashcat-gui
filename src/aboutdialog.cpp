/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "helperutils.h"
#include "settingsmanager.h"
#include "appconstants.h"
#include <QApplication>
#include <QClipboard>
#include <QCursor>
#include <QFileInfo>
#include <QFont>
#include <QFutureWatcher>
#include <QShowEvent>
#include <QLabel>
#include <QPixmap>
#include <QSysInfo>
#include <QStringList>
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

// Wraps the text into lines of at most width pixels.
//
// A label wraps its text at word boundaries only and does not tell its
// layout how tall the result becomes. The version text is therefore wrapped
// here - a word longer than the line is broken as well - and handed to the
// label with the line breaks in it. The label can then report its size, and
// the dialog grows with the text instead of cutting it off.
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

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    // hashcat's answer is not markup: a path or an error with "<" in it would
    // be read as a tag and disappear from the label.
    ui->label_hc_version->setTextFormat(Qt::PlainText);

    updateVersionLabel();
    updateSystemLabels();
    updateLinks();
    updateLicense();

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

    connect(ui->pushButton_ok, &QPushButton::clicked, this, &AboutDialog::okClicked);
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

    // The content decides how large the dialog is; the user may then make it
    // whatever size they like.
    resize(sizeHint());
}

void AboutDialog::okClicked()
{
    close();
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
    QToolTip::showText(QCursor::pos(), tr("Copied to the clipboard"), nullptr, {}, 2000);
}

// What a bug report needs: the version of the application, of the toolkit
// and of the system, and the hashcat it drives.
//
// The text goes into bug reports, so it is not translated - a report that
// mixes languages is harder to read.
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

// The license of the application.
//
// hashcat is a program of its own, with a license of its own; the dialog does
// not distribute it, so it does not state its terms either.
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

    if (path.isEmpty()) {
        // No hashcat is configured, so there is no version to show. The
        // details of a bug report say the same.
        setVersionLabel(tr("not configured"));
    } else {
        ui->label_hc_version_text->setText(QFileInfo(path).fileName());

        QFutureWatcher<HashcatResult> *watcher = new QFutureWatcher<HashcatResult>(this);
        connect(watcher, &QFutureWatcher<HashcatResult>::finished, this, [this, watcher]() {
            const HashcatResult &result = watcher->result();

            if (result.exitStatus != QProcess::NormalExit || result.exitCode != 0) {
                setVersionError(result.standardError.simplified());
            } else {
                setVersionText(result.standardOutput.simplified());
            }

            watcher->deleteLater();
        });

        watcher->setFuture(
            HelperUtils::executeHashcat(QStringList() << AppConstants::Hashcat::Version));
    }

    ui->label_hc_gui_version->setText(QApplication::applicationVersion());
}

// Shows the version of hashcat.
void AboutDialog::setVersionText(const QString &text)
{
    m_hashcatVersion = text;
    setVersionLabel(text);
}

// The error hashcat answered with: it goes into the label, not into the
// version the details report.
void AboutDialog::setVersionError(const QString &error)
{
    m_hashcatVersion.clear();
    setVersionLabel(tr("Error: %1").arg(error));
}

// The text is wrapped before it goes into the label, because the label
// neither breaks words that are longer than its line nor tells its layout
// how tall the wrapped text is.
void AboutDialog::setVersionLabel(const QString &text)
{
    // The label has a fixed width, so the text is wrapped at the width it
    // will be shown in.
    const int width = ui->label_hc_version->minimumWidth();
    ui->label_hc_version->setText(wrapText(text, ui->label_hc_version->font(), width));
}
