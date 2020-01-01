#include"jmesh_beacon.h"
#include"../../jmesh/jmesh_print.h"
#include"../../jmesh/jmesh_pdu.h"
#include"../../jmesh/jmesh_task.h"
#include"../adv/jmesh_adv.h"

void jmesh_beacon_handler(unsigned char *mac,unsigned char rssi,jmesh_beacon_t* beacon)
{
    jmesh_pdu_t* pdu;
    unsigned short len;
    switch(beacon->type){
    case(JMESH_BEACON_TYPE_DEVICE):
        //LOG(LOG_LVL_INFO,"\nmac:...%02x receive device beacon\n",mac[5]);
        jmesh_device_beacon_handler(&beacon->device_beacon);
        break;

    case(JMESH_BEACON_TYPE_SECURE):
        //LOG(LOG_LVL_INFO,"\nmac:...%02x receive secure beacon\n",mac[5]);
        if(jmesh_rev_beacon_timer_is_flow()){
            jmesh_rev_beacon_timer_start();
        }
        else{
            return;
        }
        len=sizeof(jmesh_secure_beacon_t);
        pdu=jmesh_pdu_new(len+3);
        if(NULL==pdu)
        {
            JMESH_LOGW(1,"secure beacon","new pdu len=%d failed\n",len+2);
        }
        else
        {
            memcpy(pdu->adv.para+1,&beacon->secure_beacon,len);
            if(OS_ERR_EVENT_FULL==os_event_post(&jmesh_task,JMESH_EVENT_SECURE_BEACON_RECV,pdu))
	          {
	              JMESH_LOGW(1,"secure beacon","os_event_post failed\n");
		            jmesh_pdu_free(pdu);
	          }
        }
        break;

    case(JMESH_BEACON_TYPE_ROUTING):
        //LOG(LOG_LVL_INFO,"\nmac:...%02x receive routing beacon\n",mac[5]);
        jmesh_routing_beacon_handler(mac,rssi,&beacon->routing_beacon);
        break;
    case(JMESH_BEACON_TYPE_CONNECT):
        //jmesh_connect_beacon_handler(mac,rssi,&beacon->connect_beacon);
        break;
    }
}
