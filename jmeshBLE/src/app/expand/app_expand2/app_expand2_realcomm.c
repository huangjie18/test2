#include"../../../driver/jmesh_uart.h"
#include "APP_protocol.h"

int jmesh_app_expand2_realcomm_set(unsigned char port,unsigned char baud,unsigned char parity)
{
    return 0;
}
void jmesh_app_expand2_realcomm_send(unsigned char port,unsigned short length,unsigned char* data)
{
    jmesh_uart_send((JMESH_UART_NAME_T)port, length, data);
		JMESH_DEBUG_SEND_TO_CKQ(length,data);
}
void jmesh_app_expand2_realcomm_recv(unsigned char port,unsigned short length,unsigned char* data)
{
    ;
}
