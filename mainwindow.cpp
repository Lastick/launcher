
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

#include <QScrollBar>
#include <QFileDialog>
#include <QInputDialog>
#include "util.h"
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
  this->button_data_dir_obj = this->findChild<QPushButton*>("button_data_dir");
  this->lineEdit_data_dir_obj = this->findChild<QLineEdit*>("lineEdit_data_dir");
  this->lineEdit_wallet_name_obj = this->findChild<QLineEdit*>("lineEdit_wallet_name");
  this->lineEdit_daemon_address_obj = this->findChild<QLineEdit*>("lineEdit_daemon_address");
  this->lineEdit_daemon_port_obj = this->findChild<QLineEdit*>("lineEdit_daemon_port");
  this->lineEdit_rpc_ip_obj = this->findChild<QLineEdit*>("lineEdit_rpc_ip");
  this->lineEdit_rpc_port_obj = this->findChild<QLineEdit*>("lineEdit_rpc_port");
  this->log_area_obj = this->findChild<QTextBrowser*>("log_area");
  this->statusbar_obj = this->findChild<QStatusBar*>("statusbar");

  connect(this->button_start_obj, SIGNAL(clicked()), this, SLOT(button_start()));
  connect(this->button_stop_obj, SIGNAL(clicked()), this, SLOT(button_stop()));
  connect(this->button_data_dir_obj, SIGNAL(clicked()), this, SLOT(button_data_dir()));
  connect(this->lineEdit_data_dir_obj, SIGNAL(textChanged(const QString &)), this, SLOT(lineEdit_data_dir_changed(const QString &)));
  connect(this->lineEdit_data_dir_obj, SIGNAL(editingFinished()), this, SLOT(lineEdit_data_dir_finished()));
  connect(this->lineEdit_wallet_name_obj, SIGNAL(textChanged(const QString &)), this, SLOT(lineEdit_wallet_name_changed(const QString &)));
  connect(this->lineEdit_wallet_name_obj, SIGNAL(editingFinished()), this, SLOT(lineEdit_wallet_name_finished()));
  connect(this->lineEdit_daemon_address_obj, SIGNAL(textChanged(const QString &)), this, SLOT(lineEdit_daemon_address_changed(const QString &)));
  connect(this->lineEdit_daemon_address_obj, SIGNAL(editingFinished()), this, SLOT(lineEdit_daemon_address_finished()));
  connect(this->lineEdit_daemon_port_obj, SIGNAL(textChanged(const QString &)), this, SLOT(lineEdit_daemon_port_changed(const QString &)));
  connect(this->lineEdit_daemon_port_obj, SIGNAL(editingFinished()), this, SLOT(lineEdit_daemon_port_finished()));
  connect(this->lineEdit_rpc_ip_obj, SIGNAL(textChanged(const QString &)), this, SLOT(lineEdit_rpc_ip_changed(const QString &)));
  connect(this->lineEdit_rpc_ip_obj, SIGNAL(editingFinished()), this, SLOT(lineEdit_rpc_ip_finished()));
  connect(this->lineEdit_rpc_port_obj, SIGNAL(textChanged(const QString &)), this, SLOT(lineEdit_rpc_port_changed(const QString &)));
  connect(this->lineEdit_rpc_port_obj, SIGNAL(editingFinished()), this, SLOT(lineEdit_rpc_port_finished()));

  if (!readSettings(this->settings)) {
    loadSettingsDefault(this->settings);
    loadConfigDefault(this->settings.config);
  }
  mkdirDataDir(this->settings.config.data_dir.c_str());

  this->lineEdit_data_dir_obj->setText(QString::fromStdString(this->settings.config.data_dir));
  this->lineEdit_wallet_name_obj->setText(QString::fromStdString(this->settings.config.wallet_name));
  this->lineEdit_daemon_address_obj->setText(QString::fromStdString(this->settings.config.daemon_address));
  this->lineEdit_daemon_port_obj->setText(QString::fromStdString(this->settings.config.daemon_port));
  this->lineEdit_rpc_ip_obj->setText(QString::fromStdString(this->settings.config.rpc_ip));
  this->lineEdit_rpc_port_obj->setText(QString::fromStdString(this->settings.config.rpc_port));

  this->timer = new QTimer();
  connect(timer, SIGNAL(timeout()), this, SLOT(gui_loop()));
  timer->start(500);

  this->lock_update = false;
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::button_start() {
  bool ok;
  QString pass = QInputDialog::getText(this, tr("Enter password for wallet"),
                                       tr("Wallet password:"), QLineEdit::Password,
                                       QString::fromStdString(this->settings.config.wallet_password), &ok);
  if (ok) this->settings.config.wallet_password = pass.toStdString();
  this->worker.start(this->settings);
}

void MainWindow::button_stop() {
  this->worker.stop();
}

void MainWindow::button_data_dir() {
  QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                  this->settings.config.data_dir.c_str(),
                                                  QFileDialog::ShowDirsOnly |
                                                    QFileDialog::DontResolveSymlinks);
  save_data_dir(dir.toStdString().c_str());
}

void MainWindow::lineEdit_data_dir_changed(const QString &text) {
   std::string std_text = text.toStdString();
   // TODO: maybe path validator need anyway
   //inputValidate(std_text);
   this->lineEdit_data_dir_obj->setText(QString::fromStdString(std_text));
}

void MainWindow::lineEdit_data_dir_finished() {
  save_data_dir(this->lineEdit_data_dir_obj->text().toStdString().c_str());
}

void MainWindow::lineEdit_wallet_name_changed(const QString &text) {
   std::string std_text = text.toStdString();
   inputValidate(std_text);
   this->lineEdit_wallet_name_obj->setText(QString::fromStdString(std_text));
}

void MainWindow::lineEdit_wallet_name_finished() {
  // TODO need address checker
  this->settings.config.wallet_name = this->lineEdit_wallet_name_obj->text().toStdString();
  saveSettings(this->settings);
}

void MainWindow::lineEdit_daemon_address_changed(const QString &text) {
  std::string std_text = text.toStdString();
  inputValidate(std_text);
  this->lineEdit_daemon_address_obj->setText(QString::fromStdString(std_text));
}

void MainWindow::lineEdit_daemon_address_finished() {
  this->settings.config.daemon_address = this->lineEdit_daemon_address_obj->text().toStdString();
  saveSettings(this->settings);
}

void MainWindow::lineEdit_daemon_port_changed(const QString &text) {
  std::string std_text = text.toStdString();
  inputValidate(std_text);
  this->lineEdit_daemon_port_obj->setText(QString::fromStdString(std_text));
}

void MainWindow::lineEdit_daemon_port_finished() {
  this->settings.config.daemon_port = this->lineEdit_daemon_port_obj->text().toStdString();
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

void MainWindow::save_data_dir(const char *path) {
  std::string path_str = path;
  QMessageBox msgBox;
  if (path_str.size() > 0) {
    if (mkdirDataDir(path_str.c_str())) {
      this->lineEdit_data_dir_obj->setText(QString::fromStdString(path_str));
      this->settings.config.data_dir = path_str;
      saveSettings(this->settings);
    } else {
      msgBox.setWindowTitle("Error!");
      msgBox.setText("Destination dir is incorrect!");
      msgBox.exec();
    }
  }
}

void MainWindow::gui_loop() {
  std::string status_mess;
  std::string log_mess;
  if (!this->lock_update) {
    this->lock_update = true;
    this->worker.getStatusbarMess(status_mess);
    this->worker.getLogMess(log_mess);
    if (this->log_mess_last.compare(log_mess) != 0) {
      this->log_mess_last = log_mess;
      this->log_area_obj->setText(QString::fromStdString(this->log_mess_last));
      QScrollBar *sb = this->log_area_obj->verticalScrollBar();
      sb->setValue(sb->maximum());
    }
    this->statusbar_obj->showMessage(QString::fromStdString(status_mess));
    this->lock_update = false;
  }
}
