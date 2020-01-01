#ifndef JMESH_APP_EXPAND2_REALCOMM_SERVER_H
#define JMESH_APP_EXPAND2_REALCOMM_SERVER_H
#include"../../../access/jmesh_model.h"

#include"../jmesh_app_expand_config.h"

#ifdef JMESH_APP_EXPAND2_REALCOMM_SERVER

typedef struct st_jmesh_app_expand2_realcomm_server_baudrate{
    unsigned char port;
    unsigned char baud;
    unsigned char parity;
}jmesh_app_expand2_realcomm_server_baudrate_t;

typedef struct st_realcomm_report{
    unsigned char id;
    unsigned short addr;
}jmesh_realcomm_report_t;

void jmesh_app_expand2_realcomm_server_init(void);
void jmesh_app_expand2_realcomm_client_return(unsigned char length,unsigned char* data);
void jmesh_app_expand2_realcomm_client_report(unsigned char length,unsigned char* data);
#endif
#endif
