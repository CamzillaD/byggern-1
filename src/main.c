#include <avr/io.h>
#include "uart.h"
#include "hid.h"
#include "display.h"

#define F_CPU 4915200UL
#include <util/delay.h>

#include <stdio.h>
#include <avr/interrupt.h>

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

    hid_init();
    sei();

    uart_init();
    fdevopen((int (*)(char, FILE*)) uart_send,(int (*)(FILE*)) uart_recv);


    volatile uint8_t * disp_c = (volatile uint8_t *)0x1000;

    uint16_t loop = 0;
    uint8_t flash_on = 1;

    HidJoystick joystick;
    HidSlider slider;
    HidButton button;

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

        joystick = hid_joystick_read();
        slider = hid_slider_read();
        button = hid_button_read();

        printf(
            "BL: %1d BR: %1d JX: %3d JY: %3d SL: %3d SR: %3d\n\r",
            button.left,
            button.right,
            joystick.x,
            joystick.y,
            slider.left,
            slider.right
        );
    }

    return 0;
}
