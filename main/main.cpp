

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