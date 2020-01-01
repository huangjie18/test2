
#include"jmesh_routing_find.h"
#include"jmesh_routing_neighbor.h"
#include"../../jmesh/jmesh_print.h"
#include"jmesh_routing_heartbeat.h"
#include"../../jmesh/jmesh_addr.h"
#include"../../jmesh/jmesh_types.h"
#include"jmesh_route.h"
#include"../../upper/control/jmesh_control.h"
#include"../../access/jmesh_access.h"
#include"../gatt/jmesh_gatt.h"


void jmesh_routing_find(unsigned short addr)
{
    jmesh_routing_find_proxy_t proxy;
    JMESH_BIG_ENDIAN_FILL2(addr,proxy.addr);
    proxy.init_ttl=JMESH_ROUTING_FIND_DEFAULT_TTL;
    jmesh_control_send(jmesh_get_primary_addr(),
                       JMESH_ADDR_ALL_ROUTING,
                       0,
                       JMESH_CONTROL_ROUTING_FIND_PROXY,
                       0,
                       sizeof(jmesh_routing_find_proxy_t),
                       (unsigned char*)&proxy);
}


void jmesh_routing_find_proxy_recv(unsigned short src,unsigned short dst,unsigned char ttl,unsigned char interface,jmesh_routing_find_proxy_t* proxy)
{
    jmesh_routing_find_t find;
    unsigned short primary_addr;
    primary_addr=jmesh_get_primary_addr();

    if(NULL==jmesh_route_get_by_addr(src)){//未知错误，能收到这个命令不应该没有它的路由信息
        return;
    }
//    if(routing->gate!=primary_addr){//未知错误，能收到ttl=0的命令，网关应该是自己
//        return;
//    }
    JMESH_BIG_ENDIAN_FILL2(src,find.addr);
    find.element_num=2;
    find.init_ttl=proxy->init_ttl;
    find.dest[0]=proxy->addr[0];
    find.dest[1]=proxy->addr[1];

    jmesh_control_send(primary_addr,
                   JMESH_ADDR_ALL_ROUTING,
                   0,
                   JMESH_CONTROL_ROUTING_FIND,
                   find.init_ttl,
                   sizeof(jmesh_routing_find_t),
                   (unsigned char*)&find);
}
void jmesh_routing_find_recv(unsigned short src,unsigned short dst,unsigned char ttl,unsigned char interface,jmesh_routing_find_t* find)
{
    unsigned short addr,find_dest;
    JMESH_BIG_ENDIAN_PICK2(addr,find->addr);
    JMESH_BIG_ENDIAN_PICK2(find_dest,find->dest);

    //jmesh_routing_add(addr,find->element_num,find->init_ttl-ttl+2,interface);
    jmesh_route_update(addr,interface,find->init_ttl-ttl+2);

    if(NULL!=jmesh_gatt_get_link_by_addr(find_dest)){
        jmesh_routing_heartbeat(find_dest,jmesh_get_element_num());
    }
}

