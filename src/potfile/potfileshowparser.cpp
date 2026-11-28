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

PotfileShowParser::Result PotfileShowParser::parse(const QString &output, int maxEntries,
                                                   const HashcatRunner::CancelToken &cancel)
{
    Result result;
    const int cap = std::max(0, maxEntries);

    const QStringView payload = QStringView(output).mid(payloadOffset(output));
    const QStringTokenizer lines(payload, QLatin1Char('\n'), Qt::SkipEmptyParts);
    for (const QStringView line : lines) {
        if (cancel && cancel->load()) {
            result.cancelled = true;
            return result;
        }
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

int PotfileShowParser::payloadOffset(const QString &output)
{
    const qsizetype size = output.size();
    const auto lineEnd = [&output, size](qsizetype start) {
        const qsizetype newline = output.indexOf(QLatin1Char('\n'), start);
        return newline < 0 ? size : newline;
    };
    // A final line without a trailing newline must not push past the end.
    const auto nextLine = [&lineEnd, size](qsizetype start) {
        return std::min(lineEnd(start) + 1, size);
    };
    const auto lineAt = [&output, &lineEnd](qsizetype start) {
        return QStringView(output).mid(start, lineEnd(start) - start).trimmed();
    };
    const auto skipThrough = [&](qsizetype start, QLatin1StringView end) {
        qsizetype pos = start;
        while (pos < size) {
            if (lineAt(pos).startsWith(end)) {
                return nextLine(pos);
            }
            pos = nextLine(pos);
        }
        return size;
    };

    qsizetype pos = 0;
    while (pos < size) {
        const QStringView text = lineAt(pos);
        if (text.isEmpty()) {
            pos = nextLine(pos);
            continue;
        }
        // Auto-detect preamble; defensive, the GUI always passes -m.
        if (text.startsWith(QLatin1StringView("Hash-mode was not specified with -m"))) {
            pos = skipThrough(pos, QLatin1StringView("Do NOT report auto-detect issues"));
            continue;
        }
        // Legacy ~/.hashcat block; it contains bare path lines, so skip whole.
        if (text.startsWith(QLatin1StringView("Found "))
            && text.contains(QLatin1StringView(", which hashcat no longer uses."))) {
            pos = skipThrough(pos, QLatin1StringView("Move what you still want across"));
            continue;
        }
        if (text.startsWith(QLatin1StringView("Old potfile detected:"))) {
            pos = skipThrough(pos, QLatin1StringView("New potfile is:"));
            continue;
        }
        // Per-line parse warnings on mixed-mode input; they can repeat.
        if (text.startsWith(QLatin1StringView("Hashfile '"))) {
            pos = nextLine(pos);
            continue;
        }
        break;
    }
    return static_cast<int>(pos);
}