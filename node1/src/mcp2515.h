#ifndef MCP2515_H
#define MCP2515_H
#include <stdint.h>

#define MCP_CANSTAT  0x0e
#define MCP_CANCTRL  0x0f

#define MCP_TXB0CTRL 0x30
#define MCP_TXB0SIDH 0x31
#define MCP_TXB0SIDL 0x32
#define MCP_TXB0EID8 0x33
#define MCP_TXB0EID0 0x34
#define MCP_TXB0DLC  0x35
#define MCP_TXB0D0   0x36

#define MCP_RXB0CTRL 0x60
#define MCP_RXB0SIDH 0x61
#define MCP_RXB0SIDL 0x62
#define MCP_RXB0DLC  0x65
#define MCP_RXB0DM   0x66
#define MCP_CANINTE  0x2B


/* Register Values */

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

#define MCP_TXB_MESSAGE_ABORTED 0x40
#define MCP_TXB_MESSAGE_LOST    0x20
#define MCP_TXB_TX_ERROR_DETECT 0x10
#define MCP_TXB_TX_REQUEST      0x08
#define MCP_TXB_TX_PRI_HIGHEST  0x03
#define MCP_TXB_TX_PRI_HIGH     0x02
#define MCP_TXB_TX_PRI_LOW      0x01
#define MCP_TXB_TX_PRI_LOWEST   0x00


void mcp_init();

uint8_t mcp_read(uint8_t address);

void mcp_write(uint8_t address, uint8_t data);

void mcp_bit_modify(uint8_t address, uint8_t bit, uint8_t data);

#endif
