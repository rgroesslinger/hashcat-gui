/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appearance.h"

#include "appconstants.h"

#include <QGuiApplication>
#include <QStyleHints>

void Appearance::applyColorScheme(const QString &scheme)
{
    QStyleHints *hints = QGuiApplication::styleHints();
    if (!hints) {
        return;
    }

    if (scheme == AppConstants::Appearance::ColorSchemeDark) {
        hints->setColorScheme(Qt::ColorScheme::Dark);
    } else if (scheme == AppConstants::Appearance::ColorSchemeLight) {
        hints->setColorScheme(Qt::ColorScheme::Light);
    } else {
        // The system default: the desktop decides again.
        hints->unsetColorScheme();
    }
}