
#include"app_expand1_device_callback.h"
#include"app_expand1_device_client.h"
#include"app_expand1_device_server.h"

int jmesh_app_expand1_device_get_version(unsigned char* version)
{
    version[0]=(unsigned char)(JMESH_APP_EXPAND1_DEVICE_VERSION>>16);
    version[1]=(unsigned char)(JMESH_APP_EXPAND1_DEVICE_VERSION>>8);
    version[2]=(unsigned char)JMESH_APP_EXPAND1_DEVICE_VERSION;
    return 4;
}
int jmesh_app_expand1_device_get_mac(unsigned char* mac)
{
    return 6;
}
int jmesh_app_expand1_device_get_type(void)
{
    return JMESH_DEVICE_TYPE_ELECTRIC_METER;
}

void jmesh_app_expand1_device_report_callback(unsigned short src,unsigned short id,unsigned char length,unsigned char* data)
{
    ;
}
