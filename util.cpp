
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

#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDateTime>
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

void convertUTF8toCP1251(std::string &text) {
  int size;
  size = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, 0, 0);
  std::wstring wstr(size, 0);
  MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, &wstr[0], size);
  int codepage = 1251;
  size = WideCharToMultiByte(codepage, 0, &wstr[0], -1, 0, 0, 0, 0);
  std::string str(size, 0);
  WideCharToMultiByte(codepage, 0, &wstr[0], -1, &str[0], size, 0, 0);
  text = str;
}

bool getFileLastMod(const char *path,
                    unsigned int &date) {
  bool res = false;
  date = 0;
  QFileInfo file_info;
  file_info.setFile(QString(path));
  if (file_info.exists()) {
    QDateTime last_mod(file_info.lastModified());
    date = (unsigned int) last_mod.toMSecsSinceEpoch() / 1000;
    res = true;
  }

  return res;
}

bool loadLogFile(const char *path,
                 const unsigned int limit_read,
                 std::string &log) {
  bool res = false;
  std::string log_buff;
  QFile file;
  qint64 file_size = 0;
  unsigned int line_n = 0;
  file.setFileName(QString(path));
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    file_size = file.size();
    if (file_size > 0) {
      file.seek(file_size - 1);
      while (line_n <= limit_read) {
        if (!file.seek(file.pos() - 3)) break;
        if (file.read(1).constData()[0] == 0x0A) line_n++;
      }
      while (!file.atEnd()) {
        log_buff += QString(file.readLine()).toStdString();
      }
    }
    file.close();
  }
  if (log_buff.size() > 0) {
    log.clear();
    log = log_buff;
    res = true;
  }
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
  const size_t ext_size = 4;
  DWORD buff_size = 1024;
  size_t pos_sep = 0;
  size_t str_len = 0;
  char exe_path[1024] = "";
  std::string exe_path_str;
  if (h != NULL) {
    if (QueryFullProcessImageNameA (h, 0, exe_path, &buff_size)) {
      exe_path_str = exe_path;
      pos_sep = exe_path_str.rfind('\\');
      str_len = exe_path_str.size();
      exe_path_str = exe_path_str.substr(pos_sep + 1, str_len - pos_sep - ext_size - 1);
      if (exe_path_str.compare(pro_name) == 0) res = true;
    }
    CloseHandle(h);
  }

  return res;
}
