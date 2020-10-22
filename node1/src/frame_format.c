#include "frame_format.h"



static CanFrame m_frame;
static uint8_t m_buffer[8];

void frame_joystick_send(HidJoystick pos){
    
    m_frame.id = 0 | 0x10;

    m_buffer[0] = pos.x;
    m_buffer[1] = pos.y;

    m_frame.size = 2;
    m_frame.buffer = m_buffer;

    can_send(&m_frame);
}

