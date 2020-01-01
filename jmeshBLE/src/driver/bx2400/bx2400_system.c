#include"../jmesh_driver_config.h"
#if (JMESH_DRIVER_BX2400)
#include"osapp_config.h"
#include"../jmesh_system.h"
#include"stdio.h"
#include "log.h"
#include"semphr.h"
#include"queue.h"
#include"app_wdt.h"

static SemaphoreHandle_t sleep_xSemaphore=NULL;

#ifdef JMESH_WDT_ENABLE

static int wdt_sub_count=0;
static int wdt_sub_flag;
static int wdt_reset_value=0;
static app_wdt_inst_t wdt_inst = WDT_INSTANCE(0);

static void jmesh_wdt_int_callback(void *param)
{
    LOG(LOG_LVL_INFO,"wdt timeout\n");
}
void jmesh_wdt_init(void)
{
    wdt_sub_flag=0;

    wdt_inst.param.mode = wdt_Direct_Reset;
    wdt_inst.param.plck = wdt_2_pclk_cycles;
    wdt_inst.param.timeout_period = JMESH_WDT_PERIOD_MS;
    wdt_inst.param.callback=jmesh_wdt_int_callback;
    app_wdt_init(&wdt_inst.inst);
    app_wdt_start(&wdt_inst.inst);
}
void jmesh_wdt_reset(void)
{
    app_wdt_feed_dog(&wdt_inst.inst);
}

int jmesh_wdt_sub_regist(void)
{
    if(wdt_sub_count<16){
        wdt_reset_value<<=1;
        wdt_reset_value|=1;
        return wdt_sub_count++;
    }
    else
        return -1;
}
void jmesh_wdt_sub_reset(int id)
{
    wdt_sub_flag|=(1<<id);
    if(wdt_reset_value == wdt_sub_flag){
        wdt_sub_flag=0;
          //LOG(LOG_LVL_INFO,"wdt reset\n");
        app_wdt_feed_dog(&wdt_inst.inst);

    }
}
#endif

void* jmesh_mutex_new(void)
{
    return xSemaphoreCreateMutex();
}
void jmesh_mutex_free(void* mutex)
{
    if(NULL==mutex){
        return;
    }
    return vSemaphoreDelete((SemaphoreHandle_t)mutex);
}
void jmesh_mutex_lock(void* mutex)
{
    if(NULL==mutex){
        return;
    }
    xSemaphoreTake((SemaphoreHandle_t)mutex,portMAX_DELAY);
}
void jmesh_mutex_unlock(void* mutex)
{
    if(NULL==mutex){
        return;
    }
    xSemaphoreGive((SemaphoreHandle_t)mutex);
}

void jmesh_mutex_interrupt_lock(void* mutex)
{
    if(NULL==mutex){
        return;
    }
    xSemaphoreTake((SemaphoreHandle_t)mutex,portMAX_DELAY);
}
void jmesh_mutex_interrupt_unlock(void* mutex)
{
    BaseType_t xHigherPriorityTaskWoken=pdFALSE;
    if(NULL==mutex){
        return;
    }
    xSemaphoreGiveFromISR((SemaphoreHandle_t)mutex,&xHigherPriorityTaskWoken);
	  if(xHigherPriorityTaskWoken==pdTRUE){
		    portYIELD_FROM_ISR(pdTRUE);
	  }
}
void jmesh_system_timer_start(unsigned short interval_ms,void(*handler)(void))
{
    void *jmesh_system_timer=NULL;

    jmesh_system_timer=xTimerCreate("Jmesh System Timer",interval_ms/portTICK_RATE_MS,pdTRUE,(void *)0,(TimerCallbackFunction_t)handler);

    if(NULL==jmesh_system_timer)
    {
        LOG(LOG_LVL_INFO,"\njmesh system timer start fail!!!\n");
        return;
    }
    xTimerStart(jmesh_system_timer,0);
}
void jmesh_thread_start(void(*handler)(void)){
    xTaskCreate((TaskFunction_t)handler,"Jmesh Thread",1600,NULL,4,NULL);
}


void jmesh_random_init(int seed)
{
//    srand(seed);
}
int jmesh_random_get(int min,int max){
    int r;
    if(min>=max){
        return min;
    }
    r=rand();
    r=min+r%(max-min);
    return r;
}

void jmesh_task_sleep_init(void)
{
    sleep_xSemaphore = xSemaphoreCreateBinary();
    xSemaphoreTake(sleep_xSemaphore,0);
}

void jmesh_task_sleep(int ticks)
{
    xSemaphoreTake(sleep_xSemaphore,ticks);
}

void jmesh_task_wakeup(unsigned char with_isr)
{
    if(with_isr){
        BaseType_t xHigherPriorityTaskWoken=pdFALSE;
        xSemaphoreGiveFromISR(sleep_xSemaphore,&xHigherPriorityTaskWoken);
	      if(xHigherPriorityTaskWoken==pdTRUE){
		        portYIELD_FROM_ISR(pdTRUE);
	      }
    }
    else{
        xSemaphoreGive(sleep_xSemaphore);
    }
}
#endif

