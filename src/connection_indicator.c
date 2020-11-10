#include "connection_indicator.h"
#include "mcp2518fd.h"

void connection_indicator_turn_on(){
    mcp2518fd_sfr_write(MCP_SFR_IOCON(1), 0x00);
}

void connection_indicator_turn_off(){
    mcp2518fd_sfr_write(MCP_SFR_IOCON(1), 0x02);
}
