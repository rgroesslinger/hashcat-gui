/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

class QTimer;
class UpdateChecker;
struct UpdateCheckResult;

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

protected:
    // The dialog opens as large as its content. The height of the
    // word-wrapped labels is only known once they have been polished, so the
    // fit happens when the dialog is shown - once per open, so that a window
    // the user has resized in the meantime is left alone.
    void showEvent(QShowEvent *event) override;

private slots:
    void copyDetailsClicked();

private:
    // The tests read the dialog through the Ui the same way the main
    // window's tests do: friendship keeps it private instead of widening the
    // dialog's public surface for their sake.
    friend class TestAboutDialog;

    // The update checker is replaceable so a test can answer without a
    // network. Production uses the constructor above and gets a checker of
    // its own.
    AboutDialog(UpdateChecker *checker, int indicatorDelayMs, QWidget *parent = nullptr);

    // Fills the row that holds the version of hashcat.
    void updateVersionLabel();

    // Wires the update check: a one-shot timer starts the request after the
    // progress indicator has been on screen for a moment.
    void setupUpdateCheck(UpdateChecker *checker, int indicatorDelayMs);
    void startUpdateCheck();
    void showUpdateResult(const UpdateCheckResult &result);

    // Shows the version of hashcat. The text is wrapped, so a long message
    // grows the dialog instead of being cut off.
    void setVersionText(const QString &text);

    // Fills the rows that describe the system the application runs on.
    void updateSystemLabels();

    // Fills the row of links to the project and to hashcat.
    void updateLinks();

    // Fills the row with the license of the application.
    void updateLicense();

    // Shows the error hashcat answered with in place of the version.
    void setVersionError(const QString &error);

    // Wraps the text into the label at the width the label shows.
    void setVersionLabel(const QString &text);

    // What a bug report needs: the versions and the hashcat in use.
    QString detailsText() const;

    // The version of hashcat - empty while the query is in flight and after
    // an error. The label shows the version or the error wrapped; this keeps
    // the version as it is.
    QString m_hashcatVersion;

    // The dialog is fitted to its content once, on the first show.
    bool m_sizedToContent = false;

    // The update check: the timer keeps the progress indicator visible for
    // at least its interval, and the flag makes sure an About window that is
    // shown again does not ask GitHub a second time.
    QTimer *m_updateTimer = nullptr;
    UpdateChecker *m_updateChecker = nullptr;
    bool m_updateCheckStarted = false;

    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
