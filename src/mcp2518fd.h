/**
 * @file mcp2518fd.h
 *
 * @brief Driver for the MCP2518FD.
 */
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


/**
 * @brief Initialize the MCP2518FD chip to serve in CAN 2.0
 * mode, without supporting Remote Frames, Extended Identifiers,
 * or Flexible Datarate (FD).
 *
 * @warning The MCP2518FD is operated via SPI. Hence, a call
 * to @p spi_init must be made before invoking this function.
 */
void mcp2518fd_init();

/**
 * @brief Read the MCP2518FD special function register (SFR)
 * given by address @p address.
 *
 * @param address Address of SFR to be read.
 *
 * @return Contents of SFR specified by @p address.
 */
uint8_t mcp2518fd_sfr_read(uint16_t address);

/**
 * @brief Write the MCP2518FD special function register (SFR)
 * given by address @p address with the data given by @p data.
 *
 * @param address Address of SFR to be written.
 * @param data Data to write into specified SFR.
 */
void mcp2518fd_sfr_write(uint16_t address, uint8_t data);

/**
 * @brief Read random access memory (RAM) contents off the
 * MCP2518FD chip. A whole word (4 bytes) is read with each
 * call to this function.
 *
 * @param address Address of RAM location to read from.
 * @param[out] p_word Pointer to a buffer where the contents
 * of the RAM word will be placed after they are read from
 * the MCP2518FD.
 *
 * @warning @p p_word must point to a buffer of at least 4
 * bytes, as this function always reads one full word (4 bytes).
 *
 * @warning The MCP2518FD can only do word aligned RAM
 * accesses. Hence, @p address must specify a valid word
 * boundary.
 */
void mcp2518fd_mem_read(uint16_t address, uint8_t * p_word);

/**
 * @brief Write a random access memory (RAM) word into the
 * location specified by @p address. Data is read from the
 * buffer pointed to by @p p_word.
 *
 * @param address Address of RAM location to write to.
 * @param[in] p_word Pointer to a buffer where data will be
 * taken from and written into the MCP2518FD RAM.
 *
 * @warning A whole word (4 bytes) is always written by this
 * function. As such, @p p_word must point to a buffer of at
 * least 4 valid bytes. Otherwise, the MCP2518FD RAM might
 * be corrupted - and the call might trigger a reset on
 * microcontrollers with a memory protection unit (MPU).
 *
 * @warning The MCP2518FD can only do word aligned RAM
 * accesses. Hence, @p address must specify a valid word
 * boundary.
 */
void mcp2518fd_mem_write(uint16_t address, const uint8_t * p_word);

#endif
