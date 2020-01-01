#include"jmesh_beacon.h"
#include"jmesh_device_beacon.h"
#include"jmesh_beacon_callback.h"
#include"../../onchip_system/os_timer_event.h"
#include"../../jmesh/jmesh_pdu.h"
#include"../adv/jmesh_adv.h"
#include"../../jmesh/jmesh_print.h"
static os_timer_event_t device_beacon_timer;
static unsigned short device_beacon_send_times;


static void send_device_beacon(void){
    jmesh_pdu_t* pdu;
    jmesh_beacon_t* beacon;
    if(device_beacon_send_times){
        device_beacon_send_times--;
        if(device_beacon_send_times==0){
            JMESH_LOGN("device beacon","finish send stop\n");
            return;
        }
    }
    os_timer_event_set(&device_beacon_timer,JMESH_DEVICE_BEACON_INTERVAL_S*1000+(rand()/*+adv_rssi*/)%1000,(os_timer_event_caller_t)send_device_beacon,NULL);
    if(NULL==(pdu=jmesh_pdu_new(sizeof(jmesh_device_beacon_t)+JMESH_BEACON_HEAD_SIZE))){
        JMESH_LOGW(1,"device beacon","new pdu len=%d failed\n",sizeof(jmesh_device_beacon_t)+JMESH_BEACON_HEAD_SIZE);
        return;
    }
    beacon=(jmesh_beacon_t*)pdu->adv.para;
    beacon->type=JMESH_BEACON_TYPE_DEVICE;
    beacon->device_beacon.OOB[0]=(unsigned char)(JMESH_DEVICE_OOB>>8);
    beacon->device_beacon.OOB[1]=(unsigned char)JMESH_DEVICE_OOB;
    beacon->device_beacon.URI[0]=(unsigned char)(JMESH_DEVICE_URI_HASH>>24);
    beacon->device_beacon.URI[1]=(unsigned char)(JMESH_DEVICE_URI_HASH>>16);
    beacon->device_beacon.URI[2]=(unsigned char)(JMESH_DEVICE_URI_HASH>>8);
    beacon->device_beacon.URI[3]=(unsigned char)(JMESH_DEVICE_URI_HASH);

    jmesh_device_beacon_usr_info_set_callback(10,beacon->device_beacon.usr_info);

    pdu->adv.length=1+sizeof(jmesh_device_beacon_t);
    JMESH_LOGD("device beacon","sending\n");
    jmesh_adv_send_beacon(pdu,1);
    jmesh_pdu_free(pdu);
}

void jmesh_device_beacon_start(unsigned short send_times){
    device_beacon_send_times=send_times;
    JMESH_LOGN("device beacon","start %d times\n",send_times);
    os_timer_event_set(&device_beacon_timer,JMESH_DEVICE_BEACON_INTERVAL_S*1000,(os_timer_event_caller_t)send_device_beacon,NULL);
}
void jmesh_device_beacon_stop(void){
    JMESH_LOGN("device beacon","stop\n");
    os_timer_event_remove(&device_beacon_timer);
}
void jmesh_device_beacon_handler(jmesh_device_beacon_t* beacon){
}
