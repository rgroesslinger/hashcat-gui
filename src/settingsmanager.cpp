/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "settingsmanager.h"
#include <QCoreApplication>

SettingsManager::SettingsManager()
    : settings(QSettings::NativeFormat, QSettings::UserScope, QCoreApplication::applicationName(),
               QStringLiteral("settings"))
{
}

SettingsManager &SettingsManager::instance()
{
    static SettingsManager instance;
    return instance;
}

void SettingsManager::setKey(const QString &key, const QVariant &value)
{
    settings.setValue(key, value);
}

QString SettingsManager::fileName() const
{
    return settings.fileName();
}
