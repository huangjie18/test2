#include"light.h"
#include"light_client.h"
#include"light_server.h"
#include"../app_expand2.h"
#include"../../app_expand.h"



void light_client_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void light_client_mode_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void light_client_init(void)
{
    static app_expand_message_t light_status ;
    static app_expand_message_t mode_status;
    jmesh_app_expand2_message_register(&light_status,    JMESH_APP_EXPAND2_LIGHT_STATUS, light_client_status_handler);
    jmesh_app_expand2_message_register(&mode_status,  JMESH_APP_EXPAND2_LIGHT_MODE_STATUS,   light_client_mode_status_handler);
}



void light_client_set(unsigned short dst,unsigned char* value)
{
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_LIGHT_SET,LIGHT_VALUE_NUM,value);
}
void light_client_get(unsigned short dst)
{
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_LIGHT_GET,0,NULL);
}
void light_client_mode_set(unsigned short dst,unsigned char id,unsigned short speed_ticks,unsigned short on_ticks,unsigned short off_ticks,unsigned short live_ticks,unsigned char direction)
{
    unsigned char buffer[10];
    buffer[0]=id;
    JMESH_BIG_ENDIAN_FILL2(speed_ticks,buffer+1);
    JMESH_BIG_ENDIAN_FILL2(on_ticks,buffer+3);
    JMESH_BIG_ENDIAN_FILL2(off_ticks,buffer+5);
    JMESH_BIG_ENDIAN_FILL2(live_ticks,buffer+7);
    buffer[9]=direction;

    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_LIGHT_MODE_SET,10,buffer);
}
void light_client_mode_get(unsigned short dst,unsigned char id)
{
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_LIGHT_MODE_GET,1,&id);
}

void light_client_mode(unsigned short dst,unsigned char id)
{
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_LIGHT_MODE,1,&id);
}
void light_client_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    ;
}
void light_client_mode_status_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned short speed_ticks,on_ticks,off_ticks,live_ticks;
    if(len==11){
        if(data[0]==0){
            JMESH_BIG_ENDIAN_PICK2(speed_ticks,data+2);
            JMESH_BIG_ENDIAN_PICK2(on_ticks,data+4);
            JMESH_BIG_ENDIAN_PICK2(off_ticks,data+6);
            JMESH_BIG_ENDIAN_PICK2(live_ticks,data+8);
            light_client_mode_callback(src,data[1],speed_ticks,on_ticks,off_ticks,live_ticks,data[10]);
        }
    }
}
void light_client_mode_callback(unsigned short src,unsigned char id,unsigned short speed_ticks,unsigned short on_ticks,unsigned short off_ticks,unsigned short live_ticks,unsigned char direction)
{
    ;
}

void light_client_bit_set(unsigned short dst,unsigned char length,unsigned char *value)
{
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_LIGHT_BIT,length,value);
}

void light_client_point_set(unsigned short dst,unsigned char length,unsigned char *value)
{
    jmesh_app_expand2_send(dst,JMESH_APP_EXPAND2_LIGHT_POINT,length,value);
}
