/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef WIDGETSTATESERIALIZER_H
#define WIDGETSTATESERIALIZER_H

#include <QObject>
#include <QJsonObject>

class WidgetStateSerializer : public QObject
{
    Q_OBJECT
public:
    explicit WidgetStateSerializer(QObject *parent = nullptr);

    bool saveStateToFile(const QString &key, const QWidget *widget, const QString &filename) const;

    bool loadStateFromFile(const QString &key, QWidget *widget, const QString &filename) const;
};

#endif // WIDGETSTATESERIALIZER_H
