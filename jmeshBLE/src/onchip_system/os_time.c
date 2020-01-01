#include"os_time.h"
#include"os_timer_event.h"
#include"../driver/jmesh_driver_config.h"
#if (JMESH_DRIVER_BX2400)
#include"io_ctrl.h"
#include"task.h"
#endif

static volatile unsigned short millseconds=0;
static volatile os_time_t last_ticks = 0;
static os_time_t os_timer_event_expired_time;
static volatile unsigned short jmesh_net_ticks=0;
/*
    this function must be called each OS_TICKS_MS in timer interrupter
*/
/*
void os_time_interrupt(void)
{
    ticks++;
    jmesh_net_ticks++;

    if(ticks==os_timer_event_expired_time){
        os_task_poll(&os_timer_event);
    }
}
void os_time_ticks_add(unsigned short t)
{
    ticks+=t;
    jmesh_net_ticks+=t;
    if(ticks>=os_timer_event_expired_time){//no matter overflow
        os_task_poll(&os_timer_event);
    }
}*/

void os_time_ticks_trigger(void)
{
  unsigned long t;
  t=xTaskGetTickCount();
  if(t-last_ticks>=os_timer_event_expired_time&&os_timer_event_expired_time!=0){
    os_timer_event_expired_time=0;
    os_task_poll(&os_timer_event,0);
    last_ticks=t;
    jmesh_net_ticks=t;
  }
}

void os_timer_event_schedule(os_time_t expired_time)
{
  os_timer_event_expired_time=expired_time;
  if(expired_time==0){
    os_task_poll(&os_timer_event,0);
  }
  last_ticks=xTaskGetTickCount();
}

os_time_t os_time_get_expired(void){
  os_time_t now=os_time_ticks();
  if(last_ticks+os_timer_event_expired_time>now){
    return last_ticks+os_timer_event_expired_time-now;
  }
  else{
    return 0;
  }
}

os_time_t os_time_ticks(void)
{
    return xTaskGetTickCount();
}

os_time_t os_time_ticks_to_ms(os_time_t ticks)
{
    return ticks*(1000/OS_TICKS_FREQUENCY);
}

os_time_t os_time_ms_to_ticks(os_time_t ms)
{
    return (ms*OS_TICKS_FREQUENCY+999)/1000;
}

void os_time_wait(os_time_t i)
{
    os_time_t start;

    start = os_time_ticks();
    while(os_time_ticks() - start < (os_time_t)i);
}


unsigned long os_time_get_seconds(void)
{
  return xTaskGetTickCount()/OS_TICKS_FREQUENCY;
}


void jmesh_net_ticks_set(unsigned short ticks)
{
    do{
        jmesh_net_ticks=ticks;
    }while(jmesh_net_ticks!=ticks);
}

unsigned short jmesh_net_ticks_get(void)
{
    unsigned short t1,t2;
    do{
        t1=jmesh_net_ticks;
        t2=jmesh_net_ticks;
    }while(t1!=t2);
    return t1;
}

