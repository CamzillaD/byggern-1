#ifndef UART_H 
#define UART_H
#define UART_BAUDRATE_REGISTER 31



//initialize
void uart_init();

//tramsnitting

void UART_Transmit(unsigned char data);

//recieving

unsigned char UART_recieve();


#endif