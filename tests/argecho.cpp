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

#include <cstdio>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QJsonArray arguments;
    const QStringList args = app.arguments();
    for (int i = 1; i < args.size(); ++i) {
        arguments.append(args.at(i));
    }

    const QByteArray json = QJsonDocument(arguments).toJson(QJsonDocument::Compact);
    std::fwrite(json.constData(), 1, static_cast<std::size_t>(json.size()), stdout);
    return 0;
}