#include"protocol_645.h"
#include"jmesh_app_pro.h"
#include"jmesh_config.h"
#include"../jmesh/jmesh_save.h"
#include"string.h"
#include"../jmesh_uart.h"
#include"../../onchip_system/os_timer_event.h"
#include"../../access/jmesh_access.h"
#include"../../driver/jmesh_system.h"
#include"../../jmesh/jmesh_save_define.h"
#include"../../jmesh/jmesh_save.h"
//#include"realcomm_client_callback.h"

#include"app_expand2.h"
#include"app_expand2_realcomm_client.h"
#include"app_expand2_realcomm_server.h"

#include"../../jmesh/jmesh_print.h"
#include"app_expand.h"
#include"../app_include.h"
#include "mx_datatype.h"
#include"../jmesh_gpio.h"
#include "arch.h"  
#include "APP_protocol.h"
#include "kehua_gas.h"
#include "beidian_app_cfg.h"
#include "si4438_hal.h"
#include "_470_phy.h"
#include "_470_app.h"
#include "app_config.h"
#include "kehua_window.h"
#include "kehua_transfer_arm.h"
#include "jmesh_remote_control.h"
#include "jmesh_button.h"


realcomm_send_save_t  realcomm_send_save;
unsigned char get_meter_addr_flag;
os_timer_event_t get_meter_addr_timer;
os_timer_event_t get_meter_addr_timer1;
os_timer_event_t read_meter_timer;
os_timer_event_t app_read_meter_timer;
os_timer_event_t report_meter_timer;
os_timer_event_t app_network_maintain_timer;
os_timer_event_t check_node_state_timer;
unsigned char get_meter_addr_repeat_num;
unsigned char report_meter_repeat_num;
unsigned char meter_addr[6];
unsigned char report_meter_data[200];
unsigned char report_meter_data_len;
os_timer_event_t led_timer;
unsigned char read_meter_flag;							//网关主动抄表
unsigned char app_read_meter_flag;					//APP通过网关抄表
unsigned char app_local_read_meter_flag;		//APP直接抄表
UINT8 JZQ_communication_address[6];
unsigned char connect_state;

uint8_t current_realcomm_task_id = NO_TASK;
unsigned char read_meter_data_cache[256];
unsigned char read_meter_data_cache_len;
unsigned char read_meter_delay_para[2]; // 第一个参数非0表示延迟抄表，第二个参数为任务id
mx188_frame_t mx188_frame_record = {0};
unsigned char blueToothOr470;
	
unsigned char report_content;
volatile unsigned char report_state = 0;
os_timer_event_t  app_task_timer;

//static const unsigned char get_addr_databuff[16]={0xFE,0xFE,0xFE,0xFE,0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x13,0x00,0xDF,0x16};//读通信地址
unsigned char get_addr_databuff[16]={0xFE,0xFE,0xFE,0xFE,0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x13,0x00,0xDF,0x16};//读通信地址
JMESH_UART_BAUD_T uart_baudrate[4] = {JMESH_UART_BAUD_1200,JMESH_UART_BAUD_2400,JMESH_UART_BAUD_4800,JMESH_UART_BAUD_9600};
static unsigned char uart_baudrate_index = 1;
os_timer_event_t broadcast_reset_timer;

//延时：ms
void delay_ms(uint16_t ms)
{
	volatile uint16_t delay;
	volatile uint16_t j;
	for(delay=0;delay<ms;delay++)
	{
		for(j=0;j<1000;j++)
		{
			__nop();
			__nop();
			__nop();
			__nop();
		}
	}
}

//系统复位
void system_reset(void)
{
	platform_reset(0);
}

//密钥初始化
void key_init(void)
{
//	unsigned char key[16]={0x66,0x22,0x33,0x44,0x11,0x11,0x11,0x11,0x22,0x11,0x22,0x22,0x33,0x33,0x33,0x66};
	unsigned char key[16]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};	
	if(JMESH_STATUS_SUCCESS!=jmesh_netkey_state_get(0,key))
  {
		jmesh_netkey_state_set(0,key);
  }
  if(NULL==jmesh_appkey_get_by_key_index(0,1))
  {
  		memset(key, 0, 16);
		jmesh_appkey_set(0,1,key);
  }
}

//OTA广播
static os_timer_event_t ota_adv_timer; 
void send_ota_adv(void)
{
	os_timer_event_set(&ota_adv_timer,1000*jmesh_random_get(3,5),(os_timer_event_caller_t)send_ota_adv,NULL);
	jmesh_adv_send(JMESH_ADV_TYPE_OTA,1,NULL,JMESH_ADV_REPEAT_NUM);
}

//参数初始化
void ApsMemoryParaRead(void)
{
	realcomm_send_read_handler();
	if(0 == jmesh_save_read(JMESH_JZQ_ADDR_SAVE,6,JZQ_communication_address))
	{
		memset(JZQ_communication_address,0x00,6);
	}
	if(0 == jmesh_save_read(JMESH_SAVE_APP_USER_ACCOUNT,12,APP_User_Account))		//add by ljj 20190410
	{
		memset(APP_User_Account,0x00,12);
	}	//end add
	
	if ((0 == jmesh_save_read(JMESH_SAVE_WIRLESS_TECH,1, (uint8_t *)&wirless_tech)) || (wirless_tech.tech >= TECH_NUM))
	{
		wirless_tech.tech = TECH_BLE_470;//TECH_BLE_470;//TECH_BLE;
		wirless_tech.synchronous = 0;
	}
	
	#ifdef  KEHUA_GAS
	if((0 == jmesh_save_read(JMESH_SAVE_REPORT_INTERVAL,1,&report_interval_m)) || (report_interval_m < 1))
	{
		report_interval_m = 5;
	}
	if((0 == jmesh_save_read(JMESH_SAVE_COLLECT_INTERVAL,1,&collect_interval_s)) || (collect_interval_s < 2))
	{
		collect_interval_s = 5;
	}
	#endif
	
	#ifdef  KEHUA_WINDOW
	if(0 == jmesh_save_read(JMESH_SAVE_REPORT_PERIOD,1,&window_report_period))
	{
		window_report_period = 5;
	}
	if(0 == jmesh_save_read(JMESH_SAVE_REPORT_COUNT,1,&window_report_count))
	{
		window_report_count = 4;
	}
	#endif
	
	#if	(defined KEHUA_TRANSFER_ARM)||(defined KEHUA_ARM_BX2400_470)
	if(0 == jmesh_save_read(JMESH_SAVE_ARM_STATE,1,&arm_state))
	{
		arm_state = 0;
	}
	if ((0 == jmesh_save_read(JMESH_SAVE_WIRLESS_TECH,1, (uint8_t *)&wirless_tech)) || (wirless_tech.tech >= TECH_NUM))
	{
		wirless_tech.tech = TECH_BLE_470;//TECH_BLE;//TECH_BLE_470;
		wirless_tech.synchronous = 0;
	}
	#endif
	
	#if (defined Three_Remote_Switch)||(defined Ble_Remote_Switch)
	Remote_Memeory_Init();
	if(0 == jmesh_save_read(JMESH_SAVE_REMOTE_MODE_INFO,1,&remote_mode))
	{
		remote_mode = 0;
	}
	#endif
	
	#ifdef Scene_Panel
	Panel_Memeory_Init();
	if(0 == jmesh_save_read(JMESH_SAVE_SCENE_MODE_INFO,1,&scene_mode))
	{
		scene_mode = 0;
	}
	#endif
	
	if(0 == jmesh_save_read(JMESH_LOG_TYPE_COUNT, 1, &jmesh_log_type))
	{
		jmesh_log_type = 0;
	}	
}
void reset_record(void)
{
	unsigned char reset_cnt[2] = {0};
	jmesh_save_read(JMESH_SAVE_RESET_COUNT, 2, reset_cnt);
	reset_cnt[0]++;
	jmesh_save_write(JMESH_SAVE_RESET_COUNT, 2, reset_cnt);
}

void app_task_timer_handler(void)
{
	os_timer_event_restart(&app_task_timer);
	if (wirless_tech.tech == TECH_BLE_470)
	{
		_470_app_recv();
	}
	#ifdef  KEHUA_GAS 
	kehua_gas_task();
	#endif
	#ifdef KEHUA_WINDOW
	kehua_window_task();
	#endif
			
	#ifdef Ble_Remote_Switch
	jmesh_remote_button_handle(trg,cont);
	#endif
}

void app_task_timer_start(void)
{
	os_timer_event_set(&app_task_timer,10,(os_timer_event_caller_t)app_task_timer_handler,NULL);
}

//应用层初始化
void ApsInit(void)
{
	report_meter_repeat_num = 0;
	connect_state = 0;
	current_realcomm_task_id = NO_TASK;
	app_network_maintain_start();
	app_conn_adv_start();	
	send_ota_adv();
	ApsMemoryParaRead();
	key_init();
	check_node_state_timer_start();			
	reset_record();
//	os_timer_event_set(&led_timer, 500, (os_timer_event_caller_t)led_timer_handler, NULL);
	#ifdef GET_ADDR_FROM_METER
	get_meter_addr_task_start();
	#else
	{
		uint8_t mac_addr[6] ={0};
		jmesh_mac_get(mac_addr);		
		memcpy(meter_addr, mac_addr, 6);
	}
	#endif // endif GET_ADDR_FROM_METER
	#ifdef  KEHUA_GAS 	
	kehua_gas_start();
	#endif // endif KEHUA_GAS
	#ifdef KEHUA_WINDOW
	report_kehua_window_start();
	#endif
	#if	(defined KEHUA_TRANSFER_ARM)||(defined KEHUA_ARM_BX2400_470)
	arm_control_init();
	report_kehua_arm_start();
	#endif
	#if(Lamp_Control_Enable==1)
	lamp_control_init();
	lamp_mode_info_read();
	#endif
	
	#ifdef	Ble_Remote_Switch
	buttom_init();
	jmesh_button_scan_task_start();
	button_state_buff_send_timer_statr();
	button_flag_clear_timer_statr();
	#endif

	#ifdef	Scene_Panel
	buttom_init();
	jmesh_button_scan_task_start();
	button_state_buff_send_timer_statr();
	button_flag_clear_timer_statr();
	#endif
	
	#ifdef	LAMP_PASSTHROUGH
	lamp_mode_info_read();
	#endif
	
	if (wirless_tech.tech == TECH_BLE_470)
	{
		radio_port_init();
		phy_init();
	}
	app_task_timer_start();
}

//发送复位广播
void send_reset_broadcast(void)
{
	unsigned char buff[6] = {0xaa,0xaa,0xaa,0xaa,0xaa,0xaa};
	send_app_broadcast_data(6,buff);
}

void broadcast_reset_timer_handler(void)
{
	system_reset();
}

//处理应用层广播数据
void app_broadcast_data_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
	unsigned char buff[6] = {0xaa,0xaa,0xaa,0xaa,0xaa,0xaa};
	if(memcmp(buff,data,6)==0)	//接收到复位广播
	{
		os_timer_event_set(&broadcast_reset_timer,15000,(os_timer_event_caller_t)broadcast_reset_timer_handler,NULL);//15s后复位
		return ;
	}
	//jmesh_uart_send(JMESH_UART_0, len, data);
	#ifdef Single_Lamp_Ctrl
	lamp_mode_control_handler(data[0],data[1],data[2]);
	#endif
	
	#ifdef LAMP_PASSTHROUGH
	lamp_mode_control_handler(data[0],data[1],data[2]);
	#endif
}

//应用层发送广播数据
void send_app_broadcast_data(unsigned short len,unsigned char* data)
{
	unsigned char buff[250];
	
	JMESH_BIG_ENDIAN_FILL2(expand_opcode_realcomm_app_broadcast_data,buff);	
	memcpy(buff+2,data,len);
	jmesh_access_send(0xffff,0,1,&app_expand,message_app_expand2,len+2,buff,JMESH_SEND_ALL_CHANNEL);

}

//保存网关通信的参数
void realcomm_send_save_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
	if(realcomm_send_save.dst != src)
	{
		memset((unsigned char*)&realcomm_send_save,0x00,sizeof (realcomm_send_save_t));
		realcomm_send_save.dst = src;
		realcomm_send_save.aid = 1;
		realcomm_send_save.nid = 0;
		jmesh_save_write(JMESH_REALCOMM_SEND_SAVE,sizeof (realcomm_send_save_t),(unsigned char*)&realcomm_send_save);
	}
}


//void realcomm_send_save_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data)
//{
//	if(realcomm_send_save.dst != src || realcomm_send_save.aid != aid || realcomm_send_save.nid != nid)
//	{
//		memset((unsigned char*)&realcomm_send_save,0x00,sizeof (realcomm_send_save_t));
//		realcomm_send_save.dst = src;
//		realcomm_send_save.aid = aid;
//		realcomm_send_save.nid = nid;
//		jmesh_save_write(JMESH_REALCOMM_SEND_SAVE,sizeof (realcomm_send_save_t),(unsigned char*)&realcomm_send_save);
//	}
//}

//读取网关通信的参数
void realcomm_send_read_handler(void)
{
	if(0 == jmesh_save_read(JMESH_REALCOMM_SEND_SAVE,sizeof (realcomm_send_save_t),(unsigned char*)&realcomm_send_save))
	{
		memset((unsigned char*)&realcomm_send_save,0x00,sizeof (realcomm_send_save_t));
	}	
}

void led_timer_handler(void* argv)
{	
		os_timer_event_restart(&led_timer);
		static unsigned char on_off = 1;
		on_off ^= 1;
		jmesh_gpio_set(JMESH_LED1, on_off);
}

//节点状态更新处理
void check_node_state_timer_handler(void)
{
	unsigned short j,flag;
	flag = 0;
	unsigned short addr_t;
	
	for(j=0;j<JMESH_ROUTE_TABLE_SIZE;)
	{
		if(p_route_table[j].lifetime||p_route_table[j+1].lifetime||p_route_table[j+2].lifetime)
		{
			addr_t = (j/JMESH_ROUTE_NODE_RECORD_NUM)*JMESH_ROUTE_NODE_ELEMENT_NUM+3;
			if(realcomm_send_save.dst-1 == addr_t)
			{
				flag = 1;
				break;
			}
		}
		j+=3;
	}
	if(flag == 1)
	{
		connect_state = 1;
	}
	else
	{
		connect_state = 0;
	}
	os_timer_event_restart(&check_node_state_timer);
}

//启动节点状态更新
void check_node_state_timer_start(void)
{
	os_timer_event_set(&check_node_state_timer,10000,(os_timer_event_caller_t)check_node_state_timer_handler,NULL);
}


#ifdef GET_ADDR_FROM_METER
// 读表地址超时处理  
void get_meter_addr_repeat_timer_handler(void)
{
	if(uart_baudrate_index < 4)
	{		
		jmesh_uart_baudrate_set(uart_baudrate[uart_baudrate_index],JMESH_UART_PARITY_EVEN);
		jmesh_uart_send(JMESH_UART_0, sizeof(get_addr_databuff), (uint8_t *)get_addr_databuff);
		current_realcomm_task_id = SELF_GET_METER_ADDR;
		uart_baudrate_index++;
		os_timer_event_set(&get_meter_addr_timer,1500,(os_timer_event_caller_t)get_meter_addr_repeat_timer_handler,NULL);		
	}
	else
	{
		uint8_t mac_addr[6] ={0};
		jmesh_mac_get(mac_addr);		
		memcpy(meter_addr, mac_addr, 6);		
		if(current_realcomm_task_id == SELF_GET_METER_ADDR)
		{
			current_realcomm_task_id = NO_TASK;
		}
		uart_baudrate_index  = 0;
		os_timer_event_set(&get_meter_addr_timer,60000,(os_timer_event_caller_t)get_meter_addr_repeat_timer_handler,NULL);
	}
}

// 启动读表地址任务  
void get_meter_addr_task_start(void)
{
	uart_baudrate_index = 0;
	if (current_realcomm_task_id == NO_TASK) {
		get_meter_addr_repeat_timer_handler();
	} else {	
		os_timer_event_set(&get_meter_addr_timer,1500,(os_timer_event_caller_t)get_meter_addr_repeat_timer_handler,NULL);
	}
}

// 读取表地址返回数据处理 
unsigned char get_meter_addr_callback(unsigned char* data, unsigned short len)
{
	unsigned short offset;
	unsigned char ctr_code;
	uint8_t mac_addr[6] ={0};
	if(0<(protocol_64507_pick(len,data,&offset)))
	{
		ctr_code = *(data+offset+8);
		switch(ctr_code)
		{	
			case 0x93:                                      // 返回表地址
				os_timer_event_remove(&get_meter_addr_timer);
				current_realcomm_task_id = NO_TASK;			
//				protocol_64507_get_data(len, data, 6, meter_addr);
				meter_addr[0] = *(data+offset+10) - 0x33;
				meter_addr[1] = *(data+offset+11) - 0x33;
				meter_addr[2] = *(data+offset+12) - 0x33;
				meter_addr[3] = *(data+offset+13) - 0x33;
				meter_addr[4] = *(data+offset+14) - 0x33;
				meter_addr[5] = *(data+offset+15) - 0x33;		
				jmesh_mac_get(mac_addr);
				if(0 != memcmp(meter_addr,mac_addr,sizeof(meter_addr)))
				{
					jmesh_mac_set(meter_addr);
					//jmesh_ble_reset();
					os_timer_event_set(&set_mac_callback_timer,500,(os_timer_event_caller_t)set_mac_callback_timer_handler,NULL);
				}
//				os_timer_event_set(&led_timer, 3000, (os_timer_event_caller_t)led_timer_handler, NULL);				
				return 1; 			
			default:
				break;
		}
	}
	return 0;
}

//启动读表号
void get_meter_addr_handler(void)
{
	uart_baudrate_index = 0;
	jmesh_uart_baudrate_set(uart_baudrate[uart_baudrate_index],JMESH_UART_PARITY_EVEN);
	jmesh_uart_send(JMESH_UART_0, 16, (unsigned char *)get_addr_databuff);
	get_meter_addr_flag = 1;
	get_meter_addr_repeat_num = 3;
	os_timer_event_set(&get_meter_addr_timer,2000,(os_timer_event_caller_t)get_meter_addr_repeat_timer_handler,NULL);
}

#endif


// 抄表超时处理 
void read_meter_overtime_handler(void *arg)
{
	static uint8_t delay_cnt;	
	

	if (!read_meter_delay_para[0]) // 正常抄表超时 
	{				
		delay_cnt = 0;
		if (current_realcomm_task_id == PHONE_DIRECT_READ_METER)
		{
			app_transmission_uart_overtime_handler();
		}
		current_realcomm_task_id = NO_TASK;
	}
	else  // 延迟抄表 
	{
		// 串口任务空闲, 启动抄表, 否则在有限次数内继续延迟  
		if (current_realcomm_task_id == NO_TASK)  
		{			
			mx188_frame_t frame;
			unsigned char task_id;
			delay_cnt = 0;
			
			task_id = read_meter_delay_para[1];
			if(task_id != PHONE_DIRECT_READ_METER)
			{
				if (!protocol_mx188_parse2(read_meter_data_cache, read_meter_data_cache_len, &frame))
					return;

				memcpy(&mx188_frame_record, &frame, sizeof(mx188_frame_t));	
				if((APP_BD_protocol_CheckFrame(read_meter_data_cache + frame.valid_data_offset,frame.valid_data_len) == 1)&&(task_id == GATEWAY_READ_METER))		//网关设置模块参数
				{
					current_realcomm_task_id = read_meter_delay_para[1];
					APP_BD_protocol_handler(read_meter_data_cache + frame.valid_data_offset,frame.valid_data_len,4);
					return ;	
				}				
				
				#ifdef  KEHUA_WINDOW
				window_rev_data_check(read_meter_data_cache + frame.valid_data_offset,frame.valid_data_len);
				#endif
				#if	(defined KEHUA_TRANSFER_ARM)||(defined KEHUA_ARM_BX2400_470)
				current_realcomm_task_id = read_meter_delay_para[1];;
				if(transfer_arm_rev_data_check(read_meter_data_cache + frame.valid_data_offset,frame.valid_data_len) == 1)
					return;
				#endif
				jmesh_uart_send(JMESH_UART_0, frame.valid_data_len, read_meter_data_cache + frame.valid_data_offset);
			}
			else  // 来自手机的直接抄表，不需要188协议 
			{			
				jmesh_uart_send(JMESH_UART_0, read_meter_data_cache_len, read_meter_data_cache);
			}	
			current_realcomm_task_id = read_meter_delay_para[1];
			read_meter_delay_para[0] = 0;
			os_timer_event_set(&read_meter_timer,METER_MAX_RESPONSE_MS,(os_timer_event_caller_t)read_meter_overtime_handler, &read_meter_delay_para);
		}
		else    
		{
			if (delay_cnt < 4)
			{
				delay_cnt++;
				os_timer_event_restart(&read_meter_timer);
			}
			else
			{
				delay_cnt = 0;				
				if (current_realcomm_task_id == PHONE_DIRECT_READ_METER)
				{
					app_transmission_uart_overtime_handler();
				}
				current_realcomm_task_id = NO_TASK;
			}
		}					
	}	
}

/* 
* 功能：执行 server 端请求的抄表任务
* 参数：
*	len：数据长度 
*	data：数据指针 
*	type：抄表任务请求方式，0表示网关请求抄表，1表示移动设备请求主节点抄表，2表示移动设备直接抄表 
*/
void read_meter_handler(unsigned short len,unsigned char* data, unsigned char type)
{
	unsigned char task_id;
	mx188_frame_t frame = {0};
	
	if (type == 0)
		task_id = GATEWAY_READ_METER;
	else if (type == 1)
		task_id = PHONE_GATEWAY_READ_METER;
	else
		task_id = PHONE_DIRECT_READ_METER;
	
	blueToothOr470 = FROM_BLE;	
	if (current_realcomm_task_id == NO_TASK)
	{		
		if(task_id != PHONE_DIRECT_READ_METER) // 
		{
			if (!protocol_mx188_parse2(data, len, &frame))
				return;
			
			memcpy(&mx188_frame_record, &frame, sizeof(mx188_frame_t));
			#ifdef  KEHUA_WINDOW
			window_rev_data_check(data + frame.valid_data_offset,frame.valid_data_len);
			#endif
			#if	(defined KEHUA_TRANSFER_ARM)||(defined KEHUA_ARM_BX2400_470)
			current_realcomm_task_id = task_id;
			if(transfer_arm_rev_data_check(data + frame.valid_data_offset,frame.valid_data_len) == 1)
				return;
			#endif
			
			#if(Lamp_Control_Enable==1)
			current_realcomm_task_id = task_id;
//			jmesh_uart_send(JMESH_UART_0, frame.valid_data_len, data + frame.valid_data_offset);
			if(Lamp_Control_rev_data_check(data + frame.valid_data_offset,frame.valid_data_len) == 1)
			return ;			
			#endif	
			if((APP_BD_protocol_CheckFrame(data + frame.valid_data_offset,frame.valid_data_len) == 1)&&(task_id == GATEWAY_READ_METER))		//网关设置模块参数
			{
				current_realcomm_task_id = task_id;
				APP_BD_protocol_handler(data + frame.valid_data_offset,frame.valid_data_len,4);
				current_realcomm_task_id = NO_TASK;	
				return ;	
			}
			jmesh_uart_send(JMESH_UART_0, frame.valid_data_len, data + frame.valid_data_offset);
		}
		else  // 来自手机的直接抄表，不需要188协议 
		{			
			jmesh_uart_send(JMESH_UART_0, len, data);
		}	
		current_realcomm_task_id = task_id;
		read_meter_delay_para[0] = 0;
		os_timer_event_set(&read_meter_timer,METER_MAX_RESPONSE_MS,(os_timer_event_caller_t)read_meter_overtime_handler, &read_meter_delay_para);
	
	}
	else if (current_realcomm_task_id != task_id)
	{
		// 当前串口忙碌，延时一下再处理,先把数据存起来  
		if(len > sizeof(read_meter_data_cache))
			return;		
		memcpy(read_meter_data_cache, data, len);
		read_meter_data_cache_len = len;
		read_meter_delay_para[0] = 1;
		read_meter_delay_para[1] = task_id;
		os_timer_event_set(&read_meter_timer, 300, (os_timer_event_caller_t)read_meter_overtime_handler, &read_meter_delay_para);			
	}
	connect_state = 1;				//add by ljj 20190410
	os_timer_event_restart(&app_network_maintain_timer);
}




// 主动上报数据应答处理  
void report_meter_return_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
	if(report_state != 0)
	{		
		#ifdef  KEHUA_GAS 
		if (report_content == 1)
			gas_con_report_flag = false;
		#endif
	}	
	report_state = 0; // 上报成功，清除所有状态位 
	os_timer_event_remove(&report_meter_timer);
	report_meter_repeat_num = 0;
}

// 主动上报数据超时处理 
void report_meter_overtime_handler(void)
{
	if((report_meter_repeat_num > 0) && (report_state != 0))
	{
		report_meter_repeat_num--;
		jmesh_access_send(realcomm_send_save.dst,realcomm_send_save.nid,realcomm_send_save.aid,&app_expand,message_app_expand2,report_meter_data_len,report_meter_data,JMESH_SEND_ALL_CHANNEL);
		os_timer_event_restart(&report_meter_timer);
	}
	else 
	{
		os_timer_event_remove(&report_meter_timer);
		report_meter_repeat_num = 0;
		report_state &= ~0x01; // 状态位清零  
	}
}
// 抄表数据返回处理  
void message_realcomm_server_data_return_handler(unsigned short len,unsigned char* data)
{
	unsigned char buff[255];
	unsigned char task_id; 
	mx188_frame_t frame = {0};
	
	os_timer_event_remove(&read_meter_timer);
	task_id = current_realcomm_task_id;
	current_realcomm_task_id = NO_TASK;	
	
	// 188协议打包 
	if((task_id == GATEWAY_READ_METER) || (task_id == PHONE_GATEWAY_READ_METER))
	{
		memcpy(&frame, &mx188_frame_record, sizeof(mx188_frame_t));	
		frame.CTR_10.direction = FROME_SLAVE_NODE;				
		len = protocol_mx188_pack(&frame, len, data, buff+2, sizeof(buff)-2);
	}
	
	//JMESH_BIG_ENDIAN_FILL2(realcomm_send_save.type,buff);
	if(task_id == GATEWAY_READ_METER)    //网关抄表
	{
		if(blueToothOr470 == FROM_BLE)
		{
			JMESH_BIG_ENDIAN_FILL2(expand_opcode_realcomm_data_return,buff);
			jmesh_access_send(realcomm_send_save.dst,realcomm_send_save.nid,realcomm_send_save.aid,&app_expand,message_app_expand2,len+2,buff,JMESH_SEND_ALL_CHANNEL);
		}
		else
		{
			phy_tx_pkt(buff + 2, len, 1);
		}
	}
	else if(task_id == PHONE_GATEWAY_READ_METER)	//APP通过主节点抄表
	{
		JMESH_BIG_ENDIAN_FILL2(expand_opcode_realcomm_app_read_meter_return,buff);
		jmesh_access_send(realcomm_send_save.dst,realcomm_send_save.nid,realcomm_send_save.aid,&app_expand,message_app_expand2,len+2,buff,JMESH_SEND_ALL_CHANNEL);
	}
	else  // app直接抄表,返回的数据不用188协议打包  
	{
		app_transmission_uart_callback_handler(len, data);
	}
	
}



//20190601
//APP通过网关抄表超时处理
void app_read_meter_overtime_handler(void)
{
	app_read_meter_flag = 0;
}
//APP通过网关抄表
void app_read_meter_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
	#if(Lamp_Control_Enable==1)
	unsigned short offset;
	unsigned char ctr_code,buff[20],buff_1[50];
	unsigned char DI[4] = {0xDD,0xDD,0xDD,0xDD};
	if(0<protocol_64507_pick(len,data,&offset))
	{
		ctr_code = *(data+offset+8);
		switch(ctr_code)
		{	
			case 0x34:                                     
				if(memcmp(data+offset+10,DI,4) == 0)
				{	
					if(*(data+offset+14) == 0x33)
					{	
						
						lamp_control_off();
					}	
					else
					{	
						lamp_control_on();
					}
					
					connect_state = 1;
					realcomm_send_save_handler(src,dst,len,data);
					
					memcpy(buff,data+offset,8);
					buff[8] = 0xB4;
					buff[9] = 0x00;
					buff[10] = check_sum(10,buff);
					buff[11] = 0x16;
					
					JMESH_BIG_ENDIAN_FILL2(expand_opcode_realcomm_app_read_meter_return,buff_1);
					memcpy(buff_1+2,buff,12);
					jmesh_access_send(realcomm_send_save.dst,realcomm_send_save.nid,realcomm_send_save.aid,&app_expand,message_app_expand2,12+2,buff_1,JMESH_SEND_ALL_CHANNEL);
					
					return ;
				}	
				break; 			
			default:
				break;
		}
	
	}
	#endif
	
	if(read_meter_flag != 1&&app_local_read_meter_flag != 1)
	{
		jmesh_uart_send(JMESH_UART_0, len, data);	
		app_read_meter_flag = 1;
		os_timer_event_set(&app_read_meter_timer,3000,(os_timer_event_caller_t)app_read_meter_overtime_handler,NULL);
	}
	connect_state = 1;
	realcomm_send_save_handler(src,dst,len,data);
	os_timer_event_restart(&app_network_maintain_timer);
}

//通过网关设置参数
void app_set_meter_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
	unsigned char state,buff[250];
	
	if((len + 2) > sizeof(buff))
		return;
	
	state = APP_BD_protocol_handler(data,len,3);
	connect_state = 1;
	realcomm_send_save_handler(src,dst,len,data);
	if(state == 2)	//数据帧不是APP通信协议
	{	
		JMESH_BIG_ENDIAN_FILL2(expand_opcode_realcomm_app_set_meter_return,buff);
		memcpy(buff+2,data,len);
    jmesh_access_send(realcomm_send_save.dst,realcomm_send_save.nid,realcomm_send_save.aid,&app_expand,message_app_expand2,len+2,buff,JMESH_SEND_ALL_CHANNEL);
	}
	os_timer_event_restart(&app_network_maintain_timer);
}






// 主动上报数据处理  
// report_content 上报内容，0为未定义，1为燃气浓度 
unsigned char report_meter_handler(unsigned short len,unsigned char* data, uint8_t content)
{	
	if (report_state != 0)
		return 1;
	if (len == 0 || data == NULL)
		return 2;
	
	report_meter_repeat_num = 1;
	memset(report_meter_data,0x00,200);
	
	{
		mx188_frame_t frame = {0};
				
		frame.meter_type = 0x80; 
		memcpy(frame.addr.u8, meter_addr, METER_ADDR_SIZE);		
		frame.CTR_20.repeat_index = 0;
		frame.CTR_20.repeat_level = 0;
		frame.CTR_10.function = 0x01;
		frame.CTR_10.respond_flag = 0;
		frame.CTR_10.direction = FROM_HOST_NODE;			
		frame.seq = get_new_mx188_seq();	
		frame.DI0 = 0x90;
		frame.DI1 = 0x25;
		
		len = protocol_mx188_pack(&frame, len, data, report_meter_data+2, sizeof(report_meter_data)-2);
	}
	
	JMESH_BIG_ENDIAN_FILL2(expand_opcode_realcomm_meter_reprot,report_meter_data);
	report_meter_data_len = len+2;
	report_content = content;	
	
	// 蓝牙上报 
	if(0 == jmesh_access_send(realcomm_send_save.dst,realcomm_send_save.nid,realcomm_send_save.aid,&app_expand,message_app_expand2,report_meter_data_len,report_meter_data,JMESH_SEND_ALL_CHANNEL))
	{
		report_state |= 0x01;
		os_timer_event_set(&report_meter_timer, 4000, (os_timer_event_caller_t)report_meter_overtime_handler, NULL);
	}
	else
	{
		;
	}

	// 470上报
	if (wirless_tech.tech == TECH_BLE_470)	
	{		
		report_by_470();
	}
	
	
	return 0;
}


void app_network_maintain_timer_handler(void)
{
	unsigned char reset_cnt[2] = {0};
	if(jmesh_save_read(JMESH_SAVE_RESET_COUNT, 2, reset_cnt))
		reset_cnt[1]++;
	jmesh_save_write(JMESH_SAVE_RESET_COUNT, 2, reset_cnt);
	system_reset();
	//jmesh_ble_reset();
}


void app_network_maintain_start(void)
{
	os_timer_event_set(&app_network_maintain_timer, maintian_period, (os_timer_event_caller_t)app_network_maintain_timer_handler, NULL);
}

//网络维护，处理主节点的点名
void app_network_maintain_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
	unsigned char buff[50];
	if(memcmp(JZQ_communication_address,data,6) != 0)
	{
		memcpy(JZQ_communication_address,data,6);
		jmesh_save_write(JMESH_JZQ_ADDR_SAVE,6,JZQ_communication_address);
	}
	if (wirless_tech.tech == TECH_BLE_470)
	{
		uint8_t channel_group = (JZQ_communication_address[0] / 0x10) * 10 + (JZQ_communication_address[0] % 0x10);
		channel_group = channel_group % CHANNEL_GROUP_NUM + 1;
		phy_set_channel_group(channel_group);                               		
		phy_set_channel(PRIVATE_CHANNEL, phy_pib.phyCurrentChannelIndex); 
	}
	connect_state = 1;
	realcomm_send_save_handler(src,dst,len,data);
	JMESH_BIG_ENDIAN_FILL2(expand_opcode_realcomm_app_network_maintain_return,buff);
	memcpy(buff+2,meter_addr,6);
	jmesh_access_send(realcomm_send_save.dst,realcomm_send_save.nid,realcomm_send_save.aid,&app_expand,message_app_expand2,2+6,buff,JMESH_SEND_GATT_CHANNEL);
	os_timer_event_restart(&app_network_maintain_timer);
}

// 子节点入网流程，主节点查询子节点地址 
void realcomm_get_meter_addr_handler (unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
	unsigned char buff[255];
	if(memcmp(JZQ_communication_address,data,6) != 0)
	{
		memcpy(JZQ_communication_address,data,6);
		jmesh_save_write(JMESH_JZQ_ADDR_SAVE,6,JZQ_communication_address);
	}
	connect_state = 1;
	realcomm_send_save_handler(src,dst,len,data);
	
	if (wirless_tech.tech == TECH_BLE_470)
	{
		uint8_t channel_group = (JZQ_communication_address[0] / 0x10) * 10 + (JZQ_communication_address[0] % 0x10);
		channel_group = channel_group % CHANNEL_GROUP_NUM + 1;
		phy_set_channel_group(channel_group);                               		
		phy_set_channel(PRIVATE_CHANNEL, phy_pib.phyCurrentChannelIndex); 
	}
	
	JMESH_BIG_ENDIAN_FILL2(expand_opcode_realcomm_get_meter_addr_return,buff);
	memcpy(buff+2,meter_addr,6);
	jmesh_access_send(realcomm_send_save.dst,realcomm_send_save.nid,realcomm_send_save.aid,&app_expand,message_app_expand2,6+2,buff,JMESH_SEND_GATT_CHANNEL);
}

