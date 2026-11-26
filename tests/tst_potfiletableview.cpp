/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfiletableview.h"
#include "testenvironment.h"

#include "appconstants.h"

#include <QApplication>
#include <QClipboard>
#include <QDialog>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSignalSpy>
#include <QStackedLayout>
#include <QTest>
#include <QTimer>
#include <QTreeWidget>
#include <QVBoxLayout>

namespace
{
PotfileTableView::Row row(const QString &hash, const QString &plain, const QStringList &extras = {})
{
    PotfileTableView::Row value;
    value.hash = hash;
    value.plain = plain;
    value.extraColumns = extras;
    value.emptyPlain = plain.isEmpty();
    return value;
}

QTreeWidget *tree(PotfileTableView &view)
{
    return view.findChild<QTreeWidget *>(QStringLiteral("treeWidget_results"));
}

QLineEdit *search(PotfileTableView &view)
{
    return view.findChild<QLineEdit *>(QStringLiteral("lineEdit_search"));
}

QPushButton *button(PotfileTableView &view, const char *name)
{
    return view.findChild<QPushButton *>(QString::fromLatin1(name));
}
} // namespace

class TestPotfileTableView : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void rowsSortByHashAndHeaderClickToggles();
    void fileRowsAreLabeledButCopyTheRawName();
    void filterEmptyStateAndVisibleRowsSignal();
    void largeRowSetsDebounceTheFilter();
    void revealTogglesTextAndMasks();
    void maskedCopyIsDisabledUnlessNothingIsRevealable();
    void ctrlCCopiesSelectedVisibleRows();
    void ctrlCWhileMaskedDoesNotCopy();
    void ctrlCInTheSearchFieldCopiesTheSearchText();
    void contextMenuCopiesSelectedRows();
    void preserveViewStateKeepsSearchRevealAndSort();
    void defaultSetRowsResetsTheViewState();
    void visibleRowsMapBackViaSourceIndex();
    void enterDoesNotFireTheDialogDefault();
    void contextMenuTargetsTheRowUnderTheCursor();
};

void TestPotfileTableView::init()
{
    QApplication::clipboard()->clear();
}

void TestPotfileTableView::rowsSortByHashAndHeaderClickToggles()
{
    PotfileTableView view;
    view.setRows({row("b", "two"), row("a", "one"), row("c", "three")});

    QCOMPARE(tree(view)->topLevelItem(0)->text(0), QStringLiteral("a"));
    QCOMPARE(tree(view)->topLevelItem(2)->text(0), QStringLiteral("c"));

    QHeaderView *header = tree(view)->header();
    QTest::mouseClick(header->viewport(), Qt::LeftButton, Qt::NoModifier,
                      QPoint(header->sectionViewportPosition(0) + (header->sectionSize(0) / 2),
                             header->height() / 2));
    QCOMPARE(tree(view)->topLevelItem(0)->text(0), QStringLiteral("c"));
}

void TestPotfileTableView::fileRowsAreLabeledButCopyTheRawName()
{
    PotfileTableView view;
    const QString longHash = QStringLiteral("a-very-long-hash-value");
    PotfileTableView::Row fileRow = row("hashes.hc22000", "secret");
    fileRow.hashIsFile = true;
    view.setRows({row(longHash, "plain"), fileRow});

    // The elided hash column carries the full value as a tooltip. The file
    // row sorts first: its display text starts with "(".
    QCOMPARE(tree(view)->topLevelItem(0)->text(0), QStringLiteral("(file) hashes.hc22000"));
    QVERIFY(tree(view)->topLevelItem(0)->toolTip(0).contains(QStringLiteral("Binary hash file")));
    QCOMPARE(tree(view)->topLevelItem(1)->toolTip(0), longHash);

    button(view, "pushButton_reveal")->click();
    button(view, "pushButton_copy_all")->click();
    QCOMPARE(QApplication::clipboard()->text(),
             QStringLiteral("hashes.hc22000\tsecret\na-very-long-hash-value\tplain"));
}

void TestPotfileTableView::filterEmptyStateAndVisibleRowsSignal()
{
    PotfileTableView view;
    view.setEmptyStateText(QStringLiteral("Nothing here."));
    view.setRows({row("hash-one", "one"), row("hash-two", "two")});

    QSignalSpy spy(&view, &PotfileTableView::visibleRowsChanged);
    search(view)->setText(QStringLiteral("hash-two"));
    QVERIFY(spy.count() > 0);
    QCOMPARE(view.visibleRowCount(), 1);

    search(view)->setText(QStringLiteral("no-match"));
    QCOMPARE(view.visibleRowCount(), 0);
    auto *stack = view.findChild<QStackedLayout *>(QStringLiteral("table_stack"));
    auto *empty = view.findChild<QLabel *>(QStringLiteral("label_empty_state"));
    QCOMPARE(stack->currentWidget(), static_cast<QWidget *>(empty));
    QVERIFY(empty->text().contains(QStringLiteral("No rows match the filter.")));

    search(view)->clear();
    QCOMPARE(stack->currentWidget(), static_cast<QWidget *>(tree(view)));
}

// A 100k-row table cannot synchronously rescan on every keystroke. Above the
// threshold the filter waits for a pause; typing again restarts the wait, so
// one pass runs for the final text.
void TestPotfileTableView::largeRowSetsDebounceTheFilter()
{
    QList<PotfileTableView::Row> rows;
    const int count = AppConstants::Hashcat::PotfileFilterDebounceThreshold + 1000;
    rows.reserve(count);
    for (int i = 0; i < count; ++i) {
        rows.append(row(QStringLiteral("hash-%1").arg(i), QStringLiteral("plain")));
    }

    PotfileTableView view;
    view.setRows(rows);
    QCOMPARE(view.visibleRowCount(), count);

    QSignalSpy spy(&view, &PotfileTableView::visibleRowsChanged);
    search(view)->setText(QStringLiteral("hash-4999"));
    // The keystroke itself does not filter; the table is unchanged.
    QCOMPARE(spy.count(), 0);
    QCOMPARE(view.visibleRowCount(), count);

    QTRY_COMPARE_WITH_TIMEOUT(view.visibleRowCount(), 1, 2000);
    QCOMPARE(spy.count(), 1);

    // A second keystroke before the timer fires restarts it: still one pass.
    spy.clear();
    search(view)->setText(QStringLiteral("hash-3999"));
    search(view)->setText(QStringLiteral("hash-"));
    QTRY_COMPARE_WITH_TIMEOUT(view.visibleRowCount(), count, 2000);
    QCOMPARE(spy.count(), 1);
}

void TestPotfileTableView::revealTogglesTextAndMasks()
{
    PotfileTableView view;
    view.setRows({row("hash", "secret")});
    auto *reveal = button(view, "pushButton_reveal");

    QCOMPARE(reveal->text(), QStringLiteral("Reveal plaintexts"));
    QVERIFY(tree(view)->topLevelItem(0)->text(1).contains(QStringLiteral("•")));
    reveal->click();
    QCOMPARE(reveal->text(), QStringLiteral("Hide plaintexts"));
    QCOMPARE(tree(view)->topLevelItem(0)->text(1), QStringLiteral("secret"));
    reveal->click();
    QVERIFY(tree(view)->topLevelItem(0)->text(1).contains(QStringLiteral("•")));
}

void TestPotfileTableView::maskedCopyIsDisabledUnlessNothingIsRevealable()
{
    PotfileTableView view;
    view.setRows({row("hash", "secret")});
    QVERIFY(!button(view, "pushButton_copy_all")->isEnabled());

    PotfileTableView::Row encrypted = row("hash", "$HCENC$1$key$cipher");
    encrypted.encrypted = true;
    view.setRows({encrypted});
    QVERIFY(button(view, "pushButton_copy_all")->isEnabled());
    QVERIFY(!button(view, "pushButton_reveal")->isEnabled());
}

void TestPotfileTableView::ctrlCCopiesSelectedVisibleRows()
{
    PotfileTableView view;
    view.setRows({row("hash-one", "one", {QStringLiteral("alice")}),
                  row("hash-two", "two", {QStringLiteral("bob")})});
    button(view, "pushButton_reveal")->click();
    view.show();
    view.activateWindow();
    QVERIFY(QTest::qWaitForWindowExposed(&view));

    tree(view)->topLevelItem(1)->setSelected(true);
    tree(view)->setFocus();
    QTest::keyClick(tree(view), Qt::Key_C, Qt::ControlModifier);
    QCOMPARE(QApplication::clipboard()->text(), QStringLiteral("hash-two\ttwo\tbob"));
}

void TestPotfileTableView::ctrlCWhileMaskedDoesNotCopy()
{
    PotfileTableView view;
    view.setRows({row("hash-one", "secret")});
    view.show();
    view.activateWindow();
    QVERIFY(QTest::qWaitForWindowExposed(&view));

    tree(view)->topLevelItem(0)->setSelected(true);
    tree(view)->setFocus();
    QTest::keyClick(tree(view), Qt::Key_C, Qt::ControlModifier);
    QVERIFY(QApplication::clipboard()->text().isEmpty());

    // Reveal, then hide again: the gate must follow the state, not latch.
    button(view, "pushButton_reveal")->click();
    button(view, "pushButton_reveal")->click();
    QTest::keyClick(tree(view), Qt::Key_C, Qt::ControlModifier);
    QVERIFY(QApplication::clipboard()->text().isEmpty());
}

void TestPotfileTableView::ctrlCInTheSearchFieldCopiesTheSearchText()
{
    PotfileTableView view;
    view.setRows({row("hash-one", "secret")});
    button(view, "pushButton_reveal")->click();
    view.show();
    view.activateWindow();
    QVERIFY(QTest::qWaitForWindowExposed(&view));

    search(view)->setText(QStringLiteral("filter text"));
    search(view)->setFocus();
    search(view)->selectAll();
    QTest::keyClick(search(view), Qt::Key_C, Qt::ControlModifier);
    QCOMPARE(QApplication::clipboard()->text(), QStringLiteral("filter text"));
}

void TestPotfileTableView::contextMenuCopiesSelectedRows()
{
    PotfileTableView view;
    view.setRows({row("hash-one", "one"), row("hash-two", "two")});
    button(view, "pushButton_reveal")->click();
    view.show();
    tree(view)->topLevelItem(0)->setSelected(true);

    QTimer timer;
    timer.setInterval(20);
    QObject::connect(&timer, &QTimer::timeout, &timer, [&] {
        // Offscreen registers no popup and never closes the menu on trigger;
        // find it among the top levels and close it explicitly.
        const auto widgets = QApplication::topLevelWidgets();
        for (QWidget *widget : widgets) {
            if (auto *menu = qobject_cast<QMenu *>(widget)) {
                const auto actions = menu->actions();
                if (!actions.isEmpty()) {
                    actions.first()->trigger();
                    menu->close();
                    return;
                }
            }
        }
    });
    timer.start();
    emit tree(view)->customContextMenuRequested(QPoint(5, 5));
    timer.stop();

    QCOMPARE(QApplication::clipboard()->text(), QStringLiteral("hash-one\tone"));
}

void TestPotfileTableView::preserveViewStateKeepsSearchRevealAndSort()
{
    PotfileTableView view;
    view.setRows({row("a", "one"), row("b", "two")});
    button(view, "pushButton_reveal")->click();
    search(view)->setText(QStringLiteral("b"));
    tree(view)->topLevelItem(0)->setSelected(true);
    // Descending sort via the header.
    tree(view)->sortByColumn(0, Qt::DescendingOrder);

    view.setRows({row("a", "one"), row("b", "two"), row("c", "three")},
                 /*preserveViewState=*/true);

    QCOMPARE(search(view)->text(), QStringLiteral("b"));
    QCOMPARE(button(view, "pushButton_reveal")->text(), QStringLiteral("Hide plaintexts"));
    QCOMPARE(tree(view)->header()->sortIndicatorOrder(), Qt::DescendingOrder);
    QVERIFY(tree(view)->topLevelItem(0)->isHidden());
    QVERIFY(tree(view)->selectedItems().isEmpty());
    QCOMPARE(view.visibleRowCount(), 1);
}

void TestPotfileTableView::defaultSetRowsResetsTheViewState()
{
    PotfileTableView view;
    view.setRows({row("a", "one")});
    button(view, "pushButton_reveal")->click();
    search(view)->setText(QStringLiteral("a"));
    tree(view)->sortByColumn(0, Qt::DescendingOrder);

    view.setRows({row("b", "two")});
    QVERIFY(search(view)->text().isEmpty());
    QCOMPARE(button(view, "pushButton_reveal")->text(), QStringLiteral("Reveal plaintexts"));
    QVERIFY(tree(view)->topLevelItem(0)->text(1).contains(QStringLiteral("•")));
    QCOMPARE(tree(view)->header()->sortIndicatorOrder(), Qt::AscendingOrder);
}

void TestPotfileTableView::visibleRowsMapBackViaSourceIndex()
{
    PotfileTableView view;
    PotfileTableView::Row a = row("a", "one");
    PotfileTableView::Row b = row("b", "two");
    PotfileTableView::Row c = row("c", "three");
    a.sourceIndex = 0;
    b.sourceIndex = 1;
    c.sourceIndex = 2;
    view.setRows({a, b, c});
    search(view)->setText(QStringLiteral("b"));
    tree(view)->sortByColumn(0, Qt::DescendingOrder);
    search(view)->clear();

    const QList<PotfileTableView::Row> visible = view.visibleRows();
    QCOMPARE(visible.size(), 3);
    // View order is descending; the source indices still name the input rows.
    QCOMPARE(visible.at(0).hash, QStringLiteral("c"));
    QCOMPARE(visible.at(0).sourceIndex, 2);
    QCOMPARE(visible.at(2).hash, QStringLiteral("a"));
    QCOMPARE(visible.at(2).sourceIndex, 0);
}

// Enter in the view must reach neither a dialog default nor the clipboard.
void TestPotfileTableView::enterDoesNotFireTheDialogDefault()
{
    QDialog dialog;
    auto *layout = new QVBoxLayout(&dialog);
    auto *view = new PotfileTableView;
    auto *close = new QPushButton("Close");
    layout->addWidget(view);
    layout->addWidget(close);
    close->setDefault(true);
    int clicks = 0;
    QObject::connect(close, &QPushButton::clicked, &dialog, [&] { ++clicks; });

    view->setRows({row("hash-one", "secret")});
    button(*view, "pushButton_reveal")->click();
    tree(*view)->topLevelItem(0)->setSelected(true);
    dialog.show();
    QApplication::clipboard()->clear();

    QTest::keyClick(tree(*view), Qt::Key_Return);
    QTest::keyClick(search(*view), Qt::Key_Return);
    QCOMPARE(clicks, 0);
    QVERIFY(QApplication::clipboard()->text().isEmpty());
    QVERIFY(dialog.isVisible());
}

void TestPotfileTableView::contextMenuTargetsTheRowUnderTheCursor()
{
    PotfileTableView view;
    view.setRows({row("hash-one", "one"), row("hash-two", "two")});
    button(view, "pushButton_reveal")->click();
    view.show();
    tree(view)->topLevelItem(0)->setSelected(true);

    QTimer timer;
    timer.setInterval(20);
    QObject::connect(&timer, &QTimer::timeout, &timer, [&] {
        const auto widgets = QApplication::topLevelWidgets();
        for (QWidget *widget : widgets) {
            if (auto *menu = qobject_cast<QMenu *>(widget)) {
                const auto actions = menu->actions();
                if (!actions.isEmpty()) {
                    actions.first()->trigger();
                    menu->close();
                    return;
                }
            }
        }
    });
    timer.start();
    const QRect rect = tree(view)->visualItemRect(tree(view)->topLevelItem(1));
    emit tree(view)->customContextMenuRequested(rect.center());
    timer.stop();

    QCOMPARE(QApplication::clipboard()->text(), QStringLiteral("hash-two\ttwo"));
}

int main(int argc, char *argv[])
{
    TestPotfileTableView test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_potfiletableview.moc"