/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2025 Rainer Größlinger
 */

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>

class SettingsManager
{
public:
    static SettingsManager& instance();

    // Getters
    QString hashcatPath() const;

    // Setters
    void hashcatPath(const QString& path);

private:
    SettingsManager();
    ~SettingsManager() = default;
    QSettings settings;
};

#endif // SETTINGSMANAGER_H
