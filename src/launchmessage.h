/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef LAUNCHMESSAGE_H
#define LAUNCHMESSAGE_H

#include "launchvalidator.h"

#include <QMessageBox>
#include <QString>

// The message box data for one launch problem: a value, so the mapping from
// error to message is testable without a display. The caller owns the box
// itself - and its title, which a launch error never sets.
struct LaunchMessage {
    QMessageBox::Icon icon = QMessageBox::Warning;
    Qt::TextFormat format = Qt::AutoText;
    QString text;
};

// The message for one launch problem. The menu labels are precomputed by the
// caller (the menu titles carry mnemonics and an ellipsis the message must
// not show). An empty text means "no message": LaunchError::None.
LaunchMessage messageFor(const LaunchValidator::LaunchProblem &problem,
                         const LaunchValidator::LaunchContext &context,
                         const QString &fileMenuLabel, const QString &settingsMenuLabel);

#endif // LAUNCHMESSAGE_H