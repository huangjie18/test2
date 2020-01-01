#ifndef APP_CONFIG_H
#define APP_CONFIG_H
#include <stdint.h>

#define FROM_BLE 0
#define FROM_470 1

// 无线通信技术宏定义
enum
{
	TECH_BLE_470,  
	TECH_BLE,     
//	TECH_470,
	
	TECH_NUM,
};


typedef struct _wirless_tech
{
	uint8_t tech: 4;
	uint8_t synchronous: 4; // 1-470与蓝牙同步执行，0-异步执行
}wirless_tech_t;
 

extern wirless_tech_t wirless_tech;

#include "os_timer_event.h"
#include "jmesh_app_pro.h"
void OTA_Switch(void);
extern unsigned char ota_req_permit_state;

#endif
