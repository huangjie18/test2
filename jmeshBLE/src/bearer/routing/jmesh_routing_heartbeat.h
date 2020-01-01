#ifndef JMESH_ROUTING_HEARTBEAT_H
#define JMESH_ROUTING_HEARTBEAT_H




typedef struct st_jmesh_routing_heartbeat{
    unsigned char addr[2];//源地址，网关是src addr
    unsigned char element_num;
    unsigned char init_ttl;//初始ttl
}jmesh_routing_heartbeat_t;

//发送一个ttl=0 的proxy请求
void jmesh_routing_heartbeat(unsigned short addr,unsigned char element_num);

void jmesh_routing_heartbeat_recv(unsigned short src,unsigned short dst,unsigned char ttl,unsigned char interface,jmesh_routing_heartbeat_t* heartbeat);




#endif // JMESH_ROUTING_HEARTBEAT_H
