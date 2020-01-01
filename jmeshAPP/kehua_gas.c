/*
科华燃气表
*/
#include "kehua_gas.h"
#include "os_timer_event.h"
#include "jmesh_uart.h"
#include "jmesh_app_pro.h" 
#include "modbus_protocol.h"
#include "protocol_645.h"
#include "beidian_app_cfg.h"
#include "protocol_mx188.h"

#ifdef  KEHUA_GAS 

#define DEFAULT_GET_GAS_CON_INTERVAL     (5) // sec
#define DEFAULT_REPORT_GAS_CON_INTERVAL  (5) // min

static const uint8_t get_gas_con_cmd[] = {0xFD, 0x03, 0x00, 0x23, 0x00,  0x0E, 0x21, 0xF8}; // 抄读燃气浓度命令 
static os_timer_event_t get_gas_con_timer;	                // 周期抄读燃气浓度定时器
static os_timer_event_t get_data_overtime_timer;         // 抄读燃气浓度超时定时器 
static os_timer_event_t report_gas_con_timer;               // 周期上报燃气浓度定时器
static task_flag_t task_flag = {0};
volatile bool gas_con_report_flag = true;
uint8_t report_interval_m; 
uint8_t collect_interval_s;

uint8_t continue_report_num;

// 读命令执行超时处理
static void get_data_overtime_timer_handler(void* arg)
{		
	current_realcomm_task_id = NO_TASK;
}

// 周期读取气体浓度定时器 
static void get_gas_con_timer_handler(void* arg)
{	
	task_flag.bit.read_con = 1;
	os_timer_event_restart(&get_gas_con_timer);			
}

// 周期上报定时器 
static void report_gas_con_timer_handler(void* arg)
{	
	gas_con_report_flag = true;
	os_timer_event_restart(&report_gas_con_timer);
}

// 抄读燃气报警器返回处理 
void get_gas_data_return_handler(uint8_t *data, uint8_t len)
{// FD 03 06 00 01 00 00 00 00 4D B1 // 探测器状态，电源状态，气体浓度 
	sensor_t *pSensor;
	uint16_t sensor_16;
	
	if (!is_modbus_frame(data,len))
		return;
	
	if(data[1] == 3)  // 功能码 3 
	{			
		switch(current_realcomm_task_id)
		{
			case SELF_READ_GAS_CON:
				os_timer_event_remove(&get_data_overtime_timer);
				current_realcomm_task_id = NO_TASK;
				sensor_16 = (((uint16_t)data[3] << 8) | data[4]);
				pSensor = (sensor_t *)&sensor_16;
				if ((pSensor->sensor1_state == SENSOR_WARNING) || (pSensor->sensor2_state == SENSOR_WARNING) || (pSensor->sensor3_state == SENSOR_WARNING) 
					||(pSensor->sensor4_state == SENSOR_WARNING) || gas_con_report_flag || continue_report_num)  // 探测器故障 || 报警 || 周期上报 ||浓度小于20%之后上报次数
				{
					uint8_t report_buff[53] = {0};
					protocol_64507_di_t di = {0x56, 0x77, 0xcc, 0xbd};
					unsigned char protocol_64507_data[37] = {0};										
						
					memcpy(protocol_64507_data, &data[3], data[2]);	
					protocol_64507_pack(meter_addr, di, 0x91, sizeof(protocol_64507_data), protocol_64507_data, sizeof(report_buff) , report_buff);								  
					report_meter_handler(sizeof(report_buff), report_buff, 1);	
					
					if ((pSensor->sensor1_state == SENSOR_WARNING) || (pSensor->sensor2_state == SENSOR_WARNING) || (pSensor->sensor3_state == SENSOR_WARNING) 
						|| (pSensor->sensor4_state == SENSOR_WARNING)) {						
						continue_report_num = 2;
					} else {
						if(continue_report_num>0) {
							continue_report_num--;
						}
					}					
				}
				break;
			default :break;
		}
	}	
}

// 启动周期抄读、上报燃气浓度任务  
void kehua_gas_start()
{	
	os_timer_event_set(&report_gas_con_timer, report_interval_m * 60 * 1000, (os_timer_event_caller_t)report_gas_con_timer_handler,NULL);
	os_timer_event_set(&get_gas_con_timer, collect_interval_s * 1000, (os_timer_event_caller_t)get_gas_con_timer_handler,NULL);
	continue_report_num = 0;

}

void kehua_gas_task()
{			
	if (task_flag.all)
	{						
		if (current_realcomm_task_id != NO_TASK)
			return;
		if (report_state != 0)
			return;
		
		if(task_flag.bit.read_con)
		{			
			task_flag.bit.read_con = 0;
			current_realcomm_task_id = SELF_READ_GAS_CON;
			jmesh_uart_send(JMESH_UART_0,  sizeof(get_gas_con_cmd), (uint8_t *)get_gas_con_cmd);
		}
		
		os_timer_event_set(&get_data_overtime_timer, METER_MAX_RESPONSE_MS, (os_timer_event_caller_t)get_data_overtime_timer_handler,NULL);				
	}
}

#endif






















