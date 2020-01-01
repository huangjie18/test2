#ifndef _SCENE_PANEL_
#define _SCENE_PANEL_

#include "jmesh_button.h"
#include "io_ctrl.h"
#include "app_gpio.h"
#include "plf.h"
#include "log.h"
#include "jmesh_gpio.h"
#include "jmesh_uart.h"
#include "os_timer_event.h"
#include "app_protocol.h"

extern unsigned char scene_mode;

void APP_BD_CF00_handler(uint8 *data,uint16 len,uint8 rev_type);
void APP_BD_CF02_handler(uint8 *data,uint16 len,uint8 rev_type);
void APP_BD_CF03_handler(uint8 *data,uint16 len,uint8 rev_type);
void Panel_Memeory_Init(void);
void Panel_factory_reset(void);
void button_led_init_start(void);

void scene_panel_button_handle(uint8_t button_state,uint8_t times);


#endif

