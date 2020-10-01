#include "can.h"
#include "mcp.h"



void can_init(){
    mcp_init();

    /* Loop-back one-shot mode */
    mcp_write(MCP_CANCTRL, 0x43);

}

uint8_t can_test(){
    return mcp_read(0x0f);
}