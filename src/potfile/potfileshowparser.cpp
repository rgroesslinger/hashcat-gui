/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfileshowparser.h"

#include "potfileencoding.h"

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QStringTokenizer>

#include <algorithm>

namespace
{

// The JSON field is the hex of the literal $HEX[…] the potfile stores, so the
// outer layer is hex and the inner layer is the potfile escaping.
QString stringField(const QJsonObject &object, const QString &key)
{
    const QJsonValue value = object.value(key);
    if (!value.isString()) {
        return {};
    }

    QByteArray bytes;
    if (!PotfileEncoding::decodeHex(value.toString(), &bytes)) {
        return {};
    }
    return QString::fromUtf8(bytes);
}

} // namespace

PotfileShowParser::Result PotfileShowParser::parse(const QString &output, int maxEntries)
{
    Result result;
    const int cap = std::max(0, maxEntries);

    const QStringTokenizer lines(output, QLatin1Char('\n'), Qt::SkipEmptyParts);
    for (const QStringView line : lines) {
        const QString text = line.trimmed().toString();
        if (text.isEmpty()) {
            continue;
        }

        QJsonParseError parseError;
        const QJsonDocument document = QJsonDocument::fromJson(text.toUtf8(), &parseError);
        if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
            ++result.malformedCount;
            continue;
        }

        const QJsonObject object = document.object();

        // Binary-hashfile modes report the file name; there is no per-hash
        // identity in hashcat's output for them.
        QJsonValue identityValue = object.value(QStringLiteral("hash_hex"));
        bool hashIsFile = false;
        if (!identityValue.isString()) {
            identityValue = object.value(QStringLiteral("filename_hex"));
            hashIsFile = true;
        }
        if (!identityValue.isString()) {
            ++result.malformedCount;
            continue;
        }

        QByteArray hashBytes;
        if (!PotfileEncoding::decodeHex(identityValue.toString(), &hashBytes)) {
            ++result.malformedCount;
            continue;
        }

        Entry entry;
        // The hash field carries the input text, so a salt with non-UTF-8
        // bytes is replaced with U+FFFD here; a lossless fallback would need
        // one escaping rule shared by display, copy, search and export.
        entry.hash = QString::fromUtf8(hashBytes);
        entry.plain =
            PotfileEncoding::decodePlain(stringField(object, QStringLiteral("password_hex")));
        entry.username = stringField(object, QStringLiteral("username_hex"));
        entry.emptyPlain = entry.plain.isEmpty();
        entry.encrypted = entry.plain.startsWith(QStringLiteral("$HCENC$"));
        entry.hashIsFile = hashIsFile;

        ++result.totalCount;
        if (result.entries.size() < cap) {
            result.entries.append(entry);
        }
    }

    return result;
}