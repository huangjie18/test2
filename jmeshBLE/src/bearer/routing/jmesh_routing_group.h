#ifndef JMESH_ROUTONG_GROUP_H
#define JMESH_ROUTING_GROUP_H

#include"../../jmesh/jmesh_pdu.h"

#define JMESH_ROUTING_GROUP_MEMBER_SIZE 72
#define JMESH_ROUTING_GROUP_MAX_DISTANCE 3
#define JMESH_ROUTING_GROUP_MAX_NSIZE 72

//·�ɽ���ֻ�����ڽ���
//·��Ѱַ��������Ѱַ��û���򴫵ݸ�����
//���ظ����������������ڵ�ת��

void jmesh_routing_group_init(void);
void jmesh_routing_group_vote_start(void);
void jmesh_routing_group_vote_listen(unsigned short src,unsigned short group_addr);
void jmesh_routing_group_leave(unsigned short group_addr);
void jmesh_routing_group_join(unsigned short group_addr);
void jmesh_routing_group_send(unsigned short dst,jmesh_pdu_t* pdu);
#endif // JMESH_ROUTONG_GROUP_H
