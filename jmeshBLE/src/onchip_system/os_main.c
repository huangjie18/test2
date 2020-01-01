#ifndef x86
#include"freertos.h"
#include"task.h"
#include"osapp_config.h"
#endif
#include"os_core.h"
#include"os_time.h"
#include"os_timer_event.h"
#include"os_callback.h"
#include"../jmesh/jmesh_task.h"
#include"../driver/jmesh_system.h"
#include"../jmesh/jmesh_print.h"
#include"../bearer/routing/jmesh_route.h"
#include"../app/expand/app_expand2/water/water_pulse.h"
#include"FreeRTOS.h"
#include"portmacro.h"
#include "task.h"
#include"jmesh_app_pro.h"
#include"_470_app.h"
#include "_470_phy.h"
#include "kehua_gas.h"
#include "kehua_window.h"
#include "app_config.h"
#include "jmesh_button.h"
#include "jmesh_remote_control.h"


os_timer_event_t test_timer;
unsigned portBASE_TYPE uxHighWaterMark;
TaskHandle_t handler_jmesh_task;
void test_timer_handler(void* argv)
{
	
		os_timer_event_restart(&test_timer);
		static unsigned char on_off = 1;
		on_off ^= 1;

	if (wirless_tech.tech == TECH_BLE_470)
		phy_tx_test();
}

void os_main(void* argv)
{
    //jmesh_system_timer_start(1000/OS_TICKS_FREQUENCY,os_time_interrupt);
    os_init();
    os_task_start(&os_timer_event, NULL);
    os_task_start(&jmesh_task, NULL);
    os_task_start(&os_uart_int_e, NULL);
    os_task_start(&os_pulse_int_e, NULL);
    os_start_callback();
    os_inited_callback();

    while(1){
        os_do_poll();
        os_do_event();
        os_cycle_callback();
				}
}
#ifndef x86
#define JMESH_TASK_STACK_DEPTH 1200
TaskHandle_t handler_jmesh_task;
StackType_t stack_jmesh_task[JMESH_TASK_STACK_DEPTH];
StaticTask_t env_jmesh_task;

void mesh_task_create(void* param)
{
    handler_jmesh_task = xTaskCreateStatic(os_main,"JMESH_TASK",JMESH_TASK_STACK_DEPTH,
		    param,OS_PRIORITY_APP_TASK,stack_jmesh_task,&env_jmesh_task);
}

#endif
