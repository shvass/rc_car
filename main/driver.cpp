
#include "driver.hpp"

// timer configuration
mcpwm_timer_config_t timer_config = {
    .group_id = 0,
    .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
    .resolution_hz = MOTOR_TIMEBASE_RESOLUTION_HZ,
    .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    .period_ticks = MOTOR_TIMEBASE_PERIOD,
    .flags = {}
};

// operator config
mcpwm_operator_config_t operator_config = {
    .group_id = 0, // operator must be in the same group to the timer
    .flags = {}
};


mcpwm_comparator_config_t comparator_config = {
    .flags = {1, 0, 0}
};

driver::driver(int left, int right) {
    mcpwm_new_timer(&timer_config, &timer);

    mcpwm_new_operator(&operator_config, &lop);
    mcpwm_new_operator(&operator_config, &rop);

    mcpwm_operator_connect_timer(lop, timer);
    mcpwm_operator_connect_timer(rop, timer);


    mcpwm_new_comparator(lop, &comparator_config, &lcmp);
    mcpwm_new_comparator(rop, &comparator_config, &rcmp);

    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = left,
        .flags = {}
    };

    ESP_ERROR_CHECK(mcpwm_new_generator(lop, &generator_config, &lgen));

    generator_config.gen_gpio_num = right;
    mcpwm_new_generator(rop, &generator_config, &rgen);



    // go high on counter empty
    mcpwm_generator_set_action_on_timer_event(lgen,
                                                              MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH));
    // go low on compare threshold
    mcpwm_generator_set_action_on_compare_event(lgen,
                                                                MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, lcmp, MCPWM_GEN_ACTION_LOW));


  // go high on counter empty
    mcpwm_generator_set_action_on_timer_event(rgen,
                                                              MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH));
    // go low on compare threshold
    mcpwm_generator_set_action_on_compare_event(rgen,
                                                                MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, rcmp, MCPWM_GEN_ACTION_LOW));

    mcpwm_timer_enable(timer);
    mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP);
    
    // mcpwm_timer_enable(rtimer);
    // mcpwm_timer_start_stop(rtimer, MCPWM_TIMER_START_NO_STOP);

};


void driver::set(float left, float right){
    if(left  >  0.0f && left  <= 1.0f) mcpwm_comparator_set_compare_value(lcmp, map(left));
    if(right > 0.0f && right <= 1.0f) mcpwm_comparator_set_compare_value(rcmp, map(right));
};


void driver::setLeft(float left){
    mcpwm_comparator_set_compare_value(lcmp, map(left));
}


void driver::setRight(float right){
    mcpwm_comparator_set_compare_value(rcmp, map(right));
};