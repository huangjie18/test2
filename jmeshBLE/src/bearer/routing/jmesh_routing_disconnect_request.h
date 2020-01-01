#ifndef JMESH_ROUTING_DISCONNECT_REQUEST_H
#define JMESH_ROUTING_DISCONNECT_REQUEST_H



void jmesh_routing_disconnect_request(unsigned short dst);

void jmesh_routing_disconnect_request_recv(unsigned short src,unsigned short dst,unsigned char ttl,unsigned char interface);






#endif // JMESH_ROUTING_DISCONNECT_REQUEST_H
