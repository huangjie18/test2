#ifndef JMESH_PROXY_H
#define JMESH_PROXY_H

#include"../../jmesh/jmesh_pdu.h"
#include"jmesh_gatt.h"
enum{
    JMESH_PROXY_SAR_COMPLETE=0x00,
    JMESH_PROXY_SAR_FIRST_SEGMENT=0x01,
    JMESH_PROXY_SAR_CONTINUE_SEGMENT=0x02,
    JMESH_PROXY_SAR_LAST_SEGMENT=0x03,
};
enum{
    JMESH_PROXY_TYPE_NETWORK=0,
    JMESH_PROXY_TYPE_BEACON=1,
    JMESH_PROXY_TYPE_CONFIG=2,
    JMESH_PROXY_TYPE_PROVISION=3,
    JMESH_PROXY_TYPE_ROUTING=4,
		JMESH_PROXY_TYPE_APPDATA=5,
};



void jmesh_proxy_send(unsigned char send_type,unsigned char id,unsigned char id_backup,unsigned char proxy_type,unsigned short length,unsigned char *data);
void jmesh_proxy_recv(unsigned char id,unsigned short instance,unsigned char length,unsigned char* data);


#endif // JMESH_PROXY_H
