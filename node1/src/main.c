#include <avr/io.h>
#include "uart.h"
#include "hid.h"
#include "display.h"
#include "can.h"
#include "menu.h"
#include "internode.h"

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
    display_clear();
    can_init();


    fdevopen((int (*)(char, FILE*)) uart_send,(int (*)(FILE*)) uart_recv);


    volatile uint8_t * disp_c = (volatile uint8_t *)0x1000;
    volatile uint8_t * disp_d = (volatile uint8_t *)0x1200;

    printf("Node1 fungerer ikke \n\r");
 
    const MenuItem * p_menu_item = menu_root_node();
    uint8_t menu_selected_item = 0;
   
    HidJoystick joystick;
    HidJoystickPosition joystick_last_position = HID_JOYSTICK_CENTER;
    HidButton button;
    HidSlider slider;
    CanFrame recv_ir;

    while(1){

        button = hid_button_read();
        internode_solenoid(button.left);

        _delay_ms(100);

        slider = hid_slider_read();
        internode_position(slider.left);
    
        _delay_ms(100);

        joystick = hid_joystick_read();
        internode_speed(joystick.x); 
        internode_servo(joystick.y);

        _delay_ms(100);



        menu_print(p_menu_item, menu_selected_item);

        if(joystick.position == HID_JOYSTICK_DOWN
            && joystick_last_position != HID_JOYSTICK_DOWN
            && menu_selected_item < 2
        ){
            menu_selected_item++;
        }
        if(joystick.position == HID_JOYSTICK_UP
            && joystick_last_position != HID_JOYSTICK_UP
            && menu_selected_item > 0
        ){
            menu_selected_item--;
        }

        if(joystick.position == HID_JOYSTICK_RIGHT){
            printf("Menu item chosen: %d\n\r", menu_selected_item);
        }

        joystick_last_position = joystick.position;

        
        /*
        if(can_recv(&recv_ir)){
            display_print(7, "Broken", 0);
            printf("%d\n\r", (uint8_t)recv_ir.buffer[1]);
        }
        else {
            display_print(7, "", 0);
        }
        */ 
    }

    return 0;
}
