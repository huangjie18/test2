#include"os_core.h"
#include"jmesh_system.h"
#include"FreeRTOS.h"
#include"semphr.h"
os_task_t *os_task_list=NULL;
static os_event_data_t os_event[OS_EVENT_SIZE];
static os_event_t os_event_head=0,os_event_tail=0;
static volatile unsigned char os_poll_flag=0;
static SemaphoreHandle_t os_event_lock=NULL;


void os_init(void)
{
    os_task_list=NULL;
    os_event_head=os_event_tail=0;
    os_poll_flag=0;
    os_event_lock=xSemaphoreCreateMutex();
    xSemaphoreGive(os_event_lock);
}
static void os_task_call(os_task_t* t, os_event_t event, os_data_t data)
{
    int ret;

    if(t->state==OS_TASK_STATE_IDLE&&t->app!=NULL){
        t->state=OS_TASK_STATE_ACTIVE;
        ret=t->app(&(t->pt),event,data);
        if(ret==OS_EXITED||event==OS_EVENT_EXIT){
            os_task_exit(t);
        }
        else{
            t->state=OS_TASK_STATE_IDLE;
        }
    }
}
void os_task_start(os_task_t* t,os_data_t data)
{
    os_task_t *p;
    for(p=os_task_list;p!=t&&p!=NULL;p=p->next);
    if(p==t){
        return;
    }

    t->next=os_task_list;
    os_task_list=t;
    OS_PT_INIT(&t->pt);
    t->state=OS_TASK_STATE_IDLE;
    t->poll_flag=0;
    os_task_call(t,OS_EVENT_INIT,data);
}

void os_task_exit(os_task_t* t)
{
    os_task_t *p;

    t->state=OS_TASK_STATE_NONE;

    if(t==os_task_list){
        os_task_list=t->next;
    }
    else{
        for(p=os_task_list;p->next!=NULL;p=p->next){
            if(p->next==t){
                p->next=t->next;
                break;
            }
        }
    }
}

void os_task_poll(os_task_t *t,unsigned char with_isr)
{
    if(t != NULL) {
        if(t->state !=OS_TASK_STATE_NONE){
            t->poll_flag = 1;
            os_poll_flag = 1;
            jmesh_task_wakeup(with_isr);
        }
    }
}

void os_do_poll(void)
{
    os_task_t* t;
    if(os_poll_flag){

        os_poll_flag=0;

        for(t=os_task_list;t!=NULL;t=t->next){
            if(t->poll_flag){
                t->state=OS_TASK_STATE_IDLE;
                t->poll_flag=0;
                os_task_call(t,OS_EVENT_POLL,NULL);
            }
        }
    }
}

int os_event_is_null(void)
{
    return os_event_head==os_event_tail;
}

void os_do_event(void)
{
    if(os_event_head!=os_event_tail){

        os_event[os_event_head].t->state=OS_TASK_STATE_IDLE;
        os_task_call(os_event[os_event_head].t,
                     os_event[os_event_head].event,
                     os_event[os_event_head].data);

        xSemaphoreTake(os_event_lock,portMAX_DELAY);
        os_event_head=(os_event_head+1)%OS_EVENT_SIZE;
        xSemaphoreGive(os_event_lock);
    }

}
int os_event_post(os_task_t* t,os_event_t event,os_data_t data)
{
    os_event_t tail;
    xSemaphoreTake(os_event_lock,portMAX_DELAY);
    tail=(os_event_tail+1)%OS_EVENT_SIZE;
    if(os_event_head==tail){
        xSemaphoreGive(os_event_lock);
        return OS_ERR_EVENT_FULL;
    }
    os_event[os_event_tail].event=event;
    os_event[os_event_tail].data=data;
    os_event[os_event_tail].t=t;
    os_event_tail=tail;
    xSemaphoreGive(os_event_lock);
    jmesh_task_wakeup(0);
    return OS_ERR_OK;
}

os_event_t os_event_alloc(void)
{
    static os_event_t last_event=0;
    return last_event++;
}

