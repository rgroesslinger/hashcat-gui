/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>

class SettingsManager
{
public:
    static SettingsManager &instance();

    template <typename T> T getKey(const QString &key, const T &defaultValue = T()) const
    {
        return settings.value(key, QVariant::fromValue(defaultValue)).template value<T>();
    }

    void setKey(const QString &key, const QVariant &value);

    // Where the settings are stored.
    //
    // A file on every platform: QSettings::IniFormat keeps the settings in
    // an INI file on Windows as well, so there is always a path to open.
    QString fileName() const;

private:
    SettingsManager();
    ~SettingsManager() = default;
    QSettings settings;
};

#endif // SETTINGSMANAGER_H
