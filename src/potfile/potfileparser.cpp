/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfileparser.h"

#include "potfileencoding.h"

#include <QByteArray>
#include <QFile>
#include <QFileDevice>
#include <QSet>
#include <QStringList>

#include <algorithm>

namespace
{

quint64 fnvFingerprint(const QByteArray &line)
{
    quint64 hash = 14695981039346656037ULL;
    for (const char character : line) {
        hash ^= static_cast<unsigned char>(character);
        hash *= 1099511628211ULL;
    }
    return hash;
}

// Structurally different from FNV-1a: a rotation and a Murmur-style finalizer
// break the correlation a second FNV pass (or a reversed-byte pass) would
// have. Both fingerprints stay fixed-width, so duplicate detection is bounded
// by the line count, not by the file's byte size.
quint64 murmurFingerprint(const QByteArray &line)
{
    quint64 hash = 0x9e3779b97f4a7c15ULL ^ static_cast<quint64>(line.size());
    for (const char character : line) {
        hash ^= static_cast<quint64>(static_cast<unsigned char>(character));
        hash *= 0xff51afd7ed558ccdULL;
        hash = (hash << 31) | (hash >> 33);
    }
    hash ^= hash >> 33;
    hash *= 0xff51afd7ed558ccdULL;
    hash ^= hash >> 33;
    hash *= 0xc4ceb9fe1a85ec53ULL;
    hash ^= hash >> 33;
    return hash;
}

} // namespace

PotfileParser::LineFingerprint PotfileParser::LineFingerprint::of(const QByteArray &line)
{
    return {fnvFingerprint(line), murmurFingerprint(line)};
}

PotfileParser::Result PotfileParser::parseFile(const QString &path, int maxEntries, QChar separator,
                                               const HashcatRunner::CancelToken &cancel)
{
    return parseFile(path, maxEntries, separator, cancel, LineFingerprint::of);
}

PotfileParser::Result PotfileParser::parseFile(const QString &path, int maxEntries, QChar separator,
                                               const HashcatRunner::CancelToken &cancel,
                                               FingerprintFunction fingerprint)
{
    Result result;
    if (cancel && cancel->load()) {
        result.cancelled = true;
        return result;
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        result.error = tr("Could not open the potfile: %1").arg(file.errorString());
        return result;
    }

    const int cap = std::max(0, maxEntries);
    QSet<LineFingerprint> seenFingerprints;
    int lineNumber = 0;

    while (!file.atEnd()) {
        if (cancel && cancel->load()) {
            result.cancelled = true;
            return result;
        }

        QByteArray line = file.readLine();
        ++lineNumber;

        const bool terminated = line.endsWith('\n');
        if (!terminated && file.atEnd()) {
            // A potfile write ends each record with '\n'; an unterminated
            // final line is a partial write, not an entry.
            ++result.truncatedCount;
            break;
        }
        if (terminated) {
            line.chop(1);
        }
        if (line.endsWith('\r')) {
            line.chop(1);
        }

        // The line is decoded as UTF-8 before splitting. The hash field carries the
        // input text, so a salt with non-UTF-8 bytes is replaced with U+FFFD
        // here; a lossless fallback would need one escaping rule shared by
        // display, copy, search and export. Plaintext $HEX[…] tokens are
        // handled separately by PotfileEncoding::decodePlain().
        const QString text = QString::fromUtf8(line);
        if (text.trimmed().isEmpty()) {
            continue;
        }

        const qsizetype separatorIndex = text.lastIndexOf(separator);
        if (separatorIndex <= 0) {
            ++result.malformedCount;
            continue;
        }

        Entry entry;
        entry.hash = text.left(separatorIndex);
        entry.plain = PotfileEncoding::decodePlain(text.mid(separatorIndex + 1));
        entry.lineNumber = lineNumber;
        entry.emptyPlain = entry.plain.isEmpty();
        entry.encrypted = entry.plain.startsWith(QStringLiteral("$HCENC$"));
        const LineFingerprint lineFingerprint = fingerprint(line);
        entry.duplicate = seenFingerprints.contains(lineFingerprint);
        seenFingerprints.insert(lineFingerprint);

        ++result.totalCount;
        if (entry.encrypted) {
            ++result.encryptedCount;
        }
        if (entry.emptyPlain) {
            ++result.noPasswordCount;
        }
        if (entry.duplicate) {
            ++result.duplicateCount;
        }
        if (result.entries.size() < cap) {
            result.entries.append(entry);
        }
    }

    if (file.error() != QFileDevice::NoError) {
        result.error = tr("Could not read the potfile: %1").arg(file.errorString());
    }
    return result;
}