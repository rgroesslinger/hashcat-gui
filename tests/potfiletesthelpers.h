/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef POTFILETESTHELPERS_H
#define POTFILETESTHELPERS_H

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

// One line of `hashcat --show --json` output.
inline QString showLine()
{
    QJsonObject object;
    object.insert(QStringLiteral("hash_hex"),
                  QString::fromLatin1(QByteArrayLiteral("hash").toHex()));
    object.insert(QStringLiteral("password_hex"),
                  QString::fromLatin1(QByteArrayLiteral("plain").toHex()));
    return QString::fromUtf8(QJsonDocument(object).toJson(QJsonDocument::Compact));
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