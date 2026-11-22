/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "identifydialog.h"

#include "appconstants.h"
#include "executable.h"
#include "hashcatinfoparser.h"
#include "hashcatrunner.h"
#include "settingsmanager.h"
#include "ui_identifydialog.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QFutureWatcher>
#include <QHeaderView>
#include <QProcess>
#include <QPushButton>
#include <QStringList>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <memory>
#include <utility>

IdentifyDialog::IdentifyDialog(QWidget *parent, Query query)
    : QDialog(parent), ui(new Ui::IdentifyDialog), m_query(std::move(query))
{
    ui->setupUi(this);
    ui->label_status->setTextFormat(Qt::PlainText);
    ui->treeWidget_results->setRootIsDecorated(false);
    ui->treeWidget_results->setAlternatingRowColors(true);
    ui->treeWidget_results->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    connect(ui->pushButton_browse, &QPushButton::clicked, this, &IdentifyDialog::browseClicked);
    connect(ui->pushButton_identify, &QPushButton::clicked, this, &IdentifyDialog::identifyClicked);
    connect(ui->pushButton_cancel, &QPushButton::clicked, this, &IdentifyDialog::cancelClicked);
    connect(ui->pushButton_apply, &QPushButton::clicked, this, &IdentifyDialog::applyClicked);
    connect(ui->pushButton_close, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->treeWidget_results, &QTreeWidget::itemDoubleClicked, this,
            &IdentifyDialog::resultDoubleClicked);
    connect(ui->treeWidget_results, &QTreeWidget::itemSelectionChanged, this, [this] {
        ui->pushButton_apply->setEnabled(m_hasResults
                                         && ui->treeWidget_results->currentItem() != nullptr);
    });
    // Stale results would apply an old mode to a new hash.
    connect(ui->lineEdit_input, &QLineEdit::textChanged, this, [this] {
        if (!m_hasResults || m_busy) {
            return;
        }
        m_hasResults = false;
        ui->treeWidget_results->clear();
        ui->pushButton_apply->setEnabled(false);
        showMessage(tr("Input changed. Identify it again."));
    });
}

IdentifyDialog::~IdentifyDialog()
{
    if (m_cancel) {
        m_cancel->store(true);
    }
    delete ui;
}

void IdentifyDialog::setInput(const QString &input)
{
    ui->lineEdit_input->setText(input);
}

void IdentifyDialog::browseClicked()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Select a hash file"));
    if (!path.isEmpty()) {
        ui->lineEdit_input->setText(path);
    }
}

void IdentifyDialog::identifyClicked()
{
    const QString entered = ui->lineEdit_input->text().trimmed();
    if (entered.isEmpty()) {
        showMessage(tr("Enter a hash or select a hash file first."));
        return;
    }

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

    // Only an existing path is a path: bcrypt hashes contain '/'. Make it
    // absolute because hashcat chdirs to its own directory.
    m_input = entered;
    if (QFileInfo::exists(entered)) {
        m_input = QFileInfo(entered).absoluteFilePath();
    }

    QStringList args;
    args << AppConstants::Hashcat::Identify;
    if (m_input.startsWith(QLatin1Char('-'))) {
        args << AppConstants::Hashcat::OptionTerminator;
    }
    args << m_input;

    if (m_cancel) {
        m_cancel->store(true);
    }
    m_watcher = std::make_unique<QFutureWatcher<HashcatResult>>(this);
    m_cancel = std::make_shared<std::atomic_bool>(false);

    m_hasResults = false;
    ui->treeWidget_results->clear();
    ui->pushButton_apply->setEnabled(false);
    showMessage(tr("Identifying hash..."));
    setBusy(true);

    connect(m_watcher.get(), &QFutureWatcher<HashcatResult>::finished, this, [this] {
        setBusy(false);
        const HashcatResult result = m_watcher->result();

        if (result.cancelled) {
            showMessage(tr("Identification cancelled."));
            return;
        }
        if (result.timedOut) {
            showMessage(tr("hashcat timed out while identifying the hash."));
            return;
        }
        if (result.exitStatus != QProcess::NormalExit || result.exitCode != 0) {
            // "No match" goes to stderr with exit 255; parse both streams.
            const QString reply =
                result.standardOutput + QStringLiteral("\n") + result.standardError;
            QString error;
            QList<HashcatInfoParser::IdentifiedHashMode> matches;
            if (HashcatInfoParser::parseIdentify(reply, matches, &error)) {
                showResults(matches);
                return;
            }
            const QString detail = result.standardError.simplified();
            showMessage(detail.isEmpty()
                            ? tr("hashcat could not identify the hash.")
                            : tr("hashcat could not identify the hash:\n%1").arg(detail));
            return;
        }

        QString error;
        QList<HashcatInfoParser::IdentifiedHashMode> matches;
        if (!HashcatInfoParser::parseIdentify(result.standardOutput, matches, &error)) {
            showMessage(error);
            return;
        }
        showResults(matches);
    });

    m_watcher->setFuture(m_query(resolved, args, AppConstants::Hashcat::QueryTimeoutMs, m_cancel));
}

void IdentifyDialog::showResults(const QList<HashcatInfoParser::IdentifiedHashMode> &matches)
{
    m_hasResults = !matches.isEmpty();
    ui->treeWidget_results->clear();
    ui->pushButton_apply->setEnabled(false);
    if (matches.isEmpty()) {
        showMessage(tr("No hash modes match this structure."));
        return;
    }

    for (const HashcatInfoParser::IdentifiedHashMode &match : matches) {
        auto *item =
            new QTreeWidgetItem(QStringList{QString::number(match.id), match.name, match.category});
        item->setData(0, Qt::UserRole, static_cast<int>(match.id));
        ui->treeWidget_results->addTopLevelItem(item);
    }
    for (int column = 0; column < ui->treeWidget_results->columnCount(); ++column) {
        ui->treeWidget_results->resizeColumnToContents(column);
    }
    if (matches.size() == 1) {
        showMessage(
            tr("1 hash mode matches this structure. Select it and apply it to the attack."));
    } else {
        showMessage(tr("%1 hash modes match this structure. Select one and apply it to the attack.")
                        .arg(matches.size()));
    }
}

void IdentifyDialog::cancelClicked()
{
    if (m_cancel) {
        m_cancel->store(true);
    }
    // The finished handler re-enables Identify once the runner has killed and
    // reaped the process. Re-enabling now would let a second query hit
    // hashcat's single-instance pid lock while this one is still alive.
}

void IdentifyDialog::applyClicked()
{
    QTreeWidgetItem *item = ui->treeWidget_results->currentItem();
    if (item == nullptr) {
        return;
    }
    emit applyRequested(item->data(0, Qt::UserRole).toInt(), m_input);
    accept();
}

void IdentifyDialog::resultDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
    applyClicked();
}

void IdentifyDialog::setBusy(bool busy)
{
    m_busy = busy;
    ui->lineEdit_input->setDisabled(busy);
    ui->pushButton_browse->setDisabled(busy);
    ui->pushButton_identify->setEnabled(!busy);
    ui->pushButton_cancel->setEnabled(busy);
    ui->pushButton_apply->setEnabled(!busy && m_hasResults
                                     && ui->treeWidget_results->currentItem() != nullptr);
}

void IdentifyDialog::showMessage(const QString &message)
{
    ui->label_status->setText(message);
}
