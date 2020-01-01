
#include"app_expand2.h"
#include"app_expand2_realcomm.h"
#include"app_expand2_realcomm_client.h"
#include"app_expand2_realcomm_server.h"

#include"../../../jmesh/jmesh_types.h"
#include"../../../access/jmesh_access.h"
#include"../app_expand.h"
#include"jmesh_app_pro.h"

#ifdef JMESH_APP_EXPAND2_REALCOMM_CLIENT

void realcomm_client_baudrate_set_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void realcomm_client_report_set_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);

void jmesh_app_expand2_realcomm_client_init(void)
{
    static app_expand_message_t baud_status ;
    static app_expand_message_t report_status;
		static app_expand_message_t message_realcomm_meter_reprot_return;

    jmesh_app_expand2_message_register(&baud_status,    JMESH_APP_EXPAND2_REALCOMM_BAUDRATE_STATUS, realcomm_client_baudrate_set_status_handler);
    jmesh_app_expand2_message_register(&report_status,  JMESH_APP_EXPAND2_REALCOMM_REPORT_STATUS,   realcomm_client_report_set_status_handler);
		jmesh_app_expand2_message_register(&message_realcomm_meter_reprot_return,expand_opcode_realcomm_meter_reprot_return,report_meter_return_handler);

}

void jmesh_app_expand2_realcomm_client_set_baudrate(unsigned short dst,unsigned char port,unsigned char baud,unsigned char parity)
{
    unsigned char buffer[3];
    buffer[0]=port;
    buffer[1]=baud;
    buffer[2]=parity;
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_REALCOMM_BAUDRATE_SET,3,buffer);
}

void jmesh_app_expand2_realcomm_client_set_report(unsigned short dst,unsigned char port,unsigned short addr)
{
    unsigned char buffer[3];
    buffer[0]=port;
    JMESH_BIG_ENDIAN_FILL2(addr,buffer+1);

    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_REALCOMM_REPORT_SET,3,buffer);
}

void jmesh_app_expand2_transparent_client_trans(unsigned short dst,unsigned char port,unsigned char len,unsigned char* data)
{
    unsigned char buffer[257];
    buffer[0]=port;
    memcpy(buffer+1,data,len);
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_TRANSPARENT_TRANS,len+1,buffer);
}

void jmesh_app_expand2_realcomm_client_trans(unsigned short dst,unsigned char port,unsigned char len,unsigned char* data)
{
    unsigned char buffer[257];
    buffer[0]=port;
    memcpy(buffer+1,data,len);
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_REALCOMM_TRANS,len+1,buffer);	
    
}

void realcomm_client_baudrate_set_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    JMESH_LOGI_BUFFER("realcomm",len,data,"0x%x baud status:");
}
void realcomm_client_report_set_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    JMESH_LOGI_BUFFER("realcomm",len,data,"0x%x report status:");
}


#endif
