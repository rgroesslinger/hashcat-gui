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
static QJsonObject widgetToJson(const QWidget *w)
{
    QJsonObject obj;

    // simple widgets
    for (const QLineEdit *lineEdit : w->findChildren<const QLineEdit *>()) {
        // text for lineEdit_command is generated automatically
        if (lineEdit->objectName() == "lineEdit_command") {
            continue;
        }
        obj[lineEdit->objectName()] = lineEdit->text();
    }

    for (const QCheckBox *checkBox : w->findChildren<const QCheckBox *>()) {
        obj[checkBox->objectName()] = checkBox->isChecked();
    }

    for (const QComboBox *comboBox : w->findChildren<const QComboBox *>()) {
        obj[comboBox->objectName()] = comboBox->currentIndex();
    }

    for (const QRadioButton *radioButton : w->findChildren<const QRadioButton *>()) {
        obj[radioButton->objectName()] = radioButton->isChecked();
    }

    for (const QSpinBox *spinBox : w->findChildren<const QSpinBox *>()) {
        obj[spinBox->objectName()] = spinBox->value();
    }

    for (const QDoubleSpinBox *doubleSpinBox : w->findChildren<const QDoubleSpinBox *>()) {
        obj[doubleSpinBox->objectName()] = doubleSpinBox->value();
    }

    // list widgets
    for (const QListWidget *listWidget : w->findChildren<const QListWidget *>()) {
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

    return obj;
}

// Serialize QJsonObject values to QWidget
static void jsonToWidget(const QJsonObject &obj, QWidget *w)
{
    // simple widgets
    for (QLineEdit *lineEdit : w->findChildren<QLineEdit *>()) {
        if (obj.contains(lineEdit->objectName()))
            lineEdit->setText(obj[lineEdit->objectName()].toString());
    }

    for (QCheckBox *checkBox : w->findChildren<QCheckBox *>()) {
        if (obj.contains(checkBox->objectName()))
            checkBox->setChecked(obj[checkBox->objectName()].toBool());
    }

    for (QComboBox *comboBox : w->findChildren<QComboBox *>()) {
        if (obj.contains(comboBox->objectName()))
            comboBox->setCurrentIndex(obj[comboBox->objectName()].toInt());
    }

    for (QRadioButton *radioButton : w->findChildren<QRadioButton *>()) {
        if (obj.contains(radioButton->objectName()))
            radioButton->setChecked(obj[radioButton->objectName()].toBool());
    }

    for (QSpinBox *spinBox : w->findChildren<QSpinBox *>()) {
        if (obj.contains(spinBox->objectName()))
            spinBox->setValue(obj[spinBox->objectName()].toInt());
    }

    for (QDoubleSpinBox *doubleSpinBox : w->findChildren<QDoubleSpinBox *>()) {
        if (obj.contains(doubleSpinBox->objectName()))
            doubleSpinBox->setValue(obj[doubleSpinBox->objectName()].toDouble());
    }

    // list widgets
    for (QListWidget *listWidget : w->findChildren<QListWidget *>()) {
        if (obj.contains(listWidget->objectName())) {
            listWidget->clear();
            QJsonArray list = obj[listWidget->objectName()].toArray();
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
                                            const QString &filename) const
{
    QJsonObject root;
    root[key] = widgetToJson(widget);

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
                                              const QString &filename) const
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

    jsonToWidget(root[key].toObject(), widget);
    return true;
}
