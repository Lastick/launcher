
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

#ifndef WORKER_H_INCLUDED
#define WORKER_H_INCLUDED

#include <Windows.h>
#include "settings.h"

class Worker {

  public:
    Worker();
    ~Worker();
    void start(const Settings settings);
    void stop();
    void exit();
    bool getStatus();
    void getStatusbarMess(std::string &mess);

  private:
    static const unsigned int pcr_timeout;
    static const unsigned int loop_interval;
    Settings settings;
    bool status;
    bool run;
    bool is_run;
    bool is_stop;
    bool be_start;
    bool be_stop;
    bool be_lock;
    bool proc_is_run;
    bool pcr_t;
    bool pcr_fail = false;
    int pid;
    int pcr_n;
    std::string proc_args;
    std::string status_bar_mess;
    void loop();
    static void *init_loop(void *vptr_args);
    void processor();

};

#endif // SETTINGS_H
