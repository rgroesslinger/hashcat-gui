/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "widgetstateserializer.h"

#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>

// Report a failure through errorMessage when the caller asked for it
static void setError(QString *errorMessage, const QString &text)
{
    if (errorMessage) {
        *errorMessage = text;
    }
}

WidgetStateSerializer::WidgetStateSerializer(QObject *parent)
    : QObject(parent)
{
}

// Serialize QWidget values to QJSonObject
static QJsonObject widgetToJson(const QWidget *w, const QStringList &ignoredWidgets = {})
{
    QJsonObject obj;

    // simple widgets
    auto const lineEdits = w->findChildren<const QLineEdit *>();
    for (const QLineEdit *lineEdit : lineEdits) {
        obj[lineEdit->objectName()] = lineEdit->text();
    }

    auto const checkBoxes = w->findChildren<const QCheckBox *>();
    for (const QCheckBox *checkBox : checkBoxes) {
        obj[checkBox->objectName()] = checkBox->isChecked();
    }

    auto const comboBoxes = w->findChildren<const QComboBox *>();
    for (const QComboBox *comboBox : comboBoxes) {
        obj[comboBox->objectName()] = comboBox->currentIndex();
    }

    auto const radioButtons = w->findChildren<const QRadioButton *>();
    for (const QRadioButton *radioButton : radioButtons) {
        obj[radioButton->objectName()] = radioButton->isChecked();
    }

    auto const spinBoxes = w->findChildren<const QSpinBox *>();
    for (const QSpinBox *spinBox : spinBoxes) {
        obj[spinBox->objectName()] = spinBox->value();
    }

    auto const doubleSpinBoxes = w->findChildren<const QDoubleSpinBox *>();
    for (const QDoubleSpinBox *doubleSpinBox : doubleSpinBoxes) {
        obj[doubleSpinBox->objectName()] = doubleSpinBox->value();
    }

    // list widgets
    auto const listWidgets = w->findChildren<const QListWidget *>();
    for (const QListWidget *listWidget : listWidgets) {
        QJsonArray list;
        const auto items = listWidget->findItems(QString("*"), Qt::MatchWildcard);
        for (const QListWidgetItem *item : items) {
            QJsonObject li;
            li["text"]    = item->text();
            li["checked"] = (item->checkState() == Qt::Checked);
            list.append(li);
        }
        obj[listWidget->objectName()] = list;
    }

    // Delete any widget that the caller flagged to ignore
    for (const QString &name : ignoredWidgets) {
        obj.remove(name);
    }

    return obj;
}

// Serialize QJsonObject values to QWidget
static void jsonToWidget(const QJsonObject &obj, QWidget *w, const QStringList &ignoredWidgets = {})
{
    QJsonObject mutableObj = obj;

    // Delete any widget that the caller flagged to ignore
    for (const QString &name : ignoredWidgets) {
        mutableObj.remove(name);
    }

    // simple widgets
    auto const lineEdits = w->findChildren<QLineEdit *>();
    for (QLineEdit *lineEdit : lineEdits) {
        if (mutableObj.contains(lineEdit->objectName()))
            lineEdit->setText(mutableObj[lineEdit->objectName()].toString());
    }

    auto const checkBoxes = w->findChildren<QCheckBox *>();
    for (QCheckBox *checkBox : checkBoxes) {
        if (mutableObj.contains(checkBox->objectName()))
            checkBox->setChecked(mutableObj[checkBox->objectName()].toBool());
    }

    auto const comboBoxes = w->findChildren<QComboBox *>();
    for (QComboBox *comboBox : comboBoxes) {
        if (mutableObj.contains(comboBox->objectName()))
            comboBox->setCurrentIndex(mutableObj[comboBox->objectName()].toInt());
    }

    auto const radioButtons = w->findChildren<QRadioButton *>();
    for (QRadioButton *radioButton : radioButtons) {
        if (mutableObj.contains(radioButton->objectName()))
            radioButton->setChecked(mutableObj[radioButton->objectName()].toBool());
    }

    auto const spinBoxes = w->findChildren<QSpinBox *>();
    for (QSpinBox *spinBox : spinBoxes) {
        if (mutableObj.contains(spinBox->objectName()))
            spinBox->setValue(mutableObj[spinBox->objectName()].toInt());
    }

    auto const doubleSpinBoxes = w->findChildren<QDoubleSpinBox *>();
    for (QDoubleSpinBox *doubleSpinBox : doubleSpinBoxes) {
        if (mutableObj.contains(doubleSpinBox->objectName()))
            doubleSpinBox->setValue(mutableObj[doubleSpinBox->objectName()].toDouble());
    }

    // list widgets
    auto const listWidgets = w->findChildren<QListWidget *>();
    for (QListWidget *listWidget : listWidgets) {
        if (mutableObj.contains(listWidget->objectName())) {
            listWidget->clear();
            const QJsonArray list = mutableObj[listWidget->objectName()].toArray();
            for (const QJsonValue &v : list) {
                QJsonObject li = v.toObject();
                QListWidgetItem *item = new QListWidgetItem(li["text"].toString(), listWidget);
                item->setCheckState(li["checked"].toBool() ? Qt::Checked : Qt::Unchecked);
            }
        }
    }
}

// Write QWidget state to a file
bool WidgetStateSerializer::saveStateToFile(const QString &key,
                                            const QWidget *widget,
                                            const QString &filename,
                                            const QStringList &ignoredWidgets,
                                            QString *errorMessage) const
{
    QJsonObject root;
    root[key] = widgetToJson(widget, ignoredWidgets);

    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        setError(errorMessage, tr("Could not open %1 for writing: %2").arg(filename, f.errorString()));
        return false;
    }

    const QByteArray data = QJsonDocument(root).toJson(QJsonDocument::Indented);
    if (f.write(data) != data.size() || !f.flush()) {
        setError(errorMessage, tr("Could not write to %1: %2").arg(filename, f.errorString()));
        return false;
    }

    f.close();
    setError(errorMessage, QString());
    return true;
}

// Read a file and restore a QWidget state
bool WidgetStateSerializer::loadStateFromFile(const QString &key,
                                              QWidget *widget,
                                              const QString &filename,
                                              const QStringList &ignoredWidgets,
                                              QString *errorMessage,
                                              QJsonObject *profileState) const
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        setError(errorMessage, tr("Could not open %1 for reading: %2").arg(filename, f.errorString()));
        return false;
    }

    QByteArray data = f.readAll();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        setError(errorMessage, tr("The file is not a valid JSON file: %1").arg(err.errorString()));
        return false;
    }

    QJsonObject root = doc.object();
    if (!root.contains(key)) {
        setError(errorMessage, tr("The file does not contain a profile for \"%1\".").arg(key));
        return false;
    }

    const QJsonObject state = root[key].toObject();
    if (profileState)
        *profileState = state;

    jsonToWidget(state, widget, ignoredWidgets);
    setError(errorMessage, QString());
    return true;
}
