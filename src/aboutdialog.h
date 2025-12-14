/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2012-2025 Rainer Größlinger
 */

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
    class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();
    void get_versions();

private slots:
    void on_pushButton_about_ok_clicked();

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
