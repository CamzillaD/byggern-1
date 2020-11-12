#include "internode.h"

static CanFrame m_frame;
static uint8_t m_buffer[8];


/* Messages this module sends */
#define INTERNODE_ID_SPEEED 0x12
#define INTERNODE_ID_POSITION 0x13
#define INTERNODE_ID_SERVO 0x16
#define INTERNODE_ID_SOLENOID 0x18
#define INTERNODE_ID_RESET 0x05

/* Messages this module recieves */
#define INTERNODE_ID_END_GAME 0x09


void internode_speed(uint8_t speed){
    m_frame.id = 0 | INTERNODE_ID_SPEEED;

    m_buffer[0] = speed;

    m_frame.size = 1;
    m_frame.buffer = m_buffer;

    can_send(&m_frame);
}

void internode_position(uint8_t pos){
    m_frame.id = 0 | INTERNODE_ID_POSITION;

    m_buffer[0] = pos;

    m_frame.size = 1;
    m_frame.buffer = m_buffer;

    can_send(&m_frame);
}

void internode_servo(uint8_t servo){
    m_frame.id = 0 | INTERNODE_ID_SERVO;

    m_buffer[0] = servo;

    m_frame.size = 1;
    m_frame.buffer = m_buffer;

    can_send(&m_frame);
}

void internode_solenoid(uint8_t active){
    m_frame.id = 0 | INTERNODE_ID_SOLENOID;

    m_buffer[0] = active;

    m_frame.size = 1;
    m_frame.buffer = m_buffer;

    can_send(&m_frame);
}

void internode_reset(){
    m_frame.id = 0 | INTERNODE_ID_RESET;
    m_frame.size = 0;

    can_send(&m_frame);
}

uint8_t internode_end_game(){
    if(can_recv(&m_frame)){
        if(m_frame.id == INTERNODE_ID_END_GAME){
            return 1;
        }
    }
    return 0;
}






/*
void frame_joystick_send(HidJoystick pos){
    
    m_frame.id = 0 | 0x10;

    m_buffer[0] = pos.x;
    m_buffer[1] = pos.y;

    m_frame.size = 2;
    m_frame.buffer = m_buffer;

    can_send(&m_frame);
}


void frame_button_send(HidButton button){
    
    m_frame.id = 0 | 0x11;
    m_buffer[0] = button.left;
    m_buffer[1] = button.right;

    m_frame.size = 2;
    m_frame.buffer = m_buffer;

    can_send(&m_frame);

}

void frame_slider_send(HidSlider slider){
    m_frame.id = 0 | 0x12;

    m_buffer[0] = slider.left;
    m_buffer[1] = slider.right;

    m_frame.size = 2;
    m_frame.buffer = m_buffer;

    can_send(&m_frame);
}
*/


