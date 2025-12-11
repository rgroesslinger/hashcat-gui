/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2025 Rainer Größlinger
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

// Getters
QString SettingsManager::hashcatPath() const
{
    return settings.value(QStringLiteral("hashcatPath"), QString()).toString();
}

// Setters
void SettingsManager::hashcatPath(const QString& path)
{
    settings.setValue(QStringLiteral("hashcatPath"), path);
}
