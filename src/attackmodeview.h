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

// The attack mode's face on the form: which groups it uses and the hint that
// says what it expects. Mode gating only; option dependencies stay with the
// window. CommandBuilder's predicates decide visibility, so the widgets show
// exactly what reaches the command line, and hidden groups leave the form to
// the mode while the window keeps room for the busiest one.
class AttackModeView
{
    // The mode names and hints are the unit's own texts
    Q_DECLARE_TR_FUNCTIONS(AttackModeView)

public:
    explicit AttackModeView(Ui::MainWindow &ui);

    // The translated name of an attack mode, shared by the combo box and the
    // example submenu so the two cannot drift apart.
    static QString name(AttackMode mode);

    // What the user has to provide, in CommandBuilder::modeRequirements()'s
    // wording, so the hint and the launch validation agree. The label wraps at
    // the tab width and stays free of line breaks; a wrapped label reports one
    // line, so the fit reserves its height through heightForWidth() (see
    // attackModeEnvelope()).
    static QString hint(AttackMode mode);

    // Shows the groups the mode uses, hides the rest, and sets the hint.
    // Fitting the window measures this once per mode, so the hint text has
    // to be here as well - it sets the width the window has to hold.
    void apply(AttackMode mode) const;

private:
    Ui::MainWindow *m_ui;
};

#endif // ATTACKMODEVIEW_H