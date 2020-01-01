#ifndef APP_EXPAND_H
#define APP_EXPAND_H
#include"jmesh_app_expand_config.h"
#include"../../access/jmesh_model.h"
#include"app_expand1/app_expand1.h"
#include"app_expand2/app_expand2.h"

#include"../../jmesh/jmesh_print.h"

#define MESSAGE_APP_EXPAND1       {0xc1,0xbc,0xbd}
#define MESSAGE_APP_EXPAND2		  {0xc2,0xbc,0xbd}
#define MESSAGE_APP_EXPAND3		  {0xc3,0xbc,0xbd}

JMESH_MESSAGE_CREATE(MESSAGE_APP_EXPAND1,message_app_expand1);
JMESH_MESSAGE_CREATE(MESSAGE_APP_EXPAND2,message_app_expand2);
JMESH_MESSAGE_CREATE(MESSAGE_APP_EXPAND3,message_app_expand3);

JMESH_MODEL_NAME(app_expand);

#define APP_EXPAND_COMPANY_ID  JMESH_MODEL_COMPANY_ID
#define APP_EXPAND_MODEL_ID   0x0001

#define JMESH_APP_EXPAND_APPKEY_INDEX 1
void app_expand_model_init(void);
typedef void (*app_expand_message_handler_t)(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
typedef struct app_expand_message_st{
    struct app_expand_message_st* next;
    unsigned long opcode;
    app_expand_message_handler_t handler;
}app_expand_message_t;

int app_expand_message_register(app_expand_message_t* message);
int app_expand_message_unregister(app_expand_message_t* message);
int jmesh_app_expand1_message_register(app_expand_message_t* message,unsigned char opcode,app_expand_message_handler_t handler);
int jmesh_app_expand2_message_register(app_expand_message_t* message,unsigned short opcode,app_expand_message_handler_t handler);
int jmesh_app_expand3_message_register(app_expand_message_t* message,unsigned long opcode,app_expand_message_handler_t handler);
unsigned char app_expand2_list_register_opcode(unsigned char* buffer);

void jmesh_app_expand1_send(unsigned short dst,unsigned char opcode,unsigned char length,unsigned char* data);
void jmesh_app_expand2_send(unsigned short dst,unsigned short opcode,unsigned char length,unsigned char* data);
void jmesh_app_expand3_send(unsigned short dst,unsigned long opcode,unsigned char length,unsigned char* data);
#endif



