#include <avr/io.h>
#include "uart.h"
#include "hid.h"
#include "display.h"
#include "can.h"
#include "menu.h"

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

    display_init();


    uart_init();
    fdevopen((int (*)(char, FILE*)) uart_send,(int (*)(FILE*)) uart_recv);


    volatile uint8_t * disp_c = (volatile uint8_t *)0x1000;
    volatile uint8_t * disp_d = (volatile uint8_t *)0x1200;

    uint16_t loop = 0;

    HidJoystick joystick;
    HidSlider slider;
    HidButton button;

    display_clear();

    //display_print_heading("WAP Menu");
    display_print(2, "Gi");
    display_print(3, "Oss");
    display_print(4, "Godkjent? <3");


    can_init();
    

    CanFrame test_recv;
    uint8_t recv_buffer[8];
    test_recv.size = 8;
    test_recv.buffer = recv_buffer;

    printf("Camilla eller whatever \n\r");

    while(1){
        //printf("0x%2x\n\r", can_test());

        //_delay_ms(5);
        if (can_recv(&test_recv)){
            printf("id: %3d ", test_recv.id);
            for(uint8_t i = 0; i < test_recv.size; i++){
            printf("%2x", test_recv.buffer[i]);
        }
        printf("\n\r");
        }

        //printf("can_test: %x\t", can_test());
        //printf("test_send: %x%x \t", test.buffer[0], test.buffer[1]);
        //printf("test_recv: %x%x \n\r", test_recv.buffer[0], test.buffer[1]);
        
        /*
        _delay_ms(5);

        joystick = hid_joystick_read();
        slider = hid_slider_read();
        button = hid_button_read();

        printf(
            "BL: %1d BR: %1d JX: %3d JY: %3d JP: %1d SL: %3d SR: %3d\n\r",
            button.left,
            button.right,
            joystick.x,
            joystick.y,
            joystick.position,
            slider.left,
            slider.right
        );
 
        */
    }

    return 0;
}
