#include"jmesh_adv.h"
#include"../../driver/jmesh_ble.h"
#include"../../driver/jmesh_system.h"
#include"../beacon/jmesh_beacon.h"
#include"../../network/jmesh_interface.h"
#include"../../network/jmesh_network.h"
#include"../../jmesh/jmesh_pdu.h"
#include"../../driver/jmesh_driver_config.h"
#include"../../jmesh/jmesh_task.h"
#include"../../jmesh/jmesh_print.h"
#include"../../upper/lowpower/jmesh_lowpower.h"
#include"stdio.h"
#ifndef x86
#include"FreeRTOS.h"
#include"task.h"
#endif
static os_timer_t adv_beacon_timer;

void jmesh_rev_adv_timer_start(void){
    os_timer_set(&adv_beacon_timer,JMESH_ADV_RECV_INTERVAL_MS);
    //os_timer_set(&adv_message_timer,JMESH_ADV_RECV_INTERVAL_MS);
}

int jmesh_rev_beacon_timer_is_flow(void)
{
    return os_timer_is_flow(&adv_beacon_timer);
}

void jmesh_rev_beacon_timer_start(void)
{
    os_timer_restart(&adv_beacon_timer);
}

void jmesh_adv_send_beacon(jmesh_pdu_t* pdu,unsigned char repeats)
{
    pdu->adv.ad_type=JMESH_ADV_TYPE_BEACON;
    jmesh_adv_send(pdu->adv.ad_type,pdu->adv.length,pdu->adv.para,repeats);
}
void jmesh_adv_send_message(jmesh_pdu_t* pdu){
    unsigned char adv_repeats;
    pdu->adv.ad_type=JMESH_ADV_TYPE_MESSAGE;
    adv_repeats=(JMESH_FEATURE_ENABLE==jmesh_get_lowpower_feature())?lowpower_param.adv_repeats:JMESH_ADV_REPEAT_NUM;
    jmesh_adv_send(pdu->adv.ad_type,pdu->adv.length,pdu->adv.para,adv_repeats);
}
void jmesh_adv_send_message2(unsigned char length,unsigned char *data){
    unsigned char adv_repeats;
    adv_repeats=(JMESH_FEATURE_ENABLE==jmesh_get_lowpower_feature())?lowpower_param.adv_repeats:JMESH_ADV_REPEAT_NUM;
    jmesh_adv_send(JMESH_ADV_TYPE_MESSAGE,length,data,adv_repeats);
}
void jmesh_adv_send_provision(jmesh_pdu_t* pdu){
    pdu->adv.ad_type=JMESH_ADV_TYPE_PROVISIONING;
    jmesh_adv_send(pdu->adv.ad_type,pdu->adv.length,pdu->adv.para,JMESH_ADV_REPEAT_NUM);
}

void jmesh_adv_recv(unsigned char* mac,unsigned char rssi,unsigned char len,unsigned char ad_type,unsigned char* data)
{
    jmesh_pdu_t* pdu;

    switch(ad_type){
    case(JMESH_ADV_TYPE_BEACON):
		    jmesh_beacon_handler(mac,rssi,(jmesh_beacon_t*)data);
        break;
    case(0xff):
    case(JMESH_ADV_TYPE_MESSAGE):
		    if(len<JMESH_PDU_NETWORK_MIN_LEN){
            return;
        }
		    if(network_cipher_cache(data)) return ;
        //JMESH_LOGD_BUFFER("adv",len,data,"recv from:%x %x %x %x %x %x,message:",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
        if(NULL==(pdu=jmesh_pdu_new(sizeof(jmesh_adv_pdu_t)))){
            JMESH_LOGW(1,"adv","new pdu len=%d failed\n",sizeof(jmesh_adv_pdu_t));
            return;
        }
        memcpy(pdu->adv.para,data,len);
        memcpy(pdu->adv.mac,mac,6);
        pdu->adv.rssi=rssi;
        pdu->length=len;
        jmesh_interface_recv(JMESH_INTERFACE_ADV,pdu);
        break;
    case(JMESH_ADV_TYPE_PROVISIONING):
        break;
    }
}
