#include <avr/io.h>
#include "uart.h"
#include "stdio.h"
//#include "sram_test.h"
#include "adc_driver.h"
#include "sram_driver.h"


/* Internal RC oscillator */
/* #define F_CPU 8000000UL */

/* External quartz oscillator */
#define F_CPU 4915200UL
#include <util/delay.h>

extern volatile char *ext_ram; // Start address for the SRAM


int main(){
    CLKPR |= (1<<CLKPCE);
    
    uart_init();
    fdevopen((int (*)(char, FILE*)) UART_Transmit,(int (*)(FILE*)) UART_recieve);
    //DDRC |= (1 << PC2);
    //PORTC &= ~(1 << PC2);
    ext_address_initialize();
    //SRAM_test();

    adc_init();


    while(1){
        
        char letter = UART_recieve();
        UART_Transmit(letter);

        uint8_t sample = adc_test();
        printf("Sample: %d\n\r", sample);
    }





    return 0;
}
