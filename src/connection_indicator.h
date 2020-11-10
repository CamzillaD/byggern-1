/**
 * @file connection_indicator.h
 *
 * @brief Connection indicator module.
 */
#ifndef CONNECTION_INDICATOR_H
#define CONNECTION_INDICATOR_H

/**
 * @brief Turns the connection indicator on the
 * front side of the board on.
 *
 * @warning The connection indicator is controlled
 * by the on-board MCP2518FD chip. Hence, a call
 * must be made to @p mcp2518fd_init before
 * calling this function.
 */
void connection_indicator_turn_on();

/**
 * @brief Turns the connection indicator on the
 * front side of the board off.
 *
 * @warning The connection indicator is controlled
 * by the on-board MCP2518FD chip. Hence, a call
 * must be made to @p mcp2518fd_init before
 * calling this function.
 */
void connection_indicator_turn_off();

#endif
