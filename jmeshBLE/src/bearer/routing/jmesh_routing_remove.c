#include"../../jmesh/jmesh_print.h"

#include"jmesh_routing_remove.h"
#include"jmesh_routing_heartbeat.h"
#include"../../jmesh/jmesh_addr.h"
#include"../../jmesh/jmesh_types.h"
#include"jmesh_route.h"
#include"../../upper/control/jmesh_control.h"
#include"../../access/jmesh_access.h"

void jmesh_routing_remove(unsigned short addr,unsigned short gate)
{
    jmesh_routing_remove_t remove;

    JMESH_BIG_ENDIAN_FILL2(addr,remove.addr);
    JMESH_BIG_ENDIAN_FILL2(gate,remove.gate);

    jmesh_control_send(jmesh_get_primary_addr(),
                       JMESH_ADDR_ALL_ROUTING,
                       0,
                       JMESH_CONTROL_ROUTING_REMOVE,
                       JMESH_ROUTING_HEARTBEAT_DEFAULT_TTL,
                       sizeof(jmesh_routing_remove_t),
                       (unsigned char*)&remove);
}


void jmesh_routing_remove_recv(unsigned short src,unsigned short dst,unsigned char ttl,unsigned char interface,jmesh_routing_remove_t* remove)
{
    unsigned short addr,gate;

    JMESH_BIG_ENDIAN_PICK2(addr,remove->addr);
    JMESH_BIG_ENDIAN_PICK2(gate,remove->gate);

    //jmesh_routing_del(gate,addr);
}
