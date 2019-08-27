
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

#include <QSettings>
#include "util.h"
#include "settings.h"


const char *exe_name = "simplewallet";
const char *dir_name = "karbowallet";

const char *WALLET_NAME_DEFAULT = "Main";
const char *DAEMON_ADDRESS_DEFAULT = "127.0.0.1";
const char *DAEMON_PORT_DEFAULT = "32348";
const char *RPC_DEFAULT_IP = "127.0.0.1";
const char *RPC_DEFAULT_PORT = "15000";

void loadConfigDefault(Config &config) {
  config.wallet_name = std::string(WALLET_NAME_DEFAULT);
  config.daemon_address = std::string(DAEMON_ADDRESS_DEFAULT);
  config.daemon_port = std::string(DAEMON_PORT_DEFAULT);
  config.rpc_ip = std::string(RPC_DEFAULT_IP);
  config.rpc_port = std::string(RPC_DEFAULT_PORT);
}

void loadSettingsDefault(Settings &settings) {
  std::string user_data_dir;
  getUserDataDirectory(user_data_dir);
  settings.exe_name = exe_name;
  settings.config.data_dir = user_data_dir + std::string("/") + std::string(dir_name);
}

bool saveSettings(Settings &settings) {

  QSettings settings_store;

  settings_store.setValue(QString(exe_name) + "/exe_name",
                          QString::fromStdString(settings.exe_name));

  settings_store.setValue(QString(exe_name) + "/data_dir",
                          QString::fromStdString(settings.config.data_dir));

  settings_store.setValue(QString(exe_name) + "/wallet_name",
                          QString::fromStdString(settings.config.wallet_name));

  settings_store.setValue(QString(exe_name) + "/daemon_address",
                          QString::fromStdString(settings.config.daemon_address));

  settings_store.setValue(QString(exe_name) + "/daemon_port",
                          QString::fromStdString(settings.config.daemon_port));

  settings_store.setValue(QString(exe_name) + "/rpc_ip",
                          QString::fromStdString(settings.config.rpc_ip));

  settings_store.setValue(QString(exe_name) + "/rpc_port",
                          QString::fromStdString(settings.config.rpc_port));

  return true;
}

bool readSettings(Settings &settings) {
  bool res = false;

  QSettings settings_store;

  if (settings_store.contains(QString(exe_name) + "/exe_name") &&
      settings_store.contains(QString(exe_name) + "/data_dir") &&
      settings_store.contains(QString(exe_name) + "/wallet_name") &&
      settings_store.contains(QString(exe_name) + "/daemon_address") &&
      settings_store.contains(QString(exe_name) + "/daemon_port") &&
      settings_store.contains(QString(exe_name) + "/rpc_ip") &&
      settings_store.contains(QString(exe_name) + "/rpc_port")) {

      settings.exe_name = settings_store.value(QString(exe_name) + "/exe_name").toString().toStdString();
      settings.config.data_dir = settings_store.value(QString(exe_name) + "/data_dir").toString().toStdString();
      settings.config.wallet_name = settings_store.value(QString(exe_name) + "/wallet_name").toString().toStdString();
      settings.config.daemon_address = settings_store.value(QString(exe_name) + "/daemon_address").toString().toStdString();
      settings.config.daemon_port = settings_store.value(QString(exe_name) + "/daemon_port").toString().toStdString();
      settings.config.rpc_ip = settings_store.value(QString(exe_name) + "/rpc_ip").toString().toStdString();
      settings.config.rpc_port = settings_store.value(QString(exe_name) + "/rpc_port").toString().toStdString();
    res = true;
  }

  return res;
}
