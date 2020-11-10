#include "adc.h"
#include "can.h"
#include "connection_indicator.h"
#include "internode.h"
#include "joystick.h"
#include "mcp2518fd.h"
#include "network.h"
#include "score_counter.h"
#include "spi.h"
#include "system_init.h"
#include "touch_button.h"
#include "uart.h"

#include <stdint.h>


static void m_show_menu(){
    network_write_show_menu();

    Joystick left, right;

    while(1){
        joystick_read(&left, &right);

        if(left.position_changed || right.position_changed){
            network_write_joystick(&left, &right);
        }

        if(network_read_game_start()){
            return;
        }

        can_clear_receive_queue();
    }
}

static void m_play_game(){
    network_write_show_game();

    Joystick left, right;

    /* Ignore two least significant bits in joystick horizontals */
    uint8_t mask_left_horizontal = 0xfc;
    uint8_t mask_right_horizontal = 0xfc;

    uint8_t last_left_x = 0;
    uint8_t last_right_x = 0;

    score_counter_start();

    while(1){
        joystick_read(&left, &right);

        left.x &= mask_left_horizontal;
        right.x &= mask_right_horizontal;

        /* Right position for remote solenoid */
        if(right.position_changed){
            if(right.position == JOYSTICK_UP){
                internode_solenoid(1);
            }
            else{
                internode_solenoid(0);
            }
        }

        /* Right horizontal for remote gimbal */
        if(right.x != last_right_x){
            internode_servo(right.x);
        }

        /* Left horizontal for remote motor position or speed */
        if(left.x != last_left_x){
            internode_speed(left.x);
        }

        last_right_x = right.x;
        last_left_x = left.x;

        if(internode_end_game()){
            uint16_t score = score_counter_end();
            network_write_game_score(score);
            return;
        }
    }
}

static void m_show_score(){
    network_write_show_score();
    can_clear_receive_queue();
}

int main(){
    system_pre_init();

    adc_init();
    touch_button_init();

    uart_init();

    spi_init();
    mcp2518fd_init();
    can_init();

    score_counter_init();

    internode_reset();

    system_post_init();

    while(1){
        m_show_menu();
        m_play_game();
        m_show_score();
    }

    return 0;
}
