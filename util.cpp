
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

#include <QDir>
#include <QFile>
#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#else
#include <unistd.h>
#include <libgen.h>
#endif
#include "util.h"


void toNativeSeparator(std::string &path) {
  const char native_sep = '/';
  const char replace_sep = '\\';
  if (path.size() > 0) {
    for (size_t i = 0; i < path.size(); i++) {
      if (path[i] == replace_sep) path[i] = native_sep;
    }
  }
}

bool loadLogFile(const char *path,
                 const unsigned int limit_read,
                 std::string &log) {
  bool res = false;
  log.clear();
  QFile file;
  std::string log_buff;
  unsigned int line_n = 0;
  unsigned int line_volume = 0;
  file.setFileName(QString(path));
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    log_buff = QString(file.readAll()).toStdString();
    file.close();
    for (size_t i = 0; i < log_buff.size(); i++) {
      if (log_buff[i] == 0x0A) line_n++;
    }
    line_volume = line_n;
    line_n = 0;
    for (size_t i = 0; i < log_buff.size(); i++) {
      if (log_buff[i] == 0x0A) line_n++;
      if (limit_read > line_volume || line_volume - limit_read < line_n) {
        log.push_back(log_buff[i]);
      }
    }
  }
  if (log.size() > 0) res = true;
  return res;
}

bool mkdirDataDir(const char *path) {
  bool res = false;
  if (!QDir(QString(path)).exists()) {
    if (QDir().mkdir(QString(path))) res = true;
  } else {
    res = true;
  }
  return res;
}

bool getUserDataDirectory(std::string &path) {
  bool res = false;
  path.clear();

  char psz_path[MAX_PATH] = "";
  if (SHGetSpecialFolderPathA(NULL, psz_path, CSIDL_APPDATA, true)) {
    path = psz_path;
  }

  if (path.size() > 0) {
    toNativeSeparator(path);
    res = true;
  }
  return res;
}

bool getExePath(std::string &path) {
  const size_t PATH_LEN = 1024;
  bool res = false;
  path.clear();
  char native_path[PATH_LEN + 1];
#ifdef _WIN32
  DWORD result = GetModuleFileNameA(nullptr, native_path, PATH_LEN);
  if (result > 0 && result != PATH_LEN) {
    path = std::string(native_path);
    toNativeSeparator(path);
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

bool isProcExist(const int pid,
                 const char *pro_name) {
  bool res = false;

  HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                         FALSE,
                         (DWORD) pid);
  DWORD buff_size = 1024;
  size_t pos_n = 0;
  size_t str_len = 0;
  char exe_path[1024];
  std::string exe_path_str;
  if (h != NULL) {
    if (QueryFullProcessImageNameA (h, 0, exe_path, &buff_size)) {
      exe_path_str = exe_path;
      pos_n = exe_path_str.rfind('\\');
      str_len = exe_path_str.size();
      exe_path_str = exe_path_str.substr(pos_n + 1, str_len - pos_n - 5);
      if (exe_path_str.compare(pro_name) == 0) res = true;
    }
    CloseHandle(h);
  }

  return res;
}
