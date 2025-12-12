/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: 2025 Rainer Größlinger
 */

#ifndef HELPERUTILS_H
#define HELPERUTILS_H

#include <QString>

class HelperUtils
{
public:
    HelperUtils();
    static QString executeHashcat(QStringList& args);
};

#endif // HELPERUTILS_H
