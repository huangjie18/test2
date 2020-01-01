#ifndef JMESH_APP_PRO_H
#define JMESH_APP_PRO_H

#include "mx_datatype.h"
#include"../../onchip_system/os_timer_event.h"
#include"jmesh_route.h"
#include"jmesh_print.h"
#include "protocol_mx188.h"

#define BX_DBG_ENABLE

#define maintian_period (60*12*1000)
#define METER_MAX_RESPONSE_MS 2400  // 表端最大响应时间，单位ms

#define APP_CMD_PRINTABLE 1

#if(APP_CMD_PRINTABLE==1)
    #define app_print_cmd(fmt, ...) _print_cmd(fmt, ##__VA_ARGS__)
    #define app_print_buffer_cmd(len,buffer,fmt,...) _print_buffer_cmd(len,(char*)buffer,fmt, ##__VA_ARGS__)
#else
    #define app_print_cmd(fmt, ...)
    #define app_print_buffer_cmd(len,buffer,fmt,...)
#endif


typedef enum 
{
	NO_TASK,
	GATEWAY_READ_METER,
	PHONE_GATEWAY_READ_METER,
	PHONE_DIRECT_READ_METER,
	SELF_READ_GAS_CON,
	SELF_READ_SENSOR_STATE,
	SELF_GET_METER_ADDR,
	SELF_READ_WINDOW_STATE,
}realcomm_task_id_t;



typedef struct {
		unsigned short dst;
		unsigned short nid;
		unsigned short aid;
		unsigned char type;	
		unsigned char  len;
		unsigned char* data;
}realcomm_send_save_t;

extern realcomm_send_save_t  realcomm_send_save;
extern unsigned char get_meter_addr_flag;
extern os_timer_event_t led_timer;
extern unsigned char read_meter_flag;
extern unsigned char app_read_meter_flag;
extern unsigned char app_local_read_meter_flag;
extern os_timer_event_t read_meter_timer;
extern os_timer_event_t report_meter_timer;
extern UINT8 JZQ_communication_address[6];
extern unsigned char meter_addr[6];
extern unsigned char connect_state;
extern uint8_t current_realcomm_task_id;
extern unsigned char read_meter_delay_para[2]; // 第一个参数非0表示延迟抄表，第二个参数为任务id
extern unsigned char read_meter_data_cache[256];
extern unsigned char read_meter_data_cache_len;
extern mx188_frame_t mx188_frame_record;
extern unsigned char blueToothOr470;
extern volatile unsigned char report_state;
extern unsigned char report_content;
extern unsigned char report_meter_data[200];
extern unsigned char report_meter_data_len;

void delay_ms(uint16_t ms);
void system_reset(void);
unsigned char get_meter_addr_callback(unsigned char* data, unsigned short len);
void message_realcomm_server_data_return_handler(unsigned short len,unsigned char* data);
void realcomm_get_meter_addr_handler (unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void led_timer_handler(void* argv);
void realcomm_send_save_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void ApsInit(void);
void realcomm_send_read_handler(void);
void get_meter_addr_repeat_timer_handler(void);
void get_meter_addr_handler(void);
void get_meter_addr_task_start(void);
void read_meter_overtime_handler(void * arg);
void read_meter_handler(unsigned short len,unsigned char* data, unsigned char type);
unsigned char report_meter_handler(unsigned short len,unsigned char* data, uint8_t content);
void report_meter_return_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void app_read_meter_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void app_gateway_read_meter_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void app_set_meter_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void app_network_maintain_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void app_network_maintain_start(void);
void check_node_state_timer_start(void);

void app_broadcast_data_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void send_app_broadcast_data(unsigned short len,unsigned char* data);
void realcomm_get_meter_addr_handler (unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void report_meter_overtime_handler(void);

void send_ota_adv(void);

#endif
