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
#include <QMessageBox>

WidgetStateSerializer::WidgetStateSerializer(QObject *parent)
    : QObject(parent)
{
}

// Serialize QWidget values to QJSonObject
static QJsonObject widgetToJson(const QWidget *w, const QStringList &ignoredWidgets = {})
{
    QJsonObject obj;

    // simple widgets
    auto lineEdits = w->findChildren<const QLineEdit *>();
    for (const QLineEdit *lineEdit : std::as_const(lineEdits)) {
        obj[lineEdit->objectName()] = lineEdit->text();
    }

    auto checkBoxes = w->findChildren<const QCheckBox *>();
    for (const QCheckBox *checkBox : std::as_const(checkBoxes)) {
        obj[checkBox->objectName()] = checkBox->isChecked();
    }

    auto comboBoxes = w->findChildren<const QComboBox *>();
    for (const QComboBox *comboBox : std::as_const(comboBoxes)) {
        obj[comboBox->objectName()] = comboBox->currentIndex();
    }

    auto radioButtons = w->findChildren<const QRadioButton *>();
    for (const QRadioButton *radioButton : std::as_const(radioButtons)) {
        obj[radioButton->objectName()] = radioButton->isChecked();
    }

    auto spinBoxes = w->findChildren<const QSpinBox *>();
    for (const QSpinBox *spinBox : std::as_const(spinBoxes)) {
        obj[spinBox->objectName()] = spinBox->value();
    }

    auto doubleSpinBoxes = w->findChildren<const QDoubleSpinBox *>();
    for (const QDoubleSpinBox *doubleSpinBox : std::as_const(doubleSpinBoxes)) {
        obj[doubleSpinBox->objectName()] = doubleSpinBox->value();
    }

    // list widgets
    auto listWidgets = w->findChildren<const QListWidget *>();
    for (const QListWidget *listWidget : std::as_const(listWidgets)) {
        QJsonArray list;
        for (int i = 0; i < listWidget->count(); ++i) {
            const QListWidgetItem *item = listWidget->item(i);
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
    auto lineEdits = w->findChildren<QLineEdit *>();
    for (QLineEdit *lineEdit : std::as_const(lineEdits)) {
        if (mutableObj.contains(lineEdit->objectName()))
            lineEdit->setText(mutableObj[lineEdit->objectName()].toString());
    }

    auto checkBoxes = w->findChildren<QCheckBox *>();
    for (QCheckBox *checkBox : std::as_const(checkBoxes)) {
        if (mutableObj.contains(checkBox->objectName()))
            checkBox->setChecked(mutableObj[checkBox->objectName()].toBool());
    }

    auto comboBoxes = w->findChildren<QComboBox *>();
    for (QComboBox *comboBox : std::as_const(comboBoxes)) {
        if (mutableObj.contains(comboBox->objectName()))
            comboBox->setCurrentIndex(mutableObj[comboBox->objectName()].toInt());
    }

    auto radioButtons = w->findChildren<QRadioButton *>();
    for (QRadioButton *radioButton : std::as_const(radioButtons)) {
        if (mutableObj.contains(radioButton->objectName()))
            radioButton->setChecked(mutableObj[radioButton->objectName()].toBool());
    }

    auto spinBoxes = w->findChildren<QSpinBox *>();
    for (QSpinBox *spinBox : std::as_const(spinBoxes)) {
        if (mutableObj.contains(spinBox->objectName()))
            spinBox->setValue(mutableObj[spinBox->objectName()].toInt());
    }

    auto doubleSpinBoxes = w->findChildren<QDoubleSpinBox *>();
    for (QDoubleSpinBox *doubleSpinBox : std::as_const(doubleSpinBoxes)) {
        if (mutableObj.contains(doubleSpinBox->objectName()))
            doubleSpinBox->setValue(mutableObj[doubleSpinBox->objectName()].toDouble());
    }

    // list widgets
    auto listWidgets = w->findChildren<QListWidget *>();
    for (QListWidget *listWidget : std::as_const(listWidgets)) {
        if (mutableObj.contains(listWidget->objectName())) {
            listWidget->clear();
            QJsonArray list = mutableObj[listWidget->objectName()].toArray();
            for (const QJsonValue &v : std::as_const(list)) {
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
                                            const QStringList &ignoredWidgets) const
{
    QJsonObject root;
    root[key] = widgetToJson(widget, ignoredWidgets);

    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::warning(nullptr, tr("Save failed"),
                             tr("Could not open %1 for writing.").arg(filename));
        return false;
    }
    f.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    return true;
}

// Read a file and restore a QWidget state
bool WidgetStateSerializer::loadStateFromFile(const QString &key,
                                              QWidget *widget,
                                              const QString &filename,
                                              const QStringList &ignoredWidgets) const
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(nullptr, tr("Load failed"),
                             tr("Could not open %1 for reading.").arg(filename));
        return false;
    }

    QByteArray data = f.readAll();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        QMessageBox::warning(nullptr, tr("Load failed"),
                             tr("The file is not a valid JSON file."));
        return false;
    }

    QJsonObject root = doc.object();
    if (!root.contains(key)) {
        QMessageBox::warning(nullptr, tr("Load failed"),
                             tr("The file does not contain a profile for \"%1\".").arg(key));
        return false;
    }

    jsonToWidget(root[key].toObject(), widget, ignoredWidgets);
    return true;
}
