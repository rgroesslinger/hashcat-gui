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
    bool loadStateFromFile(const QString &key, QWidget *widget, const QString &filename,
                           const QStringList &ignoredWidgets = {}, QString *errorMessage = nullptr,
                           QJsonObject *profileState = nullptr) const;
};

#endif // WIDGETSTATESERIALIZER_H
