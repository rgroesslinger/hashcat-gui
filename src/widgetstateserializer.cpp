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
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSaveFile>

// Report a failure through errorMessage when the caller asked for it
static void setError(QString *errorMessage, const QString &text)
{
    if (errorMessage) {
        *errorMessage = text;
    }
}

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

static QJsonObject widgetToJson(const QWidget *w, const QStringList &ignoredWidgets = {})
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

static void jsonToWidget(const QJsonObject &obj, QWidget *w, const QStringList &ignoredWidgets = {})
{
    QJsonObject mutableObj = obj;

    for (const QString &name : ignoredWidgets) {
        mutableObj.remove(name);
    }

    auto const lineEdits = w->findChildren<QLineEdit *>();
    for (QLineEdit *lineEdit : lineEdits) {
        if (mutableObj.contains(lineEdit->objectName())) {
            lineEdit->setText(mutableObj[lineEdit->objectName()].toString());
        }
    }

    auto const checkBoxes = w->findChildren<QCheckBox *>();
    for (QCheckBox *checkBox : checkBoxes) {
        if (mutableObj.contains(checkBox->objectName())) {
            checkBox->setChecked(mutableObj[checkBox->objectName()].toBool());
        }
    }

    auto const comboBoxes = w->findChildren<QComboBox *>();
    for (QComboBox *comboBox : comboBoxes) {
        const QString name = comboBox->objectName();
        if (!mutableObj.contains(name)) {
            continue;
        }

        const QJsonValue stored = mutableObj.value(name);
        if (stored.isUndefined() || stored.isNull()) {
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
        if (mutableObj.contains(radioButton->objectName())) {
            radioButton->setChecked(mutableObj[radioButton->objectName()].toBool());
        }
    }

    auto const spinBoxes = w->findChildren<QSpinBox *>();
    for (QSpinBox *spinBox : spinBoxes) {
        if (mutableObj.contains(spinBox->objectName())) {
            spinBox->setValue(mutableObj[spinBox->objectName()].toInt());
        }
    }

    auto const doubleSpinBoxes = w->findChildren<QDoubleSpinBox *>();
    for (QDoubleSpinBox *doubleSpinBox : doubleSpinBoxes) {
        if (mutableObj.contains(doubleSpinBox->objectName())) {
            doubleSpinBox->setValue(mutableObj[doubleSpinBox->objectName()].toDouble());
        }
    }

    auto const listWidgets = w->findChildren<QListWidget *>();
    for (QListWidget *listWidget : listWidgets) {
        if (mutableObj.contains(listWidget->objectName())) {
            listWidget->clear();
            const QJsonArray list = mutableObj[listWidget->objectName()].toArray();
            for (const auto &v : list) {
                QJsonObject li = v.toObject();
                QListWidgetItem *item =
                    new QListWidgetItem(li[QStringLiteral("text")].toString(), listWidget);
                item->setCheckState(li[QStringLiteral("checked")].toBool() ? Qt::Checked
                                                                           : Qt::Unchecked);
            }
        }
    }
}

bool WidgetStateSerializer::saveStateToFile(const QString &key, const QWidget *widget,
                                            const QString &filename,
                                            const QStringList &ignoredWidgets,
                                            QString *errorMessage) const
{
    QJsonObject root;
    root[QStringLiteral("version")] = AppConstants::Profiles::Version;
    root[key] = widgetToJson(widget, ignoredWidgets);

    // QSaveFile writes to a temporary file next to the target and renames it
    // over the target only when the write succeeded. A crash in the middle
    // therefore leaves the old profile alone instead of a truncated one,
    // which the next start would report as a broken profile.
    QSaveFile f(filename);
    if (!f.open(QIODevice::WriteOnly)) {
        setError(errorMessage,
                 tr("Could not open %1 for writing: %2").arg(filename, f.errorString()));
        return false;
    }

    const QByteArray data = QJsonDocument(root).toJson(QJsonDocument::Indented);
    if (f.write(data) != data.size() || !f.commit()) {
        setError(errorMessage, tr("Could not write to %1: %2").arg(filename, f.errorString()));
        return false;
    }

    setError(errorMessage, QString());
    return true;
}

bool WidgetStateSerializer::loadStateFromFile(const QString &key, QWidget *widget,
                                              const QString &filename,
                                              const QStringList &ignoredWidgets,
                                              QString *errorMessage, QJsonObject *profileState,
                                              QString *warning) const
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        setError(errorMessage,
                 tr("Could not open %1 for reading: %2").arg(filename, f.errorString()));
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

    // A profile names the widgets by their object name, so the names are part
    // of the format. A profile from another version still names the widgets
    // that did not change, so it is read - the values of the widgets it names
    // no longer exist are left at their defaults, and the user is told that
    // the profile is not complete.
    const int version = root.value(QStringLiteral("version")).toInt(-1);
    if (version != AppConstants::Profiles::Version && warning) {
        *warning = tr("This profile was made by another version of hashcat-gui. It was loaded, but "
                      "the values it does not know about were left at their defaults.");
    }

    const QJsonObject state = root[key].toObject();
    if (profileState) {
        *profileState = state;
    }

    jsonToWidget(state, widget, ignoredWidgets);
    setError(errorMessage, QString());
    return true;
}
