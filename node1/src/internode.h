#ifndef FRAME_FORNAT_H
#define FRAME_FORMAT_H
#include "can.h"
#include "hid.h"

/*
void frame_joystick_send(HidJoystick pos);

void frame_button_send(HidButton button);

void frame_slider_send(HidSlider slider);
*/

void internode_speed(uint8_t speed);

void internode_position(uint8_t pos);

void internode_servo(uint8_t servo);

void internode_solenoid(uint8_t active);

uint8_t internode_end_game();

void internode_reset();


#endif