/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "widgetstateserializer.h"
#include "appconstants.h"

#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QStandardPaths>

// Whether the entries of a combo box carry identifiers (item data) at all.
// It decides whether a selection is stored by identifier or by position: the
// two never mix within one combo box, so the stored value is unambiguous.
static bool comboBoxHasItemData(const QComboBox *comboBox)
{
    for (int i = 0; i < comboBox->count(); ++i) {
        if (comboBox->itemData(i).isValid()) {
            return true;
        }
    }
    return false;
}

WidgetStateSerializer::WidgetStateSerializer(QObject *parent) : QObject(parent)
{
}

QString WidgetStateSerializer::defaultProfilePath()
{
    const QString dirPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return QDir(dirPath).filePath(AppConstants::Files::DefaultProfile);
}

QJsonObject WidgetStateSerializer::toJson(const QWidget *w, const QStringList &ignoredWidgets)
{
    QJsonObject obj;

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
        // Stored by the identifier of the selected entry - the attack mode or
        // hash type id - not by its position, which a list that grows in the
        // middle shifts. A combo box without identifiers falls back to that.
        obj[comboBox->objectName()] = comboBoxHasItemData(comboBox)
                                          ? QJsonValue::fromVariant(comboBox->currentData())
                                          : QJsonValue(comboBox->currentIndex());
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

    auto const listWidgets = w->findChildren<const QListWidget *>();
    for (const QListWidget *listWidget : listWidgets) {
        QJsonArray list;
        const auto items = listWidget->findItems(QStringLiteral("*"), Qt::MatchWildcard);
        for (const QListWidgetItem *item : items) {
            QJsonObject li;
            li[QStringLiteral("text")] = item->text();
            li[QStringLiteral("checked")] = (item->checkState() == Qt::Checked);
            list.append(li);
        }
        obj[listWidget->objectName()] = list;
    }

    for (const QString &name : ignoredWidgets) {
        obj.remove(name);
    }

    return obj;
}

WidgetStateSerializer::ApplyResult
WidgetStateSerializer::applyToWidgets(const QJsonObject &obj, QWidget *w,
                                      const QStringList &ignoredWidgets)
{
    // The names are removed from a copy, so that the caller can still read the
    // values afterwards - the hash type, for one.
    QJsonObject mutableObj = obj;

    for (const QString &name : ignoredWidgets) {
        mutableObj.remove(name);
    }

    ApplyResult result;

    // Every name that is applied is taken off the copy, so that what is left
    // over names the values no widget claimed.
    auto const lineEdits = w->findChildren<QLineEdit *>();
    for (QLineEdit *lineEdit : lineEdits) {
        const QString name = lineEdit->objectName();
        if (!mutableObj.contains(name)) {
            continue;
        }

        const QJsonValue value = mutableObj.value(name);
        mutableObj.remove(name);

        if (!value.isString()) {
            result.wrongTypes << name;
            continue;
        }
        lineEdit->setText(value.toString());
    }

    auto const checkBoxes = w->findChildren<QCheckBox *>();
    for (QCheckBox *checkBox : checkBoxes) {
        const QString name = checkBox->objectName();
        if (!mutableObj.contains(name)) {
            continue;
        }

        const QJsonValue value = mutableObj.value(name);
        mutableObj.remove(name);

        if (!value.isBool()) {
            result.wrongTypes << name;
            continue;
        }
        checkBox->setChecked(value.toBool());
    }

    auto const comboBoxes = w->findChildren<QComboBox *>();
    for (QComboBox *comboBox : comboBoxes) {
        const QString name = comboBox->objectName();
        if (!mutableObj.contains(name)) {
            continue;
        }

        const QJsonValue stored = mutableObj.value(name);
        // The widget claimed the name, even when the value is empty.
        mutableObj.remove(name);

        if (stored.isUndefined() || stored.isNull()) {
            continue;
        }

        // A combo box holds the identifier of its entry or its position; both
        // are numbers, but an entry may carry a text as its identifier.
        if (!stored.isDouble() && !stored.isString()) {
            result.wrongTypes << name;
            continue;
        }

        if (comboBoxHasItemData(comboBox)) {
            // The identifier is the item data and nothing else; an unknown one
            // leaves the combo box alone instead of selecting another entry.
            const int index = comboBox->findData(stored.toVariant());
            if (index >= 0) {
                comboBox->setCurrentIndex(index);
            }
        } else {
            // No entry has an identifier, so the position is all that is left.
            const int index = stored.toInt(-1);
            if (index >= 0 && index < comboBox->count()) {
                comboBox->setCurrentIndex(index);
            }
        }
    }

    auto const radioButtons = w->findChildren<QRadioButton *>();
    for (QRadioButton *radioButton : radioButtons) {
        const QString name = radioButton->objectName();
        if (!mutableObj.contains(name)) {
            continue;
        }

        const QJsonValue value = mutableObj.value(name);
        mutableObj.remove(name);

        if (!value.isBool()) {
            result.wrongTypes << name;
            continue;
        }
        radioButton->setChecked(value.toBool());
    }

    auto const spinBoxes = w->findChildren<QSpinBox *>();
    for (QSpinBox *spinBox : spinBoxes) {
        const QString name = spinBox->objectName();
        if (!mutableObj.contains(name)) {
            continue;
        }

        const QJsonValue value = mutableObj.value(name);
        mutableObj.remove(name);

        if (!value.isDouble()) {
            result.wrongTypes << name;
            continue;
        }
        spinBox->setValue(value.toInt());
    }

    auto const doubleSpinBoxes = w->findChildren<QDoubleSpinBox *>();
    for (QDoubleSpinBox *doubleSpinBox : doubleSpinBoxes) {
        const QString name = doubleSpinBox->objectName();
        if (!mutableObj.contains(name)) {
            continue;
        }

        const QJsonValue value = mutableObj.value(name);
        mutableObj.remove(name);

        if (!value.isDouble()) {
            result.wrongTypes << name;
            continue;
        }
        doubleSpinBox->setValue(value.toDouble());
    }

    auto const listWidgets = w->findChildren<QListWidget *>();
    for (QListWidget *listWidget : listWidgets) {
        const QString name = listWidget->objectName();
        if (!mutableObj.contains(name)) {
            continue;
        }

        const QJsonValue value = mutableObj.value(name);
        mutableObj.remove(name);

        if (!value.isArray()) {
            result.wrongTypes << name;
            continue;
        }

        listWidget->clear();
        const QJsonArray list = value.toArray();
        for (const auto &v : list) {
            QJsonObject li = v.toObject();
            QListWidgetItem *item =
                new QListWidgetItem(li[QStringLiteral("text")].toString(), listWidget);
            item->setCheckState(li[QStringLiteral("checked")].toBool() ? Qt::Checked
                                                                       : Qt::Unchecked);
        }
    }

    result.unknownKeys = mutableObj.keys();
    return result;
}
