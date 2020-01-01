#ifndef JMESH_LOWER_H
#define JMESH_LOWER_H
#include"../jmesh/jmesh_pdu.h"




int jmesh_lower_send(unsigned short netkey_index,jmesh_pdu_t* pdu);
int jmesh_lower_recv(unsigned short netkey_index,jmesh_pdu_t* pdu);

#endif // JMESH_LOWER_H
