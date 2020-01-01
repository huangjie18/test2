#ifndef JMESH_ADV_H
#define JMESH_ADV_H
#include"../../jmesh/jmesh_pdu.h"
#include"../../jmesh/jmesh_config.h"
#include"../../onchip_system/os_timer_event.h"
#include"../../onchip_system/os_queue.h"

void jmesh_rev_adv_timer_start(void);
int jmesh_rev_beacon_timer_is_flow(void);
void jmesh_rev_beacon_timer_start(void);
void jmesh_adv_recv(unsigned char* mac,unsigned char rssi,unsigned char len,unsigned char ad_type,unsigned char* data);
void jmesh_adv_send_beacon(jmesh_pdu_t* pdu,unsigned char repeats);
void jmesh_adv_send_message(jmesh_pdu_t* pdu);
void jmesh_adv_send_message2(unsigned char length,unsigned char *data);
void jmesh_adv_send_provision(jmesh_pdu_t* pdu);


#endif // JMESH_ADV_H
