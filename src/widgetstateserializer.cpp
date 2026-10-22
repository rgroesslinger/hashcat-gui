/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "widgetstateserializer.h"

#include <QAbstractSpinBox>
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

// Whether the widget is one the application put there.
//
// Two kinds of widgets are not: the editor a spin box or an editable combo
// box builds for itself is a child of that widget, and the name Qt gives it
// is the same for every spin box - a value stored under it would mean
// nothing, and a file that names it would change a spin box. A widget
// without a name at all would share the key "" with every other unnamed
// widget.
static bool isApplicationWidget(const QWidget *widget)
{
    if (widget->objectName().isEmpty()) {
        return false;
    }

    const QWidget *parent = widget->parentWidget();
    if (!parent) {
        return true;
    }

    return !parent->metaObject()->inherits(&QAbstractSpinBox::staticMetaObject)
           && !parent->metaObject()->inherits(&QComboBox::staticMetaObject);
}

WidgetStateSerializer::WidgetStateSerializer(QObject *parent) : QObject(parent)
{
}

QJsonObject WidgetStateSerializer::toJson(const QWidget *w, const QStringList &ignoredWidgets)
{
    QJsonObject obj;

    auto const lineEdits = w->findChildren<const QLineEdit *>();
    for (const QLineEdit *lineEdit : lineEdits) {
        if (!isApplicationWidget(lineEdit)) {
            continue;
        }
        obj[lineEdit->objectName()] = lineEdit->text();
    }

    auto const checkBoxes = w->findChildren<const QCheckBox *>();
    for (const QCheckBox *checkBox : checkBoxes) {
        if (!isApplicationWidget(checkBox)) {
            continue;
        }
        obj[checkBox->objectName()] = checkBox->isChecked();
    }

    auto const comboBoxes = w->findChildren<const QComboBox *>();
    for (const QComboBox *comboBox : comboBoxes) {
        if (!isApplicationWidget(comboBox)) {
            continue;
        }
        // Stored by the identifier of the selected entry - the attack mode or
        // hash type id - not by its position, which a list that grows in the
        // middle shifts. A combo box without identifiers falls back to that.
        obj[comboBox->objectName()] = comboBoxHasItemData(comboBox)
                                          ? QJsonValue::fromVariant(comboBox->currentData())
                                          : QJsonValue(comboBox->currentIndex());
    }

    auto const radioButtons = w->findChildren<const QRadioButton *>();
    for (const QRadioButton *radioButton : radioButtons) {
        if (!isApplicationWidget(radioButton)) {
            continue;
        }
        obj[radioButton->objectName()] = radioButton->isChecked();
    }

    auto const spinBoxes = w->findChildren<const QSpinBox *>();
    for (const QSpinBox *spinBox : spinBoxes) {
        if (!isApplicationWidget(spinBox)) {
            continue;
        }
        obj[spinBox->objectName()] = spinBox->value();
    }

    auto const doubleSpinBoxes = w->findChildren<const QDoubleSpinBox *>();
    for (const QDoubleSpinBox *doubleSpinBox : doubleSpinBoxes) {
        if (!isApplicationWidget(doubleSpinBox)) {
            continue;
        }
        obj[doubleSpinBox->objectName()] = doubleSpinBox->value();
    }

    auto const listWidgets = w->findChildren<const QListWidget *>();
    for (const QListWidget *listWidget : listWidgets) {
        if (!isApplicationWidget(listWidget)) {
            continue;
        }
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

    auto const checkBoxes = w->findChildren<QCheckBox *>();
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

    auto const comboBoxes = w->findChildren<QComboBox *>();
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

    auto const radioButtons = w->findChildren<QRadioButton *>();
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

    auto const spinBoxes = w->findChildren<QSpinBox *>();
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

        if (!value.isDouble()) {
            result.wrongTypes << name;
            continue;
        }
        spinBox->setValue(value.toInt());
    }

    auto const doubleSpinBoxes = w->findChildren<QDoubleSpinBox *>();
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

    auto const listWidgets = w->findChildren<QListWidget *>();
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
