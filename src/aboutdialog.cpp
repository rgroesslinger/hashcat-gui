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
#include <QFileInfo>
#include <QFont>
#include <QFutureWatcher>
#include <QLabel>
#include <QPixmap>
#include <QSysInfo>
#include <QStringList>

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
} // namespace

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    updateVersionLabel();
    updateSystemLabels();
    updateLinks();

    // The logo is scaled to the size the .ui gives the label, so the artwork
    // stays sharp on a high-dpi display without the .ui having to know about
    // the device pixel ratio.
    const QSize logoSize = ui->label_logo->minimumSize();
    const QPixmap logo(logoPath());
    if (!logo.isNull()) {
        ui->label_logo->setPixmap(
            logo.scaled(logoSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    ui->label_logo->setAlignment(Qt::AlignCenter);

    // The application name is the headline of the dialog.
    QFont nameFont = ui->label_app_name->font();
    nameFont.setBold(true);
    ui->label_app_name->setFont(nameFont);

    connect(ui->pushButton_ok, &QPushButton::clicked, this, &AboutDialog::okClicked);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::okClicked()
{
    close();
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

void AboutDialog::updateVersionLabel()
{
    auto &settings = SettingsManager::instance();
    QFileInfo fileInfo(settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath));

    if (!settings.getKey<QString>(AppConstants::SettingsKeys::HashcatPath).isEmpty()) {
        ui->label_hc_version_text->setText(fileInfo.fileName());

        QFutureWatcher<HashcatResult> *watcher = new QFutureWatcher<HashcatResult>(this);
        connect(watcher, &QFutureWatcher<HashcatResult>::finished, this, [this, watcher]() {
            const HashcatResult &result = watcher->result();

            if (result.exitStatus != QProcess::NormalExit || result.exitCode != 0) {
                ui->label_hc_version->setText(
                    tr("Error: %1").arg(result.standardError.simplified()));
            } else {
                ui->label_hc_version->setText(result.standardOutput.simplified());
            }

            watcher->deleteLater();
        });

        watcher->setFuture(
            HelperUtils::executeHashcat(QStringList() << AppConstants::Hashcat::Version));
    }

    ui->label_hc_gui_version->setText(QApplication::applicationVersion());
}
