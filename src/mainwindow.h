/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QComboBox>
#include <QProcess>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionHelp_About_triggered();

    void on_pushButton_execute_clicked();

    void on_pushButton_open_hashfile_clicked();

    void on_actionReset_fields_triggered();

    void on_actionQuit_triggered();

    void on_pushButton_output_clicked();

    void on_pushButton_remove_wordlist_clicked();

    void on_pushButton_add_wordlist_clicked();

    void on_toolButton_wordlist_sort_asc_clicked();

    void on_toolButton_wordlist_sort_desc_clicked();

    void on_listWidget_wordlist_itemClicked(QListWidgetItem* item);

    void on_checkBox_outfile_toggled(bool checked);

    void on_pushButton_add_wordlist_folder_clicked();

    void CommandChanged(QString arg="");

    void on_comboBox_attack_currentIndexChanged(int index);

    void on_checkBox_custom1_toggled(bool checked);

    void on_checkBox_custom2_toggled(bool checked);

    void on_checkBox_custom3_toggled(bool checked);

    void on_checkBox_custom4_toggled(bool checked);

    void on_lineEdit_open_hashfile_textChanged(const QString &arg1);

    void on_checkBox_rulesfile_1_toggled(bool checked);

    void on_checkBox_rulesfile_2_toggled(bool checked);

    void on_checkBox_rulesfile_3_toggled(bool checked);

    void on_radioButton_generate_rules_toggled(bool checked);

    void on_radioButton_use_rules_file_toggled(bool checked);

    void on_pushButton_open_rulesfile_1_clicked();

    void on_pushButton_open_rulesfile_2_clicked();

    void on_pushButton_open_rulesfile_3_clicked();

    void on_actionAbout_Qt_triggered();

    void on_actionSettings_triggered();

private:
    Ui::MainWindow *ui;

    QMap <quint32, QString> hashModes;
    QMap <quint32, QString> attackModes;

    void init_hash_and_attack_modes();
    void update_view_attack_mode();

    QStringList generate_arguments();

    void add_hash_and_attack_modes(QComboBox *&, QMap <quint32, QString> &);
    void add_wordlist_item(QString &);
    void add_wordlist_item(QStringList &);
    void set_outfile_path();

    // Supported attack modes
    enum AttackMode
    {
        Straight       = 0,
        Combination    = 1,
        BruteForce     = 3,
        HybridWordMask = 6,
        HybridMaskWord = 7,
        Association    = 9
    };
};

#endif // MAINWINDOW_H
