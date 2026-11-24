/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "identifydialog.h"

#include "appconstants.h"
#include "executable.h"
#include "hashcatinfoparser.h"
#include "hashcatrunner.h"
#include "potfilequery.h"
#include "settingsmanager.h"
#include "ui_identifydialog.h"

#include <QFileDialog>
#include <QFutureWatcher>
#include <QHeaderView>
#include <QKeyEvent>
#include <QProcess>
#include <QPushButton>
#include <QStringList>
#include <QTimer>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <memory>
#include <utility>

namespace
{

// The mode column holds ids, not numbers-as-text: 1000 must sort after 999.
class ModeItem : public QTreeWidgetItem
{
public:
    using QTreeWidgetItem::QTreeWidgetItem;

    bool operator<(const QTreeWidgetItem &other) const override
    {
        const int column = treeWidget() != nullptr ? treeWidget()->sortColumn() : 0;
        if (column == 0) {
            return data(0, Qt::UserRole).toInt() < other.data(0, Qt::UserRole).toInt();
        }
        return QTreeWidgetItem::operator<(other);
    }
};

} // namespace

HashInputLineEdit::HashInputLineEdit(QWidget *parent) : QLineEdit(parent)
{
    // textEdited catches every user insertion path: keyboard paste,
    // middle-click, context-menu Paste and drops.
    connect(this, &QLineEdit::textEdited, this, [this] {
        QString text = this->text();
        if (!text.contains(QLatin1Char('\n')) && !text.contains(QLatin1Char('\r'))) {
            return;
        }
        // One trailing break (\r\n, \n or \r) belongs to the copied line.
        if (text.endsWith(QLatin1StringView("\r\n"))) {
            text.chop(2);
        } else if (text.endsWith(QLatin1Char('\n')) || text.endsWith(QLatin1Char('\r'))) {
            text.chop(1);
        }
        if (text.contains(QLatin1Char('\n')) || text.contains(QLatin1Char('\r'))) {
            undo();
            emit multiLinePasteRejected();
            return;
        }
        setText(text);
    });
}

IdentifyDialog::IdentifyDialog(QWidget *parent, Query query)
    : QDialog(parent), ui(new Ui::IdentifyDialog), m_query(std::move(query)),
      m_progressTimer(new QTimer(this))
{
    ui->setupUi(this);
    ui->label_status->setTextFormat(Qt::PlainText);
    ui->label_status->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->treeWidget_results->setRootIsDecorated(false);
    ui->treeWidget_results->setAlternatingRowColors(true);
    ui->treeWidget_results->header()->setSectionResizeMode(QHeaderView::Interactive);
    ui->treeWidget_results->header()->setSortIndicator(0, Qt::AscendingOrder);

    // Enter applies the selection; key events reach the viewport too.
    ui->treeWidget_results->installEventFilter(this);
    ui->treeWidget_results->viewport()->installEventFilter(this);

    m_progressTimer->setInterval(1000);
    connect(m_progressTimer, &QTimer::timeout, this, [this] {
        ui->label_elapsed->setText(tr("Elapsed: %1 s").arg(m_queryClock.elapsed() / 1000));
    });

    connect(ui->pushButton_browse, &QPushButton::clicked, this, &IdentifyDialog::browseClicked);
    connect(ui->pushButton_identify, &QPushButton::clicked, this, &IdentifyDialog::identifyClicked);
    connect(ui->pushButton_cancel, &QPushButton::clicked, this, &IdentifyDialog::cancelClicked);
    connect(ui->pushButton_apply, &QPushButton::clicked, this, &IdentifyDialog::applyClicked);
    connect(ui->pushButton_close, &QPushButton::clicked, this, &IdentifyDialog::closeClicked);
    connect(ui->lineEdit_input, &HashInputLineEdit::multiLinePasteRejected, this, [this] {
        showMessage(
            tr("Multiple lines pasted. Use Browse to select a hash file, or paste one hash."));
    });
    connect(ui->treeWidget_results, &QTreeWidget::itemSelectionChanged, this, [this] {
        ui->pushButton_apply->setEnabled(m_hasResults
                                         && ui->treeWidget_results->currentItem() != nullptr);
    });
    // Stale results would apply an old mode to a new hash. A rejected paste
    // is undone within the same event turn, so judge the settled text against
    // the queried one, not the intermediate state.
    connect(ui->lineEdit_input, &QLineEdit::textChanged, this, [this] {
        updateApplyButton();
        QMetaObject::invokeMethod(
            this,
            [this] {
                if (!m_hasResults || m_busy
                    || ui->lineEdit_input->text().trimmed() == m_queriedInput) {
                    return;
                }
                m_hasResults = false;
                ui->treeWidget_results->clear();
                ui->pushButton_apply->setEnabled(false);
                showMessage(tr("Input changed. Identify it again."));
            },
            Qt::QueuedConnection);
    });
}

IdentifyDialog::~IdentifyDialog()
{
    cancelQuery();
    delete ui;
}

void IdentifyDialog::setInput(const QString &input)
{
    ui->lineEdit_input->setText(input);
    m_originalInput = input.trimmed();
    updateApplyButton();
}

void IdentifyDialog::setApplyHandler(ApplyHandler handler)
{
    m_applyHandler = std::move(handler);
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

    // Only an existing path is a path: bcrypt hashes contain '/'.
    const QString input = PotfileQuery::resolveHashInput(entered, resolved);
    m_queriedInput = entered;

    // Own session: the default pidfile is shared with a running attack.
    QStringList args;
    args << AppConstants::Hashcat::Identify << AppConstants::Hashcat::LogfileDisable
         << QStringLiteral("--session=%1")
                .arg(HashcatRunner::uniqueSession(AppConstants::Hashcat::IdentifySessionPrefix));
    if (input.startsWith(QLatin1Char('-'))) {
        args << AppConstants::Hashcat::OptionTerminator;
    }
    args << input;

    cancelQuery();
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

        // "No match" goes to stderr with exit 255; parse both streams.
        const QString reply = result.standardOutput + QStringLiteral("\n") + result.standardError;
        const HashcatInfoParser::IdentifyResult parsed = HashcatInfoParser::parseIdentify(reply);
        if (parsed.status == HashcatInfoParser::IdentifyResult::Status::Matched) {
            showResults(parsed.matches);
            return;
        }
        if (parsed.status == HashcatInfoParser::IdentifyResult::Status::NoMatch) {
            showMessage(tr("No hash modes match this structure."));
            return;
        }
        showMessage(queryErrorMessage(result.standardError));
    });

    m_watcher->setFuture(m_query(resolved, args, AppConstants::Hashcat::QueryTimeoutMs, m_cancel));
}

void IdentifyDialog::showResults(const QList<HashcatInfoParser::IdentifiedHashMode> &matches)
{
    m_hasResults = !matches.isEmpty();
    ui->treeWidget_results->setSortingEnabled(false);
    ui->treeWidget_results->clear();
    ui->pushButton_apply->setEnabled(false);
    if (matches.isEmpty()) {
        showMessage(tr("No hash modes match this structure."));
        return;
    }

    for (const HashcatInfoParser::IdentifiedHashMode &match : matches) {
        auto *item =
            new ModeItem(QStringList{QString::number(match.id), match.name, match.category});
        item->setData(0, Qt::UserRole, static_cast<int>(match.id));
        ui->treeWidget_results->addTopLevelItem(item);
    }
    ui->treeWidget_results->setSortingEnabled(true);
    // One fit after populating; the columns stay draggable afterwards.
    for (int column = 0; column < ui->treeWidget_results->columnCount(); ++column) {
        ui->treeWidget_results->resizeColumnToContents(column);
    }
    if (matches.size() == 1) {
        ui->treeWidget_results->setCurrentItem(ui->treeWidget_results->topLevelItem(0));
    }

    const bool changed = inputChanged();
    if (matches.size() == 1) {
        showMessage(changed ? tr("1 hash mode matches this structure. Select it and apply the mode "
                                 "and hash to the attack.")
                            : tr("1 hash mode matches this structure. Select it and apply the mode "
                                 "to the attack."));
    } else {
        showMessage(changed
                        ? tr("%1 hash modes match this structure. Select one and apply the mode "
                             "and hash to the attack.")
                              .arg(matches.size())
                        : tr("%1 hash modes match this structure. Select one and apply the mode "
                             "to the attack.")
                              .arg(matches.size()));
    }
}

void IdentifyDialog::cancelClicked()
{
    cancelQuery();
    // The finished handler re-enables Identify once the runner has killed and
    // reaped the process. Re-enabling now would let a second query hit
    // hashcat's single-instance pid lock while this one is still alive.
    ui->pushButton_cancel->setEnabled(false);
    showMessage(tr("Cancelling…"));
}

void IdentifyDialog::applyClicked()
{
    QTreeWidgetItem *item = ui->treeWidget_results->currentItem();
    if (item == nullptr || !m_applyHandler) {
        return;
    }
    // Keep the user's text; never write back a resolved path.
    const QString raw = ui->lineEdit_input->text().trimmed();
    const std::optional<QString> inputText =
        inputChanged() ? std::optional<QString>(raw) : std::nullopt;
    if (m_applyHandler(item->data(0, Qt::UserRole).toInt(), inputText)) {
        accept();
    }
}

void IdentifyDialog::closeClicked()
{
    cancelQuery();
    accept();
}

void IdentifyDialog::closeEvent(QCloseEvent *event)
{
    cancelQuery();
    QDialog::closeEvent(event);
}

void IdentifyDialog::reject()
{
    cancelQuery();
    QDialog::reject();
}

bool IdentifyDialog::eventFilter(QObject *watched, QEvent *event)
{
    if ((watched == ui->treeWidget_results || watched == ui->treeWidget_results->viewport())
        && event->type() == QEvent::KeyPress) {
        const auto *key = dynamic_cast<const QKeyEvent *>(event);
        if (key != nullptr && (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter)) {
            if (ui->treeWidget_results->currentItem() != nullptr) {
                applyClicked();
            }
            return true;
        }
    }
    return QDialog::eventFilter(watched, event);
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
    ui->progressBar_busy->setVisible(busy);
    ui->label_elapsed->setVisible(busy);
    if (busy) {
        m_queryClock.start();
        ui->label_elapsed->setText(tr("Elapsed: %1 s").arg(0));
        m_progressTimer->start();
    } else {
        m_progressTimer->stop();
    }
}

void IdentifyDialog::showMessage(const QString &message)
{
    ui->label_status->setText(message);
}

void IdentifyDialog::updateApplyButton()
{
    ui->pushButton_apply->setText(inputChanged() ? tr("Apply mode and hash to attack")
                                                 : tr("Apply mode to attack"));
}

void IdentifyDialog::cancelQuery()
{
    if (m_cancel) {
        m_cancel->store(true);
    }
}

bool IdentifyDialog::inputChanged() const
{
    return ui->lineEdit_input->text().trimmed() != m_originalInput;
}

QString IdentifyDialog::queryErrorMessage(const QString &standardError)
{
    if (standardError.contains(QStringLiteral("Already an instance"))) {
        return tr("Another hashcat instance is using this session. Close it and try again.");
    }
    if (standardError.contains(QStringLiteral("No hashes loaded"))) {
        return tr("hashcat could not load any hashes from the input.");
    }
    if (standardError.contains(QStringLiteral("Token encoding exception"))) {
        return tr("The input contains characters hashcat cannot encode.");
    }
    const QString detail = standardError.trimmed();
    if (!detail.isEmpty()) {
        return tr("hashcat reported:\n%1").arg(detail);
    }
    return tr("hashcat's output could not be parsed. Check the hashcat version or report a bug.");
}
