#ifndef JMESH_ROUTONG_GROUP_H
#define JMESH_ROUTING_GROUP_H

#include"../../jmesh/jmesh_pdu.h"

#define JMESH_ROUTING_GROUP_MEMBER_SIZE 72
#define JMESH_ROUTING_GROUP_MAX_DISTANCE 3
#define JMESH_ROUTING_GROUP_MAX_NSIZE 72

//路由交换只在组内进行
//路由寻址先在组内寻址，没有则传递给网关
//网关负责数据在两个组内的转发

void jmesh_routing_group_init(void);
void jmesh_routing_group_vote_start(void);
void jmesh_routing_group_vote_listen(unsigned short src,unsigned short group_addr);
void jmesh_routing_group_leave(unsigned short group_addr);
void jmesh_routing_group_join(unsigned short group_addr);
void jmesh_routing_group_send(unsigned short dst,jmesh_pdu_t* pdu);
#endif // JMESH_ROUTONG_GROUP_H
