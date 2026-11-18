/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QLatin1String>
#include <QMetaType>
#include <QSettings>
#include <QString>
#include <QVariant>
#include <type_traits>

class SettingsManager
{
public:
    static SettingsManager &instance();

    template <typename T>
    [[nodiscard]] T getKey(const QString &key, const T &defaultValue = T()) const
    {
        const QVariant value = settings.value(key, QVariant::fromValue(defaultValue));
        // IniFormat stores text. Within one process the cache holds the typed
        // value, but a fresh process reads a QString, so a plain metaType check
        // would drop every bool on restart. Accept "true"/"false" and refuse
        // anything else, so a hand-edited "no" keeps the default instead of
        // becoming true through QVariant's lossy conversion.
        if constexpr (std::is_same_v<T, bool>) {
            if (value.metaType() == QMetaType::fromType<QString>()) {
                const QString text = value.toString();
                if (text == QLatin1String("true")) {
                    return true;
                }
                if (text == QLatin1String("false")) {
                    return false;
                }
                return defaultValue;
            }
        }
        if (value.metaType() != QMetaType::fromType<T>()) {
            return defaultValue;
        }
        return value.value<T>();
    }

    void setKey(const QString &key, const QVariant &value);

    // Where the settings are stored.
    //
    // A file on every platform: QSettings::IniFormat keeps the settings in
    // an INI file on Windows as well, so there is always a path to open.
    [[nodiscard]] QString fileName() const;

    SettingsManager(const SettingsManager &) = delete;
    SettingsManager &operator=(const SettingsManager &) = delete;
    SettingsManager(SettingsManager &&) = delete;
    SettingsManager &operator=(SettingsManager &&) = delete;

private:
    SettingsManager();
    ~SettingsManager() = default;

    QSettings settings;
};

#endif // SETTINGSMANAGER_H
