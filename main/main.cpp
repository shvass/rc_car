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

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>

#include "driver.hpp"
#include "reader.hpp"

struct motor_speeds{
    float left = 0.0f, right = 0.0f;
};

struct joystick {
    float x = 0.0f, y = 0.0f, throttle = 0.0f;
};

motor_speeds map(joystick input){
    motor_speeds ret;

    ret.left = ret.right = input.y - 0.5f;


    input.x -= 0.5f;

    ret.left += input.x;
    ret.right -= input.x;
    // if(input.y < 0.5){

    // ret.left -= (input.x - 0.5f);
    // ret.right += (input.x - 0.5f);
    // } else {
    //     ret.left += (input.x - 0.5f);
    //     ret.right -= (input.x - 0.5f);
    // }

    ret.right *= input.throttle;
    ret.left *= input.throttle;

    ret.left += 0.5f;
    ret.right += 0.5f;
    return ret;
};



extern "C" void cpp_app_main(void){

    reader input(GPIO_NUM_34, GPIO_NUM_35, GPIO_NUM_32);
    driver wheels(GPIO_NUM_13, GPIO_NUM_12);

    joystick cords;
    motor_speeds speeds;

    while (1)
    {
        cords = joystick{.x = input.getData(0), .y = input.getData(1), .throttle = input.getData(2)};

        printf("map %f %f %f\n", cords.x, cords.y, cords.throttle);

        speeds = map(cords);

        printf("speeds %f %f \n\n\n", speeds.left, speeds.right);
        wheels.set(speeds.left, speeds.right);

        vTaskDelay(pdMS_TO_TICKS(50));
    }
    
};