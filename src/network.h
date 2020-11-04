#ifndef NETWORK_H
#define NETWORK_H
#include <stdint.h>
#include "joystick.h"
#include "can.h"

typedef enum {
    /* Host to slave */
    NETWORK_EVENT_INDICATOR   = 0x16,
    NETWORK_EVENT_REQUEST_RESET = 0x17,

    /* Slave to host */
    NETWORK_EVENT_JOYSTICK_LH = 0x31,   // Horizontal
    NETWORK_EVENT_JOYSTICK_LV = 0x32,   // Vertical
    NETWORK_EVENT_JOYSTICK_LP = 0x33,   // Position
    NETWORK_EVENT_JOYSTICK_LS = 0x34,   // Select
    NETWORK_EVENT_JOYSTICK_RH = 0x36,   // Horizontal
    NETWORK_EVENT_JOYSTICK_RV = 0x37,   // Vertical
    NETWORK_EVENT_JOYSTICK_RP = 0x38,   // Position
    NETWORK_EVENT_JOYSTICK_RS = 0x39,   // Select

    /* Bidirectional */
    NETWORK_EVENT_CAN_CLEAR   = 0x40,
    NETWORK_EVENT_CAN_ID_LOW  = 0x42,
    NETWORK_EVENT_CAN_ID_HIGH = 0x43,
    NETWORK_EVENT_CAN_SIZE    = 0x44,
    NETWORK_EVENT_CAN_DATA    = 0x45,
    NETWORK_EVENT_CAN_COMMIT  = 0x4f,

    /* Debugging */
    NETWORK_EVENT_CAN_INTERRUPT = 0x66,

    NETWORK_EVENT_GENERIC = 0xef
} NetworkEvent;

void network_write_joystick(Joystick * p_left, Joystick * p_right);

void network_write_can_message(const CanFrame * p_frame);

void network_write_can_interrupt();

void network_write_generic(uint8_t value);

#endif
