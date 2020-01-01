#include"test_app_server.h"
#include"test_app_client.h"
#include"test_app_server_callback.h"
#include"test_app_client_callback.h"
#include"../../jmesh/jmesh_print.h"

//operation+destination+interval(s)+data
static unsigned short read_addr,write_addr;
static unsigned long read_seq=0,write_seq=0;
static os_timer_event_t read_timer;
static os_timer_event_t write_timer;
static unsigned long write_count=0;
static unsigned long total_count=0;
static void test_server_start_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
static void test_server_stop_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
static void test_server_write_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
static void test_server_read_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
static void test_server_result_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
static void test_server_result_init_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
JMESH_VENDOR_MODEL_CREATE_START(test_server,TEST_SERVER_COMPANY_ID,TEST_SERVER_MODEL_ID,6)
    JMESH_MESSAGE_ADD(MESSAGE_TEST_SERVER_READ,  test_server_read_handler)
    JMESH_MESSAGE_ADD(MESSAGE_TEST_SERVER_WRITE,  test_server_write_handler)
    JMESH_MESSAGE_ADD(MESSAGE_TEST_SERVER_START,  test_server_start_handler)
    JMESH_MESSAGE_ADD(MESSAGE_TEST_SERVER_STOP,  test_server_stop_handler)
    JMESH_MESSAGE_ADD(MESSAGE_TEST_SERVER_RESULT,  test_server_result_handler)
    JMESH_MESSAGE_ADD(MESSAGE_TEST_SERVER_RESULT_INIT,  test_server_result_init_handler)

JMESH_MODEL_CREATE_END(test_server)


void test_server_init(void){
    jmesh_element_add_model(&element_app,&test_server);
}

void test_read(void)
{
    unsigned char buffer[4];
    JMESH_BIG_ENDIAN_FILL4(read_seq,buffer);
    read_seq++;
    jmesh_access_send(read_addr,0,1,&test_server,message_test_server_read,4,buffer,JMESH_SEND_ALL_CHANNEL);
    os_timer_event_restart(&read_timer);
}

void test_write(void)
{
    unsigned char buffer[4];
    JMESH_BIG_ENDIAN_FILL4(write_seq,buffer);
    write_seq++;
    jmesh_access_send(write_addr,0,1,&test_server,message_test_server_write,4,buffer,JMESH_SEND_ALL_CHANNEL);
    os_timer_event_restart(&write_timer);
    JMESH_LOGI("wrt","dst:%d,seq:%d\n",write_addr,write_seq);
}

static void test_server_start_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data)
{
		unsigned short interval;
		unsigned char op_type;
    if(5==len){
        op_type=data[0];
        if(0==op_type){//read
            JMESH_BIG_ENDIAN_PICK2(read_addr, &data[1]);
            JMESH_BIG_ENDIAN_PICK2(interval, &data[3]);
            read_seq=0;
            os_timer_event_set(&read_timer,interval*1000,(os_timer_event_caller_t)test_read,NULL);
            jmesh_access_send(src, nid, aid, &test_client, message_test_client_exc_status, 7, "SUCCESS",JMESH_SEND_ALL_CHANNEL);
        }
        else if(1==op_type){//write
            JMESH_BIG_ENDIAN_PICK2(write_addr, &data[1]);
            JMESH_BIG_ENDIAN_PICK2(interval, &data[3]);
            write_seq=0;
            os_timer_event_set(&write_timer,interval*1000,(os_timer_event_caller_t)test_write,NULL);
            jmesh_access_send(src, nid, aid, &test_client, message_test_client_exc_status, 7, "SUCCESS",JMESH_SEND_ALL_CHANNEL);
        }
    }
}

static void test_server_read_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data)
{
    jmesh_access_send(src, nid, aid, &test_client, message_test_client_read_status, len, data,JMESH_SEND_ALL_CHANNEL);
}

static void test_server_write_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data)
{
    unsigned long seq;
    if(4==len){
        JMESH_BIG_ENDIAN_PICK4(seq,data);
        write_count++;
        total_count=seq+1;
        JMESH_LOGI("rev wrt","src:%d,seq:%d\n",src,total_count);
    }
}

static void test_server_stop_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data)
{
		unsigned char op_type;
    if(1==len){
        op_type=data[0];
        if(0==op_type){//read
            os_timer_event_remove(&read_timer);
            jmesh_access_send(src, nid, aid, &test_client, message_test_client_exc_status, 7, "SUCCESS",JMESH_SEND_ALL_CHANNEL);
        }
        else if(1==op_type){//write
            os_timer_event_remove(&write_timer);
            jmesh_access_send(src, nid, aid, &test_client, message_test_client_exc_status, 7, "SUCCESS",JMESH_SEND_ALL_CHANNEL);
        }
    }
}

void test_get_write_result(unsigned long *wri_count,unsigned long *tot_count)
{
    *wri_count=write_count;
    *tot_count=total_count;
    //write_count=total_count=0;
}

void test_write_result_init(void)
{
    write_count=total_count=0;
}

static void test_server_result_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data)
{
		unsigned char op_type;
		unsigned char buff[8];
		unsigned long suc_count,tot_count;
    if(1==len){
        op_type=data[0];
        if(0==op_type){//read
            test_get_read_result(&suc_count,&tot_count);

        }
        else if(1==op_type){//write
            test_get_write_result(&suc_count,&tot_count);
        }
        else{
            return;
        }
        //JMESH_LOGI("ser rcv","src:%d\n",src);
        JMESH_BIG_ENDIAN_FILL4(suc_count,&buff[0]);
        JMESH_BIG_ENDIAN_FILL4(tot_count,&buff[4]);
        jmesh_access_send(src, nid, aid, &test_client, message_test_client_result_status, 8, buff,JMESH_SEND_ALL_CHANNEL);
    }
    else{
        JMESH_LOGI("ser rcv","err:%d\n",len);
    }
}

static void test_server_result_init_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data)
{
		unsigned char op_type;
    if(1==len){
        op_type=data[0];
        if(0==op_type){//read
            test_read_result_init();
        }
        else if(1==op_type){//write
            test_write_result_init();
        }
        else{
            return;
        }
        jmesh_access_send(src, nid, aid, &test_client, message_test_client_exc_status, 7, "SUCCESS",JMESH_SEND_ALL_CHANNEL);
    }
}





