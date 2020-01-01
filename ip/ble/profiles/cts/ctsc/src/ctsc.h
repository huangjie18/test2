#ifndef CTSC_H_
#define CTSC_H_

#include "rwip_config.h"
#if (BLE_CTS_CLIENT)
#include <stdint.h>
#include <stdbool.h>
#include "ke_task.h"
#include "prf_utils.h"
#include "ctsc_task.h"
#include "gattc_task.h"
#define CTSC_IDX_MAX (BLE_USER_CONN_NUM)

enum 
{
    CTSC_FREE,
    CTSC_IDLE,
    CTSC_DISCOVERING,
    CTSC_CURRENT_TIME_REQUESTING,
    CTSC_STATE_MAX,
};

struct ctsc_cnx_env
{
    struct ctsc_cts_content cts;
    uint8_t nb_svc;
};

struct ctsc_env_tag
{
    prf_env_t prf_env;
    struct ctsc_cnx_env *env[CTSC_IDX_MAX];
    ke_state_t state[CTSC_IDX_MAX];
};

void ctsc_enable_rsp_send(struct ctsc_env_tag *ctsc_env,uint8_t conidx,uint8_t status);

void ctsc_get_current_time_rsp_send(struct ctsc_env_tag *ctsc_env,uint8_t conidx,uint8_t status,struct gattc_read_ind const*ind);

    
#endif
#endif
