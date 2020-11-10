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
#include "internode.h"
#include "score_counter.h"

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

    score_counter_init();

    /* connection_indicator_turn_off(); */

    Joystick left, right;

    CanFrame can_write_frame;
    can_write_frame.id = 0;
    can_write_frame.size = 0;

    CanFrame can_read_frame;
    can_read_frame.id = 0;
    can_read_frame.size = 0;

    sei();

    score_counter_start();

    while(1){
        uint8_t error = can_read(&can_read_frame);

        if(!error && can_read_frame.id == 0x09){
            uint16_t score = score_counter_end();

            can_read_frame.size = 2;
            can_read_frame.buffer[0] = (score >> 8);
            can_read_frame.buffer[1] = score;

            network_write_can_message(&can_read_frame);
        }
    }

    can_write_frame.size = 2;

    uint8_t left_count = 0;
    uint8_t right_count = 0;
    uint8_t send = 0;

    while(1){
        joystick_read(&left, &right);

        if(left.position_changed){
            if(left.position == JOYSTICK_UP){
                left_count++;
            }
            if(left.position == JOYSTICK_DOWN){
                left_count--;
            }

            send = 1;
        }

        if(right.position_changed){
            if(right.position == JOYSTICK_UP){
                right_count++;
            }
            if(right.position == JOYSTICK_DOWN){
                right_count--;
            }

            send = 1;
        }

        if(send){
            can_write_frame.id = 16;
            can_write_frame.size = 2;
            can_write_frame.buffer[0] = left_count;
            can_write_frame.buffer[1] = right_count;
            can_write(&can_write_frame);
        }

        send = 0;

        uint8_t error = can_read(&can_read_frame);

        if(!error){
            network_write_can_message(&can_read_frame);
        }
    }

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

        if(left.position_changed){
            network_write_joystick(&left, &right);
        }

        if(right.position_changed){
            can_write_frame.id = 0x10;
            can_write_frame.size = 2;
            can_write_frame.buffer[0] = 100;
            can_write_frame.buffer[1] = 100;
            can_write(&can_write_frame);
        }

        _delay_ms(500);
    }

    return 0;
}
