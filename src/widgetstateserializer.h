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

    // Serialize the state of widget into a profile file.
    //
    // Errors are reported through the return value and, when errorMessage is
    // not nullptr, the reason is written there. The serializer deliberately
    // does not open a dialog: callers decide how to present a failure, and
    // the class stays usable from unit tests.
    bool saveStateToFile(const QString &key, const QWidget *widget, const QString &filename,
                         const QStringList &ignoredWidgets = {},
                         QString *errorMessage = nullptr) const;

    // Restore the state of widget from a profile file.
    //
    // When profileState is not nullptr the profile object that was applied
    // is copied there, so callers can read values the widgets could not take
    // yet - a combobox that has no items until an asynchronous query fills
    // it, see MainWindow::loadDefaultProfile().
    //
    // A profile from another version of the format is read as well: the
    // widgets it names that still exist keep their values, the ones that do
    // not exist any more are left at their defaults. warning receives a note
    // about it, so the caller can tell the user that the profile is not
    // complete.
    bool loadStateFromFile(const QString &key, QWidget *widget, const QString &filename,
                           const QStringList &ignoredWidgets = {}, QString *errorMessage = nullptr,
                           QJsonObject *profileState = nullptr, QString *warning = nullptr) const;
};

#endif // WIDGETSTATESERIALIZER_H
