#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "play_ping_pong.h"

#include "timer.h"
#include "servo.h"
#include "ir.h"
#include "motor.h"
#include "solenoid.h"
#include "pid_regulator.h"
#include "sam.h"


int main()
{
    SystemInit();
    motor_init();
    timer_pwm_init();
    ir_adc_init();
    solenoid_init();
    play_ping_pong_init();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    //CAN_BR = 0x00069333
    configure_uart();
    //0x007f1633
    can_init(0x00290561, 1, 2);
    printf("Mye dyrere enn studentvin\n\r");

    CAN_MESSAGE test;
    test.data[0] = 0xff;
    test.data[1] = 0x31;
    test.data[2] = 0x11;
    test.data[3] = 0xa0;
    test.data[4] = 0x0e;
    test.data[5] = 0xad;
    test.data[6] = 0xad;
    //test.data[7] = 0x5f;
    test.id = 0x09;
    test.data_length = 7;
    can_send(&test,0);

    CAN_MESSAGE recv_can;
    test.data[0] = 0x00;

    CAN_MESSAGE send_can;
    test.data[0] = 0x00;


    //pid_regulator_init(0.5, 0, 10);

    printf("%d \n\r", 22);
    while (1){

        //motor_go_to_position(120);

    /*
        motor_dac_set_speed(0x07);
        delay(50000);
        motor_dac_set_speed(-0x0);
        delay(50000);
        motor_dac_set_speed(0x04);
        delay(50000);
    */
   // printf("%d \n\r", motor_read_encoder());


   //  Funksjon for å teste can-meldingene 


        if (can_receive(&recv_can, 0)){
            if(recv_can.id == 0x10){
                float a = recv_can.data[1];
                float b = 255;
                float value2 = recv_can.data[1] /255.0;
                timer_set_duty_cycle(value2);

            }
            if(recv_can.id == 0x12){
                int16_t value = recv_can.data[0] - 130;
                 motor_command_speed(value);
                 }

            if(recv_can.id == 0x11 & recv_can.data[0]){
                printf("%d \n\r",recv_can.data[0]);
                solenoid_activate();
            }
            else{
                solenoid_deactivate();
            }
        }



/*        
     REG_PIOC_SODR = PIO_SODR_P12;
    delay(1000);
     REG_PIOC_CODR = PIO_CODR_P12;
    delay(1000);
*/

/* if(ir_beam_broken()){
            send_can.data[0] = ir_beam_broken();
            send_can.data[1] = play_ping_pong_read_score();
            can_send(&send_can, 0);
        }
        
    if (can_receive(&test_broken, 0)){ if(ir_beam_broken()){
            send_can.data[0] = ir_beam_broken();
            send_can.data[1] = play_ping_pong_read_score();
            can_send(&send_can, 0);
        }
        
                solenoid_activate();
            }
            else{
                solenoid_deactivate();
            }
        }
    }

    */

        //motor_dac_send(0xaaaa);
        //uint32_t score = play_ping_pong_read_score();
        //printf("%d.%2d s\n\r", score/4,25* (score % 4));
       
        ADC->ADC_CR = ADC_CR_START;
            /*
             if(ir_beam_broken()){
            send_can.data[0] = ir_beam_broken();
            send_can.data[1] = play_ping_pong_read_score();
            can_send(&send_can, 0);
        }
    */
        
        while (!(ADC->ADC_ISR & ADC_ISR_EOC0)){}
        printf("%d \n\r", ir_beam_broken());
    
    /*
        if(ir_beam_broken()){
            send_can.data[0] = ir_beam_broken();
            send_can.data[1] = play_ping_pong_read_score();
            can_send(&send_can, 0);
        }
   */     
        

     
    /*
        printf("%x \n\r", CAN0->CAN_SR);
        REG_PIOA_SODR = (PIO_PA19) | (PIO_PA20);
        delay(1);
        REG_PIOA_CODR = (PIO_PA19) | (PIO_PA20);
        delay(1);
        printf("%4x \n\r", timer_read_cv());

        */
    }
}
