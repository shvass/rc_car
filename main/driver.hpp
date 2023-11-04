// rc_car - esp-idf RC driver libraries 
// Copyright (C) 2023  akshay bansod <akshayb@gmx.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#ifndef DRIVER_HPP
#define DRIVER_HPP


#include <driver/mcpwm_prelude.h>


#define MOTOR_MIN_PULSEWIDTH_US 10  // Minimum pulse width in microsecond
#define MOTOR_MAX_PULSEWIDTH_US 19000
#define MOTOR_TIMEBASE_RESOLUTION_HZ 1 * 1000 * 1000  // 1MHz, 1us per tick
#define MOTOR_TIMEBASE_PERIOD        20000




// two channel motor driver
class driver{

    private:




    public:

    mcpwm_timer_handle_t timer;
    mcpwm_oper_handle_t lop = nullptr, rop = nullptr;
    mcpwm_cmpr_handle_t lcmp = nullptr, rcmp = nullptr;
    mcpwm_gen_handle_t lgen = nullptr, rgen = nullptr;



    driver(int left, int right);


    static int map(float norm){
        return (norm) * (MOTOR_MAX_PULSEWIDTH_US - MOTOR_MIN_PULSEWIDTH_US) + MOTOR_MIN_PULSEWIDTH_US;
    }

    void set(float left = -1, float right = -1);

    void setLeft(float left);
    void setRight(float right);

};


#endif// DRIVER_HPP
