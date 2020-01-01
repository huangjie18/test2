#include "rwip_config.h"
#if (BLE_CTS_CLIENT)
#include "ctsc.h"
#include "prf_utils.h"
#include "prf_types.h"
#include "att.h"
#include "log.h"

const struct prf_char_def ctsc_cts_char[CTS_CHAR_MAX] = 
{
    [CTS_CHAR_CURRENT_TIME] = {ATT_CHAR_CT_TIME,ATT_MANDATORY,ATT_CHAR_PROP_RD|ATT_CHAR_PROP_NTF},
    [CTS_CHAR_LOCAL_TIME_INFO] = {ATT_CHAR_LOCAL_TIME_INFO,ATT_OPTIONAL,ATT_CHAR_PROP_RD},
    [CTS_CHAR_REF_TIME_INFO] = {ATT_CHAR_REFERENCE_TIME_INFO,ATT_OPTIONAL,0},
};

const struct prf_char_desc_def ctsc_cts_char_desc[CTS_DESC_MAX]=
{
   [CTS_DESC_CURRENT_TIME_CL_CFG] = {ATT_DESC_CLIENT_CHAR_CFG,ATT_MANDATORY,CTS_CHAR_CURRENT_TIME},    
};

static uint8_t ctsc_svc_validity_check(struct ctsc_cts_content const*cts)
{
    uint8_t status = prf_check_svc_char_validity(CTS_CHAR_MAX,cts->chars,ctsc_cts_char);
    if(status == ATT_ERR_NO_ERROR)
    {
        status = prf_check_svc_char_desc_validity(CTS_DESC_MAX,cts->descs,ctsc_cts_char_desc,cts->chars);
    }
    return status;
}

static int ctsc_enable_req_handler(ke_msg_id_t const msgid,
    struct ctsc_enable_req const *param,
    ke_task_id_t const dest_id,
    ke_task_id_t const src_id)
{
    uint8_t state = ke_state_get(dest_id);
    uint8_t conidx = KE_IDX_GET(dest_id);
    struct ctsc_env_tag *ctsc_env = PRF_ENV_GET(CTSC,ctsc);
    ASSERT_INFO(ctsc_env != NULL, dest_id, src_id);
    if(state == CTSC_IDLE && ctsc_env->env[conidx] == NULL)
    {
        ctsc_env->env[conidx] = (struct ctsc_cnx_env*)ke_malloc(sizeof(struct ctsc_cnx_env),KE_MEM_ATT_DB);
        memset(ctsc_env->env[conidx],0,sizeof(struct ctsc_cnx_env));
        
        if(param->con_type == PRF_CON_DISCOVERY)
        {
            prf_disc_svc_send(&ctsc_env->prf_env,conidx,ATT_SVC_CURRENT_TIME);
            ke_state_set(dest_id,CTSC_DISCOVERING);
        }else
        {
            uint8_t status = ctsc_svc_validity_check(&param->cts);
            if(status == ATT_ERR_NO_ERROR)
            {
                ctsc_env->env[conidx]->cts = param->cts;
            }
            ctsc_enable_rsp_send(ctsc_env,conidx,status);
        }
    }else
    {
        ctsc_enable_rsp_send(ctsc_env,conidx,PRF_ERR_REQ_DISALLOWED);
    }
    return KE_MSG_CONSUMED;
}

static int ctsc_req_return_status(uint8_t state)
{
    int status;
    switch(state)
    {
    case CTSC_FREE:
    case CTSC_IDLE:
    case CTSC_DISCOVERING:
        status = KE_MSG_CONSUMED;
    break;
    case CTSC_CURRENT_TIME_REQUESTING:
        status = KE_MSG_SAVED;
    break;
    }
    return status;
}

static int ctsc_get_current_time_req_handler(ke_msg_id_t const msgid,
    void *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    uint8_t state = ke_state_get(dest_id);
    uint8_t conidx = KE_IDX_GET(dest_id);
    struct ctsc_env_tag *ctsc_env = PRF_ENV_GET(CTSC,ctsc); 
    if(state == CTSC_IDLE)
    {
        prf_read_char_send(&ctsc_env->prf_env,conidx,
            ctsc_env->env[conidx]->cts.svc.shdl,
            ctsc_env->env[conidx]->cts.svc.ehdl,
            ctsc_env->env[conidx]->cts.chars[CTS_CHAR_CURRENT_TIME].val_hdl);
        ke_state_set(dest_id,CTSC_CURRENT_TIME_REQUESTING);
    }
    return ctsc_req_return_status(state);
}

static int gattc_cmp_evt_handler(ke_msg_id_t const msgid,
                                struct gattc_cmp_evt const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id)
{
    uint8_t state = ke_state_get(dest_id);
    uint8_t conidx = KE_IDX_GET(dest_id);
    struct ctsc_env_tag *ctsc_env = PRF_ENV_GET(CTSC,ctsc); 
    uint8_t status = param->status;
    switch(state)
    {
    case CTSC_DISCOVERING: 
        if(status == ATT_ERR_NO_ERROR)
        {
            if(ctsc_env->env[conidx]->nb_svc == 1)
            {
                status = ctsc_svc_validity_check(&ctsc_env->env[conidx]->cts);
            }else if(ctsc_env->env[conidx]->nb_svc > 1)
            {
                status = PRF_ERR_MULTIPLE_SVC;
            }else
            {
                status = PRF_ERR_STOP_DISC_CHAR_MISSING;
            }
        }
        ctsc_enable_rsp_send(ctsc_env,conidx,status);
    break;
    case CTSC_CURRENT_TIME_REQUESTING:
        if(status != GAP_ERR_NO_ERROR)
        {
            ctsc_get_current_time_rsp_send(ctsc_env,conidx,status,NULL);
        }
        ke_state_set(dest_id,CTSC_IDLE);
    break;
    case CTSC_IDLE:
        BX_ASSERT(param->operation == GATTC_REGISTER && param->status == GAP_ERR_NO_ERROR);
    break;
    default:
        LOG(LOG_LVL_WARN,"ctsc state:%d,gattc_cmp_evt,%d,%d\n",state,param->operation,param->status);
    break;
    }
    return KE_MSG_CONSUMED;
}

static int gattc_read_ind_handler(ke_msg_id_t const msgid,
                                  struct gattc_read_ind const *param,
                                  ke_task_id_t const dest_id,
                                  ke_task_id_t const src_id)
{
    uint8_t state = ke_state_get(dest_id);
    uint8_t conidx = KE_IDX_GET(dest_id);
    struct ctsc_env_tag *ctsc_env = PRF_ENV_GET(CTSC,ctsc); 
    if(state == CTSC_CURRENT_TIME_REQUESTING)
    {
        ctsc_get_current_time_rsp_send(ctsc_env,conidx,GAP_ERR_NO_ERROR,param);
    }
    return KE_MSG_CONSUMED;
}

static int gattc_sdp_svc_ind_handler(ke_msg_id_t const msgid,
    struct gattc_sdp_svc_ind const *ind,
    ke_task_id_t const dest_id,
    ke_task_id_t const src_id)
{
    uint8_t state = ke_state_get(dest_id);
    if(state == CTSC_DISCOVERING)
    {
        uint8_t conidx = KE_IDX_GET(dest_id);
        struct ctsc_env_tag *ctsc_env = PRF_ENV_GET(CTSC,ctsc);   
        ASSERT_INFO(ctsc_env != NULL, dest_id, src_id);
        ASSERT_INFO(ctsc_env->env[conidx] != NULL, dest_id, src_id);
        if(ctsc_env->env[conidx]->nb_svc == 0)
        {
            prf_extract_svc_info(ind,CTS_CHAR_MAX,ctsc_cts_char,ctsc_env->env[conidx]->cts.chars,CTS_DESC_MAX,ctsc_cts_char_desc,ctsc_env->env[conidx]->cts.descs);
            ctsc_env->env[conidx]->cts.svc.shdl = ind->start_hdl;
            ctsc_env->env[conidx]->cts.svc.ehdl = ind->end_hdl;
        }
        ctsc_env->env[conidx]->nb_svc++;
    }
    return KE_MSG_CONSUMED;
}

const struct ke_msg_handler ctsc_default_state[] = 
{
    {CTSC_ENABLE_REQ,(ke_msg_func_t)ctsc_enable_req_handler},
    {CTSC_GET_CURRENT_TIME_REQ,(ke_msg_func_t)ctsc_get_current_time_req_handler},
    {GATTC_READ_IND,(ke_msg_func_t)gattc_read_ind_handler},
    {GATTC_CMP_EVT,(ke_msg_func_t)gattc_cmp_evt_handler},
    {GATTC_SDP_SVC_IND,(ke_msg_func_t)gattc_sdp_svc_ind_handler},
};

const struct ke_state_handler ctsc_default_handler = KE_STATE_HANDLER(ctsc_default_state);

#endif
