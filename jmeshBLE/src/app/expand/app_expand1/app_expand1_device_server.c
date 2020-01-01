

#include"app_expand1_device_callback.h"
#include"app_expand1_device_client.h"
#include"app_expand1_device_server.h"
#include"app_expand1.h"

#ifdef JMESH_APP_EXPAND1_DEVICE_SERVER


void app_expand1_device_server_info_get_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void app_expand1_device_server_report_bind_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void app_expand1_device_server_report_unbind_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void app_expand1_device_server_report_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);

static jmesh_app_expand1_device_report_t report[JMESH_APP_EXPAND1_DEVICE_REPORT_SIZE];
void jmesh_app_expand1_device_server_init(void)
{

    static app_expand_message_t get;
    static app_expand_message_t bind;
    static app_expand_message_t unbind;
    static app_expand_message_t report_m;
    jmesh_app_expand1_message_register(&get,    JMESH_APP_EXPAND1_DEVICE_INFO_GET,      app_expand1_device_server_info_get_handler);
    jmesh_app_expand1_message_register(&bind,   JMESH_APP_EXPAND1_DEVICE_REPORT_BIND,   app_expand1_device_server_report_bind_handler);
    jmesh_app_expand1_message_register(&unbind, JMESH_APP_EXPAND1_DEVICE_REPORT_UNBIND, app_expand1_device_server_report_unbind_handler);
    jmesh_app_expand1_message_register(&report_m, JMESH_APP_EXPAND1_DEVICE_REPORT,        app_expand1_device_server_report_handler);
    memset(report,0,sizeof(report));
}

void app_expand1_device_server_info_get_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned char buffer[100],i,send_len=2;
    buffer[0]=jmesh_app_expand1_device_get_type();
    jmesh_app_expand1_device_get_version(buffer+1);
    jmesh_app_expand1_device_get_mac(buffer+5);
    buffer[12]=0;
    send_len=app_expand2_list_register_opcode(buffer+13);
    buffer[11]=send_len/2;
    send_len+=13;
    for(i=0;i<JMESH_APP_EXPAND1_DEVICE_REPORT_SIZE;i++){
        if(report[i].addr!=0){
            JMESH_BIG_ENDIAN_FILL2(report[i].addr,buffer+send_len);
            JMESH_BIG_ENDIAN_FILL2(report[i].id,buffer+send_len+2);
            send_len+=4;
            buffer[12]++;
        }
    }

    jmesh_app_expand1_send(dst,JMESH_APP_EXPAND1_DEVICE_INFO_STATUS,send_len,buffer);
}
void app_expand1_device_server_report_bind_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    int i;
    unsigned short id,addr,send_len;
    unsigned char buffer[JMESH_APP_EXPAND1_DEVICE_REPORT_SIZE*sizeof(jmesh_app_expand1_device_report_t)];
    JMESH_BIG_ENDIAN_PICK2(addr,data);
    JMESH_BIG_ENDIAN_PICK2(id,data+2);
    if(len!=4)
    {
        return;
    }
    for(i=0;i<JMESH_APP_EXPAND1_DEVICE_REPORT_SIZE;i++){
        if(report[i].addr==addr&&report[i].id==id){
            break;
        }
    }
    if(i==JMESH_APP_EXPAND1_DEVICE_REPORT_SIZE){
        for(i=0;i<JMESH_APP_EXPAND1_DEVICE_REPORT_SIZE;i++){
            if(report[i].addr==0){
                report[i].addr=addr;
                report[i].id=id;
                break;
            }
        }
    }
    send_len=0;
    for(i=0;i<JMESH_APP_EXPAND1_DEVICE_REPORT_SIZE;i++){
        if(report[i].addr!=0){
            memcpy(buffer+send_len,&report[i],sizeof(jmesh_app_expand1_device_report_t));
            send_len+=sizeof(jmesh_app_expand1_device_report_t);
        }
    }
    jmesh_app_expand1_send(src,JMESH_APP_EXPAND1_DEVICE_REPORT_STATUS,send_len,buffer);
}
void app_expand1_device_server_report_unbind_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    int i;
    unsigned short id,addr,send_len;
    unsigned char buffer[JMESH_APP_EXPAND1_DEVICE_REPORT_SIZE*sizeof(jmesh_app_expand1_device_report_t)+1];
    JMESH_BIG_ENDIAN_PICK2(addr,data);
    JMESH_BIG_ENDIAN_PICK2(id,data+2);
    if(len!=sizeof(jmesh_app_expand1_device_report_t))
    {
        return;
    }
    for(i=0;i<JMESH_APP_EXPAND1_DEVICE_REPORT_SIZE;i++){
        if(report[i].addr==addr&&report[i].id==id){
            report[i].addr=0;
            break;
        }
    }
    send_len=0;
    for(i=0;i<JMESH_APP_EXPAND1_DEVICE_REPORT_SIZE;i++){
        if(report[i].addr!=0){
            memcpy(buffer+send_len,&report[i],sizeof(jmesh_app_expand1_device_report_t));
            send_len+=sizeof(jmesh_app_expand1_device_report_t);
        }
    }
    jmesh_app_expand1_send(src,JMESH_APP_EXPAND1_DEVICE_REPORT_STATUS,send_len,buffer);
}

void app_expand1_device_server_report_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned short id;
    if(len>=2){
        JMESH_BIG_ENDIAN_PICK2(id,data);
        jmesh_app_expand1_device_report_callback(src,id,len-2,data+2);
    }
}
void jmesh_app_expand1_device_report(unsigned short report_id,unsigned short length,unsigned char* data)
{
    int i;
    unsigned char buffer[100];
    JMESH_BIG_ENDIAN_FILL2(report_id,buffer);
    memcpy(buffer+2,data,length);

    for(i=0;i<JMESH_APP_EXPAND1_DEVICE_REPORT_SIZE;i++){
        if(report[i].addr!=0 && report[i].id== report_id){
            jmesh_app_expand2_send(report[i].addr,JMESH_APP_EXPAND1_DEVICE_REPORT,length+2,data);
        }
    }
}


#endif // JMESH_APP_EXPAND1_DEVICE_SERVER
