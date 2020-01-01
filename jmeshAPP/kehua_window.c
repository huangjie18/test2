#include "kehua_window.h"
#include "os_timer_event.h"
#include "jmesh_uart.h"
#include "jmesh_app_pro.h" 
#include "modbus_protocol.h"
#include "protocol_645.h"
#include "beidian_app_cfg.h"
#include "protocol_mx188.h"




#ifdef KEHUA_WINDOW

static const uint8_t get_window_state_cmd[] = {0xFF, 0x03, 0x00, 0x03, 0x00,  0x02, 0x21, 0xD5}; // 查询窗户状态命令 
static os_timer_event_t get_window_state_timer;	                // 查询窗户状态定时器
static os_timer_event_t get_window_state_overtime_timer;         // 查询窗户状态超时定时器 
static os_timer_event_t report_window_state_timer;	                // 定时上报窗户状态定时器
static window_task_flag_t window_task_flag = {0};
unsigned char window_report_period;								//上报周期
unsigned char window_report_count;								//上报次数
unsigned char window_report_current_count;						//目前上报次数

// 查询窗户状态超时处理
static void get_window_state_overtime_timer_handler(void* arg)
{		
	current_realcomm_task_id = NO_TASK;
	window_report_current_count++;
	if(window_report_current_count < window_report_count)
	{
		kehua_window_start(window_report_period);
	}
		
}

// 查询窗户状态定时器 
static void get_window_state_timer_handler(void* arg)
{	
	window_task_flag.bit.read_con = 1;		
}


// 查询窗户状态返回处理 
void get_window_state_return_handler(uint8_t *data, uint8_t len)
{
	if (!is_modbus_frame(data,len))
		return;
	
	if(data[1] == 3)  // 功能码 3 
	{			
		switch(current_realcomm_task_id)
		{
			case SELF_READ_WINDOW_STATE:
				os_timer_event_remove(&get_window_state_overtime_timer);
				current_realcomm_task_id = NO_TASK;
				report_meter_handler(len, data, 0);	
				window_report_current_count++;			
				if(window_report_current_count < window_report_count)
				{
					kehua_window_start(window_report_period);
				}
				else
				{
					os_timer_event_restart(&report_window_state_timer);
				}
				break;
			default :break;
		}
	}	
}

// 启动查询窗户状态任务  
void kehua_window_start(unsigned char time_s)
{	
	os_timer_event_set(&get_window_state_timer, time_s*1000, (os_timer_event_caller_t)get_window_state_timer_handler,NULL);
}

void kehua_window_task(void)
{			
	if (window_task_flag.all)
	{						
		if (current_realcomm_task_id != NO_TASK)
			return;
		if (report_state != 0)
			return;
		
		if(window_task_flag.bit.read_con)
		{			
			window_task_flag.bit.read_con = 0;
			current_realcomm_task_id = SELF_READ_WINDOW_STATE;
			jmesh_uart_send(JMESH_UART_0,  sizeof(get_window_state_cmd), (uint8_t *)get_window_state_cmd);
		}
		
		os_timer_event_set(&get_window_state_overtime_timer, METER_MAX_RESPONSE_MS, (os_timer_event_caller_t)get_window_state_overtime_timer_handler,NULL);				
	}
}




void window_rev_data_check(unsigned char *data,unsigned short len)
{
	if (!is_modbus_frame(data,len))
		return;
	if(data[1] == 0x06) //写操作
	{
		if(data[3] == 0x03) //平移窗开启/关闭/停止寄存器
		{
			if(data[5] == 0x01 || data[5] == 0x05) //开启、关闭
			{
				window_report_current_count = 0;
				kehua_window_start(window_report_period);
				
			}
			else if(data[5] == 0x02) //停止
			{
				window_report_current_count = window_report_count;
				kehua_window_start(2);
			}	
		}
		else if(data[3] == 0x04) //平移窗位置寄存器
		{
			window_report_current_count = 0;
			kehua_window_start(window_report_period);
		
		}
		os_timer_event_restart(&report_window_state_timer);
	}

}



static void report_window_state_timer_handler(void* arg)
{	
	window_report_current_count = window_report_count;
	get_window_state_timer_handler(NULL);	
	os_timer_event_restart(&report_window_state_timer);	
}


// 启动定时上报窗户状态任务  
void report_kehua_window_start(void)
{	
	os_timer_event_set(&report_window_state_timer, 5*60*1000, (os_timer_event_caller_t)report_window_state_timer_handler,NULL);
}




















#endif
