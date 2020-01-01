
#define JMESH_MESSAGE_DEFINE
#include"../app/app_include.h"
#include"../foundation/config/config_client.h"
#include"../foundation/config/config_server.h"
#include"../app/expand/app_expand2/test_app/test_app_client.h"
#include"../app/expand/app_expand2/test_app/test_app_server.h"
#include"../app/expand/app_expand2/meter/meter_client.h"
#include"../app/expand/app_expand2/meter/meter_server.h"
#undef JMESH_MESSAGE_DEFINE

#include"jmesh_message.h"

unsigned char jmesh_message_fill_opcode(unsigned char* message,unsigned char length,unsigned char* buffer){
    unsigned char i,opcode_size;

    opcode_size=JMESH_MESSAGE_OPCODE_SIZE(message[0]);
    for(i=0;i<opcode_size;i++){
        buffer[i]=message[i];
    }
    return opcode_size;
}
