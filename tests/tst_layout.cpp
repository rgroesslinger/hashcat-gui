/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "appconstants.h"
#include "mainwindow.h"
#include "mainwindowtestaccess.h"
#include "mainwindowtesthelpers.h"
#include "settingsmanager.h"
#include "testenvironment.h"
#include "ui_mainwindow.h"
#include "widgetstateserializer.h"

#include <QAction>
#include <QClipboard>
#include <QComboBox>
#include <QCompleter>
#include <QDir>
#include <QDirIterator>
#include <QDoubleSpinBox>
#include <QFile>
#include <QFileInfo>
#include <QGridLayout>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>
#include <QPointer>
#include <QRadioButton>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QSet>
#include <QSpinBox>
#include <QStandardPaths>
#include <QTimer>
#include <QToolTip>
#include <QWidget>

// The layout half of the main window: group box alignment, content sizing
// and shortcuts.
class TestLayout : public QObject
{
    Q_OBJECT

private slots:
    void init()
    {
        m_fixture.init();
    }
    void cleanup()
    {
        m_fixture.cleanup();
    }
    void groupBoxesShareOneTitleAlignment();
    void generalTabGroupBoxesAlignTheirContentToTheTop();
    void attackTabGroupBoxesAlignTheirContentToTheTop();
    void windowFitsItsContent();
    void windowFitsEveryAttackMode();
    void commandBarStaysVisibleOnEveryTab();
    void noShortcutCombinesControlAndAlt();
    void noUiFileContainsAsciiEllipsis();

private:
    MainWindowFixture m_fixture;
};

void TestLayout::groupBoxesShareOneTitleAlignment()
{
    MainWindow window;

    TestEnvironment::checkGroupBoxTitleAlignment(&window);
}

void TestLayout::generalTabGroupBoxesAlignTheirContentToTheTop()
{
    MainWindow window;

    const QStringList offenders =
        groupBoxesWithoutTopAlignment(MainWindowTestAccess::ui(window)->tab_general);
    QVERIFY2(offenders.isEmpty(),
             qPrintable(QStringLiteral("group boxes whose content is not top aligned: %1")
                            .arg(offenders.join(QStringLiteral(", ")))));
}

// The attack tab's boxes sit in rows too: the rules box is stretched by the
// mask and charset boxes next to it, and vice versa.
void TestLayout::attackTabGroupBoxesAlignTheirContentToTheTop()
{
    MainWindow window;

    const QStringList offenders =
        groupBoxesWithoutTopAlignment(MainWindowTestAccess::ui(window)->tab_attack);
    QVERIFY2(offenders.isEmpty(),
             qPrintable(QStringLiteral("group boxes whose content is not top aligned: %1")
                            .arg(offenders.join(QStringLiteral(", ")))));

    // The settings grids next to the PCFG and table lists are stretched by the
    // lists; their rows have to stay at the top as well.
    for (const QString &name :
         {QStringLiteral("gridLayout_pcfg"), QStringLiteral("gridLayout_table")}) {
        auto *grid = MainWindowTestAccess::ui(window)->tab_attack->findChild<QGridLayout *>(name);
        if (grid == nullptr) {
            QFAIL(qPrintable(QStringLiteral("missing layout: %1").arg(name)));
        }
        QVERIFY2(grid->alignment() & Qt::AlignTop,
                 qPrintable(QStringLiteral("layout not top aligned: %1").arg(name)));
    }
}

// No single size fits every font, DPI scaling, style and translation: a fixed
// geometry either clips the content or leaves an empty strip. The window
// measures the content on its first show instead.
void TestLayout::windowFitsItsContent()
{
    // A configured path keeps the constructor from queueing the settings
    // dialog; the path need not exist.
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("/nonexistent/hashcat"));
    m_fixture.startWatchdog();

    MainWindow window;
    window.show();
    QCoreApplication::processEvents();

    QScrollArea *attackArea = MainWindowTestAccess::ui(window)->scrollArea_attack;
    QWidget *attackContent = attackArea->widget();
    QScrollArea *generalArea = MainWindowTestAccess::ui(window)->scrollArea_general;
    QVERIFY(attackContent);
    QVERIFY(generalArea->widget());
    QVERIFY(window.screen());

    // Regression: the General tab needs more width than the Attack tab, so the
    // window must take its width from the tab that is not in front at startup.
    QVERIFY(generalArea->widget()->minimumSizeHint().width()
            > attackContent->minimumSizeHint().width());

    // The expected size: window chrome plus the largest content minimum in
    // each direction. A tall tab also needs its vertical scrollbar, and the
    // width decides how far the mode hint rewraps, so measure without
    // wrapping, then at the chosen width, until the width settles.
    const QSize chrome = window.size() - attackArea->size();
    const QSize cap = window.screen()->availableGeometry().size() - QSize(40, 80);

    // The attack tab hides the groups the other modes need, so the window has
    // to hold the largest minimum across all of them. The helper's result is
    // checked against an independent loop in windowFitsEveryAttackMode().

    QSize attackEnvelope;
    QSize natural;
    int contentWidth = 0;
    for (int pass = 0; pass < 3; ++pass) {
        attackEnvelope = MainWindowTestAccess::attackModeEnvelope(window, contentWidth);
        QSize contentMinimum(0, 0);
        QList<QScrollArea *> tabAreas;
        QList<QSize> minima;
        for (int i = 0; i < MainWindowTestAccess::ui(window)->tabWidget_main->count(); ++i) {
            QScrollArea *tabArea = MainWindowTestAccess::ui(window)
                                       ->tabWidget_main->widget(i)
                                       ->findChild<QScrollArea *>();
            QVERIFY(tabArea);
            QVERIFY(tabArea->widget());
            tabAreas.append(tabArea);
            if (tabArea == attackArea) {
                minima.append(attackEnvelope);
            } else {
                QSize minimum = tabArea->widget()->minimumSizeHint();
                if (contentWidth > 0) {
                    const int wrapped = tabArea->widget()->heightForWidth(contentWidth);
                    if (wrapped > minimum.height()) {
                        minimum.setHeight(wrapped);
                    }
                }
                minima.append(minimum);
            }
            contentMinimum = contentMinimum.expandedTo(minima.last());
        }
        natural = chrome + contentMinimum;
        const int viewportHeight = natural.boundedTo(cap).height() - chrome.height();
        int requiredWidth = 0;
        for (int i = 0; i < tabAreas.count(); ++i) {
            int width = minima.at(i).width();
            if (minima.at(i).height() > viewportHeight) {
                width += tabAreas.at(i)->verticalScrollBar()->sizeHint().width();
            }
            requiredWidth = qMax(requiredWidth, width);
        }
        if (requiredWidth == contentWidth) {
            break;
        }
        contentWidth = requiredWidth;
    }
    natural.setWidth(chrome.width() + contentWidth);
    const QSize target = natural.boundedTo(cap);

    // The window has to be big enough for the content: a scrollbar for
    // content that fits is the bug this guards. Checked before the exact
    // size, so that a failure says what went wrong.
    if (natural.height() <= cap.height()) {
        QTRY_VERIFY(!attackArea->verticalScrollBar()->isVisible());
    }

    // QTRY because a resize inside showEvent() needs the event loop to settle
    // the child layouts before the final scrollbar state is visible.
    QTRY_COMPARE(window.size(), target);

    // Both tabs fit horizontally. Only the height may fall back to a scrollbar
    // when the screen is too small for the tallest tab.
    if (target.width() == natural.width()) {
        for (int i = 0; i < MainWindowTestAccess::ui(window)->tabWidget_main->count(); ++i) {
            MainWindowTestAccess::ui(window)->tabWidget_main->setCurrentIndex(i);
            QCoreApplication::processEvents();
            QScrollArea *tabArea = MainWindowTestAccess::ui(window)
                                       ->tabWidget_main->widget(i)
                                       ->findChild<QScrollArea *>();
            QTRY_VERIFY(!tabArea->horizontalScrollBar()->isVisible());
        }
    }

    // The fit uses the same chrome for every tab and must not depend on which
    // one is in front.
    MainWindowTestAccess::ui(window)->tabWidget_main->setCurrentIndex(1);
    QCoreApplication::processEvents();
    MainWindowTestAccess::fitToContent(window);
    QCoreApplication::processEvents();
    QCOMPARE(window.size(), target);
    MainWindowTestAccess::ui(window)->tabWidget_main->setCurrentIndex(0);
    QCoreApplication::processEvents();

    if (natural.height() <= cap.height()) {
        // The content fits the screen: no scrollbar, and no empty strip
        // beyond the envelope the window was sized to. A shorter mode keeps
        // the rest of the envelope empty on purpose, so that switching modes
        // never resizes the window.
        QTRY_VERIFY(!attackArea->verticalScrollBar()->isVisible());
        QVERIFY(attackContent->height() - attackEnvelope.height() < 30);
    } else {
        // The content is taller than the screen: the window stays on-screen
        // and the scroll area has to show its scrollbar.
        QTRY_VERIFY(attackArea->verticalScrollBar()->isVisible());
    }

    // The hint wraps at the tab width and the fit reserves the wrapped height.
    QVERIFY(MainWindowTestAccess::ui(window)->label_mode_hint->wordWrap());
}

// A window sized to the attack-mode envelope must not show a scrollbar when
// the mode changes, and switching must not resize it: the size is chosen once,
// on the first show. The offscreen test screen is smaller than the envelope,
// so the window is grown here; a window manager that clamps that makes the
// check impossible, and the test skips instead of failing.
void TestLayout::windowFitsEveryAttackMode()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("/nonexistent/hashcat"));
    m_fixture.startWatchdog();

    MainWindow window;
    window.show();
    QCoreApplication::processEvents();

    QScrollArea *attackArea = MainWindowTestAccess::ui(window)->scrollArea_attack;
    QComboBox *attackCombo = MainWindowTestAccess::ui(window)->comboBox_attack;
    QVERIFY(attackArea);
    QVERIFY(attackArea->widget());
    QVERIFY(attackCombo->count() > 1);

    const int startIndex = attackCombo->currentIndex();
    QVERIFY(startIndex >= 0);

    // The fit wraps the mode hint at the content width, and the envelope the
    // helper reports has to include the height that costs - measured at the
    // width the content actually has.
    const int wrapWidth = attackArea->widget()->width();
    QSize envelope(0, 0);
    for (int i = 0; i < attackCombo->count(); ++i) {
        attackCombo->setCurrentIndex(i);
        QCoreApplication::processEvents();
        QSize minimum = attackArea->widget()->minimumSizeHint();
        const int wrapped = attackArea->widget()->heightForWidth(wrapWidth);
        if (wrapped > minimum.height()) {
            minimum.setHeight(wrapped);
        }
        envelope = envelope.expandedTo(minimum);
    }
    attackCombo->setCurrentIndex(startIndex);
    QCoreApplication::processEvents();

    // The helper the fit uses has to measure the same envelope and restore
    // the selected mode, not leave the last measured one on screen.
    QCOMPARE(MainWindowTestAccess::attackModeEnvelope(window, wrapWidth), envelope);
    QCOMPARE(attackCombo->currentIndex(), startIndex);

    const QSize chrome = window.size() - attackArea->size();
    const QSize needed = chrome + envelope;
    window.resize(needed + QSize(40, 40));
    QCoreApplication::processEvents();
    if (window.width() < needed.width() || window.height() < needed.height()) {
        QSKIP("The screen is too small to hold the attack-mode envelope");
    }

    const QSize fittedSize = window.size();
    for (int i = 0; i < attackCombo->count(); ++i) {
        attackCombo->setCurrentIndex(i);
        QCoreApplication::processEvents();
        QCOMPARE(window.size(), fittedSize);
        QTRY_VERIFY(!attackArea->verticalScrollBar()->isVisible());
        QTRY_VERIFY(!attackArea->horizontalScrollBar()->isVisible());
    }
}

// The command bar belongs to the window, not to one tab: switching tabs must
// not take Start, Copy or the preview away.
void TestLayout::commandBarStaysVisibleOnEveryTab()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath,
                                       QStringLiteral("/nonexistent/hashcat"));
    m_fixture.startWatchdog();

    MainWindow window;
    window.show();
    QCoreApplication::processEvents();

    QPushButton *execute = MainWindowTestAccess::ui(window)->pushButton_start;
    QPushButton *copy = MainWindowTestAccess::ui(window)->pushButton_copy_clipboard;
    QLineEdit *preview = MainWindowTestAccess::ui(window)->lineEdit_command;

    // The row lives below the tabs, not in the first tab's scroll content.
    QVERIFY(window.centralWidget()->isAncestorOf(execute));
    QVERIFY(!MainWindowTestAccess::ui(window)->tabWidget_main->isAncestorOf(execute));
    QVERIFY(!MainWindowTestAccess::ui(window)->scrollArea_attack->isAncestorOf(execute));
    QVERIFY(!MainWindowTestAccess::ui(window)->tabWidget_main->isAncestorOf(copy));
    QVERIFY(!MainWindowTestAccess::ui(window)->tabWidget_main->isAncestorOf(preview));

    QVERIFY(execute->isVisible());
    QVERIFY(copy->isVisible());
    QVERIFY(preview->isVisible());

    // ... and the row stays on screen on the second tab as well.
    MainWindowTestAccess::ui(window)->tabWidget_main->setCurrentWidget(
        MainWindowTestAccess::ui(window)->tab_general);
    QCoreApplication::processEvents();

    QVERIFY(execute->isVisible());
    QVERIFY(copy->isVisible());
    QVERIFY(preview->isVisible());
}

// Ctrl+Alt is how Qt reports AltGr on Windows unless the application starts
// with -platform windows:altgr. A shortcut on that combination would swallow
// characters such as Euro on German and French layouts while the user types
// into the hash or mask field. Keeping the pair out of the action list is the
// only portable guard; a test cannot see AltGr itself.
void TestLayout::noShortcutCombinesControlAndAlt()
{
    MainWindow window;

    const auto actions = window.findChildren<QAction *>();
    QVERIFY(!actions.isEmpty());
    for (const QAction *action : actions) {
        // QKeySequence has no bounds-safe accessor for its combinations, so
        // the analyzer rejects operator[]; the portable text names the same
        // modifiers.
        const QString shortcut = action->shortcut().toString(QKeySequence::PortableText);
        QVERIFY2(!shortcut.contains(QStringLiteral("Ctrl+Alt")),
                 qPrintable(QStringLiteral("%1: %2").arg(action->objectName(), shortcut)));
    }
}

// The typographic ellipsis is the house style; ASCII "..." in a .ui file
// renders three dots and bypasses the translators' punctuation conventions.
void TestLayout::noUiFileContainsAsciiEllipsis()
{
    const QDir sourceDir(QStringLiteral(HASHCAT_GUI_SOURCE_DIR) + QStringLiteral("/src"));
    QDirIterator it(sourceDir.path(), {QStringLiteral("*.ui")}, QDir::Files,
                    QDirIterator::Subdirectories);
    int found = 0;
    while (it.hasNext()) {
        const QString path = it.next();
        ++found;
        QFile file(path);
        QVERIFY2(file.open(QIODevice::ReadOnly), qPrintable(path));
        const QString text = QString::fromUtf8(file.readAll());
        QVERIFY2(!text.contains(QStringLiteral("...")), qPrintable(path));
    }
    QVERIFY(found > 0);
}

int main(int argc, char *argv[])
{
    TestLayout test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_layout.moc"
