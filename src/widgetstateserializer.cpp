/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "widgetstateserializer.h"

#include "statewidgets.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QRadioButton>
#include <QSpinBox>
#include <algorithm>

namespace
{
// Whether the entries of a combo box carry identifiers (item data) at all.
// It decides whether a selection is stored by identifier or by position: the
// two never mix within one combo box, so the stored value is unambiguous.
bool comboBoxHasItemData(const QComboBox *comboBox)
{
    for (int i = 0; i < comboBox->count(); ++i) {
        if (comboBox->itemData(i).isValid()) {
            return true;
        }
    }
    return false;
}

} // namespace

WidgetStateSerializer::WidgetStateSerializer(QObject *parent) : QObject(parent) {}

QJsonObject WidgetStateSerializer::toJson(const QWidget *widget, const QStringList &ignoredWidgets)
{
    QJsonObject obj;

    auto const lineEdits = widget->findChildren<const QLineEdit *>();
    for (const QLineEdit *lineEdit : lineEdits) {
        if (!isApplicationWidget(lineEdit)) {
            continue;
        }
        obj.insert(lineEdit->objectName(), lineEdit->text());
    }

    auto const checkBoxes = widget->findChildren<const QCheckBox *>();
    for (const QCheckBox *checkBox : checkBoxes) {
        if (!isApplicationWidget(checkBox)) {
            continue;
        }
        obj.insert(checkBox->objectName(), checkBox->isChecked());
    }

    auto const comboBoxes = widget->findChildren<const QComboBox *>();
    for (const QComboBox *comboBox : comboBoxes) {
        if (!isApplicationWidget(comboBox)) {
            continue;
        }
        // Stored by the identifier of the selected entry - the attack mode or
        // hash type id - not by its position, which a list that grows in the
        // middle shifts. A combo box without identifiers falls back to that.
        obj.insert(comboBox->objectName(), comboBoxHasItemData(comboBox)
                                               ? QJsonValue::fromVariant(comboBox->currentData())
                                               : QJsonValue(comboBox->currentIndex()));
    }

    auto const radioButtons = widget->findChildren<const QRadioButton *>();
    for (const QRadioButton *radioButton : radioButtons) {
        if (!isApplicationWidget(radioButton)) {
            continue;
        }
        obj.insert(radioButton->objectName(), radioButton->isChecked());
    }

    auto const spinBoxes = widget->findChildren<const QSpinBox *>();
    for (const QSpinBox *spinBox : spinBoxes) {
        if (!isApplicationWidget(spinBox)) {
            continue;
        }
        obj.insert(spinBox->objectName(), spinBox->value());
    }

    auto const doubleSpinBoxes = widget->findChildren<const QDoubleSpinBox *>();
    for (const QDoubleSpinBox *doubleSpinBox : doubleSpinBoxes) {
        if (!isApplicationWidget(doubleSpinBox)) {
            continue;
        }
        obj.insert(doubleSpinBox->objectName(), doubleSpinBox->value());
    }

    auto const listWidgets = widget->findChildren<const QListWidget *>();
    for (const QListWidget *listWidget : listWidgets) {
        if (!isApplicationWidget(listWidget)) {
            continue;
        }
        QJsonArray list;
        const auto items = listWidget->findItems(QStringLiteral("*"), Qt::MatchWildcard);
        for (const QListWidgetItem *item : items) {
            QJsonObject li;
            li.insert(QStringLiteral("text"), item->text());
            li.insert(QStringLiteral("checked"), (item->checkState() == Qt::Checked));
            list.append(li);
        }
        obj.insert(listWidget->objectName(), list);
    }

    for (const QString &name : ignoredWidgets) {
        obj.remove(name);
    }

    return obj;
}

WidgetStateSerializer::ApplyResult
WidgetStateSerializer::applyToWidgets(const QJsonObject &values, QWidget *widget,
                                      const QStringList &ignoredWidgets)
{
    // The names are removed from a copy, so that the caller can still read the
    // values afterwards - the hash type, for one.
    QJsonObject mutableObj = values;

    for (const QString &name : ignoredWidgets) {
        mutableObj.remove(name);
    }

    ApplyResult result;

    // Every name that is applied is taken off the copy, so that what is left
    // over names the values no widget claimed.
    auto const lineEdits = widget->findChildren<QLineEdit *>();
    for (QLineEdit *lineEdit : lineEdits) {
        if (!isApplicationWidget(lineEdit)) {
            continue;
        }
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

    auto const checkBoxes = widget->findChildren<QCheckBox *>();
    for (QCheckBox *checkBox : checkBoxes) {
        if (!isApplicationWidget(checkBox)) {
            continue;
        }
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

    auto const comboBoxes = widget->findChildren<QComboBox *>();
    for (QComboBox *comboBox : comboBoxes) {
        if (!isApplicationWidget(comboBox)) {
            continue;
        }
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

    auto const radioButtons = widget->findChildren<QRadioButton *>();
    for (QRadioButton *radioButton : radioButtons) {
        if (!isApplicationWidget(radioButton)) {
            continue;
        }
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

    auto const spinBoxes = widget->findChildren<QSpinBox *>();
    for (QSpinBox *spinBox : spinBoxes) {
        if (!isApplicationWidget(spinBox)) {
            continue;
        }
        const QString name = spinBox->objectName();
        if (!mutableObj.contains(name)) {
            continue;
        }

        const QJsonValue value = mutableObj.value(name);
        mutableObj.remove(name);

        // A spin box takes whole numbers. QJsonValue::toInt() answers with
        // its default for a value with a fraction, so the widget would take
        // a number the file never held.
        if (!value.isDouble() || value.toDouble() != value.toInt()) {
            result.wrongTypes << name;
            continue;
        }
        spinBox->setValue(value.toInt());
    }

    auto const doubleSpinBoxes = widget->findChildren<QDoubleSpinBox *>();
    for (QDoubleSpinBox *doubleSpinBox : doubleSpinBoxes) {
        if (!isApplicationWidget(doubleSpinBox)) {
            continue;
        }
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

    auto const listWidgets = widget->findChildren<QListWidget *>();
    for (QListWidget *listWidget : listWidgets) {
        if (!isApplicationWidget(listWidget)) {
            continue;
        }
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

        // The entries have to be the objects the exporter wrote, with a string
        // text and a bool checked. An entry of another shape would become an
        // empty row - and the list of the user would be gone before the file
        // is even looked at, with no note about why.
        const QJsonArray list = value.toArray();
        const bool allEntriesAreValid =
            std::all_of(list.constBegin(), list.constEnd(), [](const QJsonValue &entry) {
                if (!entry.isObject()) {
                    return false;
                }
                const QJsonObject item = entry.toObject();
                return item.value(QStringLiteral("text")).isString()
                       && item.value(QStringLiteral("checked")).isBool();
            });
        if (!allEntriesAreValid) {
            result.wrongTypes << name;
            continue;
        }

        listWidget->clear();
        for (const auto &v : list) {
            QJsonObject li = v.toObject();
            QListWidgetItem *item =
                new QListWidgetItem(li.value(QStringLiteral("text")).toString(), listWidget);
            item->setCheckState(li.value(QStringLiteral("checked")).toBool() ? Qt::Checked
                                                                             : Qt::Unchecked);
        }
    }

    result.unknownKeys = mutableObj.keys();
    return result;
}
