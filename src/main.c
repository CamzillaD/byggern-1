#include <avr/io.h>
#include "uart.h"
#include "hid.h"
#include "can.h"
#include "frame_format.h"

#define F_CPU 4915200UL
#include <util/delay.h>

#include <stdio.h>
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

int main(){
    system_init();

    hid_init();
    sei();

    /* display_init(); */


    uart_init();


    while(1){
        uart_send('h');
        uart_send('e');
        uart_send('l');
        uart_send('l');
        uart_send('o');
        uart_send('\n');
        uart_send('\r');
    }


    HidJoystick joystick;
    HidSlider slider;
    HidButton button;



    can_init();


    CanFrame test_recv;
    uint8_t recv_buffer[8];
    test_recv.size = 8;
    test_recv.buffer = recv_buffer;

    CanFrame test_send;
    uint8_t send_buffer[8] = {0xff, 0xee, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22};
    test_send.size = 8;
    test_send.buffer = send_buffer;
    test_send.id = 2;

    can_send(&test_send);
    HidJoystick stick = hid_joystick_read();
    frame_joystick_send(stick);

    while(1){

        frame_joystick_send(hid_joystick_read());

    }

    return 0;
}
