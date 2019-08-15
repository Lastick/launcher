
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

#include "mainwindow.h"
#include "ui_mainwindow.h"


void inputValidate(std::string &text) {
  const char subs[] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '.', ':', '/'};
  std::string buff;
  if (!text.empty()) {
    for (unsigned int i = 0; i < text.size(); i++) {
      for (unsigned int n = 0; n < sizeof(subs) / sizeof(char); n++) {
        if (text[i] == subs[n]) {
          buff.push_back(subs[n]);
          break;
        }
      }
    }
    text.clear();
    text = buff;
  }
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow) {
  ui->setupUi(this);

  this->button_start_obj = this->findChild<QPushButton*>("button_start");
  this->button_stop_obj = this->findChild<QPushButton*>("button_stop");
  this->lineEdit_p2p_ip_obj = this->findChild<QLineEdit*>("lineEdit_p2p_ip");
  this->lineEdit_p2p_port_obj = this->findChild<QLineEdit*>("lineEdit_p2p_port");
  this->lineEdit_p2p_ext_port_obj = this->findChild<QLineEdit*>("lineEdit_p2p_ext_port");
  this->lineEdit_rpc_ip_obj = this->findChild<QLineEdit*>("lineEdit_rpc_ip");
  this->lineEdit_rpc_port_obj = this->findChild<QLineEdit*>("lineEdit_rpc_port");
  this->statusbar_obj = this->findChild<QStatusBar*>("statusbar");

  connect(this->button_start_obj, SIGNAL(clicked()), this, SLOT(button_start()));
  connect(this->button_stop_obj, SIGNAL(clicked()), this, SLOT(button_stop()));
  connect(this->lineEdit_p2p_ip_obj, SIGNAL(textChanged(const QString &)), this, SLOT(lineEdit_p2p_ip_changed(const QString &)));
  connect(this->lineEdit_p2p_ip_obj, SIGNAL(editingFinished()), this, SLOT(lineEdit_p2p_ip_finished()));
  connect(this->lineEdit_p2p_port_obj, SIGNAL(textChanged(const QString &)), this, SLOT(lineEdit_p2p_port_changed(const QString &)));
  connect(this->lineEdit_p2p_port_obj, SIGNAL(editingFinished()), this, SLOT(lineEdit_p2p_port_finished()));
  connect(this->lineEdit_p2p_ext_port_obj, SIGNAL(textChanged(const QString &)), this, SLOT(lineEdit_p2p_ext_port_changed(const QString &)));
  connect(this->lineEdit_p2p_ext_port_obj, SIGNAL(editingFinished()), this, SLOT(lineEdit_p2p_ext_port_finished()));
  connect(this->lineEdit_rpc_ip_obj, SIGNAL(textChanged(const QString &)), this, SLOT(lineEdit_rpc_ip_changed(const QString &)));
  connect(this->lineEdit_rpc_ip_obj, SIGNAL(editingFinished()), this, SLOT(lineEdit_rpc_ip_finished()));
  connect(this->lineEdit_rpc_port_obj, SIGNAL(textChanged(const QString &)), this, SLOT(lineEdit_rpc_port_changed(const QString &)));
  connect(this->lineEdit_rpc_port_obj, SIGNAL(editingFinished()), this, SLOT(lineEdit_rpc_port_finished()));

  loadSettingsDefault(this->settings);
  loadConfigDefault(this->settings.config);
  readSettings(this->settings);

  this->lineEdit_p2p_ip_obj->setText(QString::fromStdString(this->settings.config.p2p_ip));
  this->lineEdit_p2p_port_obj->setText(QString::fromStdString(this->settings.config.p2p_port));
  this->lineEdit_p2p_ext_port_obj->setText(QString::fromStdString(this->settings.config.p2p_ext_port));
  this->lineEdit_rpc_ip_obj->setText(QString::fromStdString(this->settings.config.rpc_ip));
  this->lineEdit_rpc_port_obj->setText(QString::fromStdString(this->settings.config.rpc_port));

  this->timer = new QTimer();
  connect(timer, SIGNAL(timeout()), this, SLOT(gui_loop()));
  timer->start(500);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::button_start() {
  //QMessageBox msgBox;
  //msgBox.setWindowTitle("MessageBox Title");
  //msgBox.setText("start");
  //msgBox.exec();
  this->worker.start(this->settings);
}

void MainWindow::button_stop() {
  this->worker.stop();
}

void MainWindow::lineEdit_p2p_ip_changed(const QString &text) {
   std::string std_text = text.toStdString();
   inputValidate(std_text);
   this->lineEdit_p2p_ip_obj->setText(QString::fromStdString(std_text));
}

void MainWindow::lineEdit_p2p_ip_finished() {
  //QMessageBox msgBox;
  //msgBox.setWindowTitle("MessageBox Title");
  //msgBox.setText("lineEdit_p2p_ip finished");
  //msgBox.exec();
  this->settings.config.p2p_ip = this->lineEdit_p2p_ip_obj->text().toStdString();
  saveSettings(this->settings);
}

void MainWindow::lineEdit_p2p_port_changed(const QString &text) {
  std::string std_text = text.toStdString();
  inputValidate(std_text);
  this->lineEdit_p2p_port_obj->setText(QString::fromStdString(std_text));
}

void MainWindow::lineEdit_p2p_port_finished() {
  this->settings.config.p2p_port = this->lineEdit_p2p_port_obj->text().toStdString();
  saveSettings(this->settings);
}

void MainWindow::lineEdit_p2p_ext_port_changed(const QString &text) {
  std::string std_text = text.toStdString();
  inputValidate(std_text);
  this->lineEdit_p2p_ext_port_obj->setText(QString::fromStdString(std_text));
}

void MainWindow::lineEdit_p2p_ext_port_finished() {
  this->settings.config.p2p_ext_port = this->lineEdit_p2p_ext_port_obj->text().toStdString();
  saveSettings(this->settings);
}

void MainWindow::lineEdit_rpc_ip_changed(const QString &text) {
  std::string std_text = text.toStdString();
  inputValidate(std_text);
  this->lineEdit_rpc_ip_obj->setText(QString::fromStdString(std_text));
}

void MainWindow::lineEdit_rpc_ip_finished() {
  this->settings.config.rpc_ip = this->lineEdit_rpc_ip_obj->text().toStdString();
  saveSettings(this->settings);
}

void MainWindow::lineEdit_rpc_port_changed(const QString &text) {
  std::string std_text = text.toStdString();
  inputValidate(std_text);
  this->lineEdit_rpc_port_obj->setText(QString::fromStdString(std_text));
}

void MainWindow::lineEdit_rpc_port_finished() {
  this->settings.config.rpc_port = this->lineEdit_rpc_port_obj->text().toStdString();
  saveSettings(this->settings);
}

void MainWindow::gui_loop() {
  std::string mess;
  this->worker.getStatusbarMess(mess);
  this->statusbar_obj->showMessage(QString::fromStdString(mess));
}
