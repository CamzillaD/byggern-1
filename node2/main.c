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

#include "sam.h"



/*
void delay(uint32_t ms){
    ms = ms * 6962;
    while(ms){
            ms--;
    }
}
*/

int main()
{
    motor_dac_init();
    motor_turnon();

    SystemInit();
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
    timer_pwm_init();
    timer_set_duty_cycle(0.5);

    CAN_MESSAGE test_broken;
    test.data[0] = 0x00;
    test.data[1] = 0x00;
    test.id = 0x09;
    test.data_length = 2;
    
    
    // LED_init
    REG_PIOA_PER = (PIO_PA19) | (PIO_PA20);
    REG_PIOA_OER = (PIO_PA19) | (PIO_PA20);   

    printf("%x \n\r",REG_PIOD_LOCKSR);

    motor_dac_set_speed(0x04);

     REG_PIOD_PER = PIO_PD10;
     REG_PIOD_OER = PIO_PD10;

    ir_adc_init();
    while (1){
            

      /*
        printf("%x \n\r",REG_PIOD_ODSR);
        REG_PIOD_CODR = PIO_PD10;
        delay(100000);
        printf("%x \n\r",REG_PIOD_ODSR);
        REG_PIOD_SODR = PIO_PD10; 
        delay(100000);  

       */ 
       
    
        /* test control servo
        if (can_receive(&test_broken, 0)){
       float a = test_broken.data[1];
       float b = 255;
       float value = test_broken.data[1] /255.0;
       timer_set_duty_cycle(value);

       
       printf("%d \n\r", (int)(value*1000));
       
       }
        
        
    */

   /*  Funksjon for å teste system med motor og servo
 
        if (can_receive(&test_broken, 0)){
            if(test_broken.id == 0x10){
                 int16_t value = test_broken.data[0] - 130;
                 printf("%d \n\r", value);
                 motor_dac_set_speed(value);

                float a = test_broken.data[1];
                float b = 255;
                float value2 = test_broken.data[1] /255.0;
                timer_set_duty_cycle(value2);


            }
        }

*/

    
        //motor_dac_send(0xaaaa);
        //uint32_t score = play_ping_pong_read_score();
        //printf("%d.%2d s\n\r", score/4,25* (score % 4));
       
       /*
        if(ir_beam_broken()){
            test_broken.data[0] = ir_beam_broken();
            test_broken.data[1] = play_ping_pong_read_score();
            can_send(&test_broken, 0);
        }
        
        ADC->ADC_CR = ADC_CR_START;
        while (!(ADC->ADC_ISR & ADC_ISR_EOC0)){}
        printf("%d \n\r", ir_beam_broken());
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
