/**
 * @file internode.h
 *
 * @brief Internode communication module.
 */
#ifndef INTERNODE_H
#define INTERNODE_H
#include <stdint.h>

/**
 * @brief Sends an internode message, requesting
 * that the remote node resets.
 */
void internode_reset();

/**
 * @brief Requests that the remote node activate-
 * or deactive its solenoid.
 *
 * @param active Requested solenoid activation. 1
 * for activation; 0 for deactivation.
 */
void internode_solenoid(uint8_t active);

/**
 * @brief Requests that the remote node attains a
 * given motor position. The position described by
 * @p position ranges from 0 (full left) to 255
 * (full right), with linear interpolation.
 *
 * @param position Requested remote motor position.
 */
void internode_position(uint8_t position);

/**
 * @brief Requests that the remote node commands
 * a given motor speed. The speed described by @p
 * speed ranges from 0 (full speed left) to 255
 * (full speed right), with both 127 and 128 taken
 * as zero speed in either direction.
 *
 * @param speed Requested remote motor speed.
 */
void internode_speed(uint8_t speed);

/**
 * @brief Requests the remote node to command a
 * given gimbal angle of the solenoid arm. The
 * argument @p servo ranges from 0 (full left)
 * to 255 (full right), with both 127 and 128
 * taken to mean the neutral angle.
 *
 * @param servo Requested remote gimbal angle.
 */
void internode_servo(uint8_t servo);

/**
 * @brief Inquires the internode messaging module
 * about the state of the remote detection beam.
 *
 * @return 1 if the remote detection beam has been
 * broken, otherwise 0.
 */
uint8_t internode_end_game();

#endif
