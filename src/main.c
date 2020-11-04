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

    CanFrame can_write_frame;
    can_write_frame.id = 0;
    can_write_frame.size = 0;

    CanFrame can_read_frame;
    can_read_frame.id = 0;
    can_read_frame.size = 0;

    sei();

    can_write_frame.size = 2;

    while(1){
        can_write(&can_write_frame);

        can_write_frame.id++;
        can_write_frame.buffer[1] = can_write_frame.id;
        can_write_frame.buffer[0] = can_write_frame.id >> 8;

        uint8_t error = can_read(&can_read_frame);

        if(!error){
            network_write_can_message(&can_read_frame);
        }

        joystick_read(&left, &right);

        network_write_joystick(&left, &right);

        _delay_ms(50);
    }

    return 0;
}
