/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include <QApplication>
#include "mainwindow.h"
#include "config.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationVersion(GUI_VERSION);
    MainWindow w;
    w.show();

    return a.exec();
}
