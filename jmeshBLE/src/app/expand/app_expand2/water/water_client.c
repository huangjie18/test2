#include"stdio.h"
#include"water.h"
#include"water_client.h"
#include"water_server.h"
#include"../app_expand2.h"
#include"../../../../foundation/config/configuration_cmd.h"


void water_client_value_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_param_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_mac_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_type_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_valve_pin_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_report_para_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_value_unit_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_pulse_pin_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void water_client_init(void)
{
    static app_expand_message_t value_status ;
    static app_expand_message_t param_status ;
    static app_expand_message_t mac_status;
    static app_expand_message_t water_type_s;
    static app_expand_message_t valve_pin_s;
    static app_expand_message_t report_para_s;
    static app_expand_message_t value_unit_s;
    static app_expand_message_t pulse_pin_s;
    jmesh_app_expand2_message_register(&value_status,       JMESH_APP_EXPAND2_WATER_VALUE_STATUS,   water_client_value_status_handler);
    jmesh_app_expand2_message_register(&param_status,       JMESH_APP_EXPAND2_WATER_PARAM_STATUS,   water_client_param_status_handler);
    jmesh_app_expand2_message_register(&mac_status,         JMESH_APP_EXPAND2_WATER_MAC_STATUS,     water_client_mac_status_handler);
    jmesh_app_expand2_message_register(&water_type_s,       JMESH_APP_EXPAND2_WATER_TYPE_STATUS,    water_client_type_status_handler);
    jmesh_app_expand2_message_register(&valve_pin_s,        JMESH_APP_EXPAND2_VALVE_PIN_STATUS,     water_client_valve_pin_status_handler);
    jmesh_app_expand2_message_register(&report_para_s,      JMESH_APP_EXPAND2_REPORT_PARA_STATUS,   water_client_report_para_status_handler);
    jmesh_app_expand2_message_register(&value_unit_s,       JMESH_APP_EXPAND2_VALUE_UNIT_STATUS,    water_client_value_unit_status_handler);
    jmesh_app_expand2_message_register(&pulse_pin_s,        JMESH_APP_EXPAND2_PULSE_PIN_STATUS,     water_client_pulse_pin_status_handler);

}

void water_client_read_value(unsigned short dst)
{
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_READ_VALUE,0,NULL);
}
void water_client_write_value(unsigned short dst,unsigned long value,unsigned char unit)
{
    unsigned char buffer[5];
    memcpy(buffer,&value,4);
    buffer[4]=unit;
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_WRITE_VALUE,5,buffer);
}
void water_client_set_valve(unsigned short dst,unsigned char status)
{
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_SET_VALVE,1,&status);
}

void water_client_get_valve(unsigned short dst)
{
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_GET_VALVE,0,NULL);
}

void water_client_set_param(unsigned short dst,water_param_t *param)
{
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_SET_PARAM,sizeof(water_param_t),(unsigned char *)param);
}
void water_client_read_param(unsigned short dst)
{
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_READ_PARAM,0,NULL);
}
void water_client_set_mac(unsigned short dst,unsigned char* mac)
{
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_SET_MAC,WATER_MAC_SIZE,mac);
}
void water_client_read_mac(unsigned short dst)
{
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_READ_MAC,0,NULL);
}

void water_client_set_type(unsigned short dst,unsigned char water_type)
{
    unsigned char data[1];
    data[0]=water_type;
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_SET_TYPE,1,data);
}

void water_client_get_type(unsigned short dst)
{
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_GET_TYPE,0,NULL);
}

void water_client_set_valve_pin(unsigned short dst,unsigned char valve_pin)
{
    unsigned char data[1];
    data[0]=valve_pin;
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_SET_VALVE_PIN,1,data);
}

void water_client_get_valve_pin(unsigned short dst)
{
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_GET_VALVE_PIN,0,NULL);
}

void water_client_set_report_para(unsigned short dst,unsigned short report_intv_ms,unsigned short report_dst)
{
    unsigned char data[4];
    memcpy(data,&report_intv_ms,2);
    memcpy(data+2,&report_dst,2);
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_SET_REPORT_PARA,4,data);
}

void water_client_get_report_para(unsigned short dst)
{
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_GET_REPORT_PARA,0,NULL);
}

void water_client_set_value_unit(unsigned short dst,unsigned char value_unit)
{
    unsigned char data[1];
    data[0]=value_unit;
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_SET_VALUE_UNIT,1,data);
}

void water_client_get_value_unit(unsigned short dst)
{
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_GET_VALUE_UNIT,0,NULL);
}

void water_client_set_pulse_pin(unsigned short dst,unsigned char pulse_a,unsigned char pulse_b,unsigned char pulse_c)
{
    unsigned char data[3];
    data[0]=pulse_a;
    data[1]=pulse_b;
    data[2]=pulse_c;
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_SET_PULSE_PIN,3,data);
}

void water_client_get_pulse_pin(unsigned short dst)
{
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_WATER_GET_PULSE_PIN,0,NULL);
}

void water_client_value_status_callback(unsigned short src,unsigned long value,unsigned char unit,unsigned char status)
{
    //JMESH_LOGI("get","val:%d,unit:%d,sta:0x%02x\n",value,unit,status);
    JMESH_CMD_RES(cur_cmd_interface,"%d,%d,0x%02x\n",value,unit,status);
}
void water_client_param_status_callback(unsigned short src,water_param_t *param)
{
    //JMESH_LOGI("get","typ:%d,pin:%d,intv:%d,des:%d,unit:%d,p_a:%d,p_b:%d,p_c:%d\n",param->type,param->valve_pin,param->report_intv_ms,param->report_dst,param->unit,param->pulse_a,param->pulse_b,param->pulse_c);
    //JMESH_LOGI_BUFFER("get",7,param->mac,"mac:");
    JMESH_CMD_RES(cur_cmd_interface,"%d,%d,%d,%d,%d,%d,%d,%d\n",param->type,param->valve_pin,param->report_intv_s,param->report_dst,param->unit,param->pulse_a,param->pulse_b,param->pulse_c);
}
void water_client_mac_status_callback(unsigned short src,unsigned char* mac)
{
    JMESH_LOGI_BUFFER("get",7,mac,"mac:");
}

void water_client_type_status_callback(unsigned short src,unsigned char type)
{
    //JMESH_LOGI_BUFFER("get",7,mac,"mac:");
    JMESH_CMD_RES(cur_cmd_interface,"%d\n",type);
}

void water_client_valve_pin_status_callback(unsigned short src,unsigned char valve_pin)
{
    //JMESH_LOGI_BUFFER("get",7,mac,"mac:");
    JMESH_CMD_RES(cur_cmd_interface,"%d\n",valve_pin);
}

void water_client_report_para_status_callback(unsigned short src,unsigned short report_intv_s,unsigned short report_dst)
{
    //JMESH_LOGI_BUFFER("get",7,mac,"mac:");
    JMESH_CMD_RES(cur_cmd_interface,"%d,%d\n",report_intv_s,report_dst);
}

void water_client_value_unit_status_callback(unsigned short src,unsigned char value_unit)
{
    //JMESH_LOGI_BUFFER("get",7,mac,"mac:");
    JMESH_CMD_RES(cur_cmd_interface,"%d\n",value_unit);
}

void water_client_pulse_pin_status_callback(unsigned short src,unsigned char pulse_a,unsigned char pulse_b,unsigned char pulse_c)
{
    //JMESH_LOGI_BUFFER("get",7,mac,"mac:");
    JMESH_CMD_RES(cur_cmd_interface,"%d,%d,%d\n",pulse_a,pulse_b,pulse_c);
}




void water_client_value_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned long value;
    if(len==6){
        memcpy(&value,data,4);
        water_client_value_status_callback(src,value,data[4],data[5]);
    }
}
void water_client_param_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    water_param_t param;
    if(len==sizeof(water_param_t)){
        memcpy(&param,data,sizeof(water_param_t));
        water_client_param_status_callback(src,&param);
    }
}
void water_client_mac_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    if(len==WATER_MAC_SIZE){
        water_client_mac_status_callback(src,data);
    }
}

void water_client_type_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    if(len==1){
        water_client_type_status_callback(src,*data);
    }
}

void water_client_valve_pin_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    if(len==1){
        water_client_valve_pin_status_callback(src,*data);
    }
}

void water_client_report_para_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned short report_intv_s;
    unsigned short report_dst;
    if(len==4){
        memcpy(&report_intv_s,data,2);
        memcpy(&report_dst,data+2,2);
        water_client_report_para_status_callback(src,report_intv_s,report_dst);
    }
}

void water_client_value_unit_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    if(len==1){
        water_client_value_unit_status_callback(src,data[0]);
    }
}

void water_client_pulse_pin_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    if(len==3){
        water_client_pulse_pin_status_callback(src,data[0],data[1],data[2]);
    }
}

