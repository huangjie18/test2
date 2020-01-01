#ifndef JMESH_UPPER_H
#define JMESH_UPPER_H
#include"../jmesh/jmesh_types.h"
#include"../jmesh/jmesh_pdu.h"
#include"../foundation/state/jmesh_appkey.h"
#include"../lower/jmesh_lower.h"
#include"../lower/jmesh_segment.h"



int jmesh_upper_recv(unsigned short netkey_index,unsigned short length,jmesh_pdu_t* pdu);

int jmesh_upper_access_send(jmesh_pdu_t* pdu);
int jmesh_upper_config_send(jmesh_pdu_t* pdu);
int jmesh_upper_control_send(unsigned short src,unsigned short dst,unsigned short netkey_index,unsigned short length,unsigned char* data);




//void jmesh_upper_send(jmesh_appkey_t* appkey,jmesh_model_t* model,jmesh_message_t* message,unsigned short length,unsigned char* data);



#endif // JMESH_UPPER_H
