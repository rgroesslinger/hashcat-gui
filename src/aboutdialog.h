/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2012-2025 Rainer Größlinger
 */

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QProcess>

namespace Ui {
    class aboutdialog;
}

class aboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit aboutDialog(QWidget *parent = 0);
    ~aboutDialog();
    void get_versions(QMap <QString, QString> &);

private slots:
    void on_pushButton_about_ok_clicked();

    void read_hc();

    void finished_proc_hc();

private:
    Ui::aboutdialog *ui;

    QProcess *proc_hc;
};

#endif // ABOUTDIALOG_H
