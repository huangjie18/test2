#ifndef JMESH_APP_EXPAND2_REALCOMM_CLIENT_H
#define JMESH_APP_EXPAND2_REALCOMM_CLIENT_H


#include"../jmesh_app_expand_config.h"

#ifdef JMESH_APP_EXPAND2_REALCOMM_CLIENT


typedef struct st_jmesh_app_expand2_realcomm_baudrate_status{
    unsigned char status;
    unsigned char port;
    unsigned char baud;
    unsigned char parity;
}jmesh_app_expand2_realcomm_baudrate_status_t;

void jmesh_app_expand2_realcomm_client_init(void);
void jmesh_app_expand2_realcomm_client_set_baudrate(unsigned short dst,unsigned char port,unsigned char baud,unsigned char parity);
void realcomm_client_addr_set(unsigned short dst,unsigned short addr,unsigned short nid,unsigned short aid);
void jmesh_app_expand2_transparent_client_trans(unsigned short dst,unsigned char port,unsigned char len,unsigned char* data);
void jmesh_app_expand2_realcomm_client_trans(unsigned short dst,unsigned char port,unsigned char len,unsigned char* data);


#endif

#endif // JMESH_APP_EXPAND2_REALCOMM_CLIENT_H
