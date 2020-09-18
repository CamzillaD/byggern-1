#ifndef HID_H 
#define HID_H 
#include <avr/io.h>


typedef struct {

    uint8_t x;
    uint8_t y;

} JoystickPosition;

typedef struct{

    uint8_t left;
    uint8_t right;


} SliderPosition;



void hid_buttons_init();

uint8_t hid_buttons_read_left();

uint8_t hid_buttons_read_right();

JoystickPosition hid_joystick_read();

SliderPosition hid_slider_read();




#endif 