/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "pathlistcontroller.h"
#include "testenvironment.h"

#include <QApplication>
#include <QDir>
#include <QListWidget>
#include <QSignalSpy>
#include <QStyle>
#include <QTemporaryDir>

// The list controller: paths in, checked rows out, the monitor decoration,
// and the changed signal the preview rebuilds from.
class TestPathListController : public QObject
{
    Q_OBJECT

private slots:
    void pathsAreAddedOnceAndChecked();
    void removeAndMoveFollowTheSelection();
    void decorationMarksUnusablePaths();
    void theDecorationIsNotACommandChange();
    void aModelResetRefreshesTheMonitor();
    void anUnmonitoredListStillAnnouncesChanges();

private:
    struct Icons {
        QIcon file = QApplication::style()->standardIcon(QStyle::SP_FileIcon);
        QIcon folder = QApplication::style()->standardIcon(QStyle::SP_DirIcon);
        QIcon warning = QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning);
    };
};

void TestPathListController::pathsAreAddedOnceAndChecked()
{
    QListWidget list;
    PathListController controller(&list, nullptr);

    controller.addPaths({QStringLiteral("/tmp/a.txt"), QStringLiteral("/tmp/a.txt"),
                         QStringLiteral("/tmp/b.txt"), QString()});
    QCOMPARE(list.count(), 2);
    // The rows hold the platform's own separator form, like the dialogs return.
    const QStringList both{QDir::toNativeSeparators(QStringLiteral("/tmp/a.txt")),
                           QDir::toNativeSeparators(QStringLiteral("/tmp/b.txt"))};
    QCOMPARE(controller.checkedItems(), both);

    // An unchecked row keeps its place but stays out of the command.
    list.item(0)->setCheckState(Qt::Unchecked);
    QCOMPARE(list.count(), 2);
    QCOMPARE(controller.checkedItems(),
             QStringList{QDir::toNativeSeparators(QStringLiteral("/tmp/b.txt"))});
}

void TestPathListController::removeAndMoveFollowTheSelection()
{
    QListWidget list;
    PathListController controller(&list, nullptr);
    controller.addPaths(
        {QStringLiteral("/tmp/a.txt"), QStringLiteral("/tmp/b.txt"), QStringLiteral("/tmp/c.txt")});

    // Nothing selected: a move does nothing, no null row appears.
    controller.moveSelected(-1);
    QCOMPARE(list.count(), 3);

    const QString b = QDir::toNativeSeparators(QStringLiteral("/tmp/b.txt"));
    list.setCurrentRow(1);
    controller.moveSelected(-1);
    QCOMPARE(list.item(0)->text(), b);
    // Two moves down carry b to the end; a move past the end does nothing.
    controller.moveSelected(1);
    controller.moveSelected(1);
    controller.moveSelected(1);
    QCOMPARE(list.item(2)->text(), b);

    list.setCurrentRow(0);
    controller.removeSelected();
    QCOMPARE(list.count(), 2);
    QCOMPARE(list.item(0)->text(), QDir::toNativeSeparators(QStringLiteral("/tmp/c.txt")));
}

void TestPathListController::decorationMarksUnusablePaths()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString present = directory.filePath(QStringLiteral("words.txt"));
    const QString missing = directory.filePath(QStringLiteral("gone.txt"));
    QFile file(present);
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write("password\n");
    file.close();

    QListWidget list;
    const Icons icons;
    PathListController controller(&list, PathMonitor::Kind::Wordlist, icons.file, icons.folder,
                                  icons.warning, nullptr);
    controller.addPaths({present, missing});

    QCOMPARE(list.item(0)->icon().cacheKey(), icons.file.cacheKey());
    QVERIFY(list.item(0)->toolTip().isEmpty());
    QCOMPARE(list.item(1)->icon().cacheKey(), icons.warning.cacheKey());
    QVERIFY(list.item(1)->toolTip().contains(QStringLiteral("does not exist")));

    // The watch is on the parent directory: the missing file arriving flips
    // the row without a re-insert.
    QFile arriving(missing);
    QVERIFY(arriving.open(QIODevice::WriteOnly));
    arriving.write("password\n");
    arriving.close();
    QTRY_VERIFY_WITH_TIMEOUT(list.item(1)->icon().cacheKey() == icons.file.cacheKey(), 5000);
    QVERIFY(list.item(1)->toolTip().isEmpty());
}

void TestPathListController::theDecorationIsNotACommandChange()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString missing = directory.filePath(QStringLiteral("gone.txt"));

    QListWidget list;
    const Icons icons;
    PathListController controller(&list, PathMonitor::Kind::Wordlist, icons.file, icons.folder,
                                  icons.warning, nullptr);

    QSignalSpy spy(&controller, &PathListController::changed);
    controller.addPaths({missing});
    const auto afterInsert = spy.count();
    QVERIFY(afterInsert > 0);

    // The warning decoration is an item change, but not a command change.
    QCOMPARE(list.item(0)->icon().cacheKey(), icons.warning.cacheKey());
    QCOMPARE(spy.count(), afterInsert);

    // A check-state change is one.
    list.item(0)->setCheckState(Qt::Unchecked);
    QVERIFY(spy.count() > afterInsert);
}

void TestPathListController::aModelResetRefreshesTheMonitor()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString missing = directory.filePath(QStringLiteral("gone.txt"));

    QListWidget list;
    const Icons icons;
    PathListController controller(&list, PathMonitor::Kind::Wordlist, icons.file, icons.folder,
                                  icons.warning, nullptr);

    // A session restore clears the model and inserts the rows again.
    controller.addPaths({missing});
    list.clear();
    QCOMPARE(list.count(), 0);
    controller.addPaths({missing});
    QCOMPARE(list.item(0)->icon().cacheKey(), icons.warning.cacheKey());
}

void TestPathListController::anUnmonitoredListStillAnnouncesChanges()
{
    QListWidget list;
    PathListController controller(&list, nullptr);

    QSignalSpy spy(&controller, &PathListController::changed);
    controller.addPaths({QStringLiteral("arg one")});
    QVERIFY(spy.count() > 0);
    // No monitor: no decoration, the row keeps its default icon.
    QCOMPARE(list.item(0)->icon().cacheKey(), QIcon().cacheKey());
}

int main(int argc, char *argv[])
{
    TestPathListController test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_pathlistcontroller.moc"