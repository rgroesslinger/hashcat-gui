/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef COMMANDBUILDER_H
#define COMMANDBUILDER_H

#include <QString>
#include <QStringList>

#include "hashcatoptions.h"

// Turns a HashcatOptions into the hashcat argument vector.
//
// The order of the arguments is stable on purpose: it is what the command
// preview shows, what gets copied to the clipboard and what the tests assert
// on. Every option that the UI leaves at its default value is left out, so
// the preview does not grow a pile of "1,2", "-c 32" style noise.
class CommandBuilder
{
public:
    static QStringList build(const HashcatOptions &options);

    // Substitutes <unixtime> and <hash> in the outfile template.
    static QString expandOutfileTemplate(const QString &tmplate, const QString &hashFile);

    // Which attack modes consume which part of the UI. These are the same
    // predicates that decide the group box states in the main window, so the
    // preview and the widgets can never disagree.
    static bool attackUsesMask(AttackMode mode);
    static bool attackUsesWordlists(AttackMode mode);
    static bool attackUsesRules(AttackMode mode);
};

#endif // COMMANDBUILDER_H
