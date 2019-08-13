
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
#include "worker.h"


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
  std::thread th (Worker::init_loop, this);
  th.detach();
  waiter(this->is_run);
}

Worker::~Worker() {
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
}

void Worker::stop() {
}

void Worker::exit() {
  this->stop();
  this->run = false;
  waiter(this->is_stop);
}

bool Worker::getStatus() {
  return this->status;
}
