/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "pathlistcontroller.h"
#include "testenvironment.h"

#include <QApplication>
#include <QDir>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFile>
#include <QListWidget>
#include <QMimeData>
#include <QSignalSpy>
#include <QStyle>
#include <QTemporaryDir>
#include <QUrl>

namespace
{
// Qt 6.12 adds the QPointF constructor and deprecates QPoint.
#if QT_VERSION >= QT_VERSION_CHECK(6, 12, 0)
using DragPoint = QPointF;
#else
using DragPoint = QPoint;
#endif

// Events that propose a move while still allowing a copy.
class MoveProposedDragEnterEvent : public QDragEnterEvent
{
public:
    using QDragEnterEvent::QDragEnterEvent;

    void proposeMove()
    {
        m_defaultAction = Qt::MoveAction;
    }
};

class MoveProposedDropEvent : public QDropEvent
{
public:
    using QDropEvent::QDropEvent;

    void proposeMove()
    {
        m_defaultAction = Qt::MoveAction;
    }
};
} // namespace

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
    void aDropAddsTheLocalFilesOnce();
    void aRulesetDropAcceptsFilesAndFolders();
    void aTableDropIgnoresFolders();
    void aDropNeverMovesTheSource();

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

// A local URL becomes a checked row; a remote URL is ignored.
void TestPathListController::aDropAddsTheLocalFilesOnce()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString first = directory.filePath(QStringLiteral("first.txt"));
    const QString second = directory.filePath(QStringLiteral("second.txt"));
    QFile firstFile(first);
    QVERIFY(firstFile.open(QIODevice::WriteOnly));
    firstFile.close();
    QFile secondFile(second);
    QVERIFY(secondFile.open(QIODevice::WriteOnly));
    secondFile.close();

    QListWidget list;
    list.setAcceptDrops(true);
    list.setDragDropMode(QAbstractItemView::DropOnly);
    PathListController controller(&list, nullptr);

    QMimeData mime;
    mime.setUrls({QUrl::fromLocalFile(first), QUrl::fromLocalFile(second),
                  QUrl(QStringLiteral("https://example.com/wordlist.txt"))});

    QDragEnterEvent enter(DragPoint(5, 5), Qt::CopyAction, &mime, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(list.viewport(), &enter);
    QVERIFY(enter.isAccepted());

    QDropEvent drop(QPointF(5, 5), Qt::CopyAction, &mime, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(list.viewport(), &drop);
    QVERIFY(drop.isAccepted());

    const QStringList expected{QDir::toNativeSeparators(first), QDir::toNativeSeparators(second)};
    QCOMPARE(controller.checkedItems(), expected);
}

// A ruleset may be a file or a folder with a grammar; both can be dropped.
void TestPathListController::aRulesetDropAcceptsFilesAndFolders()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString ruleset = directory.filePath(QStringLiteral("rules.rule"));
    QFile file(ruleset);
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.close();
    const QString folder = directory.filePath(QStringLiteral("grammar"));
    QVERIFY(QDir().mkpath(folder));

    QListWidget list;
    list.setAcceptDrops(true);
    list.setDragDropMode(QAbstractItemView::DropOnly);
    const Icons icons;
    PathListController controller(&list, PathMonitor::Kind::Pcfg, icons.file, icons.folder,
                                  icons.warning, nullptr);

    QMimeData mime;
    mime.setUrls({QUrl::fromLocalFile(ruleset), QUrl::fromLocalFile(folder)});

    QDragEnterEvent enter(DragPoint(5, 5), Qt::CopyAction, &mime, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(list.viewport(), &enter);
    QVERIFY(enter.isAccepted());

    QDropEvent drop(QPointF(5, 5), Qt::CopyAction, &mime, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(list.viewport(), &drop);
    QVERIFY(drop.isAccepted());

    const QStringList expected{QDir::toNativeSeparators(ruleset), QDir::toNativeSeparators(folder)};
    QCOMPARE(controller.checkedItems(), expected);
}

// A table is a file only, so a folder drag cannot add a row.
void TestPathListController::aTableDropIgnoresFolders()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString table = directory.filePath(QStringLiteral("table.txt"));
    QFile file(table);
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.close();
    const QString folder = directory.filePath(QStringLiteral("folder"));
    QVERIFY(QDir().mkpath(folder));

    QListWidget list;
    list.setAcceptDrops(true);
    list.setDragDropMode(QAbstractItemView::DropOnly);
    const Icons icons;
    PathListController controller(&list, PathMonitor::Kind::Table, icons.file, icons.folder,
                                  icons.warning, nullptr);

    QMimeData folderOnly;
    folderOnly.setUrls({QUrl::fromLocalFile(folder)});
    QDragEnterEvent folderEnter(DragPoint(5, 5), Qt::CopyAction, &folderOnly, Qt::LeftButton,
                                Qt::NoModifier);
    QApplication::sendEvent(list.viewport(), &folderEnter);
    QDropEvent folderDrop(QPointF(5, 5), Qt::CopyAction, &folderOnly, Qt::LeftButton,
                          Qt::NoModifier);
    QApplication::sendEvent(list.viewport(), &folderDrop);
    QCOMPARE(list.count(), 0);

    // A mixed drop keeps the file and skips the folder.
    QMimeData mixed;
    mixed.setUrls({QUrl::fromLocalFile(table), QUrl::fromLocalFile(folder)});
    QDragEnterEvent enter(DragPoint(5, 5), Qt::CopyAction, &mixed, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(list.viewport(), &enter);
    QVERIFY(enter.isAccepted());
    QDropEvent drop(QPointF(5, 5), Qt::CopyAction, &mixed, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(list.viewport(), &drop);
    QVERIFY(drop.isAccepted());
    QCOMPARE(controller.checkedItems(), QStringList{QDir::toNativeSeparators(table)});
}

// Drops must copy; a move-only drag could make the source remove the file.
void TestPathListController::aDropNeverMovesTheSource()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString wordlist = directory.filePath(QStringLiteral("words.txt"));
    QFile file(wordlist);
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.close();

    QListWidget list;
    list.setAcceptDrops(true);
    list.setDragDropMode(QAbstractItemView::DropOnly);
    const Icons icons;
    PathListController controller(&list, PathMonitor::Kind::Wordlist, icons.file, icons.folder,
                                  icons.warning, nullptr);

    QMimeData mime;
    mime.setUrls({QUrl::fromLocalFile(wordlist)});

    // Copy is possible: the proposed move is overridden.
    MoveProposedDragEnterEvent enter(DragPoint(5, 5), Qt::MoveAction | Qt::CopyAction, &mime,
                                     Qt::LeftButton, Qt::NoModifier);
    enter.proposeMove();
    QApplication::sendEvent(list.viewport(), &enter);
    QVERIFY(enter.isAccepted());
    QCOMPARE(enter.dropAction(), Qt::CopyAction);

    MoveProposedDropEvent drop(QPointF(5, 5), Qt::MoveAction | Qt::CopyAction, &mime,
                               Qt::LeftButton, Qt::NoModifier);
    drop.proposeMove();
    QApplication::sendEvent(list.viewport(), &drop);
    QVERIFY(drop.isAccepted());
    QCOMPARE(drop.dropAction(), Qt::CopyAction);
    QCOMPARE(controller.checkedItems(), QStringList{QDir::toNativeSeparators(wordlist)});

    // Move only: refusing the drag leaves the file with its source.
    list.clear();
    QDragEnterEvent moveEnter(DragPoint(5, 5), Qt::MoveAction, &mime, Qt::LeftButton,
                              Qt::NoModifier);
    QApplication::sendEvent(list.viewport(), &moveEnter);
    QVERIFY(!moveEnter.isAccepted());
    QDropEvent moveDrop(QPointF(5, 5), Qt::MoveAction, &mime, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(list.viewport(), &moveDrop);
    QVERIFY(!moveDrop.isAccepted());
    QCOMPARE(list.count(), 0);
}

int main(int argc, char *argv[])
{
    TestPathListController test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_pathlistcontroller.moc"