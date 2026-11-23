/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfileexporter.h"

#include <QFileDevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>

namespace
{

QString boolText(bool value)
{
    return value ? QStringLiteral("true") : QStringLiteral("false");
}

void appendField(QByteArray *line, const QString &field, QChar delimiter, bool *first)
{
    if (!*first) {
        *line += delimiter.toLatin1();
    }
    *first = false;

    QByteArray bytes = field.toUtf8();
    if (bytes.contains(delimiter.toLatin1()) || bytes.contains('"') || bytes.contains('\n')
        || bytes.contains('\r')) {
        bytes.replace("\"", "\"\"");
        *line += '"';
        *line += bytes;
        *line += '"';
    } else {
        *line += bytes;
    }
}

QByteArray tabular(const QList<PotfileExporter::Row> &rows, QChar delimiter, bool includeUsername,
                   bool includeLine)
{
    QByteArray output;
    bool first = true;
    appendField(&output, QStringLiteral("hash"), delimiter, &first);
    appendField(&output, QStringLiteral("plaintext"), delimiter, &first);
    if (includeUsername) {
        appendField(&output, QStringLiteral("username"), delimiter, &first);
    }
    if (includeLine) {
        appendField(&output, QStringLiteral("duplicate"), delimiter, &first);
        appendField(&output, QStringLiteral("line"), delimiter, &first);
    }
    appendField(&output, QStringLiteral("encrypted"), delimiter, &first);
    output += '\n';

    for (const PotfileExporter::Row &row : rows) {
        first = true;
        appendField(&output, row.hash, delimiter, &first);
        appendField(&output, row.plaintext, delimiter, &first);
        if (includeUsername) {
            appendField(&output, row.username, delimiter, &first);
        }
        if (includeLine) {
            appendField(&output, boolText(row.duplicate), delimiter, &first);
            appendField(&output, QString::number(row.lineNumber), delimiter, &first);
        }
        appendField(&output, boolText(row.encrypted), delimiter, &first);
        output += '\n';
    }
    return output;
}

QByteArray json(const QList<PotfileExporter::Row> &rows, bool includeUsername, bool includeLine)
{
    QJsonArray array;
    for (const PotfileExporter::Row &row : rows) {
        QJsonObject object;
        object.insert(QStringLiteral("hash"), row.hash);
        object.insert(QStringLiteral("plaintext"), row.plaintext);
        if (includeUsername) {
            object.insert(QStringLiteral("username"), row.username);
        }
        if (includeLine) {
            object.insert(QStringLiteral("duplicate"), row.duplicate);
            object.insert(QStringLiteral("line"), row.lineNumber);
        }
        object.insert(QStringLiteral("encrypted"), row.encrypted);
        array.append(object);
    }

    QByteArray output = QJsonDocument(array).toJson(QJsonDocument::Compact);
    output += '\n';
    return output;
}

} // namespace

QByteArray PotfileExporter::serialize(const QList<Row> &rows, Format format, bool includeUsername,
                                      bool includeLine)
{
    switch (format) {
    case Format::Csv:
        return tabular(rows, QLatin1Char(','), includeUsername, includeLine);
    case Format::Txt:
        return tabular(rows, QLatin1Char('\t'), includeUsername, includeLine);
    case Format::Json:
        return json(rows, includeUsername, includeLine);
    }
    return {};
}

bool PotfileExporter::write(const QString &path, const QList<Row> &rows, Format format,
                            bool includeUsername, bool includeLine, QString *error)
{
    if (error) {
        error->clear();
    }

    const QByteArray data = serialize(rows, format, includeUsername, includeLine);
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        if (error) {
            *error = tr("Could not open the export file: %1").arg(file.errorString());
        }
        return false;
    }
    // The export contains credentials; keep it owner-only where the platform
    // supports POSIX permissions. Failure here is best-effort, not an export
    // failure (Windows ACLs do not map one-to-one).
    (void)file.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    if (file.write(data) != data.size()) {
        if (error) {
            *error = tr("Could not write the export file: %1").arg(file.errorString());
        }
        file.cancelWriting();
        return false;
    }
    if (!file.commit()) {
        if (error) {
            *error = tr("Could not save the export file: %1").arg(file.errorString());
        }
        return false;
    }
    return true;
}