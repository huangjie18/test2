#ifndef JMESHLIB_H
#define JMESHLIB_H

#define JMESH_BIND_APP_MAX_NUM 6
#define JMESH_SUBSCRIPTION_LIST_SIZE 5

typedef struct st_jmesh_model jmesh_model_t;
typedef struct st_jmesh_element jmesh_element_t;
typedef struct st_jmesh_model_info jmesh_model_info_t;
typedef void(*jmesh_message_handler_t)(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
typedef unsigned char jmesh_message_t[3];


typedef struct st_jmesh_model_bind{
    unsigned char index[JMESH_BIND_APP_MAX_NUM];
}jmesh_model_bind_t;

typedef struct st_jmesh_subscription_list{
    unsigned short addr[JMESH_SUBSCRIPTION_LIST_SIZE];
}jmesh_subscription_list_t;

typedef struct st_jmesh_model_para{
    jmesh_model_bind_t bind_app;
    jmesh_subscription_list_t subscription_list;
}jmesh_model_para_t;

struct st_jmesh_model{
    jmesh_model_t* next;
    const jmesh_model_info_t* info;
    jmesh_model_para_t para;
    jmesh_element_t* element;
    unsigned char id;
};
struct st_jmesh_element{
    jmesh_element_t* next;
    jmesh_model_t* model_list;
};
typedef struct st_jmesh_model_id
{
    union
	{
        unsigned short company_id;
        unsigned short sig_id;
    };
    unsigned short model_id;
}jmesh_model_id_t;

struct st_jmesh_model_info
{
    jmesh_model_id_t model_id;
    unsigned char message_num;
    struct
	{
        jmesh_message_t message;
        jmesh_message_handler_t handler;
    }
    message[1];
};

#define JMESH_MAC_SIZE 6
typedef unsigned char jmesh_mac_t[JMESH_MAC_SIZE];

typedef struct st_jmesh_appkey{
    unsigned long netkey_index:12;
    unsigned long appkey_index:12;
    unsigned long aid:7;
    unsigned long is_valid:1;
    unsigned char key[16];
}jmesh_appkey_t;

#define JMESH_ROUTING_CONNECT_MAX_NUM 6

typedef struct st_jmesh_routing_neighbor{

    unsigned short addr;
//    unsigned char server_flag:1;
    unsigned char mac[6];
//    unsigned short subnet;
//    unsigned char element_num:4;
//    unsigned char net_distance:4;
//    unsigned char net_size;
    unsigned char rssi;
//    unsigned char interface;
    unsigned char try_con_count;

    //unsigned short link_node[JMESH_ROUTING_CONNECT_MAX_NUM];
    unsigned short lifetime;
    unsigned short intv_mask;
}jmesh_routing_neighbor_t;

typedef struct st_jmesh_gatt{
    unsigned char mac[6];
    unsigned char id;
    unsigned short addr;
    union{
        unsigned char flag;
        struct{
            unsigned char is_linked:1;
            unsigned char is_ready:1;
            unsigned char is_server:1;
            unsigned char intv_pos:5;//connect interval table position
        };
    };
    unsigned short mtu;
    unsigned short svc_instance;
    unsigned short timer;
}jmesh_gatt_t;

typedef enum {
	PIN_NUM_0,
	PIN_NUM_1,
    PIN_NUM_2,
	PIN_NUM_3,
	PIN_NUM_4,
	PIN_NUM_5,
}jmesh_pwm_pin_num_t;

typedef enum {
    JMESH_PWM_CHANNEL_0,
    JMESH_PWM_CHANNEL_1,
    JMESH_PWM_CHANNEL_2,
    JMESH_PWM_CHANNEL_3,
    JMESH_PWM_CHANNEL_4,
}jmesh_pwm_channel_t;

typedef enum {
    JMESH_LED1,
    JMESH_LED2,
    JMESH_LED_GREEN,
    JMESH_LED_RED,
    JMESH_OUTPUT_5,
    JMESH_INPUT_6,
		POWER_LIGHT,
} JMESH_GPIO_NAME_T;

typedef enum 
{
    IO_PULL_DISABLE,
    IO_PULL_UP,
    IO_PULL_DOWN,    
}io_pull_type_t;

//reset
void jmesh_ble_platform_reset(void);
//baud
int jmesh_uart_baudrate_set(unsigned char enum_baud,unsigned char parity);
//address
void jmesh_mac_set(jmesh_mac_t mac);
void jmesh_mac_get(jmesh_mac_t mac);
int jmesh_set_primary_addr(unsigned short primary_addr);
unsigned short jmesh_get_primary_addr(void);
//key
int jmesh_netkey_state_set(unsigned short index,unsigned char* key);
int jmesh_netkey_state_get(unsigned short index,unsigned char* key);
int jmesh_appkey_set(unsigned short netkey_index,unsigned short appkey_index,unsigned char *key);
int jmesh_appkey_list(unsigned short netkey_index,unsigned char pdu_len,unsigned char* pdu);
//model bind
int jmesh_model_bind_app(jmesh_model_t* model,jmesh_appkey_t* appkey);
int jmesh_model_unbind_app(jmesh_model_t* model,jmesh_appkey_t* appkey);
int jmesh_model_bind_list(jmesh_model_t* model,unsigned char buffer_size,unsigned char* buffer);
//subscribe
int jmesh_model_subscription_list_add(jmesh_model_t* model,unsigned short addr);
int jmesh_model_subscription_list_remove(jmesh_model_t* model,unsigned short addr);
int jmesh_model_subscription_list(jmesh_model_t* model,unsigned char* ret_buffer);
//trans
void jmesh_app_expand2_transparent_client_trans(unsigned short dst,unsigned char port,unsigned char len,unsigned char* data);
void jmesh_app_expand2_realcomm_client_trans(unsigned short dst,unsigned char port,unsigned char len,unsigned char* data);
void app_realcomm_server_transparent_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void app_realcomm_server_trans_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
//get_neighbor
int jmesh_routing_get_neighbor_link_num(jmesh_routing_neighbor_t *neighbor);
//get_connected
jmesh_gatt_t* jmesh_gatt_get_link_by_interface(unsigned char interface);
//iv_index
int jmesh_netkey_test_set_iv_index(unsigned short netkey_index,unsigned long iv_index);
unsigned long jmesh_netkey_get_iv_index(unsigned short netkey_index);
int jmesh_netkey_test_start_update_iv_index(unsigned short netkey_index);
int jmesh_netkey_test_stop_update_iv_index(unsigned short netkey_index);
//light mode
void light_client_mode_set(unsigned short dst,unsigned char id,unsigned short speed_ticks,unsigned short on_ticks,unsigned short off_ticks,unsigned short live_ticks,unsigned char direction);
void light_client_mode(unsigned short dst,unsigned char id);
//feature
int jmesh_set_relay_feature(unsigned char feature);
int jmesh_set_proxy_feature(unsigned char feature);
int jmesh_set_friend_feature(unsigned char feature);
int jmesh_set_lowpower_feature(unsigned char feature);
unsigned char jmesh_get_relay_feature(void);
unsigned char jmesh_get_proxy_feature(void);
unsigned char jmesh_get_friend_feature(void);
unsigned char jmesh_get_lowpower_feature(void);
//dbm
int jmesh_ble_set_dbm(unsigned char dbm);
unsigned char jmesh_ble_get_dbm(void);
//pwm
void jmesh_pwm_start(jmesh_pwm_pin_num_t pin_num,jmesh_pwm_channel_t channel, unsigned short high_ms, unsigned short low_ms);
void jmesh_pwm_stop(jmesh_pwm_channel_t channel);
int jmesh_pwm_list(unsigned char *pin_num,jmesh_pwm_channel_t index, unsigned short *high_ms, unsigned short *low_ms,unsigned short *channel);
//io
void jmesh_gpio_set(JMESH_GPIO_NAME_T gpio, int value);
int jmesh_gpio_get(JMESH_GPIO_NAME_T gpio);
void jmesh_gpio_pull_write(JMESH_GPIO_NAME_T gpio, io_pull_type_t pull);
//handler
void config_client_init(void);
void config_appkey_list_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_appkey_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_beacon_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_composition_data_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_default_ttl_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_gatt_proxy_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_friend_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_relay_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_model_publication_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_model_subscription_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_sig_model_subscription_list_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_vendor_model_subscription_list_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_vendor_model_subscription_list_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_netkey_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_netkey_list_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_model_app_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_sig_model_app_list_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_vendor_model_app_list_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_node_identity_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_node_reset_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_key_refresh_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_heartbeat_publication_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_heartbeat_publication_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_heartbeat_subscription_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);
void config_network_transmit_status_handler(unsigned short src,unsigned short dst,unsigned short nid,unsigned short aid,unsigned short len,unsigned char* data);



#endif 