
// Copyright (c) 2019 The Karbowanec developers
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Code formatting based on CS106B Style

#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QTextBrowser>
#include <QStatusBar>
#include <QMainWindow>
#include <QTimer>
#include "settings.h"
#include "worker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  private:
    Ui::MainWindow *ui;

    Settings settings;

    QPushButton *button_start_obj;
    QPushButton *button_stop_obj;
    QPushButton *button_data_dir_obj;
    QLineEdit *lineEdit_data_dir_obj;
    QLineEdit *lineEdit_wallet_name_obj;
    QLineEdit *lineEdit_daemon_address_obj;
    QLineEdit *lineEdit_daemon_port_obj;
    QLineEdit *lineEdit_rpc_ip_obj;
    QLineEdit *lineEdit_rpc_port_obj;
    QTextBrowser *log_area_obj;
    QStatusBar *statusbar_obj;

    QTimer *timer;
    Worker worker;

    bool lock_update;
    std::string log_mess_last;

    void save_data_dir(const char *path);

  private slots:
    void button_start();
    void button_stop();
    void button_data_dir();
    void lineEdit_data_dir_changed(const QString &text);
    void lineEdit_data_dir_finished();
    void lineEdit_wallet_name_changed(const QString &text);
    void lineEdit_wallet_name_finished();
    void lineEdit_daemon_address_changed(const QString &text);
    void lineEdit_daemon_address_finished();
    void lineEdit_daemon_port_changed(const QString &text);
    void lineEdit_daemon_port_finished();
    void lineEdit_rpc_ip_changed(const QString &text);
    void lineEdit_rpc_ip_finished();
    void lineEdit_rpc_port_changed(const QString &text);
    void lineEdit_rpc_port_finished();
    void gui_loop();
};

#endif // MAINWINDOW_H_INCLUDED
