/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "statefile.h"
#include "appconstants.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QSaveFile>
#include <QStandardPaths>

namespace
{
// Reports a failure through errorMessage when the caller asked for it
void setError(QString *errorMessage, const QString &text)
{
    if (errorMessage) {
        *errorMessage = text;
    }
}
} // namespace

QString StateFile::sessionFilePath()
{
    const QString dirPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return QDir(dirPath).filePath(AppConstants::Files::SessionFile);
}

bool StateFile::write(const QString &filename, QLatin1StringView section, int version,
                      const QJsonObject &values, QString *errorMessage)
{
    QJsonObject sectionObject;
    sectionObject.insert(QStringLiteral("version"), version);
    sectionObject.insert(QStringLiteral("values"), values);

    QJsonObject sections;
    sections.insert(QString(section), sectionObject);

    QJsonObject root;
    root.insert(QStringLiteral("sections"), sections);

    // QSaveFile writes to a temporary file next to the target and renames it
    // over the target only when the write succeeded. A crash in the middle
    // therefore leaves the old file alone instead of a truncated one.
    QSaveFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        setError(errorMessage,
                 tr("Could not open %1 for writing: %2").arg(filename, file.errorString()));
        return false;
    }

    const QByteArray data = QJsonDocument(root).toJson(QJsonDocument::Indented);
    if (file.write(data) != data.size() || !file.commit()) {
        setError(errorMessage, tr("Could not write to %1: %2").arg(filename, file.errorString()));
        return false;
    }

    setError(errorMessage, QString());
    return true;
}

bool StateFile::read(const QString &filename, QLatin1StringView section, QJsonObject *values,
                     int *version, QStringList *otherSections, QString *errorMessage)
{
    if (values) {
        *values = QJsonObject();
    }
    if (version) {
        *version = -1;
    }
    if (otherSections) {
        otherSections->clear();
    }

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        setError(errorMessage,
                 tr("Could not open %1 for reading: %2").arg(filename, file.errorString()));
        return false;
    }

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        setError(errorMessage, tr("The file is not valid JSON: %1").arg(parseError.errorString()));
        return false;
    }

    // Valid JSON that is not an object - an array, a number, a string - has no
    // parse error to report, so it gets its own message.
    if (!document.isObject()) {
        setError(errorMessage, tr("The file does not contain a JSON object."));
        return false;
    }

    const QJsonObject root = document.object();
    if (!root.value(QStringLiteral("sections")).isObject()) {
        setError(errorMessage, tr("The file does not contain any sections."));
        return false;
    }

    const QJsonObject sections = root.value(QStringLiteral("sections")).toObject();

    // The sections the file holds besides the one that was asked for. They are
    // collected before the section is looked up, so that the message can name
    // them when the file holds something else.
    QStringList others;
    for (auto it = sections.constBegin(); it != sections.constEnd(); ++it) {
        if (it.key() != QString(section)) {
            others << it.key();
        }
    }
    if (otherSections) {
        *otherSections = others;
    }

    if (!sections.contains(QString(section))) {
        const QString message =
            others.isEmpty() ? tr("The file does not contain a %1 section.").arg(QString(section))
                             : tr("The file does not contain a %1 section; it holds %2.")
                                   .arg(QString(section), nameList(others));
        setError(errorMessage, message);
        return false;
    }

    const QJsonObject sectionObject = sections.value(QString(section)).toObject();
    if (!sectionObject.value(QStringLiteral("values")).isObject()) {
        setError(errorMessage,
                 tr("The %1 section does not contain any values.").arg(QString(section)));
        return false;
    }

    if (values) {
        *values = sectionObject.value(QStringLiteral("values")).toObject();
    }
    if (version) {
        *version = sectionObject.value(QStringLiteral("version")).toInt(-1);
    }

    setError(errorMessage, QString());
    return true;
}

QString StateFile::nameList(const QStringList &names, int limit)
{
    if (names.isEmpty()) {
        return {};
    }

    if (names.size() == 1) {
        return names.first();
    }

    if (names.size() <= limit) {
        return names.mid(0, names.size() - 1).join(QStringLiteral(", ")) + tr(" and ")
               + names.last();
    }

    // More names than fit in a line: the first ones and the number of the
    // rest.
    return names.mid(0, limit).join(QStringLiteral(", "))
           + tr(" and %1 more").arg(names.size() - limit);
}

QString StateFile::unknownValuesNote(const QStringList &names)
{
    if (names.isEmpty()) {
        return {};
    }

    return tr("The file names values this version does not know, so they were not applied: %1.")
        .arg(nameList(names));
}

QString StateFile::wrongTypeValuesNote(const QStringList &names)
{
    if (names.isEmpty()) {
        return {};
    }

    return tr("The file holds values of the wrong type, so they were not applied: %1.")
        .arg(nameList(names));
}
