#ifndef OS_BUFFER_QUEUE_H
#define OS_BUFFER_QUEUE_H
#include"FreeRTOS.h"
#include"semphr.h"

typedef volatile struct st_os_buffer_queue{
    unsigned char* buffer;
    unsigned short buffer_size;
    unsigned short head;
    unsigned short tail;
    SemaphoreHandle_t mutex_lock;
}os_buffer_queue_t;

#define OS_BUFFER_QUEUE_CREATE(name,buffer_size)                            \
    static unsigned char queue_##name##_zone[buffer_size];                          \
    static os_buffer_queue_t queue_##name={queue_##name##_zone, buffer_size, 0, 0};     \
    os_buffer_queue_t* name=&queue_##name;


#define OS_BUFFER_QUEUE_NAME(name) extern os_buffer_queue_t* name;

void os_buffer_queue_init(os_buffer_queue_t* q);
int os_buffer_queue_push(os_buffer_queue_t* q,unsigned char length,unsigned char* data);
int os_buffer_queue_pop(os_buffer_queue_t* q,unsigned char size,unsigned char* data);


#endif // OS_BUFFER_QUEUE_H
