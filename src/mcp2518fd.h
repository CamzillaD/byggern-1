#ifndef MCP2518FD_H
#define MCP2518FD_H
#include <stdint.h>

#define MCP_SFR_OSC(byte) (0xe00 + byte)
#define MCP_SFR_IOCON(byte) (0xe04 + byte)
#define MCP_SFR_CRC(byte) (0xe08 + byte)
#define MCP_SFR_ECCCON(byte) (0xe0c + byte)
#define MCP_SFR_ECCSTAT(byte) (0xe10 + byte)
#define MCP_SFR_DEVID(byte) (0xe14 + byte)

#define MCP_SFR_C1CON(byte) (0x000 + byte)
#define MCP_SFR_C1NBTCFG(byte) (0x004 + byte)
#define MCP_SFR_C1DBTCFG(byte) (0x008 + byte)
#define MCP_SFR_C1TDC(byte) (0x00c + byte)
#define MCP_SFR_C1TBC(byte) (0x010 + byte)
#define MCP_SFR_C1TSCON(byte) (0x014 + byte)
#define MCP_SFR_C1VEC(byte) (0x018 + byte)
#define MCP_SFR_C1INT(byte) (0x01c + byte)
#define MCP_SFR_C1RXIF(byte) (0x020 + byte)
#define MCP_SFR_C1TXIF(byte) (0x024 + byte)
#define MCP_SFR_C1RXOVIF(byte) (0x028 + byte)
#define MCP_SFR_C1TXATIF(byte) (0x02c + byte)
#define MCP_SFR_C1TXREQ(byte) (0x030 + byte)
#define MCP_SFR_C1TREC(byte) (0x034 + byte)
#define MCP_SFR_C1BDIAG0(byte) (0x038 + byte)
#define MCP_SFR_C1BDIAG1(byte) (0x03c + byte)
#define MCP_SFR_C1TEFCON(byte) (0x040 + byte)
#define MCP_SFR_C1TEFSTA(byte) (0x044 + byte)
#define MCP_SFR_C1TEFUA(byte) (0x048 + byte)

#define MCP_SFR_C1TXQCON(byte) (0x050 + byte)
#define MCP_SFR_C1TXQSTA(byte) (0x054 + byte)
#define MCP_SFR_C1TXQUA(byte) (0x058 + byte)

#define MCP_SFR_C1FIFOCON(n, byte) (0x05c - 12 + 12 * n + byte)
#define MCP_SFR_C1FIFOSTA(n, byte) (0x060 - 12 + 12 * n + byte)
#define MCP_SFR_C1FIFOUA(n, byte) (0x064 - 12 + 12 * n + byte)

#define MCP_SFR_C1FLTCON(n, byte) (0x1d0 + 4 * n + byte)
#define MCP_SFR_FLTOBJ(n, byte) (0x1f0 + 8 * n + byte)
#define MCP_SFR_C1MASK(n, byte) (0x1f4 + 8 * n + byte)


void mcp2518fd_init();

uint8_t mcp2518fd_sfr_read(uint16_t address);

void mcp2518fd_sfr_write(uint16_t address, uint8_t data);

void mcp2518fd_mem_read(uint16_t address, uint8_t * p_word);

void mcp2518fd_mem_write(uint16_t address, const uint8_t * p_word);

#endif
