#include "uart.h"
#include <avr/io.h>

void uart_init(){
    UBRR0H = (uint8_t)(UART_BAUDRATE_REGISTER >> 8);
    UBRR0L = (uint8_t)(UART_BAUDRATE_REGISTER);

    UCSR0C = (1 << URSEL0) | (1 << UCSZ00) | (1 << UCSZ01);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

void uart_send(uint8_t byte){
    while(!(UCSR0A & (1<<UDRE0)));

    UDR0=data;
}

uint8_t uart_recv(){
    while(!(UCSR0A &(1<<RXC0)));

    return UDR0;
}
