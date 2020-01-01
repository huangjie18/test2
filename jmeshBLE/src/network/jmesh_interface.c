#include"jmesh_interface.h"
#include"jmesh_network.h"
#include"jmesh_filter.h"
#include"../bearer/adv/jmesh_adv.h"
#include"../bearer/gatt/jmesh_gatt.h"
#include"../jmesh/jmesh_task.h"
#include"../jmesh/jmesh_addr.h"
#include"../jmesh/jmesh_features.h"
#include"../access/jmesh_access.h"
#include"../jmesh/jmesh_print.h"
static jmesh_filter_t interface_output_filter[JMESH_INTERFACE_SIZE];
void jmesh_interface_init(void)
{
    int i;
    //unsigned short primary_addr;
    unsigned char element_num;
    for(i=0;i<JMESH_INTERFACE_SIZE;i++){
        jmesh_filter_init(&interface_output_filter[i]);
        jmesh_filter_set_type(&interface_output_filter[i],JMESH_FILTER_TYPE_BLACK_LIST);
    }


    //add broadcast addr
    jmesh_filter_set_type(&interface_output_filter[JMESH_INTERFACE_LOCAL],JMESH_FILTER_TYPE_WHITE_LIST);

    //suggest addr all not add in filter
    jmesh_filter_add_addr(&interface_output_filter[JMESH_INTERFACE_LOCAL],JMESH_ADDR_ALL);
    jmesh_filter_add_addr(&interface_output_filter[JMESH_INTERFACE_LOCAL],JMESH_ADDR_ALL_ROUTING);
    jmesh_filter_add_addr(&interface_output_filter[JMESH_INTERFACE_LOCAL],JMESH_ADDR_ALL_SUBNET);
    if(JMESH_FEATURE_ENABLE==jmesh_get_proxy_feature()){
        jmesh_filter_add_addr(&interface_output_filter[JMESH_INTERFACE_LOCAL],JMESH_ADDR_ALL_PROXIES);
    }
    if(JMESH_FEATURE_ENABLE==jmesh_get_friend_feature()){
        jmesh_filter_add_addr(&interface_output_filter[JMESH_INTERFACE_LOCAL],JMESH_ADDR_ALL_FRIENDS);
    }
    if(JMESH_FEATURE_ENABLE==jmesh_get_relay_feature()){
        jmesh_filter_add_addr(&interface_output_filter[JMESH_INTERFACE_LOCAL],JMESH_ADDR_ALL_RELAYS);
    }

    //add element addr
    //element_num=jmesh_element_get_num();
    //primary_addr=jmesh_get_primary_addr();
    //for(i=0;i<element_num;i++){
    //    jmesh_filter_add_addr(&interface_output_filter[JMESH_INTERFACE_LOCAL],primary_addr+i);
    //}
    //JMESH_LOGI_BUFFER("interface",interface_output_filter[JMESH_INTERFACE_LOCAL].num*2,interface_output_filter[JMESH_INTERFACE_LOCAL].addr,"set filter:");
    //add group addr in model load and group addr set function
}

void jmesh_interface_set_filter_type(unsigned char interface,unsigned char type){
    if(interface<JMESH_INTERFACE_SIZE){
        jmesh_filter_set_type(&interface_output_filter[interface],type);
    }
}
void jmesh_interface_add_filter_addr(unsigned char interface,unsigned short addr){
    if(interface<JMESH_INTERFACE_SIZE){
        jmesh_filter_add_addr(&interface_output_filter[interface],addr);
    }
}
void jmesh_interface_remove_filter_addr(unsigned char interface,unsigned short addr){
    if(interface<JMESH_INTERFACE_SIZE){
        jmesh_filter_remove_addr(&interface_output_filter[interface],addr);
    }
}
int jmesh_interface_filter_status(unsigned char interface,unsigned char length,unsigned char* buffer){
    if(interface<JMESH_INTERFACE_SIZE){
        return jmesh_filter_status(&interface_output_filter[interface],length,buffer);
    }
    return 0;
}

jmesh_filter_t* jmesh_interface_get_filter(unsigned char interface){
    if(interface<JMESH_INTERFACE_SIZE){
        return &interface_output_filter[interface];
    }
    return NULL;
}
int jmesh_interface_get_filter_num(unsigned char interface){
    if(interface<JMESH_INTERFACE_SIZE){
        return interface_output_filter[interface].num;
    }
    return 0;
}
int jmesh_interface_get_filter_type(unsigned char interface){
    if(interface<JMESH_INTERFACE_SIZE){
        return interface_output_filter[interface].type;
    }
    return 0;
}
int jmesh_interface_filtering(unsigned char interface,unsigned short addr){

    if(interface<JMESH_INTERFACE_SIZE){
        return jmesh_filter_filtering(&interface_output_filter[interface],addr);
    }
    return 0;
}


void jmesh_interface_recv(unsigned char interface,jmesh_pdu_t* pdu)
{
    pdu->network.itf=interface;
	  if(OS_ERR_EVENT_FULL == os_event_post(&jmesh_task,JMESH_EVENT_INTERFACE_RECV,pdu))
	  {
	      JMESH_LOGW(1,"interface","os_event_post failed\n");
		    jmesh_pdu_free(pdu);
	  }
}
void jmesh_interface_send(unsigned char interface,jmesh_pdu_t* pdu){
    pdu->network.itf=interface;
	if(OS_ERR_EVENT_FULL == os_event_post(&jmesh_task,JMESH_EVENT_INTERFACE_SEND,pdu))
	{
	    JMESH_LOGW(1,"interface","os_event_post failed\n");
		jmesh_pdu_free(pdu);
	}
}

int jmesh_interface_recv_handler(jmesh_pdu_t* pdu)
{
    //JMESH_LOGI("int recv","%d\n",pdu->length);
    jmesh_network_recv(pdu);
    return 0;
}
int jmesh_interface_send_handler(jmesh_pdu_t* pdu){
    if(pdu->network.itf<JMESH_INTERFACE_ADV){
        jmesh_proxy_send(JMESH_SEND_TYPE_CONNECT,pdu->network.itf,0xff,JMESH_PROXY_TYPE_NETWORK,pdu->length,pdu->pdu+2);
    }
    else{
        jmesh_adv_send_message(pdu);
    }
    return 0;
}
void jmesh_interface_broadcast(jmesh_pdu_t* pdu)
{
    jmesh_adv_send_message(pdu);
}
