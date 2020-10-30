#include "spi.h"
#include <avr/io.h>

static void spi_slave_select(){
    PORTB &= ~(1 << PB4);
}

static void spi_slave_deselect(){
    PORTB |= (1 << PB4);
}

void spi_init(){
    /* MISO */
    DDRB &= ~(1 << PB6);

    /* Slave Select, MOSI, Serial Clock */
    DDRB |= (1 << PB4) | (1 << PB5) | (1 << PB7);
    PORTB |= (1 << PB4);

    /* Enable SPI, Master mode */
    SPCR |= (1 << SPE) | (1 << MSTR);

    /* 128 clock prescaler on SCK */
    SPCR |= (1 << SPR0) | (1 << SPR1);
}


void spi_shift(uint8_t * buffer, uint16_t size){
    spi_slave_select();

    for(uint16_t i = 0; i < size; i++){
        SPDR = buffer[i];

        while(!(SPSR & (1 << SPIF)));

        buffer[i] = SPDR;
    }

    spi_slave_deselect();
}
