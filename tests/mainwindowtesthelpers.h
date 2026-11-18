/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef MAINWINDOWTESTHELPERS_H
#define MAINWINDOWTESTHELPERS_H

#include "appconstants.h"
#include "appearance.h"
#include "examples.h"
#include "mainwindowtestaccess.h"
#include "statefile.h"
#include "testenvironment.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDir>
#include <QDoubleSpinBox>
#include <QFile>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QRadioButton>
#include <QSpinBox>
#include <QStandardPaths>
#include <QTimer>
#include <QWidget>

// Puts every widget below root into a state the defaults do not have, so a
// reset that forgets one of them shows up in the comparison with a fresh
// window. The command preview is left alone: it is generated, not entered.
inline void changeEveryValue(QWidget *root)
{
    const auto lineEdits = root->findChildren<QLineEdit *>();
    for (QLineEdit *lineEdit : lineEdits) {
        if (lineEdit->objectName() == AppConstants::Files::PreviewWidget) {
            continue;
        }
        lineEdit->setText(lineEdit->text().isEmpty() ? QStringLiteral("changed") : QString());
    }

    const auto checkBoxes = root->findChildren<QCheckBox *>();
    for (QCheckBox *checkBox : checkBoxes) {
        checkBox->setChecked(!checkBox->isChecked());
    }

    // The radio buttons of a group exclude each other, so the reset has to
    // name the one that is checked.
    const auto radioButtons = root->findChildren<QRadioButton *>();
    for (QRadioButton *radioButton : radioButtons) {
        radioButton->setChecked(!radioButton->isChecked());
    }

    const auto comboBoxes = root->findChildren<QComboBox *>();
    for (QComboBox *comboBox : comboBoxes) {
        if (comboBox->count() > 1) {
            comboBox->setCurrentIndex(comboBox->currentIndex() == 0 ? 1 : 0);
        }
    }

    const auto spinBoxes = root->findChildren<QSpinBox *>();
    for (QSpinBox *spinBox : spinBoxes) {
        if (spinBox->minimum() != spinBox->maximum()) {
            spinBox->setValue(spinBox->value() == spinBox->maximum() ? spinBox->value() - 1
                                                                     : spinBox->value() + 1);
        }
    }

    const auto doubleSpinBoxes = root->findChildren<QDoubleSpinBox *>();
    for (QDoubleSpinBox *doubleSpinBox : doubleSpinBoxes) {
        if (doubleSpinBox->minimum() != doubleSpinBox->maximum()) {
            doubleSpinBox->setValue(doubleSpinBox->value() == doubleSpinBox->maximum()
                                        ? doubleSpinBox->value() - 1
                                        : doubleSpinBox->value() + 1);
        }
    }

    const auto listWidgets = root->findChildren<QListWidget *>();
    for (QListWidget *listWidget : listWidgets) {
        listWidget->addItem(QStringLiteral("changed"));
    }
}

// The session file a serializer wrote, as the object it holds.
inline QJsonObject readJsonObject(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return {};
    }
    const QByteArray data = file.readAll();
    file.close();
    return QJsonDocument::fromJson(data).object();
}

// The widgets whose values in filename differ from the ones in reference, by
// name. A widget the reset forgot is named here, not just counted.
inline QStringList differingWidgets(const QString &filename, const QJsonObject &reference)
{
    const QJsonObject state = readJsonObject(filename)
                                  .value(QStringLiteral("sections"))
                                  .toObject()
                                  .value(QString(AppConstants::FileSections::Session))
                                  .toObject()
                                  .value(QStringLiteral("values"))
                                  .toObject();

    QStringList names;
    for (auto it = state.begin(); it != state.end(); ++it) {
        if (reference.value(it.key()) != it.value()) {
            names << it.key();
        }
    }
    for (auto it = reference.begin(); it != reference.end(); ++it) {
        if (!state.contains(it.key())) {
            names << it.key();
        }
    }
    return names;
}

// A wordlist the monitor can see. The handle is closed at the end of the
// scope, so the file can be removed again while the window watches it.
inline bool writeFile(const QString &path)
{
    const QByteArray content = QByteArrayLiteral("password\n");
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }
    return file.write(content) == content.size();
}

// A group box in a row is as tall as its tallest neighbour. Without top
// alignment Qt spreads the children of the shorter boxes over that height or
// centers them, so the content would not start at the same height.
inline QStringList groupBoxesWithoutTopAlignment(QWidget *page)
{
    QStringList offenders;
    const auto boxes = page->findChildren<QGroupBox *>();
    for (QGroupBox *box : boxes) {
        if (!(box->layout()->alignment() & Qt::AlignTop)) {
            offenders << box->objectName();
        }
    }
    return offenders;
}

// A hashcat that answers the query for the hash types.
//
// A binary that is not there opens a "hashcat error" box while the test
// runs, and the recorder keeps only the last box it saw - so a test that
// wants a box of its own needs a hashcat that answers.
inline QString hashcatStub(const QString &name)
{
    return TestEnvironment::writeHashcatStub(name);
}

// The example submenu action for one attack mode.
inline QAction *exampleAction(MainWindow &window, AttackMode mode)
{
    const QList<QAction *> actions =
        MainWindowTestAccess::ui(window)->menuLoadExampleSession->actions();
    for (QAction *action : actions) {
        if (action->data().toInt() == static_cast<int>(mode)) {
            return action;
        }
    }
    return nullptr;
}

// The shared per-test state: deterministic settings, the session scratch
// cleanup, and the modal watchdog. A fixture object, not a base class with
// init()/cleanup() slots: a derived slot of the same name shadows the base
// one in the meta-object and silently stops the shared part.
class MainWindowFixture : public QObject
{
public:
    MainWindowFixture()
    {
        // Safety net for the tests that spin the event loop: a reply the stub
        // cannot produce opens a modal warning, and a modal must never be able
        // to hang the run. cleanup() stops it before it can fire into a later
        // test.
        m_watchdog.setSingleShot(false);
        m_watchdog.setInterval(100);
        connect(&m_watchdog, &QTimer::timeout, this, [this] {
            if (QWidget *modal = QApplication::activeModalWidget()) {
                ++m_closedModals;
                modal->close();
            }
        });
    }

    void init()
    {
        // The test-mode directories persist across runs, so a run that crashed or
        // was killed can leave a session and unpacked examples behind. Removing
        // them here - not in cleanup() - is what guarantees that the first test of
        // the next run starts as fresh as every later one. resetSettings() clears
        // the ini so every setting falls back to its production default.
        QFile::remove(StateFile::sessionFilePath());
        QDir(QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))
                 .filePath(QString(Examples::DataDirectory)))
            .removeRecursively();
        TestEnvironment::resetSettings();

        // Save applies the color scheme to the whole application, and the import
        // tests drive Save: reset it like TestSettingsDialog::init() does, so the
        // contract is the same in every fixture.
        Appearance::applyColorScheme(QString());

        m_closedModals = 0;
    }

    void cleanup()
    {
        m_watchdog.stop();
    }

    void startWatchdog()
    {
        m_watchdog.start();
    }
    void stopWatchdog()
    {
        m_watchdog.stop();
    }
    [[nodiscard]] int closedModals() const
    {
        return m_closedModals;
    }

private:
    // The modals the watchdog had to close: a test that expects none.
    int m_closedModals = 0;
    QTimer m_watchdog;
};

#endif // MAINWINDOWTESTHELPERS_H