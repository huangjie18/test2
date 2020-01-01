#include"light.h"
#include"../../../../jmesh/jmesh_print.h"
#include"../../../../onchip_system/os_time.h"
#include"../../../../onchip_system/os_timer_event.h"
#include"../../../../driver/jmesh_gpio.h"


void light_set(unsigned char* value)
{
}
void light_get(unsigned char* value)
{
    ;
}

void light_on(void){
    //JMESH_LOGI("light","on\n");
    jmesh_gpio_set(JMESH_LED1, GPIO_VALUE_LOW);
}
void light_off(void){
    //JMESH_LOGI("light","off\n");
    jmesh_gpio_set(JMESH_LED1, GPIO_VALUE_HIGH);
}


static unsigned long base_ms,on_ms,off_ms,live_ms;
static os_timer_event_t timer;

static void light_mode_timer_handler(void)
{
    unsigned long now_ms;

    now_ms=os_time_ticks_to_ms(jmesh_net_ticks_get());
    //JMESH_LOGI("now1","%d\n",now_ms);
    now_ms=(now_ms +os_time_ticks_to_ms(0xffff) - base_ms)%(on_ms+off_ms);
    //JMESH_LOGI("now2","%d\n",now_ms);
    if(live_ms!=0){
        if(((now_ms + os_time_ticks_to_ms(0xffff) - base_ms  )%os_time_ticks_to_ms(0xffff)) > live_ms){
            return;
        }
    }
    if(now_ms>=on_ms){
        light_off();
        //JMESH_LOGI("off","%d\n",on_ms+off_ms-now_ms);
        os_timer_event_set(&timer,on_ms+off_ms-now_ms,(os_timer_event_caller_t)light_mode_timer_handler,NULL);
    }
    else{
        light_on();
        //JMESH_LOGI("on","%d\n",on_ms-now_ms);
        os_timer_event_set(&timer,on_ms-now_ms,(os_timer_event_caller_t)light_mode_timer_handler,NULL);
    }
}
void light_start_mode(unsigned short base_ticks,unsigned short on_ticks,unsigned short off_ticks,unsigned short live_ticks)
{
    unsigned long now_ms;
    if(on_ticks==0){
        light_off();
        return;
    }
    if(off_ticks==0){
        light_on();
        return;
    }
    base_ms=os_time_ticks_to_ms(base_ticks);
    on_ms=os_time_ticks_to_ms(on_ticks);
    off_ms=os_time_ticks_to_ms(off_ticks);
    live_ms=os_time_ticks_to_ms(live_ticks);
    now_ms=os_time_ticks_to_ms(jmesh_net_ticks_get());
    now_ms=(now_ms + os_time_ticks_to_ms(0xffff) - base_ms) % (on_ms + off_ms);
    if(now_ms>=on_ms){
        light_off();
        os_timer_event_set(&timer,on_ms+off_ms-now_ms,(os_timer_event_caller_t)light_mode_timer_handler,NULL);
    }
    else{
        light_on();
        os_timer_event_set(&timer,on_ms-now_ms,(os_timer_event_caller_t)light_mode_timer_handler,NULL);
    }

}
