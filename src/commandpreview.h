/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef COMMANDPREVIEW_H
#define COMMANDPREVIEW_H

#include <QString>
#include <QStringList>

// The command line as the preview and the clipboard show it. Display-only:
// the string never feeds the launch path, which quotes for the shell on its
// own (TerminalCatalog::launchCommand).
class CommandPreview
{
public:
    // The binary name (empty when none is configured) and one token per
    // argument, joined with spaces. Every token the platform's shell would
    // split or expand - spaces, quotes, masks, recipes - is quoted for
    // display, so a copied command can be pasted into a shell. The argv
    // handed to hashcat keeps the tokens unquoted.
    static QString format(const QString &binary, const QStringList &arguments);

    // The tooltip of the command preview: the whole command, because the
    // line edit shows only its beginning. Plain text, not rich text: a rich
    // text tooltip is squeezed into whatever width its text control happens
    // to have and grows tall instead, while a plain text one keeps the line
    // breaks it is given. The breaks go between the arguments at 120
    // columns, so a flag stays with the value that follows it; markup-like
    // text is escaped.
    static QString tooltip(const QString &command);
};

#endif // COMMANDPREVIEW_H