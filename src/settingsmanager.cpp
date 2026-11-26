/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "settingsmanager.h"

#include <QCoreApplication>
#include <QFile>

namespace
{
std::unique_ptr<QSettings> makeSettings()
{
    return std::make_unique<QSettings>(QSettings::IniFormat, QSettings::UserScope,
                                       QCoreApplication::applicationName(),
                                       QStringLiteral("settings"));
}

QString syncErrorText(QSettings::Status status, const QString &file)
{
    switch (status) {
    case QSettings::AccessError:
        return QCoreApplication::translate(
                   "SettingsManager",
                   "The settings could not be written to “%1”. Check the file permissions.")
            .arg(file);
    case QSettings::FormatError:
        return QCoreApplication::translate("SettingsManager",
                                           "The settings file “%1” is not in a readable format.")
            .arg(file);
    case QSettings::NoError:
        break;
    }
    return {};
}
} // namespace

SettingsManager::SettingsManager() : m_settings(makeSettings()) {}

SettingsManager &SettingsManager::instance()
{
    static SettingsManager instance;
    return instance;
}

void SettingsManager::setKey(const QString &key, const QVariant &value)
{
    m_pending.insert(key, value);
}

void SettingsManager::discardPending()
{
    m_pending.clear();
}

bool SettingsManager::moveFileAside(QString *backupPath, QString *errorMessage)
{
    if (backupPath) {
        backupPath->clear();
    }
    if (errorMessage) {
        errorMessage->clear();
    }

    // Bump the suffix until the backup name is free.
    const QString path = fileName();
    QString backup = path + QStringLiteral(".bak");
    for (int suffix = 1; QFile::exists(backup); ++suffix) {
        backup = path + QStringLiteral(".bak.%1").arg(suffix);
    }
    if (!QFile::rename(path, backup)) {
        if (errorMessage) {
            *errorMessage = QCoreApplication::translate(
                                "SettingsManager", "The settings file could not be moved to “%1”.")
                                .arg(backup);
        }
        return false;
    }
    if (backupPath) {
        *backupPath = backup;
    }
    return true;
}

QSettings::Status SettingsManager::lastStatus() const
{
    return m_lastStatus;
}

bool SettingsManager::sync(QString *errorMessage)
{
    if (errorMessage) {
        errorMessage->clear();
    }
    m_lastStatus = QSettings::NoError;
    if (m_pending.isEmpty()) {
        return true;
    }

    // A fresh instance reads the file as it is now.
    QSettings writer(QSettings::IniFormat, QSettings::UserScope,
                     QCoreApplication::applicationName(), QStringLiteral("settings"));

    // Read first: writing a malformed file would keep only the parsed part
    // before reporting the error.
    (void)writer.allKeys();
    if (writer.status() != QSettings::NoError) {
        m_lastStatus = writer.status();
        if (errorMessage) {
            *errorMessage = syncErrorText(m_lastStatus, fileName());
        }
        return false;
    }

    for (auto it = m_pending.cbegin(); it != m_pending.cend(); ++it) {
        writer.setValue(it.key(), it.value());
    }
    writer.sync();
    m_lastStatus = writer.status();
    if (m_lastStatus != QSettings::NoError) {
        if (errorMessage) {
            *errorMessage = syncErrorText(m_lastStatus, fileName());
        }
        return false;
    }

    m_pending.clear();
    // Start the read instance again from the file just written.
    m_settings = makeSettings();
    return true;
}

QString SettingsManager::fileName() const
{
    return m_settings->fileName();
}