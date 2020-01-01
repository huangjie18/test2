#ifndef METER_SERVER_H
#define METER_SERVER_H
#include"../../access/jmesh_model.h"

#define METER_SERVER_MODEL_ID  3
#define MESSAGE_OPCODE_METER_SERVER_CMD_SEND     {0xc0,0x02,0x00}

JMESH_MESSAGE_CREATE(MESSAGE_OPCODE_METER_SERVER_CMD_SEND, message_opcode_meter_server_cmd_send);

JMESH_MODEL_NAME(meter_server);

void meter_server_init(void);

void meter_server_res_send(unsigned short len,unsigned char* data);

#endif //


