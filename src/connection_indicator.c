#include "connection_indicator.h"
#include "mcp2518fd.h"

void connection_indicator_init(){
    /* Enable MCP2518FD GPIO1 (Disable ~INT1) */
    mcp2518fd_sfr_write(MCP_SFR_IOCON(3), 0x02);

    /* Use MCP2518FD GPIO1 as output */
    mcp2518fd_sfr_write(MCP_SFR_IOCON(0), 0x00);
}

void connection_indicator_turn_on(){
    mcp2518fd_sfr_write(MCP_SFR_IOCON(1), 0x00);
}

void connection_indicator_turn_off(){
    mcp2518fd_sfr_write(MCP_SFR_IOCON(1), 0x02);
}
