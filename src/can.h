/**
 * @file can.h
 *
 * @brief CAN 2.0 interface module.
 */
#ifndef CAN_H
#define CAN_H
#include <stdint.h>

typedef struct {
    /**
     * @brief ID of the CAN frame.
     *
     * @warning Extended identifiers are not
     * supported; a maximum of 11 ID bits may
     * be used.
     */
    uint16_t id;

    /**
     * @brief The size of the data payload
     * contained in the CAN frame.
     *
     * @warning Flexible Datarate (FD) mode
     * is disabled in this driver. Hence, the
     * maximum payload size is 8 bytes. Any
     * number of bytes between 0 and 8 (inclusive)
     * may be used.
     */
    uint8_t size;

    /**
     * @brief Buffer for the CAN frame payload.
     * buffer[0] contains the zeroth payload byte;
     * buffer[1] contains the first, and so on.
     */
    uint8_t buffer[8];
} CanFrame;

/**
 * @brief Initialize the CAN interface. Must be called
 * before @p can_write and @p can_read can be used.
 *
 * @warning Depends on the mcp2518fd module. The function
 * @p mcp2518fd_init must be called before @p can_init.
 */
void can_init();

/**
 * @brief Write a CAN frame pointed to by @p p_frame
 * to the CAN bus.
 *
 * @param[in] p_frame Pointer to a CAN frame.
 *
 * @return 0 if the CAN frame was sent successfully;
 * 1 if the CAN transmit queue was full at the time
 * @p can_write was invoked.
 */
uint8_t can_write(const CanFrame * p_frame);

/**
 * @brief Attempt to read a pending CAN frame
 * received over the CAN bus.
 *
 * @param[out] p_frame Pointer to a CAN frame.
 *
 * @return 0 if a CAN frame was read successfully;
 * 1 if the receiver was empty at the time @p can_read
 * was invoked.
 */
uint8_t can_read(CanFrame * p_frame);

#endif
