#ifndef KEHUA_WINDOW_H
#define KEHUA_WINDOW_H


#include <stdint.h>
#include <stdbool.h>

typedef union 
{
	struct 
	{
		uint8_t read_con:1;	
		uint8_t reserve:7;
	}bit;
	uint8_t all;	
}window_task_flag_t;


extern unsigned char window_report_period;								//上报周期
extern unsigned char window_report_count;								//上报次数


void get_window_state_return_handler(uint8_t *data, uint8_t len);
void kehua_window_start(unsigned char time_s);
void kehua_window_task(void);
void window_rev_data_check(unsigned char *data,unsigned short len);
void report_kehua_window_start(void);
#endif


