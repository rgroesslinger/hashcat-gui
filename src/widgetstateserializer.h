/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef WIDGETSTATESERIALIZER_H
#define WIDGETSTATESERIALIZER_H

#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QStringList>

class QWidget;

class WidgetStateSerializer : public QObject
{
    Q_OBJECT
public:
    explicit WidgetStateSerializer(QObject *parent = nullptr);

    // The file the application keeps its state in between two runs.
    //
    // The location comes from QStandardPaths::AppDataLocation, which the
    // application name alone decides - the organization name is deliberately
    // left unset, see main.cpp.
    static QString defaultProfilePath();

    // The widget state as a JSON object; the object names are the keys.
    //
    // The ignored widgets are left out: they hold something transient, like
    // the command preview, which is rebuilt from the other values.
    static QJsonObject toJson(const QWidget *widget, const QStringList &ignoredWidgets = {});

    // The values of a file that could not be applied to the widgets.
    struct ApplyResult {
        // The names no widget claimed: the file was made by another version,
        // or it was edited by hand.
        QStringList unknownKeys;

        // The names whose value is of a type the widget does not take. The
        // widget keeps the value it had: applying what the file holds would
        // change something the user did not ask for.
        QStringList wrongTypes;
    };

    // Applies a JSON object to the widgets and reports the values that could
    // not be applied.
    //
    // The input object is not modified: the names are removed from a copy to
    // compute the remainder, so that the caller can still read the values
    // afterwards - the hash type, for one.
    static ApplyResult applyToWidgets(const QJsonObject &values, QWidget *widget,
                                      const QStringList &ignoredWidgets = {});
};

#endif // WIDGETSTATESERIALIZER_H
