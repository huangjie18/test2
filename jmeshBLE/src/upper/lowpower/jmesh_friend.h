/**
 *  low power node                                  friend node
    request            ---------------------->
                       <----------------------      offer
    subscription add   ---------------------->
                       <----------------------      subscription confirm
    poll               ---------------------->
                       <----------------------      update MD=0
                       <----------------------      clear(previous addr!=local)
    sleep
    poll               ---------------------->
                       <----------------------      pdu
    poll               ---------------------->
                       <----------------------      update MD=0
    sleep
    ...
 *
 */
 //new establish flow
 /**
 *  low power node                                  friend node
    request            ---------------------->
                       <----------------------      offer
    subscription add   ---------------------->
                       <----------------------      subscription confirm
    poll               ---------------------->
                       <----------------------      connect
    connected          <--------------------->      connected
                       <----------------------      update MD=0
                       <----------------------      clear(previous addr!=local)
    sleep
    ...

    poll               ---------------------->
                       <----------------------      connect
    connected          <--------------------->      connected
                       <----------------------      pdu
                                                    ...
                       <----------------------      pdu
                       <----------------------      update MD=0
    sleep
    ...
 *
 */


#ifndef JMESH_FRIEND_H
#define JMESH_FRIEND_H
#include"../../jmesh/jmesh_config.h"
#include"../../jmesh/jmesh_types.h"
#include"../../jmesh/jmesh_pdu.h"
#include"../../onchip_system/os_timer_event.h"
#include"../../bearer/routing/jmesh_routing_beacon.h"
#include"../../bearer/gatt/jmesh_gatt.h"

#define JMESH_FRIEND_OFFER_TIMEOUT_MS 4000
//#define JMESH_FRIEND_CONNECT_AFTER_POLL_RETRY_TIMES 5
#define JMESH_FRIEND_LOWPOWER_LINK_TIMEOUT_S 4

//typedef struct st_jmesh_friend_poll{
//    //unsigned char FSN:1;
//    //unsigned char padding:7;
//    unsigned char valid;
//    unsigned char poll_fail_counter;
//    unsigned char cache_not_empty;
//}jmesh_friend_poll_t;



typedef struct st_jmesh_friend_update{
    unsigned char key_refresh_flag:1;
    unsigned char IV_update_flag:1;
    unsigned char reserved:6;
    unsigned char iv_index[4];
    unsigned char MD;
}jmesh_friend_update_t;

typedef struct st_jmesh_friend_request{
    unsigned char RFU:1;
    unsigned char RSSI_factor:2;
    unsigned char receive_window_factor:2;
    unsigned char min_queue_size_log:3;

    unsigned char receive_delay;
    unsigned char poll_interval[2];
    unsigned char poll_retry_times;
    unsigned char previous_address[2];
    unsigned char element_num;
    unsigned char LPN_counter[2];
}jmesh_friend_request_t;

typedef struct st_jmesh_friend_offer{
    unsigned char receive_window;
    unsigned char queue_size;
    unsigned char subscription_list_size;
    unsigned char rssi;
    unsigned char friend_counter[2];
}jmesh_friend_offer_t;

typedef struct st_jmesh_friend_clear{
    unsigned char LPN_addr[2];
    unsigned char LPN_counter[2];
}jmesh_friend_clear_t;

typedef struct st_jmesh_friend_clear_confirm{
    unsigned char LPN_addr[2];
    unsigned char LPN_counter[2];
}jmesh_friend_clear_confirm_t;

typedef struct st_jmesh_friend_subscription_list_add{
    unsigned char transaction_number;
    unsigned char address_list[1];
}jmesh_friend_subscription_list_add_t;

typedef struct st_jmesh_friend_subscription_list_remove{
    unsigned char transaction_number;
    unsigned char address_list[1];
}jmesh_friend_subscription_list_remove_t;

typedef struct st_jmesh_friend_subscription_list_confirm{
    unsigned char transaction_number;
}jmesh_friend_subscription_list_confirm_t;


typedef struct st_jmesh_friend_pdu{
    union{
        jmesh_friend_poll_t                         poll;
        jmesh_friend_update_t                       update;
        jmesh_friend_request_t                      request;
        jmesh_friend_offer_t                        offer;
        jmesh_friend_clear_t                        clear;
        jmesh_friend_clear_confirm_t                clear_confirm;
        jmesh_friend_subscription_list_add_t        subscription_list_add;
        jmesh_friend_subscription_list_remove_t     subscription_list_remove;
        jmesh_friend_subscription_list_confirm_t    subscription_list_confirm;
    };
}jmesh_friend_pdu_t;



typedef struct st_jmesh_lowpower_para{
    unsigned short poll_timeout;
}jmesh_lowpower_para_t;

void jmesh_friendship_init(void);
int jmesh_friend_recv(unsigned short netkey_index,unsigned short length,jmesh_pdu_t* pdu);
int jmesh_friend_send(unsigned short dst,jmesh_pdu_t* pdu);

void jmesh_friend_connected(unsigned short addr);

int jmesh_friend_pdu_cache(unsigned short dst,jmesh_pdu_t* pdu);

void friend_poll_handler(unsigned short src,unsigned short dst,unsigned short netkey_index,jmesh_friend_poll_t* poll);
#endif // JMESH_FRIEND_H
