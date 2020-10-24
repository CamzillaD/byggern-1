#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "can_controller.h"
#include "can_interrupt.h"

#include "timer.h"
#include "servo.h"

#include "sam.h"



void delay(uint32_t ms){
    ms = ms * 6962;
    while(ms){
            ms--;
    }
}

int main()
{
    SystemInit();

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
    



    // LED_init
    REG_PIOA_PER = (PIO_PA19) | (PIO_PA20);
    REG_PIOA_OER = (PIO_PA19) | (PIO_PA20);

    while (1)
    {
        /* code */
        //printf("%x \n\r", CAN0->CAN_SR);
        //REG_PIOA_SODR = (PIO_PA19) | (PIO_PA20);
        //delay(1);
        //REG_PIOA_CODR = (PIO_PA19) | (PIO_PA20);
        //delay(1);
        //printf("%4x \n\r", timer_read_cv());

    }
}