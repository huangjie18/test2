
#include"app_expand1_device_callback.h"
#include"app_expand1_device_client.h"
#include"app_expand1_device_server.h"
#include"app_expand1.h"


#ifdef JMESH_APP_EXPAND1_DEVICE_CLIENT



void jmesh_app_expand1_device_client_init(void);
void jmesh_app_expand1_device_client_get_info(unsigned short dst);
void jmesh_app_expand1_device_client_bind_report(unsigned short dst,unsigned short report_addr,unsigned short report_id);
void jmesh_app_expand1_device_client_unbind_report(unsigned short dst,unsigned short report_addr,unsigned short report_id);

void jmesh_app_expand1_device_client_info_status(unsigned short src,unsigned short dst,unsigned short length,unsigned char* status);
void jmesh_app_expand1_device_client_report_status(unsigned short src,unsigned short dst,unsigned short length,unsigned char* status);


void jmesh_app_expand1_device_client_init(void)
{
    static app_expand_message_t info_status;
    static app_expand_message_t report_status;
    jmesh_app_expand1_message_register(&info_status,JMESH_APP_EXPAND1_DEVICE_INFO_STATUS,jmesh_app_expand1_device_client_info_status);
    jmesh_app_expand1_message_register(&report_status,JMESH_APP_EXPAND1_DEVICE_REPORT_STATUS,jmesh_app_expand1_device_client_report_status);
}
void jmesh_app_expand1_device_client_get_info(unsigned short dst)
{
    jmesh_app_expand1_send(dst,JMESH_APP_EXPAND1_DEVICE_INFO_GET,0,NULL);
}
void jmesh_app_expand1_device_client_bind_report(unsigned short dst,unsigned short report_addr,unsigned short report_id)
{
    unsigned char buffer[4];
    JMESH_BIG_ENDIAN_FILL2(report_addr,buffer);
    JMESH_BIG_ENDIAN_FILL2(report_id,buffer+2);
    jmesh_app_expand1_send(dst,JMESH_APP_EXPAND1_DEVICE_REPORT_BIND,4,buffer);
}
void jmesh_app_expand1_device_client_unbind_report(unsigned short dst,unsigned short report_addr,unsigned short report_id)
{
    unsigned char buffer[4];
    JMESH_BIG_ENDIAN_FILL2(report_addr,buffer);
    JMESH_BIG_ENDIAN_FILL2(report_id,buffer+2);
    jmesh_app_expand1_send(dst,JMESH_APP_EXPAND1_DEVICE_REPORT_UNBIND,4,buffer);
}




void jmesh_app_expand1_device_client_info_status(unsigned short src,unsigned short dst,unsigned short length,unsigned char* status)
{
    JMESH_LOGI_BUFFER("expand1 device info status",length,status,"type=%d,version=0x%x%x%x,server nums=%d report_nums=%d,data:\n",status[0],status[1],status[2],status[3],status[10],status[11]);
}
void jmesh_app_expand1_device_client_report_status(unsigned short src,unsigned short dst,unsigned short length,unsigned char* status)
{
   JMESH_LOGI_BUFFER("expand1 device report status",length,status,"data:");

}




#endif // JMESH_APP_EXPAND1_DEVICE_CLIENT
