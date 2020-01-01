#ifndef JMESH_GATT_H
#define JMESH_GATT_H

#include"../../onchip_system/os_timer_event.h"
#include"jmesh_routing_neighbor.h"
#include"../../driver/jmesh_ble.h"



#define JMESH_GATT_ID_INVALID       0xff
#define JMESH_GATT_CONNECT_S        5
#define JMESH_GATT_ACTIVE_S         10
#define JMESH_GATT_SIZE             10

#define JMESH_GATT_CONNECT_LIMIT 6
#define JMESH_GATT_CONNECT_INTERVAL_S 5
#define JMESH_ROUTE_REMOVE_INTERVAL_S 60


#define JMESH_SVC_UUID {0xfb,0x34,0x9b,0x5f,0x80,0x00,0x00,0x80,0x00,0x10,0x00,0x00,0x03,0xff,0x00,0x00}

typedef struct st_jmesh_pdu jmesh_pdu_t;

typedef struct st_jmesh_gatt{
    unsigned char mac[6];
    unsigned char id;
    unsigned char features;
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

typedef struct st_gatt_phone
{
    unsigned char id;
    unsigned char mac[6];
    unsigned short mtu;
}gatt_phone_t;

void jmesh_gatt_init(void);
void jmesh_gatt_disconnect(jmesh_gatt_t* gatt);
void jmesh_gatt_disconnect_all(void);
int jmesh_gatt_non_lowp_link_num(void);
int jmesh_gatt_total_link_num(void);
int jmesh_gatt_ready_num(void);

jmesh_gatt_t* jmesh_gatt_get_link_by_mac(unsigned char* mac);
jmesh_gatt_t* jmesh_gatt_get_ready_by_mac(unsigned char* mac);
jmesh_gatt_t* jmesh_gatt_get_link_by_id(unsigned char id);
jmesh_gatt_t* jmesh_gatt_get_ready_by_id(unsigned char id);
jmesh_gatt_t* jmesh_gatt_get_ready_by_interface(unsigned char interface);
jmesh_gatt_t* jmesh_gatt_get_link_by_interface(unsigned char interface);
jmesh_gatt_t* jmesh_gatt_get_ready_by_addr(unsigned short addr);
jmesh_gatt_t* jmesh_gatt_get_link_by_addr(unsigned short addr);
unsigned char jmesh_gatt_get_interface(jmesh_gatt_t* gatt);
jmesh_gatt_t* jmesh_gatt_link_next(jmesh_gatt_t* gatt);
int jmesh_gatt_is_ready(jmesh_gatt_t* gatt);
int jmesh_gatt_is_link(jmesh_gatt_t* gatt);

void jmesh_gatt_features_update(unsigned char *mac,unsigned char features);
void jmesh_gatt_connected(unsigned char* mac,unsigned char id,unsigned short con_intv,unsigned char is_server);
void jmesh_gatt_disconnected(unsigned char connect_id,unsigned char reason);
void jmesh_gatt_set_mtu(unsigned char connect_id, unsigned short mtu);
void jmesh_gatt_set_svc_instance(unsigned char connect_id,unsigned short instance);
void jmesh_gatt_recv(unsigned char connect_id,unsigned short instance,unsigned short length,unsigned char* data);

unsigned short jmesh_gatt_get_intv_mask(void);
unsigned short jmesh_gatt_get_null_intv(unsigned char *mac);
unsigned char jmesh_gatt_get_intv_pos(unsigned short intv_param);

int jmesh_gatt_trigger_send(unsigned char id,unsigned short instance,unsigned short length,unsigned char *data);

int jmesh_gatt_is_mesh_device(unsigned char *mac);
void jmesh_gatt_phone_connected(unsigned char *mac,unsigned char id);
void jmesh_gatt_phone_disconnected(unsigned char id,unsigned char reason);
gatt_phone_t *jmesh_gatt_get_phone(unsigned char id);
void jmesh_gatt_phone_set_mtu(unsigned char id, unsigned short mtu);

unsigned short jmesh_gatt_mtu_get_by_id(unsigned char id);
gatt_phone_t *jmesh_app_get_phone(void);

#endif // JMESH_GATT_H
