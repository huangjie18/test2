#ifndef JMESH_ROUTING_NEIGHBOR_H
#define JMESH_ROUTING_NEIGHBOR_H

#include"jmesh_routing_beacon.h"
#include"../../jmesh/jmesh_pdu.h"

#define JMESH_ROUTING_CONNECT_MAX_NUM 6

typedef struct st_jmesh_routing_neighbor{

    unsigned short addr;
//    unsigned char server_flag:1;
    unsigned char mac[6];
//    unsigned short subnet;
//    unsigned char element_num:4;
//    unsigned char net_distance:4;
//    unsigned char net_size;
    unsigned char rssi;
//    unsigned char interface;
    unsigned char try_con_count;

    //unsigned short link_node[JMESH_ROUTING_CONNECT_MAX_NUM];
    unsigned short lifetime;
    unsigned short intv_mask;
    unsigned char features;
}jmesh_routing_neighbor_t;

extern jmesh_routing_neighbor_t* routing_neighbor_table_p;

void jmesh_routing_neighbor_init(void);
int jmesh_routing_neighbor_add(unsigned short addr,unsigned char* mac,unsigned char rssi,unsigned char features);
void jmesh_routing_neig_lifetime_refresh(unsigned short addr);
jmesh_routing_neighbor_t* jmesh_routing_neighbor_update(unsigned char* mac,unsigned char rssi,jmesh_routing_beacon_t* beacon);
jmesh_routing_neighbor_t* jmesh_routing_neighbor_get_by_addr(unsigned short addr);
jmesh_routing_neighbor_t* jmesh_routing_neighbor_get_by_mac(unsigned char* mac);
int jmesh_routing_get_neighbor_link_num(jmesh_routing_neighbor_t *neighbor);

void jmesh_routing_neighbor_flush(jmesh_routing_neighbor_t* neighbor);

int jmesh_routing_neighbor_is_multilinked(jmesh_routing_neighbor_t* neighbor);

void jmesh_routing_connected(unsigned char* mac,unsigned char interface);
void jmesh_routing_disconnected(unsigned char interface);
//unsigned short jmesh_routing_get_local_gate(void);
void jmesh_routing_fill_beacon(jmesh_routing_beacon_t* beacon);
void jmesh_routing_link_require(jmesh_routing_neighbor_t* neighbor);

void jmesh_routing_neighbor_broadcast_all(jmesh_pdu_t* pdu);
void jmesh_routing_neighbor_broadcast_subnet(jmesh_pdu_t* pdu);

#endif
