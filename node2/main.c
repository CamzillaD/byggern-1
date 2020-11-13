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

static inline void m_watchdog_disable(){
    WDT->WDT_MR = WDT_MR_WDDIS;
}

int main(){
    SystemInit();
    m_watchdog_disable();

    uart_init();
    can_init(0x00290561, 1, 2);

    solenoid_init();
    ir_beam_init();
    servo_init();
    
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
        servo_command_gimbal(0x00);
        printf("Ref 10\n\r");

        for(uint32_t i = 0; i < 8000000; i++){
            __asm__("nop");
        }
        motor_position_set_reference(200);
        servo_command_gimbal(0xff);
        printf("Ref 200\n\r");
    }

    InternodeCommand command;

    while(1){
        if(ir_beam_broken()){
            internode_end_game();
        }

        internode_command(&command);

        if(command.solenoid){
            solenoid_activate();
        }
        else{
            solenoid_deactivate();
        }

        servo_command_gimbal(command.servo);

        switch(command.command_type){
            case INTERNODE_COMMAND_POSITION:
            motor_position_set_reference(command.position_or_speed);
            motor_position_tracking_enable();
            break;

            case INTERNODE_COMMAND_SPEED:
            motor_command_speed(command.position_or_speed);
            motor_position_tracking_disable();
            break;
        }        
    }
}

