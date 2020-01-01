#ifndef JMESH_BEACON_H
#define JMESH_BEACON_H

#include"jmesh_device_beacon.h"
#include"jmesh_secure_beacon.h"
#include"../routing/jmesh_routing_beacon.h"
#include"../../jmesh/jmesh_types.h"
#include"../../network/jmesh_netkey.h"
#include"../../secure/AES_CMAC/toolbox/security_kn.h"





#define JMESH_BEACON_HEAD_SIZE 3
#define JMESH_BEACON_SEND_FOREVER 0

typedef struct st_jmesh_beacon{
    unsigned char type;
    union{
        jmesh_secure_beacon_t secure_beacon;
        jmesh_device_beacon_t device_beacon;
        jmesh_routing_beacon_t routing_beacon;
    };
}jmesh_beacon_t;
void jmesh_beacon_handler(unsigned char *mac,unsigned char rssi,jmesh_beacon_t* beacon);


#endif // JMESH_BEACON_H
