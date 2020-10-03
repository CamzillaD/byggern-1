#include "can.h"
#include "mcp.h"



void can_init(){
    mcp_init();

    uint8_t mcp_config
        = MCP_CANCTRL_LOOPBACK
        | MCP_CANCTRL_ONESHOT
        | MCP_CANCTRL_PRESCALE_8
        ;
    mcp_write(MCP_CANCTRL, mcp_config);
}

uint8_t can_test(){
    return mcp_read(0x0f);
}
