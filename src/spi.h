#ifndef SPI_H
#define SPI_H
#include <stdint.h>

void spi_init();

void spi_shift(uint8_t * buffer, uint16_t size);

#endif