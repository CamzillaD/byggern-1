#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "can_controller.h"
#include "play_ping_pong.h"

#include "timer.h"
#include "servo.h"
#include "ir.h"
#include "motor.h"
#include "solenoid.h"
#include "pid_regulator.h"
#include "sam.h"

#include "encoder.h"
#include "motor_position.h"

static inline void m_watchdog_disable(){
    WDT->WDT_MR = WDT_MR_WDDIS;
}

int main()
{
    SystemInit();
    timer_pwm_init();
    ir_adc_init();
    solenoid_init();

    m_watchdog_disable();

    uart_init();
    can_init(0x00290561, 1, 2);


    
    encoder_init();
    motor_init();

    motor_position_init();
    motor_position_set_reference(127.0);
    motor_position_tracking_enable();

    while(1){
        for(uint32_t i = 0; i < 8000000; i++){
            __asm__("nop;");
        }
        motor_position_set_reference(10);
        printf("Ref 10\n\r");

        for(uint32_t i = 0; i < 8000000; i++){
            __asm__("nop");
        }
        motor_position_set_reference(200);
        printf("Ref 200\n\r");
    }
}

