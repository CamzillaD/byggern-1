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

    uint8_t id = mcp2518fd_sfr_read(MCP_SFR_C1CON(3));
    network_write_generic(id);

    CanFrame frame;
    frame.id = 0x0016;
    frame.size = 4;
    frame.buffer[0] = 'D';
    frame.buffer[1] = 'I';
    frame.buffer[2] = 'C';
    frame.buffer[3] = 'K';

    can_send(&frame);


    _delay_ms(100);

    while(1){
        joystick_read(&left, &right);

        network_write_joystick(&left, &right);
    }

    return 0;
}
