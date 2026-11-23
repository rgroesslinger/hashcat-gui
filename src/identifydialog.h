/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef IDENTIFYDIALOG_H
#define IDENTIFYDIALOG_H

#include "hashcatinfoparser.h"
#include "hashcatrunner.h"

#include <QDialog>
#include <QFutureWatcher>
#include <QList>
#include <memory>

class QTreeWidgetItem;

namespace Ui
{
class IdentifyDialog;
}

// Read-only `hashcat --identify` query; results are candidates the user
// applies explicitly.
class IdentifyDialog : public QDialog
{
    Q_OBJECT

public:
    using Query = HashcatRunner::Query;

    explicit IdentifyDialog(QWidget *parent = nullptr, Query query = HashcatRunner::execute);
    ~IdentifyDialog() override;

    IdentifyDialog(const IdentifyDialog &) = delete;
    IdentifyDialog &operator=(const IdentifyDialog &) = delete;
    IdentifyDialog(IdentifyDialog &&) = delete;
    IdentifyDialog &operator=(IdentifyDialog &&) = delete;

    // Prefilled from the attack form's "Hash file or hash:" field.
    void setInput(const QString &input);

signals:
    // Carries the queried input (absolute for a path) for the hash field.
    void applyRequested(int hashTypeId, const QString &input);

private slots:
    void browseClicked();
    void identifyClicked();
    void cancelClicked();
    void applyClicked();
    void resultDoubleClicked(QTreeWidgetItem *item, int column);

private:
    void setBusy(bool busy);
    void showMessage(const QString &message);
    void showResults(const QList<HashcatInfoParser::IdentifiedHashMode> &matches);

    Ui::IdentifyDialog *ui;
    Query m_query;
    std::unique_ptr<QFutureWatcher<HashcatResult>> m_watcher;
    HashcatRunner::CancelToken m_cancel;
    QString m_input;
    bool m_busy = false;
    bool m_hasResults = false;
};

#endif // IDENTIFYDIALOG_H
