#include "kehua_transfer_arm.h"
#include "os_timer_event.h"
#include "jmesh_uart.h"
#include "jmesh_app_pro.h" 
#include "modbus_protocol.h"
#include "protocol_645.h"
#include "beidian_app_cfg.h"
#include "protocol_mx188.h"
#include "app_expand.h"
#include "io_ctrl.h"
#include "APP_protocol.h"
#include"../jmesh_gpio.h"
#include"../../jmesh/jmesh_save_define.h"
#include"../../jmesh/jmesh_save.h"


#if	(defined KEHUA_TRANSFER_ARM)||(defined KEHUA_ARM_BX2400_470)	

unsigned char arm_state;  // 0:关闭  1：打开
static os_timer_event_t stop_timer;	
static os_timer_event_t arm_report_timer;	

#ifdef KEHUA_ARM_BX2400_470
	#define ARM_CONTROL_M1 8
	#define ARM_CONTROL_M2 9
#else
	#define ARM_CONTROL_M1 21
	#define ARM_CONTROL_M2 22
#endif

//初始化IO
void arm_control_init(void)
{	
	io_pin_dir_set(ARM_CONTROL_M1,JMESH_GPIO_MODE_OUTPUT);
	io_pin_dir_set(ARM_CONTROL_M2,JMESH_GPIO_MODE_OUTPUT);
	
	if(arm_state == 0)
	{
		arm_control_off();
	}
	else
	{
		arm_control_on();
	}
}

//启动打开
void arm_control_on(void)
{
	io_pin_set(ARM_CONTROL_M1);
	io_pin_clear(ARM_CONTROL_M2);
	os_timer_event_set(&stop_timer, 30*1000, (os_timer_event_caller_t)stop_timer_handler,NULL);
}

//启动关闭
void arm_control_off(void)
{
	io_pin_set(ARM_CONTROL_M2);
	io_pin_clear(ARM_CONTROL_M1);
	os_timer_event_set(&stop_timer, 30*1000, (os_timer_event_caller_t)stop_timer_handler,NULL);
}

//停止操作
void arm_control_stop(void)
{
	io_pin_clear(ARM_CONTROL_M1);
	io_pin_clear(ARM_CONTROL_M2);
}


void stop_timer_handler(void* arg)
{
	arm_control_stop();
}

unsigned char transfer_arm_rev_data_check(unsigned char *data,unsigned short len)
{
	unsigned short offset;
	int ret;
	unsigned char ctr_code,buff[30],buff_1[50];
	unsigned char DI[4] = {0x34,0x33,0xCC,0xFF};	
	if(0<(ret=protocol_64507_pick(len,data,&offset)))
	{
		ctr_code = *(data+offset+8);
		switch(ctr_code)
		{	
			case 0x14:                                     
				if(memcmp(data+offset+10,DI,4) == 0)
				{	
					if(*(data+offset+22) == 0xDD)	//打开
					{	
						arm_control_on();
						arm_state = 1;
						jmesh_save_write(JMESH_SAVE_ARM_STATE,1,(unsigned char*)&arm_state);
						os_timer_event_restart(&arm_report_timer);
					}	
					else if(*(data+offset+22) == 0xEE)	//关闭
					{	
						arm_control_off();
						arm_state = 0;
						jmesh_save_write(JMESH_SAVE_ARM_STATE,1,(unsigned char*)&arm_state);
						os_timer_event_restart(&arm_report_timer);
					}
					else
					{
						return 0;
					}
					
					memcpy(buff,data+offset,8);
					buff[8] = 0x94;
					buff[9] = 0x00;
					buff[10] = check_sum(10,buff);
					buff[11] = 0x16;
					message_realcomm_server_data_return_handler(12,buff);
					return 1;
				}	
				break; 	
			case 0x11: 	
				if(memcmp(data+offset+10,DI,4) == 0)
				{	
					
					//68 11 11 11 11 11 11 68 81 09 34 33 CC FF DD 33 33 33 33 9B 16 
					memcpy(buff,data+offset,8);
					buff[8] = 0x81;
					buff[9] = 0x09;
					buff[10] = 0x34;
					buff[11] = 0x33;
					buff[12] = 0xCC;
					buff[13] = 0xFF;
					if(arm_state == 1)
					{
						buff[14] = 0xDD;
					}
					else
					{
						buff[14] = 0xEE;
					}
					buff[15] = 0x33;
					buff[16] = 0x33;
					buff[17] = 0x33;
					buff[18] = 0x33;
					buff[19] = check_sum(19,buff);
					buff[20] = 0x16;
					
					message_realcomm_server_data_return_handler(21,buff);
					return 1;
				}
				break; 			
			default:
				break;
		}
	
	}
	return 0;

}
	


static void report_arm_state_timer_handler(void* arg)
{	
	unsigned char buff[50];
	
	buff[0] = 0x68;
	memcpy(buff+1,meter_addr,6);
	buff[7] = 0x68;
	buff[8] = 0x81;
	buff[9] = 0x09;
	buff[10] = 0x34;
	buff[11] = 0x33;
	buff[12] = 0xCC;
	buff[13] = 0xFF;
	if(arm_state == 1)
	{
		buff[14] = 0xDD;
	}
	else
	{
		buff[14] = 0xEE;
	}
	buff[15] = 0x33;
	buff[16] = 0x33;
	buff[17] = 0x33;
	buff[18] = 0x33;
	buff[19] = check_sum(19,buff);
	buff[20] = 0x16;
	report_meter_handler(21, buff, 0);
	os_timer_event_restart(&arm_report_timer);	
}


// 启动定时上报机械手状态任务  
void report_kehua_arm_start(void)
{	
	os_timer_event_set(&arm_report_timer, 5*60*1000, (os_timer_event_caller_t)report_arm_state_timer_handler,NULL);
}

//APP直接控制处理
unsigned char APP_BD_B101_ARM_handler(unsigned char *data,unsigned short len,unsigned char rev_type)
{
	DL_APP_BD_FRAME_T *p;
	uint8 ctr;
	unsigned short offset;
	int ret;
	unsigned char ctr_code,buff[30],buff_1[50];
	unsigned char DI[4] = {0x34,0x33,0xCC,0xFF};	
		
	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
		ctr = 0x81;
	else if(p->ctr == 0x04)
		ctr = 0x84;
	if(0<(ret=protocol_64507_pick(p->load_len-2,p->data,&offset)))
	{
		ctr_code = *(p->data+offset+8);
		switch(ctr_code)
		{	
			case 0x14:                                     
				if(memcmp(p->data+offset+10,DI,4) == 0)
				{	
					if(*(p->data+offset+22) == 0xDD)	//打开
					{	
						arm_control_on();
						arm_state = 1;
						jmesh_save_write(JMESH_SAVE_ARM_STATE,1,(unsigned char*)&arm_state);
					}	
					else if(*(p->data+offset+22) == 0xEE)	//关闭
					{	
						arm_control_off();
						arm_state = 0;
						jmesh_save_write(JMESH_SAVE_ARM_STATE,1,(unsigned char*)&arm_state);
					}
					else
					{
						return 0;
					}
					
					memcpy(buff,p->data+offset,8);
					buff[8] = 0x94;
					buff[9] = 0x00;
					buff[10] = check_sum(10,buff);
					buff[11] = 0x16;
					
					app_local_read_meter.type = p->type;
					app_local_read_meter.ctr = ctr;
					memcpy(app_local_read_meter.addr, p->addr,6);
					app_local_read_meter.DT[0] = p->DT[0];
					app_local_read_meter.DT[1] = p->DT[1];
					app_local_read_meter.rev_type = rev_type;
					APP_BD_protocol_handler_callback(app_local_read_meter.type,
													app_local_read_meter.addr,
													app_local_read_meter.ctr,
													app_local_read_meter.DT[0],
													app_local_read_meter.DT[1],buff,12,app_local_read_meter.rev_type);
					return 1;
				}	
				break; 	
			case 0x11: 	
				if(memcmp(p->data+offset+10,DI,4) == 0)
				{	
					
					//68 11 11 11 11 11 11 68 81 09 34 33 CC FF DD 33 33 33 33 9B 16 
					memcpy(buff,p->data+offset,8);
					buff[8] = 0x81;
					buff[9] = 0x09;
					buff[10] = 0x34;
					buff[11] = 0x33;
					buff[12] = 0xCC;
					buff[13] = 0xFF;
					if(arm_state == 1)
					{
						buff[14] = 0xDD;
					}
					else
					{
						buff[14] = 0xEE;
					}
					buff[15] = 0x33;
					buff[16] = 0x33;
					buff[17] = 0x33;
					buff[18] = 0x33;
					buff[19] = check_sum(19,buff);
					buff[20] = 0x16;
					
					app_local_read_meter.type = p->type;
					app_local_read_meter.ctr = ctr;
					memcpy(app_local_read_meter.addr, p->addr,6);
					app_local_read_meter.DT[0] = p->DT[0];
					app_local_read_meter.DT[1] = p->DT[1];
					app_local_read_meter.rev_type = rev_type;
					APP_BD_protocol_handler_callback(app_local_read_meter.type,
													app_local_read_meter.addr,
													app_local_read_meter.ctr,
													app_local_read_meter.DT[0],
													app_local_read_meter.DT[1],buff,21,app_local_read_meter.rev_type);
					return 1;
				}
				break; 			
			default:
				break;
		}
	
	}
	return 0;
}

#endif
