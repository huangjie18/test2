#include"jmesh_uart_callback.h"
#include"../foundation/config/configuration_cmd.h"
#include"../jmesh/jmesh_print.h"
#include"../app/expand/app_expand2/water/water_gate.h"
#ifndef x86
#include"FreeRTOS.h"
#include"semphr.h"
#endif
#include "protocol_645.h"
#include "jmesh_app_pro.h"
#include "APP_protocol.h"
#include "kehua_gas.h"
#include "kehua_window.h"
#include "beidian_app_cfg.h"
#include "jmesh_remote_control.h"
#include "jmesh_lamp.h"

//#define UART_REALCOMM_ENABLE
//void jmesh_uart_recv_callback(JMESH_UART_NAME_T uart,unsigned short len,unsigned char* data)
//{
//    meter_server_res_send(len,data);

//    /*if(0==water_gate_188_handler(JMESH_CMD_INTERFACE_UART,len,data)){
//        return;
//    }*/
//    configuration_cmd_process(JMESH_CMD_INTERFACE_UART,len,data);

//#ifdef UART_REALCOMM_ENABLE
//		if(memcmp("AT+",data,3)==0) return ;
//		//realcomm_client_uart_recv_callback(len,data);
//#endif
//}

void jmesh_uart_recv_callback(JMESH_UART_NAME_T uart,unsigned short len,unsigned char* data)
{
	unsigned char buff[255];
	memset(buff,0x00,sizeof(buff));
	if(len > sizeof(buff))
		memcpy(buff,data,sizeof(buff));
	else
		memcpy(buff,data,len);
	
	//meter_server_res_send(len,data);

	/*if(0==water_gate_188_handler(JMESH_CMD_INTERFACE_UART,len,data)){
			return;
	}*/
	configuration_cmd_process(JMESH_CMD_INTERFACE_UART, len, buff);
	if(APP_BD_protocol_handler(data,len,0) != 2)
	{
		return ;
	}

		#if(Scene_Control_Enable==1)
		if(scene_control_handler(data,len) == 1)
			return ;
		#endif
		
		#ifdef Three_Remote_Switch
		if(three_remote_control_handler(data,len) == 1)
			return ;
		#endif
		
		#ifdef Four_Remote_Switch
		if(four_remote_control_handler(data,len) == 1)
			return ;
		#endif
		
		#ifdef Single_Lamp_Ctrl
		if(lamp_return_handler(data,len) == 1)
			return ;
		#endif
		
		#ifdef Control_Switch
		if(lamp_return_handler(data,len) == 1)
			return ;
		#endif
		
		#ifdef LAMP_PASSTHROUGH
		if(lamp_passthrough_return_handler(data,len) == 1)
			return ;
		#endif
	if(app_read_meter_by_node_flag == 1)
	{
		app_read_meter_by_node_flag = 0;
		app_read_meter_by_node_uart_handler(len,data);
		return ;
	}	
	switch(current_realcomm_task_id)
	{
		#ifdef GET_ADDR_FROM_METER
		case SELF_GET_METER_ADDR:
			get_meter_addr_callback(data,len);
			break;
		#endif
		case PHONE_GATEWAY_READ_METER:
		case PHONE_DIRECT_READ_METER:
		case GATEWAY_READ_METER:
			message_realcomm_server_data_return_handler(len,data);
			break;
		#ifdef  KEHUA_GAS 
		case SELF_READ_GAS_CON:
		case SELF_READ_SENSOR_STATE:
			get_gas_data_return_handler(data,len);
			break;
		#endif
		#ifdef  KEHUA_WINDOW
		case SELF_READ_WINDOW_STATE:
			get_window_state_return_handler(data,len);
			break;
		#endif
		
		default:
			report_meter_handler( len, data, 0);
			break;		
	}	
		
}