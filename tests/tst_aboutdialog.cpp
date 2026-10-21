/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QClipboard>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSysInfo>

#include "aboutdialog.h"
#include "appconstants.h"

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
    void showsTheToolkitAndTheOperatingSystem();
    void linksToTheProjectAndToHashcat();
    void namesTheLicensesOfBothPrograms();
    void copyDetailsPutsThemOnTheClipboard();
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

    dialog.setVersionText(longText);

    // SetFixedSize re-applies the layout's size once the event loop turns.
    QTRY_VERIFY(dialog.height() > heightBefore);
    // The text wrapped onto several lines instead of widening the dialog to
    // the whole one-line string, so it stays on-screen.
    QVERIFY(label->height() > label->fontMetrics().height());
    QVERIFY(dialog.width() < singleLineWidth);
    // Every line fits into the label, so nothing is cut off.
    const QStringList lines = label->text().split(QLatin1Char('\n'));
    for (const QString &line : lines) {
        QVERIFY(label->fontMetrics().horizontalAdvance(line) <= label->width());
    }
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

// The toolkit and the system underneath it are part of what a bug report
// needs, so the dialog states both.
void TestAboutDialog::showsTheToolkitAndTheOperatingSystem()
{
    AboutDialog dialog;

    QLabel *qtVersion = dialog.findChild<QLabel *>(QStringLiteral("label_qt_version"));
    QVERIFY(qtVersion);
    QCOMPARE(qtVersion->text(), QString::fromLatin1(qVersion()));

    QLabel *os = dialog.findChild<QLabel *>(QStringLiteral("label_os"));
    QVERIFY(os);
    QCOMPARE(os->text(), QSysInfo::prettyProductName());
}

// The links are the way out of the dialog: to the source of the application,
// to the place where a problem can be reported and to hashcat itself. All of
// them have to be there, and the tooltip of a link says where it leads.
void TestAboutDialog::linksToTheProjectAndToHashcat()
{
    AboutDialog dialog;

    const QStringList names{
        QStringLiteral("label_link_project"),
        QStringLiteral("label_link_issue"),
        QStringLiteral("label_link_hashcat"),
        QStringLiteral("label_link_hashcat_faq"),
    };
    const QStringList urls{
        QString(AppConstants::Urls::Project),
        QString(AppConstants::Urls::Issues),
        QString(AppConstants::Urls::Hashcat),
        QString(AppConstants::Urls::HashcatFaq),
    };

    for (int i = 0; i < names.size(); ++i) {
        QLabel *label = dialog.findChild<QLabel *>(names.at(i));
        QVERIFY(label);
        QVERIFY(label->openExternalLinks());
        QVERIFY(label->text().contains(urls.at(i)));
        // The tooltip says where the link leads.
        QCOMPARE(label->toolTip(), urls.at(i));
    }
}

// Both programs are free software, and both say so in their own license.
// The dialog has to name them, and it has to link the texts.
void TestAboutDialog::namesTheLicensesOfBothPrograms()
{
    AboutDialog dialog;

    QLabel *gui = dialog.findChild<QLabel *>(QStringLiteral("label_license_gui"));
    QVERIFY(gui);
    QVERIFY(gui->openExternalLinks());
    QVERIFY(gui->text().contains(QStringLiteral("GPL")));
    QVERIFY(gui->text().contains(QString(AppConstants::Urls::License)));
    QCOMPARE(gui->toolTip(), QString(AppConstants::Urls::License));

    QLabel *hashcat = dialog.findChild<QLabel *>(QStringLiteral("label_license_hashcat"));
    QVERIFY(hashcat);
    QVERIFY(hashcat->openExternalLinks());
    QVERIFY(hashcat->text().contains(QStringLiteral("MIT")));
    QCOMPARE(hashcat->toolTip(), QString(AppConstants::Urls::HashcatLicense));

    // Both sentences have the same shape, and neither of them names the
    // license a second time.
    QVERIFY(gui->text().contains(QStringLiteral(">GPL</a>")));
    QVERIFY(hashcat->text().contains(QStringLiteral(">MIT</a>")));
    QVERIFY(gui->text().contains(QStringLiteral("licensed under the")));
    QVERIFY(hashcat->text().contains(QStringLiteral("licensed under the")));
}

// The details a bug report needs are one click away: the version of the
// application, of the toolkit and of the system, and the hashcat in use.
void TestAboutDialog::copyDetailsPutsThemOnTheClipboard()
{
    AboutDialog dialog;

    QPushButton *button =
        dialog.findChild<QPushButton *>(QStringLiteral("pushButton_copy_details"));
    QVERIFY(button);
    button->click();

    QClipboard *clipboard = QApplication::clipboard();
    QVERIFY(clipboard);

    const QString text = clipboard->text();
    QVERIFY(text.contains(QApplication::applicationVersion()));
    QVERIFY(text.contains(QString::fromLatin1(qVersion())));
    QVERIFY(text.contains(QSysInfo::prettyProductName()));
    // One line per fact, so that a report can quote them one by one.
    QCOMPARE(text.count(QLatin1Char('\n')), 3);
}

int main(int argc, char *argv[])
{
    TestAboutDialog test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_aboutdialog.moc"
