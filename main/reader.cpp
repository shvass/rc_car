
#include "reader.hpp"

#define GRP_ID 1


bool reader::isr_callback(mcpwm_cap_channel_handle_t cap_chan, const mcpwm_capture_event_data_t *edata, void *user_data)
{
    // TaskHandle_t task_to_notify = (TaskHandle_t)user_data;
    // BaseType_t high_task _wakeup = pdFALSE;

    marker* mrk = (marker*) user_data;
    reader* rd = mrk->rd;
    int channel = mrk->channel;


    //calculate the interval in the ISR,
    //so that the interval will be always correct even when capture_queue is not handled in time and overflow.
    if (edata->cap_edge == MCPWM_CAP_EDGE_POS) {
        // store the timestamp when pos edge is detected
        rd->p_buf[channel] = edata->cap_value;
    } else {
        uint32_t time = edata->cap_value;
        rd->chan[channel] = time - rd->p_buf[channel];
        // notify the task to calculate the distance
        // xTaskNotifyFromISR(task_to_notify, tof_ticks, eSetValueWithOverwrite, &high_task_wakeup);
    }

    return true;
}



mcpwm_capture_timer_config_t cap_conf = {
    .group_id = GRP_ID,
    .clk_src = MCPWM_CAPTURE_CLK_SRC_DEFAULT,
};

mcpwm_capture_event_callbacks_t cbs = {
    .on_cap = reader::isr_callback,
};

mcpwm_capture_channel_config_t cap_ch_conf = {
    .gpio_num = 0,
    .prescale = 1,
    .flags = {true, true}
};

reader::reader(int c1_GPIO, int c2_GPIO, int c3_GPIO){

    markers[0] = marker{.channel = 0, .rd = this};
    markers[1] = marker{.channel = 1, .rd = this};
    markers[2] = marker{.channel = 2, .rd = this};


    mcpwm_new_capture_timer(&cap_conf, &cap_timer);
    
    cap_ch_conf.gpio_num = c1_GPIO;
    mcpwm_new_capture_channel(cap_timer, &cap_ch_conf, &cap_channels[0]);


    mcpwm_capture_channel_register_event_callbacks(cap_channels[0], &cbs, &markers[0]);
    mcpwm_capture_channel_enable(cap_channels[0]);



    ESP_ERROR_CHECK(mcpwm_capture_timer_enable(cap_timer));
    ESP_ERROR_CHECK(mcpwm_capture_timer_start(cap_timer));
}