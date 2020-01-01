#include "ble_adapt.h"
#include "osapp_config.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "gapc_task.h"
#include "jmesh_print.h"
#include"../../bearer/beacon/jmesh_device_beacon.h"
#include "diss_task.h"
#include"../onchip_system/os_time.h" 
#include "set_adv_payload_31Byte_patch.h"
#include"semphr.h"
#include"../onchip_system/os_main.h"
#include"../driver/jmesh_ble_callback.h"
#include"jmesh_ble.h"
#include"jmesh_gpio.h"
#include"jmesh_gatt.h"
#include"../jmesh/jmesh_pdu.h"
#include"../bearer/adv/jmesh_adv.h"
#include "jmesh_prov_svc.h"
#include"llm.h"
#include"jmesh_route.h"
#include "diss_task.h"
#include "bxotas_task.h"

static unsigned char aec_128_enc_result[16] = {0};
extern void mesh_task_create(void* param);
static void ble_adv_report_handler(ke_msg_id_t const msgid, struct adv_report *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    if(param->data[0]==2&&param->data_len>3){
        jmesh_ble_recv_adv_callback(param->adv_addr.addr, param->rssi,param->data[4], param->data[3]-1,(unsigned char *)(param->data+5));
    }
    else{
        jmesh_ble_recv_adv_callback(param->adv_addr.addr, param->rssi,param->data[1], param->data[0]-1,(unsigned char *)(param->data+2));
    }	
}

int ble_config()
{
    // Set Device configuration
    struct gapm_set_dev_config_cmd* cmd = AHI_MSG_ALLOC(GAPM_SET_DEV_CONFIG_CMD,TASK_ID_GAPM,gapm_set_dev_config_cmd);
    cmd->operation = GAPM_SET_DEV_CONFIG;
    cmd->role      = GAP_ROLE_ALL;//;GAP_ROLE_PERIPHERAL;//
    cmd->att_cfg = GAPM_MASK_ATT_SVC_CHG_EN | GAPM_MASK_ATT_SLV_PREF_CON_PAR_EN; 
    // Set Data length parameters
    cmd->sugg_max_tx_octets = BLE_MAX_OCTETS;
    cmd->sugg_max_tx_time   = BLE_MAX_TIME;
    cmd->addr_type = GAPM_CFG_ADDR_PUBLIC;
    cmd->pairing_mode = GAPM_PAIRING_LEGACY;
    cmd->max_mtu=GAP_MAX_LE_MTU;
    LOG(LOG_LVL_INFO,"ble_config: operation = %d,role =%d,sugg_max_tx_octets=%d,sugg_max_tx_time=%d,addr_type=%d,pairing_mode=%d,max_mtu = %d\n",
    cmd->operation,cmd->role,cmd->sugg_max_tx_octets,cmd->sugg_max_tx_time,cmd->addr_type,cmd->pairing_mode,cmd->max_mtu);  
    return osapp_msg_build_send(cmd, sizeof(struct gapm_set_dev_config_cmd) );
}


static void ble_default_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"defualt handler,msgid: 0x%x, dest_id: 0x%x, src_id: 0x%x\n",msgid,dest_id,src_id);
}
static int32_t ble_gapc_update_handler(ke_msg_id_t const msgid, struct gapc_param_updated_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{  
    //LOG(LOG_LVL_INFO,"param updated\n");
    //JMESH_LOGI("param update","sucess\n");
		return 0;
}
static int32_t ble_param_update_req_handler(ke_msg_id_t const msgid, struct gapm_cmp_evt const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{   struct gapc_param_update_cfm *cfm = AHI_MSG_ALLOC(GAPC_PARAM_UPDATE_CFM, src_id, gapc_param_update_cfm);
    cfm->accept = 0x01;
    cfm->ce_len_max = 0xffff;
    cfm->ce_len_min = 0xffff;
    //LOG(LOG_LVL_INFO,"ble_param_update_req_handler\n");
    //JMESH_LOGI("param update","confirm\n");	
    return osapp_msg_build_send(cfm, sizeof(struct gapc_param_update_cfm));
}

static void ble_device_ready_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    jmesh_ble_reset();
}
static int32_t osapp_gapc_conn_confirm(ke_task_id_t dest_id)
{
    struct gapc_connection_cfm *cfm = AHI_MSG_ALLOC(GAPC_CONNECTION_CFM, dest_id, gapc_connection_cfm);
    cfm->auth = GAP_AUTH_REQ_NO_MITM_NO_BOND;
    return osapp_msg_build_send(cfm,sizeof(struct gapc_connection_cfm));
}
static int32_t osapp_set_le_pkt_size(ke_task_id_t dst_id)
{
    struct gapc_set_le_pkt_size_cmd *cmd =  AHI_MSG_ALLOC(GAPC_SET_LE_PKT_SIZE_CMD,dst_id,gapc_set_le_pkt_size_cmd);
    cmd->operation = GAPC_SET_LE_PKT_SIZE;
    cmd->tx_octets = BLE_MAX_OCTETS;
    cmd->tx_time = BLE_MAX_TIME;
    return osapp_msg_build_send(cmd, sizeof(struct gapc_set_le_pkt_size_cmd));
}
static os_timer_event_t jmesh_connect_light_timer;
static unsigned int jmesh_connect_light_times=0;
static unsigned int jmesh_connect_light_flag;
void jmesh_connect_light_schedule(void){
    if(jmesh_connect_light_flag==1){
        jmesh_connect_light_times++;
		if(jmesh_connect_light_times%2!=0)
		{
		    jmesh_gpio_set(JMESH_LED1, GPIO_VALUE_LOW);
		}
		else
		{
		    jmesh_gpio_set(JMESH_LED1, GPIO_VALUE_HIGH);
		}
        os_timer_event_set(&jmesh_connect_light_timer,1500,(os_timer_event_caller_t)jmesh_connect_light_schedule,NULL);
        
    }
	else if(jmesh_connect_light_flag==2){
		jmesh_connect_light_times++;
		if(jmesh_connect_light_times%2!=0)
		{
		    jmesh_gpio_set(JMESH_LED1, GPIO_VALUE_LOW);
		}
		else
		{
		    jmesh_gpio_set(JMESH_LED1, GPIO_VALUE_HIGH);
		}
        os_timer_event_set(&jmesh_connect_light_timer,250,(os_timer_event_caller_t)jmesh_connect_light_schedule,NULL);
	}
	else{
	    return;
	}
}
static void ble_connect_req_handler(ke_msg_id_t const msgid, struct gapc_connection_req_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    int status=jmesh_ble_status();
    jmesh_ble_stop_cancel();
    jmesh_ble_status_switch();
    osapp_gapc_conn_confirm(src_id);
    //jmesh_prov_svc_env.connect_id=src_id;
	  osapp_set_le_pkt_size(src_id);  
  
    if(JMESH_BLE_STATUS_CONNECT==status){
      jmesh_ble_gatt_connected_callback((unsigned char *)param->peer_addr.addr,param->conhdl,param->con_interval,1);
    }
    else{
      jmesh_ble_gatt_connected_callback((unsigned char *)param->peer_addr.addr, param->conhdl,param->con_interval,0);
    }
    //jmesh_gpio_set(JMESH_LED1, GPIO_VALUE_LOW);
//	jmesh_connect_light_flag=1;
//	jmesh_connect_light_times=0;
//    os_timer_event_set(&jmesh_connect_light_timer,1000,(os_timer_event_caller_t)jmesh_connect_light_schedule,NULL);
}
static void ble_gapc_connection_confirm_handler(ke_msg_id_t const msgid, struct gapc_disconnect_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"connection[%x] confirmed\n",src_id);  
}
static void ble_disconnect_handler(ke_msg_id_t const msgid, struct gapc_disconnect_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    //LOG(LOG_LVL_WARN,"%d disconnected reason %d\n",KE_IDX_GET(src_id),param->reason);	
    jmesh_ble_gatt_disconnected_callback(KE_IDX_GET(src_id),param->reason);
    //jmesh_gpio_set(JMESH_LED1, GPIO_VALUE_HIGH);
//	jmesh_connect_light_flag=2;
//	jmesh_connect_light_times=0;
//	os_timer_event_set(&jmesh_connect_light_timer,1000,(os_timer_event_caller_t)jmesh_connect_light_schedule,NULL);
}
static void ble_mtu_change_handler(ke_msg_id_t const msgid, struct gattc_mtu_changed_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    jmesh_ble_exchange_mtu_callback(KE_IDX_GET(src_id), param->mtu);
}

static void ble_gapc_cmp_evt_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
}
static void ble_disc_incl_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"incl");
}
static void ble_disc_char_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"char");
}
static void ble_disc_desc_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"desc");
}
static void ble_sdp_svc_handler(ke_msg_id_t const msgid, struct gattc_sdp_svc_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    jmesh_ble_disc_svc_callback(KE_IDX_GET(src_id),NULL,param->start_hdl);
}
//static mesh_env_t mesh_env;
//void mesh_env_init(void){
//    ;
//}
//const unsigned char* ble_get_mac(void)
//{
//    return mesh_env.MAC;
//}
//const unsigned char* ble_get_uri(void)
//{
//    return mesh_env.URI;
//}
//const unsigned char* ble_get_net_id(void)
//{
//    return mesh_env.net_ID;
//}
//void ble_set_net_id(const unsigned char* net_id)
//{
//    memcpy(mesh_env.net_ID,net_id,6);
//}
static void ble_gapc_bond_req_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapc_bond_req_ind const *ind = param;
    LOG(LOG_LVL_INFO,"gapc_bond_req_ind_%d\n",ind->request);
    switch(ind->request)
    {
        case GAPC_PAIRING_REQ:
        {
            LOG(LOG_LVL_INFO,"GAPC_PAIRING_REQ\n");
            struct gapc_bond_cfm *cfm = AHI_MSG_ALLOC(GAPC_BOND_CFM, jmesh_prov_svc_env.connect_id, gapc_bond_cfm);
            cfm->request = GAPC_PAIRING_RSP;
            cfm->accept = 0x01;
            cfm->data.pairing_feat.iocap = 0x03;
            cfm->data.pairing_feat.oob = 0x00;
            cfm->data.pairing_feat.auth = 0x09;
            cfm->data.pairing_feat.key_size = 0x10;
            cfm->data.pairing_feat.ikey_dist = 0x04;
            cfm->data.pairing_feat.rkey_dist = 0x03;
            cfm->data.pairing_feat.sec_req = 0x00;
            osapp_msg_build_send(cfm, sizeof(struct gapc_bond_cfm));
            break;
        }
        case GAPC_LTK_EXCH:
        {
            uint8_t i;
            LOG(LOG_LVL_INFO,"GAPC_LTK_EXCH\n");
            struct gapc_bond_cfm *cfm = AHI_MSG_ALLOC(GAPC_BOND_CFM, jmesh_prov_svc_env.connect_id, gapc_bond_cfm);
            cfm->accept = 0x1;
            cfm->request = GAPC_LTK_EXCH;
            cfm->data.ltk.ediv = 0xA6A4;
            for(i=0;i<KEY_LEN;i++)
            {
                cfm->data.ltk.ltk.key[i] = (uint8_t)co_rand_word();
            }
//                memcpy(cfm->data.ltk.ltk.key, ltk_key, sizeof(ltk_key));
            for(i=0;i<RAND_NB_LEN;i++)
            {
                cfm->data.ltk.randnb.nb[i] = (uint8_t)co_rand_word();
            }
//                memcpy(cfm->data.ltk.randnb.nb, ltk_randnb, sizeof(ltk_randnb));
            cfm->data.ltk.key_size = KEY_LEN;
            osapp_msg_build_send(cfm, sizeof(struct gapc_bond_cfm));
            break;
        }
        default:
            break;
    }
}
static void osapp_gattm_add_svc_rsp_handler(ke_msg_id_t const msgid, struct gattm_add_svc_rsp const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    if(param->status == ATT_ERR_NO_ERROR)
    {
				jmesh_ble_set_local_svc_ins(param->start_hdl + 1);
    }else
    {
        LOG(LOG_LVL_ERROR,"add svc fail\n");
				LOG(3,"SVC fail num = 0x%x\n",param->status);
    }
    mesh_task_create(NULL);
}

static void ble_gapc_bond_ind_handler(ke_msg_id_t const msgid, void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{    
    struct gapc_bond_ind const *ind = param;
    LOG(LOG_LVL_INFO,"bond_ind_%d\n",ind->info);
    switch(ind->info)
    {
        case GAPC_CSRK_EXCH:
            LOG(LOG_LVL_INFO,"Csrk = %x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x\n",
                ind->data.csrk.key[15],ind->data.csrk.key[14],ind->data.csrk.key[13],ind->data.csrk.key[12],
                ind->data.csrk.key[11],ind->data.csrk.key[10],ind->data.csrk.key[9],ind->data.csrk.key[8],
                ind->data.csrk.key[7],ind->data.csrk.key[6],ind->data.csrk.key[5],ind->data.csrk.key[4],
                ind->data.csrk.key[3],ind->data.csrk.key[2],ind->data.csrk.key[1],ind->data.csrk.key[0]);
            break;
        case GAPC_PAIRING_SUCCEED:
            LOG(LOG_LVL_INFO,"Auth_%d,Ltk Present_%d\n",ind->data.auth.info,ind->data.auth.ltk_present);
            break;
    }
}

static void osapp_gattc_event_ind_handler(ke_msg_id_t const msgid, struct gattc_event_ind const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    jmesh_ble_gatt_notified_callback(KE_IDX_GET(src_id),param->handle,param->length,(unsigned char *)param->value);
}
void osapp_gapm_use_enc_blck_cmd(const uint8_t* key,const uint8_t* data)
{
	struct gapm_use_enc_block_cmd* cmd = AHI_MSG_ALLOC(GAPM_USE_ENC_BLOCK_CMD,TASK_ID_GAPM,gapm_use_enc_block_cmd);
	cmd->operation = GAPM_USE_ENC_BLOCK;
	memcpy(cmd->operand_1,key,16);
	memcpy(cmd->operand_2,data,16);		
    osapp_msg_build_send(cmd, sizeof(struct gapm_use_enc_block_cmd));	
}
void aes_128_enc_last_result_get(unsigned char* result)
{
	memcpy(result,aec_128_enc_result,16);
}
static void osapp_gapm_use_enc_block_ind_handler(ke_msg_id_t const msgid,
                                       struct gapm_use_enc_block_ind const *param,
                                       ke_task_id_t const dest_id,
                                       ke_task_id_t const src_id)
{
	memcpy(aec_128_enc_result,param->result,16);
	xSemaphoreGive(ble_aec_enc_xSemaphore);
}
static void gattc_write_req_ind_handler(ke_msg_id_t const msgid,
                                       struct gattc_write_req_ind const *param,
                                       ke_task_id_t const dest_id,
                                       ke_task_id_t const src_id)
{
    struct gattc_write_cfm *cfm = AHI_MSG_ALLOC(GATTC_WRITE_CFM,src_id,gattc_write_cfm);
		cfm->handle = param->handle;
		cfm->status = ATT_ERR_NO_ERROR;
		osapp_msg_build_send(cfm,sizeof(struct gattc_write_cfm));
//		LOG(LOG_LVL_INFO,"param gattc_write_ind:%d,%d,%s\n", param->length, param->handle, param->value);
    jmesh_ble_gatt_writed_callback(KE_IDX_GET(src_id),param->handle,param->length,(unsigned char *)param->value);
}

static void gapc_le_pkt_size_ind_handler(ke_msg_id_t const msgid,
                            void const *param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id)
{
//    struct gapc_le_pkt_size_ind *ind = AHI_MSG_ALLOC(GATTC_WRITE_CFM,src_id,gattc_write_cfm);	
	
}

static int32_t osapp_add_bxotas_task(void)
{
    struct gapm_profile_task_add_cmd *req=AHI_MSG_ALLOC(GAPM_PROFILE_TASK_ADD_CMD, TASK_ID_GAPM, gapm_profile_task_add_cmd);
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl = PERM(SVC_AUTH,NO_AUTH);
    req->prf_task_id = TASK_ID_BXOTAS;
    req->app_task = TASK_AHI;
    req->start_hdl = 0;
    return osapp_msg_build_send(req,sizeof(struct gapm_profile_task_add_cmd));
}

//static void osapp_bxotas_config(void)
//{
//    struct bxotas_firmware_dest_cmd *cmd =AHI_MSG_ALLOC(BXOTAS_FIRMWARE_DEST_CMD,TASK_ID_BXOTAS,bxotas_firmware_dest_cmd);
//    cmd->firmware_dest = LOCAL_FLASH;
//    osapp_ahi_msg_send(cmd,sizeof(struct bxotas_firmware_dest_cmd),portMAX_DELAY);
//}

//static void osapp_gapm_profile_added_ind_handler(ke_msg_id_t const msgid, struct gapm_profile_added_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
//{
//    LOG(LOG_LVL_INFO,"GAPM profile added indication,id:%d,nb:%d,hdl:%d\n",param->prf_task_id,param->prf_task_nb,param->start_hdl);
//    if(param->prf_task_id == TASK_ID_DISS)
//    {
//        osapp_add_bxotas_task();
//    }
//    if(param->prf_task_id==TASK_ID_BXOTAS)
//    {
//        osapp_bxotas_config();
//    }
//}

static void osapp_bxotas_start_cfm(uint8_t status)
{
    struct bxotas_start_cfm *cfm = AHI_MSG_ALLOC(BXOTAS_START_CFM, TASK_ID_BXOTAS, bxotas_start_cfm);
    cfm->status = status;
    osapp_ahi_msg_send(cfm, sizeof(struct bxotas_start_cfm), portMAX_DELAY);
}

static void osapp_bxotas_start_req_ind_handler(ke_msg_id_t const msgid,struct bxotas_start_req_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"OTA START:%d,%d\n",param->conidx,param->segment_data_max_length);
		#if(OTA_SWICTCH == 1) 
		if(ota_req_permit_state)
		{
			osapp_bxotas_start_cfm(OTA_REQ_ACCEPTED);
			LOG(LOG_LVL_INFO,"osapp_bxotas_start_cfm:OTA_REQ_ACCEPTED\n");
		}
		else
		{
			osapp_bxotas_start_cfm(OTA_REQ_REJECTED);
			LOG(LOG_LVL_INFO,"osapp_bxotas_start_cfm:OTA_REQ_REJECTED\n");
		}
		#else
		osapp_bxotas_start_cfm(OTA_REQ_ACCEPTED);
		#endif
}

static void osapp_bxotas_finish_ind_handler(ke_msg_id_t const msgid,void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"OTA_DONE\n");
    /* do not reset chip immediately */
    BX_DELAY_US(1000000);
    platform_reset(0);
}

static int32_t osapp_get_dev_name(ke_task_id_t const dest_id)
{
    nvds_tag_len_t device_name_length = NVDS_LEN_DEVICE_NAME;
    struct gapc_get_dev_info_cfm *cfm=AHI_MSG_ALLOC_DYN(GAPC_GET_DEV_INFO_CFM, dest_id,gapc_get_dev_info_cfm, device_name_length);
    cfm->req = GAPC_DEV_NAME;
    nvds_get(NVDS_TAG_DEVICE_NAME, &device_name_length, cfm->info.name.value);
    cfm->info.name.length = device_name_length;
    return osapp_msg_build_send(cfm, sizeof(struct gapc_get_dev_info_cfm)+device_name_length);
}

static int32_t osapp_get_dev_appearance(ke_task_id_t const dest_id)
{
    struct gapc_get_dev_info_cfm *cfm=AHI_MSG_ALLOC(GAPC_GET_DEV_INFO_CFM, dest_id,gapc_get_dev_info_cfm);
    cfm->req = GAPC_DEV_APPEARANCE;
    cfm->info.appearance = 0;
    return osapp_msg_build_send(cfm, sizeof(struct gapc_get_dev_info_cfm));
}

static int32_t osapp_get_dev_slv_pref_params(ke_task_id_t const dest_id)
{
    struct gapc_get_dev_info_cfm *cfm=AHI_MSG_ALLOC(GAPC_GET_DEV_INFO_CFM, dest_id,gapc_get_dev_info_cfm);
    cfm->req = GAPC_DEV_SLV_PREF_PARAMS;
    cfm->info.slv_params.con_intv_min = 8;
    cfm->info.slv_params.con_intv_max = 10;
    cfm->info.slv_params.slave_latency = 0;
    cfm->info.slv_params.conn_timeout = 200;
    return osapp_msg_build_send(cfm, sizeof(struct gapc_get_dev_info_cfm));
}

static void osapp_gapc_get_dev_info_req_ind_handler(ke_msg_id_t const msgid, struct gapc_get_dev_info_req_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    struct gapc_get_dev_info_req_ind const *req_ind =param;
    switch(req_ind->req)
    {
    case GAPC_DEV_NAME:
        osapp_get_dev_name(src_id);
        break;
    case GAPC_DEV_APPEARANCE:
        osapp_get_dev_appearance(src_id);
        break;
    case GAPC_DEV_SLV_PREF_PARAMS:
        osapp_get_dev_slv_pref_params(src_id);
        break;
    default :
        //BX_ASSERT(0);
        break;
    }
}

static void osapp_gapm_profile_added_ind_handler(ke_msg_id_t const msgid, struct gapm_profile_added_ind const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    LOG(LOG_LVL_INFO,"GAPM profile added indication,id:%d,nb:%d,hdl:%d\n",param->prf_task_id,param->prf_task_nb,param->start_hdl);
}

static void gattc_read_req_ind_handler(ke_msg_id_t const msgid,
                            const struct gattc_read_req_ind *param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id)
{
    struct gattc_read_cfm *cfm = AHI_MSG_ALLOC_DYN(GATTC_READ_CFM,src_id,gattc_read_cfm,4);
    cfm->handle = param->handle;
    cfm->status = ATT_ERR_NO_ERROR;
    cfm->length=0;
    LOG(LOG_LVL_INFO,"param gattc_write_ind:%d\n", param->handle);
    osapp_msg_build_send(cfm,sizeof(struct gattc_read_cfm));  
}








static void ble_gapm_cmp_evt_handler(ke_msg_id_t const msgid, struct gapm_cmp_evt const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{    
    switch(param->operation)
    {
        case GAPM_RESET:
            BX_ASSERT(param->status==GAP_ERR_NO_ERROR);
            LOG(LOG_LVL_INFO,"ble config\n");
            ble_config();
            break;
        case GAPM_SET_DEV_CONFIG:
            osapp_add_bxotas_task();
            break;
        case GAPM_SCAN_ACTIVE:
        case GAPM_SCAN_PASSIVE: 
            //LOG(LOG_LVL_INFO,"%ld: scan passive ind status = 0x%x\n",xTaskGetTickCount(),param->status);
            if(param->status==GAP_ERR_CANCELED)
            {
                jmesh_ble_cancel_callback(1);//cancel success
                jmesh_ble_canceled_callback(JMESH_BLE_STATUS_SCAN);
            }
            else
            {
                JMESH_LOGE(1,"bluetooth operation","scan error:0x%02x\n",param->status);
                //BX_ASSERT(param->status==GAP_ERR_NO_ERROR);
            }              
            break;				
        case GAPM_ADV_NON_CONN:								
        case GAPM_ADV_UNDIRECT:
			      //LOG(LOG_LVL_INFO,"%ld: adv udirect ind status = 0x%x\n",xTaskGetTickCount(),param->status);	
            if(param->status==GAP_ERR_CANCELED)
            {
                jmesh_ble_cancel_callback(1);//cancel success
                jmesh_ble_canceled_callback(JMESH_BLE_STATUS_ADV);
            }
            else if(param->status==GAP_ERR_NO_ERROR)
            {
            }
            else
            {
                JMESH_LOGE(1,"bluetooth operation","adv error:0x%02x\n",param->status);
                //BX_ASSERT(param->status==GAP_ERR_NO_ERROR);
            }                      
            break;
        case GAPM_CANCEL:
			      //LOG(LOG_LVL_INFO,"gapm canceled ind status = 0x%x\n",param->status);
            JMESH_LOGE(1,"bluetooth operation","cancel error:0x%02x\n",param->status);
            jmesh_ble_cancel_callback(0);//cancel fail
            //BX_ASSERT(param->status==GAP_ERR_NO_ERROR);
            break;
        case GAPM_PROFILE_TASK_ADD:
            LOG(LOG_LVL_INFO,"ble add svc\n");
			      jmesh_add_prov_svc();
            break;
        case GAPM_CONNECTION_DIRECT: 		
            if(param->status==GAP_ERR_CANCELED){
                jmesh_ble_cancel_callback(1);//cancel success
                jmesh_ble_canceled_callback(JMESH_BLE_STATUS_CONNECT);
            }
            else if(param->status==GAP_ERR_NO_ERROR){
		        }
            else
            {
                JMESH_LOGE(1,"bluetooth operation","connect error:0x%02x\n",param->status);
                //BX_ASSERT(param->status==GAP_ERR_NO_ERROR);
            }
            break;
        default:
            break;
    }
}

static void ble_gattc_cmp_evt_handler(ke_msg_id_t const msgid, struct gattc_cmp_evt const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{    
    switch(param->operation)
    {
        case GATTC_WRITE:
            if(param->status!=GAP_ERR_NO_ERROR){
                jmesh_ble_gatt_disconnect(src_id>>8);
                jmesh_gatt_queue_send(0);
                JMESH_LOGE(1,"bluetooth operation","write error:0x%02x\n",param->status);               
                return;              
            }
            jmesh_gatt_queue_send(1);           
            break;
        case GATTC_NOTIFY:
            if(param->status!=GAP_ERR_NO_ERROR){
                jmesh_ble_gatt_disconnect(src_id>>8);
                jmesh_gatt_queue_send(0); 
                JMESH_LOGE(1,"bluetooth operation","notify error:0x%02x\n",param->status);              
                return;              	
            }	
            jmesh_gatt_queue_send(1);             
            break;
        case GATTC_WRITE_NO_RESPONSE:
            if(param->status!=GAP_ERR_NO_ERROR){
                jmesh_ble_gatt_disconnect(src_id>>8);
                jmesh_gatt_queue_send(0);
                JMESH_LOGE(1,"bluetooth operation","write error:0x%02x\n",param->status); 
                return;              
            }
            jmesh_gatt_queue_send(1);             
            break;
        default:
            break;
    }
}

static const osapp_msg_handler_table_t handler_table[]=
{
    [0] =   {KE_MSG_DEFAULT_HANDLER,  (osapp_msg_handler_t)ble_default_handler},
            {GAPC_CONNECTION_REQ_IND,       (osapp_msg_handler_t)ble_connect_req_handler},
            {GAPC_CONNECTION_CFM,   (osapp_msg_handler_t)ble_gapc_connection_confirm_handler},
            {GAPC_PARAM_UPDATED_IND,    (osapp_msg_handler_t)ble_gapc_update_handler},
            {GAPC_DISCONNECT_IND,   (osapp_msg_handler_t)ble_disconnect_handler},
            {GAPM_CMP_EVT,  (osapp_msg_handler_t)ble_gapm_cmp_evt_handler},
            {GAPM_DEVICE_READY_IND, (osapp_msg_handler_t)ble_device_ready_handler},
            {GAPC_CMP_EVT,  (osapp_msg_handler_t)ble_gapc_cmp_evt_handler},
            {GAPM_ADV_REPORT_IND,   (osapp_msg_handler_t)ble_adv_report_handler},
            {GATTM_ADD_SVC_RSP,(osapp_msg_handler_t)osapp_gattm_add_svc_rsp_handler},
            {GAPM_USE_ENC_BLOCK_IND,(osapp_msg_handler_t)osapp_gapm_use_enc_block_ind_handler},	
            {GATTC_DISC_SVC_INCL_IND,   (osapp_msg_handler_t)ble_disc_incl_handler},
            {GATTC_DISC_CHAR_IND,   (osapp_msg_handler_t)ble_disc_char_handler},
            {GATTC_DISC_CHAR_DESC_IND,  (osapp_msg_handler_t)ble_disc_desc_handler},
            {GATTC_SDP_SVC_IND, (osapp_msg_handler_t)ble_sdp_svc_handler},
            {GAPC_PARAM_UPDATE_REQ_IND, (osapp_msg_handler_t)ble_param_update_req_handler},
            {GATTC_MTU_CHANGED_IND, (osapp_msg_handler_t)ble_mtu_change_handler},
            {GAPC_BOND_REQ_IND,             (osapp_msg_handler_t)ble_gapc_bond_req_ind_handler},
            {GAPC_BOND_IND,                 (osapp_msg_handler_t)ble_gapc_bond_ind_handler},
            {GATTC_EVENT_IND,(osapp_msg_handler_t)osapp_gattc_event_ind_handler},
            {GATTC_CMP_EVT,(osapp_msg_handler_t)ble_gattc_cmp_evt_handler},
            {GATTC_WRITE_REQ_IND,(osapp_msg_handler_t)gattc_write_req_ind_handler},		
            {GAPC_LE_PKT_SIZE_IND,(osapp_msg_handler_t)gapc_le_pkt_size_ind_handler},
            {GAPC_GET_DEV_INFO_REQ_IND,(osapp_msg_handler_t)osapp_gapc_get_dev_info_req_ind_handler},
			      {GAPM_PROFILE_ADDED_IND,(osapp_msg_handler_t)osapp_gapm_profile_added_ind_handler},
			      {GATTC_READ_REQ_IND,(osapp_msg_handler_t)gattc_read_req_ind_handler},
            {BXOTAS_START_REQ_IND,(osapp_msg_handler_t)osapp_bxotas_start_req_ind_handler},
            {BXOTAS_FINISH_IND,(osapp_msg_handler_t)osapp_bxotas_finish_ind_handler},
};

osapp_msg_handler_info_t handler_info = HANDLER_ARRAY_INFO(handler_table);

void user_init()
{
//    osapp_svc_manager_init();
    ahi_handler_register(&handler_info);
}

