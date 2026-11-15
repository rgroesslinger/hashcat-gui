/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef OPTIONCOLLECTOR_H
#define OPTIONCOLLECTOR_H

#include "hashcatoptions.h"
#include "ui_mainwindow.h"

// Reads the current widget state into the UI-independent description of a
// hashcat invocation. Everything the command line is built from goes through
// here, so CommandBuilder never has to know about widgets. The short/long
// parameter choice is a parameter, read from the settings by the caller.
class OptionCollector
{
public:
    static HashcatOptions collect(const Ui::MainWindow &ui, bool useShortParameters);
};

#endif // OPTIONCOLLECTOR_H