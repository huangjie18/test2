#include"jmesh_routing_heartbeat.h"
#include"../../jmesh/jmesh_addr.h"
#include"../../jmesh/jmesh_types.h"
#include"jmesh_route.h"
#include"../../upper/control/jmesh_control.h"
#include"../../jmesh/jmesh_print.h"
#include"jmesh_routing_neighbor.h"

void jmesh_routing_heartbeat(unsigned short addr,unsigned char element_num)
{
    jmesh_routing_heartbeat_t heartbeat;
    
    JMESH_BIG_ENDIAN_FILL2(addr,heartbeat.addr);
    heartbeat.element_num=element_num;
    heartbeat.init_ttl=JMESH_ROUTING_HEARTBEAT_DEFAULT_TTL;
    
    jmesh_control_send(jmesh_get_primary_addr(),
                       JMESH_ADDR_ALL_ROUTING,
                       0,
                       JMESH_CONTROL_ROUTING_HEARTBEAT,
                       JMESH_ROUTING_HEARTBEAT_DEFAULT_TTL,
                       sizeof(jmesh_routing_heartbeat_t),
                       (unsigned char*)&heartbeat);
}

void jmesh_routing_heartbeat_recv(unsigned short src,unsigned short dst,unsigned char ttl,unsigned char interface,jmesh_routing_heartbeat_t* heartbeat)
{
    unsigned short addr;
    JMESH_BIG_ENDIAN_PICK2(addr,heartbeat->addr);
    if(interface == JMESH_INTERFACE_INVALID) return;
//    jmesh_routing_add(addr,//
//                      heartbeat->element_num,//
//                      heartbeat->init_ttl - ttl + 1,
//                      interface);//
    jmesh_route_update(addr,interface,heartbeat->init_ttl-ttl+1);
}
