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
#include "reader.hpp"




bool reader::isr_callback(mcpwm_cap_channel_handle_t cap_chan, const mcpwm_capture_event_data_t *edata, void *user_data)
{

    marker* mrk = (marker*) user_data;
    reader* rd = mrk->rd;
    int channel = mrk->channel;


    // store the timestamp when pos edge is detected
    if (edata->cap_edge == MCPWM_CAP_EDGE_POS) {
        rd->p_buf[channel] = edata->cap_value;
    } else {
        uint32_t time = edata->cap_value;
        rd->chan[channel] = time - rd->p_buf[channel];
    }

    return true;
}


float reader::getData(int channel){
    uint32_t pw = chan[channel];
    return  (float)(pw - MIN_PW) / ( MAX_PW - MIN_PW);
};

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

    cap_ch_conf.gpio_num = c2_GPIO;
    mcpwm_new_capture_channel(cap_timer, &cap_ch_conf, &cap_channels[1]);

    cap_ch_conf.gpio_num = c3_GPIO;
    mcpwm_new_capture_channel(cap_timer, &cap_ch_conf, &cap_channels[2]);


    mcpwm_capture_channel_register_event_callbacks(cap_channels[0], &cbs, &markers[0]);
    mcpwm_capture_channel_register_event_callbacks(cap_channels[1], &cbs, &markers[1]);
    mcpwm_capture_channel_register_event_callbacks(cap_channels[2], &cbs, &markers[2]);


    mcpwm_capture_channel_enable(cap_channels[0]);
    mcpwm_capture_channel_enable(cap_channels[1]);
    mcpwm_capture_channel_enable(cap_channels[2]);



    mcpwm_capture_timer_enable(cap_timer);
    mcpwm_capture_timer_start(cap_timer);
}