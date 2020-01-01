#include"app_expand.h"
#include"stdio.h"
#include"../../access/jmesh_access_callback.h"

#include"../../onchip_system/os_list.h"
#include"../../access/jmesh_access.h"
static app_expand_message_t* expand1_message_list;
static app_expand_message_t* expand2_message_list;
static app_expand_message_t* expand3_message_list;

static void message_app_expand1_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
static void message_app_expand2_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
static void message_app_expand3_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);

JMESH_VENDOR_MODEL_CREATE_START(app_expand,APP_EXPAND_COMPANY_ID,APP_EXPAND_MODEL_ID,3)
    JMESH_MESSAGE_ADD(MESSAGE_APP_EXPAND1,message_app_expand1_handler)
    JMESH_MESSAGE_ADD(MESSAGE_APP_EXPAND2,message_app_expand2_handler)
    JMESH_MESSAGE_ADD(MESSAGE_APP_EXPAND3,message_app_expand3_handler)
JMESH_MODEL_CREATE_END(app_expand)

void app_expand_model_init(void)
{
    expand1_message_list=NULL;
    expand2_message_list=NULL;
    expand3_message_list=NULL;

    jmesh_element_add_model(&element_app,&app_expand);

	jmesh_app_expand1_init();
	jmesh_app_expand2_init();
}

int jmesh_app_expand1_message_register(app_expand_message_t* message,unsigned char opcode,app_expand_message_handler_t handler)
{
    if(message==NULL||handler==NULL){
        return -1;
    }
    message->opcode=opcode;
    message->handler=handler;
    os_list_add((os_list_t)&expand1_message_list,message);
    return 0;
}
int jmesh_app_expand2_message_register(app_expand_message_t* message,unsigned short opcode,app_expand_message_handler_t handler)
{
    if(message==NULL||handler==NULL){
        return -1;
    }
    message->opcode=opcode;
    message->handler=handler;
    os_list_add((os_list_t)&expand2_message_list,message);
    return 0;
}
int jmesh_app_expand3_message_register(app_expand_message_t* message,unsigned long opcode,app_expand_message_handler_t handler)
{
    if(message==NULL||handler==NULL){
        return -1;
    }
    message->opcode=opcode;
    message->handler=handler;
    os_list_add((os_list_t)&expand3_message_list,message);
    return 0;
}


static void message_app_expand1_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data)
{
    app_expand_message_t* temp;
    if(len<1){
        return;
    }
	for(temp = expand1_message_list; temp != NULL; temp = temp->next)
	{
		if(temp->opcode == data[0])
		{
			temp->handler(src,dst,len-1,data+1);
			return;
		}
	}
}
static void message_app_expand2_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data)
{
    app_expand_message_t* temp;
    unsigned short opcode;
    if(len<2){
        return;
    }
    JMESH_BIG_ENDIAN_PICK2(opcode,data);

	for(temp = expand2_message_list; temp != NULL; temp = temp->next)
	{
		if(temp->opcode == opcode)
		{
			temp->handler(src,dst,len-2,data+2);
			return;
		}
	}
}
static void message_app_expand3_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data)
{
    app_expand_message_t* temp;
    unsigned short opcode;
    if(len<3){
        return;
    }
    JMESH_BIG_ENDIAN_PICK3(opcode,data);

	for(temp = expand3_message_list; temp != NULL; temp = temp->next)
	{
		if(temp->opcode == opcode)
		{
			temp->handler(src,dst,len-3,data+3);
			return;
		}
	}
}

unsigned char app_expand2_list_register_opcode(unsigned char* buffer)
{
    app_expand_message_t* temp;
    int len=0;

	for(temp = expand2_message_list; temp != NULL; temp = temp->next)
	{
	    JMESH_BIG_ENDIAN_FILL2(temp->opcode,buffer+len);
	    len+=2;
	}
	return len;
}

void jmesh_app_expand1_send(unsigned short dst,unsigned char opcode,unsigned char length,unsigned char* data)
{
    unsigned char buffer[257];
    buffer[0]=opcode;
    memcpy(buffer+1,data,length);
    jmesh_access_send(dst,0,JMESH_APP_EXPAND_APPKEY_INDEX,&app_expand,message_app_expand1,length+1,buffer,JMESH_SEND_ALL_CHANNEL);
}
void jmesh_app_expand2_send(unsigned short dst,unsigned short opcode,unsigned char length,unsigned char* data)
{
    unsigned char buffer[258];
    JMESH_BIG_ENDIAN_FILL2(opcode,buffer);
    memcpy(buffer+2,data,length);
    jmesh_access_send(dst,0,JMESH_APP_EXPAND_APPKEY_INDEX,&app_expand,message_app_expand2,length+2,buffer,JMESH_SEND_ALL_CHANNEL);
}
void jmesh_app_expand3_send(unsigned short dst,unsigned long opcode,unsigned char length,unsigned char* data)
{
    unsigned char buffer[259];
    JMESH_BIG_ENDIAN_FILL3(opcode,buffer);
    memcpy(buffer+3,data,length);
    jmesh_access_send(dst,0,JMESH_APP_EXPAND_APPKEY_INDEX,&app_expand,message_app_expand3,length+3,buffer,JMESH_SEND_ALL_CHANNEL);
}


