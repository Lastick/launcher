
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

#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include <string>

struct Config {
  std::string p2p_ip;
  std::string p2p_port;
  std::string p2p_ext_port;
  std::string rpc_ip;
  std::string rpc_port;
};

struct Settings {
  Config config;
  std::string exe_name;
  std::string data_dir;
};

void loadSettingsDefault(Settings &settings);
void loadConfigDefault(Config &config);
bool saveSettings(Settings &settings);
bool readSettings(Settings &settings);

#endif // SETTINGS_H
