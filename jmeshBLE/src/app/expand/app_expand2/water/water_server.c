#include"water.h"
#include"water_client.h"
#include"water_server.h"
#include"../app_expand2.h"


void water_client_read_value_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_write_value_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_set_valve_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_get_valve_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_set_param_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_read_param_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_set_mac_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_read_mac_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_set_type_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_get_type_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_set_valve_pin_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_get_valve_pin_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_set_report_para_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_get_report_para_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_set_value_unit_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_get_value_unit_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_set_pulse_pin_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_get_pulse_pin_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);

void water_server_init(void)
{
    static app_expand_message_t read_value ;
    static app_expand_message_t write_value ;
    static app_expand_message_t set_valve;
    static app_expand_message_t get_valve;
    static app_expand_message_t set_param;
    static app_expand_message_t read_param;
    static app_expand_message_t set_mac;
    static app_expand_message_t read_mac;
    static app_expand_message_t set_water_type;
    static app_expand_message_t get_water_type;
    static app_expand_message_t set_valve_pin;
    static app_expand_message_t get_valve_pin;
    static app_expand_message_t set_report_para;
    static app_expand_message_t get_report_para;
    static app_expand_message_t set_value_unit;
    static app_expand_message_t get_value_unit;
    static app_expand_message_t set_pulse_pin;
    static app_expand_message_t get_pulse_pin;

    //TODO:read from flash
    jmesh_app_expand2_message_register(&read_value,     JMESH_APP_EXPAND2_WATER_READ_VALUE,     water_client_read_value_handler);
    jmesh_app_expand2_message_register(&write_value,    JMESH_APP_EXPAND2_WATER_WRITE_VALUE,    water_client_write_value_handler);
    jmesh_app_expand2_message_register(&set_valve,      JMESH_APP_EXPAND2_WATER_SET_VALVE,      water_client_set_valve_handler);
    jmesh_app_expand2_message_register(&get_valve,      JMESH_APP_EXPAND2_WATER_GET_VALVE,      water_client_get_valve_handler);
    jmesh_app_expand2_message_register(&set_param,      JMESH_APP_EXPAND2_WATER_SET_PARAM,      water_client_set_param_handler);
    jmesh_app_expand2_message_register(&read_param,     JMESH_APP_EXPAND2_WATER_READ_PARAM,     water_client_read_param_handler);
    jmesh_app_expand2_message_register(&set_mac,        JMESH_APP_EXPAND2_WATER_SET_MAC,        water_client_set_mac_handler);
    jmesh_app_expand2_message_register(&read_mac,       JMESH_APP_EXPAND2_WATER_READ_MAC,       water_client_read_mac_handler);
    jmesh_app_expand2_message_register(&set_water_type, JMESH_APP_EXPAND2_WATER_SET_TYPE,       water_client_set_type_handler);
    jmesh_app_expand2_message_register(&get_water_type, JMESH_APP_EXPAND2_WATER_GET_TYPE,       water_client_get_type_handler);
    jmesh_app_expand2_message_register(&set_valve_pin,  JMESH_APP_EXPAND2_WATER_SET_VALVE_PIN,  water_client_set_valve_pin_handler);
    jmesh_app_expand2_message_register(&get_valve_pin,  JMESH_APP_EXPAND2_WATER_GET_VALVE_PIN,  water_client_get_valve_pin_handler);
    jmesh_app_expand2_message_register(&set_report_para,JMESH_APP_EXPAND2_WATER_SET_REPORT_PARA,water_client_set_report_para_handler);
    jmesh_app_expand2_message_register(&get_report_para,JMESH_APP_EXPAND2_WATER_GET_REPORT_PARA,water_client_get_report_para_handler);
    jmesh_app_expand2_message_register(&set_value_unit, JMESH_APP_EXPAND2_WATER_SET_VALUE_UNIT, water_client_set_value_unit_handler);
    jmesh_app_expand2_message_register(&get_value_unit, JMESH_APP_EXPAND2_WATER_GET_VALUE_UNIT, water_client_get_value_unit_handler);
    jmesh_app_expand2_message_register(&set_pulse_pin,  JMESH_APP_EXPAND2_WATER_SET_PULSE_PIN,  water_client_set_pulse_pin_handler);
    jmesh_app_expand2_message_register(&get_pulse_pin,  JMESH_APP_EXPAND2_WATER_GET_PULSE_PIN,  water_client_get_pulse_pin_handler);

}


void water_client_read_value_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned long value;
    unsigned char buffer[6];
    value=water_get_value();
    memcpy(buffer,&value,4);
    buffer[4]=water_get_unit();
    buffer[5]=water_get_status();
    jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_WATER_VALUE_STATUS,6,buffer);
}
void water_client_write_value_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned long value;
    unsigned char buffer[6];
    if(len==5){
        memcpy(&value,data,4);
        water_set_value(value,data[4]);
        value=water_get_value();
        memcpy(buffer,&value,4);
        buffer[4]=water_get_unit();
        buffer[5]=water_get_status();
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_WATER_VALUE_STATUS,6,buffer);
    }
}
void water_client_set_valve_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned long value;
    unsigned char buffer[6];
    if(len==1){
        water_set_valve(data[0]);
        value=water_get_value();
        memcpy(buffer,&value,4);
        buffer[4]=water_get_unit();
        buffer[5]=water_get_status();
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_WATER_VALUE_STATUS,6,buffer);
    }
}
void water_client_get_valve_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned long value;
    unsigned char buffer[6];
    if(len==0){
        value=water_get_value();
        memcpy(buffer,&value,4);
        buffer[4]=water_get_unit();
        buffer[5]=water_get_status();
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_WATER_VALUE_STATUS,6,buffer);
    }
}
void water_client_set_param_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    water_param_t param;
    if(len==sizeof(water_param_t)){
        memcpy(&param,data,len);
        water_set_param(&param);
        water_get_param(&param);
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_WATER_PARAM_STATUS,sizeof(water_param_t),(unsigned char *)&param);
    }

}

void water_client_read_param_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    water_param_t param;
    if(len==0){
        water_get_param(&param);
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_WATER_PARAM_STATUS,sizeof(water_param_t),(unsigned char *)&param);
    }
}
void water_client_set_mac_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned char mac[WATER_MAC_SIZE];
    if(len==WATER_MAC_SIZE){
        water_set_mac(data);
        water_get_mac(mac);
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_WATER_MAC_STATUS,WATER_MAC_SIZE,mac);
    }
}
void water_client_read_mac_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned char mac[WATER_MAC_SIZE];
    if(len==0){
        water_get_mac(mac);
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_WATER_MAC_STATUS,WATER_MAC_SIZE,mac);
    }
}

void water_client_set_type_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    if(len==1){
        water_set_type(*data);
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_WATER_TYPE_STATUS,1,data);
    }
}

void water_client_get_type_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned char buffer[1];
    if(len==0){
        buffer[0]=water_get_type();
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_WATER_TYPE_STATUS,1,buffer);
    }
}

void water_client_set_valve_pin_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    if(len==1){
        water_set_valve_pin(*data);
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_VALVE_PIN_STATUS,1,data);
    }
}

void water_client_get_valve_pin_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned char buffer[1];
    if(len==0){
        buffer[0]=water_get_valve_pin();
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_VALVE_PIN_STATUS,1,buffer);
    }
}

void water_client_set_report_para_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned short report_intv,addr;
    if(len==4){
        memcpy(&report_intv,data,2);
        memcpy(&addr,data+2,2);
        water_set_report(report_intv,addr);
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_REPORT_PARA_STATUS,4,data);
    }
}

void water_client_get_report_para_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned short report_intv,addr;
    unsigned char buffer[4];
    if(len==0){
        water_get_report(&report_intv,&addr);
        memcpy(buffer,&report_intv,2);
        memcpy(buffer+2,&addr,2);
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_REPORT_PARA_STATUS,4,buffer);
    }
}

void water_client_set_value_unit_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    if(len==1){
        water_set_value_unit(*data);
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_VALUE_UNIT_STATUS,1,data);
    }
}

void water_client_get_value_unit_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned char buffer[1];
    if(len==0){
        buffer[0]=water_get_value_unit();
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_VALUE_UNIT_STATUS,1,buffer);
    }
}

void water_client_set_pulse_pin_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    if(len==3){
        water_set_pulse_pin(data[0],data[1],data[2]);
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_PULSE_PIN_STATUS,3,data);
    }
}

void water_client_get_pulse_pin_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned char buffer[3];
    if(len==0){
        water_get_pulse_pin(&buffer[0],&buffer[1],&buffer[2]);
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_PULSE_PIN_STATUS,3,buffer);
    }
}
