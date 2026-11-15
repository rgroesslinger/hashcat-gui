/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef MAINWINDOWTESTACCESS_H
#define MAINWINDOWTESTACCESS_H

#include "hashcatoptions.h"
#include "mainwindow.h"
#include "statefile.h"

#include <QJsonObject>
#include <QSize>
#include <QStringList>

namespace Ui
{
class MainWindow;
}

// The one test seam into MainWindow's private surface: the friend declaration
// in mainwindow.h names this struct, and every entry here is deleted in the
// commit that turns it into a public unit API. No production API exists only
// for a test.
struct MainWindowTestAccess {
    static Ui::MainWindow *ui(MainWindow &window)
    {
        return window.ui;
    }

    static HashcatOptions collectHashcatOptions(MainWindow &window)
    {
        return window.collectHashcatOptions();
    }

    static QStringList generateArguments(MainWindow &window)
    {
        return window.generateArguments();
    }

    static void saveSession(MainWindow &window)
    {
        window.saveSession();
    }

    static QString sessionFile(MainWindow &window)
    {
        return window.sessionFile();
    }

    static QStringList applySessionFile(MainWindow &window, const StateFile::Document &document)
    {
        return window.applySessionFile(document);
    }

    static void addPcfgRulesetNameClicked(MainWindow &window)
    {
        window.addPcfgRulesetNameClicked();
    }

    static void initHashAndAttackModes(MainWindow &window)
    {
        window.initHashAndAttackModes();
    }

    static void fitToContent(MainWindow &window)
    {
        window.fitToContent();
    }

    static QSize attackModeEnvelope(MainWindow &window, int wrapWidth)
    {
        return window.attackModeEnvelope(wrapWidth);
    }

    static void addPathsToList(QListWidget *list, const QStringList &paths)
    {
        MainWindow::addPathsToList(list, paths);
    }

    static QFutureWatcher<HashcatResult> *&hashTypeWatcher(MainWindow &window)
    {
        return window.m_hashTypeWatcher;
    }

    static int &pendingHashTypeId(MainWindow &window)
    {
        return window.m_pendingHashTypeId;
    }

    static int previewRebuilds(MainWindow &window)
    {
        return window.m_previewRebuilds;
    }
};

#endif // MAINWINDOWTESTACCESS_H