#ifndef MCP_H
#define MCP_H
#include <stdint.h>

#define MCP_TXB0CTRL 0x30
#define MCP_CANCTRL  0x0f

void mcp_init();

uint8_t mcp_read(uint8_t address);

void mcp_write(uint8_t address, uint8_t data);

#endif