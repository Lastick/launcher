
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

#include <thread>
#include <iostream>
#include "sleep.h"
#include "util.h"
#include "worker.h"


const char *name_exe = "karbowanecd";

bool starter_win(const char *args,
                 int &pid) {
  bool res = false;
  pid = 0;

  std::string exe_dir;
  std::string exe_path;

  GetExePath(exe_dir);
  exe_path = exe_dir + std::string("/") + std::string(name_exe) + std::string(".exe");

  STARTUPINFOA si;
  PROCESS_INFORMATION pi;

  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  SetConsoleCtrlHandler(NULL, false);

  if (CreateProcessA(exe_path.c_str(),
                     const_cast<LPSTR>(args),
                     NULL,
                     NULL,
                     FALSE,
                     CREATE_NEW_CONSOLE,
                     NULL,
                     NULL,
                     &si,
                     &pi)) res = true;

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);

  if (res) pid = (int) pi.dwProcessId;

  return res;
}

bool stopper_win(const int pid) {
  bool res = false;

  if (AttachConsole((DWORD) pid) != FALSE) {
    SetConsoleCtrlHandler(nullptr, true);
    if (GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0) != NULL) res = true;
    FreeConsole();
  }

  return res;
}

void genProcArgs(const Config config,
                 std::string &args) {
  args.clear();
  args += std::string(" --p2p-bind-ip \"") + config.p2p_ip + std::string("\"");
  args += std::string(" --p2p-bind-port " + config.p2p_port);
  args += std::string(" --p2p-external-port " + config.p2p_ext_port);
  args += std::string(" --rpc-bind-ip \"") + config.rpc_ip + std::string("\"");
  args += std::string(" --rpc-bind-port " + config.rpc_port);
}

void waiter(bool &flag){
  while (flag){
    Sleep(100);
  }
}

const unsigned int Worker::pcr_timeout = 10;
const unsigned int Worker::loop_interval = 1000;

Worker::Worker() {
  this->status = false;
  this->run = true;
  this->is_run = true;
  this->is_stop = true;
  this->be_start = false;
  this->be_stop = false;
  this->be_lock = false;
  this->proc_is_run = false;
  this->pcr_t = false;
  this->pcr_fail = false;
  this->pid = 0;
  this->pcr_n = 0;
  this->proc_args = "";
  std::thread th(Worker::init_loop, this);
  th.detach();
  waiter(this->is_run);
}

Worker::~Worker() {
  this->exit();
}

void Worker::loop() {
  while(this->run) {
    this->is_run = false;
    this->processor();
    Sleep(Worker::loop_interval);
  }
  this->is_stop = false;
  std::cout << "exit" << std::endl;
}

void *Worker::init_loop(void *vptr_args) {
  ((Worker *) vptr_args)->loop();
  return NULL;
}

void Worker::processor() {

  if (this->be_lock && !this->pcr_t) {
    if (this->be_start && this->proc_is_run) {
      this->be_start = false;
      this->be_lock = false;
    }
    if (this->be_stop && !this->proc_is_run) {
      this->be_stop = false;
      this->be_lock = false;
    }
  }

  if (this->be_start && !this->proc_is_run) {
    if (!this->pcr_t) {
      this->pcr_t = true;
      genProcArgs(this->config, this->proc_args);
      if (!starter_win(this->proc_args.c_str(), this->pid)) {
        this->pcr_fail = true;
        this->be_start = false;
        this->be_lock = false;
        this->proc_is_run = false;
        this->pcr_t = false;
        this->pcr_n = 0;
      } else {
        std::cout << "start init ok" << std::endl;
      }
    } else {
      if (IsProcExist(this->pid)) {
        this->pcr_fail = false;
        this->be_start = false;
        this->be_lock = false;
        this->proc_is_run = true;
        this->pcr_t = false;
        this->pcr_n = 0;
        std::cout << "start success" << std::endl;
      } else {
        this->pcr_n++;
        if (this->pcr_n >= Worker::pcr_timeout) {
          this->pcr_fail = true;
          this->be_start = false;
          this->be_lock = false;
          this->proc_is_run = false;
          this->pcr_t = false;
          this->pcr_n = 0;
        }
      }
    }
  }

  if (this->be_stop && this->proc_is_run) {
    if (!this->pcr_t) {
      this->pcr_t = true;
      if (!stopper_win(this->pid)) {
        this->pcr_fail = true;
        this->be_stop = false;
        this->be_lock = false;
        this->proc_is_run = true;
        this->pcr_t = false;
        this->pcr_n = 0;
      } else {
        std::cout << "stop init ok" << std::endl;
      }
    } else {
      if (!IsProcExist(this->pid)) {
        this->pcr_fail = false;
        this->be_stop = false;
        this->be_lock = false;
        this->proc_is_run = false;
        this->pcr_t = false;
        this->pcr_n = 0;
        std::cout << "stop success" << std::endl;
      } else {
        this->pcr_n++;
        if (this->pcr_n >= Worker::pcr_timeout) {
          this->pcr_fail = true;
          this->be_stop = false;
          this->be_lock = false;
          this->proc_is_run = true;
          this->pcr_t = false;
          this->pcr_n = 0;
        }
      }
    }
  }

  if (!this->pcr_t && this->proc_is_run) {
    this->proc_is_run = IsProcExist(this->pid);
  }

}

void Worker::start(const Config config) {
  this->config = config;
  if (!this->be_lock && !this->pcr_t) {
    this->be_start = true;
    this->be_lock = true;
  }
}

void Worker::stop() {
  if (!this->be_lock && !this->pcr_t) {
    this->be_stop = true;
    this->be_lock = true;
  }
}

void Worker::exit() {
  this->stop();
  // TODO maybe it's not optimal
  if (!this->pcr_fail) waiter(this->proc_is_run);
  this->run = false;
  waiter(this->is_stop);
}

bool Worker::getStatus() {
  return this->status;
}
