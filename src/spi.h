#ifndef SPI_H
#define SPI_H
#include <stdint.h>

/**
 * @brief Init the SPI peripheral in Master mode.
 *
 * Required resources:
 * - SPI (peripheral)
 * - Pin 1 (PB5)
 * - Pin 2 (PB6)
 * - Pin 3 (PB7)
 * - Pin 44 (PB4)
 */
void spi_init();

/**
 * @brief Shift @p size number of bytes over SPI.
 * The buffer @p buffer is used for both transmission
 * and reception, and will be modified with new data
 * once this function returns.
 *
 * @param buffer Buffer for transmission and reception.
 * @param size Number of bytes in @p buffer.
 *
 * @warning @p buffer must be at least @p size number
 * of bytes large, otherwise illegal memory operations
 * will be performed.
 */
void spi_shift(uint8_t * buffer, uint16_t size);

#endif
