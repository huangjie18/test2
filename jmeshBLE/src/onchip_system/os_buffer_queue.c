
#include"os_buffer_queue.h"
#include"jmesh_print.h"

void os_buffer_queue_init(os_buffer_queue_t* q)
{
    q->head=q->tail=0;
    q->mutex_lock=xSemaphoreCreateMutex();
    xSemaphoreGive(q->mutex_lock);
}


int os_buffer_queue_push(os_buffer_queue_t* q,unsigned char length,unsigned char* data)
{
    int i;
    unsigned short size_left=0;
    xSemaphoreTake(q->mutex_lock,portMAX_DELAY);

    size_left=(q->head+q->buffer_size-q->tail-1)%q->buffer_size;

    if(size_left > length){
        q->buffer[q->tail++]=length;

        for(i=0;i<length&&q->tail<q->buffer_size;i++){
            q->buffer[q->tail++]=data[i];
        }

        if(q->tail==q->buffer_size){
            for(q->tail=0;i<length;i++){
                q->buffer[q->tail++]=data[i];
            }
        }
        xSemaphoreGive(q->mutex_lock);
        return 1;
    }
    xSemaphoreGive(q->mutex_lock);
    return 0;
}

#include"log.h"
int os_buffer_queue_pop(os_buffer_queue_t* q,unsigned char size,unsigned char* data)
{
    int length,i;
    xSemaphoreTake(q->mutex_lock,portMAX_DELAY);
    if(q->head!=q->tail){
        length=q->buffer[q->head];
        //q->buffer[q->head]=0;
        if(length>size || length==0
           || (((q->tail + q->buffer_size - q->head) % q->buffer_size) < (1+length))){
            JMESH_LOGI("pop err","l:%d,h:%d,t:%d\n",length,q->head,q->tail);
            LOG(LOG_LVL_INFO,"h:%d,t:%d,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\n",q->head,q->tail,q->buffer[q->head],q->buffer[q->head+1],q->buffer[q->head+2],
            q->buffer[q->head+3],q->buffer[q->head+4],q->buffer[q->head+5],q->buffer[q->head+6],q->buffer[q->head+7],q->buffer[q->head+8],q->buffer[q->head+9],
            q->buffer[q->head+10],q->buffer[q->head+11],q->buffer[q->head+12],q->buffer[q->head+13],q->buffer[q->head+14],q->buffer[q->head+15],q->buffer[q->head+16],
            q->buffer[q->head+17],q->buffer[q->head+18],q->buffer[q->head+19]);
            q->head=q->tail=0;
            xSemaphoreGive(q->mutex_lock);
            return 0;
        }
        else{
            q->head++;
        }
        for(i=0;i<length&&q->head<q->buffer_size;i++){
            data[i]=q->buffer[q->head++];
        }
        if(q->head==q->buffer_size){
            for(q->head=0;i<length;i++){
                data[i]=q->buffer[q->head++];
            }
        }
        xSemaphoreGive(q->mutex_lock);
        return length;
    }
    xSemaphoreGive(q->mutex_lock);
    return 0;
}



