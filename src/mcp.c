#include "mcp.h"
#include "spi.h"

#define MCP_RESET 0xc0
#define MCP_READ  0x03
#define MCP_WRITE 0x02

void mcp_init(){
    spi_init();

    uint8_t command = MCP_RESET;
    spi_shift(&command, 1);
}

uint8_t mcp_read(uint8_t address){
    uint8_t command[] = {MCP_READ, address, 0x00};
    spi_shift(command, 3);

    return command[2];
}

void mcp_write(uint8_t address, uint8_t data){
    uint8_t command[] = {MCP_WRITE, address, data};
    spi_shift(command, 3);
}
