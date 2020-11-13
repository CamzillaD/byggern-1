/**
 * @file joystick.h
 *
 * @brief Joystick module.
 */
#ifndef JOYSTICK_H
#define JOYSTICK_H
#include <stdint.h>

typedef enum {
    JOYSTICK_INVALID,
    JOYSTICK_CENTER,
    JOYSTICK_LEFT,
    JOYSTICK_RIGHT,
    JOYSTICK_UP,
    JOYSTICK_DOWN
} JoystickPosition;

typedef struct {
    uint8_t x;
    uint8_t y;
    JoystickPosition position;
    uint8_t position_changed;
} Joystick;

/**
 * @brief Read the positions of the on board joystick
 * and update the structure pointed to by @p p_joystick
 * accordingly.
 *
 * @param[out] p_joystick Pointer to joystick struct.
 *
 * @warning The ADC module must be enabled before this
 * function may be used.
 */
void joystick_read(Joystick * p_joystick);

#endif