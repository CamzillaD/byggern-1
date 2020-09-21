#ifndef HID_H
#define HID_H
#include <stdint.h>

typedef struct {
    uint8_t x;
    uint8_t y;
} HidJoystick;

typedef struct {
    uint8_t left;
    uint8_t right;
} HidSlider;

typedef struct {
    uint8_t left;
    uint8_t right;
} HidButton;

/**
 * @brief Initializes the Human Interface Device (HID) module,
 * along with its underlying dependencies, including the ADC.
 *
 * Resources used:
 * - PORT D pin 4 for left capacitive touch button.
 * - PORT D pin 5 for right capacitive touch button.
 * - All of the ADC resources.
 *
 * @warning Global interrupts must be enabled in order for
 * calls to @c hid_joystick_read and @c hid_slider_read to
 * be valid. This is because the underlying ADC relies on
 * timed interrupts to sample these assets.
 */
void hid_init();

/**
 * @brief Read the joystick position.
 *
 * @return Position of the on-board joystick.
 */
HidJoystick hid_joystick_read();

/**
 * @brief Read the capacitive slider positions.
 *
 * @return Position of the left- and right
 * on-board slider positions.
 */
HidSlider hid_slider_read();

/**
 * @brief Read the capacitive button states.
 *
 * @return Activation status for the left- and
 * right on-board buttons.
 */
HidButton hid_button_read();

#endif
