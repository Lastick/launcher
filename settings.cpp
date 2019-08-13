
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

#include "settings.h"


const char *P2P_DEFAULT_IP = "0.0.0.0";
const char *P2P_DEFAULT_PORT = "32347";
const char *P2P_DEFAULT_EXT_PORT = "0";
const char *RPC_DEFAULT_IP = "127.0.0.1";
const char *RPC_DEFAULT_PORT = "32348";

void loadConfigDefault(Config &config) {
  config.p2p_ip = std::string(P2P_DEFAULT_IP);
  config.p2p_port = std::string(P2P_DEFAULT_PORT);
  config.p2p_ext_port = std::string(P2P_DEFAULT_EXT_PORT);
  config.rpc_ip = std::string(RPC_DEFAULT_IP);
  config.rpc_port = std::string(RPC_DEFAULT_PORT);
}
