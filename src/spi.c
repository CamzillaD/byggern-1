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

    /* Slave Select, Serial Clock, MOSI */
    DDRB |= (1 << PB4) | (1 << PB5) | (1 << PB7);
    PORTB |= (1 << PB4);

    /* Enable SPI, Master mode */
    SPCR |= (1 << SPE) | (1 << MSTR);
}

void spi_shift(uint8_t * buffer, uint16_t size){
    spi_slave_select();
    for(int i = 0; i < size; i++){

        SPDR = buffer[i];
        while(!(SPSR & (1 << SPIF))){

        }
        
        buffer[i] = SPDR;
    }

    spi_slave_deselect();
}