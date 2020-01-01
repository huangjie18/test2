#include"meter_server.h"
#include"meter_client.h"
#include"meter_client_callback.h"
#include"../../access/jmesh_access.h"
#include"../../access/jmesh_access_callback.h"
#include"../app_include.h"
#include"../../onchip_system/os_timer_event.h"
#include"../../foundation/config/configuration_cmd.h"
#include"stdio.h"
static void meter_client_response_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
JMESH_VENDOR_MODEL_CREATE_START(meter_client,JMESH_MODEL_COMPANY_ID,METER_CLIENT_MODEL_ID,1)
JMESH_MESSAGE_ADD(MESSAGE_OPCODE_METER_CLIENT_CMD_RESPONSE,meter_client_response_handler)
JMESH_MODEL_CREATE_END(meter_client)


void meter_client_init(void){
    jmesh_element_add_model(&element_app,&meter_client);
}

static void meter_client_response_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data)
{
    if(0==memcmp(data,"TIMEOUT\n",len)){
        JMESH_CMD_RES(cur_cmd_interface,"RESPONSE:TIMEOUT\n");
    }
    else{
        JMESH_CMD_RES_BUFFER(cur_cmd_interface,len,data,"RESPONSE:");
    }
}




void meter_client_cmd_send(unsigned short dst,unsigned short netkey_index,unsigned short appkey_index,unsigned short len,unsigned char* data){
    jmesh_access_send(dst,netkey_index,appkey_index,&meter_server,message_opcode_meter_server_cmd_send,len,data,JMESH_SEND_ALL_CHANNEL);
}


