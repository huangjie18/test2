#ifndef TEST_APP_CLIENT_H
#define TEST_APP_CLIENT_H
#include"test_app_server.h"
#include"../../access/jmesh_model.h"

#define MESSAGE_TEST_CLIENT_EXC_STATUS                      {0xc0,0x0b,0x01}
#define MESSAGE_TEST_CLIENT_READ_STATUS                     {0xc0,0x0b,0x02}
#define MESSAGE_TEDT_CLIENT_RESULT_STATUS                   {0xc0,0x0b,0x03}

JMESH_MESSAGE_CREATE(MESSAGE_TEST_CLIENT_EXC_STATUS,        message_test_client_exc_status);
JMESH_MESSAGE_CREATE(MESSAGE_TEST_CLIENT_READ_STATUS,       message_test_client_read_status);
JMESH_MESSAGE_CREATE(MESSAGE_TEDT_CLIENT_RESULT_STATUS,     message_test_client_result_status);


JMESH_MODEL_NAME(test_client);


void test_client_init(void);
void test_client_start(unsigned short addr,unsigned char op_type,unsigned short dst,unsigned short intv_s);
void test_client_stop(unsigned short addr,unsigned char op_type);
void test_client_get_result(unsigned short addr,unsigned char op_type);
void test_get_read_result(unsigned long *red_count,unsigned long *tot_count);
void test_read_result_init(void);
void test_client_result_init(unsigned short addr,unsigned char op_type);
#define TEST_CLIENT_COMPANY_ID JMESH_MODEL_COMPANY_ID
#define TEST_CLIENT_MODEL_ID   0x000b























#endif
