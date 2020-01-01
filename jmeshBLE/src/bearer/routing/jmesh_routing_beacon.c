#include"jmesh_routing_beacon.h"
#include"../../jmesh/jmesh_print.h"
#include"jmesh_route.h"
#include"jmesh_routing_neighbor.h"
#include"../../onchip_system/os_timer_event.h"
#include"../../network/jmesh_netkey.h"
#include"../../jmesh/jmesh_pdu.h"
#include"../beacon/jmesh_beacon.h"
#include"../gatt/jmesh_gatt.h"
#include"../../driver/jmesh_system.h"
#include"../../jmesh/jmesh_addr.h"
#include"../adv/jmesh_adv.h"
#include"../../driver/jmesh_ble.h"
#include <stdlib.h>
#include"../../upper/lowpower/jmesh_friend.h"
static os_timer_event_t routing_beacon_timer;

static void routing_beacon_send(void){
    //JMESH_LOGI("r b s","\n");
    if(JMESH_FEATURE_ENABLE==jmesh_get_lowpower_feature()){
        return;
    }
    jmesh_routing_beacon_link_require(0,NULL);
    os_timer_event_set(&routing_beacon_timer,
                       jmesh_random_get(JMESH_ROUTING_BEACON_INTERVAL_MIN_MS,JMESH_ROUTING_BEACON_INTERVAL_MAX_MS),
                       (os_timer_event_caller_t)routing_beacon_send,
                       NULL);
}
void jmesh_routing_beacon_start(void){
    if(JMESH_FEATURE_ENABLE==jmesh_get_lowpower_feature()){
        return;
    }
    os_timer_event_set(&routing_beacon_timer,
                       JMESH_ROUTING_BEACON_INTERVAL_MIN_MS,
                       (os_timer_event_caller_t)routing_beacon_send,
                       NULL);
}
void jmesh_routing_beacon_stop(void)
{
    //if(JMESH_FEATURE_ENABLE!=jmesh_get_lowpower_feature()){
        os_timer_event_remove(&routing_beacon_timer);
    //}
}
int jmesh_routing_beacon_link_require(unsigned short dest,jmesh_friend_poll_t *poll)
{
    jmesh_netkey_t* netkey;
    jmesh_pdu_t* pdu;
    //os_timer_event_restart(&routing_beacon_timer);
    //if(0==jmesh_get_primary_addr()){
    //    return -1;
    //}
    if(NULL==(pdu=jmesh_pdu_new(sizeof(jmesh_routing_beacon_t)+3))){
        JMESH_LOGW(1,"route beacon","new pdu len=%d failed\n",sizeof(jmesh_routing_beacon_t)+3);
        return -1;
    }
    netkey=jmesh_netkey_get_by_index(0);

    jmesh_routing_fill_beacon(&((jmesh_beacon_t*)pdu->adv.para)->routing_beacon);
    JMESH_BIG_ENDIAN_FILL2(dest,((jmesh_beacon_t*)pdu->adv.para)->routing_beacon.link_require);
    if(poll){
        memcpy(&((jmesh_beacon_t*)pdu->adv.para)->routing_beacon.poll,poll,sizeof(jmesh_friend_poll_t));
    }
    else{
        memset(&((jmesh_beacon_t*)pdu->adv.para)->routing_beacon.poll,0x00,sizeof(jmesh_friend_poll_t));
    }
    ((jmesh_beacon_t*)pdu->adv.para)->type=JMESH_BEACON_TYPE_ROUTING;
//    security_AES_CMAC((uint8_t *)JMESH_NETKEY_GET_BEACON_KEY(netkey),JMESH_ROUTING_BEACON_MAC_OFFSET,(uint8_t *)&((jmesh_beacon_t*)pdu->adv.para)->routing_beacon,(uint8_t *)cmac);
    memcpy(&((jmesh_beacon_t*)pdu->adv.para)->routing_beacon.auth_value,(uint8_t *)JMESH_NETKEY_GET_BEACON_KEY(netkey),JMESH_ROUTING_BEACON_MIC_SIZE);

    pdu->adv.length=1+sizeof(jmesh_routing_beacon_t);
    if(dest)
    {
        jmesh_adv_send_beacon(pdu,JMESH_ADV_REPEAT_NUM);
    }
    else
    {
        jmesh_adv_send_beacon(pdu,1);
    }
    jmesh_pdu_free(pdu);
    return 0;
}
void jmesh_routing_beacon_handler(unsigned char* mac,unsigned char rssi,jmesh_routing_beacon_t* beacon)
{

    unsigned short link_require_addr;
    jmesh_netkey_t *netkey;
    jmesh_routing_neighbor_t* neighbor;
    unsigned short prim_addr;
    unsigned short peer_addr;
    //unsigned long secure_iv_index;
    //unsigned long iv_index;
    unsigned short net_ticks;

    if(NULL==(netkey=jmesh_netkey_get_by_index(0))){
        return;
    }
//    security_AES_CMAC((uint8_t *)JMESH_NETKEY_GET_BEACON_KEY(netkey),JMESH_ROUTING_BEACON_MAC_OFFSET,(uint8_t *)beacon,(uint8_t *)auth);
    if(0==memcmp((uint8_t *)JMESH_NETKEY_GET_BEACON_KEY(netkey),beacon->auth_value,JMESH_ROUTING_BEACON_MIC_SIZE)){
        JMESH_BIG_ENDIAN_PICK2(peer_addr,beacon->addr);
        JMESH_BIG_ENDIAN_PICK2(link_require_addr,beacon->link_require);
        if(0==peer_addr){
            return;
        }
        prim_addr=jmesh_get_primary_addr();
        if(peer_addr==prim_addr){
            JMESH_LOGW(1,"route beacon","mesh addr cannot be same\n");
            return;
        }
        if(beacon->poll.valid){
            if(JMESH_FEATURE_ENABLE==jmesh_get_friend_feature()){
                friend_poll_handler(peer_addr,link_require_addr,0,&beacon->poll);
            }
            return;
        }
        /*else if(3!=peer_addr&&5!=peer_addr){
            JMESH_LOGW(1,"error","%d\n",peer_addr);
        }*/
        if(NULL!=(neighbor=jmesh_routing_neighbor_update(mac,rssi,beacon))){
            if(link_require_addr==prim_addr && rssi >JMESH_ROUTING_NEIGHBOR_MIN_RSSI){
                JMESH_LOGD("route beacon","%x %x %x %x %x %x link require\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
                jmesh_routing_link_require(neighbor);
            }
            else if(0==link_require_addr&&peer_addr<prim_addr){
                JMESH_BIG_ENDIAN_PICK2(net_ticks,beacon->net_ticks);
                //JMESH_LOGI("tick","%d,%d,%d\n",net_ticks,jmesh_net_ticks_get(),peer_addr);
                net_ticks++;
                if(abs(net_ticks-jmesh_net_ticks_get())>5)
                {
                    jmesh_net_ticks_set(net_ticks);
                }
            }
            /*
            secure_iv_index=beacon->iv_index[0];
            secure_iv_index<<=8;
            secure_iv_index|=beacon->iv_index[1];
            secure_iv_index<<=8;
            secure_iv_index|=beacon->iv_index[2];
            secure_iv_index<<=8;
            secure_iv_index|=beacon->iv_index[3];

            iv_index=jmesh_netkey_get_iv_index(0);

            JMESH_LOGD("secure beacon","recv iv index 0x%x local=0x%x\n",secure_iv_index,iv_index);
            if(secure_iv_index==1+iv_index){
                JMESH_LOGN("secure beacon","update iv index to 0x%x\n",secure_iv_index);
                if(beacon->iv_update_flag==1){
                    jmesh_netkey_start_update_iv_index(0);//start update iv index
                }
                else{
                    jmesh_netkey_recover_iv_index(0,secure_iv_index);//recover iv index
                }
            }
            if(secure_iv_index>iv_index+1 && secure_iv_index<iv_index+48){
                JMESH_LOGN("secure beacon","recover iv index from 0x%x to 0x%x\n",iv_index,secure_iv_index);
                jmesh_netkey_recover_iv_index(0,secure_iv_index);
            }
            if(beacon->iv_update_flag==0&&secure_iv_index==iv_index){//stop update iv_index
                jmesh_netkey_stop_update_iv_index(0);
            }
            if(secure_iv_index+48<iv_index){//ignore
                JMESH_LOGW(1,"secure beacon","wrong iv index\n");
                return;
            }*/
        }
    }
}
