/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef MAINWINDOWTESTACCESS_H
#define MAINWINDOWTESTACCESS_H

#include "hashcatoptions.h"
#include "hashtypeloader.h"
#include "mainwindow.h"
#include "statefile.h"

#include <QJsonObject>
#include <QSize>
#include <QStringList>

#include <memory>
#include <utility>

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

    static void reloadHashTypes(MainWindow &window)
    {
        window.reloadHashTypes();
    }

    static void applyIdentifiedHashType(MainWindow &window, int id, const QString &input)
    {
        window.applyIdentifiedHashType(id, input);
    }

    static HashTypeLoader &hashTypeLoader(MainWindow &window)
    {
        return window.m_hashTypeLoader;
    }

    static PotfileCheck &potfileCheck(MainWindow &window)
    {
        return *window.m_potfileCheck;
    }

    static void setPotfileCheck(MainWindow &window, std::unique_ptr<PotfileCheck> check)
    {
        window.m_potfileCheck = std::move(check);
    }

    static QString hashTypeBaseToolTip(MainWindow &window)
    {
        return window.m_hashTypeBaseToolTip;
    }

    static void fitToContent(MainWindow &window)
    {
        window.fitToContent();
    }

    static QSize attackModeEnvelope(MainWindow &window, int wrapWidth)
    {
        return window.attackModeEnvelope(wrapWidth);
    }

    static int &pendingHashTypeId(MainWindow &window)
    {
        return window.m_pendingHashTypeId;
    }

    static int previewRebuilds(MainWindow &window)
    {
        return window.m_previewRebuilds;
    }

    static QJsonObject defaultSnapshot(MainWindow &window)
    {
        return window.m_defaultSnapshot.value_or(QJsonObject{});
    }
};

#endif // MAINWINDOWTESTACCESS_H