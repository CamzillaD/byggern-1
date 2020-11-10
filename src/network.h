/**
 * @file network.h
 *
 * @brief Network (host <-> slave) communication module.
 */
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
    NETWORK_EVENT_JOYSTICK_LH = 0x31,
    NETWORK_EVENT_JOYSTICK_LV = 0x32,
    NETWORK_EVENT_JOYSTICK_LP = 0x33,
    NETWORK_EVENT_JOYSTICK_LS = 0x34,
    NETWORK_EVENT_JOYSTICK_RH = 0x36,
    NETWORK_EVENT_JOYSTICK_RV = 0x37,
    NETWORK_EVENT_JOYSTICK_RP = 0x38,
    NETWORK_EVENT_JOYSTICK_RS = 0x39,

    /* Bidirectional */
    NETWORK_EVENT_CAN_CLEAR   = 0x40,
    NETWORK_EVENT_CAN_ID_LOW  = 0x42,
    NETWORK_EVENT_CAN_ID_HIGH = 0x43,
    NETWORK_EVENT_CAN_SIZE    = 0x44,
    NETWORK_EVENT_CAN_DATA    = 0x45,
    NETWORK_EVENT_CAN_COMMIT  = 0x4f
} NetworkEvent;

/**
 * @brief Update the host about the slave joystick positions.
 * Movement information is only sent if it has changed since
 * the last update.
 *
 * @param[in] p_left Pointer to the left on board joystick.
 * @param[in] p_right Pointer to the right on board joystick.
 */
void network_write_joystick(
    const Joystick * p_left,
    const Joystick * p_right
);

/**
 * @brief Update the host about a CAN message.
 *
 * @param[in] p_frame Pointer to a CAN message.
 */
void network_write_can_message(const CanFrame * p_frame);

#endif
