#ifndef JMESH_LAMP_H
#define JMESH_LAMP_H

#include "APP_protocol.h"
#include "jmesh_app_pro.h"


extern unsigned char lamp_mode[16];

unsigned char lamp_return_handler(unsigned char* data, unsigned short len);
void lamp_control_init(void);
void lamp_control_on(void);
void lamp_control_off(void);
void lamp_mode_init(void);
void lamp_mode_info_save(void);
void lamp_mode_info_read(void);
void lamp_mode_control_handler(unsigned char mode,unsigned char value,unsigned char device_type);
unsigned char Lamp_Control_rev_data_check(unsigned char *data,unsigned short len);
unsigned char APP_BD_B101_Lamp_handler(unsigned char *data,unsigned short len,unsigned char rev_type);
unsigned char Lamp_Control_rev_data_by_node(unsigned char *data,unsigned short len);
#endif

