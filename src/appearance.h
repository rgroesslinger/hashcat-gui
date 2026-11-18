/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef APPEARANCE_H
#define APPEARANCE_H

#include <QString>

// The color scheme the user chose, applied to the whole application.
class Appearance
{
public:
    // An empty scheme follows the system; the other two force light or dark.
    static void applyColorScheme(const QString &scheme);
};

#endif // APPEARANCE_H