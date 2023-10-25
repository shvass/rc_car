#ifndef DRIVER_HPP
#define DRIVER_HPP


#include <driver/mcpwm_prelude.h>


#define MOTOR_MIN_PULSEWIDTH_US 500  // Minimum pulse width in microsecond
#define MOTOR_MAX_PULSEWIDTH_US 2500
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
