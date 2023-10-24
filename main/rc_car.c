#include <stdio.h>

#include "driver/gpio.h"
// #include "driver/timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gptimer.h"

#define RCC1 GPIO_NUM_34
#define RCC2 GPIO_NUM_35
#define RCC3 GPIO_NUM_32

#define RCC1m  (1ULL<<RCC1)
#define RCC2m  (1ULL<<RCC2)
#define RCC3m  (1ULL<<RCC3)


uint64_t times[3] = {0, 0, 0};
uint64_t delay[3];

void rcHandler(void * data){
    uint64_t time = 0;
    int channel = *(int*)data;
    timer_get_counter_value(TIMER_GROUP_0, TIMER_0, &time);

    delay[channel] = time - times[channel];
    times[channel] = time;

}

int RC_INT_CHANNEL1 = 0;
int RC_INT_CHANNEL2 = 1;
int RC_INT_CHANNEL3 = 2;



void init_timer(){

    timer_config_t cfg = {
        .alarm_en = TIMER_ALARM_EN, 
        .auto_reload = TIMER_AUTORELOAD_EN,
        .clk_src = TIMER_SRC_CLK_APB,
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        // .divider = 1,
    };
    
    timer_init(TIMER_GROUP_0, TIMER_0, &cfg);
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
    timer_start(TIMER_GROUP_0, TIMER_0);
}

void config_rc_gpio(){
    gpio_reset_pin(RCC1);
    gpio_reset_pin(RCC2);
    gpio_reset_pin(RCC3);


    gpio_config_t cfg = {
        .pin_bit_mask = RCC1m | RCC2m,
        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE
    };

    gpio_config(&cfg);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(RCC1, rcHandler, &RC_INT_CHANNEL1);
    gpio_isr_handler_add(RCC2, rcHandler, &RC_INT_CHANNEL2);
    gpio_isr_handler_add(RCC3, rcHandler, &RC_INT_CHANNEL3);
}

void app_main(void)
{

    init_timer();
    config_rc_gpio();

    while (1)
    {
        printf(delay[0]);
        printf(delay[1]);
        printf(delay[2]);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
    
}
