#ifndef KEHUA_GAS_H
#define KEHUA_GAS_H

#include <stdint.h>
#include <stdbool.h>


typedef union task_flag
{
	struct 
	{
		uint8_t read_con:1;	
		uint8_t reserve:7;
	}bit;
	uint8_t all;	
}task_flag_t;

typedef struct _sensor_state
{
	uint16_t sensor1_state: 4;
	uint16_t sensor2_state: 4;
	uint16_t sensor3_state: 4;
	uint16_t sensor4_state: 4;
}sensor_t;

typedef enum
{
	SENSOR_PREHEAT = 0,
	SENSOR_NORMAL = 1,
	SENSOR_MALFUNTION = 2,
	SENSOR_OVERFLOW = 3,
	SENSOR_WARNING = 4,
}sensor_state_t;

extern volatile bool gas_con_report_flag;
extern uint8_t report_interval_m; 
extern uint8_t collect_interval_s;

// 启动周期抄读燃气浓度任务  
void kehua_gas_start(void);

// 抄读报警器数据返回处理 
void get_gas_data_return_handler(uint8_t *data, uint8_t len);
void kehua_gas_task(void);

#endif
