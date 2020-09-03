#include "uart.h"
#include <avr/io.h>

//initialize UART
void uart_init(){

    UBRR0H = (uint8_t)(UART_BAUDRATE_REGISTER >> 8);
    UBRR0L = (uint8_t)(UART_BAUDRATE_REGISTER);

    UCSR0C = (1 << URSEL0) | (1 << UCSZ00) | (1 << UCSZ01);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

}

//Transmitter
void UART_Transmit(unsigned char data){

    //Wait for empty transmit buffer
    while(!(UCSR0A & (1<<UDRE0)));

    //Put data into buffer, sends data
    UDR0=data; 
}


unsigned char UART_recieve(){

    //wait for data to be recieved
    while(!(UCSR0A &(1<<RXC0)));

    //get and return recieved data from buffer
    return UDR0;
}
