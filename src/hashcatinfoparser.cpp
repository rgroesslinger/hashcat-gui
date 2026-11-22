/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "hashcatinfoparser.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QStringList>

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

bool HashcatInfoParser::parseIdentify(const QString &rawOutput, QList<IdentifiedHashMode> &matches,
                                      QString *errorMessage)
{
    setError(errorMessage, QString());

    static const QRegularExpression row(
        QStringLiteral(R"(^\s*(\d+)\s*\|\s*(.+?)\s*\|\s*(.+?)\s*$)"));
    const QString noMatch = QStringLiteral("No hash-mode matches the structure of the input hash.");
    const QStringList lines = rawOutput.split(QLatin1Char('\n'));

    bool sawHeader = false;
    QList<IdentifiedHashMode> parsed;
    for (const QString &line : lines) {
        const QString text = line.trimmed();

        if (!sawHeader) {
            // Exit 255 with this sentence is an answer, not a failed query.
            if (text == noMatch) {
                matches.clear();
                return true;
            }
            // Warnings can precede the header.
            if (text.contains(QStringLiteral("| Name"))
                && text.contains(QStringLiteral("| Category"))) {
                sawHeader = true;
            }
            continue;
        }

        if (text.isEmpty()) {
            if (!parsed.isEmpty()) {
                break;
            }
            continue;
        }

        const QRegularExpressionMatch match = row.match(text);
        if (!match.hasMatch()) {
            // The '====+====' separator lands here.
            if (!parsed.isEmpty()) {
                break;
            }
            continue;
        }

        bool isNumber = false;
        const quint32 id = match.captured(1).toUInt(&isNumber);
        if (!isNumber) {
            continue;
        }
        parsed.append({id, match.captured(2), match.captured(3)});
    }

    if (!sawHeader || parsed.isEmpty()) {
        setError(errorMessage, tr("hashcat did not report any matching hash modes."));
        return false;
    }

    matches = std::move(parsed);
    return true;
}
