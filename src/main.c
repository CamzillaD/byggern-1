#include <avr/io.h>
#include "uart.h"
#include "spi.h"
#include "mcp2518fd.h"
#include "can.h"
#include "network.h"
#include "adc.h"
#include "joystick.h"
#include "touch_button.h"
#include "connection_indicator.h"

#define F_CPU 16000000ul
#include <util/delay.h>

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
    cli();
    system_init();

    adc_init();
    touch_button_init();

    uart_init();

    spi_init();
    mcp2518fd_init();
    can_init();

    connection_indicator_init();
    connection_indicator_turn_off();

    Joystick left, right;

    sei();

    CanFrame frame;
    frame.id = 0;
    frame.size = 4;
    frame.buffer[0] = 'D';
    frame.buffer[1] = 'I';
    frame.buffer[2] = 'C';
    frame.buffer[3] = 'K';

    CanFrame recv;

    while(1){
        can_send(&frame);
        frame.id++;
        frame.buffer[3]++;

        uint8_t error = can_recv(&recv);

        if(!error){
            /* network_write_generic(recv.id); */

            network_write_can_message(&recv);
        }

        _delay_ms(1000);
    }

    while(1){
        joystick_read(&left, &right);

        network_write_joystick(&left, &right);
    }

    return 0;
}
