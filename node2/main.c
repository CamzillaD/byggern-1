#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"

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

    configure_uart();
    printf("Hello World\n\r");

    

    // LED_init
    REG_PIOA_PER = (PIO_PA19) | (PIO_PA20);
    REG_PIOA_OER = (PIO_PA19) | (PIO_PA20);



    while (1)
    {
        /* code */
        
        REG_PIOA_SODR = (PIO_PA19) | (PIO_PA20);
        delay(1000);
        REG_PIOA_CODR = (PIO_PA19) | (PIO_PA20);
        delay(1000);

    }

}