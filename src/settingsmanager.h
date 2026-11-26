/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QHash>
#include <QLatin1String>
#include <QMetaType>
#include <QSettings>
#include <QString>
#include <QVariant>
#include <memory>
#include <type_traits>

class SettingsManager
{
public:
    static SettingsManager &instance();

    template <typename T>
    [[nodiscard]] T getKey(const QString &key, const T &defaultValue = T()) const
    {
        // A set but unsaved value is what the application runs with.
        const QVariant value = m_pending.contains(key)
                                   ? m_pending.value(key)
                                   : m_settings->value(key, QVariant::fromValue(defaultValue));
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

    // Drops the values that were set but not saved.
    void discardPending();

    // Renames the settings file aside to .bak[.N], never deleting it.
    [[nodiscard]] bool moveFileAside(QString *backupPath = nullptr,
                                     QString *errorMessage = nullptr);

    // The result of the last sync(): NoError when the values reached the file.
    [[nodiscard]] QSettings::Status lastStatus() const;

    // Flushes the pending writes and reports a failure. Every save goes
    // through here instead of trusting that the file was written.
    [[nodiscard]] bool sync(QString *errorMessage = nullptr);

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

    std::unique_ptr<QSettings> m_settings;
    QHash<QString, QVariant> m_pending;
    QSettings::Status m_lastStatus = QSettings::NoError;
};

#endif // SETTINGSMANAGER_H
