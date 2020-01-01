#include"jmesh_friend.h"
#include"../control/jmesh_control.h"
#include"../../jmesh/jmesh_features.h"
#include"../../jmesh/jmesh_addr.h"
#include"../../network/jmesh_filter.h"
#include"../../network/jmesh_netkey.h"
#include"../../onchip_system/os_timer_event.h"
#include"../../access/jmesh_access.h"
#include"../../driver/jmesh_system.h"
#include"../../jmesh/jmesh_print.h"
#include"../../bearer/routing/jmesh_routing_neighbor.h"
#include"../../bearer/gatt/jmesh_proxy.h"
#include"jmesh_lowpower.h"
#include"jmesh_route.h"
typedef struct st_jmesh_friendship{

    unsigned short addr;
    unsigned char element_num;
    unsigned char established_flag;

    unsigned long receive_delay:8;
    unsigned long poll_interval:16;//0x0a-0x34bbff *100ms
    unsigned long poll_retry_times:8;

    unsigned short previous_address;
    unsigned short local_delay;
    unsigned short LPN_counter;

    unsigned char cache_head;
    unsigned char cache_num;
    jmesh_pdu_t* cache[JMESH_FRIEND_CACHE_QUEUE_SIZE];

    jmesh_filter_t subscription_filter;

    os_timer_event_t timer;

}jmesh_friendship_t;


static unsigned short friend_counter;/**< LPN counter or friend counter */
static jmesh_friendship_t jmesh_friendship[JMESH_FRIENDSHIP_MAX_NUM];

void jmesh_friendship_init(void){

    friend_counter=0;
    memset(jmesh_friendship,0,sizeof(jmesh_friendship));//filter=white list
  //  for(int i=0;i<JMESH_FRIENDSHIP_MAX_NUM;i++){
   //     jmesh_filter_set_type(&jmesh_friendship[i].subscription_filter,JMESH_FILTER_TYPE_WHITE_LIST);
   // }
}
jmesh_friendship_t* jmesh_friendship_get(unsigned short addr){
    int i;
    for(i=0;i<JMESH_FRIENDSHIP_MAX_NUM;i++){
        if(jmesh_friendship[i].addr==addr){
            return &jmesh_friendship[i];
        }
    }
    return NULL;
}

jmesh_friendship_t* jmesh_friendship_new(void){
    int i;
    for(i=0;i<JMESH_FRIENDSHIP_MAX_NUM;i++){
        if(jmesh_friendship[i].addr==0){
            return &jmesh_friendship[i];
        }
    }
    return NULL;
}
void jmesh_friendship_free(jmesh_friendship_t* friendship){
    int i;
    jmesh_gatt_t* gatt;
    JMESH_LOGI("frd free","ca num:%d,src:%d\n",friendship->cache_num,friendship->addr);
    gatt=jmesh_gatt_get_link_by_addr(friendship->addr);
    jmesh_gatt_disconnect(gatt);
    for(i=0;i<friendship->cache_num;i++){
        jmesh_pdu_free(friendship->cache[friendship->cache_head]);
        friendship->cache_head=(friendship->cache_head+1)%JMESH_FRIEND_CACHE_QUEUE_SIZE;
    }
    jmesh_lowpower_node_route_delete(friendship->addr);
    friendship->addr=0;
    os_timer_event_remove(&friendship->timer);
    //memset(friendship,0,sizeof(jmesh_friendship_t));
}
void jmesh_friendship_cache_push(jmesh_friendship_t* friendship,jmesh_pdu_t* pdu){
    if(friendship->cache_num>=JMESH_FRIEND_CACHE_QUEUE_SIZE){
        friendship->cache_num=JMESH_FRIEND_CACHE_QUEUE_SIZE-1;
        jmesh_pdu_free(friendship->cache[friendship->cache_head]);//cache full,free and overwrite eldest cache
        friendship->cache_head=(friendship->cache_head+1)%JMESH_FRIEND_CACHE_QUEUE_SIZE;
    }

    friendship->cache[(friendship->cache_head+friendship->cache_num)%JMESH_FRIEND_CACHE_QUEUE_SIZE]=pdu;
    friendship->cache_num++;
    JMESH_LOGI("frd push","ca num:%d\n",friendship->cache_num);

}

int jmesh_friend_pdu_cache(unsigned short dst,jmesh_pdu_t* pdu)
{
    int i;
    for(i=0;i<JMESH_FRIENDSHIP_MAX_NUM;i++){
        if(jmesh_friendship[i].addr){
            if(jmesh_filter_filtering(&jmesh_friendship[i].subscription_filter,dst)){
                JMESH_LOGI("frd fil","%d\n",dst);
                jmesh_friendship_cache_push(&jmesh_friendship[i],pdu);
                return 0;
			      }
        }
    }
    return -1;
}

jmesh_pdu_t* jmesh_friendship_cache_pop(jmesh_friendship_t* friendship){
    jmesh_pdu_t* pdu=NULL;
     if(friendship->cache_num>0){
        pdu=friendship->cache[friendship->cache_head];
        friendship->cache_head=(friendship->cache_head+1)%JMESH_FRIEND_CACHE_QUEUE_SIZE;
        friendship->cache_num--;
    }
    return pdu;
}

void jmesh_friendship_update(jmesh_friendship_t* friendship,jmesh_friend_update_t* update){
    ;
}
unsigned char jmesh_friend_get_rssi(unsigned short addr){//TODO: get rssi from neighbor table
    jmesh_routing_neighbor_t *neighbor=jmesh_routing_neighbor_get_by_addr(addr);
    if(NULL!=neighbor){
        return neighbor->rssi;
    }
    else{
        JMESH_LOGI("get rssi error","no neighbor\n");
        return 0;
    }
}


static void friend_offer(jmesh_friendship_t* friendship){
    jmesh_friend_offer_t offer;
    offer.receive_window=JMESH_FRIEND_RECEIVE_WINDOW;
    offer.queue_size=JMESH_FRIEND_CACHE_QUEUE_SIZE;
    offer.subscription_list_size=JMESH_FILTER_SIZE;
    offer.rssi=jmesh_friend_get_rssi(friendship->addr);
    JMESH_BIG_ENDIAN_FILL2(friend_counter,offer.friend_counter);
    JMESH_LOGI("offer","addr:%d\n",friendship->addr);
    os_timer_event_set(&friendship->timer,JMESH_FRIEND_OFFER_TIMEOUT_MS,(os_timer_event_caller_t)jmesh_friendship_free,friendship);//only receive poll in 1 second ,otherwise discard friendship
    jmesh_control_send(jmesh_get_primary_addr(),
                       friendship->addr,
                       JMESH_FRIEND_NETKEY_INDEX,
                       JMESH_CONTROL_FRIEND_OFFER,
                       1,//fix 0
                       sizeof(jmesh_friend_offer_t),
                       (unsigned char*)&offer);
}

static void friend_update(jmesh_friendship_t* friendship){
    jmesh_friend_update_t update;
    unsigned long iv_index;
    iv_index=jmesh_netkey_get_iv_index(0);
    JMESH_BIG_ENDIAN_FILL4(iv_index,update.iv_index);
    update.IV_update_flag=jmesh_netkey_get_iv_update_flag(0);
    update.MD=friendship->cache_num?1:0;

    jmesh_control_send(jmesh_get_primary_addr(),
                       friendship->addr,
                       JMESH_FRIEND_NETKEY_INDEX,
                       JMESH_CONTROL_FRIEND_UPDATE,
                       1,//fix 0
                       sizeof(jmesh_friend_update_t),
                       (unsigned char*)&update);
}
void jmesh_friend_connected(unsigned short addr)
{
    jmesh_friendship_t* friendship;
    jmesh_pdu_t* pdu;
    if(NULL!=(friendship=jmesh_friendship_get(addr))){


        //if(friendship->established_flag){
          while(NULL!=(pdu=jmesh_friendship_cache_pop(friendship))){
              JMESH_LOGI("frd send","addr:%d\n",friendship->addr);
              pdu->network.itf=JMESH_SEND_GATT_CHANNEL;
              jmesh_friend_send(friendship->addr,pdu);
          }
          JMESH_LOGI("frd upd","addr:%d\n",addr);
          friend_update(friendship);
          //os_timer_event_set(&friendship->timer,friendship->poll_timeout*100,(os_timer_event_caller_t)jmesh_friendship_free,friendship);
         //}
         //else{
         //   os_timer_event_set(&friendship->timer,3000,(os_timer_event_caller_t)jmesh_friendship_free,friendship);

         //}

    }
}


void friend_poll_handler(unsigned short src,unsigned short dst,unsigned short netkey_index,jmesh_friend_poll_t* poll){

    jmesh_friendship_t* friendship;
    //jmesh_pdu_t* pdu;
    jmesh_routing_neighbor_t* neighbor;
    if(NULL!=(friendship=jmesh_friendship_get(src))){
        if(dst!=jmesh_get_primary_addr()){
            //JMESH_LOGI("poll ha","dst:%d\n",dst);
            os_timer_event_set(&friendship->timer,0,(os_timer_event_caller_t)jmesh_friendship_free,friendship);
            return;
        }
        neighbor=jmesh_routing_neighbor_get_by_addr(src);
        if(NULL!=neighbor){
            if(friendship->cache_num||poll->cache_not_empty||poll->poll_fail_counter>=friendship->poll_retry_times/2){
                //JMESH_LOGI("connect","src:%d\n",src);
                //jmesh_connect_start(neighbor->mac);
                jmesh_routing_link_require(neighbor);
            }
        }
        //if(poll->poll_fail_counter<friendship->poll_retry_times){//refresh friend free timer
            os_timer_event_set(&friendship->timer,((friendship->poll_interval+JMESH_LOWPOWER_POLL_INTVAL_OFFSET_S)*1000+JMESH_LOWPOWER_POLL_TIMEOUT_MS)*friendship->poll_retry_times+JMESH_LOWPOWER_POLL_TIMEOUT_MS,(os_timer_event_caller_t)jmesh_friendship_free,friendship);
        //}
        /*while(NULL!=(pdu=jmesh_friendship_cache_pop(friendship))){
            jmesh_friend_send(friendship->addr,pdu);
        }
        friend_update(friendship);

        os_timer_event_set(&friendship->timer,friendship->poll_timeout*100,(os_timer_event_caller_t)jmesh_friendship_free,friendship);
    */
    }
    /*else{
        JMESH_LOGI("poll ha","no frd,src:%d\n",src);
    }*/
}
static void friend_request_handler(unsigned short src,unsigned short netkey_index,jmesh_friend_request_t* request){
    jmesh_friendship_t* friendship;
    /*if(JMESH_FEATURE_ENABLE!=jmesh_get_friend_feature()){
        return;
    }*/
    if(NULL==(friendship=jmesh_friendship_get(src))){
        if(NULL==(friendship=jmesh_friendship_new())){
            return;
        }
    }

    if((1<<request->min_queue_size_log)>JMESH_FRIEND_CACHE_QUEUE_SIZE){//ignore
        jmesh_friendship_free(friendship);
        return;
    }
    friendship->established_flag=0;
    friendship->addr=src;
    friendship->receive_delay=request->receive_delay;
    JMESH_BIG_ENDIAN_PICK2(friendship->poll_interval,request->poll_interval);
    friendship->poll_retry_times=request->poll_retry_times;
    JMESH_BIG_ENDIAN_PICK2(friendship->previous_address,request->previous_address);
    JMESH_BIG_ENDIAN_PICK2(friendship->LPN_counter,request->LPN_counter);
    friendship->element_num=request->element_num;

    friendship->local_delay=JMESH_FRIEND_RECEIVE_WINDOW*(request->receive_window_factor+2)/2
                            + jmesh_friend_get_rssi(friendship->addr)*(request->RSSI_factor+2)/2;
    if(friendship->local_delay<100){
        friendship->local_delay=100;
    }
    JMESH_LOGI("req ha","src:%d\n",src);
    os_timer_event_set(&friendship->timer,friendship->local_delay,(os_timer_event_caller_t)friend_offer,friendship);
}

static void friend_clear_handler(unsigned short src,unsigned short netkey_index,jmesh_friend_clear_t* clear){
    ;
}
static void friend_clear_confirm_handler(unsigned short src,unsigned short netkey_index,jmesh_friend_clear_confirm_t* confirm){
    ;
}
static void friend_subscription_list_confirm(unsigned char transaction_number,jmesh_friendship_t* friendship){

    jmesh_friend_subscription_list_confirm_t confirm;

    confirm.transaction_number=transaction_number;
    jmesh_control_send(jmesh_get_primary_addr(),
                       friendship->addr,
                       JMESH_FRIEND_NETKEY_INDEX,
                       JMESH_CONTROL_FRIEND_SUBSCRIPTION_LIST_CONFIRM,
                       1,//fix 0
                       sizeof(jmesh_friend_subscription_list_confirm_t),
                       (unsigned char*)&confirm);
}
static void friend_subscription_list_add_handler(unsigned short src,unsigned short length,jmesh_friend_subscription_list_add_t* add){
    int i;
    unsigned short addr;
    jmesh_friendship_t* friendship;
    if(NULL==(friendship=jmesh_friendship_get(src))){
        return;
    }
    for(i=0;i<length-1;i+=2){
        JMESH_BIG_ENDIAN_PICK2(addr,add->address_list+i);
        //JMESH_LOGI("frd list add","addr:0x%x\n",addr);
        //JMESH_LOGI_BUFFER("frd list:",length,(unsigned char *)add,"\n");
        jmesh_filter_add_addr(&friendship->subscription_filter,addr);
    }
    JMESH_LOGI("list add confim","num:%d\n",add->transaction_number);
    friend_subscription_list_confirm(add->transaction_number,friendship);
    //friendship->established_flag=1;
    jmesh_friend_connected(src);
    //friend_update(friendship);
    //os_timer_event_set(&friendship->timer,friendship->poll_timeout*100,(os_timer_event_caller_t)jmesh_friendship_free,friendship);

}

static void friend_subscription_list_remove_handler(unsigned short src,unsigned short length,jmesh_friend_subscription_list_remove_t* remove){
    int i;
    unsigned short addr;
    jmesh_friendship_t* friendship;
    if(NULL==(friendship=jmesh_friendship_get(src))){
        return;
    }
    for(i=0;i<length;i+=2){
        JMESH_BIG_ENDIAN_PICK2(addr,remove->address_list+i*2);
        jmesh_filter_remove_addr(&friendship->subscription_filter,addr);
    }
    friend_subscription_list_confirm(remove->transaction_number,friendship);
}
int jmesh_friend_recv(unsigned short netkey_index,unsigned short length,jmesh_pdu_t* pdu){

    unsigned short src;
    jmesh_features_t features;
    if(JMESH_FEATURE_ENABLE==jmesh_get_friend_feature()){
        src=pdu->control.src;
        //JMESH_LOGI("frd recv","src:%d\n",src);
        switch(pdu->network.control.OPCODE){
        /*case(JMESH_CONTROL_FRIEND_POLL):
            if( length == sizeof(jmesh_friend_poll_t)){
                friend_poll_handler(src,netkey_index,(jmesh_friend_poll_t*)pdu->network.control.pdu);
            }
            break;*/
        case(JMESH_CONTROL_FRIEND_REQUEST):
            if(length==sizeof(jmesh_friend_request_t)){
                features.features=0;
                features.lowpower=JMESH_FEATURE_ENABLE;
                jmesh_routing_neighbor_add(src,pdu->adv.mac,pdu->adv.rssi,features.features);//neighbor is low power
                friend_request_handler(src,netkey_index,(jmesh_friend_request_t*)pdu->network.control.pdu);
            }
            break;
        case(JMESH_CONTROL_FRIEND_CLEAR):
            if(length==sizeof(jmesh_friend_clear_t)){
                friend_clear_handler(src,netkey_index,(jmesh_friend_clear_t*)pdu->network.control.pdu);
            }
            break;
        case(JMESH_CONTROL_FRIEND_CLEAR_CONFIRM):
            if(length==sizeof(jmesh_friend_clear_confirm_t)){
                friend_clear_confirm_handler(src,netkey_index,(jmesh_friend_clear_confirm_t*)pdu->network.control.pdu);
            }
            break;
        case(JMESH_CONTROL_FRIEND_SUBSCRIPTION_LIST_ADD):
            if(length >= 3 && (length%2)==1){
                friend_subscription_list_add_handler(src,length,(jmesh_friend_subscription_list_add_t*)pdu->network.control.pdu);
            }
            break;
        case(JMESH_CONTROL_FRIEND_SUBSCRIPTION_LIST_REMOVE):
            if(length >= 3 && (length%2)==1){
                friend_subscription_list_remove_handler(src,length,(jmesh_friend_subscription_list_remove_t*)pdu->network.control.pdu);
            }
            break;
        }
    }
    return 0;
}

int jmesh_friend_send(unsigned short dst,jmesh_pdu_t* pdu){
    jmesh_gatt_t* gatt;
    gatt=jmesh_gatt_get_link_by_addr(dst);
    if(NULL==gatt){
        return -1;
    }
    jmesh_proxy_send(JMESH_SEND_TYPE_CONNECT,gatt->id,0xff,JMESH_PROXY_TYPE_NETWORK,pdu->length,pdu->pdu+2);
    jmesh_pdu_free(pdu);
    return 0;
}



