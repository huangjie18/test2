#ifndef JMESH_ROUTING_FIND_H
#define JMESH_ROUTING_FIND_H


typedef struct st_jmesh_routing_find_proxy{
    unsigned char addr[2];
    unsigned char init_ttl;
}jmesh_routing_find_proxy_t;

typedef struct st_jmesh_routing_find{//dst=JMESH_ADDR_GROUP_ROUTING,ans heartbeat
    unsigned char addr[2];//Դ��ַ
    unsigned char element_num:4;//Դ��ַԪ�ظ���
    unsigned char init_ttl:4;//���ط�������ĳ�ʼttl�����սڵ㵽Դ��ַ����Ϊ init_ttl - ttl + 2

    unsigned char dest[2];//Ҫ���ҵ�Ŀ�ĵ�ַ�����ҳɹ���Ŀ�Ľڵ�Ӧ��һ����Դ��ַΪĿ�ĵ�ַ��������
}jmesh_routing_find_t;


void jmesh_routing_find(unsigned short addr);
void jmesh_routing_find_proxy_recv(unsigned short src,unsigned short dst,unsigned char ttl,unsigned char interface,jmesh_routing_find_proxy_t* pdu);
void jmesh_routing_find_recv(unsigned short src,unsigned short dst,unsigned char ttl,unsigned char interface,jmesh_routing_find_t* pdu);

#endif
