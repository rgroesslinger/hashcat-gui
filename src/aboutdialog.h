/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

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
    void updateVersionLabel();

    // Shows the version of hashcat. The text is wrapped, so a long message
    // grows the dialog instead of being cut off.
    void setVersionText(const QString &text);

protected:
    // The dialog opens as large as its content. The height of the
    // word-wrapped labels is only known once they have been polished, so the
    // fit happens when the dialog is shown - and only then, so that a window
    // the user has resized is left alone.
    void showEvent(QShowEvent *event) override;

private slots:
    void okClicked();
    void copyDetailsClicked();

private:
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

    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
