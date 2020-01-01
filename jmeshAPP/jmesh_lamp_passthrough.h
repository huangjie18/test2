#ifndef _JMESH_LAMP_PASSTHROUGH_
#define _JMESH_LAMP_PASSTHROUGH_

#include "APP_protocol.h"
#include "jmesh_app_pro.h"
#include "protocol_645.h"
#include "log.h"


void lamp_mode_init(void);
void lamp_mode_info_save(void);
void Lamp_factory_reset(void);
void lamp_mode_info_read(void);

void read_meter_when_control(void);
unsigned char lamp_return_handler(unsigned char* data, unsigned short len);
void lamp_read_meter_timer_handler(void);
void lamp_read_meter_timer_start(void);
uint8_t hex2BCD(uint8_t hex);
void lamp_mode_control_handler(unsigned char mode,unsigned char value,unsigned char device_type);

void lamp_mode_control_handler(unsigned char mode,unsigned char value,unsigned char device_type);
void APP_BD_CA01_handler(uint8 *data,uint16 len,uint8 rev_type);



#endif
