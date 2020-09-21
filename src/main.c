#include <avr/io.h>
#include "uart.h"
#include "adc.h"
#include "hid.h"
#include "display.h"

#define F_CPU 4915200UL
#include <util/delay.h>

#include <stdio.h>

static void system_init(){
    /* System clock prescaler of 1 */
    CLKPR = (1 << CLKPCE);

    /* Enable external memory interface */
    MCUCR |= (1 << SRE);

    /* Mask out JTAG pins from external address lines */
    SFIOR |= (1 << XMM2);
}

int main(){
    system_init();

    adc_init();

    uart_init();
    fdevopen((int (*)(char, FILE*)) uart_send,(int (*)(FILE*)) uart_recv);

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
