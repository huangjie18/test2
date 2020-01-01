#ifndef JMESH_APP_EXPAND1_DEVICE_CLIENT_H
#define JMESH_APP_EXPAND1_DEVICE_CLIENT_H
#include"../jmesh_app_expand_config.h"

#ifdef JMESH_APP_EXPAND1_DEVICE_CLIENT






void jmesh_app_expand1_device_client_init(void);
void jmesh_app_expand1_device_client_get_info(unsigned short dst);
void jmesh_app_expand1_device_client_bind_report(unsigned short dst,unsigned short report_addr,unsigned short report_id);
void jmesh_app_expand1_device_client_unbind_report(unsigned short dst,unsigned short report_addr,unsigned short report_id);

void jmesh_app_expand1_device_client_info_status(unsigned short src,unsigned short dst,unsigned short length,unsigned char* status);
void jmesh_app_expand1_device_client_report_status(unsigned short src,unsigned short dst,unsigned short length,unsigned char* status);


#endif

#endif // JMESH_APP_EXPAND1_DEVICE_CLIENT_H
