#include "network.h"
#include "mcp2518fd.h"

void network_init(){
    /* Enable MCP2518FD GPIO1 (Disable ~INT1) */
    mcp2518fd_write(MCP_SFR_IOCON(3), 0x02);

    /* Use MCP2518FD GPIO1 as output */
    mcp2518fd_write(MCP_SFR_IOCON(0), 0x00);
}

void network_indicate(NetworkState state){
    switch(state){
        case NETWORK_STATE_CONNECTED:
            mcp2518fd_write(MCP_SFR_IOCON(1), 0x00);
            break;

        case NETWORK_STATE_DISCONNECTED:
            mcp2518fd_write(MCP_SFR_IOCON(1), 0x02);
            break;
    }
}
