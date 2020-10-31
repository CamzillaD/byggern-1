#include <avr/io.h>
#include "uart.h"
#include "mcp2518fd.h"
#include "network.h"
#include "adc.h"
#include "spi.h"
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
    system_init();

    adc_init();

    sei();

    uart_init();
    spi_init();
    mcp2518fd_init();

    connection_indicator_init();

    Joystick left, right;

    touch_button_init();

    NetworkMessage command;

    while(1){
        if(network_message_read(&command)){
            uart_send('K');
            uart_send(':');
            uart_send(' ');
            uart_send(command.key);

            uart_send(' ');

            uart_send('V');
            uart_send(':');
            uart_send(' ');
            uart_send(command.value);

            uart_send('\n');
            uart_send('\r');

            if(command.key == 'A'){
                connection_indicator_turn_on();
            }
            if(command.key == 'B'){
                connection_indicator_turn_off();
            }
        }
    }

    return 0;
}
