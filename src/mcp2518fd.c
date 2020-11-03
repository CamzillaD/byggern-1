#include "mcp2518fd.h"
#include "spi.h"

#define MCP2518FD_SPI_RESET 0x00
#define MCP2518FD_SPI_READ  0x30
#define MCP2518FD_SPI_WRITE 0x20

void mcp2518fd_init(){
    /* Enter configuration mode */
    mcp2518fd_sfr_write(MCP_SFR_C1CON(3), 0x04);

    uint8_t buffer[2] = {
        MCP2518FD_SPI_RESET, 0x00
    };

    spi_shift(buffer, 2);
}

uint8_t mcp2518fd_sfr_read(uint16_t address){
    uint8_t buffer[3] = {
        MCP2518FD_SPI_READ | (uint8_t)((address >> 8) & 0x0f),
        (uint8_t)(address),
        0x00
    };

    spi_shift(buffer, 3);

    return buffer[2];
}

void mcp2518fd_sfr_write(uint16_t address, uint8_t data){
    uint8_t buffer[3] = {
        MCP2518FD_SPI_WRITE | (uint8_t)((address >> 8) & 0x0f),
        (uint8_t)(address),
        data
    };

    spi_shift(buffer, 3);
}

void mcp2518fd_mem_read(uint16_t address, uint8_t * p_word){
    uint8_t buffer[6] = {
        MCP2518FD_SPI_READ | (uint8_t)((address >> 8) & 0x0f),
        (uint8_t)(address),
        0x00, 0x00, 0x00, 0x00
    };

    spi_shift(buffer, 6);

    p_word[0] = buffer[2];
    p_word[1] = buffer[3];
    p_word[2] = buffer[4];
    p_word[3] = buffer[5];
}

void mcp2518fd_mem_write(uint16_t address, const uint8_t * p_word){
    uint8_t buffer[6] = {
        MCP2518FD_SPI_WRITE | (uint8_t)((address >> 8) & 0x0f),
        (uint8_t)(address),
        p_word[0], p_word[1], p_word[2], p_word[3]
    };

    spi_shift(buffer, 6);
}
