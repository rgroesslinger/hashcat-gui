/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QListWidget>
#include <QRadioButton>
#include <QSpinBox>

#include "widgetstateserializer.h"

// What a profile write does to a form, and what a profile read gives back:
// every widget type the serializer knows, the fields the caller asked to
// keep out of the file, and a clear answer when the file is unusable.
class TestWidgetStateSerializer : public QObject
{
    Q_OBJECT

private slots:
    void roundTrip();
    void ignoredWidgetsAreNotSaved();
    void ignoredWidgetsAreNotRestored();
    void corruptJsonIsRejected();
    void missingProfileIsRejected();
    void writeFailureIsReported();
    void readFailureIsReported();
    void errorIsClearedOnSuccess();
};

namespace
{

// One of every widget type the serializer understands, under object names a
// real profile would carry.
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

        radioButton = new QRadioButton(&widget);
        radioButton->setObjectName(QStringLiteral("radioButton_plain"));

        spinBox = new QSpinBox(&widget);
        spinBox->setObjectName(QStringLiteral("spinBox_segment"));

        doubleSpinBox = new QDoubleSpinBox(&widget);
        doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox_ratio"));

        listWidget = new QListWidget(&widget);
        listWidget->setObjectName(QStringLiteral("listWidget_wordlists"));
    }

    QWidget widget;
    QLineEdit *lineEdit;
    QCheckBox *checkBox;
    QComboBox *comboBox;
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

    WidgetStateSerializer serializer;
    QString errorMessage;
    const QString file = TestEnvironment::configHome().filePath(QStringLiteral("roundtrip.json"));

    QVERIFY2(serializer.saveStateToFile(QStringLiteral("default"), &source.widget, file, {},
                                        &errorMessage),
             qPrintable(errorMessage));

    Form restored;
    QVERIFY2(serializer.loadStateFromFile(QStringLiteral("default"), &restored.widget, file, {},
                                          &errorMessage),
             qPrintable(errorMessage));

    QCOMPARE(restored.lineEdit->text(), QStringLiteral("-m 0 --quiet"));
    QCOMPARE(restored.checkBox->isChecked(), true);
    QCOMPARE(restored.comboBox->currentIndex(), 2);
    QCOMPARE(restored.radioButton->isChecked(), true);
    QCOMPARE(restored.spinBox->value(), 42);
    QCOMPARE(restored.doubleSpinBox->value(), 2.5);
    QCOMPARE(restored.listWidget->count(), 2);
    QCOMPARE(restored.listWidget->item(0)->text(), QStringLiteral("rockyou.txt"));
    QCOMPARE(restored.listWidget->item(0)->checkState(), Qt::Checked);
    QCOMPARE(restored.listWidget->item(1)->text(), QStringLiteral("best64.rule"));
    QCOMPARE(restored.listWidget->item(1)->checkState(), Qt::Unchecked);
}

// The caller named lineEdit_command; its key and its value must not reach
// the file, while the rest of the form does.
void TestWidgetStateSerializer::ignoredWidgetsAreNotSaved()
{
    Form source;
    fill(source);

    WidgetStateSerializer serializer;
    QString errorMessage;
    const QString file =
        TestEnvironment::configHome().filePath(QStringLiteral("ignored-save.json"));

    QVERIFY2(serializer.saveStateToFile(QStringLiteral("default"), &source.widget, file,
                                        {QStringLiteral("lineEdit_command")}, &errorMessage),
             qPrintable(errorMessage));

    QFile saved(file);
    QVERIFY(saved.open(QIODevice::ReadOnly));
    const QByteArray data = saved.readAll();
    QVERIFY(!data.contains("lineEdit_command"));
    QVERIFY(!data.contains("-m 0 --quiet"));
    QVERIFY(data.contains("spinBox_segment"));
}

// The file does hold the value; the loader was told to leave lineEdit_command
// alone, so the target keeps its own text while everything else restores.
void TestWidgetStateSerializer::ignoredWidgetsAreNotRestored()
{
    Form source;
    fill(source);

    WidgetStateSerializer serializer;
    QString errorMessage;
    const QString file =
        TestEnvironment::configHome().filePath(QStringLiteral("ignored-load.json"));
    QVERIFY2(serializer.saveStateToFile(QStringLiteral("default"), &source.widget, file, {},
                                        &errorMessage),
             qPrintable(errorMessage));

    Form target;
    target.lineEdit->setText(QStringLiteral("keep me"));
    QVERIFY2(serializer.loadStateFromFile(QStringLiteral("default"), &target.widget, file,
                                          {QStringLiteral("lineEdit_command")}, &errorMessage),
             qPrintable(errorMessage));

    QCOMPARE(target.lineEdit->text(), QStringLiteral("keep me"));
    // ... and the widgets not on the list still restored
    QVERIFY(target.checkBox->isChecked());
    QCOMPARE(target.spinBox->value(), 42);
}

// A file that is not JSON, and one that is JSON but not an object, are both
// refused - with a reason, and without touching a single widget.
void TestWidgetStateSerializer::corruptJsonIsRejected()
{
    const QString file = TestEnvironment::configHome().filePath(QStringLiteral("corrupt.json"));

    WidgetStateSerializer serializer;
    QString errorMessage;

    {
        QFile out(file);
        QVERIFY(out.open(QIODevice::WriteOnly | QIODevice::Truncate));
        out.write("{this is not json");
        out.close();
    }

    Form form;
    form.lineEdit->setText(QStringLiteral("must survive"));
    QVERIFY(!serializer.loadStateFromFile(QStringLiteral("default"), &form.widget, file, {},
                                          &errorMessage));
    QVERIFY(!errorMessage.isEmpty());
    QCOMPARE(form.lineEdit->text(), QStringLiteral("must survive"));

    {
        QFile out(file);
        QVERIFY(out.open(QIODevice::WriteOnly | QIODevice::Truncate));
        out.write("[1, 2, 3]");
        out.close();
    }

    QVERIFY(!serializer.loadStateFromFile(QStringLiteral("default"), &form.widget, file, {},
                                          &errorMessage));
    QVERIFY(!errorMessage.isEmpty());
    QCOMPARE(form.lineEdit->text(), QStringLiteral("must survive"));
}

// A perfectly good file that simply holds a different profile: the reply
// names the profile that was looked for, so the caller can say what went
// wrong without parsing anything.
void TestWidgetStateSerializer::missingProfileIsRejected()
{
    Form source;
    fill(source);

    WidgetStateSerializer serializer;
    QString errorMessage;
    const QString file =
        TestEnvironment::configHome().filePath(QStringLiteral("other-profile.json"));
    QVERIFY2(serializer.saveStateToFile(QStringLiteral("other"), &source.widget, file, {},
                                        &errorMessage),
             qPrintable(errorMessage));

    Form target;
    QVERIFY(!serializer.loadStateFromFile(QStringLiteral("default"), &target.widget, file, {},
                                          &errorMessage));
    QVERIFY(!errorMessage.isEmpty());
    QVERIFY(errorMessage.contains(QStringLiteral("default")));
}

// A profile path whose directory does not exist: the save has to fail with
// the path in the message, not write somewhere else and claim success.
void TestWidgetStateSerializer::writeFailureIsReported()
{
    Form form;
    WidgetStateSerializer serializer;
    QString errorMessage;
    const QString file =
        TestEnvironment::configHome().filePath(QStringLiteral("no-such-dir/profile.json"));

    QVERIFY(!serializer.saveStateToFile(QStringLiteral("default"), &form.widget, file, {},
                                        &errorMessage));
    QVERIFY(!errorMessage.isEmpty());
    QVERIFY(errorMessage.contains(file));
}

// The reading side of the same coin - the path again travels with the error.
void TestWidgetStateSerializer::readFailureIsReported()
{
    Form form;
    WidgetStateSerializer serializer;
    QString errorMessage;
    const QString file =
        TestEnvironment::configHome().filePath(QStringLiteral("never-written.json"));

    QVERIFY(!serializer.loadStateFromFile(QStringLiteral("default"), &form.widget, file, {},
                                          &errorMessage));
    QVERIFY(!errorMessage.isEmpty());
    QVERIFY(errorMessage.contains(file));
}

// The same QString rides through several saves and loads; a reason left
// behind by a failed one must not appear as the error of the next success.
void TestWidgetStateSerializer::errorIsClearedOnSuccess()
{
    Form source;
    fill(source);

    WidgetStateSerializer serializer;
    QString errorMessage = QStringLiteral("left over from an earlier failure");
    const QString file = TestEnvironment::configHome().filePath(QStringLiteral("cleared.json"));

    QVERIFY2(serializer.saveStateToFile(QStringLiteral("default"), &source.widget, file, {},
                                        &errorMessage),
             qPrintable(errorMessage));
    QVERIFY(errorMessage.isEmpty());

    Form restored;
    QVERIFY2(serializer.loadStateFromFile(QStringLiteral("default"), &restored.widget, file, {},
                                          &errorMessage),
             qPrintable(errorMessage));
    QVERIFY(errorMessage.isEmpty());
}

int main(int argc, char *argv[])
{
    TestWidgetStateSerializer test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_widgetstateserializer.moc"
