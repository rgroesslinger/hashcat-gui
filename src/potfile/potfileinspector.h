/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef POTFILEINSPECTOR_H
#define POTFILEINSPECTOR_H

#include "potfileexporter.h"
#include "potfileparser.h"

#include <QDialog>
#include <QFutureWatcher>
#include <QString>

#include <memory>

class QCloseEvent;

namespace Ui
{
class PotfileInspectorDialog;
}

// Read-only browser for an explicit potfile. It parses the file directly, so
// it has no hash type and cannot verify that a plaintext belongs to its hash.
class PotfileInspectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PotfileInspectorDialog(QWidget *parent = nullptr);
    ~PotfileInspectorDialog() override;

    PotfileInspectorDialog(const PotfileInspectorDialog &) = delete;
    PotfileInspectorDialog &operator=(const PotfileInspectorDialog &) = delete;
    PotfileInspectorDialog(PotfileInspectorDialog &&) = delete;
    PotfileInspectorDialog &operator=(PotfileInspectorDialog &&) = delete;

    // Prefills the path from the form. An empty path tries the labelled
    // heuristic candidates next to hashcat and in XDG's data directory.
    void setPotfilePath(const QString &path);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void browseClicked();
    void reloadClicked();
    void pathEditingFinished();
    void exportClicked();
    void statisticsClicked();

private:
    void loadPath();
    void showResult(const PotfileParser::Result &result);
    void clearView();
    void populateRows();
    void updateNote();
    void updateStatus();
    void updateButtons();
    [[nodiscard]] QList<PotfileExporter::Row> resultRows() const;
    [[nodiscard]] QString suggestedPotfilePath() const;
    [[nodiscard]] QString resolvedHashcatPath() const;
    [[nodiscard]] QString absolutePath(const QString &path) const;

    Ui::PotfileInspectorDialog *ui;
    std::unique_ptr<QFutureWatcher<PotfileParser::Result>> m_watcher;
    HashcatRunner::CancelToken m_cancel;
    PotfileParser::Result m_result;
    bool m_heuristic = false;
    bool m_loading = false;
};

#endif // POTFILEINSPECTOR_H