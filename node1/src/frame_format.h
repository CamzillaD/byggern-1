#ifndef FRAME_FORNAT_H
#define FRAME_FORMAT_H
#include "can.h"
#include "hid.h"


void frame_joystick_send(HidJoystick pos);

void frame_button_send(HidButton button);

void frame_slider_send(HidSlider slider);

void frame_start_game_send();

#endif