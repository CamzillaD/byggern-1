
void uart_init(){

    UBRRH = (uint8_t)(UART_BAUDRATE_REGISTER >> 8);
    UBRRL = (uint8_t)(UART_BAUDRATE_REGISTER));

    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
    UCSRB = (1 << RXEN) | (1 << TXEN);

}

