#ifndef OS_TIMER_EVENT_H
#define OS_TIMER_EVENT_H

#include"os_timer.h"
#include"../jmesh/jmesh_features.h"

#define OS_TIMER_EVENT_MAX_EXPIRED_TICKS 360000//((JMESH_FEATURE_ENABLE==jmesh_get_lowpower_feature())?360000:360000)
#define OS_TIMER_EVENT_CALLER_SIZE 10

typedef void(*os_timer_event_caller_t)(os_data_t data);

typedef volatile struct st_os_timer_event os_timer_event_t;

struct st_os_timer_event{
    os_timer_event_t* next;
    os_timer_t timer;
    os_timer_event_caller_t caller;
    os_data_t data;
};

void os_timer_event_set(os_timer_event_t* e,os_time_t interval_ms,os_timer_event_caller_t caller,os_data_t data);
void os_timer_event_reset(os_timer_event_t* e);
void os_timer_event_restart(os_timer_event_t* e);
void os_timer_event_trigger(os_timer_event_t* e);
int os_timer_event_is_active(os_timer_event_t* e);
void os_timer_event_remove(os_timer_event_t* event);


extern void os_timer_event_schedule(os_time_t expired_time);

OS_TASK_NAME(os_timer_event);







#endif // OS_EVENT_H
