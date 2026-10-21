/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QJsonObject>
#include <QLineEdit>
#include <QListWidget>
#include <QRadioButton>
#include <QSpinBox>

#include "widgetstateserializer.h"

// What the mapping between a form and its values does: every widget type the
// serializer knows, the fields the caller asked to keep out, and the names of
// the values that no widget claimed.
class TestWidgetStateSerializer : public QObject
{
    Q_OBJECT

private slots:
    void roundTrip();
    void comboBoxIdentifiersSurviveAShiftedList();
    void comboBoxWithoutDataIsStoredByItsIndex();
    void comboBoxIdentifierIsNotMatchedByText();
    void anUnknownIdentifierChangesNothing();
    void nonAsciiValuesSurviveTheValues();
    void ignoredWidgetsAreNotSaved();
    void ignoredWidgetsAreNotRestored();
    void aValueNoWidgetClaimedIsReported();
    void theInputIsNotModified();
};

namespace
{

// One of every widget type the serializer understands, under object names a
// real session would carry.
struct Form {
    Form()
    {
        lineEdit = new QLineEdit(&widget);
        lineEdit->setObjectName(QStringLiteral("lineEdit_command"));

        checkBox = new QCheckBox(&widget);
        checkBox->setObjectName(QStringLiteral("checkBox_rules"));

        comboBox = new QComboBox(&widget);
        comboBox->setObjectName(QStringLiteral("comboBox_language"));
        comboBox->addItems(
            {QStringLiteral("English"), QStringLiteral("Deutsch"), QStringLiteral("Srpski")});

        // A combo box whose entries carry an id, the way the attack modes
        // and the hash types do.
        comboBoxWithData = new QComboBox(&widget);
        comboBoxWithData->setObjectName(QStringLiteral("comboBox_hash_type"));
        comboBoxWithData->addItem(QStringLiteral("MD5"), 0u);
        comboBoxWithData->addItem(QStringLiteral("NTLM"), 1000u);

        radioButton = new QRadioButton(&widget);
        radioButton->setObjectName(QStringLiteral("radioButton_plain"));

        spinBox = new QSpinBox(&widget);
        spinBox->setObjectName(QStringLiteral("spinBox_segment_size"));

        doubleSpinBox = new QDoubleSpinBox(&widget);
        doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox_ratio"));

        listWidget = new QListWidget(&widget);
        listWidget->setObjectName(QStringLiteral("listWidget_wordlist"));
    }

    QWidget widget;
    QLineEdit *lineEdit;
    QCheckBox *checkBox;
    QComboBox *comboBox;
    QComboBox *comboBoxWithData;
    QRadioButton *radioButton;
    QSpinBox *spinBox;
    QDoubleSpinBox *doubleSpinBox;
    QListWidget *listWidget;
};

// Values no freshly built form holds, so a load that changes nothing is
// caught by every single comparison below.
void fill(Form &form)
{
    form.lineEdit->setText(QStringLiteral("-m 0 --quiet"));
    form.checkBox->setChecked(true);
    form.comboBox->setCurrentIndex(2);
    form.comboBoxWithData->setCurrentIndex(1);
    form.radioButton->setChecked(true);
    form.spinBox->setValue(42);
    form.doubleSpinBox->setValue(2.5);

    auto *rockyou = new QListWidgetItem(QStringLiteral("rockyou.txt"), form.listWidget);
    rockyou->setCheckState(Qt::Checked);
    auto *best64 = new QListWidgetItem(QStringLiteral("best64.rule"), form.listWidget);
    best64->setCheckState(Qt::Unchecked);
}

} // namespace

// Save one form, restore it into a second one, and find every value where
// it was - text, flags, indices, numbers and the list with its check states.
void TestWidgetStateSerializer::roundTrip()
{
    Form source;
    fill(source);

    const QJsonObject values = WidgetStateSerializer::toJson(&source.widget);

    Form restored;
    WidgetStateSerializer::applyToWidgets(values, &restored.widget);

    QCOMPARE(restored.lineEdit->text(), QStringLiteral("-m 0 --quiet"));
    QCOMPARE(restored.checkBox->isChecked(), true);
    QCOMPARE(restored.comboBox->currentIndex(), 2);
    QCOMPARE(restored.comboBoxWithData->currentIndex(), 1);
    QCOMPARE(restored.comboBoxWithData->currentData().toUInt(), 1000u);
    QCOMPARE(restored.radioButton->isChecked(), true);
    QCOMPARE(restored.spinBox->value(), 42);
    QCOMPARE(restored.doubleSpinBox->value(), 2.5);
    QCOMPARE(restored.listWidget->count(), 2);
    QCOMPARE(restored.listWidget->item(0)->text(), QStringLiteral("rockyou.txt"));
    QCOMPARE(restored.listWidget->item(0)->checkState(), Qt::Checked);
    QCOMPARE(restored.listWidget->item(1)->text(), QStringLiteral("best64.rule"));
    QCOMPARE(restored.listWidget->item(1)->checkState(), Qt::Unchecked);
}

// A combo box is stored by the identifier of its selected entry. A list
// that grew in the middle must not change what a session selects.
void TestWidgetStateSerializer::comboBoxIdentifiersSurviveAShiftedList()
{
    Form source;
    source.comboBoxWithData->setCurrentIndex(1); // NTLM, id 1000

    // The values hold the id, not the position...
    const QJsonObject values = WidgetStateSerializer::toJson(&source.widget);
    QCOMPARE(values.value(QStringLiteral("comboBox_hash_type")).toInt(), 1000);

    // ... and a list that has grown in front of it still selects the same
    // entry.
    Form restored;
    restored.comboBoxWithData->clear();
    restored.comboBoxWithData->addItem(QStringLiteral("SHA-1"), 100u);
    restored.comboBoxWithData->addItem(QStringLiteral("MD5"), 0u);
    restored.comboBoxWithData->addItem(QStringLiteral("NTLM"), 1000u);

    WidgetStateSerializer::applyToWidgets(values, &restored.widget);

    QCOMPARE(restored.comboBoxWithData->currentIndex(), 2);
    QCOMPARE(restored.comboBoxWithData->currentData().toUInt(), 1000u);
}

// A combo box whose entries carry no identifier falls back to its position.
void TestWidgetStateSerializer::comboBoxWithoutDataIsStoredByItsIndex()
{
    Form source;
    source.comboBox->setCurrentIndex(1); // "Deutsch"

    const QJsonObject values = WidgetStateSerializer::toJson(&source.widget);
    QCOMPARE(values.value(QStringLiteral("comboBox_language")).toInt(), 1);

    Form restored;
    WidgetStateSerializer::applyToWidgets(values, &restored.widget);

    QCOMPARE(restored.comboBox->currentIndex(), 1);
    QCOMPARE(restored.comboBox->currentText(), QStringLiteral("Deutsch"));
}

// A stored identifier is looked up in the item data only; a display text that
// happens to match it must not select an entry the session never named.
void TestWidgetStateSerializer::comboBoxIdentifierIsNotMatchedByText()
{
    QJsonObject values;
    values.insert(QStringLiteral("comboBox_hash_type"), 1);

    Form restored;
    restored.comboBoxWithData->clear();
    // The text "1" is not the identifier 1; only the data counts.
    restored.comboBoxWithData->addItem(QStringLiteral("1"), 5u);
    restored.comboBoxWithData->addItem(QStringLiteral("MD5"), 0u);
    restored.comboBoxWithData->setCurrentIndex(1); // MD5, id 0

    WidgetStateSerializer::applyToWidgets(values, &restored.widget);

    // The identifier 1 is unknown, so nothing changes - even though an entry
    // displays "1".
    QCOMPARE(restored.comboBoxWithData->currentIndex(), 1);
    QCOMPARE(restored.comboBoxWithData->currentData().toUInt(), 0u);
}

// An identifier the list does not know leaves the combo box alone. The
// position would point at a different entry, and silently selecting that is
// exactly what the identifier exists to prevent.
void TestWidgetStateSerializer::anUnknownIdentifierChangesNothing()
{
    QJsonObject values;
    values.insert(QStringLiteral("comboBox_hash_type"), 9999);

    Form restored;
    restored.comboBoxWithData->setCurrentIndex(1); // NTLM

    WidgetStateSerializer::applyToWidgets(values, &restored.widget);

    QCOMPARE(restored.comboBoxWithData->currentIndex(), 1);
    QCOMPARE(restored.comboBoxWithData->currentData().toUInt(), 1000u);
}

// A field may hold non-ASCII text, and it has to survive the round trip
// unchanged. The literals are \u escapes so that the test does not depend on
// the encoding of its own source file.
void TestWidgetStateSerializer::nonAsciiValuesSurviveTheValues()
{
    const QString text = QStringLiteral("W\u00f6rterbuch \u2013 \u00e4\u00f6\u00fc\u00df");
    const QString entry = QStringLiteral("\u65e5\u672c\u8a9e.txt"); // 日本語.txt

    Form source;
    source.lineEdit->setText(text);
    auto *item = new QListWidgetItem(entry, source.listWidget);
    item->setCheckState(Qt::Checked);

    const QJsonObject values = WidgetStateSerializer::toJson(&source.widget);

    Form restored;
    WidgetStateSerializer::applyToWidgets(values, &restored.widget);

    QCOMPARE(restored.lineEdit->text(), text);
    QCOMPARE(restored.listWidget->count(), 1);
    QCOMPARE(restored.listWidget->item(0)->text(), entry);
    QCOMPARE(restored.listWidget->item(0)->checkState(), Qt::Checked);
}

// The caller named lineEdit_command; its key and its value must not reach
// the values, while the rest of the form does.
void TestWidgetStateSerializer::ignoredWidgetsAreNotSaved()
{
    Form source;
    fill(source);

    const QJsonObject values =
        WidgetStateSerializer::toJson(&source.widget, {QStringLiteral("lineEdit_command")});

    QVERIFY(!values.contains(QStringLiteral("lineEdit_command")));
    QVERIFY(!values.contains(QStringLiteral("-m 0 --quiet")));
    QVERIFY(values.contains(QStringLiteral("spinBox_segment_size")));
}

// The values do hold the value; the loader was told to leave lineEdit_command
// alone, so the target keeps its own text while everything else restores.
void TestWidgetStateSerializer::ignoredWidgetsAreNotRestored()
{
    Form source;
    fill(source);

    const QJsonObject values = WidgetStateSerializer::toJson(&source.widget);

    Form target;
    target.lineEdit->setText(QStringLiteral("keep me"));
    WidgetStateSerializer::applyToWidgets(values, &target.widget,
                                          {QStringLiteral("lineEdit_command")});

    QCOMPARE(target.lineEdit->text(), QStringLiteral("keep me"));
    // ... and the widgets not on the list still restored
    QVERIFY(target.checkBox->isChecked());
    QCOMPARE(target.spinBox->value(), 42);
}

// A value that names no widget is the one of a file that was made by another
// version, or that was edited by hand. The caller has to be able to name it.
void TestWidgetStateSerializer::aValueNoWidgetClaimedIsReported()
{
    QJsonObject values;
    values.insert(QStringLiteral("lineEdit_command"), QStringLiteral("-m 0 --quiet"));
    values.insert(QStringLiteral("lineEdit_fromTheFuture"), QStringLiteral("something"));

    Form restored;
    const QStringList unknown = WidgetStateSerializer::applyToWidgets(values, &restored.widget);

    QCOMPARE(unknown, QStringList{QStringLiteral("lineEdit_fromTheFuture")});
    // The value that did name a widget was applied.
    QCOMPARE(restored.lineEdit->text(), QStringLiteral("-m 0 --quiet"));
}

// The caller reads the values after they were applied - the hash type, for
// one - so the mapping must not touch what it was given.
void TestWidgetStateSerializer::theInputIsNotModified()
{
    Form source;
    fill(source);

    const QJsonObject values = WidgetStateSerializer::toJson(&source.widget);

    Form restored;
    WidgetStateSerializer::applyToWidgets(values, &restored.widget);

    QCOMPARE(values.value(QStringLiteral("comboBox_hash_type")).toInt(), 1000);
    QCOMPARE(values.value(QStringLiteral("lineEdit_command")).toString(),
             QStringLiteral("-m 0 --quiet"));
}

int main(int argc, char *argv[])
{
    TestWidgetStateSerializer test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_widgetstateserializer.moc"
