#ifndef TEST_APP_SERVER_H
#define TEST_APP_SERVER_H
#include"../../access/jmesh_model.h"
#include"../../access/jmesh_access.h"


#define MESSAGE_TEST_SERVER_START               {0xc0,0x0a,0x01}
#define MESSAGE_TEST_SERVER_STOP                {0xc0,0x0a,0x02}
#define MESSAGE_TEST_SERVER_READ                {0xc0,0x0a,0x03}
#define MESSAGE_TEST_SERVER_WRITE               {0xc0,0x0a,0x04}
#define MESSAGE_TEST_SERVER_RESULT              {0xc0,0x0a,0x05}
#define MESSAGE_TEST_SERVER_RESULT_INIT         {0xc0,0x0a,0x06}

JMESH_MESSAGE_CREATE(MESSAGE_TEST_SERVER_START,   message_test_server_start);
JMESH_MESSAGE_CREATE(MESSAGE_TEST_SERVER_STOP,   message_test_server_stop);
JMESH_MESSAGE_CREATE(MESSAGE_TEST_SERVER_READ,   message_test_server_read);
JMESH_MESSAGE_CREATE(MESSAGE_TEST_SERVER_WRITE,   message_test_server_write);
JMESH_MESSAGE_CREATE(MESSAGE_TEST_SERVER_RESULT,   message_test_server_result);
JMESH_MESSAGE_CREATE(MESSAGE_TEST_SERVER_RESULT_INIT, message_test_server_result_init);
JMESH_MODEL_NAME(test_server);
void test_server_init(void);
void test_write_result_init(void);





#define TEST_SERVER_COMPANY_ID JMESH_MODEL_COMPANY_ID
#define TEST_SERVER_MODEL_ID   0x000a
#endif
