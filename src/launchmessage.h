/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef LAUNCHMESSAGE_H
#define LAUNCHMESSAGE_H

#include "launchvalidator.h"

#include <QMessageBox>
#include <QString>

struct LaunchCommand;

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

// The message for a command the catalog refused to build. An empty text
// means the command was built (LaunchCommand::Refusal::None).
LaunchMessage messageFor(const LaunchCommand &command, const QString &configuredTerminal);

#endif // LAUNCHMESSAGE_H