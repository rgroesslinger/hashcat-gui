/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfileexporter.h"
#include "potfileexportui.h"
#include "testenvironment.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTemporaryDir>
#include <QTest>

namespace
{

PotfileExporter::Row sampleRow()
{
    return {QStringLiteral("hash,one"),
            QStringLiteral("plain\"text\nline"),
            QStringLiteral("alice"),
            0,
            false,
            true};
}

} // namespace

class TestPotfileExporter : public QObject
{
    Q_OBJECT

private slots:
    void csvEscapesFieldsAndUsesTheExpectedColumns();
    void stepThreeCsvIncludesDuplicateAndLine();
    void txtUsesTabsAndQuotesTabbedFields();
    void jsonUsesObjectsAndOptionalFields();
    void writeCreatesTheFile();
    void writeReportsAnOpenFailure();
    void protectedDestinationIsDetected();
};

void TestPotfileExporter::csvEscapesFieldsAndUsesTheExpectedColumns()
{
    const QList<PotfileExporter::Row> rows{sampleRow()};
    const QByteArray data =
        PotfileExporter::serialize(rows, PotfileExporter::Format::Csv, true, false);

    QCOMPARE(QString::fromUtf8(data), QStringLiteral("hash,plaintext,username,encrypted\n"
                                                     "\"hash,one\",\"plain\"\"text\n"
                                                     "line\",alice,true\n"));
}

void TestPotfileExporter::stepThreeCsvIncludesDuplicateAndLine()
{
    const QList<PotfileExporter::Row> rows{
        {QStringLiteral("hash"), QStringLiteral("plain"), {}, 7, true, false},
    };
    const QByteArray data =
        PotfileExporter::serialize(rows, PotfileExporter::Format::Csv, false, true);

    QCOMPARE(QString::fromUtf8(data), QStringLiteral("hash,plaintext,duplicate,line,encrypted\n"
                                                     "hash,plain,true,7,false\n"));
}

void TestPotfileExporter::txtUsesTabsAndQuotesTabbedFields()
{
    const QList<PotfileExporter::Row> rows{
        {QStringLiteral("hash"), QStringLiteral("plain\ttext"), {}, 0, false, false},
    };
    const QByteArray data =
        PotfileExporter::serialize(rows, PotfileExporter::Format::Txt, false, false);

    QCOMPARE(QString::fromUtf8(data), QStringLiteral("hash\tplaintext\tencrypted\n"
                                                     "hash\t\"plain\ttext\"\tfalse\n"));
}

void TestPotfileExporter::jsonUsesObjectsAndOptionalFields()
{
    const QList<PotfileExporter::Row> rows{sampleRow()};
    const QByteArray data =
        PotfileExporter::serialize(rows, PotfileExporter::Format::Json, true, true);

    const QJsonDocument document = QJsonDocument::fromJson(data);
    QVERIFY(document.isArray());
    const QJsonArray array = document.array();
    QCOMPARE(array.size(), 1);
    const QJsonObject object = array.at(0).toObject();
    QCOMPARE(object.value(QStringLiteral("hash")).toString(), QStringLiteral("hash,one"));
    QCOMPARE(object.value(QStringLiteral("plaintext")).toString(),
             QStringLiteral("plain\"text\nline"));
    QCOMPARE(object.value(QStringLiteral("username")).toString(), QStringLiteral("alice"));
    QCOMPARE(object.value(QStringLiteral("duplicate")).toBool(), false);
    QCOMPARE(object.value(QStringLiteral("line")).toInt(), 0);
    QCOMPARE(object.value(QStringLiteral("encrypted")).toBool(), true);
}

void TestPotfileExporter::writeCreatesTheFile()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("results.csv"));

    QString error;
    QVERIFY(PotfileExporter::write(path, {sampleRow()}, PotfileExporter::Format::Csv, true, false,
                                   &error));
    QVERIFY(error.isEmpty());

    QFile file(path);
    QVERIFY(file.open(QIODevice::ReadOnly));
    const QByteArray content = file.readAll();
    QVERIFY(content.startsWith("hash,plaintext,username,encrypted\n"));
    QVERIFY(content.contains("alice"));
#ifndef Q_OS_WIN
    const QFileDevice::Permissions permissions = QFile::permissions(path);
    QVERIFY(permissions.testFlag(QFileDevice::ReadOwner));
    QVERIFY(permissions.testFlag(QFileDevice::WriteOwner));
    QVERIFY(!permissions.testFlag(QFileDevice::ReadGroup));
    QVERIFY(!permissions.testFlag(QFileDevice::ReadOther));
#endif
}

void TestPotfileExporter::protectedDestinationIsDetected()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashcat.potfile"));
    QFile file(path);
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.close();

    QVERIFY(PotfileExportUi::isSamePath(path, path));
    QVERIFY(PotfileExportUi::isSamePath(dir.filePath(QStringLiteral("./hashcat.potfile")), path));
    QVERIFY(!PotfileExportUi::isSamePath(dir.filePath(QStringLiteral("other.csv")), path));
    QVERIFY(!PotfileExportUi::isSamePath(path, QString()));
}

void TestPotfileExporter::writeReportsAnOpenFailure()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    QString error;
    QVERIFY(!PotfileExporter::write(dir.path(), {sampleRow()}, PotfileExporter::Format::Json, false,
                                    false, &error));
    QVERIFY(!error.isEmpty());
}

int main(int argc, char *argv[])
{
    TestPotfileExporter test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_potfileexporter.moc"