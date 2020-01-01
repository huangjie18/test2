#include"jmesh_routing_neighbor.h"
#include"jmesh_routing_disconnect_request.h"
#include"../../upper/control/jmesh_control.h"
#include"jmesh_route.h"
#include"../../jmesh/jmesh_addr.h"
#include"../../driver/jmesh_ble.h"
#include"../../jmesh/jmesh_print.h"
void jmesh_routing_disconnect_request(unsigned short dst)
{
    JMESH_LOGI("disconnect request","send to 0x%x\n",dst);
    jmesh_control_send(jmesh_get_primary_addr(),
                       dst,
                       0,
                       JMESH_CONTROL_ROUTING_DISCONNECT_REQUEST,
                       0,
                       0,
                       NULL);
}

void jmesh_routing_disconnect_request_recv(unsigned short src,unsigned short dst,unsigned char ttl,unsigned char interface)
{
    jmesh_routing_neighbor_t* neighbor;

    JMESH_LOGI("disconnect request","recv from 0x%x\n",dst);
    if(NULL!=(neighbor=jmesh_routing_neighbor_get_by_addr(src))){
        if(jmesh_routing_neighbor_is_multilinked(neighbor)){
            JMESH_LOGI("disconnect request","agree\n");
            //jmesh_ble_gatt_disconnect(neighbor->interface);
        }
        else{
            JMESH_LOGI("disconnect request","disagree\n");
        }
    }
    else{
        JMESH_LOGW(1,"disconnect request","0x %x no neighbor\n",src);
    }
}
