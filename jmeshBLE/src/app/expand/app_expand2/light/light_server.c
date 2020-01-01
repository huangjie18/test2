#include"light.h"
#include"light_client.h"
#include"light_server.h"
#include"../app_expand2.h"
#include"../../app_expand.h"
#include"../../../../jmesh/jmesh_addr.h"
#include"../../../../jmesh/jmesh_save.h"
#include"../../../../jmesh/jmesh_config.h"



void light_server_set_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void light_server_get_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void light_server_mode_set_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void light_server_mode_get_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void light_server_mode_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void light_server_bit_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void light_server_point_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);

static light_mode_param_t mode_param[LIGHT_MODE_SIZE];

void light_server_init(void)
{
    static app_expand_message_t set ;
    static app_expand_message_t get;
    static app_expand_message_t mode_set ;
    static app_expand_message_t mode_get;
    static app_expand_message_t mode ;
    static app_expand_message_t bit;
    static app_expand_message_t point;
    unsigned short addr;
    jmesh_app_expand2_message_register(&set,        JMESH_APP_EXPAND2_LIGHT_SET,        light_server_set_handler);
    jmesh_app_expand2_message_register(&get,        JMESH_APP_EXPAND2_LIGHT_GET,        light_server_get_handler);
    jmesh_app_expand2_message_register(&mode_set,   JMESH_APP_EXPAND2_LIGHT_MODE_SET,   light_server_mode_set_handler);
    jmesh_app_expand2_message_register(&mode_get,   JMESH_APP_EXPAND2_LIGHT_MODE_GET,   light_server_mode_get_handler);
    jmesh_app_expand2_message_register(&mode,       JMESH_APP_EXPAND2_LIGHT_MODE,       light_server_mode_handler);
    jmesh_app_expand2_message_register(&bit,        JMESH_APP_EXPAND2_LIGHT_BIT,        light_server_bit_handler);
    jmesh_app_expand2_message_register(&point,      JMESH_APP_EXPAND2_LIGHT_POINT,      light_server_point_handler);
    addr=jmesh_get_primary_addr();
    if(0==jmesh_save_buffer_read(JMESH_SAVE_LIGHT_MODE_PARA,0,sizeof(light_mode_param_t),(unsigned char *)&mode_param[0]))
    {
        //light_client_mode_set(addr+1,0,LIGHT_SPEED_TICKS,LIGHT_ON_TICKS,LIGHT_OFF_TICKS,0,LIGHT_DIRECTION);
    }
    else
    {
        //light_client_mode_set(addr+1,0,mode_param[0].speed_ticks,mode_param[0].on_ticks,mode_param[0].off_ticks,0,mode_param[0].direction);
    }
}

void light_server_set_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned char buffer[LIGHT_VALUE_NUM];
    light_set(data);
    light_get(buffer);
    if(jmesh_addr_get_type(dst)==JMESH_ADDR_UNICAST){
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_LIGHT_STATUS,LIGHT_VALUE_NUM,buffer);
    }
}
void light_server_get_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned char buffer[LIGHT_VALUE_NUM];
    light_get(buffer);
    jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_LIGHT_STATUS,LIGHT_VALUE_NUM,buffer);

}

void light_server_mode_set_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned char buffer[11];
    if(len==10){
        if(data[0]<LIGHT_MODE_SIZE){
            JMESH_BIG_ENDIAN_PICK2(mode_param[data[0]].speed_ticks,data+1);
            JMESH_BIG_ENDIAN_PICK2(mode_param[data[0]].on_ticks,data+3);
            JMESH_BIG_ENDIAN_PICK2(mode_param[data[0]].off_ticks,data+5);
            JMESH_BIG_ENDIAN_PICK2(mode_param[data[0]].live_ticks,data+7);
            mode_param[data[0]].direction=data[9];
            jmesh_save_buffer_write(JMESH_SAVE_LIGHT_MODE_PARA,data[0],sizeof(light_mode_param_t),(unsigned char *)&mode_param[data[0]]);
            light_client_mode(jmesh_get_primary_addr()+1,data[0]);
            buffer[0]=LIGHT_MODE_SET_ERROR_OK;
            memcpy(buffer+1,data,10);
        }
        else{
            buffer[0]=LIGHT_MODE_SET_ERROR_ID;
        }
    }
    else{
        buffer[0]=LIGHT_MODE_SET_ERROR_LEN;
    }
    if(jmesh_addr_get_type(dst)==JMESH_ADDR_UNICAST){
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_LIGHT_MODE_STATUS,11,buffer);
    }
}


void light_server_mode_get_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned char buffer[11];
    if(len==1){
        if(data[0]<LIGHT_MODE_SIZE){
            buffer[0]=LIGHT_MODE_SET_ERROR_OK;
            buffer[1]=data[0];
            JMESH_BIG_ENDIAN_FILL2(mode_param[data[0]].speed_ticks,buffer+2);
            JMESH_BIG_ENDIAN_FILL2(mode_param[data[0]].on_ticks,buffer+4);
            JMESH_BIG_ENDIAN_FILL2(mode_param[data[0]].off_ticks,buffer+6);
            JMESH_BIG_ENDIAN_FILL2(mode_param[data[0]].live_ticks,buffer+8);
            buffer[10]=mode_param[data[0]].direction;

        }
        else{
            buffer[0]=LIGHT_MODE_SET_ERROR_ID;
        }
    }
    else{
        buffer[0]=LIGHT_MODE_SET_ERROR_LEN;
    }
    jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_LIGHT_MODE_STATUS,11,buffer);
}

void light_server_mode_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned char buffer[11];
    if(len==1){
        if(data[0]<LIGHT_MODE_SIZE){
            if(mode_param[data[0]].direction)
            {
                light_start_mode((jmesh_get_primary_addr()+1)*mode_param[data[0]].speed_ticks,mode_param[data[0]].on_ticks,mode_param[data[0]].off_ticks,mode_param[data[0]].live_ticks);
            }
            else
            {
                light_start_mode(((mode_param[data[0]].on_ticks+mode_param[data[0]].off_ticks)/mode_param[data[0]].speed_ticks-jmesh_get_primary_addr()+1)*mode_param[data[0]].speed_ticks,mode_param[data[0]].on_ticks,mode_param[data[0]].off_ticks,mode_param[data[0]].live_ticks);
            }
            buffer[0]=0;
            buffer[1]=data[0];
            JMESH_BIG_ENDIAN_FILL2(mode_param[data[0]].speed_ticks,buffer+2);
            JMESH_BIG_ENDIAN_FILL2(mode_param[data[0]].on_ticks,buffer+4);
            JMESH_BIG_ENDIAN_FILL2(mode_param[data[0]].off_ticks,buffer+6);
            JMESH_BIG_ENDIAN_FILL2(mode_param[data[0]].live_ticks,buffer+8);
            buffer[10]=mode_param[data[0]].direction;
        }
    }
    if(jmesh_addr_get_type(dst) == JMESH_ADDR_UNICAST){
        jmesh_app_expand2_send(src,JMESH_APP_EXPAND2_LIGHT_MODE_STATUS,LIGHT_VALUE_NUM+1,buffer);
    }
}

void light_server_bit_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned short primary_addr;
    unsigned char byte_index,bit_index;
    primary_addr=jmesh_get_primary_addr();
    byte_index=(primary_addr-1)/16;
    bit_index=((primary_addr-1)%16)/2;

    if((primary_addr/16) < len){
        if(data[byte_index]& (1<<bit_index)){
            light_on();
            return;
        }
    }
    light_off();
}

void light_server_point_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    if(1==len){
        if(0==data[0]){
            light_off();

        }
        else if(1==data[0]){
            light_on();

        }

    }
}


