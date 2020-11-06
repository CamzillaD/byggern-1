#include <avr/io.h>
#include "uart.h"
#include "hid.h"
#include "display.h"
#include "can.h"
#include "menu.h"
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
    display_init();
    uart_init();
    display_clear();
    can_init();

    fdevopen((int (*)(char, FILE*)) uart_send,(int (*)(FILE*)) uart_recv);


    volatile uint8_t * disp_c = (volatile uint8_t *)0x1000;
    volatile uint8_t * disp_d = (volatile uint8_t *)0x1200;

    printf("Node1 fungerer \n\r");
 
    const MenuItem * p_menu_item = menu_root_node();
    uint8_t menu_selected_item = 0;
   
    HidJoystick joystick;
    HidJoystickPosition joystick_last_position = HID_JOYSTICK_CENTER;
    HidButton button;
    HidSlider slider;
    CanFrame recv_ir;

    while(1){

        button = hid_button_read();
        frame_button_send(button);

        _delay_ms(100);

        slider = hid_slider_read();
        frame_slider_send(slider);

        _delay_ms(100);

        joystick = hid_joystick_read();
        frame_joystick_send(joystick); 

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

        //KAN IKKE LESE HVIS LINJEN ER BRUTT FRA START
        if(can_recv(&recv_ir)){
            display_print(7, "Broken", 0);
            printf("%d\n\r", (uint8_t)recv_ir.buffer[1]);
        }
        else {
            display_print(7, "", 0);
        }
        
        //printf("0x%2x\n\r", can_test());
        

        /*frame_joystick_send(hid_joystick_read());

        //_delay_ms(5);
        if (can_recv(&test_recv)){
            printf("id: %3d ", test_recv.id);
            for(uint8_t i = 0; i < test_recv.size; i++){
            printf("%3x", test_recv.buffer[i]);
        }
        printf("\n\r");
        }
*/
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
