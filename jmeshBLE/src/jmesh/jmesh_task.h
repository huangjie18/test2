#ifndef JMESH_TASK_H
#define JMESH_TASK_H

#include"../jmesh/jmesh_pdu.h"
#include"../onchip_system/os_core.h"

enum{
    JMESH_EVENT_ACCESS_SEND,
    JMESH_EVENT_ACCESS_RECV,

    JMESH_EVENT_CONFIG_SEND,
    JMESH_EVENT_CONFIG_RECV,

    JMESH_EVENT_INTERFACE_SEND,
    JMESH_EVENT_INTERFACE_RECV,

    JMESH_EVENT_PROVISION_SEND,
    JMESH_EVENT_PROVISION_RECV,

    JMESH_EVENT_CONTROL_SEND,
    JMESH_EVENT_CONTROL_RECV,
    JMESH_EVENT_BEACON_RECV,

    JMESH_EVENT_SECURE_BEACON_RECV,

    JMESH_EVENT_CONFIG_CMD_RECV,
		JMESH_EVENT_APP_DATA_RECV,
	
    JMESH_EVENT_SIZE,
};

int jmesh_interface_send_handler(jmesh_pdu_t* pdu);
int jmesh_interface_recv_handler(jmesh_pdu_t* pdu);

int jmesh_provision_send_handler(jmesh_pdu_t* pdu);
int jmesh_provision_recv_handler(jmesh_pdu_t* pdu);
int jmesh_upper_control_send_handler(jmesh_pdu_t *pdu);
int jmesh_upper_control_recv_handler(jmesh_pdu_t *pdu);

int jmesh_secure_beacon_recv_handler(jmesh_pdu_t *pdu);

int jmesh_config_cmd_recv_handler(jmesh_pdu_t *pdu);

OS_TASK_NAME(jmesh_task);
#endif // JMESH_TASK_H
