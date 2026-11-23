/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef BACKENDINFODIALOG_H
#define BACKENDINFODIALOG_H

#include "hashcatrunner.h"

#include <QDialog>
#include <QFutureWatcher>
#include <memory>

namespace Ui
{
class BackendInfoDialog;
}

// Read-only `hashcat --backend-info` query; builds its own argv, so no
// attack pipeline or configured terminal is needed.
class BackendInfoDialog : public QDialog
{
    Q_OBJECT

public:
    // Injectable for tests; the caller resolves the path on the GUI thread.
    using Query = HashcatRunner::Query;

    explicit BackendInfoDialog(QWidget *parent = nullptr, Query query = HashcatRunner::execute);
    ~BackendInfoDialog() override;

    BackendInfoDialog(const BackendInfoDialog &) = delete;
    BackendInfoDialog &operator=(const BackendInfoDialog &) = delete;
    BackendInfoDialog(BackendInfoDialog &&) = delete;
    BackendInfoDialog &operator=(BackendInfoDialog &&) = delete;

private slots:
    void refreshClicked();
    void cancelClicked();

private:
    void setBusy(bool busy);
    void showMessage(const QString &message);

    Ui::BackendInfoDialog *ui;
    Query m_query;
    std::unique_ptr<QFutureWatcher<HashcatResult>> m_watcher;
    HashcatRunner::CancelToken m_cancel;
};

#endif // BACKENDINFODIALOG_H
