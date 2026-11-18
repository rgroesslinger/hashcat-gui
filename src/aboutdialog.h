/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

class UpdateChecker;
struct HashcatResult;
struct UpdateCheckResult;

template <typename T> class QFutureWatcher;

namespace Ui
{
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog() override;

    AboutDialog(const AboutDialog &) = delete;
    AboutDialog &operator=(const AboutDialog &) = delete;
    AboutDialog(AboutDialog &&) = delete;
    AboutDialog &operator=(AboutDialog &&) = delete;

protected:
    // Fits the dialog to its content on the first show only.
    void showEvent(QShowEvent *event) override;

private slots:
    void copyDetailsClicked();
    void checkUpdatesClicked();

private:
    friend class TestAboutDialog;

    // The update checker is replaceable so a test can answer without a
    // network. Production uses the constructor above and gets a checker of
    // its own.
    AboutDialog(UpdateChecker *checker, QWidget *parent = nullptr);

    void updateVersionLabel();

    // Wires the update row. The check is opt-in: nothing contacts the network
    // until the user presses the button.
    void setupUpdateCheck(UpdateChecker *checker);
    void showUpdateResult(const UpdateCheckResult &result);

    void growToFitContent();

    void setVersionText(const QString &text);

    void updateSystemLabels();

    void updateLinks();

    void updateLicense();

    void setVersionError(const QString &error);

    // Wraps the text into the label at the width the label shows.
    void setVersionLabel(const QString &text);

    [[nodiscard]] QString detailsText() const;

    // The version of hashcat - empty while the query is in flight and after
    // an error. The label shows the version or the error wrapped; this keeps
    // the version as it is.
    QString m_hashcatVersion;

    // The query for the version, a child of the dialog so it is destroyed
    // with it even if the reply never arrives.
    QFutureWatcher<HashcatResult> *m_versionWatcher = nullptr;

    bool m_sizedToContent = false;

    // The check runs only on request, through the button in the update row.
    UpdateChecker *m_updateChecker = nullptr;

    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
