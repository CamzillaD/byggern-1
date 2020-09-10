#ifndef UART_H
#define UART_H
#define UART_BAUDRATE_REGISTER 31
#include <stdint.h>

void uart_init();

void uart_send(uint8_t byte);

uint8_t uart_recv();

#endif
