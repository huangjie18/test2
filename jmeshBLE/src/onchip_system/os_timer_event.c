#include"os_timer_event.h"
#include"os_core.h"
#include"os_list.h"
#include"../driver/jmesh_system.h"

static os_timer_event_t* timer_event_list=NULL;
typedef void* os_mutex_t;
static os_mutex_t timer_event_mutex=NULL;
static void os_timer_event_add(os_timer_event_t* event);
static os_timer_event_t *os_timer_get_flow(os_time_t now,os_time_t *expired_time);
OS_TASK(os_timer_event,event,data)
{
    OS_START();
    os_timer_event_t* e;
    os_time_t expired_time;
    os_time_t now;
    os_list_init((os_list_t)&timer_event_list);
    timer_event_mutex=jmesh_mutex_new();
    while(1){
        OS_YIELD();
        expired_time=OS_TIMER_EVENT_MAX_EXPIRED_TICKS;
        now=os_time_ticks();
        while(NULL!=(e=os_timer_get_flow(now,&expired_time))){
            os_timer_event_remove(e);
            if(e->caller!=NULL){
                e->caller(e->data);
            }
        }
        os_timer_event_schedule(expired_time);
    }
    OS_END();
}

void os_timer_event_set(os_timer_event_t* e,os_time_t interval_ms,os_timer_event_caller_t caller,os_data_t data)
{
    os_timer_set(&e->timer,interval_ms);
    e->caller=caller;
    e->data=data;
    os_timer_event_add(e);
}
//#include"log.h"
//extern os_timer_event_t routing_beacon_timer;
//extern os_timer_event_t neighbor_maintain_timer;
static void os_timer_event_add(os_timer_event_t* event)
{
    os_timer_event_t* e;
    //os_timer_event_t* cache[60];
    //int i=0;


    jmesh_mutex_lock(timer_event_mutex);

    for(e=timer_event_list;e!=NULL;e=e->next){
        //cache[i++]=e;
        if(e==event){
            jmesh_mutex_unlock(timer_event_mutex);
            os_task_poll(&os_timer_event,0);
            return;
        }
    }
    event->next=timer_event_list;
    timer_event_list=event;


    /*while(i!=0)
      LOG(LOG_LVL_INFO,"a:%x,",cache[--i]);
    for(e=timer_event_list;e!=NULL;e=e->next){
      if(e==&routing_beacon_timer||e==&neighbor_maintain_timer){
          jmesh_mutex_unlock(timer_event_mutex);
          os_task_poll(&os_timer_event,0);
          return;
      }
    }
    for(e=timer_event_list;e!=NULL;e=e->next){
        LOG(LOG_LVL_INFO,"b:%x,",e);
    }
    LOG(LOG_LVL_INFO,"ac:%x\n",event);
    JMESH_ASSERT(0);*/
    jmesh_mutex_unlock(timer_event_mutex);
    os_task_poll(&os_timer_event,0);
}

void os_timer_event_remove(os_timer_event_t* event)
{
    os_timer_event_t* e;
    //os_timer_event_t* cache[60];
    //int i=0;
    jmesh_mutex_lock(timer_event_mutex);
    /*for(e=timer_event_list;e!=NULL;e=e->next){
        cache[i++]=e;
    }*/
    if(event==timer_event_list){
        timer_event_list=event->next;
    }
    else{
        if(timer_event_list==NULL){
            jmesh_mutex_unlock(timer_event_mutex);
            return;
        }
        for(e=timer_event_list;e->next!=NULL;e=e->next){
            if(e->next==event){
                e->next=event->next;
                break;
            }
        }
    }
    /*while(i!=0)
      LOG(LOG_LVL_INFO,"c:%x,",cache[--i]);
    for(e=timer_event_list;e!=NULL;e=e->next){
      if(e==&routing_beacon_timer||e==&neighbor_maintain_timer){
          jmesh_mutex_unlock(timer_event_mutex);
          os_task_poll(&os_timer_event,0);
          return;
      }
    }
    for(e=timer_event_list;e!=NULL;e=e->next){
        LOG(LOG_LVL_INFO,"d:%x,",e);
    }
    LOG(LOG_LVL_INFO,"rc:%x\n",event);
    JMESH_ASSERT(0);*/
    jmesh_mutex_unlock(timer_event_mutex);
    os_task_poll(&os_timer_event,0);
}


void os_timer_event_reset(os_timer_event_t* e)
{
    os_timer_reset(&e->timer);
    os_timer_event_add(e);
}

void os_timer_event_restart(os_timer_event_t* e)
{
    os_timer_restart(&e->timer);
    os_timer_event_add(e);
}

void os_timer_event_trigger(os_timer_event_t* e)
{
    os_timer_trigger(&e->timer);
    os_task_poll(&os_timer_event,0);
}
int os_timer_event_is_active(os_timer_event_t* event)
{
    os_timer_event_t* e;
    for(e=timer_event_list;e!=NULL;e=e->next){
        if(e==event){
            return !os_timer_is_flow(&event->timer);
        }
    }
    return 0;
}

static os_timer_event_t *os_timer_get_flow(os_time_t now,os_time_t *expired_time)
{
    os_timer_event_t* e;
    jmesh_mutex_lock(timer_event_mutex);
    for(e=timer_event_list;e!=NULL;e=e->next){
        if(os_timer_is_flow(&e->timer)){
            jmesh_mutex_unlock(timer_event_mutex);
            return e;
        }
        else if(*expired_time>e->timer.start+e->timer.interval-now){
            *expired_time=e->timer.start+e->timer.interval-now;
        }
    }
    jmesh_mutex_unlock(timer_event_mutex);
    return NULL;
}

#include"jmesh_print.h"
void jmesh_get_timer_list(void) //for test
{
    os_timer_event_t* e;
    for(e=timer_event_list;e!=NULL;e=e->next){
        if(e){
            JMESH_LOGI("timer","0x%x,0x%x\n",e,e->caller);
        }
    }
}


