#ifndef MCP_H
#define MCP_H
#include <stdint.h>

#define MCP_TXB0CTRL 0x30
#define MCP_CANCTRL  0x0f

#define MCP_CANCTRL_NORMAL      0x00
#define MCP_CANCTRL_SLEEP       0x20
#define MCP_CANCTRL_LOOPBACK    0x40
#define MCP_CANCTRL_LISTEN      0x60
#define MCP_CANCTRL_CONFIG      0x80
#define MCP_CANCTRL_ABORT_TX    0x10
#define MCP_CANCTRL_ONESHOT     0x08
#define MCP_CANCTRL_CLKOUT      0x04
#define MCP_CANCTRL_PRESCALE_1  0x00
#define MCP_CANCTRL_PRESCALE_2  0x01
#define MCP_CANCTRL_PRESCALE_4  0x02
#define MCP_CANCTRL_PRESCALE_8  0x03

void mcp_init();

uint8_t mcp_read(uint8_t address);

void mcp_write(uint8_t address, uint8_t data);

#endif
