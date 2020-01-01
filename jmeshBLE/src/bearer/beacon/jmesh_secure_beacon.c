#include"jmesh_secure_beacon.h"
#include"../../jmesh/jmesh_pdu.h"
#include"../../jmesh/jmesh_print.h"
#include"../../onchip_system/os_timer_event.h"
#include"jmesh_beacon.h"
#include"../adv/jmesh_adv.h"
//#include "log.h"
static os_timer_event_t secure_beacon_timer;
static unsigned short secure_beacon_time_cache[JMESH_SECURE_BEACON_TIME_CACHE_SIZE]={0};//60*20/10=120s,
static unsigned char secure_beacon_time_index=0;

static void send_secure_beacon(void)
{
    int i,j;
    jmesh_pdu_t* pdu;
    jmesh_beacon_t* beacon;
    jmesh_netkey_t *net_key;
    jmesh_key_t cmac;
    unsigned long iv_index;

    for(i=1;i<JMESH_SECURE_BEACON_TIME_CACHE_SIZE;i++){
        j=(secure_beacon_time_index+JMESH_SECURE_BEACON_TIME_CACHE_SIZE-i)%JMESH_SECURE_BEACON_TIME_CACHE_SIZE;
        if(secure_beacon_time_cache[secure_beacon_time_index]-secure_beacon_time_cache[j]>JMESH_SECURE_BEACON_OBSERVATION_PERIOD_S
           ||secure_beacon_time_cache[j]==0){//now-history>observation_period
            break;
        }
    }
    os_timer_event_set(&secure_beacon_timer,(JMESH_SECURE_BEACON_OBSERVATION_PERIOD_S*i/JMESH_SECURE_BEACON_EXPECTED_NUMBER)*1000,(os_timer_event_caller_t)send_secure_beacon,NULL);

    net_key=jmesh_netkey_get_by_index(0);
    if(NULL==net_key)
    {
        return;
    }
    if(NULL==(pdu=jmesh_pdu_new(sizeof(jmesh_secure_beacon_t)+JMESH_BEACON_HEAD_SIZE))){

        JMESH_LOGW(1,"secure beacon","new pdu len=%d failed\n",sizeof(jmesh_secure_beacon_t)+JMESH_BEACON_HEAD_SIZE);
        return;
    }

    beacon=(jmesh_beacon_t*)pdu->adv.para;
    beacon->type=JMESH_BEACON_TYPE_SECURE;
    beacon->secure_beacon.key_refresh_flag=(2==jmesh_netkey_get_phase(net_key))? 1 : 0;
    beacon->secure_beacon.iv_update_flag=jmesh_netkey_get_iv_update_flag(0);
    iv_index=jmesh_netkey_get_iv_index(0);
    beacon->secure_beacon.iv_index[0]=(unsigned char)(iv_index>>24);
    beacon->secure_beacon.iv_index[1]=(unsigned char)(iv_index>>16);
    beacon->secure_beacon.iv_index[2]=(unsigned char)(iv_index>>8);
    beacon->secure_beacon.iv_index[3]=(unsigned char)(iv_index);

    memcpy(beacon->secure_beacon.network_id,JMESH_NETKEY_GET_NETID(net_key),JMESH_NETID_SIZE);

    security_AES_CMAC((uint8_t *)JMESH_NETKEY_GET_BEACON_KEY(net_key),13,(uint8_t *)&beacon->secure_beacon,(uint8_t *)cmac);
    memcpy(beacon->secure_beacon.authentication_value,cmac,8);

    pdu->adv.length=1+sizeof(jmesh_secure_beacon_t);
    //JMESH_LOGN("secure beacon","sending\n");
    jmesh_adv_send_beacon(pdu,JMESH_ADV_REPEAT_NUM);
    jmesh_pdu_free(pdu);
}

void jmesh_secure_beacon_start(void)
{
    int i;
    /*if(JMESH_FEATURE_ENABLE==jmesh_get_lowpower_feature()){
        return;
    }*/
    for(i=0;i<JMESH_SECURE_BEACON_TIME_CACHE_SIZE;i++){
        secure_beacon_time_cache[i]=0;
    }
    //JMESH_LOGN("secure beacon","start\n");
    os_timer_event_set(&secure_beacon_timer,6000,(os_timer_event_caller_t)send_secure_beacon,NULL);
}
void jmesh_secure_beacon_stop(void){
    //JMESH_LOGN("secure beacon","stop\n");
    //if(JMESH_FEATURE_ENABLE!=jmesh_get_lowpower_feature()){
        os_timer_event_remove(&secure_beacon_timer);
    //}
}
void jmesh_secure_beacon_handler(jmesh_secure_beacon_t* beacon){
    jmesh_key_t auth;
    static unsigned char cache_auth[8]={0};

    jmesh_netkey_t *net_key;
    unsigned long secure_iv_index,iv_index;

    net_key=jmesh_netkey_get_by_index(0);
    if(NULL==net_key)
    {
        return;
    }
    if(0==memcmp(beacon->network_id,JMESH_NETKEY_GET_NETID(net_key),JMESH_NETID_SIZE)
       &&0!=memcmp(cache_auth,beacon->authentication_value,8)){
        security_AES_CMAC((uint8_t *)JMESH_NETKEY_GET_BEACON_KEY(net_key),13,(uint8_t *)beacon,(uint8_t *)auth);
        if(0==memcmp(auth,beacon->authentication_value,8)){
            //LOG(LOG_LVL_INFO,"\nprocess secure beacon\n");

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
            }

            memcpy(cache_auth,auth,8);
            secure_beacon_time_cache[secure_beacon_time_index]=os_time_get_seconds();
            secure_beacon_time_index=(secure_beacon_time_index+1)%JMESH_SECURE_BEACON_TIME_CACHE_SIZE;
        }
    }
}

int jmesh_secure_beacon_recv_handler(jmesh_pdu_t *pdu)
{
    jmesh_secure_beacon_handler(&pdu->adv.beacon.secure_beacon);
    jmesh_pdu_free(pdu);
    return 0;
}
