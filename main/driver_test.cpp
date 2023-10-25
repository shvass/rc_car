
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver.hpp"
#include <driver/gpio.h>



extern "C" void app_main(void){


    driver driv(GPIO_NUM_13, GPIO_NUM_12);
    float angle = 0;
    float step = 0.05f;
    while (1) {
        printf("Angle of rotation: %f \n", angle);

        driv.setLeft(angle);
        //Add delay, since it takes time for servo to rotate, usually 200ms/60degree rotation under 5V power supply
        vTaskDelay(pdMS_TO_TICKS(500));
        if ((angle + step) >= 1.0f   || (angle + step) <= 0.0f) {
            step *= -1;
        }
        angle += step;
    }
};

