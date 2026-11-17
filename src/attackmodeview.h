/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef ATTACKMODEVIEW_H
#define ATTACKMODEVIEW_H

#include "hashcatoptions.h"

#include <QCoreApplication>
#include <QString>

namespace Ui
{
class MainWindow;
}

// The attack mode's face on the form: which groups the mode uses, and the
// hint that says what it expects. Mode gating only - the option dependencies
// that follow a checkbox, not the mode, stay with the window.
//
// The very predicates CommandBuilder applies decide the visibility, so the
// widgets show exactly the options that end up in the command line and the
// two can never drift apart. Groups are hidden rather than disabled so the
// form only shows the options the mode uses; the window keeps room for the
// busiest mode anyway, see MainWindow::attackModeEnvelope().
class AttackModeView
{
    // The mode names and hints are the unit's own texts
    Q_DECLARE_TR_FUNCTIONS(AttackModeView)

public:
    explicit AttackModeView(Ui::MainWindow &ui);

    // The translated name of an attack mode, shared by the combo box and the
    // example submenu so the two cannot drift apart.
    static QString name(AttackMode mode);

    // What the user has to provide for the mode, in the wording of
    // CommandBuilder::modeRequirements(), so the hint and the launch
    // validation cannot tell the user different things. The label wraps at
    // the tab width, so the texts stay free of line breaks and translations
    // can rewrap. A wrapped label reports one line in its minimum size hint,
    // so the fit reserves its height through heightForWidth(); see
    // attackModeEnvelope() and fitToContent().
    static QString hint(AttackMode mode);

    // Shows the groups the mode uses, hides the rest, and sets the hint.
    // Fitting the window measures this once per mode, so the hint text has
    // to be here as well - it sets the width the window has to hold.
    void apply(AttackMode mode) const;

private:
    Ui::MainWindow *m_ui;
};

#endif // ATTACKMODEVIEW_H