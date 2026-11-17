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
    // The dialog opens as large as its content. The height of the
    // word-wrapped labels is only known once they have been polished, so the
    // fit happens when the dialog is shown - once per open, so that a window
    // the user has resized in the meantime is left alone.
    void showEvent(QShowEvent *event) override;

private slots:
    void copyDetailsClicked();
    void checkUpdatesClicked();

private:
    // The tests read the dialog through the Ui the same way the main
    // window's tests do: friendship keeps it private instead of widening the
    // dialog's public surface for their sake.
    friend class TestAboutDialog;

    // The update checker is replaceable so a test can answer without a
    // network. Production uses the constructor above and gets a checker of
    // its own.
    AboutDialog(UpdateChecker *checker, QWidget *parent = nullptr);

    // Fills the row that holds the version of hashcat.
    void updateVersionLabel();

    // Wires the update row. The check is opt-in: nothing contacts the network
    // until the user presses the button.
    void setupUpdateCheck(UpdateChecker *checker);
    void showUpdateResult(const UpdateCheckResult &result);

    // Grows the dialog when the update row needs more room. A window the user
    // has resized is left at its size; only the missing space is added.
    void growToFitContent();

    // Shows the version of hashcat.
    void setVersionText(const QString &text);

    // Fills the rows that describe the system the application runs on.
    void updateSystemLabels();

    // Fills the row of links to the project and to hashcat.
    void updateLinks();

    // Fills the row with the license of the application.
    void updateLicense();

    // Shows an error in place of the version.
    void setVersionError(const QString &error);

    // Wraps the text into the label at the width the label shows.
    void setVersionLabel(const QString &text);

    // What a bug report needs: the versions and the hashcat in use.
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
