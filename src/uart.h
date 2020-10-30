#ifndef UART_H
#define UART_H
#include <stdint.h>

/**
 * @brief Initializes the UART 0 peripheral, with
 * the following settings:
 * - 9600 baudrate
 * - 8 data bits
 * - 1 stop bit
 * - No parity
 * - No hardware flow control
 *
 * Required resources:
 * - UART0 (peripheral)
 * - Pin 5 (PD0)
 * - Pin 7 (PD1)
 */
void uart_init();

/**
 * @brief Sends @p byte over the UART link.
 *
 * @param byte Byte to be sent by the UART.
 */
void uart_send(uint8_t byte);

/**
 * @brief Block until a byte is received by the UART link,
 * then return the received byte.
 *
 * @return Byte received by the UART.
 */
uint8_t uart_recv();

#endif
