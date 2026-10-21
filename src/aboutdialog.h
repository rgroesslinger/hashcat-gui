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

    // Shows the version of hashcat, or the error it answered with. The text
    // is wrapped, so a long message grows the dialog instead of being cut
    // off.
    void setVersionText(const QString &text);

private slots:
    void okClicked();

private:
    // Fills the rows that describe the system the application runs on.
    void updateSystemLabels();

    // Fills the row of links to the project and to hashcat.
    void updateLinks();

    // Fills the row with the licenses of the application and of hashcat.
    void updateLicense();

    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
