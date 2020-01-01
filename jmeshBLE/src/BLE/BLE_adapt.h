#ifndef BLE_ADAPT_H
#define BLE_ADAPT_H
#include "stdint.h"
#include "FreeRTOS.H"
#include "semphr.h"
#include "jmesh_ble.h"

typedef struct st_mesh_env{
    unsigned char MAC[6];
    unsigned char URI[32];
    unsigned char net_ID[6];
}mesh_env_t;
void mesh_env_init(void);
const unsigned char* ble_get_mac(void);
const unsigned char* ble_get_uri(void);
const unsigned char* ble_get_net_id(void);
void ble_set_net_id(const unsigned char* net_id);
void aes_128_enc_last_result_get(unsigned char* result);
void osapp_gapm_use_enc_blck_cmd(const uint8_t* key,const uint8_t* data);
#endif
