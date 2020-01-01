#include"water.h"
#include"water_pulse.h"
#include"water_client.h"
#include"water_server.h"
#include"../app_expand2.h"
#include"io_ctrl.h"
#include"log.h"


#include"../../../../driver/jmesh_gpio.h"
#include"../../../../jmesh/jmesh_print.h"
#include"../../../../jmesh/jmesh_save.h"
#include"../../../../onchip_system/os_timer_event.h"
#include"../../../../foundation/state/jmesh_state.h"

#include"FreeRTOS.h"
#include"timers.h"



//bx2400:p16,p22
//rf04:p15,p17
#define WATER_PARAM_DEFAULT {WATER_TYPE_PULSE2,21,30,3,/*60,*/100,17,15,0,{11,22,33,44,55,66,77}};
#define WATER_VALVE_PIN_DEFAULT 21

static water_param_t water_param=WATER_PARAM_DEFAULT;
static volatile unsigned long water_value=0;
static os_timer_event_t report_timer;
static os_timer_event_t save_timer;
//os_timer_t report_timer;

void water_report(unsigned short dst)
{
    jmesh_mac_t mac;
    unsigned char buffer[12];
    unsigned long value=water_get_value();
    //static unsigned long value=0;

    //value++;//test
    memcpy(buffer,&value,4);
    buffer[4]=water_param.unit;
    buffer[5]=water_get_status();
    jmesh_mac_get(mac);
    memcpy(&buffer[6],mac,6);
    //JMESH_LOGI("report","dst:%d,value:%d\n",water_param.report_dst,value);
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_REPORT,12,buffer);
}
void water_auto_report(void)
{
    jmesh_device_role_t device_roles;
    device_roles=jmesh_get_device_roles();
    if(!device_roles.water){
        return;
    }
    //if(os_timer_is_flow(&report_timer)){
        water_report(water_param.report_dst+1);
    //    os_timer_restart(&report_timer);
    //}
    os_timer_event_set(&report_timer,water_param.report_intv_s*1000,(os_timer_event_caller_t)water_auto_report,NULL);
}

void water_init(void)
{
    water_param_t wa_para_temp;
    if(sizeof(water_param_t)==jmesh_save_read(WATER_PARAM,sizeof(water_param_t),(unsigned char *)&wa_para_temp)){
        water_param=wa_para_temp;
    }
    if(sizeof(water_value)!=jmesh_save_read(WATER_VALUE,sizeof(water_value),(unsigned char *)&water_value)){
        water_value=0;
    }
    os_timer_event_set(&report_timer,water_param.report_intv_s*1000,(os_timer_event_caller_t)water_auto_report,NULL);
    os_timer_event_set(&save_timer,60*1000,(os_timer_event_caller_t)water_save_value,NULL);
    //os_timer_set(&report_timer,water_param.report_intv_ms);
    if(water_param.type==WATER_TYPE_PULSE1){
        water_1pulse_init(water_param.pulse_a);
    }
    if(water_param.type==WATER_TYPE_PULSE2){
        water_2pulse_init(water_param.pulse_a,water_param.pulse_b);
    }
    if(water_param.type==WATER_TYPE_PULSE3){
        water_3pulse_init(water_param.pulse_a,water_param.pulse_b,water_param.pulse_c);
    }
    io_pin_dir_set(water_param.valve_pin,1);
}

/*void water_uninit(void)
{
    os_timer_event_remove(&report_timer);
}*/

void water_set_mac(unsigned char* mac)
{
    memcpy(water_param.mac,mac,WATER_MAC_SIZE);
    jmesh_save_write(WATER_PARAM,sizeof(water_param_t),(unsigned char *)&water_param);
}
void water_get_mac(unsigned char* mac)
{
    memcpy(mac,water_param.mac,WATER_MAC_SIZE);
}
int water_set_value(unsigned long litre,unsigned char unit)
{
    water_value=litre;
    jmesh_save_write(WATER_VALUE,4,(unsigned char *)&water_value);
    if(water_param.unit!=unit){
        water_param.unit=unit;
        jmesh_save_write(WATER_PARAM,sizeof(water_param_t),(unsigned char *)&water_param);
    }
    return 0;
}
unsigned long water_get_value(void)
{
    return water_value;
}

void water_save_value(void)
{
    os_timer_event_set(&save_timer,60*1000,(os_timer_event_caller_t)water_save_value,NULL);
    jmesh_save_write(WATER_VALUE,4,(unsigned char *)&water_value);
}
/*
    D0 D1   D2  D3  D4  D5  D6  D7
    00:开    0：正常
    01：关   1：欠压
    11：异常
*/
int water_get_valve(void)
{
    return io_pin_read(water_param.valve_pin);
}
unsigned char water_get_status(void)
{
    unsigned char status=0;
    status|=water_get_valve();//valve pin status
    status|=4;//voltage status
    return status;
}
unsigned char water_get_unit(void)
{
    return water_param.unit;
}
int water_set_time(unsigned long second)
{
    return 0;
}
unsigned long water_get_time(void)
{
    return os_time_get_seconds();
}
int water_get_history_data(unsigned long start_second,unsigned long end_second,unsigned short intv_s)
{
    return 0;
}
int water_get_history_record(void)
{
    return 0;
}
int water_set_valve(unsigned char onOff)
{
    //valve pin set;
    io_pin_write(water_param.valve_pin,onOff);
    return 0;
}

void water_set_type(unsigned char water_type)
{
    if(water_param.type!=water_type){
        water_param.type=water_type;
        jmesh_save_write(WATER_PARAM,sizeof(water_param_t),(unsigned char *)&water_param);
    }
}

unsigned char water_get_type(void)
{
    return water_param.type;
}

void water_set_valve_pin(unsigned char valve_pin)
{
    if(water_param.valve_pin!=valve_pin){
        water_param.valve_pin=valve_pin;
        jmesh_save_write(WATER_PARAM,sizeof(water_param_t),(unsigned char *)&water_param);
    }
}

unsigned char water_get_valve_pin(void)
{
    return water_param.valve_pin;
}

int water_set_report(unsigned short intv_s,unsigned short dst)
{
    if(water_param.report_intv_s!=intv_s||water_param.report_dst!=dst){
        water_param.report_intv_s=intv_s;
        water_param.report_dst=dst;
        jmesh_save_write(WATER_PARAM,sizeof(water_param_t),(unsigned char *)&water_param);
    }
    return 0;
}

void water_get_report(unsigned short *intv_s,unsigned short *dst)
{
    if(NULL!=intv_s&&NULL!=dst){
        *intv_s=water_param.report_intv_s;
        *dst=water_param.report_dst;
    }
}

void water_set_value_unit(unsigned char value_unit)
{
    if(water_param.unit!=value_unit){
        water_param.unit=value_unit;
        jmesh_save_write(WATER_PARAM,sizeof(water_param_t),(unsigned char *)&water_param);
    }
}

unsigned char water_get_value_unit(void)
{
    return water_param.unit;
}

void water_set_pulse_pin(unsigned char pulse_a,unsigned char pulse_b,unsigned char pulse_c)
{
    if(water_param.pulse_a!=pulse_a||water_param.pulse_b!=pulse_b||water_param.pulse_c!=pulse_c){
        water_param.pulse_a=pulse_a;
        water_param.pulse_b=pulse_b;
        water_param.pulse_c=pulse_c;
        jmesh_save_write(WATER_PARAM,sizeof(water_param_t),(unsigned char *)&water_param);
    }
}

void water_get_pulse_pin(unsigned char *pulse_a,unsigned char *pulse_b,unsigned char *pulse_c)
{
    if(NULL!=pulse_a){
        *pulse_a=water_param.pulse_a;
    }
    if(NULL!=pulse_b){
        *pulse_b=water_param.pulse_b;
    }
    if(NULL!=pulse_c){
        *pulse_c=water_param.pulse_c;
    }
}


void water_get_param(water_param_t* param)
{
    memcpy(param,&water_param,sizeof(water_param_t));
}

void water_set_param(water_param_t* param)
{
    if(param->type==WATER_TYPE_PULSE1){
        water_1pulse_uinit(water_param.pulse_a);
    }
    if(param->type==WATER_TYPE_PULSE2){
        water_2pulse_uinit(water_param.pulse_a,water_param.pulse_b);
    }
    if(param->type==WATER_TYPE_PULSE3){
        water_3pulse_uinit(water_param.pulse_a,water_param.pulse_b,water_param.pulse_c);
    }
    memcpy(&water_param,param,sizeof(water_param_t));

    if(param->type==WATER_TYPE_PULSE1){
        water_1pulse_init(water_param.pulse_a);
    }
    if(param->type==WATER_TYPE_PULSE2){
        water_2pulse_init(water_param.pulse_a,water_param.pulse_b);
    }
    if(param->type==WATER_TYPE_PULSE3){
        water_3pulse_init(water_param.pulse_a,water_param.pulse_b,water_param.pulse_c);
    }
    jmesh_save_write(WATER_PARAM,sizeof(water_param_t),(unsigned char *)&water_param);
}
void water_pulse_add(void)
{
    //static unsigned long t=0;
    //static unsigned char c=0;
    //unsigned long n=os_time_get_seconds();
    //static unsigned long last_tick=0;
    water_value+=1;
    //LOG(LOG_LVL_INFO,"V%d\n",water_value);
    //JMESH_LOGI("val","val:%d,%d\n",water_value,(xTaskGetTickCountFromISR()-last_tick)*10/1000);
    //last_tick=xTaskGetTickCountFromISR();
    /*c++;
    if(n-t>=60||c>=10){
        c=0;
        t=n;
        //jmesh_save_write(WATER_VALUE,4,(unsigned char *)&water_value);
    }*/
}



















