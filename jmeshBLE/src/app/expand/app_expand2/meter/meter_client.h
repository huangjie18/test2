#ifndef METER_CLIENT_H
#define METER_CLIENT_H
#include"../../access/jmesh_model.h"

#define  METER_CLIENT_MODEL_ID   0X04

#define MESSAGE_OPCODE_METER_CLIENT_CMD_RESPONSE          {0xc0,0x03,0x00}

JMESH_MESSAGE_CREATE(MESSAGE_OPCODE_METER_CLIENT_CMD_RESPONSE,      message_opcode_meter_client_cmd_res);

JMESH_MODEL_NAME(meter_client);

void meter_client_init(void);
void meter_client_cmd_send(unsigned short dst,unsigned short netkey_index,unsigned short appkey_index,unsigned short len,unsigned char* data);

#endif // LIGHT_CLIENT_H
