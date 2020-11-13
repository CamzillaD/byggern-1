#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "can_controller.h"

#include "servo.h"
#include "ir_beam.h"
#include "motor.h"
#include "solenoid.h"
#include "sam.h"

#include "encoder.h"
#include "motor_position.h"

#include "internode.h"
#include "real_time.h"

static inline void m_watchdog_disable(){
    WDT->WDT_MR = WDT_MR_WDDIS;
}

int main(){
    SystemInit();
    m_watchdog_disable();

    uart_init();
    can_init(0x00290561, 1, 2);
    printf("System Reset\n\r");

    solenoid_init();
    ir_beam_init();
    servo_init();

    real_time_init();
    
    encoder_init();
    motor_init();

    motor_position_init();
    motor_position_set_reference(127.0);
    motor_position_tracking_enable();

    while(1){
        if(ir_beam_broken()){
            if(real_time_incremented()){
                internode_end_game();
            }
        }

        if(g_COMMAND.solenoid){
            solenoid_activate();
        }
        else{
            solenoid_deactivate();
        }

        servo_command_gimbal(g_COMMAND.servo);

        motor_position_set_reference(
            0xff - g_COMMAND.position_or_speed
        );
    }
}

