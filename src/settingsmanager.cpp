/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "settingsmanager.h"

SettingsManager::SettingsManager()
    : settings(QSettings::NativeFormat, QSettings::UserScope,
                 QStringLiteral("hashcat-gui"), QStringLiteral("settings"))
{

}

SettingsManager& SettingsManager::instance()
{
    static SettingsManager instance;
    return instance;
}

// Getter
QString SettingsManager::getKey(const QString& key) const
{
    return settings.value(key, QString()).toString();
}

// Setter
void SettingsManager::setKey(const QString& key, const QString& value)
{
    settings.setValue(key, value);
}
