#include "uart.h"
#include <avr/io.h>

/* Register = f_osc / (16 * baud) - 1 */
#define UART_BAUDRATE_REGISTER 103

void uart_init(){
    /* Baudrate 9600 */
    UBRR0H = (uint8_t)(UART_BAUDRATE_REGISTER >> 8);
    UBRR0L = (uint8_t)(UART_BAUDRATE_REGISTER);

    /* Asynchronous operation, no parity, 1 stop bit, 8 data bits */
    UCSR0C = (1 << URSEL0) | (1 << UCSZ00) | (1 << UCSZ01);

    /* Enable buffers, interrupt on receive */
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
}

void uart_write(uint8_t byte){
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = byte;
}

uint8_t uart_read(){
    return UDR0;
}
