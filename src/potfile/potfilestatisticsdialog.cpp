/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "potfilestatisticsdialog.h"

#include "textutils.h"
#include "ui_potfilestatisticsdialog.h"

#include <QFont>
#include <QHeaderView>
#include <QPushButton>
#include <QStringList>
#include <QTreeWidgetItem>

PotfileStatisticsDialog::PotfileStatisticsDialog(QWidget *parent,
                                                 const PotfileStatistics::Result &statistics,
                                                 const QString &context, int totalCount)
    : QDialog(parent), ui(new Ui::PotfileStatisticsDialog)
{
    ui->setupUi(this);

    QFont contextFont = ui->label_context->font();
    contextFont.setBold(true);
    ui->label_context->setFont(contextFont);
    ui->label_context->setTextFormat(Qt::PlainText);
    ui->label_summary->setTextFormat(Qt::PlainText);
    ui->label_classes->setTextFormat(Qt::PlainText);
    ui->label_context->setText(context.isEmpty() ? tr("Password statistics") : context);

    const QString entryText = countText(statistics.entries, tr("1 entry"), tr("%1 entries"));
    const QString analyzedText =
        countText(statistics.analyzed, tr("1 password analyzed"), tr("%1 passwords analyzed"));
    QString summary = tr("%1 · %2").arg(entryText, analyzedText);
    summary += QStringLiteral("\n")
               + tr("%1 empty · %2 encrypted · %3 duplicate lines · %4 unique plaintexts")
                     .arg(statistics.empty)
                     .arg(statistics.encrypted)
                     .arg(statistics.duplicate)
                     .arg(statistics.unique);
    summary += QStringLiteral("\n")
               + tr("Length: min %1 · max %2 · average %3")
                     .arg(statistics.minLength)
                     .arg(statistics.maxLength)
                     .arg(QString::number(statistics.averageLength, 'f', 1));
    if (totalCount > statistics.entries) {
        summary +=
            QStringLiteral("\n")
            + tr("Based on the retained %1 of %2 entries.").arg(statistics.entries).arg(totalCount);
    }
    ui->label_summary->setText(summary);

    if (statistics.analyzed == 0) {
        ui->label_classes->setText(tr("No plaintext passwords to analyze."));
    } else {
        const auto percent = [&statistics](int count) {
            return QString::number(100.0 * count / statistics.analyzed, 'f', 0);
        };
        ui->label_classes->setText(tr("Passwords containing:\n"
                                      "lowercase: %1 (%2%)\n"
                                      "uppercase: %3 (%4%)\n"
                                      "digits: %5 (%6%)\n"
                                      "symbols: %7 (%8%)\n"
                                      "whitespace: %9 (%10%)\n"
                                      "non-ASCII: %11 (%12%)")
                                       .arg(statistics.classes.lowercase)
                                       .arg(percent(statistics.classes.lowercase))
                                       .arg(statistics.classes.uppercase)
                                       .arg(percent(statistics.classes.uppercase))
                                       .arg(statistics.classes.digits)
                                       .arg(percent(statistics.classes.digits))
                                       .arg(statistics.classes.symbols)
                                       .arg(percent(statistics.classes.symbols))
                                       .arg(statistics.classes.whitespace)
                                       .arg(percent(statistics.classes.whitespace))
                                       .arg(statistics.classes.nonAscii)
                                       .arg(percent(statistics.classes.nonAscii)));
    }

    ui->treeWidget_lengths->clear();
    for (auto it = statistics.lengthHistogram.cbegin(); it != statistics.lengthHistogram.cend();
         ++it) {
        auto *item = new QTreeWidgetItem(
            QStringList{QString::number(it.key()), QString::number(it.value())});
        item->setTextAlignment(1, Qt::AlignRight);
        ui->treeWidget_lengths->addTopLevelItem(item);
    }
    ui->treeWidget_lengths->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeWidget_lengths->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    connect(ui->pushButton_close, &QPushButton::clicked, this, &QDialog::accept);
}

PotfileStatisticsDialog::~PotfileStatisticsDialog()
{
    delete ui;
}