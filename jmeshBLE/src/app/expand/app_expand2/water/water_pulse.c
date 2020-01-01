

#include"water_pulse.h"
#include"io_ctrl.h"
#include"water.h"
#include"../../../../jmesh/jmesh_print.h"
#include"FreeRTOS.h"
#include"timers.h"
#include "reg_sysc_awo.h"
#include"plf.h"
static void water_1pulse_a_handler(void);
static void water_2pulse_a_handler(void);
static void water_2pulse_b_handler(void);
static void water_3pulse_a_handler(void);
static void water_3pulse_b_handler(void);
static void water_3pulse_c_handler(void);

#include"os_timer_event.h"
static os_timer_event_t io_read_test;

static void io_read_test_handler(void)
{
    //JMESH_LOGI("io","a:%d,b:%d\n",io_pin_read(15),io_pin_read(17));
    os_timer_event_restart(&io_read_test);
}

static void water_1pulse_a_handler(void)
{
    //JMESH_LOGI("p","1_p_a:%d\n",xTaskGetTickCountFromISR()*10/1000);
    water_pulse_add();
}
#include"log.h"
static unsigned char cycle_flag=0;

static os_timer_event_t a_delay_timer;

static os_timer_event_t b_delay_timer;

static unsigned long a_int_valid_t=0;

static unsigned char pulse_a,pulse_b,pulse_c;

static void jmesh_pulse_int_poll(unsigned char pulse_num);

static void water_2pulse_a_delay_handler(void)
{
    int i,times=0;
    for(i=0;i<10;i++){
        BX_DELAY_US(10);
        if(io_pin_read(pulse_a)==0&&io_pin_read(pulse_b)==1){
            times++;
        }
        else{
            times=0;
        }
    }
    if(times>=8){
        cycle_flag=1;
        a_int_valid_t=xTaskGetTickCount();
    }
}

static void water_2pulse_b_delay_handler(void)
{
    int i,times=0;
    unsigned long t=xTaskGetTickCount();
    if(t<=a_int_valid_t||t-a_int_valid_t<10){
        return;
    }
    for(i=0;i<10;i++){
        BX_DELAY_US(10);
        if(io_pin_read(pulse_b)==1&&io_pin_read(pulse_a)==1){
            times++;
        }
        else{
            times=0;
        }
    }
    if(times>=8&&cycle_flag==1){
        cycle_flag=0;
        water_pulse_add();
    }
}

static void water_2pulse_a_handler(void)
{
    jmesh_pulse_int_poll(1);
}

static void water_2pulse_b_handler(void)
{
    jmesh_pulse_int_poll(2);
}


static void water_3pulse_a_handler(void)
{
    cycle_flag=1;
    //JMESH_LOGI("p","3_p_a:%d\n",xTaskGetTickCountFromISR()*10/1000);
}
static void water_3pulse_b_handler(void)
{
    if(cycle_flag==1){
        cycle_flag=2;
        //JMESH_LOGI("p","3_p_b:%d\n",xTaskGetTickCountFromISR()*10/1000);
    }
}
static void water_3pulse_c_handler(void)
{
    if(cycle_flag==2){
        cycle_flag=0;
        //JMESH_LOGI("p","3_p_c:%d\n",xTaskGetTickCountFromISR()*10/1000);
        water_pulse_add();
    }
}

void water_1pulse_uinit(unsigned char io_a)
{
    io_cfg_input(io_a);
}
void water_1pulse_init(unsigned char io_a)
{
    io_ext_int_cfg(io_a,EXT_INT_TRIGGER_POS_EDGE,water_1pulse_a_handler);
    io_ext_int_en(io_a,true);
    io_pin_pull_write(io_a,IO_PULL_UP);
}
void water_2pulse_uinit(unsigned char io_a,unsigned char io_b)
{
    io_cfg_input(io_a);
    io_cfg_input(io_b);
}

void water_2pulse_init(unsigned char io_a,unsigned char io_b)
{
    LOG(LOG_LVL_INFO,"a:%d,b:%d\n",io_a,io_b);
    pulse_a=io_a;
    pulse_b=io_b;

    io_ext_int_cfg(pulse_a,EXT_INT_TRIGGER_NEG_EDGE,water_2pulse_a_handler);
    io_ext_int_en(pulse_a,true);

    io_ext_int_cfg(pulse_b,EXT_INT_TRIGGER_POS_EDGE,water_2pulse_b_handler);
    io_ext_int_en(pulse_b,true);
}
void water_3pulse_uinit(unsigned char io_a,unsigned char io_b,unsigned char io_c)
{
    io_cfg_input(io_a);
    io_cfg_input(io_b);
    io_cfg_input(io_c);
}
void water_3pulse_init(unsigned char io_a,unsigned char io_b,unsigned char io_c)
{
    io_ext_int_cfg(io_a,EXT_INT_TRIGGER_POS_EDGE,water_3pulse_a_handler);
    io_ext_int_en(io_a,true);
    io_pin_pull_write(io_a,IO_PULL_UP);

    io_ext_int_cfg(io_b,EXT_INT_TRIGGER_POS_EDGE,water_3pulse_b_handler);
    io_ext_int_en(io_b,true);
    io_pin_pull_write(io_b,IO_PULL_UP);

    io_ext_int_cfg(io_c,EXT_INT_TRIGGER_POS_EDGE,water_3pulse_c_handler);
    io_ext_int_en(io_c,true);
    io_pin_pull_write(io_c,IO_PULL_UP);
}

static volatile unsigned char pulse_a_int=0,pulse_b_int=0,pulse_c_int=0;

OS_TASK(os_pulse_int_e,event,data)
{
    OS_START();
    while(1){
        OS_YIELD();
        if(pulse_a_int){
            pulse_a_int=0;
            os_timer_event_set(&a_delay_timer,20,(os_timer_event_caller_t)water_2pulse_a_delay_handler,NULL);
        }
        if(pulse_b_int){
            pulse_b_int=0;
            os_timer_event_set(&b_delay_timer,20,(os_timer_event_caller_t)water_2pulse_b_delay_handler,NULL);
        }
        if(pulse_c_int){
            pulse_c_int=0;
            //set timer
        }
    }
    OS_END();
}

static void jmesh_pulse_int_poll(unsigned char pulse_num)
{
    switch(pulse_num){
    case 1:
        pulse_a_int=1;
        break;
    case 2:
        pulse_b_int=1;
        break;
    case 3:
        pulse_c_int=1;
        break;
    default:
        return;
    }
    os_task_poll(&os_pulse_int_e,1);
}

