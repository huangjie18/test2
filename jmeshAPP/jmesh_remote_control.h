#ifndef _REMOTE_CONTROL_
#define _REMOTE_CONTROL_

#include "io_ctrl.h"
#include "app_gpio.h"
#include "plf.h"
#include "log.h"
#include "jmesh_gpio.h"
#include "jmesh_uart.h"
#include "os_timer_event.h"
#include "app_protocol.h"
#include "jmesh_button.h"

extern unsigned char ButtonInfo[20];
extern unsigned char remote_mode;

void Remote_Memeory_Init(void);
void Remote_factory_reset(void);

void APP_BD_D300_handler(uint8 *data,uint16 len,uint8 rev_type);
void APP_BD_D302_handler(uint8 *data,uint16 len,uint8 rev_type);
void APP_BD_D303_handler(uint8 *data,uint16 len,uint8 rev_type);
void jmesh_remote_button_handle(uint8_t button_state,uint8_t times);

void jmesh_remote_control_BD_protocol_pakage_send(unsigned char num,unsigned char mode);
unsigned char three_remote_control_handler(unsigned char* data, unsigned short len);
unsigned char four_remote_control_handler(unsigned char* data, unsigned short len);
void button_state_set_data_pakage_send(unsigned char Button_Num,unsigned char SW_State);

void button_state_buff_send_timer_statr(void);
void button_flag_clear_timer_statr(void);

#endif
