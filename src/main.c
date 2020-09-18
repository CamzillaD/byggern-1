#include <avr/io.h>
#include "uart.h"
#include "stdio.h"
#include "sram_test.h"
#include "adc.h"
#include "sram_driver.h"
#include "hid.h"
#include "display.h"


/* Internal RC oscillator */
/* #define F_CPU 8000000UL */

/* External quartz oscillator */
#define F_CPU 4915200UL
#include <util/delay.h>

extern volatile char *ext_ram; // Start address for the SRAM

int main(){
    CLKPR |= (1<<CLKPCE);
    
    uart_init();
    fdevopen((int (*)(char, FILE*)) uart_send,(int (*)(FILE*)) uart_recv);
    //DDRC |= (1 << PC2);
    //PORTC &= ~(1 << PC2);
    ext_address_initialize();
    SRAM_test();
    adc_init();

    hid_buttons_init();

    JoystickPosition pos;
    SliderPosition slider_pos;


    volatile uint8_t * disp_c = (volatile uint8_t *)0x1000;

    uint16_t loop = 0;
    uint8_t flash_on = 1;

    while(1){
        loop++;
        if(loop > 10){
            loop = 0;

            if(flash_on){
                *disp_c = 0xaf;
                _delay_us(10);
                *disp_c = 0xa5;
            }
            else{
                *disp_c = 0xae;
            }

            flash_on = !flash_on;
        }
        _delay_ms(1);

        
        adc_sample();

        printf(
            "CH0: %3d CH1: %3d CH2: %3d CH3: %3d\n\r",
            adc_read(ADC_CHANNEL_0),
            adc_read(ADC_CHANNEL_1),
            adc_read(ADC_CHANNEL_2),
            adc_read(ADC_CHANNEL_3)
        );
        
    }

    return 0;
}
