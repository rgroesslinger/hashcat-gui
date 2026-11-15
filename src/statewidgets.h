/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef STATEWIDGETS_H
#define STATEWIDGETS_H

#include <QAbstractSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QList>
#include <QListWidget>
#include <QRadioButton>
#include <QSpinBox>
#include <QWidget>

// Whether the widget is one the application put there.
//
// Two kinds of widgets are not: the editor a spin box or an editable combo
// box builds for itself is a child of that widget, and the name Qt gives it
// is the same for every spin box - a value stored under it would mean
// nothing, and a file that names it would change a spin box. A widget
// without a name at all would share the key "" with every other unnamed
// widget.
inline bool isApplicationWidget(const QWidget *widget)
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

// The seven widget types the session format knows, walked recursively below
// root and filtered to the application's own widgets. The preview wiring
// connects this set; the serializer shares the isApplicationWidget predicate
// above, so persistence and wiring cannot drift apart.
struct StateWidgetLists {
    QList<QLineEdit *> lineEdits;
    QList<QCheckBox *> checkBoxes;
    QList<QComboBox *> comboBoxes;
    QList<QRadioButton *> radioButtons;
    QList<QSpinBox *> spinBoxes;
    QList<QDoubleSpinBox *> doubleSpinBoxes;
    QList<QListWidget *> listWidgets;
};

inline StateWidgetLists collectStateWidgets(QWidget *root)
{
    StateWidgetLists lists;

    const auto lineEdits = root->findChildren<QLineEdit *>();
    for (QLineEdit *widget : lineEdits) {
        if (isApplicationWidget(widget)) {
            lists.lineEdits << widget;
        }
    }
    const auto checkBoxes = root->findChildren<QCheckBox *>();
    for (QCheckBox *widget : checkBoxes) {
        if (isApplicationWidget(widget)) {
            lists.checkBoxes << widget;
        }
    }
    const auto comboBoxes = root->findChildren<QComboBox *>();
    for (QComboBox *widget : comboBoxes) {
        if (isApplicationWidget(widget)) {
            lists.comboBoxes << widget;
        }
    }
    const auto radioButtons = root->findChildren<QRadioButton *>();
    for (QRadioButton *widget : radioButtons) {
        if (isApplicationWidget(widget)) {
            lists.radioButtons << widget;
        }
    }
    const auto spinBoxes = root->findChildren<QSpinBox *>();
    for (QSpinBox *widget : spinBoxes) {
        if (isApplicationWidget(widget)) {
            lists.spinBoxes << widget;
        }
    }
    const auto doubleSpinBoxes = root->findChildren<QDoubleSpinBox *>();
    for (QDoubleSpinBox *widget : doubleSpinBoxes) {
        if (isApplicationWidget(widget)) {
            lists.doubleSpinBoxes << widget;
        }
    }
    const auto listWidgets = root->findChildren<QListWidget *>();
    for (QListWidget *widget : listWidgets) {
        if (isApplicationWidget(widget)) {
            lists.listWidgets << widget;
        }
    }

    return lists;
}

#endif // STATEWIDGETS_H