/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfileexportui.h"

#include "textutils.h"

#include <QAbstractButton>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QPushButton>

bool PotfileExportUi::isSamePath(const QString &destination, const QString &source)
{
    if (source.isEmpty()) {
        return false;
    }

    const QFileInfo destinationInfo(destination);
    const QFileInfo sourceInfo(source);
    if (destinationInfo.absoluteFilePath() == sourceInfo.absoluteFilePath()) {
        return true;
    }
    return !destinationInfo.canonicalFilePath().isEmpty()
           && destinationInfo.canonicalFilePath() == sourceInfo.canonicalFilePath();
}

bool PotfileExportUi::exportRows(QWidget *parent, const QString &suggestedPath,
                                 const QList<PotfileExporter::Row> &rows, bool includeUsername,
                                 bool includeLine, int totalCount,
                                 const QStringList &protectedPaths)
{
    if (rows.isEmpty()) {
        return false;
    }

    const QString csvFilter = tr("CSV (*.csv)");
    const QString tsvFilter = tr("TSV (*.tsv *.txt)");
    const QString jsonFilter = tr("JSON (*.json)");
    QString selectedFilter;
    QString path = QFileDialog::getSaveFileName(parent, tr("Export potfile results"), suggestedPath,
                                                csvFilter + QStringLiteral(";;") + tsvFilter
                                                    + QStringLiteral(";;") + jsonFilter,
                                                &selectedFilter);
    if (path.isEmpty()) {
        return false;
    }
    const QString chosenPath = path;

    // The selected filter decides the format; without one, the suffix does.
    PotfileExporter::Format format = PotfileExporter::Format::Csv;
    QString filterSuffix = QStringLiteral(".csv");
    QString formatName = QStringLiteral("CSV");
    if (selectedFilter == tsvFilter) {
        format = PotfileExporter::Format::Tsv;
        filterSuffix = QStringLiteral(".tsv");
        formatName = QStringLiteral("TSV");
    } else if (selectedFilter == jsonFilter) {
        format = PotfileExporter::Format::Json;
        filterSuffix = QStringLiteral(".json");
        formatName = QStringLiteral("JSON");
    } else if (selectedFilter != csvFilter) {
        const QString fallback = QFileInfo(path).suffix().toLower();
        if (fallback == QStringLiteral("json")) {
            format = PotfileExporter::Format::Json;
            filterSuffix = QStringLiteral(".json");
            formatName = QStringLiteral("JSON");
        } else if (fallback == QStringLiteral("tsv") || fallback == QStringLiteral("txt")) {
            format = PotfileExporter::Format::Tsv;
            filterSuffix = QStringLiteral(".tsv");
            formatName = QStringLiteral("TSV");
        }
    }

    // Keep a suffix that names the selected format (.txt aliases TSV);
    // otherwise append the filter's suffix so the name matches the content.
    const QString suffix = QFileInfo(path).suffix().toLower();
    const bool suffixMatches =
        (format == PotfileExporter::Format::Csv && suffix == QStringLiteral("csv"))
        || (format == PotfileExporter::Format::Tsv
            && (suffix == QStringLiteral("tsv") || suffix == QStringLiteral("txt")))
        || (format == PotfileExporter::Format::Json && suffix == QStringLiteral("json"));
    if (!suffixMatches) {
        path += filterSuffix;
    }

    // Check the chosen name too: a suffix would hide a picked potfile path.
    for (const QString &protectedPath : protectedPaths) {
        if (isSamePath(chosenPath, protectedPath) || isSamePath(path, protectedPath)) {
            QMessageBox::warning(parent, tr("Export failed"),
                                 tr("Choose a path other than the potfile."));
            return false;
        }
    }

    // An unmatched hashcat.potfile name may still be the live potfile; ask.
    if (QFileInfo(chosenPath)
            .fileName()
            .compare(QStringLiteral("hashcat.potfile"), Qt::CaseInsensitive)
        == 0) {
        const auto answer = QMessageBox::question(
            parent, tr("Export potfile results"),
            tr("The destination is named hashcat.potfile. If this is hashcat’s live potfile, "
               "exporting overwrites it.\n\nExport anyway?"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (answer != QMessageBox::Yes) {
            return false;
        }
    }

    if (path != chosenPath && QFileInfo::exists(path)) {
        // The suffix came from the selected filter, after the file dialog's
        // own overwrite check. The name is literal text: a file name may
        // contain what AutoText would read as markup.
        QMessageBox overwrite(QMessageBox::Question, tr("Export potfile results"),
                              tr("The file already exists:\n%1\n\nOverwrite it?").arg(path),
                              QMessageBox::Yes | QMessageBox::No, parent);
        overwrite.setTextFormat(Qt::PlainText);
        overwrite.setDefaultButton(QMessageBox::No);
        overwrite.setEscapeButton(QMessageBox::No);
        if (overwrite.exec() != QMessageBox::Yes) {
            return false;
        }
    }

    const QString entriesText =
        countText(static_cast<int>(rows.size()), tr("1 entry"), tr("%1 entries"));
    QString text = tr("Export %1 as %2 to:\n%3\n\nThe file will contain plaintext passwords. "
                      "Store it locally and protect it. Spreadsheet applications may "
                      "evaluate cells beginning with =, +, -, or @ as formulas; open the "
                      "file as text or disable formula evaluation.")
                       .arg(entriesText, formatName, path);
    if (totalCount > rows.size()) {
        text += tr("\n\nOnly %1 of %2 entries are included.").arg(rows.size()).arg(totalCount);
    }
    QMessageBox box(QMessageBox::Warning, tr("Export plaintexts"), text, QMessageBox::Cancel,
                    parent);
    box.setTextFormat(Qt::PlainText);
    QAbstractButton *exportButton = box.addButton(tr("Export"), QMessageBox::AcceptRole);
    box.setEscapeButton(QMessageBox::Cancel);
    box.exec();
    if (box.clickedButton() != exportButton) {
        return false;
    }

    QString error;
    if (!PotfileExporter::write(path, rows, format, includeUsername, includeLine, &error)) {
        QMessageBox failed(QMessageBox::Warning, tr("Export failed"), error, QMessageBox::Ok,
                           parent);
        failed.setTextFormat(Qt::PlainText);
        failed.exec();
        return false;
    }

    QMessageBox complete(QMessageBox::Information, tr("Export complete"),
                         tr("Exported %1 to:\n%2").arg(entriesText, path), QMessageBox::Ok, parent);
    complete.setTextFormat(Qt::PlainText);
    complete.exec();
    return true;
}