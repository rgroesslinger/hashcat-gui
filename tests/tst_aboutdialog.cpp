/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QGroupBox>
#include <QLabel>

#include "aboutdialog.h"

// The About dialog must not be pinned to the 500x160 in the .ui: under a
// large font or a longer version string that clips its content, under a small
// one it leaves an empty margin. SetFixedSize makes it exactly content-sized,
// and the version label wraps so a long hashcat error cannot push the dialog
// off the screen.
class TestAboutDialog : public QObject
{
    Q_OBJECT

private slots:
    void fitsItsContent();
    void longVersionTextWrapsInsteadOfWidening();
    void groupBoxesShareOneTitleAlignment();
    void showsTheLogoAndTheApplicationName();
};

void TestAboutDialog::fitsItsContent()
{
    AboutDialog dialog;
    dialog.show();
    QCoreApplication::processEvents();

    QCOMPARE(dialog.size(), dialog.sizeHint());
    // content-sized, and still not resizable
    QCOMPARE(dialog.minimumSize(), dialog.maximumSize());
    QCOMPARE(dialog.size(), dialog.maximumSize());
}

void TestAboutDialog::longVersionTextWrapsInsteadOfWidening()
{
    AboutDialog dialog;
    dialog.show();
    QCoreApplication::processEvents();

    QLabel *label = dialog.findChild<QLabel *>(QStringLiteral("label_hc_version"));
    QVERIFY(label);

    const QString longText = QStringLiteral("Error: ") + QString(300, QLatin1Char('x'));
    const int singleLineWidth = label->fontMetrics().horizontalAdvance(longText);
    const int heightBefore = dialog.height();
    label->setText(longText);

    // SetFixedSize re-applies the layout's size once the event loop turns.
    QTRY_VERIFY(dialog.height() > heightBefore);
    // The text wrapped onto several lines instead of widening the dialog to
    // the whole one-line string, so it stays on-screen.
    QVERIFY(label->height() > label->fontMetrics().height());
    QVERIFY(dialog.width() < singleLineWidth);
}

// The titles of the group boxes have to look alike. The alignment is the only
// thing that decides it, and a group box without it would draw its title
// above the frame while its neighbours break the border with it.
void TestAboutDialog::groupBoxesShareOneTitleAlignment()
{
    AboutDialog dialog;

    TestEnvironment::checkGroupBoxTitleAlignment(&dialog);
}

// The dialog opens with the artwork of the application and its name, so it
// says what it is about before it lists any version.
void TestAboutDialog::showsTheLogoAndTheApplicationName()
{
    AboutDialog dialog;

    QLabel *logo = dialog.findChild<QLabel *>(QStringLiteral("label_logo"));
    QVERIFY(logo);
    QVERIFY2(!logo->pixmap().isNull(), "the logo is missing from the dialog");
    // The artwork is scaled down into the space the .ui gives it.
    QVERIFY(logo->pixmap().width() <= logo->width());
    QVERIFY(logo->pixmap().height() <= logo->height());

    QLabel *name = dialog.findChild<QLabel *>(QStringLiteral("label_app_name"));
    QVERIFY(name);
    QCOMPARE(name->text(), QStringLiteral("hashcat-gui"));

    QLabel *description = dialog.findChild<QLabel *>(QStringLiteral("label_app_description"));
    QVERIFY(description);
    QVERIFY(!description->text().isEmpty());
}

int main(int argc, char *argv[])
{
    TestAboutDialog test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_aboutdialog.moc"
