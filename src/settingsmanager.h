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

    /* returns the value converted to T */
    template <typename T>
    T getKey(const QString &key, const T &defaultValue = T()) const
    {
        // QSettings::value() returns a QVariant – we convert it to T
        return settings.value(key, QVariant::fromValue(defaultValue)).template value<T>();
    }

    void setKey(const QString &key, const QVariant &value);

private:
    SettingsManager();
    ~SettingsManager() = default;
    QSettings settings;
};

#endif // SETTINGSMANAGER_H
