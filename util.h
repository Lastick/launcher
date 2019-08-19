
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

#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <string>

bool getFileLastMod(const char *path, unsigned int &date);
bool loadLogFile(const char *path, const unsigned int limit_read, std::string &log);
bool mkdirDataDir(const char *path);
bool getUserDataDirectory(std::string &path);
bool getExePath(std::string &path);
bool isProcExist(const int pid, const char *pro_name);

#endif // UTIL_H_INCLUDED
