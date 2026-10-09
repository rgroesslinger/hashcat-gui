/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "hashcatinfoparser.h"

#include <QJsonDocument>
#include <QJsonObject>

namespace
{

void setError(QString *errorMessage, const QString &text)
{
    if (errorMessage) {
        *errorMessage = text;
    }
}

} // namespace

bool HashcatInfoParser::parseExampleHashes(const QString &rawOutput,
                                           QMap<quint32, QString> &hashModes, QString *errorMessage)
{
    setError(errorMessage, QString());

    // The output is machine readable JSON; fromJson skips any whitespace
    const QJsonDocument doc = QJsonDocument::fromJson(rawOutput.toUtf8());
    if (!doc.isObject()) {
        setError(errorMessage, tr("Invalid JSON returned from hashcat."));
        return false;
    }

    QMap<quint32, QString> parsed;
    const QJsonObject root = doc.object();

    for (auto it = root.constBegin(); it != root.constEnd(); ++it) {
        bool isNumber = false;
        const quint32 id = it.key().toUInt(&isNumber);
        if (!isNumber) {
            // Anything that is not a hash type id cannot be passed as -m
            continue;
        }
        const QString name = it.value().toObject().value(QStringLiteral("name")).toString();
        if (name.isEmpty()) {
            continue;
        }
        parsed.insert(id, it.key() + QStringLiteral(" | ") + name);
    }

    if (parsed.isEmpty()) {
        setError(errorMessage, tr("hashcat did not report any hash types."));
        return false;
    }

    hashModes = std::move(parsed);
    return true;
}
