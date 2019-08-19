
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


bool starter_win(const char *exe_name,
                 const char *args,
                 int &pid) {
  bool res = false;
  pid = 0;

  std::string exe_dir;
  std::string exe_path;

  getExePath(exe_dir);
  exe_path = exe_dir + std::string("/") + std::string(exe_name) + std::string(".exe");

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
  args += std::string(" --data-dir \"") + config.data_dir + std::string("\"");
  args += std::string(" --p2p-bind-ip \"") + config.p2p_ip + std::string("\"");
  args += std::string(" --p2p-bind-port " + config.p2p_port);
  args += std::string(" --p2p-external-port " + config.p2p_ext_port);
  args += std::string(" --rpc-bind-ip \"") + config.rpc_ip + std::string("\"");
  args += std::string(" --rpc-bind-port " + config.rpc_port);
  args += std::string(" --log-file \"") + config.log_path + std::string("\"");
  args += std::string(" --fee-address \"") + config.fee_address + std::string("\"");

  convertUTF8toCP1251(args);
}

void waiter(bool &flag){
  while (flag){
    Sleep(100);
  }
}

const unsigned int Worker::pcr_timeout = 10;
const unsigned int Worker::loop_interval = 1000;
const unsigned int Worker::log_limit = 100;

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
  this->log_lock_update = false;
  this->pid = 0;
  this->pcr_n = 0;
  this->log_last_mod = 0;
  this->proc_args = "";
  this->status_bar_mess = "";
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

  unsigned int local_log_last = 0;

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
      genProcArgs(this->settings.config, this->proc_args);
      if (!starter_win(this->settings.exe_name.c_str(), this->proc_args.c_str(), this->pid)) {
        this->pcr_fail = true;
        this->be_start = false;
        this->be_lock = false;
        this->proc_is_run = false;
        this->pcr_t = false;
        this->pcr_n = 0;
        this->status_bar_mess = "start error";
      } else {
        this->status_bar_mess = "start init ok";
      }
    } else {
      if (isProcExist(this->pid, this->settings.exe_name.c_str())) {
        this->pcr_fail = false;
        this->be_start = false;
        this->be_lock = false;
        this->proc_is_run = true;
        this->pcr_t = false;
        this->pcr_n = 0;
        this->status_bar_mess = "start success";
      } else {
        this->pcr_n++;
        if (this->pcr_n >= Worker::pcr_timeout) {
          this->pcr_fail = true;
          this->be_start = false;
          this->be_lock = false;
          this->proc_is_run = false;
          this->pcr_t = false;
          this->pcr_n = 0;
          this->status_bar_mess = "start failure";
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
        this->status_bar_mess = "stop error";
      } else {
        this->status_bar_mess = "stop init ok";
      }
    } else {
      if (!isProcExist(this->pid, this->settings.exe_name.c_str())) {
        this->pcr_fail = false;
        this->be_stop = false;
        this->be_lock = false;
        this->proc_is_run = false;
        this->pcr_t = false;
        this->pcr_n = 0;
        this->status_bar_mess = "stop success";
      } else {
        this->pcr_n++;
        if (this->pcr_n >= Worker::pcr_timeout) {
          this->pcr_fail = true;
          this->be_stop = false;
          this->be_lock = false;
          this->proc_is_run = true;
          this->pcr_t = false;
          this->pcr_n = 0;
          this->status_bar_mess = "stop failure";
        }
      }
    }
  }

  if (!this->pcr_t && this->proc_is_run) {
    this->proc_is_run = isProcExist(this->pid, this->settings.exe_name.c_str());
  }

  if (this->proc_is_run) {
    waiter(this->log_lock_update);
    if (getFileLastMod(this->settings.config.log_path.c_str(),
                       local_log_last)) {
      //if (local_log_last != this->log_last_mod) {
        loadLogFile(this->settings.config.log_path.c_str(),
                    Worker::log_limit,
                    this->log_mess);
          this->log_last_mod = local_log_last;
      //}
    }
  }

}

void Worker::start(const Settings settings) {
  this->settings = settings;
  this->settings.config.log_path = this->settings.config.data_dir + std::string("/node.log");
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

void Worker::getStatusbarMess(std::string &mess) {
  mess.clear();
  mess = this->status_bar_mess;
}

void Worker::getLogMess(std::string &mess) {
  mess.clear();
  this->log_lock_update = true;
  mess = this->log_mess;
  this->log_lock_update = false;
}

bool Worker::getStatus() {
  return this->status;
}
