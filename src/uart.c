#include "uart.h"
#include <avr/io.h>
#include <stdio.h>

void uart_init(){
    UBRR0H = (uint8_t)(UART_BAUDRATE_REGISTER >> 8);
    UBRR0L = (uint8_t)(UART_BAUDRATE_REGISTER);

    UCSR0C = (1 << URSEL0) | (1 << UCSZ00) | (1 << UCSZ01);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

void uart_send(uint8_t byte){
    while(!(UCSR0A & (1<<UDRE0)));

    UDR0=byte;
}

uint8_t uart_recv(){
    while(!(UCSR0A &(1<<RXC0)));

    return UDR0;
}

/*
void uart_test(){
    uart_init();
    fdevopen(UART_Transmit,UART_recieve);

    while(1){
    printf("Hei");
    _delay_ms(500);
    }
}
*/