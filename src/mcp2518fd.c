#include "mcp2518fd.h"
#include "spi.h"

#define MCP2518FD_SPI_RESET 0x00
#define MCP2518FD_SPI_READ  0x30
#define MCP2518FD_SPI_WRITE 0x20

void mcp2518fd_init(){
    spi_init();

    uint8_t reset = MCP2518FD_SPI_RESET;
    spi_shift(&reset, 1);
}

uint8_t mcp2518fd_read(uint16_t address){

}

void mcp2518fd_write(uint16_t address, uint8_t data){
    uint8_t buffer[3] = {
        MCP2518FD_SPI_WRITE | (uint8_t)((address >> 8) & 0x0f),
        (uint8_t)(address),
        data
    };

    spi_shift(buffer, 3);
}
