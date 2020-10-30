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
 * @brief Read the positions of the two on board joysticks
 * and update the structures pointed to by @p p_left and
 * @p p_right accordingly.
 *
 * @param[out] p_left Pointer to left joystick struct.
 * @param[out] p_right Pointer to right joystick struct.
 *
 * @warning The ADC module must be enabled before this
 * function may be used.
 */
void joystick_read(Joystick * p_left, Joystick * p_right);

#endif
