#ifndef JMESH_ROUTING_BEACON_H
#define JMESH_ROUTING_BEACON_H

#define JMESH_ROUTING_BEACON_MIC_SIZE 3
typedef struct st_jmesh_friend_poll{
    //unsigned char FSN:1;
    //unsigned char padding:7;
    unsigned char valid;
    unsigned char poll_fail_counter;
    unsigned char cache_not_empty;
}jmesh_friend_poll_t;

typedef struct st_jmesh_routing_beacon{
    unsigned char addr[2];
    //unsigned char subnet[2];
    //unsigned char element_num:4;
    //unsigned char net_distance:4;
    //unsigned char net_size;
    //unsigned char link_node[6][2];
    unsigned char link_require[2];
    unsigned char intv_mask[2];
    //unsigned char iv_update_flag;
    //unsigned char iv_index[4];
    unsigned char net_ticks[2];
    unsigned char features;
    jmesh_friend_poll_t poll;
    unsigned char auth_value[JMESH_ROUTING_BEACON_MIC_SIZE];
}jmesh_routing_beacon_t;

#define JMESH_ROUTING_BEACON_MAC_OFFSET ((int)(&((jmesh_routing_beacon_t*)0)->auth_value))


void jmesh_routing_beacon_start(void);
void jmesh_routing_beacon_stop(void);
int jmesh_routing_beacon_link_require(unsigned short dest,jmesh_friend_poll_t *poll);
void jmesh_routing_beacon_handler(unsigned char* mac,unsigned char rssi,jmesh_routing_beacon_t* beacon);


#endif
