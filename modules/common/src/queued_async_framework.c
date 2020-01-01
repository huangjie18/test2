#include "queued_async_framework.h"
#include "log.h"

static queued_async_call_func_t async_call;

static uint16_t queued_data_size(queued_async_framework_t *inst)
{
    uint16_t size = bx_ring_queue_element_size(inst->rq);
    if(inst->common_callback == NULL)
    {
        size -= sizeof(queued_async_callback_t);
    }
    return size;
}

static void copy_to_queue(uint8_t *dest,uint16_t element_size,va_list *param_list)
{
    void *data = va_arg(*param_list,void *);
    queued_async_callback_t cb = va_arg(*param_list,queued_async_callback_t);
    uint16_t data_size;
    if(NULL == cb)
    {
        data_size = element_size;
    }else
    {
        data_size = element_size-sizeof(queued_async_callback_t);
        queued_async_callback_t *cb_buf = (queued_async_callback_t *)&dest[data_size];
        *cb_buf = cb;
    }
    memcpy(dest,data,data_size);
}

void *queued_async_process_get_current(queued_async_framework_t *inst)
{
    return bx_ring_queue_glance(inst->rq, 0);    
}

uint16_t queued_async_process_get_amount(queued_async_framework_t *inst)
{
    return bx_ring_queue_amount(inst->rq);
}

void queued_async_call_func_set(queued_async_call_func_t func)
{
    async_call = func;
}

bool queued_async_process_start(queued_async_framework_t *inst,void *data,queued_async_callback_t cb)
{
    BX_ASSERT(cb && inst->common_callback == NULL || cb==NULL && inst->common_callback); 
    bool empty = bx_ring_queue_empty(inst->rq);
    if(bx_ring_queue_full(inst->rq))
    {
        LOG(LOG_LVL_ERROR,"async process queue full,inst:%x\n",inst);
        return false;
    }
    bx_enqueue_specific(inst->rq,copy_to_queue,data,cb);
    if(empty)
    {
        async_call(inst->pre_process,queued_async_process_get_current(inst));
    }
    return true;    
}

void queued_async_process_end(queued_async_framework_t *inst,void *cb_dummy,uint8_t cb_status)
{
    void *old = bx_dequeue(inst->rq);
    bool empty = bx_ring_queue_empty(inst->rq);
    queued_async_callback_t callback;
    if(inst->common_callback)
    {
        callback = inst->common_callback;
    }else
    {
        uint8_t *buf = old;
        queued_async_callback_t *cb_buf = (queued_async_callback_t *)&buf[queued_data_size(inst)];
        callback = *cb_buf;
    }
    callback(old,cb_dummy,cb_status);
    if(!empty)
    {
        async_call(inst->pre_process,queued_async_process_get_current(inst));
    }
}
