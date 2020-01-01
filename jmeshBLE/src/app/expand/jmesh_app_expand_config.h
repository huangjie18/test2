#ifndef JMESH_APP_EXPAND_CONFIG_H
#define JMESH_APP_EXPAND_CONFIG_H

#define JMESH_APP_EXPAND1_DEVICE_CLIENT
#define JMESH_APP_EXPAND1_DEVICE_SERVER

#define JMESH_APP_EXPAND2_REALCOMM_CLIENT
#define JMESH_APP_EXPAND2_REALCOMM_SERVER

//#define JMESH_APP_EXPAND2_WATER_CLIENT
//#define JMESH_APP_EXPAND2_WATER_SERVER

//#define JMESH_APP_EXPAND2_WATER_GATE
typedef struct st_jmesh_device_role{
    union{
        unsigned char roles;
        struct{
            unsigned char water:1;
            unsigned char gate:1;
            unsigned char reserve:6;
        };
    };
}jmesh_device_role_t;

#endif
