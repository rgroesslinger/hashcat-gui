/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef IDENTIFYDIALOG_H
#define IDENTIFYDIALOG_H

#include "hashcatinfoparser.h"
#include "hashcatrunner.h"

#include <QDialog>
#include <QElapsedTimer>
#include <QFutureWatcher>
#include <QLineEdit>
#include <QList>
#include <functional>
#include <memory>
#include <optional>

class QCloseEvent;
class QKeyEvent;
class QTimer;
class QTreeWidgetItem;

namespace Ui
{
class IdentifyDialog;
}

// Rejects multi-line pastes: a hash file dropped into the hash field is not
// one hash.
class HashInputLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit HashInputLineEdit(QWidget *parent = nullptr);

signals:
    void multiLinePasteRejected();
};

// Read-only `hashcat --identify` query; results are candidates the user
// applies explicitly.
class IdentifyDialog : public QDialog
{
    Q_OBJECT

public:
    using Query = HashcatRunner::Query;

    // Applies the chosen mode (and the edited input, when present) to the
    // attack form. False keeps the dialog open: the handler owns the feedback.
    using ApplyHandler =
        std::function<bool(int hashTypeId, const std::optional<QString> &inputText)>;

    explicit IdentifyDialog(QWidget *parent = nullptr, Query query = HashcatRunner::execute);
    ~IdentifyDialog() override;

    IdentifyDialog(const IdentifyDialog &) = delete;
    IdentifyDialog &operator=(const IdentifyDialog &) = delete;
    IdentifyDialog(IdentifyDialog &&) = delete;
    IdentifyDialog &operator=(IdentifyDialog &&) = delete;

    // Prefilled from the attack form's "Hash file or hash:" field.
    void setInput(const QString &input);
    void setApplyHandler(ApplyHandler handler);

protected:
    void closeEvent(QCloseEvent *event) override;
    void reject() override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void browseClicked();
    void identifyClicked();
    void cancelClicked();
    void applyClicked();
    void closeClicked();

private:
    void setBusy(bool busy);
    void showMessage(const QString &message);
    void showResults(const QList<HashcatInfoParser::IdentifiedHashMode> &matches);
    void updateApplyButton();
    void cancelQuery();
    [[nodiscard]] bool inputChanged() const;
    [[nodiscard]] static QString queryErrorMessage(const QString &standardError);

    Ui::IdentifyDialog *ui;
    Query m_query;
    ApplyHandler m_applyHandler;
    std::unique_ptr<QFutureWatcher<HashcatResult>> m_watcher;
    HashcatRunner::CancelToken m_cancel;
    QString m_originalInput;
    QString m_queriedInput;
    QElapsedTimer m_queryClock;
    QTimer *m_progressTimer = nullptr;
    bool m_busy = false;
    bool m_hasResults = false;
};

#endif // IDENTIFYDIALOG_H
