#ifndef JMESH_ROUTING_LINK_H
#define JMESH_ROUTING_LINK_H


typedef struct st_jmesh_routing_link{
    jmesh_neighbor_t* neighbor;
    jmesh_gatt_t* gatt;
    unsigned short seq;
}jmesh_routing_link_t;


void jmesh_routing_link_init(void);
void jmesh_routing_link_request();
void jmesh_routing_linked();
void jmesh_routing_link();




#endif // JMESH_ROUTING_LINK_H
