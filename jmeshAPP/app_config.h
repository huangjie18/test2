#ifndef APP_CONFIG_H
#define APP_CONFIG_H
#include <stdint.h>

#define FROM_BLE 0
#define FROM_470 1

// ����ͨ�ż����궨��
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
	uint8_t synchronous: 4; // 1-470������ͬ��ִ�У�0-�첽ִ��
}wirless_tech_t;
 

extern wirless_tech_t wirless_tech;

#include "os_timer_event.h"
#include "jmesh_app_pro.h"
void OTA_Switch(void);
extern unsigned char ota_req_permit_state;

#endif
