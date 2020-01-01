#ifndef JMESH_SYSTEM_H
#define JMESH_SYSTEM_H
#include"../jmesh/jmesh_config.h"

extern void jmesh_system_timer_start(unsigned short interval_ms,void(*handler)(void));
extern void jmesh_thread_start(void(*handler)(void));
extern void os_main(void* argv);
extern void jmesh_random_init(int seed);
extern int jmesh_random_get(int min,int max);
void* jmesh_mutex_new(void);
void jmesh_mutex_lock(void* mutex);
void jmesh_mutex_unlock(void* mutex);
void jmesh_mutex_interrupt_lock(void* mutex);
void jmesh_mutex_interrupt_unlock(void* mutex);
#ifdef JMESH_WDT_ENABLE
void jmesh_wdt_reset(void);
int jmesh_wdt_sub_regist(void);
void jmesh_wdt_init(void);
void jmesh_wdt_sub_reset(int id);
#else
    #define jmesh_wdt_reset()
    #define jmesh_wdt_sub_regist() 0
    #define jmesh_wdt_init()
    #define jmesh_wdt_sub_reset(id)
#endif

void jmesh_task_sleep_init(void);
void jmesh_task_sleep(int ticks);
void jmesh_task_wakeup(unsigned char with_isr);

#endif // JMESH_SYSTEM_H
