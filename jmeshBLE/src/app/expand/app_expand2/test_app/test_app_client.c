#include"test_app_client.h"
#include"test_app_server.h"
#include"../../driver/jmesh_system.h"
#include"../../network/jmesh_seq.h"
#include"../../access/jmesh_access.h"
#include"../../access/jmesh_access_callback.h"
#include"../../onchip_system/os_timer_event.h"
#include"../../jmesh/jmesh_print.h"
#include "test_app_client_callback.h"
#include"../../foundation/config/configuration_cmd.h"

static unsigned long read_count=0;
static unsigned long total_count=0;
static void test_client_exc_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
static void test_client_read_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
static void test_client_result_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);

JMESH_VENDOR_MODEL_CREATE_START(test_client,TEST_CLIENT_COMPANY_ID,TEST_CLIENT_MODEL_ID,3)
    JMESH_MESSAGE_ADD(MESSAGE_TEST_CLIENT_EXC_STATUS,test_client_exc_status_handler)
    JMESH_MESSAGE_ADD(MESSAGE_TEST_CLIENT_READ_STATUS,test_client_read_status_handler)
    JMESH_MESSAGE_ADD(MESSAGE_TEDT_CLIENT_RESULT_STATUS,test_client_result_status_handler)

JMESH_MODEL_CREATE_END(test_client)


void test_client_init(void)
{
    jmesh_element_add_model(&element_app,&test_client);
}

void test_client_start(unsigned short addr,unsigned char op_type,unsigned short dst,unsigned short intv_s)
{
    unsigned char buff[5];
    buff[0]=op_type;
    JMESH_BIG_ENDIAN_FILL2(dst,&buff[1]);
    JMESH_BIG_ENDIAN_FILL2(intv_s,&buff[3]);
    jmesh_access_send(addr, 0, 1, &test_server, message_test_server_start, 5, buff, JMESH_SEND_ALL_CHANNEL);
}

void test_client_stop(unsigned short addr,unsigned char op_type)
{
    unsigned char buff[1];
    buff[0]=op_type;
    jmesh_access_send(addr, 0, 1, &test_server, message_test_server_stop, 1, buff, JMESH_SEND_ALL_CHANNEL);
}

void test_client_get_result(unsigned short addr,unsigned char op_type)
{
    unsigned char buff[1];
    buff[0]=op_type;
    jmesh_access_send(addr, 0, 1, &test_server, message_test_server_result, 1, buff, JMESH_SEND_ALL_CHANNEL);
}

void test_client_result_init(unsigned short addr,unsigned char op_type)
{
    unsigned char buff[1];
    buff[0]=op_type;
    jmesh_access_send(addr, 0, 1, &test_server, message_test_server_result_init, 1, buff, JMESH_SEND_ALL_CHANNEL);
}


static void test_client_read_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data)
{
    unsigned long seq;
    if(len==4){
        JMESH_BIG_ENDIAN_PICK4(seq,data);
        read_count++;
        total_count=seq+1;
    }
}

void test_get_read_result(unsigned long *red_count,unsigned long *tot_count)
{
    *red_count=read_count;
    *tot_count=total_count;
    //read_count=total_count=0;
}

void test_read_result_init(void)
{
    read_count=total_count=0;
}

static void test_client_exc_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data)
{
    if(0==memcmp("SUCCESS",data,len)){
        JMESH_CMD_RES(cur_cmd_interface,"success\n");
    }
}

static void test_client_result_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data)
{
    unsigned long suc_count,total_count;
    if(len==8){
        JMESH_BIG_ENDIAN_PICK4(suc_count,&data[0]);
        JMESH_BIG_ENDIAN_PICK4(total_count,&data[4]);
        JMESH_CMD_RES(cur_cmd_interface,"%d/%d\n",suc_count,total_count);
    }
    else{
        JMESH_LOGI("cli rcv","err:%d\n",len);
    }
}



