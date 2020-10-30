#ifndef MCP2518FD_H
#define MCP2518FD_H
#include <stdint.h>

#define MCP_SFR_IOCON(byte) (0xe04 + byte)

void mcp2518fd_init();

uint8_t mcp2518fd_read(uint16_t address);

void mcp2518fd_write(uint16_t address, uint8_t data);

#endif
