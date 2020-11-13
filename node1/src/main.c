#include <avr/io.h>

#include "uart.h"
#include "display.h"
#include "menu.h"
#include "internode.h"

#define F_CPU 4915200ul
#include <util/delay.h>

#include <avr/interrupt.h>

#include "adc.h"

#include "spi.h"
#include "mcp2515.h"
#include "can.h"

#include "score_counter.h"
#include "joystick.h"
#include "slider.h"
#include "hid.h"

#include <stdlib.h>


static inline void system_init(){
    /* System clock prescaler of 1 */
    CLKPR = (1 << CLKPCE);

    /* Enable external memory interface */
    MCUCR |= (1 << SRE);

    /* Mask out JTAG pins from external address lines */
    SFIOR |= (1 << XMM2);
}

static void m_show_menu(){
    display_clear();

    Joystick joystick;
    const MenuItem * p_item = menu_root_node();
    uint8_t item_selected = 0;

    uint8_t remain_in_menu = 1;

    while(remain_in_menu){
        menu_print(p_item, item_selected);

        joystick_read(&joystick);
        if(joystick.position_changed){
            switch(joystick.position){
                case JOYSTICK_UP:
                if(item_selected > 0)
                    item_selected--;
                break;

                case JOYSTICK_DOWN:
                if(item_selected < menu_children(p_item, NULL) - 1)
                    item_selected++;
                break;

                case JOYSTICK_RIGHT:
                remain_in_menu = (p_item + item_selected + 1)->action();
                break;
            }
        }
    }
}

static void m_play_game(){
    Joystick joystick;
    uint8_t slider_left, slider_right;

    /* Ignore three least significant bits in joystick x */
    uint8_t mask_joystick_x = 0xf8;

    uint8_t joystick_x_last = 0;

    /* Ignore three least significant bits in sliders */
    uint8_t mask_slider_left = 0xf8;
    uint8_t mask_slider_right = 0xf8;

    uint8_t slider_left_last = 0;
    uint8_t slider_right_last = 0;

    score_counter_start();

    while(1){
        uint16_t score = score_counter_end();
        display_clear();
        display_print(0, "Playing Game", 0);
        display_print_number(3, score);

        joystick_read(&joystick);
        slider_read(&slider_left, &slider_right);

        slider_left &= mask_slider_left;
        slider_right &= mask_slider_right;

        joystick.x &= mask_joystick_x;

        /* Remote solenoid */
        if(hid_button_read().right){
            internode_solenoid(1);
        }
        else{
            internode_solenoid(0);
        }

        /* Right slider for remote position */
        if(slider_right != slider_right_last){
            internode_position(slider_right);
        }

        /* Joystick horizontal for remote gimbal */
        if(joystick.x != joystick_x_last){
            internode_servo(joystick.x);
        }

        joystick_x_last = joystick.x;
        slider_right_last = slider_right;
        slider_left_last = slider_left;

        if(internode_end_game()){
            return;
        }
    }
}

static void m_show_score(){
    uint16_t score = score_counter_end();
    display_clear();
    display_print(0, "Final Score", 0);
    display_print_number(3, score);

    _delay_ms(4000);
}


int main(){
    cli();
    system_init();

    uart_init();
    adc_init();
    hid_init();
    
    display_init();

    spi_init();
    mcp_init();
    can_init();

    score_counter_init();

    internode_reset();

    sei();

    
    while(1){
        m_show_menu();
        m_play_game();
        m_show_score();
    }

    return 0;
}
















