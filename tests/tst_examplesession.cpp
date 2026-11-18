/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appconstants.h"
#include "examplesession.h"
#include "statefile.h"
#include "testenvironment.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QStandardPaths>
#include <utility>

namespace
{
void collectPaths(const QJsonValue &value, QStringList *paths)
{
    if (value.isString()) {
        const QString text = value.toString();
        if (text.contains(QString(Examples::DataPrefix))) {
            paths->append(text);
        }
        return;
    }
    if (value.isObject()) {
        const QJsonObject object = value.toObject();
        for (auto it = object.constBegin(); it != object.constEnd(); ++it) {
            collectPaths(it.value(), paths);
        }
        return;
    }
    if (value.isArray()) {
        const QJsonArray array = value.toArray();
        for (const auto &entry : array) {
            collectPaths(entry, paths);
        }
    }
}
} // namespace

// The bundled examples: finding one, the <examples>/ token, and unpacking
// the data files hashcat can read.
class TestExampleSession : public QObject
{
    Q_OBJECT

private slots:
    void everyModeHasOneExample();
    void thePrefixIsFoundAtAnyDepth();
    void thePathsAreReplacedAtAnyDepth();
    void materializeUnpacksEveryDataFile();
    void everyExampleNamesOnlyBundledData();
};

void TestExampleSession::everyModeHasOneExample()
{
    for (const AttackMode mode : AllAttackModes) {
        const Examples::Entry *entry = ExampleSession::find(mode);
        QVERIFY2(entry != nullptr, "an attack mode has no example");
        QCOMPARE(entry->attackMode, mode);
    }
    QVERIFY(ExampleSession::find(static_cast<AttackMode>(999)) == nullptr);
}

void TestExampleSession::thePrefixIsFoundAtAnyDepth()
{
    const QString prefix = QString(Examples::DataPrefix);
    QVERIFY(ExampleSession::containsExamplePrefix(QJsonValue(prefix + QStringLiteral("w.txt"))));
    QVERIFY(!ExampleSession::containsExamplePrefix(QJsonValue(QStringLiteral("/tmp/w.txt"))));

    QJsonObject row;
    row.insert(QStringLiteral("text"), prefix + QStringLiteral("w.txt"));
    QJsonArray list;
    list.append(row);
    QJsonObject values;
    values.insert(QStringLiteral("listWidget_wordlist"), list);
    QVERIFY(ExampleSession::containsExamplePrefix(QJsonValue(values)));

    QVERIFY(!ExampleSession::containsExamplePrefix(QJsonValue(42)));
    QVERIFY(!ExampleSession::containsExamplePrefix(QJsonValue(true)));
}

void TestExampleSession::thePathsAreReplacedAtAnyDepth()
{
    const QString prefix = QString(Examples::DataPrefix);
    const QString directory = QStringLiteral("/tmp/examples/");

    QJsonObject row;
    row.insert(QStringLiteral("text"), prefix + QStringLiteral("w.txt"));
    row.insert(QStringLiteral("checked"), true);
    QJsonArray list;
    list.append(row);
    QJsonObject values;
    values.insert(QStringLiteral("listWidget_wordlist"), list);
    values.insert(QStringLiteral("lineEdit_hashfile"), prefix + QStringLiteral("h.txt"));
    values.insert(QStringLiteral("spinBox_runtime"), 60);

    const QJsonObject replaced =
        ExampleSession::withExamplePaths(QJsonValue(values), directory).toObject();
    QCOMPARE(replaced.value(QStringLiteral("lineEdit_hashfile")).toString(),
             directory + QStringLiteral("h.txt"));
    const QJsonObject replacedRow =
        replaced.value(QStringLiteral("listWidget_wordlist")).toArray().at(0).toObject();
    QCOMPARE(replacedRow.value(QStringLiteral("text")).toString(),
             directory + QStringLiteral("w.txt"));
    QCOMPARE(replacedRow.value(QStringLiteral("checked")).toBool(), true);
    QCOMPARE(replaced.value(QStringLiteral("spinBox_runtime")).toInt(), 60);
}

void TestExampleSession::materializeUnpacksEveryDataFile()
{
    QString directory;
    QString error;
    QVERIFY2(ExampleSession::materialize(&directory, &error), qPrintable(error));
    QVERIFY(directory.endsWith(QDir::separator()));
    QVERIFY(QDir(directory).exists());

    // Every bundled file is there, with the resource's content.
    for (const QLatin1StringView &resource : Examples::DataFiles) {
        QFile source{QString(resource)};
        QVERIFY(source.open(QIODevice::ReadOnly));
        QFile unpacked(QDir(directory).filePath(QFileInfo(QString(resource)).fileName()));
        QVERIFY2(unpacked.open(QIODevice::ReadOnly), qPrintable(unpacked.fileName()));
        QCOMPARE(unpacked.readAll(), source.readAll());
    }

    // A second load replaces the copy instead of failing over it.
    QString again;
    QVERIFY2(ExampleSession::materialize(&again, &error), qPrintable(error));
    QCOMPARE(again, directory);
}

// The examples are compiled into this build: every data path they name has
// to be one of the bundled files, so an example can never depend on a file
// the unpacking does not write.
void TestExampleSession::everyExampleNamesOnlyBundledData()
{
    QStringList bundled;
    for (const QLatin1StringView &resource : Examples::DataFiles) {
        bundled << QFileInfo(QString(resource)).fileName();
    }

    for (const Examples::Entry &entry : Examples::All) {
        QString error;
        QJsonObject values;
        int version = 0;
        QStringList otherSections;
        QVERIFY2(StateFile::read(QString(entry.resource), AppConstants::FileSections::Session,
                                 &values, &version, &otherSections, &error),
                 qPrintable(error));

        QStringList paths;
        collectPaths(QJsonValue(values), &paths);
        for (const QString &path : std::as_const(paths)) {
            const QString name = path.mid(QString(Examples::DataPrefix).size());
            QVERIFY2(bundled.contains(name),
                     qPrintable(QStringLiteral("%1 names unbundled data: %2")
                                    .arg(QString(entry.resource), path)));
        }
    }
}

int main(int argc, char *argv[])
{
    TestExampleSession test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_examplesession.moc"