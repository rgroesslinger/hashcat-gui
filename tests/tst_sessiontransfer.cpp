/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appconstants.h"
#include "sessiontransfer.h"
#include "statefile.h"
#include "testenvironment.h"

#include <QCheckBox>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QLineEdit>
#include <QListWidget>
#include <QStandardPaths>
#include <QWidget>

// The document statuses, the transfer onto the widgets, the note order and
// the value immutability - on a plain widget tree, no MainWindow needed.
class TestSessionTransfer : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void documentStatuses_data();
    void documentStatuses();
    void theTransferAppliesTheValues();
    void ignoredNamesAreNotApplied();
    void theNoteOrderIsPinned();
    void theValuesAreNotMutated();
    void toDocumentSkipsThePreview();
    void theNoteTextsArePinned();

private:
    // A root with one widget of each kind the transfer touches.
    struct Root {
        QWidget widget;
        QLineEdit *edit;
        QCheckBox *check;
        QListWidget *list;

        Root()
            : edit(new QLineEdit(&widget)), check(new QCheckBox(&widget)),
              list(new QListWidget(&widget))
        {
            edit->setObjectName(QStringLiteral("lineEdit_a"));
            check->setObjectName(QStringLiteral("checkBox_b"));
            list->setObjectName(QStringLiteral("listWidget_c"));
        }
    };

    [[nodiscard]] QString writeSession(int version) const
    {
        QJsonObject values;
        values.insert(QStringLiteral("lineEdit_a"), QStringLiteral("x"));
        const QString file = QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))
                                 .filePath(QStringLiteral("transfer-test.json"));
        QString error;
        if (!StateFile::write(file, AppConstants::FileSections::Session, version, values, &error)) {
            qFatal("could not write the test session: %s", qPrintable(error));
        }
        return file;
    }
};

void TestSessionTransfer::initTestCase()
{
    QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
}

void TestSessionTransfer::documentStatuses_data()
{
    QTest::addColumn<QString>("kind");
    QTest::addColumn<int>("status");
    QTest::addColumn<int>("version");

    using Status = StateFile::Status;
    QTest::newRow("current") << QStringLiteral("current") << int(Status::Read)
                             << AppConstants::Sessions::Version;
    QTest::newRow("older") << QStringLiteral("older") << int(Status::Older)
                           << AppConstants::Sessions::Version - 1;
    QTest::newRow("newer") << QStringLiteral("newer") << int(Status::Newer)
                           << AppConstants::Sessions::Version + 1;
    QTest::newRow("no version") << QStringLiteral("noVersion") << int(Status::Read) << -1;
    QTest::newRow("not JSON") << QStringLiteral("notJson") << int(Status::Unreadable) << -1;
    QTest::newRow("another section")
        << QStringLiteral("otherSection") << int(Status::Unreadable) << -1;
}

void TestSessionTransfer::documentStatuses()
{
    QFETCH(QString, kind);
    QFETCH(int, status);
    QFETCH(int, version);

    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString file;
    if (kind == QStringLiteral("current") || kind == QStringLiteral("older")
        || kind == QStringLiteral("newer")) {
        file = writeSession(version);
    } else if (kind == QStringLiteral("noVersion")) {
        // A hand-edited file: values without a version.
        file = QDir(dir).filePath(QStringLiteral("no-version.json"));
        QFile out(file);
        QVERIFY(out.open(QIODevice::WriteOnly));
        out.write(R"({"sections":{"session":{"values":{"lineEdit_a":"x"}}}})");
        out.close();
    } else if (kind == QStringLiteral("notJson")) {
        file = QDir(dir).filePath(QStringLiteral("not-json.json"));
        QFile out(file);
        QVERIFY(out.open(QIODevice::WriteOnly));
        out.write("this is not JSON");
        out.close();
    } else {
        file = QDir(dir).filePath(QStringLiteral("other-section.json"));
        QString error;
        QVERIFY(StateFile::write(file, AppConstants::FileSections::Settings,
                                 AppConstants::Settings::Version, {}, &error));
    }

    const StateFile::Document document = StateFile::readDocument(
        file, AppConstants::FileSections::Session, AppConstants::Sessions::Version);
    QCOMPARE(int(document.status), status);
    QCOMPARE(document.version, version);
    if (status == int(StateFile::Status::Unreadable)) {
        QVERIFY(!document.error.isEmpty());
    }
}

void TestSessionTransfer::theTransferAppliesTheValues()
{
    Root root;

    QJsonObject row;
    row.insert(QStringLiteral("text"), QStringLiteral("/tmp/w.txt"));
    row.insert(QStringLiteral("checked"), true);
    QJsonArray list;
    list.append(row);

    StateFile::Document document;
    document.status = StateFile::Status::Read;
    document.version = AppConstants::Sessions::Version;
    document.values.insert(QStringLiteral("lineEdit_a"), QStringLiteral("applied"));
    document.values.insert(QStringLiteral("checkBox_b"), true);
    document.values.insert(QStringLiteral("listWidget_c"), list);

    const QStringList notes =
        SessionTransfer::applyToWidgets(document, &root.widget, {QStringLiteral("ignored")});
    QVERIFY2(notes.isEmpty(), qPrintable(notes.join(QStringLiteral("\n"))));
    QCOMPARE(root.edit->text(), QStringLiteral("applied"));
    QVERIFY(root.check->isChecked());
    QCOMPARE(root.list->count(), 1);
    QCOMPARE(root.list->item(0)->text(), QStringLiteral("/tmp/w.txt"));
    QCOMPARE(root.list->item(0)->checkState(), Qt::Checked);
}

void TestSessionTransfer::ignoredNamesAreNotApplied()
{
    Root root;
    root.edit->setText(QStringLiteral("before"));

    StateFile::Document document;
    document.status = StateFile::Status::Read;
    document.version = AppConstants::Sessions::Version;
    document.values.insert(QStringLiteral("lineEdit_a"), QStringLiteral("after"));

    // The ignored name is not applied - and not reported as unknown either.
    const QStringList notes =
        SessionTransfer::applyToWidgets(document, &root.widget, {QStringLiteral("lineEdit_a")});
    QVERIFY2(notes.isEmpty(), qPrintable(notes.join(QStringLiteral("\n"))));
    QCOMPARE(root.edit->text(), QStringLiteral("before"));
}

void TestSessionTransfer::theNoteOrderIsPinned()
{
    Root root;

    StateFile::Document document;
    document.status = StateFile::Status::Older;
    document.version = AppConstants::Sessions::Version - 1;
    document.otherSections = QStringList{QStringLiteral("settings")};
    document.values.insert(QStringLiteral("lineEdit_a"), 42); // wrong type
    document.values.insert(QStringLiteral("unknown_key"), QStringLiteral("x"));

    const QStringList notes =
        SessionTransfer::applyToWidgets(document, &root.widget, {QStringLiteral("ignored")});
    QCOMPARE(notes.size(), 4);
    QVERIFY(notes.at(0).contains(QStringLiteral("older version")));
    QVERIFY(notes.at(1).contains(QStringLiteral("sections this import does not use")));
    QVERIFY(notes.at(2).contains(QStringLiteral("unknown_key")));
    QVERIFY(notes.at(3).contains(QStringLiteral("lineEdit_a")));
}

void TestSessionTransfer::theValuesAreNotMutated()
{
    Root root;

    StateFile::Document document;
    document.status = StateFile::Status::Read;
    document.version = AppConstants::Sessions::Version;
    document.values.insert(QStringLiteral("lineEdit_a"), QStringLiteral("x"));
    document.values.insert(QStringLiteral("unknown_key"), QStringLiteral("y"));
    const QJsonObject before = document.values;

    SessionTransfer::applyToWidgets(document, &root.widget, {});
    QCOMPARE(document.values, before);
}

void TestSessionTransfer::toDocumentSkipsThePreview()
{
    Root root;
    auto *preview = new QLineEdit(&root.widget);
    preview->setObjectName(QString(AppConstants::Files::PreviewWidget));
    preview->setText(QStringLiteral("hashcat --attack-mode 0"));
    root.edit->setText(QStringLiteral("value"));

    const QJsonObject values = SessionTransfer::toDocument(&root.widget);
    QCOMPARE(values.value(QStringLiteral("lineEdit_a")).toString(), QStringLiteral("value"));
    QVERIFY(!values.contains(QString(AppConstants::Files::PreviewWidget)));
}

void TestSessionTransfer::theNoteTextsArePinned()
{
    const int current = AppConstants::Sessions::Version;

    const QStringList older = SessionTransfer::fileNotes(current - 1, current, {});
    QCOMPARE(
        older,
        QStringList{QStringLiteral(
                        "The file was made by an older version of hashcat-gui (format version %1; "
                        "this version reads format version %2).")
                        .arg(current - 1)
                        .arg(current)});

    // A file without a version gets no older-version note.
    QVERIFY(SessionTransfer::fileNotes(-1, current, {}).isEmpty());

    const QStringList sections =
        SessionTransfer::fileNotes(current, current, {QStringLiteral("settings")});
    QCOMPARE(sections, QStringList{QStringLiteral(
                           "The file holds sections this import does not use: settings.")});

    QCOMPARE(SessionTransfer::newerFileNote(current + 1, current),
             QStringLiteral("The file is in format version %1, but this version reads format "
                            "version %2, so it was not applied. Update hashcat-gui to read it.")
                 .arg(current + 1)
                 .arg(current));

    QCOMPARE(SessionTransfer::newerSessionNote(current + 1),
             QStringLiteral("The session file is in format version %1, but this version reads "
                            "format version %2, so the session was not restored. The file was "
                            "left as it is, so the session of this run is not stored either. "
                            "Update hashcat-gui to read it.")
                 .arg(current + 1)
                 .arg(current));
}

int main(int argc, char *argv[])
{
    TestSessionTransfer test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_sessiontransfer.moc"