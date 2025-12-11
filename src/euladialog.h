/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2012-2025 Rainer Größlinger
 */

#ifndef EULADIALOG_H
#define EULADIALOG_H

#include <QDialog>
#include <QProcess>

namespace Ui {
    class eulaDialog;
}

class eulaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit eulaDialog(QWidget *parent = 0);
    ~eulaDialog();
    void get_eula(QMap <QString, QString> &, quint16);

private slots:

    void on_pushButton_eula_ok_clicked();

    void readFromStdout();

    void finished_proc();

    void proc_error(QProcess::ProcessError);

private:
    Ui::eulaDialog *ui;

    QProcess *proc_hc;
};

#endif // EULADIALOG_H
