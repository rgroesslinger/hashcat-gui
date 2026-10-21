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
    // A file on the systems that keep settings in files, the registry on
    // Windows - so the answer is a path only where there is one.
    QString fileName() const;

private:
    SettingsManager();
    ~SettingsManager() = default;
    QSettings settings;
};

#endif // SETTINGSMANAGER_H
