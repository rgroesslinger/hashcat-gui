/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfileexporter.h"
#include "potfileexportui.h"
#include "testenvironment.h"

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QTemporaryDir>
#include <QTest>
#include <QTimer>

namespace
{

PotfileExporter::Row sampleRow()
{
    return {QStringLiteral("hash,one"),
            QStringLiteral("plain\"text\nline"),
            QStringLiteral("alice"),
            0,
            false,
            true,
            true};
}

// Drives the export modal flow: picks the file and filter, answers the
// Yes/No questions and the plaintext confirmation, records the boxes.
struct ExportDrive {
    QString pickPath;
    QString pickFilter;
    bool confirmQuestion = false;
    QString warningText;
    QString questionText;
    QString confirmationText;
    QString initialPath;
    Qt::TextFormat questionFormat = Qt::AutoText;
    Qt::TextFormat warningFormat = Qt::AutoText;
};

bool driveExportUi(const QString &suggested, const QStringList &protectedPaths, ExportDrive &drive)
{
    QTimer timer;
    timer.setInterval(20);
    int pickAttempts = 0;
    bool filterSet = false;
    QObject::connect(&timer, &QTimer::timeout, &timer, [&] {
        QWidget *modal = QApplication::activeModalWidget();
        if (!modal) {
            return;
        }
        if (auto *fileDialog = qobject_cast<QFileDialog *>(modal)) {
            if (drive.initialPath.isEmpty()) {
                drive.initialPath = fileDialog->selectedFiles().value(0);
            }
            if (QWidget *focused = fileDialog->focusWidget()) {
                focused->clearFocus();
            }
            // Re-applying the filter restores the dialog's remembered file
            // and undoes selectFile(); set it once.
            if (!drive.pickFilter.isEmpty() && !filterSet) {
                fileDialog->selectNameFilter(drive.pickFilter);
                filterSet = true;
            }
            // selectFile loses to the dialog's own focus restoration for a
            // tick or two, and the dialog may append the filter's suffix.
            const QStringList selection = fileDialog->selectedFiles();
            bool matches = selection.contains(drive.pickPath);
            if (!matches) {
                for (const QString &selected : selection) {
                    if (selected.startsWith(drive.pickPath + QLatin1Char('.'))) {
                        matches = true;
                        break;
                    }
                }
            }
            if (!matches && ++pickAttempts < 100) {
                fileDialog->selectFile(drive.pickPath);
                return;
            }
            static_cast<QDialog *>(fileDialog)->accept();
            return;
        }
        if (auto *box = qobject_cast<QMessageBox *>(modal)) {
            if (box->button(QMessageBox::Yes) != nullptr) {
                drive.questionText = box->text();
                drive.questionFormat = box->textFormat();
                box->button(drive.confirmQuestion ? QMessageBox::Yes : QMessageBox::No)->click();
                return;
            }
            if (box->button(QMessageBox::Cancel) != nullptr) {
                drive.confirmationText = box->text();
                const auto buttons = box->buttons();
                for (QAbstractButton *b : buttons) {
                    if (box->buttonRole(b) == QMessageBox::AcceptRole) {
                        b->click();
                        return;
                    }
                }
            }
            drive.warningText = box->text();
            drive.warningFormat = box->textFormat();
            box->close();
        }
    });
    timer.start();
    const bool ok = PotfileExportUi::exportRows(nullptr, suggested, {sampleRow()}, true, true, 1,
                                                protectedPaths);
    timer.stop();
    return ok;
}

} // namespace

class TestPotfileExporter : public QObject
{
    Q_OBJECT

private slots:
    void csvEscapesFieldsAndUsesTheExpectedColumns();
    void stepThreeCsvIncludesDuplicateAndLine();
    void tsvUsesTabsAndQuotesTabbedFields();
    void jsonUsesObjectsAndOptionalFields();
    void formulaLeadingCellsAreNeutralized();
    void rowsWithoutLineInfoExportEmptyCells();
    void writeCreatesTheFile();
    void writeReportsAnOpenFailure();
    void protectedDestinationIsDetected();
    void exportToAProtectedCandidateIsRefused();
    void unrelatedHashcatPotfileAsksAndDefaultsToNo();
    void filterAndSuffixCombinations();
    void markupLikeFileNamesStayLiteral();
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
        {QStringLiteral("hash"), QStringLiteral("plain"), {}, 7, true, false, true},
    };
    const QByteArray data =
        PotfileExporter::serialize(rows, PotfileExporter::Format::Csv, false, true);

    QCOMPARE(QString::fromUtf8(data), QStringLiteral("hash,plaintext,duplicate,line,encrypted\n"
                                                     "hash,plain,true,7,false\n"));
}

void TestPotfileExporter::tsvUsesTabsAndQuotesTabbedFields()
{
    const QList<PotfileExporter::Row> rows{
        {QStringLiteral("hash"), QStringLiteral("plain\ttext"), {}, 0, false, false},
    };
    const QByteArray data =
        PotfileExporter::serialize(rows, PotfileExporter::Format::Tsv, false, false);

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

// A spreadsheet evaluates a cell that starts with = + - or @ as a formula,
// so the tabular exports prefix an apostrophe. A leading apostrophe of the
// value itself is kept as it is, and JSON stays the literal value.
void TestPotfileExporter::formulaLeadingCellsAreNeutralized()
{
    const QList<PotfileExporter::Row> rows{
        {QStringLiteral("hash"), QStringLiteral("=1+1"), QStringLiteral("+user"), 0, false, false,
         true},
        {QStringLiteral("hash"), QStringLiteral("-cmd"), QStringLiteral("@name"), 0, false, false,
         true},
        {QStringLiteral("hash"), QStringLiteral("'quoted"), QStringLiteral("plain"), 0, false,
         false, true},
        {QStringLiteral("hash"), QStringLiteral("safe"), QStringLiteral("plain"), 0, false, false,
         true},
    };

    const QByteArray csv =
        PotfileExporter::serialize(rows, PotfileExporter::Format::Csv, true, false);
    QCOMPARE(QString::fromUtf8(csv), QStringLiteral("hash,plaintext,username,encrypted\n"
                                                    "hash,'=1+1,'+user,false\n"
                                                    "hash,'-cmd,'@name,false\n"
                                                    "hash,'quoted,plain,false\n"
                                                    "hash,safe,plain,false\n"));

    const QByteArray tsv =
        PotfileExporter::serialize(rows, PotfileExporter::Format::Tsv, true, false);
    QVERIFY(QString::fromUtf8(tsv).contains(QStringLiteral("\t'=1+1\t")));

    const QByteArray json =
        PotfileExporter::serialize(rows, PotfileExporter::Format::Json, true, false);
    const QJsonObject object = QJsonDocument::fromJson(json).array().at(0).toObject();
    QCOMPARE(object.value(QStringLiteral("plaintext")).toString(), QStringLiteral("=1+1"));
}

// --show results carry no line information: the columns stay, the cells empty.
void TestPotfileExporter::rowsWithoutLineInfoExportEmptyCells()
{
    const QList<PotfileExporter::Row> rows{
        {QStringLiteral("hash"), QStringLiteral("plain"), {}, 7, true, false, false},
    };

    const QByteArray csv =
        PotfileExporter::serialize(rows, PotfileExporter::Format::Csv, true, true);
    QCOMPARE(QString::fromUtf8(csv),
             QStringLiteral("hash,plaintext,username,duplicate,line,encrypted\n"
                            "hash,plain,,,,false\n"));

    const QByteArray json =
        PotfileExporter::serialize(rows, PotfileExporter::Format::Json, true, true);
    const QJsonObject object = QJsonDocument::fromJson(json).array().at(0).toObject();
    QVERIFY(object.value(QStringLiteral("duplicate")).isNull());
    QVERIFY(object.value(QStringLiteral("line")).isNull());
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

void TestPotfileExporter::exportToAProtectedCandidateIsRefused()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashcat.potfile"));

    ExportDrive drive;
    drive.pickPath = path;
    QVERIFY(!driveExportUi(path, {path}, drive));
    QVERIFY(drive.warningText.contains(QStringLiteral("other than the potfile")));
    QVERIFY(!QFile::exists(path));
}

void TestPotfileExporter::unrelatedHashcatPotfileAsksAndDefaultsToNo()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("hashcat.potfile"));

    // No protected path matches: the name alone triggers the question, and
    // closing it (the default No) refuses the export.
    ExportDrive drive;
    drive.pickPath = path;
    QVERIFY(!driveExportUi(path, {}, drive));
    QVERIFY(drive.questionText.contains(QStringLiteral("hashcat.potfile")));
    QVERIFY(!QFile::exists(path));

    drive.confirmQuestion = true;
    QVERIFY(driveExportUi(path, {}, drive));
    // The unknown suffix gains the filter's suffix; the potfile itself stays.
    QVERIFY(QFile::exists(path + QStringLiteral(".csv")));
    QVERIFY(!QFile::exists(path));
}

void TestPotfileExporter::filterAndSuffixCombinations()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    const auto content = [](const QString &path) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            return QByteArray();
        }
        return file.readAll();
    };

    // No suffix: the filter's suffix is appended.
    ExportDrive drive;
    drive.pickPath = dir.filePath(QStringLiteral("results"));
    drive.pickFilter = QStringLiteral("CSV (*.csv)");
    QVERIFY(driveExportUi(dir.filePath(QStringLiteral("results")), {}, drive));
    QVERIFY(QFile::exists(dir.filePath(QStringLiteral("results.csv"))));
    QVERIFY(content(dir.filePath(QStringLiteral("results.csv"))).startsWith("hash,plaintext"));
    QVERIFY(drive.confirmationText.contains(QStringLiteral("as CSV")));
    QVERIFY(drive.confirmationText.contains(QStringLiteral("results.csv")));

    // A suffix naming another format: the filter wins, its suffix is appended.
    ExportDrive cross;
    cross.pickPath = dir.filePath(QStringLiteral("results.txt"));
    cross.pickFilter = QStringLiteral("CSV (*.csv)");
    QVERIFY(driveExportUi(dir.filePath(QStringLiteral("results.txt")), {}, cross));
    QVERIFY(QFile::exists(dir.filePath(QStringLiteral("results.txt.csv"))));

    // .txt aliases TSV: the name stays.
    ExportDrive tsv;
    tsv.pickPath = dir.filePath(QStringLiteral("results.txt"));
    tsv.pickFilter = QStringLiteral("TSV (*.tsv *.txt)");
    QVERIFY(driveExportUi(dir.filePath(QStringLiteral("results.txt")), {}, tsv));
    QVERIFY(content(dir.filePath(QStringLiteral("results.txt"))).startsWith("hash\tplaintext"));
    QVERIFY(tsv.confirmationText.contains(QStringLiteral("as TSV")));

    // JSON over a .csv name.
    ExportDrive json;
    json.pickPath = dir.filePath(QStringLiteral("data.csv"));
    json.pickFilter = QStringLiteral("JSON (*.json)");
    QVERIFY(driveExportUi(dir.filePath(QStringLiteral("data.csv")), {}, json));
    QVERIFY(QFile::exists(dir.filePath(QStringLiteral("data.csv.json"))));
    QVERIFY(content(dir.filePath(QStringLiteral("data.csv.json"))).startsWith("["));
}

// Plain-text boxes keep markup-like file names literal. Windows uses '&amp;'
// in place of illegal '<br>'.
void TestPotfileExporter::markupLikeFileNamesStayLiteral()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
#ifdef Q_OS_WIN
    const QString name = QStringLiteral("my &amp;notes");
#else
    const QString name = QStringLiteral("my <br>notes");
#endif
    const QString path = dir.filePath(name + QStringLiteral(".csv"));

    ExportDrive drive;
    drive.pickPath = path;
    QVERIFY(driveExportUi(path, {}, drive));
    QVERIFY(QFile::exists(path));
    QCOMPARE(drive.warningFormat, Qt::PlainText);
    QVERIFY(drive.warningText.contains(name + QStringLiteral(".csv")));

    // The overwrite question covers the suffix-appended name; the default No
    // refuses, a Yes exports and the completion names the file literally too.
    ExportDrive again;
    again.pickPath = dir.filePath(name);
    QVERIFY(!driveExportUi(dir.filePath(name), {}, again));
    QCOMPARE(again.questionFormat, Qt::PlainText);
    QVERIFY(again.questionText.contains(name + QStringLiteral(".csv")));

    again.confirmQuestion = true;
    QVERIFY(driveExportUi(dir.filePath(name), {}, again));
    QCOMPARE(again.warningFormat, Qt::PlainText);
    QVERIFY(again.warningText.contains(name + QStringLiteral(".csv")));
}

int main(int argc, char *argv[])
{
    TestPotfileExporter test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_potfileexporter.moc"