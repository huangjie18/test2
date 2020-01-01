#ifndef JMESH_ROUTING_FIND_H
#define JMESH_ROUTING_FIND_H


typedef struct st_jmesh_routing_find_proxy{
    unsigned char addr[2];
    unsigned char init_ttl;
}jmesh_routing_find_proxy_t;

typedef struct st_jmesh_routing_find{//dst=JMESH_ADDR_GROUP_ROUTING,ans heartbeat
    unsigned char addr[2];//源地址
    unsigned char element_num:4;//源地址元素个数
    unsigned char init_ttl:4;//网关发送命令的初始ttl，接收节点到源地址距离为 init_ttl - ttl + 2

    unsigned char dest[2];//要查找的目的地址，查找成功后，目的节点应答一个以源地址为目的地址的心跳包
}jmesh_routing_find_t;


void jmesh_routing_find(unsigned short addr);
void jmesh_routing_find_proxy_recv(unsigned short src,unsigned short dst,unsigned char ttl,unsigned char interface,jmesh_routing_find_proxy_t* pdu);
void jmesh_routing_find_recv(unsigned short src,unsigned short dst,unsigned char ttl,unsigned char interface,jmesh_routing_find_t* pdu);

#endif
