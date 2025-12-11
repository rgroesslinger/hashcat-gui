/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2012-2025 Rainer Größlinger
 */

#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QProcess>
#include <QMessageBox>

aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutdialog)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
}

aboutDialog::~aboutDialog()
{
    delete ui;
}

void aboutDialog::on_pushButton_about_ok_clicked()
{
    this->close();
}

void aboutDialog::get_versions(QMap <QString, QString> &info) {

#if defined(Q_WS_WIN)
    QString exec_hc = info.value("dir_hc") + info.value("cmd_hc");
#else
    QString exec_hc = info.value("dir_current") + info.value("cmd_hc");
#endif

    if(info.value("cmd_hc").length() > 0) {
        proc_hc = new QProcess(this);
        proc_hc->setWorkingDirectory(info.value("dir_hc"));

        connect(proc_hc, SIGNAL(readyRead()), this, SLOT(read_hc()) );
        connect(proc_hc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished_proc_hc()) );

        proc_hc->start(exec_hc, QStringList() << "--version");
        ui->label_hc_version_text->setText(info.value("cmd_hc"));
    }

    ui->label_hc_gui_version->setText(QApplication::applicationVersion());
}

void aboutDialog::read_hc()
{
    while (proc_hc->canReadLine())
        ui->label_hc_version->setText(proc_hc->readLine().replace("\n", ""));
}

void aboutDialog::finished_proc_hc() {
    this->proc_hc = NULL;
}
