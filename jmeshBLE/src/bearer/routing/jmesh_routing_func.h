#ifndef JMESH_ROUTING_FUNC_H
#define JMESH_ROUTING_FUNC_H

#include"jmesh_routing_neighbor.h"
#include"../gatt/jmesh_gatt.h"




void jmesh_routing_recv(unsigned short src,unsigned short dst,unsigned char ttl,unsigned char opcode,unsigned char interface,unsigned char* pdu);


#endif // JMESH_ROUTING_FUNC_H
