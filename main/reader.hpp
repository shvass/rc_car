#ifndef READER_HPP
#define READER_HPP

#include "driver/mcpwm_cap.h"
#include "driver/gpio.h"

class reader{

public:

    volatile uint32_t chan[3] = {0};


    reader(int c1_GPIO, int c2_GPIO, int c3_GPIO);

private:

    mcpwm_cap_timer_handle_t cap_timer = NULL;
    mcpwm_cap_channel_handle_t cap_channels[3] = {NULL};



    volatile uint32_t p_buf[3];


    struct marker
    {
        int channel = 0;
        reader* rd = nullptr;
    } markers[3];
    
public:
    static bool isr_callback(mcpwm_cap_channel_handle_t cap_chan, const mcpwm_capture_event_data_t *edata, void *user_data);
};
#endif // READER_HPP