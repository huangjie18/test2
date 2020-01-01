#ifndef JMESH_APP_EXPAND1_DEVICE_SERVER_H
#define JMESH_APP_EXPAND1_DEVICE_SERVER_H

#include"../jmesh_app_expand_config.h"

#ifdef JMESH_APP_EXPAND1_DEVICE_SERVER


enum{
    JMESH_DEVICE_TYPE_ELECTRIC_METER=0,
    JMESH_DEVICE_TYPE_WATER_METER=10,
    JMESH_DEVICE_TYPE_GAS_METER=10,
    JMESH_DEVICE_TYPE_SMART_SOCKET=20,
    JMESH_DEVICE_TYPE_SMART_CONTROLLER=30,
    JMESH_DEVICE_TYPE_SENSOR=40,
};
#define JMESH_APP_EXPAND1_DEVICE_TYPE JMESH_DEVICE_TYPE_ELECTRIC_METER
#define JMESH_APP_EXPAND1_DEVICE_VERSION 0x020301

#define JMESH_APP_EXPAND1_DEVICE_REPORT_SIZE 3

typedef enum{
    JMESH_APP_EXPAND2_DEVICE_REPORT_LIGHT_=10,
    JMESH_APP_EXPAND2_DEVICE_REPORT_GAS_=20,
    JMESH_APP_EXPAND2_DEVICE_REPORT_METER_=30,
    JMESH_APP_EXPAND2_DEVICE_REPORT_WATER_=40,
    JMESH_APP_EXPAND2_DEVICE_REPORT_AIR_=50,
    JMESH_APP_EXPAND2_DEVICE_REPORT_BUTTON_ON=60,
    JMESH_APP_EXPAND2_DEVICE_REPORT_BUTTON_OFF=61,

    JMESH_APP_EXPAND2_DEVICE_REPORT_USER_=70,
}JMESH_APP_EXPAND2_REPORT_ID_T;

typedef struct st_jmesh_app_expand1_device_report{
    unsigned short addr;
    unsigned short id;
}jmesh_app_expand1_device_report_t;




typedef struct st_jmesh_app_expand_device_info{
    unsigned char device_type;
    unsigned char soft_version[3];
    unsigned char mac[6];
    unsigned char server_nums;
    unsigned char report_nums;
    unsigned short expand2_server[1];
    jmesh_app_expand1_device_report_t report[1];
}jmesh_app_expand_device_info_t;

void jmesh_app_expand1_device_server_init(void);

void jmesh_app_expand1_device_report(unsigned short report_id,unsigned short length,unsigned char* data);

#endif


#endif // JMESH_APP_EXPAND1_DEVICE_SERVER_H
