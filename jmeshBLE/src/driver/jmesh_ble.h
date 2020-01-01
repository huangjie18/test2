#ifndef JMESH_BLE_H
#define JMESH_BLE_H
#include"../driver/jmesh_system.h"
#include"os_timer_event.h"

#define GATT_DEFAULT_MTU 20

enum{
    JMESH_BLE_STATUS_INIT,
    JMESH_BLE_STATUS_SCAN,
    JMESH_BLE_STATUS_ADV,
    JMESH_BLE_STATUS_CONNECT,
    JMESH_BLE_STATUS_BUSY,
    JMESH_BLE_STATUS_SLEEP,
};

typedef struct st_send_cache
{
    unsigned char send_type;
    unsigned char id;
    unsigned char backup_id;
    unsigned short instance;
    unsigned short length;
    unsigned char data[JMESH_BLE_MTU_SIZE];
}send_cache_t;


typedef struct st_jmesh_ble_adapter{
    unsigned char status;
    os_timer_event_t timer;
    struct{
        unsigned char mac[6];
        unsigned char flag;
    }connect;
    struct{
        unsigned char repeats;
        unsigned char ad_type;
        unsigned char length;
        unsigned char ad_data[JMESH_ADV_CACHE_SIZE];
        SemaphoreHandle_t mutex_lock;
    }adv;
    struct{
        unsigned char is_busy;
        unsigned char this_id;//1-unicast id,2-unicast backup_id,3-broadcast id
        send_cache_t send_cache;
    }gatt;
}jmesh_ble_adapter_t;

#define SEND_CACHE_DATA_OFFSET ((int)(((send_cache_t *)0)->data))

enum
{
    JMESH_SEND_TYPE_CONNECT,
    JMESH_SEND_TYPE_ROUTE,
    JMESH_SEND_TYPE_ALL,
};

#define ADV_MODE_NON_CONN 0
#define ADV_MODE_CONN 1
#define ADV_MODE_CREATE_CONN 2

#define JMESH_ADV_RECV_INTERVAL_MS 100

#define GATT_CON_ACTIVE_MS 50000

extern SemaphoreHandle_t  ble_aec_enc_xSemaphore;
extern void jmesh_ble_init(void);
extern void jmesh_ble_status_start(void);
extern int jmesh_ble_status(void);
extern void jmesh_ble_reset(void);
extern void jmesh_ble_platform_reset(void);
extern void jmesh_ble_scan_adv(void);
extern void jmesh_ble_send_adv(unsigned char ad_type,unsigned char length,unsigned char *data);
extern void jmesh_ble_send_ota_adv(void);//ota
extern void jmesh_ble_gatt_connect(unsigned char *mac);
extern void jmesh_ble_gatt_disconnect(unsigned char connect_id);

extern void jmesh_ble_disc_svc(unsigned char *uuid,unsigned char connect_id);
extern void jmesh_ble_set_local_svc_ins(unsigned short instance);
extern unsigned short jmesh_ble_get_local_svc_ins(void);
extern void jmesh_ble_exchange_mtu(unsigned char connect_id);

extern void jmesh_ble_gatt_write(unsigned char connect_id, unsigned short char_handler, unsigned short length,unsigned char *data);
extern void jmesh_ble_gatt_notify(unsigned char connect_id, unsigned short char_handler, unsigned short length,unsigned char *data);

void jmesh_adv_send(unsigned char ad_type,unsigned char length,unsigned char* data,unsigned char repeats);
void jmesh_ble_status_switch(void);
void jmesh_ble_stop_cancel(void);
void jmesh_connect_start(unsigned char* mac);
void jmesh_ble_canceled_callback(unsigned char state_canceled);
void jmesh_ble_cancel_callback(unsigned char success);
void jmesh_adapter_gatt_send(unsigned char send_type,unsigned char id,unsigned char id_backup,unsigned short instance,unsigned short length,unsigned char *data);
int jmesh_gatt_queue_send(unsigned char success);

void jmesh_ble_scan_enable(void);
void jmesh_ble_scan_disable(void);

int jmesh_ble_set_dbm(unsigned char dbm);
unsigned char jmesh_ble_get_dbm(void);
#endif // JMESH_BLE_H
