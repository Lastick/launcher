
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
                 PROCESS_INFORMATION &pi) {
  bool res = false;

  std::string exe_dir;
  std::string exe_path;

  GetExePath(exe_dir);
  exe_path = exe_dir + std::string("/") + std::string(name_exe) + std::string(".exe");

  STARTUPINFOA si;

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

  return res;
}

bool stoper_win(PROCESS_INFORMATION &pi) {
  bool res = false;

  if (AttachConsole(pi.dwProcessId) != FALSE) {
    SetConsoleCtrlHandler(nullptr, true);
    res = GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);
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

Worker::Worker() {
  this->status = false;
  this->run = true;
  this->is_run = true;
  this->is_stop = true;
  this->proc_args = "";
  ZeroMemory(&this->pi, sizeof(this->pi));
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
    Sleep(1000);
    std::cout << "ok" << std::endl;
  }
  this->is_stop = false;
  std::cout << "exit" << std::endl;
}

void *Worker::init_loop(void *vptr_args) {
  ((Worker *) vptr_args)->loop();
  return NULL;
}

void Worker::start(const Config config) {
  this->config = config;
  genProcArgs(this->config, this->proc_args);
  std::cout << this->proc_args << std::endl;
  starter_win(this->proc_args.c_str(), this->pi);
}

void Worker::stop() {
  stoper_win(this->pi);
}

void Worker::exit() {
  this->stop();
  this->run = false;
  waiter(this->is_stop);
}

bool Worker::getStatus() {
  return this->status;
}
