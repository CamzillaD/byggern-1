#include "can.h"
#include "mcp2515.h"

void can_init(){
    mcp_init();

    uint8_t mcp_config
        = MCP_CANCTRL_LOOPBACK
        | MCP_CANCTRL_ONESHOT
        | MCP_CANCTRL_PRESCALE_8
        ;
    mcp_write(MCP_CANCTRL, mcp_config);

    /* TODO Jon and Camilla: */
    /* Enable interrupts on receieve */
    /* Disable interrputs on send */
}

void can_send(const CanFrame * p_frame){
    /*
    TODO: Place the MCP2515 in normal mode and call
    this function a _couple_ of times, while checking
    the CANH and CANL (high and low) lines for activity.

    This will only work if the MCP2515 is in one-shot
    mode, otherwise it will attempt a retransfer, which
    will very quickly bring it into a failed state, since
    we don't have any other nodes on the bus to acknowledge
    our transmissions.

    Do this after we have implemented can_recv and managed
    to receive messages in loopback mode.
    */

    mcp_write(MCP_TXB0SIDH, (uint8_t)(p_frame->id >> 3));
    mcp_write(MCP_TXB0SIDL, (uint8_t)(p_frame->id << 5));

    mcp_write(MCP_TXB0DLC, p_frame->size & 0x0f);

    for(uint8_t i = 0; i < p_frame->size; i++){
        mcp_write(MCP_TXB0D0 + i, p_frame->buffer[i]);
    }

    mcp_bit_modify(MCP_TXB0CTRL, MCP_TXB_TX_REQUEST, 1);
}

uint8_t can_recv(CanFrame * p_frame){
    /*
    Suggestion for implementation:

    You guys take in a pointer to a CAN frame, namely p_frame.
    p_frame->size must be set to indicate how big p_frame->buffer
    is, since this is not controlled by this function, but the
    environment that called this function. This way we don't have
    to do malloc on a microcontroller :)

    You then follow the steps outlined in chapter 4.0 "Message Reception"
    in the MCP2515 data sheet. If the received message has more data bytes
    than p_frame->size, you must stop reception, and return 1 to indicate
    that the function did not run as expected.
    If p_frame->size is large enough to accomodate all the received bytes,
    you simply copy over all the bytes you got, and return 0 to indicate
    that the function ran successfully.

    FAQ:
    -> Why call the function "can_recv" and not "can_receive"?
    Sadly, this is the naming convention that people have settled for in
    other areas, so we follow the status quo here.

    -> Why return 0 to indicate success, and not 1?
    Oftentimes, a function can fail in many ways, but only succeed in one.
    So, we reserve 0 to indicate "no errors", and we keep all the other
    numbers for possible error codes.
    You guys might have seen this when trying to execute commands on Linux,
    or when you request web pages. The dreaded "404" is just one such
    error code.

    -> Why not just use malloc and be done with it?
    Calls to malloc will incur huge penalties for us on a microcontroller,
    so we'd rather not, unless we for some reason needed dynamic memory.
    Even in those cases, we might actually be better off writing our own
    implementation of a heap-like structure.

    -> Do we have to follow the suggested implementation?
    Of course not, you are free to do whatever you feel pertinent, I
    will not be a nazi on this ;)

    Happy coding you two!
    */

    return 1;
}
