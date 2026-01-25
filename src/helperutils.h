/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef HELPERUTILS_H
#define HELPERUTILS_H

#include <QString>

class HelperUtils
{
public:
    HelperUtils();
    static QString executeHashcat(QStringList& args);
    static QMap<QString, QStringList> getAvailableTerminals();
};

#endif // HELPERUTILS_H
