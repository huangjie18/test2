#ifndef JMESH_ROUTING_HEARTBEAT_H
#define JMESH_ROUTING_HEARTBEAT_H




typedef struct st_jmesh_routing_heartbeat{
    unsigned char addr[2];//Դ��ַ��������src addr
    unsigned char element_num;
    unsigned char init_ttl;//��ʼttl
}jmesh_routing_heartbeat_t;

//����һ��ttl=0 ��proxy����
void jmesh_routing_heartbeat(unsigned short addr,unsigned char element_num);

void jmesh_routing_heartbeat_recv(unsigned short src,unsigned short dst,unsigned char ttl,unsigned char interface,jmesh_routing_heartbeat_t* heartbeat);




#endif // JMESH_ROUTING_HEARTBEAT_H
