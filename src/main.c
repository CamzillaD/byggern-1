#include <avr/io.h>
#include "uart.h"
#include "mcp2518fd.h"
#include "network.h"
#include "adc.h"
#include "joystick.h"
#include "touch_button.h"

#define F_CPU 16000000ul
#include <util/delay.h>

#include <avr/interrupt.h>

static void system_init(){
    /* System clock prescaler of 1 */
    CLKPR = (1 << CLKPCE);
    //CLKPR = (1 << CLKPS3);

    /* Enable external memory interface */
    MCUCR |= (1 << SRE);

    /* Mask out JTAG pins from external address lines */
    SFIOR |= (1 << XMM2);
}


void print_8bit(uint8_t value){
    uart_send(value / 100 + '0');
    value %= 100;

    uart_send(value / 10 + '0');
    value %= 10;

    uart_send(value + '0');
}

int main(){
    system_init();

    adc_init();

    sei();

    uart_init();
    mcp2518fd_init();

    network_init();

    Joystick left, right;

    touch_button_init();

    while(1){
        network_indicate(NETWORK_STATE_CONNECTED);

        uart_send(touch_button_read(TOUCH_BUTTON_LEFT) + '0');
        uart_send(' ');
        uart_send(touch_button_read(TOUCH_BUTTON_RIGHT) + '0');

        uart_send('\n');
        uart_send('\r');

        network_indicate(NETWORK_STATE_DISCONNECTED);
    }

    return 0;
}
