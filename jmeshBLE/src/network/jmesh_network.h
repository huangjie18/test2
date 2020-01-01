#ifndef JMESH_NETWORK_H
#define JMESH_NETWORK_H
#include"../jmesh/jmesh_types.h"
#include"../jmesh/jmesh_pdu.h"

void jmesh_network_init(void);
int jmesh_network_send(unsigned short netkey_index,jmesh_pdu_t* pdu);
int jmesh_network_recv(jmesh_pdu_t* pdu);
int network_cipher_cache(unsigned char* pdu);
#endif // JMESH_NETWORK_H
