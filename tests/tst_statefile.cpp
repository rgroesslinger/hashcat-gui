/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "appconstants.h"
#include "statefile.h"

// The envelope of the files the application writes: the session it keeps
// between two runs and the settings the user exports share it, so the class
// that owns it has to write it, read it and refuse a file it cannot use.
class TestStateFile : public QObject
{
    Q_OBJECT

private slots:
    void aSectionSurvivesTheRoundTrip();
    void theWrittenFileHasTheExpectedShape();
    void anotherSectionIsRefused();
    void aFileWithTwoSectionsReportsTheOther();
    void aFileWithoutSectionsIsRefused();
    void aSectionWithoutValuesIsRefused();
    void aCorruptFileIsRefused();
    void aMissingFileIsRefused();
    void aWriteThatFailsIsReported();
    void theErrorIsClearedOnSuccess();
    void nameListFormatsTheNames();
    void nameListCountsTheNamesItLeavesOut();
    void theUnknownValuesNoteNamesTheValues();
};

namespace
{
// A file in the scratch directory of the test.
QString scratchFile(const QString &name)
{
    return TestEnvironment::configHome().filePath(name);
}

// Writes a file with the given content.
bool writeFile(const QString &path, const QByteArray &content)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }
    file.write(content);
    file.close();
    return true;
}
} // namespace

// What goes in comes out again: the values of a section, and the version
// they were written with.
void TestStateFile::aSectionSurvivesTheRoundTrip()
{
    const QString path = scratchFile(QStringLiteral("roundtrip.json"));

    QJsonObject values;
    values.insert(QStringLiteral("lineEdit_hashfile"), QStringLiteral("/tmp/hashes.txt"));
    values.insert(QStringLiteral("checkBox_outfile"), true);

    QString errorMessage;
    QVERIFY2(StateFile::write(path, AppConstants::FileSections::Session, 1, values, &errorMessage),
             qPrintable(errorMessage));
    QVERIFY(errorMessage.isEmpty());

    QJsonObject read;
    int version = 0;
    QStringList otherSections;
    QVERIFY2(StateFile::read(path, AppConstants::FileSections::Session, &read, &version,
                             &otherSections, &errorMessage),
             qPrintable(errorMessage));
    QCOMPARE(read, values);
    QCOMPARE(version, 1);
    QVERIFY(otherSections.isEmpty());
}

// The shape of the file is what the other programs and the tests read, so it
// is pinned here.
void TestStateFile::theWrittenFileHasTheExpectedShape()
{
    const QString path = scratchFile(QStringLiteral("shape.json"));

    QJsonObject values;
    values.insert(QStringLiteral("lineEdit_hashfile"), QStringLiteral("/tmp/hashes.txt"));

    QVERIFY(StateFile::write(path, AppConstants::FileSections::Session, 1, values, nullptr));

    QFile file(path);
    QVERIFY(file.open(QIODevice::ReadOnly));
    const QJsonObject root = QJsonDocument::fromJson(file.readAll()).object();
    file.close();

    // The release that wrote the file is named, but it is only information.
    QVERIFY(root.value(QStringLiteral("createdWith")).isString());

    const QJsonObject sections = root.value(QStringLiteral("sections")).toObject();
    QCOMPARE(sections.keys(), QStringList{QString(AppConstants::FileSections::Session)});

    const QJsonObject section =
        sections.value(QString(AppConstants::FileSections::Session)).toObject();
    QCOMPARE(section.value(QStringLiteral("version")).toInt(), 1);
    QCOMPARE(section.value(QStringLiteral("values")).toObject(), values);
}

// A file that holds something else is not a file of this kind. The message
// says what it does hold, so the user knows what they picked.
void TestStateFile::anotherSectionIsRefused()
{
    const QString path = scratchFile(QStringLiteral("settings_only.json"));

    QJsonObject values;
    values.insert(QStringLiteral("hashcatPath"), QStringLiteral("/usr/bin/hashcat"));
    QVERIFY(StateFile::write(path, AppConstants::FileSections::Settings, 1, values, nullptr));

    QString errorMessage;
    QJsonObject read;
    int version = 0;
    QStringList otherSections;
    QVERIFY(!StateFile::read(path, AppConstants::FileSections::Session, &read, &version,
                             &otherSections, &errorMessage));
    QVERIFY(errorMessage.contains(QString(AppConstants::FileSections::Session)));
    QVERIFY(errorMessage.contains(QString(AppConstants::FileSections::Settings)));
    QCOMPARE(otherSections, QStringList{QString(AppConstants::FileSections::Settings)});
    QVERIFY(read.isEmpty());
}

// A file that holds both is read for the section that is wanted; the other
// one is reported, so that the user knows it was not applied.
void TestStateFile::aFileWithTwoSectionsReportsTheOther()
{
    const QString path = scratchFile(QStringLiteral("both.json"));

    QJsonObject session;
    session.insert(QStringLiteral("version"), 1);
    session.insert(QStringLiteral("values"), QJsonObject{{QStringLiteral("lineEdit_hashfile"),
                                                          QStringLiteral("/tmp/hashes.txt")}});
    QJsonObject settings;
    settings.insert(QStringLiteral("version"), 1);
    settings.insert(QStringLiteral("values"), QJsonObject{{QStringLiteral("hashcatPath"),
                                                           QStringLiteral("/usr/bin/hashcat")}});

    QJsonObject root;
    root.insert(QStringLiteral("sections"),
                QJsonObject{{QString(AppConstants::FileSections::Session), session},
                            {QString(AppConstants::FileSections::Settings), settings}});
    QVERIFY(writeFile(path, QJsonDocument(root).toJson()));

    QString errorMessage;
    QJsonObject read;
    int version = 0;
    QStringList otherSections;
    QVERIFY(StateFile::read(path, AppConstants::FileSections::Session, &read, &version,
                            &otherSections, &errorMessage));
    QVERIFY(errorMessage.isEmpty());
    QCOMPARE(otherSections, QStringList{QString(AppConstants::FileSections::Settings)});
    QCOMPARE(read.value(QStringLiteral("lineEdit_hashfile")).toString(),
             QStringLiteral("/tmp/hashes.txt"));
}

// A file without sections is not one of ours.
void TestStateFile::aFileWithoutSectionsIsRefused()
{
    const QString path = scratchFile(QStringLiteral("no_sections.json"));
    QVERIFY(writeFile(path, R"({"version": 1, "MainWindow": {}})"));

    QString errorMessage;
    QVERIFY(!StateFile::read(path, AppConstants::FileSections::Session, nullptr, nullptr, nullptr,
                             &errorMessage));
    QVERIFY(!errorMessage.isEmpty());
}

// A section without values cannot be applied either.
void TestStateFile::aSectionWithoutValuesIsRefused()
{
    const QString path = scratchFile(QStringLiteral("no_values.json"));

    QJsonObject root;
    root.insert(QStringLiteral("sections"),
                QJsonObject{{QString(AppConstants::FileSections::Session),
                             QJsonObject{{QStringLiteral("version"), 1}}}});
    QVERIFY(writeFile(path, QJsonDocument(root).toJson()));

    QString errorMessage;
    QVERIFY(!StateFile::read(path, AppConstants::FileSections::Session, nullptr, nullptr, nullptr,
                             &errorMessage));
    QVERIFY(errorMessage.contains(QString(AppConstants::FileSections::Session)));
}

// A file that is not JSON at all.
void TestStateFile::aCorruptFileIsRefused()
{
    const QString path = scratchFile(QStringLiteral("corrupt.json"));
    QVERIFY(writeFile(path, "this is not json"));

    QString errorMessage;
    QVERIFY(!StateFile::read(path, AppConstants::FileSections::Session, nullptr, nullptr, nullptr,
                             &errorMessage));
    QVERIFY(!errorMessage.isEmpty());
}

// A file that is not there.
void TestStateFile::aMissingFileIsRefused()
{
    QString errorMessage;
    QVERIFY(!StateFile::read(scratchFile(QStringLiteral("missing.json")),
                             AppConstants::FileSections::Session, nullptr, nullptr, nullptr,
                             &errorMessage));
    QVERIFY(!errorMessage.isEmpty());
}

// A write that cannot succeed is reported, and a nullptr errorMessage is the
// documented default and must be just as valid on the failing path.
void TestStateFile::aWriteThatFailsIsReported()
{
    QString errorMessage;
    QVERIFY(!StateFile::write(QStringLiteral("/this/path/does/not/exist/state.json"),
                              AppConstants::FileSections::Session, 1, QJsonObject(),
                              &errorMessage));
    QVERIFY(!errorMessage.isEmpty());

    QVERIFY(!StateFile::write(QStringLiteral("/this/path/does/not/exist/state.json"),
                              AppConstants::FileSections::Session, 1, QJsonObject(), nullptr));
}

// The error of an earlier failure must not show up as the error of a
// successful read.
void TestStateFile::theErrorIsClearedOnSuccess()
{
    const QString path = scratchFile(QStringLiteral("cleared.json"));
    QVERIFY(StateFile::write(path, AppConstants::FileSections::Session, 1, QJsonObject(), nullptr));

    QString errorMessage = QStringLiteral("left over from an earlier failure");
    QVERIFY(StateFile::read(path, AppConstants::FileSections::Session, nullptr, nullptr, nullptr,
                            &errorMessage));
    QVERIFY(errorMessage.isEmpty());
}

// The list of names is one sentence, whatever the number of names.
void TestStateFile::nameListFormatsTheNames()
{
    QVERIFY(StateFile::nameList({}).isEmpty());
    QCOMPARE(StateFile::nameList({QStringLiteral("a")}), QStringLiteral("a"));
    QCOMPARE(StateFile::nameList({QStringLiteral("a"), QStringLiteral("b")}),
             QStringLiteral("a and b"));
    QCOMPARE(StateFile::nameList({QStringLiteral("a"), QStringLiteral("b"), QStringLiteral("c")}),
             QStringLiteral("a, b and c"));
}

// More names than fit in a line: the first ones and the number of the rest.
void TestStateFile::nameListCountsTheNamesItLeavesOut()
{
    QStringList names;
    for (int i = 0; i < 13; ++i) {
        names << QStringLiteral("name%1").arg(i);
    }

    const QString list = StateFile::nameList(names, 10);
    QVERIFY(list.contains(QStringLiteral("name0")));
    QVERIFY(list.contains(QStringLiteral("name9")));
    QVERIFY(!list.contains(QStringLiteral("name10")));
    QVERIFY(list.contains(QStringLiteral("3 more")));
}

// The note about the values a file names that this version does not know names
// them, in one sentence.
void TestStateFile::theUnknownValuesNoteNamesTheValues()
{
    QVERIFY(StateFile::unknownValuesNote({}).isEmpty());

    const QString note = StateFile::unknownValuesNote(
        {QStringLiteral("lineEdit_mask"), QStringLiteral("spinBox_runtime")});
    QVERIFY(note.contains(QStringLiteral("lineEdit_mask")));
    QVERIFY(note.contains(QStringLiteral("spinBox_runtime")));
}

int main(int argc, char *argv[])
{
    TestStateFile test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_statefile.moc"
