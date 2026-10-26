/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QClipboard>
#include <QGroupBox>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QSysInfo>

#include "aboutdialog.h"
#include "appconstants.h"
#include "settingsmanager.h"
#include "updatechecker.h"

namespace
{
// A checker that answers without a network. The result is emitted from
// check(), so a test can inspect both the busy state before it and the answer
// after it.
class FakeUpdateChecker : public UpdateChecker
{
public:
    UpdateCheckResult result;
    int checks = 0;
    bool answers = false;

    void check() override
    {
        ++checks;
        if (answers) {
            emit finished(result);
        }
    }
};
} // namespace

// The About dialog opens as large as its content: the size in the .ui would
// clip the content under a large font or a longer version string and leave an
// empty margin under a small one. The version label wraps, so a long hashcat
// error grows the dialog instead of pushing it off the screen.
class TestAboutDialog : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void fitsItsContent();
    void longVersionTextWrapsInsteadOfWidening();
    void groupBoxesShareOneTitleAlignment();
    void showsTheLogoAndTheApplicationName();
    void versionLabelShowsPlainText();
    void showsTheToolkitAndTheOperatingSystem();
    void linksToTheProjectAndToHashcat();
    void namesTheLicenseOfTheApplication();
    void copyDetailsPutsThemOnTheClipboard();
    void okAcceptsTheDialog();
    void saysThatNoHashcatIsConfigured();
    void updateCheckShowsProgressBeforeTheResult();
    void updateCheckReportsNewerVersion();
    void updateCheckReportsLatestVersion();
    void updateCheckReportsFailure();
    void closingBeforeTheDelaySkipsTheCheck();
    void updateCheckRunsOnlyOnce();
};

void TestAboutDialog::initTestCase()
{
    QCoreApplication::setApplicationVersion(QStringLiteral("0.8.0"));
    qRegisterMetaType<UpdateCheckResult>("UpdateCheckResult");
}

void TestAboutDialog::fitsItsContent()
{
    FakeUpdateChecker checker;
    AboutDialog dialog(&checker, 60000);
    dialog.show();
    QCoreApplication::processEvents();

    QCOMPARE(dialog.size(), dialog.sizeHint());

    // The user may make the dialog whatever size they like afterwards.
    QVERIFY(dialog.minimumSize() != dialog.maximumSize());
}

void TestAboutDialog::longVersionTextWrapsInsteadOfWidening()
{
    FakeUpdateChecker checker;
    AboutDialog dialog(&checker, 60000);
    dialog.show();
    QCoreApplication::processEvents();

    QLabel *label = dialog.findChild<QLabel *>(QStringLiteral("label_hc_version"));
    QVERIFY(label);

    const QString longText = QStringLiteral("Error: ") + QString(300, QLatin1Char('x'));
    const int singleLineWidth = label->fontMetrics().horizontalAdvance(longText);
    const int heightBefore = dialog.height();

    dialog.setVersionText(longText);

    // The layout grows the dialog once the new text has reached the label.
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
// above the frame while its neighbors break the border with it.
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
    const QPixmap pixmap = logo->pixmap();
    QVERIFY2(!pixmap.isNull(), "the logo is missing from the dialog");
    // The artwork is scaled down into the space the .ui gives it, and the
    // pixmap carries the ratio so that it stays sharp on a high-dpi display.
    QVERIFY(pixmap.deviceIndependentSize().width() <= logo->width());
    QVERIFY(pixmap.deviceIndependentSize().height() <= logo->height());
    QCOMPARE(pixmap.devicePixelRatio(), dialog.devicePixelRatioF());

    QLabel *name = dialog.findChild<QLabel *>(QStringLiteral("label_app_name"));
    QVERIFY(name);
    QCOMPARE(name->text(), QStringLiteral("hashcat-gui"));

    QLabel *description = dialog.findChild<QLabel *>(QStringLiteral("label_app_description"));
    QVERIFY(description);
    QVERIFY(!description->text().isEmpty());
}

// hashcat's answer is not markup: a message with "<" in it is shown as typed
// instead of disappearing as if it were a tag.
void TestAboutDialog::versionLabelShowsPlainText()
{
    AboutDialog dialog;

    QLabel *version = dialog.findChild<QLabel *>(QStringLiteral("label_hc_version"));
    QVERIFY(version);
    QCOMPARE(version->textFormat(), Qt::PlainText);
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

// The dialog names the license of the application and links its text.
//
// The license of hashcat is not part of it: the application does not
// distribute hashcat, so it does not state its terms either.
void TestAboutDialog::namesTheLicenseOfTheApplication()
{
    AboutDialog dialog;

    QLabel *gui = dialog.findChild<QLabel *>(QStringLiteral("label_license_gui"));
    QVERIFY(gui);
    QVERIFY(gui->openExternalLinks());
    QVERIFY(gui->text().contains(QStringLiteral("GNU GPL")));
    QVERIFY(gui->text().contains(QStringLiteral("v3.0 or later")));
    QVERIFY(gui->text().contains(QString(AppConstants::Urls::License)));
    QCOMPARE(gui->toolTip(), QString(AppConstants::Urls::License));

    // The name of the license is the link, and nothing else is named.
    QVERIFY(gui->text().contains(QStringLiteral(">GNU GPL v3.0 or later</a>")));

    // The license of hashcat is nowhere in the dialog.
    QVERIFY(dialog.findChild<QLabel *>(QStringLiteral("label_license_hashcat")) == nullptr);
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

// OK is the response of the dialog: it is the default button, so Enter closes
// it, and the dialog accepts rather than rejects.
void TestAboutDialog::okAcceptsTheDialog()
{
    AboutDialog dialog;

    QPushButton *ok = dialog.findChild<QPushButton *>(QStringLiteral("pushButton_ok"));
    QVERIFY(ok);
    QVERIFY2(ok->isDefault(), "Enter does not press OK");
    QCOMPARE(ok->text(), QStringLiteral("OK"));

    ok->click();
    QCOMPARE(dialog.result(), int(QDialog::Accepted));
}

// Without a hashcat there is no version to show. The label says so instead of
// keeping the placeholder of the .ui file, and the details of a bug report
// say the same.
void TestAboutDialog::saysThatNoHashcatIsConfigured()
{
    SettingsManager::instance().setKey(AppConstants::SettingsKeys::HashcatPath, QString());

    AboutDialog dialog;

    QLabel *label = dialog.findChild<QLabel *>(QStringLiteral("label_hc_version"));
    QVERIFY(label);
    QCOMPARE(label->text(), QStringLiteral("not configured"));
}

// The update row starts busy: the progress indicator and the checking label
// are visible, the answer and the release link are not.
void TestAboutDialog::updateCheckShowsProgressBeforeTheResult()
{
    FakeUpdateChecker checker;
    AboutDialog dialog(&checker, 60000);
    dialog.show();
    QCoreApplication::processEvents();

    QLabel *checking = dialog.findChild<QLabel *>(QStringLiteral("label_update_checking"));
    QProgressBar *progress = dialog.findChild<QProgressBar *>(QStringLiteral("progressBar_update"));
    QLabel *result = dialog.findChild<QLabel *>(QStringLiteral("label_update_result"));
    QLabel *link = dialog.findChild<QLabel *>(QStringLiteral("label_update_link"));
    QVERIFY(checking);
    QVERIFY(progress);
    QVERIFY(result);
    QVERIFY(link);

    QVERIFY(checking->isVisible());
    QVERIFY(progress->isVisible());
    QCOMPARE(progress->minimum(), 0);
    QCOMPARE(progress->maximum(), 0);
    // A hairline, not a bar that spans the row: the check is a footnote of
    // the dialog.
    QVERIFY(progress->maximumHeight() < progress->fontMetrics().height());
    QVERIFY(progress->maximumWidth() < progress->parentWidget()->width());
    QVERIFY(!result->isVisible());
    QVERIFY(!link->isVisible());
    QCOMPARE(checker.checks, 0);
}

void TestAboutDialog::updateCheckReportsNewerVersion()
{
    FakeUpdateChecker checker;
    checker.answers = true;
    checker.result.status = UpdateCheckResult::Status::UpdateAvailable;
    checker.result.latestVersion = QStringLiteral("0.9.0");

    AboutDialog dialog(&checker, 0);
    dialog.show();

    QTRY_COMPARE(checker.checks, 1);

    QLabel *checking = dialog.findChild<QLabel *>(QStringLiteral("label_update_checking"));
    QProgressBar *progress = dialog.findChild<QProgressBar *>(QStringLiteral("progressBar_update"));
    QLabel *result = dialog.findChild<QLabel *>(QStringLiteral("label_update_result"));
    QLabel *link = dialog.findChild<QLabel *>(QStringLiteral("label_update_link"));
    QVERIFY(checking);
    QVERIFY(progress);
    QVERIFY(result);
    QVERIFY(link);

    QTRY_VERIFY(!checking->isVisible());
    QVERIFY(!progress->isVisible());
    QVERIFY(result->isVisible());
    QVERIFY(result->text().contains(QStringLiteral("0.9.0")));
    // The version of the running application is one row above; repeating it
    // here would only make the line longer.
    QVERIFY(!result->text().contains(QStringLiteral("0.8.0")));
    QVERIFY(link->isVisible());
    QVERIFY(link->openExternalLinks());
    QVERIFY(link->text().contains(QString(AppConstants::Urls::LatestReleasePage)));
    QCOMPARE(link->toolTip(), QString(AppConstants::Urls::LatestReleasePage));
}

void TestAboutDialog::updateCheckReportsLatestVersion()
{
    FakeUpdateChecker checker;
    checker.answers = true;
    checker.result.status = UpdateCheckResult::Status::UpToDate;
    checker.result.latestVersion = QStringLiteral("0.8.0");

    AboutDialog dialog(&checker, 0);
    dialog.show();

    QTRY_COMPARE(checker.checks, 1);

    QProgressBar *progress = dialog.findChild<QProgressBar *>(QStringLiteral("progressBar_update"));
    QLabel *result = dialog.findChild<QLabel *>(QStringLiteral("label_update_result"));
    QLabel *link = dialog.findChild<QLabel *>(QStringLiteral("label_update_link"));
    QVERIFY(progress);
    QVERIFY(result);
    QVERIFY(link);

    QTRY_VERIFY(result->isVisible());
    QVERIFY(!progress->isVisible());
    // Direct, and without the version the rows above already show.
    QCOMPARE(result->text(), QStringLiteral("No updates available."));
    QVERIFY(!link->isVisible());
}

void TestAboutDialog::updateCheckReportsFailure()
{
    FakeUpdateChecker checker;
    checker.answers = true;
    checker.result.status = UpdateCheckResult::Status::Failed;
    checker.result.detail = QStringLiteral("network unreachable");

    AboutDialog dialog(&checker, 0);
    dialog.show();

    QTRY_COMPARE(checker.checks, 1);

    QLabel *result = dialog.findChild<QLabel *>(QStringLiteral("label_update_result"));
    QLabel *link = dialog.findChild<QLabel *>(QStringLiteral("label_update_link"));
    QVERIFY(result);
    QVERIFY(link);

    QTRY_VERIFY(result->isVisible());
    QVERIFY(result->text().contains(QStringLiteral("network unreachable")));
    QVERIFY(!link->isVisible());
}

// An About dialog that is closed before the delay must not leave a request
// behind.
void TestAboutDialog::closingBeforeTheDelaySkipsTheCheck()
{
    FakeUpdateChecker checker;
    {
        AboutDialog dialog(&checker, 200);
        dialog.show();
        QCoreApplication::processEvents();
        QCOMPARE(checker.checks, 0);
        dialog.close();
    }

    QTest::qWait(300);
    QCOMPARE(checker.checks, 0);
}

// Showing the same dialog again does not ask GitHub a second time.
void TestAboutDialog::updateCheckRunsOnlyOnce()
{
    FakeUpdateChecker checker;
    AboutDialog dialog(&checker, 0);
    dialog.show();
    QTRY_COMPARE(checker.checks, 1);

    dialog.hide();
    dialog.show();
    QTest::qWait(100);
    QCOMPARE(checker.checks, 1);
}

int main(int argc, char *argv[])
{
    TestAboutDialog test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/true);
}

#include "tst_aboutdialog.moc"
