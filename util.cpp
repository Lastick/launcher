
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

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <libgen.h>
#endif
#include "util.h"


bool GetExePath(std::string &path) {
  const size_t PATH_LEN = 1024;
  bool res = false;
  path.clear();
  char native_path[PATH_LEN + 1];
#ifdef _WIN32
  DWORD result = GetModuleFileNameA(nullptr, native_path, PATH_LEN);
  if (result > 0 && result != PATH_LEN) {
    path = std::string(native_path);
    for (size_t i = 0; i < path.size(); i++){
      if (path[i] == '\\') path[i] = '/';
    }
    path = path.substr(0, path.rfind('/'));
    res = true;
  }
#else
#ifdef __FreeBSD__
  // Must be enable procfs
  ssize_t count = readlink("/proc/curproc/file", native_path, PATH_LEN);
#else
  ssize_t count = readlink("/proc/self/exe", native_path, PATH_LEN);
#endif
  const char *path_base;
  if (count != -1) {
    path_base = dirname(native_path);
    path = std::string(path_base);
    res = true;
  }
#endif
  return res;
}

bool IsProcExist(const int pid) {
  bool res = false;

  HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                         FALSE,
                         (DWORD) pid);
  if (h != NULL) {
    res = true;
    CloseHandle(h);
  }

  return res;
}
