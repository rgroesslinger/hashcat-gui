/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "testenvironment.h"

#include <QDir>
#include <QFile>

#include "appconstants.h"

// What main() loads from and what the settings dialog offers have to be the
// same thing: the catalogs are inside the binary - not files sitting next to
// it that only a developer's build tree has - and neither list may know more
// than the other.
class TestTranslations : public QObject
{
    Q_OBJECT

private slots:
    void germanCatalogIsEmbedded();
    void serbianCatalogIsEmbedded();
    void everyOfferedLanguageIsEmbedded();
    void everyEmbeddedCatalogIsOffered();
};

// One check per catalog: a single one would pass while the other language
// silently fell out of the build.
void TestTranslations::germanCatalogIsEmbedded()
{
#ifndef HASHCAT_GUI_QM_EMBEDDED
    QSKIP("built without Qt6 LinguistTools - there are no catalogs to embed");
#endif
    QVERIFY(QFile::exists(QStringLiteral(":/i18n/hashcat-gui_de.qm")));
}

void TestTranslations::serbianCatalogIsEmbedded()
{
#ifndef HASHCAT_GUI_QM_EMBEDDED
    QSKIP("built without Qt6 LinguistTools - there are no catalogs to embed");
#endif
    QVERIFY(QFile::exists(QStringLiteral(":/i18n/hashcat-gui_sr.qm")));
}

// Every language the settings dialog offers must really arrive - a code
// without a catalog would be a combo entry whose selection changes nothing.
void TestTranslations::everyOfferedLanguageIsEmbedded()
{
#ifndef HASHCAT_GUI_QM_EMBEDDED
    QSKIP("built without Qt6 LinguistTools - there are no catalogs to embed");
#endif
    for (const auto &language : AppConstants::Languages::Available) {
        const QString code = QString::fromUtf8(language.code);
        // English is the source language and deliberately has no catalog:
        // nothing answers for an English locale, so main() stays on the
        // source strings - which are English.
        if (code == QStringLiteral("en")) {
            continue;
        }
        QVERIFY2(QFile::exists(QStringLiteral(":/i18n/hashcat-gui_%1.qm").arg(code)),
                 qPrintable(QStringLiteral("the offered language \"%1\" has no catalog")
                                .arg(code)));
    }
}

// ... and the other way round: a catalog that the settings dialog does not
// offer is dead weight the dialog forgot about.
void TestTranslations::everyEmbeddedCatalogIsOffered()
{
#ifndef HASHCAT_GUI_QM_EMBEDDED
    QSKIP("built without Qt6 LinguistTools - there are no catalogs to embed");
#endif
    const QString prefix = QStringLiteral("hashcat-gui_");
    const QStringList catalogs =
        QDir(QStringLiteral(":/i18n")).entryList({prefix + QStringLiteral("*.qm")});
    QVERIFY2(!catalogs.isEmpty(), "no catalogs are embedded at all");

    for (const QString &catalog : catalogs) {
        QString code = catalog.mid(prefix.size());
        code.chop(3); // ".qm"

        bool offered = false;
        for (const auto &language : AppConstants::Languages::Available) {
            if (code == QString::fromUtf8(language.code)) {
                offered = true;
                break;
            }
        }
        QVERIFY2(offered,
                 qPrintable(QStringLiteral("the catalog \"%1\" is not offered by the "
                                           "settings dialog")
                                .arg(catalog)));
    }
}

int main(int argc, char *argv[])
{
    TestTranslations test;
    return TestEnvironment::run(&test, argc, argv, /*needsGui=*/false);
}

#include "tst_translations.moc"
