/**
 * @file touch_button.h
 *
 * @brief Touch Button interface module.
 */
#ifndef TOUCH_BUTTON_H
#define TOUCH_BUTTON_H
#include <stdint.h>

typedef enum {
    TOUCH_BUTTON_LEFT,
    TOUCH_BUTTON_RIGHT
} TouchButton;

/**
 * @brief Enables the touch button interface.
 *
 * Required resources:
 * - Pin 9 (PD3)
 * - Pin 10 (PD4)
 */
void touch_button_init();

/**
 * @brief Checks whether button @p button is activated.
 *
 * @param button Which touch button to inquire.
 *
 * @return 0 if button is not activated; non-zero if
 * the button is activated.
 */
uint8_t touch_button_read(TouchButton button);

#endif
