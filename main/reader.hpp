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
#ifndef READER_HPP
#define READER_HPP

#include "driver/mcpwm_cap.h"
#include "driver/gpio.h"

#define MAX_PW  160000
#define MIN_PW  80000
#define GRP_ID 1


/**
 * @brief reader - Three channel PWM reader - optimized for FS-i6 reciever
 * 
 * @details FS-i6 generates pwm of frequency 50Hz ( 20 ms ) with pulse width
 * ranging from 80000 to 16000  (clock ticks). 
 */
class reader{

public:

    // pulse with from the three data channels
    volatile uint32_t chan[3] = {0};

    /**
     * @brief Construct a new reader object
     * 
     * @param c1_GPIO 1nd channel GPIO input pin
     * @param c2_GPIO 2nd channel GPIO input pin
     * @param c3_GPIO 3rd channel GPIO input pin
     */
    reader(int c1_GPIO, int c2_GPIO, int c3_GPIO);

    /**
     * @brief get realtime pwm reading
     * 
     * @param channel index of the channel 
     * @return float - normalized float from 0 - 1 (mappend using MAX_PW and MIN_PW )
     */
    float getData(int channel=0);

private:

    mcpwm_cap_timer_handle_t cap_timer = NULL;
    mcpwm_cap_channel_handle_t cap_channels[3] = {NULL};

    /**
     * @brief buffer for internal
     */
    volatile uint32_t p_buf[3];

    /**
     * @brief struct to pass data to the interrupt callback
     */
    struct marker
    {
        int channel = 0;
        reader* rd = nullptr;
    } markers[3];
    

public:

    /**
     * @brief interrupt callback - called at every pwm transition
     * 
     * @param user_data pointer to marker
     * @return true always
     */
    static bool isr_callback(mcpwm_cap_channel_handle_t cap_chan, const mcpwm_capture_event_data_t *edata, void *user_data);
};
#endif // READER_HPP