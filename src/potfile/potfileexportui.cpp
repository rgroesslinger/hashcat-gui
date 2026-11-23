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
                                 bool includeLine, int totalCount, const QString &protectedPath)
{
    if (rows.isEmpty()) {
        return false;
    }

    const QString csvFilter = tr("CSV (*.csv)");
    const QString txtFilter = tr("Text (*.txt)");
    const QString jsonFilter = tr("JSON (*.json)");
    QString selectedFilter;
    QString path = QFileDialog::getSaveFileName(parent, tr("Export potfile results"), suggestedPath,
                                                csvFilter + QStringLiteral(";;") + txtFilter
                                                    + QStringLiteral(";;") + jsonFilter,
                                                &selectedFilter);
    if (path.isEmpty()) {
        return false;
    }
    const QString chosenPath = path;

    PotfileExporter::Format format = PotfileExporter::Format::Csv;
    const QString suffix = QFileInfo(path).suffix().toLower();
    if (suffix == QStringLiteral("json")) {
        format = PotfileExporter::Format::Json;
    } else if (suffix == QStringLiteral("txt")) {
        format = PotfileExporter::Format::Txt;
    } else if (suffix == QStringLiteral("csv")) {
        format = PotfileExporter::Format::Csv;
    } else if (selectedFilter == jsonFilter) {
        format = PotfileExporter::Format::Json;
        path += QStringLiteral(".json");
    } else if (selectedFilter == txtFilter) {
        format = PotfileExporter::Format::Txt;
        path += QStringLiteral(".txt");
    } else {
        format = PotfileExporter::Format::Csv;
        if (suffix.isEmpty()) {
            path += QStringLiteral(".csv");
        }
    }

    if (isSamePath(path, protectedPath)) {
        QMessageBox::warning(parent, tr("Export failed"),
                             tr("Choose a path other than the potfile."));
        return false;
    }

    if (path != chosenPath && QFileInfo::exists(path)) {
        // The suffix came from the selected filter, after the file dialog's
        // own overwrite check.
        const auto answer =
            QMessageBox::question(parent, tr("Export potfile results"),
                                  tr("The file already exists:\n%1\n\nOverwrite it?").arg(path),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (answer != QMessageBox::Yes) {
            return false;
        }
    }

    const QString entriesText =
        countText(static_cast<int>(rows.size()), tr("1 entry"), tr("%1 entries"));
    QString text = tr("Export %1 to:\n%2\n\nThe file will contain plaintext passwords. "
                      "Store it locally and protect it. Spreadsheet applications may "
                      "evaluate cells beginning with =, +, -, or @ as formulas; open the "
                      "file as text or disable formula evaluation.")
                       .arg(entriesText, path);
    if (totalCount > rows.size()) {
        text += tr("\n\nOnly the first %1 of %2 entries are retained.")
                    .arg(rows.size())
                    .arg(totalCount);
    }
    // Without a known source potfile the default location cannot be resolved
    // reliably, so warn when the export looks like it is the live potfile.
    if (protectedPath.isEmpty()
        && QFileInfo(path).fileName().compare(QStringLiteral("hashcat.potfile"),
                                              Qt::CaseInsensitive)
               == 0) {
        text += tr("\n\nWarning: this may be hashcat's live potfile. Choose another path.");
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
        QMessageBox::warning(parent, tr("Export failed"), error);
        return false;
    }

    QMessageBox::information(parent, tr("Export complete"),
                             tr("Exported %1 to:\n%2").arg(entriesText, path));
    return true;
}