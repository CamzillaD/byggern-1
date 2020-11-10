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
    NETWORK_EVENT_INDICATOR      = 0x16,
    NETWORK_EVENT_REQUEST_RESET  = 0x17,
    NETWORK_EVENT_GAME_START     = 0x25,

    /* Slave to host */
    NETWORK_EVENT_SCORE_LOW      = 0x26,
    NETWORK_EVENT_SCORE_HIGH     = 0x27,
    NETWORK_EVENT_SCORE_COMMIT   = 0x28,
    NETWORK_EVENT_JOYSTICK_LH    = 0x31,
    NETWORK_EVENT_JOYSTICK_LV    = 0x32,
    NETWORK_EVENT_JOYSTICK_LP    = 0x33,
    NETWORK_EVENT_JOYSTICK_LS    = 0x34,
    NETWORK_EVENT_JOYSTICK_RH    = 0x36,
    NETWORK_EVENT_JOYSTICK_RV    = 0x37,
    NETWORK_EVENT_JOYSTICK_RP    = 0x38,
    NETWORK_EVENT_JOYSTICK_RS    = 0x39,

    NETWORK_EVENT_SHOW_MENU      = 0x50,
    NETWORK_EVENT_SHOW_GAME      = 0x51,
    NETWORK_EVENT_SHOW_SCORE     = 0x52,

    /* Bidirectional */
    NETWORK_EVENT_CAN_CLEAR      = 0x40,
    NETWORK_EVENT_CAN_ID_LOW     = 0x42,
    NETWORK_EVENT_CAN_ID_HIGH    = 0x43,
    NETWORK_EVENT_CAN_SIZE       = 0x44,
    NETWORK_EVENT_CAN_DATA       = 0x45,
    NETWORK_EVENT_CAN_COMMIT     = 0x4f
} NetworkEvent;

void network_write_show_menu();

void network_write_show_game();

void network_write_show_score();

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
 * @param this_node_origin Set to a 1 if this
 * is a message sent by the node. Set to 0 if the
 * message originated somewhere else on the bus.
 */
void network_write_can_message(
    const CanFrame * p_frame,
    uint8_t this_node_origin
);

/**
 * @brief Update the host about the score after
 * a completed game.
 *
 * @param score Score to update the host about.
 */
void network_write_game_score(uint16_t score);

/**
 * @brief Inquire the host about requests to start
 * a new game.
 *
 * @return 1 if a new game has been requested,
 * otherwise 0.
 */
uint8_t network_read_game_start();

#endif
