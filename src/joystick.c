#include "joystick.h"
#include "adc.h"

#define JOYSTICK_CENTER_L 50
#define JOYSTICK_CENTER_H 184
#define JOYSTICK_DEAD_BAND 20

static JoystickPosition joystick_position(const Joystick * p_joy){
    uint8_t x = p_joy->x;
    uint8_t y = p_joy->y;

    if(x > JOYSTICK_CENTER_L && x < JOYSTICK_CENTER_H){
        if(y > JOYSTICK_CENTER_L && y < JOYSTICK_CENTER_H){
            return JOYSTICK_CENTER;
        }

        if(y > JOYSTICK_CENTER_H + JOYSTICK_DEAD_BAND){
            return JOYSTICK_UP;
        }

        if(y < JOYSTICK_CENTER_L - JOYSTICK_DEAD_BAND){
            return JOYSTICK_DOWN;
        }
    }

    if(y > JOYSTICK_CENTER_L && y < JOYSTICK_CENTER_H){
        if(x > JOYSTICK_CENTER_H + JOYSTICK_DEAD_BAND){
            return JOYSTICK_RIGHT;
        }

        if(x < JOYSTICK_CENTER_L - JOYSTICK_DEAD_BAND){
            return JOYSTICK_LEFT;
        }
    }

    return JOYSTICK_INVALID;
}

static void joystick_set_position(Joystick * p_joy){
    JoystickPosition pos = joystick_position(p_joy);

    p_joy->position_changed = (pos != p_joy->position);
    if(pos == JOYSTICK_INVALID){
        p_joy->position_changed = 0;
    }

    p_joy->position = pos;
}

void joystick_read(Joystick * p_left, Joystick * p_right){
    p_left->x = adc_read(1);
    p_left->y = adc_read(0);

    p_right->x = adc_read(2);
    p_right->y = adc_read(3);

    joystick_set_position(p_left);
    joystick_set_position(p_right);
}
