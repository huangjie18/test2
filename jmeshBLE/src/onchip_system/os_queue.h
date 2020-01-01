#ifndef OS_QUEUE_H
#define OS_QUEUE_H
#include"os_core.h"
#include"FreeRTOS.h"
#include"semphr.h"
typedef struct st_os_queue{
    unsigned char queue_size;
    unsigned char item_size;
    unsigned char item_num;
    unsigned char head;
    unsigned char* zone;
}os_queue_t;

#define os_queue_item_align_size(item_size) (((item_size+RAM_ALIGN_OCTECTS-1)/RAM_ALIGN_OCTECTS)*RAM_ALIGN_OCTECTS)

#define OS_QUEUE_CREATE(name,item_size,queue_size)                            \
    static unsigned char queue_##name##_zone[os_queue_item_align_size(item_size)*queue_size];                          \
    static os_queue_t queue_##name={queue_size, item_size, 0, 0, queue_##name##_zone};     \
    os_queue_t* name=&queue_##name;


#define OS_QUEUE_NAME(name) extern os_queue_t* name;

typedef struct st_send_queue
{
    unsigned char queue_size;
    unsigned char item_size;
    unsigned char head;
    unsigned char tail;
    unsigned char *zone;
}send_queue_t;

#define SEND_QUEUE_CREATE(name,item_size,queue_size)                            \
    static unsigned char queue_##name##_zone[os_queue_item_align_size(item_size)*queue_size];                          \
    static send_queue_t queue_##name={queue_size, item_size, 0, 0, queue_##name##_zone};     \
    send_queue_t* name=&queue_##name;


#define SEND_QUEUE_NAME(name) extern send_queue_t* name;


//0=match
typedef int(*os_queue_query_t)(void* item1,void* query_param,unsigned int len);
void os_queue_init(os_queue_t* q);
void os_queue_push(os_queue_t* q,void* item);
void* os_queue_pop(os_queue_t* q);
int os_queue_is_in(os_queue_t* q,void* item);
int os_queue_is_empty(os_queue_t* q);
int os_queue_is_full(os_queue_t* q);
void* os_queue_query(os_queue_t* q,os_queue_query_t query,void* query_param);

void* os_queue_peek(os_queue_t* q);

void send_queue_init(send_queue_t* q);
int send_queue_is_full(send_queue_t* q);
int send_queue_is_empty(send_queue_t* q);
//before push must check queue is full or not
void send_queue_push(send_queue_t* q,void* item);
void* send_queue_peek(send_queue_t* q);
void send_queue_free(send_queue_t* q);
void *send_queue_pop(send_queue_t *q);

#ifdef JMESH_TEST
#ifdef JMESH_TEST_OS_QUEUE
void os_queue_test(void);
#endif // JMESH_TEST_OS_QUEUE
#endif // JMESH_TEST

#endif // QUEUE_H
