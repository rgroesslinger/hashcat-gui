/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

// Test helper: writes its own argv (minus argv[0]) as a JSON array to stdout.
// The launch round-trip test starts it through a real terminal shell and
// compares what arrives with what launchCommand() encoded. Not a Qt Test
// binary, so it stays out of HASHCAT_GUI_TESTS.

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStringList>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QJsonArray arguments;
    const QStringList args = QCoreApplication::arguments();
    for (int i = 1; i < args.size(); ++i) {
        arguments.append(args.at(i));
    }

    QTextStream out(stdout);
    out << QJsonDocument(arguments).toJson(QJsonDocument::Compact);
    return 0;
}
