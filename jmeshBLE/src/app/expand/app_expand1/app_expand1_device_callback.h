#ifndef JMESH_APP_EXPAND1_DEVICE_CALLBACK_H
#define JMESH_APP_EXPAND1_DEVICE_CALLBACK_H





int jmesh_app_expand1_device_get_version(unsigned char* version);
int jmesh_app_expand1_device_get_mac(unsigned char* mac);
int jmesh_app_expand1_device_get_type(void);
void jmesh_app_expand1_device_report_callback(unsigned short src,unsigned short id,unsigned char length,unsigned char* data);

#endif // JMESH_APP_EXPAND1_DEVICE_CALLBACK_H
