
#include"../../../jmesh/jmesh_types.h"
#include"../../../access/jmesh_access.h"
#include"../../../jmesh/jmesh_print.h"
#include"app_expand2_realcomm_client.h"
#include"app_expand2_realcomm_server.h"
#include"app_expand2_realcomm.h"
#include"../app_expand.h"
#include"app_expand2.h"
#include"jmesh_app_pro.h"
#include"jmesh_uart.h"
#include "APP_protocol.h"

unsigned short ext_realcomm_src = 0;

void message_realcomm_server_data_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void app_gateway_read_meter_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);


#ifdef JMESH_APP_EXPAND2_REALCOMM_SERVER

static unsigned short realcomm_return_addr[JMESH_APP_EXPAND2_REALCOMM_PORT_SIZE];/**< add a flow timer maybe */
static unsigned short realcomm_report_addr[JMESH_APP_EXPAND2_REALCOMM_PORT_SIZE];

static void app_realcomm_server_baudrate_set_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
static void app_realcomm_server_report_set_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
static void app_realcomm_server_transparent_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
static void app_realcomm_server_trans_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);

void jmesh_app_expand2_realcomm_server_init(void)
{

    static app_expand_message_t baud;
    static app_expand_message_t report;
    static app_expand_message_t transparent;
		static app_expand_message_t trans;
	
		static app_expand_message_t message_realcomm_data_send;
		static app_expand_message_t message_realcomm_get_meter_addr;
		static app_expand_message_t message_realcomm_app_read_meter;
		static app_expand_message_t message_realcomm_app_network_maintain;
		static app_expand_message_t message_realcomm_app_broadcast_data;
		static app_expand_message_t message_realcomm_app_set_meter;	
		static app_expand_message_t message_realcomm_app_read_meter_by_node;
		static app_expand_message_t message_realcomm_app_read_meter_by_node_return;


    memset(realcomm_report_addr,0,sizeof(realcomm_report_addr));
    memset(realcomm_return_addr,0,sizeof(realcomm_return_addr));

    jmesh_app_expand2_message_register(&baud,   JMESH_APP_EXPAND2_REALCOMM_BAUDRATE_SET,    app_realcomm_server_baudrate_set_handler);
    jmesh_app_expand2_message_register(&report, JMESH_APP_EXPAND2_REALCOMM_REPORT_SET,      app_realcomm_server_report_set_handler);
    jmesh_app_expand2_message_register(&transparent,  JMESH_APP_EXPAND2_TRANSPARENT_TRANS,           app_realcomm_server_transparent_handler);
		jmesh_app_expand2_message_register(&trans,  JMESH_APP_EXPAND2_REALCOMM_TRANS,           app_realcomm_server_trans_handler);

		jmesh_app_expand2_message_register(&message_realcomm_data_send,expand_opcode_realcomm_data,message_realcomm_server_data_handler);
		jmesh_app_expand2_message_register(&message_realcomm_get_meter_addr,expand_opcode_realcomm_get_meter_addr,realcomm_get_meter_addr_handler);
		jmesh_app_expand2_message_register(&message_realcomm_app_read_meter,expand_opcode_realcomm_app_read_meter,app_gateway_read_meter_handler);
//	jmesh_app_expand2_message_register(&message_realcomm_app_read_meter,expand_opcode_realcomm_app_read_meter,app_read_meter_handler);
		jmesh_app_expand2_message_register(&message_realcomm_app_network_maintain,expand_opcode_realcomm_app_network_maintain,app_network_maintain_handler);
		jmesh_app_expand2_message_register(&message_realcomm_app_broadcast_data,expand_opcode_realcomm_app_broadcast_data,app_broadcast_data_handler);
		jmesh_app_expand2_message_register(&message_realcomm_app_set_meter,expand_opcode_realcomm_app_set_meter,app_set_meter_handler);
		jmesh_app_expand2_message_register(&message_realcomm_app_read_meter_by_node,expand_opcode_realcomm_app_read_meter_by_node,app_read_meter_by_node_handler);
		jmesh_app_expand2_message_register(&message_realcomm_app_read_meter_by_node_return,expand_opcode_realcomm_app_read_meter_by_node_return,app_read_meter_by_node_return_handler);

}

void message_realcomm_server_data_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
//	jmesh_uart_send(JMESH_UART_0,sizeof(data), data);
	ext_realcomm_src = src;
	realcomm_send_save_handler(src,dst,len,data);
	read_meter_handler(len,data,0);	
}

void app_gateway_read_meter_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
//	jmesh_uart_send(JMESH_UART_0,sizeof(data), data);
	ext_realcomm_src = src;
	realcomm_send_save_handler(src,dst,len,data);
	read_meter_handler(len, data, 1);
}

static void app_realcomm_server_baudrate_set_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned char buffer[4];
    if(len==3){

        JMESH_LOGI("realcomm","src 0x%x set port %d baud %d parity %d\n",src,data[0],data[1],data[2]);
        buffer[0]=jmesh_app_expand2_realcomm_set(data[0],data[1],data[2]);
        buffer[1]=data[0];
        buffer[2]=data[1];
        buffer[3]=data[2];
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_REALCOMM_BAUDRATE_STATUS,4,buffer);
    }
}
static void app_realcomm_server_report_set_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned char buffer[3];
    unsigned short report_addr;
    if(len==3){
        JMESH_BIG_ENDIAN_PICK2(report_addr,data+1);
        if(data[0]<JMESH_APP_EXPAND2_REALCOMM_PORT_SIZE){
            realcomm_report_addr[data[0]]=report_addr;
        }
        buffer[0]=0;
        JMESH_BIG_ENDIAN_FILL2(report_addr,buffer+1);
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_REALCOMM_REPORT_STATUS,3,buffer);
    }
}
static void app_realcomm_server_transparent_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    if(len>1 && data[0]<JMESH_APP_EXPAND2_REALCOMM_PORT_SIZE){
        realcomm_return_addr[data[0]]=src;
        jmesh_app_expand2_realcomm_send(data[0],len-1,data+1);
    }
}
static void app_realcomm_server_trans_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
	unsigned char buff[273]; 
	unsigned short head_len=0;
    if(len>1 && data[0]<JMESH_APP_EXPAND2_REALCOMM_PORT_SIZE){
		
        realcomm_return_addr[data[0]]=src;
		sprintf((char *)buff,"source addr:%d,",src-1);
		head_len=strlen((char *)buff);
		if((head_len+len-1)<=273){
            memcpy(buff+head_len,data+1,len-1);	
            jmesh_app_expand2_realcomm_send(data[0],head_len+len-1,buff);
		}
		else{
		    memcpy(buff+head_len,data+1,273-head_len);	         
            jmesh_app_expand2_realcomm_send(data[0],273,buff);
		}
    }
	

}

void jmesh_app_expand2_realcomm_return(unsigned char port,unsigned char length,unsigned char* data)
{
    if(port<JMESH_APP_EXPAND2_REALCOMM_PORT_SIZE && length!=0){
        jmesh_app_expand2_realcomm_client_trans(realcomm_return_addr[port],port,length,data);
    }
}
void jmesh_app_expand2_realcomm_report(unsigned char port,unsigned char length,unsigned char* data)
{
    if(port<JMESH_APP_EXPAND2_REALCOMM_PORT_SIZE && length!=0){
        jmesh_app_expand2_realcomm_client_trans(realcomm_report_addr[port],port,length,data);
    }
}

#endif
