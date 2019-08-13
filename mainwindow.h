
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
#include <QMainWindow>
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
    QLineEdit *lineEdit_p2p_ip_obj;
    QLineEdit *lineEdit_p2p_port_obj;
    QLineEdit *lineEdit_p2p_ext_port_obj;
    QLineEdit *lineEdit_rpc_ip_obj;
    QLineEdit *lineEdit_rpc_port_obj;

    Worker worker;

  private slots:
    void button_start();
    void button_stop();
    void lineEdit_p2p_ip_changed(const QString &text);
    void lineEdit_p2p_ip_finished();
    void lineEdit_p2p_port_changed(const QString &text);
    void lineEdit_p2p_port_finished();
    void lineEdit_p2p_ext_port_changed(const QString &text);
    void lineEdit_p2p_ext_port_finished();
    void lineEdit_rpc_ip_changed(const QString &text);
    void lineEdit_rpc_ip_finished();
    void lineEdit_rpc_port_changed(const QString &text);
    void lineEdit_rpc_port_finished();
};

#endif // MAINWINDOW_H_INCLUDED
