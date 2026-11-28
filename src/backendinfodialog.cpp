/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "backendinfodialog.h"

#include "appconstants.h"
#include "executable.h"
#include "hashcatrunner.h"
#include "settingsmanager.h"
#include "ui_backendinfodialog.h"

#include <QFontDatabase>
#include <QFutureWatcher>
#include <QKeySequence>
#include <QProcess>
#include <QPushButton>
#include <QStringList>
#include <QTextCursor>

#include <memory>
#include <utility>

BackendInfoDialog::BackendInfoDialog(QWidget *parent, Query query)
    : QDialog(parent), ui(new Ui::BackendInfoDialog), m_query(std::move(query))
{
    ui->setupUi(this);
    ui->label_status->setTextFormat(Qt::PlainText);
    ui->plainTextEdit_output->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    ui->pushButton_refresh->setShortcut(QKeySequence(QStringLiteral("F5")));
    connect(ui->pushButton_refresh, &QPushButton::clicked, this,
            &BackendInfoDialog::refreshClicked);
    connect(ui->pushButton_cancel, &QPushButton::clicked, this, &BackendInfoDialog::cancelClicked);
    connect(ui->pushButton_copy, &QPushButton::clicked, this, [this] {
        ui->plainTextEdit_output->selectAll();
        ui->plainTextEdit_output->copy();
        QTextCursor cursor = ui->plainTextEdit_output->textCursor();
        cursor.clearSelection();
        ui->plainTextEdit_output->setTextCursor(cursor);
    });
    connect(ui->pushButton_close, &QPushButton::clicked, this, &QDialog::accept);

    refreshClicked();
}

BackendInfoDialog::~BackendInfoDialog()
{
    resetHashcatQuery(m_cancel, m_watcher);
    delete ui;
}

void BackendInfoDialog::refreshClicked()
{
    const QString configured =
        SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath);
    if (configured.isEmpty()) {
        showMessage(tr("No hashcat executable is configured. Set one in Settings."));
        return;
    }
    const QString resolved = Executable::resolve(configured);
    if (resolved.isEmpty()) {
        showMessage(tr("The configured hashcat executable is not found:\n%1").arg(configured));
        return;
    }

    // A second refresh kills the earlier query's process, not just its reply.
    resetHashcatQuery(m_cancel, m_watcher);
    m_watcher = std::make_unique<QFutureWatcher<HashcatResult>>(this);
    m_cancel = std::make_shared<std::atomic_bool>(false);

    ui->plainTextEdit_output->clear();
    showMessage(tr("Reading backend information…"));
    setBusy(true);

    connect(m_watcher.get(), &QFutureWatcher<HashcatResult>::finished, this, [this] {
        setBusy(false);
        const HashcatResult result = m_watcher->result();

        if (result.cancelled) {
            showMessage(tr("Backend query cancelled."));
            return;
        }
        if (result.timedOut) {
            showMessage(tr("hashcat timed out while reading backend information."));
            return;
        }
        if (result.exitStatus != QProcess::NormalExit || result.exitCode != 0) {
            const QString detail = result.standardError.simplified();
            showMessage(detail.isEmpty()
                            ? tr("hashcat could not read backend information.")
                            : tr("hashcat could not read backend information:\n%1").arg(detail));
            return;
        }

        // Verbatim: a parsed device model would rot as backends change.
        ui->plainTextEdit_output->setPlainText(result.standardOutput);
        showMessage(tr("Backend information reported by hashcat."));
    });

    // Own session: probes would otherwise collide on hashcat's pidfile.
    QStringList args;
    args << AppConstants::Hashcat::BackendInfo << AppConstants::Hashcat::LogfileDisable
         << QStringLiteral("--session=%1")
                .arg(HashcatRunner::uniqueSession(AppConstants::Hashcat::BackendInfoSessionPrefix));
    m_watcher->setFuture(
        m_query(resolved, args, AppConstants::Hashcat::BackendInfoTimeoutMs, m_cancel));
}

void BackendInfoDialog::cancelClicked()
{
    if (m_cancel) {
        m_cancel->store(true);
    }
    // The finished handler re-enables Refresh once the runner has killed and
    // reaped the process. Re-enabling now would let a second query hit
    // hashcat's single-instance pid lock while this one is still alive.
}

void BackendInfoDialog::setBusy(bool busy)
{
    ui->pushButton_refresh->setEnabled(!busy);
    ui->pushButton_cancel->setEnabled(busy);
}

void BackendInfoDialog::showMessage(const QString &message)
{
    ui->label_status->setText(message);
}
