#include "internode.h"
#include "can.h"

static CanFrame m_frame;

/* Messages this module sends */
#define INTERNODE_ID_SPEEED 0x12
#define INTERNODE_ID_POSITION 0x13
#define INTERNODE_ID_SERVO 0x16
#define INTERNODE_ID_SOLENOID 0x18
#define INTERNODE_ID_RESET 0x05

/* Messages this module recieves */
#define INTERNODE_ID_END_GAME 0x09

void internode_speed(uint8_t speed){
    m_frame.id = INTERNODE_ID_SPEEED;
    m_frame.buffer[0] = speed;
    m_frame.size = 1;

    can_write(&m_frame);
}

void internode_position(uint8_t position){
    m_frame.id = INTERNODE_ID_POSITION;
    m_frame.buffer[0] = position;
    m_frame.size = 1;

    can_write(&m_frame);
}

void internode_servo(uint8_t servo){
    m_frame.id = INTERNODE_ID_SERVO;
    m_frame.buffer[0] = servo;
    m_frame.size = 1;

    can_write(&m_frame);
}

void internode_solenoid(uint8_t active){
    m_frame.id = INTERNODE_ID_SOLENOID;
    m_frame.buffer[0] = active;
    m_frame.size = 1;

    can_write(&m_frame);
}

void internode_reset(){
    m_frame.id = INTERNODE_ID_RESET;
    m_frame.size = 0;

    can_write(&m_frame);
}

uint8_t internode_end_game(){
    if(can_read(&m_frame)){
        if(m_frame.id == INTERNODE_ID_END_GAME){
            return 1;
        }
    }
    return 0;
}