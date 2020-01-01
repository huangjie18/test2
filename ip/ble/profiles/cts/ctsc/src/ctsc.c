#include "rwip_config.h"

#if (BLE_CTS_CLIENT)
#include "ctsc.h"
#include "ctsc_task.h"
#include "prf_utils.h"
#include "ke_mem.h"

static uint8_t ctsc_init(struct prf_task_env *env,uint16_t *start_hdl,uint16_t app_task,uint8_t sec_lvl,void *params)
{
    struct ctsc_env_tag *ctsc_env = (struct ctsc_env_tag *)ke_malloc(sizeof(struct ctsc_env_tag),KE_MEM_ATT_DB);
    
    env->env = &ctsc_env->prf_env;
    ctsc_env->prf_env.app_task = app_task 
        | (PERM_GET(sec_lvl, SVC_MI) ? PERM(PRF_MI, ENABLE) : PERM(PRF_MI, DISABLE));
    ctsc_env->prf_env.prf_task = env->task | PERM(PRF_MI,ENABLE);
    
    env->id = TASK_ID_CTSC;
    env->desc.idx_max = CTSC_IDX_MAX;
    env->desc.state = ctsc_env->state;
    env->desc.default_handler = &ctsc_default_handler;
    
    uint8_t idx;
    for(idx = 0; idx < CTSC_IDX_MAX; ++idx)
    {
        ctsc_env->env[idx] = NULL;
        ke_state_set(KE_BUILD_ID(env->task,idx),CTSC_FREE);
    }
    
    return GAP_ERR_NO_ERROR;
}
  
static void ctsc_destroy(struct prf_task_env *env)
{
    struct ctsc_env_tag *ctsc_env = (struct ctsc_env_tag *)env->env;
    uint8_t idx;
    // cleanup environment variable for each task instances
    for(idx = 0; idx < CTSC_IDX_MAX ; idx++)
    {
        if(ctsc_env->env[idx] != NULL)
        {
            ke_free(ctsc_env->env[idx]);
        }
    }

    // free profile environment variables
    env->env = NULL;
    ke_free(ctsc_env);
}

static void ctsc_create(struct prf_task_env *env,uint8_t conidx)
{
    ke_state_set(KE_BUILD_ID(env->task,conidx),CTSC_IDLE);    
}

static void ctsc_cleanup(struct prf_task_env *env,uint8_t conidx,uint8_t reason)
{
    struct ctsc_env_tag *ctsc_env = (struct ctsc_env_tag*)env->env;
    if(ctsc_env->env[conidx])
    {
        ke_free(ctsc_env->env[conidx]);
        ctsc_env->env[conidx] = NULL;
    }
    
    ke_state_set(KE_BUILD_ID(env->task,conidx),CTSC_FREE);    

}

void ctsc_enable_rsp_send(struct ctsc_env_tag *ctsc_env,uint8_t conidx,uint8_t status)
{
    ke_task_id_t src_id = prf_src_task_get(&(ctsc_env->prf_env), conidx);
    struct ctsc_enable_rsp *rsp = KE_MSG_ALLOC(CTSC_ENABLE_RSP,   
        prf_dst_task_get(&(ctsc_env->prf_env), conidx),src_id,
        ctsc_enable_rsp);
    rsp->status = status;
    if(status == GAP_ERR_NO_ERROR)
    {
        rsp->cts = ctsc_env->env[conidx]->cts;
        prf_register_atthdl2gatt(&ctsc_env->prf_env,conidx,&ctsc_env->env[conidx]->cts.svc);
        ke_state_set(src_id,CTSC_IDLE);
    }else
    {
        ke_free(ctsc_env->env[conidx]);
        ctsc_env->env[conidx] = NULL;
    }
    ke_msg_send(rsp);
}

void ctsc_get_current_time_rsp_send(struct ctsc_env_tag *ctsc_env,uint8_t conidx,uint8_t status,struct gattc_read_ind const*ind)
{
    ke_task_id_t src_id = prf_src_task_get(&(ctsc_env->prf_env), conidx);
    ke_task_id_t dst_id = prf_dst_task_get(&(ctsc_env->prf_env), conidx);
    struct ctsc_get_current_time_rsp *rsp = KE_MSG_ALLOC(CTSC_GET_CURRENT_TIME_RSP,dst_id,src_id,ctsc_get_current_time_rsp);
    rsp->status = status;
    if(status == GAP_ERR_NO_ERROR)
    {
        BX_ASSERT(ind->length == sizeof(rsp->ct));
        memcpy(&rsp->ct,ind->value,sizeof(rsp->ct));
    }
    ke_msg_send(rsp);
}

const struct prf_task_cbs ctsc_itf =
{
    ctsc_init,
    ctsc_destroy,
    ctsc_create,
    ctsc_cleanup,
};

const struct prf_task_cbs *ctsc_prf_itf_get()
{
    return &ctsc_itf;
}

#endif
