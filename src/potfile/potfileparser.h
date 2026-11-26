/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef POTFILEPARSER_H
#define POTFILEPARSER_H

#include "appconstants.h"
#include "hashcatrunner.h"

#include <QByteArray>
#include <QChar>
#include <QCoreApplication>
#include <QHashFunctions>
#include <QList>
#include <QString>

#include <cstddef>

// Parser for an explicit hashcat potfile. It reads one line at a time, so a
// large file is never copied into one string; entries are capped for display.
class PotfileParser
{
    Q_DECLARE_TR_FUNCTIONS(PotfileParser)

public:
    // Two structurally different 64-bit fingerprints over one raw line. A
    // duplicate needs both to match, which keeps the collision probability
    // negligible while the memory per seen line stays fixed and small.
    struct LineFingerprint {
        quint64 first = 0;
        quint64 second = 0;

        static LineFingerprint of(const QByteArray &line);

        friend bool operator==(const LineFingerprint &lhs, const LineFingerprint &rhs) = default;
        friend std::size_t qHash(const LineFingerprint &fingerprint, std::size_t seed = 0) noexcept
        {
            return qHashMulti(seed, fingerprint.first, fingerprint.second);
        }
    };

    struct Entry {
        QString hash;
        QString plain;
        int lineNumber = 0;
        bool emptyPlain = false;
        bool encrypted = false;
        bool duplicate = false;
    };

    using Entries = QList<Entry>;

    struct Result {
        Entries entries;
        int totalCount = 0;
        int malformedCount = 0;
        int encryptedCount = 0;
        int noPasswordCount = 0;
        int duplicateCount = 0;
        // False once the fingerprint cap stopped tracking new lines: the
        // count then is a lower bound, not the exact number.
        bool duplicateCountComplete = true;
        int truncatedCount = 0;
        bool cancelled = false;
        QString error;
    };

    static Result parseFile(const QString &path,
                            int maxEntries = AppConstants::Hashcat::PotfileMaxEntries,
                            QChar separator = QLatin1Char(':'),
                            const HashcatRunner::CancelToken &cancel = {});

private:
    using FingerprintFunction = LineFingerprint (*)(const QByteArray &line);

    // The fingerprint provider is a test seam; production always uses
    // LineFingerprint::of(). Fingerprints are bounded independently of the
    // display cap: the counters stay exact, duplicate detection stops there.
    static Result parseFile(const QString &path, int maxEntries, int maxFingerprints,
                            QChar separator, const HashcatRunner::CancelToken &cancel,
                            FingerprintFunction fingerprint);

    friend class TestPotfileParser;
};

#endif // POTFILEPARSER_H