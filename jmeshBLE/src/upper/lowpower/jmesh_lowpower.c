#include"jmesh_friend.h"
#include"jmesh_lowpower.h"
#include"../../onchip_system/os_timer_event.h"
#include"../../jmesh/jmesh_addr.h"
#include"../../jmesh/jmesh_config.h"
#include"../../jmesh/jmesh_print.h"
#include"../../jmesh/jmesh_features.h"
#include"../../network/jmesh_interface.h"
#include"../../network/jmesh_netkey.h"
#include"../control/jmesh_control.h"
#include"../../access/jmesh_access.h"
#include"../../jmesh/jmesh_print.h"
#include"../../app/expand/app_expand2/water/water.h"
#include"../../jmesh/jmesh_save.h"

static struct st_jmesh_lowpower_friendship{
    unsigned short addr;

    unsigned char receive_window;
    unsigned char FSN:1;
    unsigned char established_flag:1;
    unsigned char poll_fail_counter:6;
    unsigned short friend_counter;
    os_timer_event_t timer;
    unsigned char cache_head;
    unsigned char cache_tail;
    jmesh_pdu_t* cache[JMESH_FRIEND_CACHE_QUEUE_SIZE];
}lowpower_friendship;

lowpower_param_t lowpower_param={
    .adv_repeats=JMESH_LOWPOWER_ADV_REPEATS,
    .poll_intval_s=JMESH_LOWPOWER_POLL_INTERVAL_S,
    .reqst_intval_s=JMESH_LOWPOWER_REQST_INTERVAL_S,
    .poll_retry_times=JMESH_LOWPOWER_POLL_RETRY_TIMES
};

static unsigned short LPN_counter;
static unsigned char lowpower_transaction_number;


static jmesh_friend_offer_t friendship_offer;
static unsigned short friendship_offer_src;
static unsigned char friendship_established_flag=0;
//static unsigned char poll_fail_counter;
//static os_timer_event_t lowpower_timer;

static unsigned short best_offer_src;

static void lowpower_request(void);
static void lowpower_poll(void);
static void lowpower_subscription_list_update(void);

void jmesh_lowpower_init(void){
    lowpower_param_t lowp_para_tmep;
    if(JMESH_FEATURE_ENABLE!=jmesh_get_lowpower_feature()){
        return;
    }
    memset(&lowpower_friendship,0,sizeof(lowpower_friendship));
    LPN_counter=0;
    lowpower_transaction_number=0;
    if(sizeof(lowpower_param_t)==jmesh_save_read(JMESH_SAVE_LOWPOWER_PARAM,sizeof(lowpower_param_t),(unsigned char *)&lowp_para_tmep)){
        lowpower_param=lowp_para_tmep;
    }
    os_timer_event_set(&lowpower_friendship.timer,lowpower_param.reqst_intval_s*1000UL,(os_timer_event_caller_t)lowpower_request,NULL);
}

void jmesh_lowpower_uninit(void)
{
    //if(JMESH_FEATURE_ENABLE==jmesh_get_lowpower_feature()){
        os_timer_event_remove(&lowpower_friendship.timer);
    //}
}
void jmesh_lowpower_sleep(void)
{
    jmesh_gatt_t* gatt;
    //JMESH_LOGI("sleep","addr:%d\n",lowpower_friendship.addr);
    gatt=jmesh_gatt_get_link_by_addr(lowpower_friendship.addr);
    jmesh_gatt_disconnect(gatt);
    os_timer_event_set(&lowpower_friendship.timer,1000*jmesh_random_get(lowpower_param.poll_intval_s-JMESH_LOWPOWER_POLL_INTVAL_OFFSET_S,lowpower_param.poll_intval_s+JMESH_LOWPOWER_POLL_INTVAL_OFFSET_S+1),(os_timer_event_caller_t)lowpower_poll,NULL);
}

int jmesh_lowpower_cache_push(jmesh_pdu_t* pdu){
    //JMESH_LOGI("lowpower push","ca num:%d\n",lowpower_friendship.cache_num);
    if(0==lowpower_friendship.addr){
        return -1;
    }
    if(lowpower_friendship.cache_head==(lowpower_friendship.cache_tail+1)%JMESH_FRIEND_CACHE_QUEUE_SIZE){
        jmesh_pdu_free(lowpower_friendship.cache[lowpower_friendship.cache_head]);//cache full,free and overwrite eldest cache
        lowpower_friendship.cache_head=(lowpower_friendship.cache_head+1)%JMESH_FRIEND_CACHE_QUEUE_SIZE;
    }
    lowpower_friendship.cache[lowpower_friendship.cache_tail]=pdu;
    lowpower_friendship.cache_tail=(lowpower_friendship.cache_tail+1)%JMESH_FRIEND_CACHE_QUEUE_SIZE;
    return 0;
}

jmesh_pdu_t* jmesh_lowpower_cache_pop(void){
    jmesh_pdu_t* pdu=NULL;
    if(lowpower_friendship.cache_tail!=lowpower_friendship.cache_head){
        pdu=lowpower_friendship.cache[lowpower_friendship.cache_head];
        lowpower_friendship.cache_head=(lowpower_friendship.cache_head+1)%JMESH_FRIEND_CACHE_QUEUE_SIZE;
    }
    return pdu;
}

void jmesh_lowpower_cache_free(void)
{
    jmesh_pdu_t *pdu=NULL;
    while(NULL!=(pdu=jmesh_lowpower_cache_pop())){
        jmesh_pdu_free(pdu);
    }
}



void jmesh_lowpower_send(unsigned short addr)
{
    jmesh_pdu_t* pdu;
    jmesh_gatt_t* gatt;
    int i=-1;
    gatt=jmesh_gatt_get_link_by_addr(addr);
    if(NULL==gatt){
        return;
    }
    while(NULL!=(pdu=jmesh_lowpower_cache_pop())){
        i++;
        JMESH_LOGI("lowpower send:","%d\n",i);
        jmesh_proxy_send(JMESH_SEND_TYPE_CONNECT,gatt->id,0xff,JMESH_PROXY_TYPE_NETWORK,pdu->length,pdu->pdu+2);
        jmesh_pdu_free(pdu);
    }
}


void jmesh_lowpower_connected_callback(void)
{
    jmesh_lowpower_send(lowpower_friendship.addr);
    lowpower_subscription_list_update();
}
void jmesh_lowpower_connected(unsigned short src){
    if(JMESH_FEATURE_ENABLE!=jmesh_get_lowpower_feature()){
        return;
    }
    if(src==lowpower_friendship.addr){
        //if(friendship_established_flag==0){
            os_timer_event_set(&lowpower_friendship.timer,3000,(os_timer_event_caller_t)jmesh_lowpower_connected_callback,NULL);
            //jmesh_lowpower_connected_callback();
            //lowpower_subscription_list_update();
        //}
    }
}

void jmesh_lowpower_wakeup_callback(void){
    if(friendship_established_flag==0){
        lowpower_request();
    }
    else{
        lowpower_poll();
    }
}
void jmesh_lowpower_recv_pdu_callback(void){
    lowpower_friendship.poll_fail_counter=0;
    lowpower_friendship.FSN++;
    lowpower_poll();
}
void lowpower_choose_offer(void)
{
    JMESH_LOGI("chose offer","offer src:%d\n",friendship_offer_src);
    jmesh_ble_scan_disable();
    if(friendship_offer_src!=0){
        //jmesh_connect(best_offer_src);
        //jmesh_connect_start(NULL);
        lowpower_friendship.addr=friendship_offer_src;
        jmesh_secure_beacon_stop();
        lowpower_poll();
    }
    else{
        //request if no offer received,not connected,not established...
        //lowpower_request();
        os_timer_event_set(&lowpower_friendship.timer,(/*LPN_counter*JMESH_LOWPOWER_REQST_INCREAMENT_S+*/lowpower_param.reqst_intval_s)*1000UL,(os_timer_event_caller_t)lowpower_request,NULL);// connected will reset timer to poll
    }
}

void lowpower_receive_offer(void){
    JMESH_LOGI("rev offer","\n");
    jmesh_ble_scan_enable();
    best_offer_src=0;
    os_timer_event_set(&lowpower_friendship.timer,JMESH_LOWPOWER_SCAN_ENABLE_MS,(os_timer_event_caller_t)lowpower_choose_offer,NULL);//receive delay 100ms,receive window 1000ms
}
static void lowpower_request(void){
    jmesh_friend_request_t request;

    request.RSSI_factor=JMESH_FRIEND_RSSI_FACTOR;
    request.receive_window_factor=JMESH_FRIEND_RECEIVE_WINDOW_FACTOR;
    request.min_queue_size_log=JMESH_FRIEND_MIN_QUEUE_SIZE_LOG;
    request.receive_delay=JMESH_FRIEND_RECEIVE_DELAY;
    JMESH_BIG_ENDIAN_FILL2(lowpower_param.poll_intval_s,request.poll_interval);
    JMESH_BIG_ENDIAN_FILL2(lowpower_friendship.addr,request.previous_address);//previous addr
    lowpower_friendship.addr=0;
    request.element_num=jmesh_element_get_num();
    JMESH_BIG_ENDIAN_FILL2(LPN_counter,request.LPN_counter);
    request.poll_retry_times=lowpower_param.poll_retry_times;
    LPN_counter++;//LPNCounter ++
    lowpower_friendship.poll_fail_counter=0;
    lowpower_friendship.established_flag=0;
    jmesh_lowpower_cache_free();
    //os_timer_event_remove(&lowpower_friendship.timer);

    //memset(&lowpower_friendship,0,sizeof(lowpower_friendship));
    JMESH_LOGI("req","LPN:%d\n",LPN_counter);
    os_timer_event_set(&lowpower_friendship.timer,JMESH_LOWPOWER_RECV_OFFER_DELAY_MS,(os_timer_event_caller_t)lowpower_receive_offer,NULL);//receive delay 100ms,receive window 1000ms
    jmesh_control_send(jmesh_get_primary_addr(),
                       JMESH_ADDR_ALL_FRIENDS,//fix all friends address
                       0,//now only support 0
                       JMESH_CONTROL_FRIEND_REQUEST,
                       1,//fix 0
                       sizeof(jmesh_friend_request_t),
                       (unsigned char*)&request);
    jmesh_secure_beacon_start();
}
static void lowpower_poll_timeout_handler(void){
    jmesh_gatt_t* gatt;
    lowpower_friendship.poll_fail_counter++;
    JMESH_LOGI("poll timeout","fail:%d\n",lowpower_friendship.poll_fail_counter);

    if(lowpower_friendship.poll_fail_counter<=lowpower_param.poll_retry_times){
        jmesh_lowpower_sleep();
    }
    else{
        gatt=jmesh_gatt_get_link_by_addr(lowpower_friendship.addr);
        jmesh_gatt_disconnect(gatt);
        lowpower_request();
    }
}
static void lowpower_poll(void){
    jmesh_friend_poll_t poll;
    JMESH_LOGI("poll","addr:%d\n",lowpower_friendship.addr);
    os_timer_event_set(&lowpower_friendship.timer,/*JMESH_FRIEND_RECEIVE_DELAY+lowpower_friendship.receive_window+*/JMESH_LOWPOWER_POLL_TIMEOUT_MS,(os_timer_event_caller_t)lowpower_poll_timeout_handler,NULL);
    if(lowpower_friendship.addr==0/*||lowpower_friendship.established_flag==1*/){
        JMESH_LOGI("poll","poll cancel without friendship\n");
        lowpower_request();
        return ;
    }
    //poll.FSN=lowpower_friendship.FSN;
    poll.valid=1;
    poll.poll_fail_counter=lowpower_friendship.poll_fail_counter;
    poll.cache_not_empty=(lowpower_friendship.cache_tail!=lowpower_friendship.cache_head)?1:0;
    /*jmesh_control_send(jmesh_get_primary_addr(),
                       lowpower_friendship.addr,
                       0,
                       JMESH_CONTROL_FRIEND_POLL,
                       1,//fix 0
                       sizeof(jmesh_friend_poll_t),
                       (unsigned char*)&poll);*/
    jmesh_routing_beacon_link_require(lowpower_friendship.addr,&poll);
}
void jmesh_lowpower_subscription_list_add(unsigned short addr){
    unsigned char buffer[10];
    int i;
    jmesh_filter_t* filter=jmesh_interface_get_filter(JMESH_INTERFACE_LOCAL);

    jmesh_friend_subscription_list_add_t *add=(jmesh_friend_subscription_list_add_t*)buffer;

    add->transaction_number=filter->num;
    for(i=0;i<add->transaction_number;i++){
        JMESH_BIG_ENDIAN_FILL2(filter->addr[i],add->address_list+i*2);
    }
    jmesh_control_send(jmesh_get_primary_addr(),
                       best_offer_src,
                       0/*lowpower_friendship.netkey_index*/,
                       JMESH_CONTROL_FRIEND_SUBSCRIPTION_LIST_ADD,
                       1,//fix 0
                       add->transaction_number*2+1,
                       (unsigned char*)add);
}
void jmesh_lowpower_subscription_list_remove(unsigned short addr){
    unsigned char buffer[3];
    jmesh_friend_subscription_list_remove_t *remove=(jmesh_friend_subscription_list_remove_t*)buffer;
    if(lowpower_friendship.established_flag==0){
        return;
    }
    remove->transaction_number=lowpower_transaction_number;
    JMESH_BIG_ENDIAN_FILL2(addr,remove->address_list);
    jmesh_control_send(jmesh_get_primary_addr(),
                       lowpower_friendship.addr,
                       0/*lowpower_friendship.netkey_index*/,
                       JMESH_CONTROL_FRIEND_SUBSCRIPTION_LIST_ADD,
                       1,//fix 0
                       3,
                       (unsigned char*)remove);
}
static void lowpower_subscription_list_update_timeout_handler(void){
    jmesh_gatt_t* gatt;
    JMESH_LOGI("list up timeout","\n");
    gatt=jmesh_gatt_get_link_by_addr(lowpower_friendship.addr);
    jmesh_gatt_disconnect(gatt);
    lowpower_request();
/*disconnect();

          os_timer_event_set(&lowpower_timer,JMESH_FRIEND_POLL_TIMEOUT*100,(os_timer_event_caller_t)lowpower_request,NULL);

    if(poll_fail_counter<3){
        //lowpower_subscription_list_update();
    }
    else{
        //¶¨Ê±Æ÷request
        lowpower_request();
    */
}


static void lowpower_subscription_list_update(void){

    jmesh_friend_subscription_list_add_t *add;
    jmesh_filter_t* filter;
    unsigned char buffer[JMESH_FILTER_SIZE*2+1];
    int i;

    filter=jmesh_interface_get_filter(JMESH_INTERFACE_LOCAL);
    add=(jmesh_friend_subscription_list_add_t*)buffer;
    add->transaction_number=lowpower_transaction_number;
    for(i=0;i<filter->num;i++){
        //JMESH_LOGI("list up","addr:0x%x\n",filter->addr[i]);
        JMESH_BIG_ENDIAN_FILL2(filter->addr[i],add->address_list+i*2);
    }
    //JMESH_LOGI_BUFFER("up list:",1+filter->num*2,buffer,"\n");
    JMESH_LOGI("list up","dst:%d\n",lowpower_friendship.addr);
    os_timer_event_set(&lowpower_friendship.timer,JMESH_LOWPOWER_SUBS_LISTUP_TIMEOUT_MS,(os_timer_event_caller_t)lowpower_subscription_list_update_timeout_handler,NULL);
    jmesh_control_send(jmesh_get_primary_addr(),
                       lowpower_friendship.addr,
                       0,
                       JMESH_CONTROL_FRIEND_SUBSCRIPTION_LIST_ADD,
                       1,//fix 0
                       1+filter->num*2,
                       (unsigned char*)add);
}

static void lowpower_update_timeout_handler(void){
    jmesh_gatt_t* gatt;
    JMESH_LOGI("update timeout","\n");
    gatt=jmesh_gatt_get_link_by_addr(lowpower_friendship.addr);
    jmesh_gatt_disconnect(gatt);
    lowpower_request();
}


static void lowpower_update_handler(unsigned short src,unsigned short netkey_index,jmesh_friend_update_t* update){
    unsigned long iv_index;
    //jmesh_gatt_t* gatt;
    lowpower_friendship.poll_fail_counter=0;
    JMESH_BIG_ENDIAN_PICK4(iv_index,update->iv_index);
    JMESH_LOGI("up ha","src:%d\n",src);
    //poll_fail_counter=0;
    if(iv_index!=jmesh_netkey_get_iv_index(netkey_index)){
        jmesh_netkey_recover_iv_index(netkey_index,iv_index);
    }
    if(update->MD){
        //lowpower_poll();
        jmesh_lowpower_sleep();
    }
    else{
        //if(1==lowpower_friendship.established_flag){

            jmesh_lowpower_sleep();
            //jmesh_gatt_disconnect(gatt);
            //os_timer_event_set(&lowpower_timer,JMESH_FRIEND_POLL_TIMEOUT*100,(os_timer_event_caller_t)lowpower_poll,NULL);
        //}
        //else{//error
        //    JMESH_LOGI("error","no recv confim\n");
        //    gatt=jmesh_gatt_get_link_by_addr(src);
        //    jmesh_gatt_disconnect(gatt);
        //    os_timer_event_set(&lowpower_friendship.timer,JMESH_FRIEND_POLL_TIMEOUT*100,(os_timer_event_caller_t)lowpower_request,NULL);
        //}
    }
}

//choose best offer

static void lowpower_offer_handler(unsigned short src,unsigned short netkey_index,jmesh_friend_offer_t* offer){
    JMESH_LOGI("offer ha","src:%d,offer_src:%d\n",src,friendship_offer_src);
    if(friendship_offer_src==0){
        friendship_offer_src=src;
        memcpy(&friendship_offer,offer,sizeof(jmesh_friend_offer_t));
    }
    else if(friendship_offer_src!=src){
        //choose best one
        friendship_offer_src=src;
        memcpy(&friendship_offer,offer,sizeof(jmesh_friend_offer_t));
    }
    /*
    lowpower_friendship.addr=src;
    lowpower_friendship.netkey_index=netkey_index;
    lowpower_friendship.receive_window=offer->receive_window;
    JMESH_BIG_ENDIAN_PICK2(lowpower_friendship.friend_counter,offer->friend_counter);

    if(offer->subscription_list_size<jmesh_interface_get_filter_num(JMESH_INTERFACE_LOCAL)){
        print_warning("lowpower recv a offer list size =%d but need %d,drop it\n",offer->subscription_list_size,jmesh_interface_get_filter_num(JMESH_INTERFACE_LOCAL));
        return;
    }
    if(offer->queue_size<(1<<JMESH_FRIEND_MIN_QUEUE_SIZE_LOG)){//queue size
        print_warning("lowpower recv a offer queue size =%d but need %d,drop it\n",offer->queue_size,(1<<JMESH_FRIEND_MIN_QUEUE_SIZE_LOG));
        return;
    }

    friend_subscription_list_add(friendship);
    lowpower_subscription_list_update();
    */
}
static void lowpower_subscription_list_confirm_handler(unsigned short src,unsigned short netkey_index,jmesh_friend_subscription_list_confirm_t* confirm){
    if(lowpower_transaction_number==confirm->transaction_number){
        lowpower_transaction_number++;

        //lowpower_friendship.addr=src;
        //lowpower_friendship.netkey_index=netkey_index;
        lowpower_friendship.receive_window=friendship_offer.receive_window;
        JMESH_BIG_ENDIAN_PICK2(lowpower_friendship.friend_counter,friendship_offer.friend_counter);
        lowpower_friendship.established_flag=1;
        os_timer_event_set(&lowpower_friendship.timer,JMESH_LOWPOWER_UPDATE_TIMEOUT_MS,(os_timer_event_caller_t)lowpower_update_timeout_handler,NULL);
        JMESH_LOGI("list confim ha","num:%d,src:%d\n",lowpower_transaction_number,lowpower_friendship.addr);

    }
}

void jmesh_lowpower_recv(unsigned short netkey_index,unsigned short length,jmesh_pdu_t* pdu){

    unsigned short src;
    jmesh_features_t features;
    if(JMESH_FEATURE_ENABLE==jmesh_get_lowpower_feature()){
        src=pdu->control.src;
        //JMESH_LOGI("lowpower recv","src:%d\n",src);
        switch(pdu->network.control.OPCODE){
        case(JMESH_CONTROL_FRIEND_UPDATE):
            if(pdu->network.length==sizeof(jmesh_friend_update_t)){
                lowpower_update_handler(src,netkey_index,(jmesh_friend_update_t*)pdu->network.control.pdu);
            }
            break;
        case(JMESH_CONTROL_FRIEND_OFFER):
            if(pdu->network.length==sizeof(jmesh_friend_offer_t)){
                features.features=0;
                features.friend=JMESH_FEATURE_ENABLE;
                jmesh_routing_neighbor_add(src,pdu->adv.mac,pdu->adv.rssi,features.features);//neighbor is friend
                lowpower_offer_handler(src,netkey_index,(jmesh_friend_offer_t*)pdu->network.control.pdu);
            }
            break;
        case(JMESH_CONTROL_FRIEND_SUBSCRIPTION_LIST_CONFIRM):
            if(pdu->network.length==sizeof(jmesh_friend_subscription_list_confirm_t)){
                lowpower_subscription_list_confirm_handler(src,netkey_index,(jmesh_friend_subscription_list_confirm_t*)pdu->network.control.pdu);
            }
            break;
        default:break;
        }
    }
}
