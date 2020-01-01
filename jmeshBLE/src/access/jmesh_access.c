#include"jmesh_access.h"
#include"jmesh_access_callback.h"
#include"../jmesh/jmesh_pdu.h"
#include"../jmesh/jmesh_task.h"
#include"../jmesh/jmesh_print.h"
#include"../jmesh/jmesh_addr.h"
#include"../jmesh/jmesh_device.h"
#include"../jmesh/jmesh_save.h"
#include"../upper/jmesh_upper.h"
#include"../foundation/state/jmesh_bind.h"
#include"../onchip_system/os_timer_event.h"
#include"../driver/jmesh_ble.h"
#include"../upper/control/jmesh_control.h"
#include"../network/jmesh_interface.h"
#ifndef x86
#include"FreeRTOS.h"
#endif
#include"stdio.h"
//#include"log.h"

static jmesh_element_t *jmesh_element_list=NULL;

void jmesh_access_load_para(void){
    int i,j;
    jmesh_element_t* list;
    jmesh_model_t* model;
    jmesh_model_para_t para;
    unsigned short primary_addr;
    list=jmesh_element_list;
    i=j=0;
    primary_addr=jmesh_get_primary_addr();
    while(list!=NULL){
        jmesh_interface_add_filter_addr(JMESH_INTERFACE_LOCAL,primary_addr+j);
        j++;
        model=list->model_list;
        while(model!=NULL){
            jmesh_model_bind_init(model);
            jmesh_model_subscription_list_init(model);
            if(sizeof(jmesh_model_para_t)==jmesh_save_buffer_read(JMESH_SAVE_MODEL,i,sizeof(jmesh_model_para_t),(unsigned char*)&para)){
                model->para=para;
                jmesh_subscription_list_add_filter(&model->para.subscription_list);
            }
            model->id=i;
            i++;
            model=model->next;
        }
        list=list->next;
    }
}


void jmesh_access_init(void){
    jmesh_element_list=NULL;
//    JMESH_LOGI("access init","add element\n");
    jmesh_access_init_element_callback();
//    JMESH_LOGI("access init","add model\n");
    jmesh_access_init_model_callback();
//    JMESH_LOGI("access init","load para\n");
    jmesh_access_load_para();
}
void jmesh_access_add_element(jmesh_element_t* element)
{
    jmesh_element_t* t;

    if(jmesh_element_list==NULL){
        jmesh_element_list=element;
        jmesh_element_list->next=NULL;
        return;
    }
    if(jmesh_element_list==element){
        return;
    }
    t=jmesh_element_list;

    while(t->next!=NULL&&t->next!=element){
        t=t->next;
    }
    if(t->next==NULL){
        element->next=NULL;
        t->next=element;
	  }
}

jmesh_element_t* jmesh_element_get_by_index(int index){
    int i;
    jmesh_element_t* list;
    list=jmesh_element_list;
    for(i=0;i<index;i++){
        if(list==NULL){
            break;
        }
        list=list->next;
    }
    return list;
}
int jmesh_element_get_num(void){
    int i;
    jmesh_element_t* list;
    list=jmesh_element_list;
    for(i=0;list!=NULL;i++){
        list=list->next;
    }
    return i;
}
int jmesh_element_get_index(jmesh_element_t* element)
{
    int i;
    jmesh_element_t* list;
    list=jmesh_element_list;
    for(i=0;list!=NULL;i++){
        if(list==element){
            return i;
        }
        list=list->next;
    }
    return 0xff;
}

jmesh_element_t* jmesh_element_get_by_addr(unsigned short addr){
    int i;
    unsigned short addr1=jmesh_get_addr_by_element(0);
    jmesh_element_t* list;
    list=jmesh_element_list;
    if(addr<addr1)
        return NULL;
    for(i=0;i<addr-addr1;i++){
        list=list->next;
        if(list==NULL){
            return NULL;
        }
    }
    return list;
}
unsigned short jmesh_element_get_addr(jmesh_element_t* element)
{
    return jmesh_get_addr_by_element(jmesh_element_get_index(element));
}


int jmesh_access_send(unsigned short dst,unsigned short netkey_index,unsigned short appkey_index,jmesh_model_t* model,const unsigned char* message,unsigned short length,unsigned char* data,unsigned char channel){

    jmesh_appkey_t* appkey;
    jmesh_access_pdu_t* pdu;
    unsigned char message_opcode_size;
    int unseg_size;

    JMESH_LOGD_BUFFER("access send",length,data,"dst=0x%x,nid=0x%x,aid=0x%x,cid=0x%x,mid=0x%x,message:%x %x %x,len:%d,data:",dst,netkey_index,appkey_index,
                      model->info->model_id.company_id,model->info->model_id.model_id,message[0],message[1],message[2],length);
    if(NULL==(appkey=jmesh_appkey_get_by_key_index(netkey_index,appkey_index))){
        JMESH_LOGW(1,"access send","cannot find appkey(nid=%x aid=0x%x)\n",netkey_index,appkey_index);
        return -1;
    }
    if(0==jmesh_model_is_bind(model,appkey)){
        JMESH_LOGW(1,"access send","model(cid=0x%x,mid=0x%x) has not bind appkey(nid=0x%x,aid=0x%x)\n",model->info->model_id.company_id,model->info->model_id.model_id,netkey_index,appkey_index);
        return -1;
    }
    if(channel == JMESH_SEND_ADV_CHANNEL && length > 8 )
    {
        return -1;
    }
    message_opcode_size=JMESH_MESSAGE_OPCODE_SIZE(message[0]);
    unseg_size=(JMESH_SEND_ADV_CHANNEL==channel)?(31-(JMESH_PDU_ACCESS_UNSEGMENT_PACKAGE_SIZE)):(JMESH_PDU_ACCESS_UNSEGMENT_DATA_SIZE);
	  if(message_opcode_size+length<=unseg_size){
        if(NULL==(pdu=(jmesh_access_pdu_t*)jmesh_pdu_new(message_opcode_size+length+JMESH_PDU_ACCESS_UNSEGMENT_PACKAGE_SIZE))){
            JMESH_LOGW(1,"access send","new pdu len=%x failed\n",message_opcode_size+length+JMESH_PDU_ACCESS_UNSEGMENT_PACKAGE_SIZE);
			      return -1;
        }
        memcpy(pdu->unsegment.data,message,message_opcode_size);
        memcpy(pdu->unsegment.data+message_opcode_size,data,length);
    }else{
		//	TODO: segment long pdu
		    if(NULL==(pdu=(jmesh_access_pdu_t*)jmesh_pdu_new(message_opcode_size+length+JMESH_PDU_ACCESS_SEGMENT_PACKAGE_SIZE))){
            JMESH_LOGW(1,"access send","new pdu len=%x failed\n",message_opcode_size+length+JMESH_PDU_ACCESS_SEGMENT_PACKAGE_SIZE);
	          return -1;
		    }
		    memcpy(pdu->segment.data,message,message_opcode_size);
		    memcpy(pdu->segment.data+message_opcode_size,data,length);
	  }
    pdu->head.channel = channel;
    pdu->head.src=jmesh_get_addr_by_element(jmesh_element_get_index(model->element));
    pdu->head.dst=dst;
    pdu->head.appkey=appkey;
    pdu->head.length=message_opcode_size+length;
	  if(OS_ERR_EVENT_FULL == os_event_post(&jmesh_task,JMESH_EVENT_ACCESS_SEND,pdu))
	  {
		    JMESH_LOGW(1,"access send","os_event_post failed\n");
		    jmesh_pdu_free(pdu);
	  }
    return 0;
}

int jmesh_access_recv(jmesh_access_pdu_t* pdu)
{
    jmesh_element_t* element;
    jmesh_model_t* model;
    int i;
    unsigned short addr;
    unsigned char opcode_size;

    //JMESH_LOGI("acc recv","\n");
    if(pdu->head.length<=JMESH_PDU_ACCESS_UNSEGMENT_DATA_SIZE){ /*JMESH_UPPER_UNSEGMENT_MAX_LENGTH*/
        opcode_size=JMESH_MESSAGE_OPCODE_SIZE(pdu->unsegment.data[0]);
        JMESH_LOGD_BUFFER("access recv",pdu->head.length,pdu->unsegment.data,"unseg src=0x%x,dst=0x%x,nid=0x%x,aid=0x%x,data:",pdu->head.src,pdu->head.dst,pdu->head.appkey->netkey_index,pdu->head.appkey->appkey_index);
    }
    else{
        opcode_size=JMESH_MESSAGE_OPCODE_SIZE(pdu->segment.data[0]);
        JMESH_LOGD_BUFFER("access recv",pdu->head.length,pdu->segment.data,"seg src=0x%x,dst=0x%x,nid=0x%x,aid=0x%x,data:",pdu->head.src,pdu->head.dst,pdu->head.appkey->netkey_index,pdu->head.appkey->appkey_index);
    }
    addr=jmesh_get_primary_addr();
    for(element=jmesh_element_list;element!=NULL;element=element->next,addr++){
        for(model=element->model_list;model!=NULL;model=model->next){
            if(0==jmesh_model_addr_check(model,addr,pdu->head.dst)){
                continue;
            }
            if(0==jmesh_model_is_bind(model,pdu->head.appkey)){
                continue;
            }
            for(i=0;i<model->info->message_num;i++){
                if(pdu->head.length<=JMESH_PDU_ACCESS_UNSEGMENT_DATA_SIZE){ /*JMESH_UPPER_UNSEGMENT_MAX_LENGTH*/
                    if(0==memcmp(model->info->message[i].message,pdu->unsegment.data,opcode_size)){
                        model->info->message[i].handler(pdu->head.src,pdu->head.dst,pdu->head.appkey->netkey_index,pdu->head.appkey->appkey_index,pdu->head.length-opcode_size,pdu->unsegment.data+opcode_size);
                    }
                }
                else{
                    if(0==memcmp(model->info->message[i].message,pdu->segment.data,opcode_size)){
                        model->info->message[i].handler(pdu->head.src,pdu->head.dst,pdu->head.appkey->netkey_index,pdu->head.appkey->appkey_index,pdu->head.length-opcode_size,pdu->segment.data+opcode_size);
                    }
                }
            }
        }
    }
    jmesh_pdu_free((jmesh_pdu_t*)pdu);
    return 0;
}
int jmesh_config_send(unsigned short dst,jmesh_model_t* model,const unsigned char* message,unsigned short netkey_index,unsigned short length,unsigned char* data){
    jmesh_access_pdu_t* pdu;
    unsigned char* devkey;
    unsigned char message_opcode_size;
    JMESH_LOGI_BUFFER("config send",length,data,"dst=0x%x,nid=0x%x,cid=0x%x,mid=0x%x,message:0x%x 0x%x 0x%x,data:",dst,model->info->model_id.company_id,model->info->model_id.model_id,message[0],message[1],message[2]);
    if(NULL==(devkey=jmesh_device_get_devkey(jmesh_get_primary_addr()))){
        JMESH_LOGW(1,"config send","cannot find dst=0x%x devkey\n",dst);
        return -1;
    }

    message_opcode_size=JMESH_MESSAGE_OPCODE_SIZE(message[0]);
	if(message_opcode_size+length<=JMESH_PDU_ACCESS_UNSEGMENT_DATA_SIZE){
        if(NULL==(pdu=(jmesh_access_pdu_t*)jmesh_pdu_new(message_opcode_size+length+JMESH_PDU_ACCESS_UNSEGMENT_PACKAGE_SIZE))){
            JMESH_LOGW(1,"config send","new pdu len=%x failed\n",message_opcode_size+length+JMESH_PDU_ACCESS_UNSEGMENT_PACKAGE_SIZE);
			return -1;
        }
        memcpy(pdu->unsegment.data,message,message_opcode_size);
        memcpy(pdu->unsegment.data+message_opcode_size,data,length);
    }else{
		if(NULL==(pdu=(jmesh_access_pdu_t*)jmesh_pdu_new(message_opcode_size+length+JMESH_PDU_ACCESS_SEGMENT_PACKAGE_SIZE))){
            JMESH_LOGW(1,"config send","new pdu len=%x failed\n",message_opcode_size+length+JMESH_PDU_ACCESS_SEGMENT_PACKAGE_SIZE);
			return -1;
		}
		memcpy(pdu->segment.data,message,message_opcode_size);
		memcpy(pdu->segment.data+message_opcode_size,data,length);
	}
    pdu->head.src=jmesh_get_addr_by_element(jmesh_element_get_index(model->element));
    pdu->head.dst=dst;
    pdu->head.netkey_index=netkey_index;
    pdu->head.length=message_opcode_size+length;
	if(OS_ERR_EVENT_FULL == os_event_post(&jmesh_task,JMESH_EVENT_CONFIG_SEND,pdu))
	{
		JMESH_LOGW(1,"config send","os_event_post failed\n");
		jmesh_pdu_free(pdu);
	}
    return 0;
}
int jmesh_config_recv(jmesh_access_pdu_t* pdu){
    jmesh_element_t* element;
    jmesh_model_t* model;
    int i;
    unsigned short addr;
    unsigned char opcode_size;

    if(pdu->head.length<=JMESH_PDU_ACCESS_UNSEGMENT_DATA_SIZE){
        opcode_size=JMESH_MESSAGE_OPCODE_SIZE(pdu->unsegment.data[0]);
        JMESH_LOGI_BUFFER("config recv",pdu->head.length,pdu->unsegment.data,"src=0x%x,dst=0x%x,nid=0x%x,data:",pdu->head.src,pdu->head.dst,pdu->head.netkey_index);
    }
    else{
        opcode_size=JMESH_MESSAGE_OPCODE_SIZE(pdu->segment.data[0]);
        JMESH_LOGI_BUFFER("config recv",pdu->head.length,pdu->segment.data,"src=0x%x,dst=0x%x,nid=0x%x,data:",pdu->head.src,pdu->head.dst,pdu->head.netkey_index);
    }
    addr=jmesh_get_primary_addr();
    if(addr!=pdu->head.dst){
		JMESH_LOGW(1,"config recv","dst failed expect 0x%x but 0x%x\n",addr,pdu->head.dst);
        jmesh_pdu_free((jmesh_pdu_t*)pdu);
        return -1;
    }
    element=jmesh_element_list;
    for(model=element->model_list;model!=NULL;model=model->next){
        for(i=0;i<model->info->message_num;i++){
			if(pdu->head.length<=JMESH_PDU_ACCESS_UNSEGMENT_DATA_SIZE){ /*JMESH_UPPER_UNSEGMENT_MAX_LENGTH*/
				if(0==memcmp(model->info->message[i].message,pdu->unsegment.data,opcode_size)){
					model->info->message[i].handler(pdu->head.src,pdu->head.dst,pdu->head.netkey_index,0,pdu->head.length-opcode_size,pdu->unsegment.data+opcode_size);
				}
			}
			else{
				if(0==memcmp(model->info->message[i].message,pdu->segment.data,opcode_size)){
					model->info->message[i].handler(pdu->head.src,pdu->head.dst,pdu->head.netkey_index,0,pdu->head.length-opcode_size,pdu->segment.data+opcode_size);
				}
			}
        }
    }
    jmesh_pdu_free((jmesh_pdu_t*)pdu);
    return 0;
}
