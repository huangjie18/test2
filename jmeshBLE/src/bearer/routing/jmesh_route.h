#ifndef JMESH_ROUTE_H
#define JMESH_ROUTE_H

#include"../../jmesh/jmesh_pdu.h"

#define JMESH_ROUTE_MAX_DISTANCE 15
#define JMESH_ROUTE_REFRESH_LIFETIME(distance) (JMESH_ROUTE_MAX_DISTANCE - distance)
#define JMESH_ROUTE_EXCHANGE_MIN_LIFETIME(distance) (JMESH_ROUTE_MAX_DISTANCE - distance - 1)

#define JMESH_ROUTE_EXCHANGE_MIN_PERIOD_S 20
#define JMESH_ROUTE_EXCHANGE_MAX_PERIOD_S 40
#define JMESH_ROUTE_LIFETIME_UNIT_S JMESH_ROUTE_EXCHANGE_MIN_PERIOD_S

#define JMESH_ROUTE_NODE_SIZE 220
#define JMESH_ROUTE_NODE_ELEMENT_NUM 2
#define JMESH_ROUTE_NODE_RECORD_NUM 3
#define JMESH_ROUTE_TABLE_SIZE (JMESH_ROUTE_NODE_SIZE * JMESH_ROUTE_NODE_RECORD_NUM)/**< 1000*3 */
#define JMESH_ROUTE_NODE_ADDR_LIMIT (JMESH_ROUTE_NODE_ELEMENT_NUM*(JMESH_ROUTE_NODE_SIZE-1)+1)
#define JMESH_ROUTE_NODE_INDEX(addr) (JMESH_ROUTE_NODE_RECORD_NUM * ((addr-3) / JMESH_ROUTE_NODE_ELEMENT_NUM))


typedef struct st_jmesh_route{
    unsigned char lifetime;
    unsigned char distance;
    unsigned char id;
}jmesh_route_t;

extern jmesh_route_t* p_route_table;

void jmesh_route_init(void);
void jmesh_route_update(unsigned short src,unsigned char id,unsigned char distance);
void jmesh_route_exchange(void);
void jmesh_route_exchange_recv(unsigned char id,unsigned char* buffer);
void jmesh_route_clear_id(unsigned char id);
jmesh_route_t* jmesh_route_get(unsigned short addr,unsigned char ttl);
jmesh_route_t* jmesh_route_get_by_addr(unsigned short addr);
void jmesh_route_send(unsigned char send_type,unsigned short dst,unsigned char ttl,jmesh_pdu_t* pdu);
void jmesh_gatt_adv_send(unsigned short dst,unsigned char ttl,jmesh_pdu_t* pdu);

void jmesh_lowpower_node_route_update(unsigned short src,unsigned char id);
void jmesh_lowpower_node_route_delete(unsigned short src);

#endif // JMESH_ROUTE_H
