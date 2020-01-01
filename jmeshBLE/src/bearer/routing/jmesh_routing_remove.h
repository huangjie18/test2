#ifndef JMESH_ROUTING_REMOVE_H
#define JMESH_ROUTING_REMOVE_H




typedef struct st_jmesh_routing_remove{
    unsigned char addr[2];
    unsigned char gate[2];
}jmesh_routing_remove_t;

void jmesh_routing_remove(unsigned short addr,unsigned short gate);
void jmesh_routing_remove_recv(unsigned short src,unsigned short dst,unsigned char ttl,unsigned char interface,jmesh_routing_remove_t* remove);




#endif // JMESH_ROUTING_REMOVE_H
