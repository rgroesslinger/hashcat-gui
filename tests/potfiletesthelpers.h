/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef POTFILETESTHELPERS_H
#define POTFILETESTHELPERS_H

#include "potfilecheck.h"
#include "potfileexporter.h"

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedLayout>
#include <QString>
#include <QTreeWidget>

#include <utility>

inline QString hexBytes(const QByteArray &bytes)
{
    return QString::fromLatin1(bytes.toHex());
}

// One line of `hashcat --show --json` output.
inline QString showLine(const QByteArray &hash = QByteArrayLiteral("hash"),
                        const QByteArray &plain = QByteArrayLiteral("plain"),
                        const QString &username = QString())
{
    QJsonObject object;
    object.insert(QStringLiteral("hash_hex"), hexBytes(hash));
    object.insert(QStringLiteral("password_hex"), hexBytes(plain));
    if (!username.isEmpty()) {
        object.insert(QStringLiteral("username_hex"), hexBytes(username.toUtf8()));
    }
    return QString::fromUtf8(QJsonDocument(object).toJson(QJsonDocument::Compact));
}

// One line of `hashcat --show --json` for a binary hash file.
inline QString
binaryHashFileShowLine(const QByteArray &filename = QByteArrayLiteral("hashes.hc22000"),
                       const QByteArray &plain = QByteArrayLiteral("plain"))
{
    QJsonObject object;
    object.insert(QStringLiteral("filename_hex"), hexBytes(filename));
    object.insert(QStringLiteral("password_hex"), hexBytes(plain));
    return QString::fromUtf8(QJsonDocument(object).toJson(QJsonDocument::Compact));
}

// A successful check result whose cracked and total counts follow the entries.
inline PotfileCheckResult okCheckResult(PotfileShowParser::Entries entries, int left = 0)
{
    PotfileCheckResult result;
    result.status = PotfileCheckResult::Status::Ok;
    result.cracked = static_cast<int>(entries.size());
    result.totalCount = static_cast<int>(entries.size());
    result.left = left;
    result.entries = std::move(entries);
    return result;
}

inline QList<PotfileExporter::Row> mixedRows()
{
    return {
        {QStringLiteral("hash-one"), QStringLiteral("abc"), {}, 0, false, false},
        {QStringLiteral("hash-two"), QStringLiteral("ABC123!"), {}, 0, false, false},
        {QStringLiteral("hash-three"), QString(), {}, 0, false, false},
        {QStringLiteral("hash-four"), QStringLiteral("$HCENC$1$key$cipher"), {}, 0, false, true},
        {QStringLiteral("hash-five"), QStringLiteral("abc"), {}, 0, true, false},
        {QStringLiteral("hash-six"), QStringLiteral("é space"), {}, 0, false, false},
    };
}

inline QList<PotfileExporter::Row> noPlaintextRows()
{
    return {
        {QStringLiteral("hash-one"), QString(), {}, 0, false, false},
        {QStringLiteral("hash-two"), QStringLiteral("$HCENC$1$key$cipher"), {}, 0, false, true},
    };
}

namespace PotfileDialogTestAccess
{
inline QTreeWidget *tree(QWidget &dialog)
{
    return dialog.findChild<QTreeWidget *>(QStringLiteral("treeWidget_results"));
}

inline QLineEdit *search(QWidget &dialog)
{
    return dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_search"));
}

inline QPushButton *button(QWidget &dialog, const QString &name)
{
    return dialog.findChild<QPushButton *>(name);
}

inline QLabel *label(QWidget &dialog, const QString &name)
{
    return dialog.findChild<QLabel *>(name);
}

inline QStackedLayout *stack(QWidget &dialog)
{
    return dialog.findChild<QStackedLayout *>(QStringLiteral("table_stack"));
}
} // namespace PotfileDialogTestAccess

#endif // POTFILETESTHELPERS_H