#include"meter_server.h"
#include"meter_client.h"
#include"meter_server_callback.h"
#include"protocol_645.h"
#include"../../access/jmesh_access.h"
#include"../../jmesh/jmesh_print.h"
#include"../app_include.h"
#include"stdio.h"
#include"stdlib.h"
#include"../../driver/jmesh_uart.h"
static void meter_server_cmd_send_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
JMESH_VENDOR_MODEL_CREATE_START(meter_server,JMESH_MODEL_COMPANY_ID,METER_SERVER_MODEL_ID,1)
JMESH_MESSAGE_ADD(MESSAGE_OPCODE_METER_SERVER_CMD_SEND,     meter_server_cmd_send_handler)
JMESH_MODEL_CREATE_END(meter_server)

static unsigned short cmd_src=0;
static os_timer_event_t timeout_timer;

void meter_server_init(void){
    jmesh_element_add_model(&element_app,&meter_server);
}

void meter_cmd_res_timeout_handler(void)
{
    jmesh_access_send(cmd_src,0,1,&meter_client,message_opcode_meter_client_cmd_res,strlen("TIMEOUT\n"),"TIMEOUT\n",JMESH_SEND_ALL_CHANNEL);

}

static void meter_server_cmd_send_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data)
{
    cmd_src=src;
    os_timer_event_set(&timeout_timer,3000,(os_timer_event_caller_t)meter_cmd_res_timeout_handler,NULL);
    jmesh_uart_send(JMESH_UART_0,len,data);
}

void meter_server_res_send(unsigned short len,unsigned char* data){
    unsigned short offset;
    if(protocol_64507_pick(len,data,&offset)){
        os_timer_event_remove(&timeout_timer);
        jmesh_access_send(cmd_src,0,1,&meter_client,message_opcode_meter_client_cmd_res,len,data,JMESH_SEND_ALL_CHANNEL);
    }
}
