#ifndef _JMESH_BUTTOM_
#define _JMESH_BUTTOM_

#include "io_ctrl.h"
#include "app_gpio.h"
#include "plf.h"
#include "log.h"
#include "jmesh_gpio.h"
#include "jmesh_uart.h"
#include "os_timer_event.h"
#include "app_protocol.h"
#include "jmesh_remote_control.h"
#include "jmesh_scene_panel.h"

extern unsigned char trg,cont,ctr_type;

typedef struct stu
{
	unsigned char pin;
	unsigned char led;
}Button_Config;

extern Button_Config button[4];

void button_init(void);
void button_scan_handle(void);
void jmesh_button_scan_task_start(void);
void button_led_state(unsigned char led_state);
void led_on(unsigned char led);
void led_off(unsigned char led);



#endif

