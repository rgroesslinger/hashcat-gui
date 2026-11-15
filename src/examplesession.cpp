/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "examplesession.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QSaveFile>
#include <QStandardPaths>
#include <utility>

const Examples::Entry *ExampleSession::find(AttackMode mode)
{
    for (const Examples::Entry &candidate : Examples::All) {
        if (candidate.attackMode == mode) {
            return &candidate;
        }
    }
    return nullptr;
}

bool ExampleSession::containsExamplePrefix(const QJsonValue &value)
{
    if (value.isString()) {
        return value.toString().contains(QString(Examples::DataPrefix));
    }
    if (value.isObject()) {
        const QJsonObject object = value.toObject();
        for (auto it = object.constBegin(); it != object.constEnd(); ++it) {
            if (containsExamplePrefix(it.value())) {
                return true;
            }
        }
    }
    if (value.isArray()) {
        const QJsonArray array = value.toArray();
        for (const auto &entry : array) {
            if (containsExamplePrefix(entry)) {
                return true;
            }
        }
    }
    return false;
}

QJsonValue ExampleSession::withExamplePaths(const QJsonValue &value, const QString &directory)
{
    if (value.isString()) {
        QString text = value.toString();
        text.replace(QString(Examples::DataPrefix), directory);
        return text;
    }
    if (value.isObject()) {
        const QJsonObject object = value.toObject();
        QJsonObject result;
        for (auto it = object.constBegin(); it != object.constEnd(); ++it) {
            result.insert(it.key(), withExamplePaths(it.value(), directory));
        }
        return result;
    }
    if (value.isArray()) {
        const QJsonArray array = value.toArray();
        QJsonArray result;
        for (const auto &entry : array) {
            result.append(withExamplePaths(entry, directory));
        }
        return result;
    }
    return value;
}

bool ExampleSession::materialize(QString *directory, QString *error)
{
    const QString target = QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))
                               .filePath(QString(Examples::DataDirectory));
    if (!QDir().mkpath(target)) {
        *error = tr("Could not create the example directory “%1”.").arg(target);
        return false;
    }

    for (const QLatin1StringView &resource : Examples::DataFiles) {
        QFile source{QString(resource)};
        if (!source.open(QIODevice::ReadOnly)) {
            *error = tr("Could not read the bundled file “%1”: %2")
                         .arg(QString(resource), source.errorString());
            return false;
        }

        QSaveFile destination{QDir(target).filePath(QFileInfo(QString(resource)).fileName())};
        if (!destination.open(QIODevice::WriteOnly)) {
            *error = tr("Could not write “%1”: %2")
                         .arg(destination.fileName(), destination.errorString());
            return false;
        }

        const QByteArray data = source.readAll();
        if (destination.write(data) != data.size() || !destination.commit()) {
            *error = tr("Could not write “%1”: %2")
                         .arg(destination.fileName(), destination.errorString());
            return false;
        }
    }

    QString prefix = QDir::toNativeSeparators(target);
    if (!prefix.endsWith(QDir::separator())) {
        prefix += QDir::separator();
    }
    *directory = std::move(prefix);
    return true;
}