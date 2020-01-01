#include"../jmesh_driver_config.h"
#if (JMESH_DRIVER_BX2400)
#include"../jmesh_ble.h"
#include"stdio.h"
#include"../../onchip_system/os_timer_event.h"
#include"../../onchip_system/os_timer.h"
#include"osapp_config.h"
#include"../../bearer/gatt/jmesh_gatt.h"
#include"set_adv_payload_31Byte_patch.h"
#include"../../jmesh/jmesh_print.h"
#include"../../bearer/adv/jmesh_adv.h"
#include"../../jmesh/jmesh_save_define.h"
#include"../../jmesh/jmesh_save.h"
#include"semphr.h"
#include"jmesh_route.h"
#include"jmesh_proxy.h"
#include"../../onchip_system/os_buffer_queue.h"
#include"arch.h"
#include"../../jmesh/jmesh_features.h"
#include "APP_protocol.h"

SemaphoreHandle_t  ble_aec_enc_xSemaphore=NULL;

static unsigned short local_svc_ins=0;

static jmesh_ble_adapter_t ble_adapter;

OS_BUFFER_QUEUE_CREATE(jmesh_gatt_queue,2552)

static unsigned char scan_enable=0;

static void jmesh_ble_adapter_timerflow(void);

int jmesh_ble_status(void)
{
    return ble_adapter.status;
}

void jmesh_ble_set_status(int status)
{
    ble_adapter.status=status;
}

void jmesh_ble_init(void)
{
	ble_aec_enc_xSemaphore = xSemaphoreCreateBinary();
  os_buffer_queue_init(jmesh_gatt_queue);
  ble_adapter.status=JMESH_BLE_STATUS_INIT;
  ble_adapter.adv.repeats=0;
  ble_adapter.adv.mutex_lock=xSemaphoreCreateMutex();
  ble_adapter.gatt.is_busy=0;
  ble_adapter.gatt.this_id=0xff;
  jmesh_task_sleep_init();
}

void jmesh_ble_cancel(void)
{
    struct gapm_cancel_cmd *cmd = AHI_MSG_ALLOC(GAPM_CANCEL_CMD,TASK_ID_GAPM,gapm_cancel_cmd);
    cmd->operation = GAPM_CANCEL;
    //JMESH_LOGI("cancel","\n");
    osapp_msg_build_send(cmd,sizeof(struct gapm_cancel_cmd));
}

void jmesh_ble_platform_reset(void)
{
    platform_reset(RESET_NO_ERROR);
}

void jmesh_ble_reset(void){
    struct gapm_reset_cmd *cmd = AHI_MSG_ALLOC(GAPM_RESET_CMD,TASK_ID_GAPM,gapm_reset_cmd);
    cmd->operation = GAPM_RESET;
    osapp_msg_build_send(cmd, sizeof(struct gapm_reset_cmd));
}
void jmesh_ble_scan_adv(void){
    struct gapm_start_scan_cmd *cmd;
		cmd = AHI_MSG_ALLOC(GAPM_START_SCAN_CMD,TASK_ID_GAPM,gapm_start_scan_cmd);
		cmd->op.code = GAPM_SCAN_PASSIVE;
		cmd->mode = GAP_OBSERVER_MODE;
		cmd->interval =16*1.6;
		cmd->window = 8*1.6;
		//JMESH_LOGI("scan","\n");
		osapp_msg_build_send(cmd, sizeof(struct gapm_start_scan_cmd));
}

void jmesh_ble_send_adv(unsigned char ad_type,unsigned char length,unsigned char *data)
{
    struct gapm_start_advertise_cmd_patch *cmd;
    unsigned char adv_data_len;
    cmd = AHI_MSG_ALLOC(GAPM_START_ADVERTISE_CMD,TASK_ID_GAPM,gapm_start_advertise_cmd_patch);
    cmd->op.code = GAPM_ADV_UNDIRECT;
	  cmd->intv_min = 320;//*0.625ms
	  cmd->intv_max = 320;//*0.625ms
	  cmd->info.host.mode = GAP_GEN_DISCOVERABLE;
    cmd->op.addr_src    = GAPM_STATIC_ADDR;
	  cmd->channel_map    = ADV_ALL_CHNLS_EN;
	  cmd->info.host.scan_rsp_data_len = 0;
	  adv_data_len=length+2>31?31:length+2;
	  cmd->info.host.adv_data_len      = adv_data_len;
	  cmd->info.host.adv_data[0]  = adv_data_len-1;
	  cmd->info.host.adv_data[1]  = ad_type;
	  memcpy(cmd->info.host.adv_data+2,data,adv_data_len-2);
	  //JMESH_LOGI("send","adv\n");
	  osapp_msg_build_send(cmd,sizeof(struct gapm_start_advertise_cmd_patch));
}

void jmesh_ble_gatt_connect(unsigned char *mac){
    struct gapm_start_connection_cmd* cmd;
    unsigned short null_intv;
    {
      cmd = AHI_MSG_ALLOC_DYN(GAPM_START_CONNECTION_CMD,TASK_ID_GAPM,gapm_start_connection_cmd,sizeof(struct gap_bdaddr));
      cmd->op.code = GAPM_CONNECTION_DIRECT;
      cmd->op.addr_src = GAPM_STATIC_ADDR;
      cmd->scan_interval = 0x04;
      cmd->scan_window = 0x04;
      null_intv=jmesh_gatt_get_null_intv(mac);
      cmd->con_intv_max = null_intv;//1.25ms
      cmd->con_intv_min = null_intv;
      cmd->con_latency = 0;
      cmd->superv_to = 800;//10ms 600*10>(9+1)*100
      cmd->ce_len_max = 0x0;
      cmd->ce_len_min = 0x0;
      cmd->nb_peers = 1;
      cmd->peers[0].addr_type=ADDR_PUBLIC;
      memcpy(cmd->peers[0].addr.addr, mac, 6);
      //JMESH_LOGI("connect","\n");
      osapp_msg_build_send(cmd, sizeof(struct gapm_start_connection_cmd) + sizeof(struct gap_bdaddr) );
    }
}

void jmesh_ble_gatt_disconnect(unsigned char connect_id){
    struct gapc_disconnect_cmd *cmd = AHI_MSG_ALLOC(GAPC_DISCONNECT_CMD,KE_BUILD_ID(TASK_ID_GAPC,connect_id),gapc_disconnect_cmd);
    cmd->operation = GAPC_DISCONNECT;
    cmd->reason    = CO_ERROR_REMOTE_USER_TERM_CON;
    osapp_msg_build_send(cmd,sizeof(struct gapc_disconnect_cmd));
}

void jmesh_ble_disc_svc(unsigned char *uuid,unsigned char connect_id){
	struct gattc_sdp_svc_disc_cmd* svc_req=AHI_MSG_ALLOC(GATTC_SDP_SVC_DISC_CMD, (connect_id<<8)|KE_TYPE_GET(TASK_ID_GATTC),gattc_sdp_svc_disc_cmd);
    svc_req->operation        = GATTC_SDP_DISC_SVC;
    svc_req->start_hdl        = 1;
    svc_req->end_hdl          = ATT_1ST_REQ_END_HDL;
    svc_req->uuid_len = ATT_UUID_128_LEN;
    memcpy(svc_req->uuid, uuid, 16);
    osapp_msg_build_send(svc_req,sizeof(struct gattc_sdp_svc_disc_cmd));
}

void jmesh_ble_set_local_svc_ins(unsigned short instance)
{
    local_svc_ins=instance;
}

unsigned short jmesh_ble_get_local_svc_ins(void)
{
    return local_svc_ins;
}

void jmesh_ble_exchange_mtu(unsigned char connect_id){
    static uint16_t seq_num = 0;
    struct gattc_exc_mtu_cmd *cmd = AHI_MSG_ALLOC(GATTC_EXC_MTU_CMD, (connect_id<<8)|KE_TYPE_GET(TASK_ID_GATTC), gattc_exc_mtu_cmd);
    cmd->operation = GATTC_MTU_EXCH;
    cmd->seq_num = seq_num++;
    osapp_msg_build_send(cmd, sizeof(struct gattc_exc_mtu_cmd));
}

void jmesh_ble_gatt_write(unsigned char connect_id, unsigned short char_handler, unsigned short length,unsigned char *data){
    if(char_handler != ATT_INVALID_HANDLE)
    {
        {
           struct gattc_write_cmd *wr_char = AHI_MSG_ALLOC_DYN(GATTC_WRITE_CMD,KE_BUILD_ID(TASK_ID_GATTC, connect_id),gattc_write_cmd,length);
           // Offset
            wr_char->offset         = 0x0000;
            // cursor always 0
            wr_char->cursor         = 0x0000;
            // Write Type
            wr_char->operation      = GATTC_WRITE_NO_RESPONSE;
            // Characteristic Value attribute handle
            wr_char->handle         = char_handler;
            // Value Length
            wr_char->length         = length;
            // Auto Execute
            wr_char->auto_execute   = true;
            // Value
            memcpy(&wr_char->value[0], data, length);
            // Send the message
            JMESH_LOGI("wri","%d,%d\n",connect_id,length);
            osapp_msg_build_send(wr_char,sizeof(struct gattc_write_cmd)+length);
        }
    }
}

void jmesh_ble_gatt_notify(unsigned char connect_id, unsigned short char_handler, unsigned short length,unsigned char *data){
    if(char_handler != ATT_INVALID_HANDLE)
    {
        static uint16_t notify_seq_num = 0;
        struct gattc_send_evt_cmd *cmd = AHI_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,KE_BUILD_ID(TASK_ID_GATTC, connect_id),gattc_send_evt_cmd,length);
        cmd->operation = GATTC_NOTIFY;
        cmd->seq_num = notify_seq_num++;
        cmd->handle = char_handler;
        cmd->length = length;
        memcpy(cmd->value,data,length);
        JMESH_LOGI("noti","%d,%d\n",connect_id,length);
        osapp_msg_build_send(cmd,sizeof(struct gattc_send_evt_cmd)+length);
    }
}

void jmesh_ble_scan_enable(void)
{
    scan_enable=1;
    if(JMESH_BLE_STATUS_SLEEP==ble_adapter.status){
        ble_adapter.status=JMESH_BLE_STATUS_SCAN;
        jmesh_ble_scan_adv();
    }
}

void jmesh_ble_scan_disable(void)
{
    scan_enable=0;
    if(JMESH_BLE_STATUS_SCAN==ble_adapter.status){
        ble_adapter.status=JMESH_BLE_STATUS_BUSY;
        jmesh_ble_cancel();
    }
}

int jmesh_ble_scan_is_enable(void)
{
    return JMESH_FEATURE_ENABLE!=jmesh_get_lowpower_feature()||scan_enable==1;
}

static void jmesh_ble_adapter_timerflow(void)
{
    if(JMESH_BLE_STATUS_ADV==ble_adapter.status||JMESH_BLE_STATUS_SCAN==ble_adapter.status
      ||JMESH_BLE_STATUS_CONNECT==ble_adapter.status){
        ble_adapter.status=JMESH_BLE_STATUS_BUSY;
        jmesh_ble_cancel();
    }
}

void jmesh_ble_status_start(void)
{
    if(JMESH_BLE_STATUS_INIT!=ble_adapter.status){
        return;
    }
    else if(jmesh_ble_scan_is_enable()){
        ble_adapter.status=JMESH_BLE_STATUS_SCAN;
        jmesh_ble_scan_adv();
    }
    else{
        ble_adapter.status=JMESH_BLE_STATUS_SLEEP;
    }
}

void jmesh_ble_status_switch(void)
{
    if(!jmesh_ble_scan_is_enable()){
        if(JMESH_BLE_STATUS_SCAN==ble_adapter.status){
            ble_adapter.status=JMESH_BLE_STATUS_BUSY;
            jmesh_ble_cancel();
        }
        else{
            ble_adapter.status=JMESH_BLE_STATUS_SLEEP;
        }
    }
    else if(JMESH_BLE_STATUS_SCAN!=ble_adapter.status){
        ble_adapter.status=JMESH_BLE_STATUS_SCAN;
        jmesh_ble_scan_adv();
    }
}

void jmesh_ble_stop_cancel(void){
    if(JMESH_BLE_STATUS_ADV==ble_adapter.status||JMESH_BLE_STATUS_SCAN==ble_adapter.status
      ||JMESH_BLE_STATUS_CONNECT==ble_adapter.status){
        os_timer_event_remove(&ble_adapter.timer);
    }
}

static void jmesh_timer_cancel(void){

    ble_adapter.status=JMESH_BLE_STATUS_BUSY;
    jmesh_ble_cancel();
}

void jmesh_ble_cancel_callback(unsigned char success){
    static unsigned char fail_times=0;
    if(success){
        if(fail_times>0){
            fail_times=0;
        }
    }
    else{
        fail_times++;
        JMESH_LOGE(1,"bluetooth operation","cancel fail %d times\n",fail_times);
        if(fail_times>=3){
            fail_times=0;
            jmesh_ble_status_switch();
        }
        else{
            os_timer_event_set(&ble_adapter.timer,500,(os_timer_event_caller_t)jmesh_timer_cancel,NULL);
        }
    }
}

void jmesh_adv_send(unsigned char ad_type,unsigned char length,unsigned char* data,unsigned char repeats)
{
    if(length!=0&&length<=JMESH_ADV_CACHE_SIZE){
        JMESH_LOGD_BUFFER("adv",length,data,"send adv type=%d,data:",ad_type);
        xSemaphoreTake(ble_adapter.adv.mutex_lock,portMAX_DELAY);
        ble_adapter.adv.repeats=repeats;
        ble_adapter.adv.length=length;
        ble_adapter.adv.ad_type=ad_type;
        memcpy(ble_adapter.adv.ad_data,data,length);
        xSemaphoreGive(ble_adapter.adv.mutex_lock);
        if(JMESH_BLE_STATUS_SLEEP==ble_adapter.status||JMESH_BLE_STATUS_INIT==ble_adapter.status){
            ble_adapter.adv.repeats--;
            ble_adapter.status=JMESH_BLE_STATUS_ADV;
            os_timer_event_set(&ble_adapter.timer,JMESH_ADV_MIN_MS,(os_timer_event_caller_t)jmesh_ble_adapter_timerflow,NULL);
            jmesh_ble_send_adv(ble_adapter.adv.ad_type,ble_adapter.adv.length,ble_adapter.adv.ad_data);
        }
        else if(JMESH_BLE_STATUS_SCAN==ble_adapter.status){
            os_timer_event_set(&ble_adapter.timer,JMESH_SCAN_MIN_MS,(os_timer_event_caller_t)jmesh_ble_adapter_timerflow,NULL);
        }
    }
}

void jmesh_connect_start(unsigned char* mac)
{
    if(JMESH_FEATURE_ENABLE!=jmesh_get_lowpower_feature()){
        if(JMESH_BLE_STATUS_CONNECT==ble_adapter.status){
            return;
        }
        memcpy(ble_adapter.connect.mac,mac,6);
        ble_adapter.connect.flag=1;
        if(JMESH_BLE_STATUS_SLEEP==ble_adapter.status){
            ble_adapter.connect.flag=0;
            ble_adapter.status=JMESH_BLE_STATUS_CONNECT;
            os_timer_event_set(&ble_adapter.timer,JMESH_CONNECT_MIN_MS,(os_timer_event_caller_t)jmesh_ble_adapter_timerflow,NULL);
            jmesh_ble_gatt_connect(ble_adapter.connect.mac);
        }
        else if(JMESH_BLE_STATUS_BUSY!=ble_adapter.status){
            ble_adapter.status=JMESH_BLE_STATUS_BUSY;
            jmesh_ble_cancel();
        }
    }
}

void jmesh_ble_canceled_callback(unsigned char state_canceled)
{
    if(ble_adapter.connect.flag){
        ble_adapter.connect.flag=0;
        ble_adapter.status=JMESH_BLE_STATUS_CONNECT;
        os_timer_event_set(&ble_adapter.timer,JMESH_CONNECT_MIN_MS,(os_timer_event_caller_t)jmesh_ble_adapter_timerflow,NULL);
        jmesh_ble_gatt_connect(ble_adapter.connect.mac);
    }
    else if(jmesh_ble_scan_is_enable()&&JMESH_BLE_STATUS_SCAN!=state_canceled){
        ble_adapter.status=JMESH_BLE_STATUS_SCAN;
        if(ble_adapter.adv.repeats){
            os_timer_event_set(&ble_adapter.timer,jmesh_random_get(JMESH_SCAN_MIN_MS,JMESH_SCAN_MAX_MS),(os_timer_event_caller_t)jmesh_ble_adapter_timerflow,NULL);
        }
        jmesh_ble_scan_adv();
    }
    else if(ble_adapter.adv.repeats){
        xSemaphoreTake(ble_adapter.adv.mutex_lock,portMAX_DELAY);
        ble_adapter.adv.repeats--;
        ble_adapter.status=JMESH_BLE_STATUS_ADV;
        jmesh_ble_send_adv(ble_adapter.adv.ad_type,ble_adapter.adv.length,ble_adapter.adv.ad_data);
        xSemaphoreGive(ble_adapter.adv.mutex_lock);
        os_timer_event_set(&ble_adapter.timer,JMESH_ADV_MIN_MS,(os_timer_event_caller_t)jmesh_ble_adapter_timerflow,NULL);
    }
    else{
        ble_adapter.status=JMESH_BLE_STATUS_SLEEP;
    }
}

int jmesh_gatt_queue_send(unsigned char success)
{
    int length=0;
    send_cache_t *p=&ble_adapter.gatt.send_cache;
    if(!success&&1==ble_adapter.gatt.this_id)
    {
        if(0xff!=p->backup_id)
        {
            ble_adapter.gatt.this_id=2;
            if(1==jmesh_gatt_trigger_send(p->backup_id,p->instance,p->length,p->data))
            {
                return 1;
            }
        }
    }
    while(1)
    {
        length=os_buffer_queue_pop(jmesh_gatt_queue,sizeof(send_cache_t),(unsigned char *)&ble_adapter.gatt.send_cache);
        if(0==length)
        {
            ble_adapter.gatt.is_busy=0;
            ble_adapter.gatt.this_id=0xff;
            return 0;
        }
        switch(p->send_type)
        {
            case JMESH_SEND_TYPE_ROUTE:
            case JMESH_SEND_TYPE_ALL:
              if(JMESH_SEND_TYPE_ALL==p->send_type){
                  jmesh_adv_send_message2(p->length-2,p->data+2);//note:proxy protocol head
              }
              ble_adapter.gatt.this_id=1;
              if(1==jmesh_gatt_trigger_send(p->id,p->instance,p->length,p->data))
              {
                  return 1;
              }
              else if(0xff!=p->backup_id)
              {
                  ble_adapter.gatt.this_id=2;
                  if(1==jmesh_gatt_trigger_send(p->backup_id,p->instance,p->length,p->data))
                  {
                      return 1;
                  }
              }
              break;
            case JMESH_SEND_TYPE_CONNECT:
              ble_adapter.gatt.this_id=3;
              if(1==jmesh_gatt_trigger_send(p->id,p->instance,p->length,p->data))
              {
                  return 1;
              }
              break;
            default:
              break;
        }
    }
}

void jmesh_adapter_gatt_send(unsigned char send_type,unsigned char id,unsigned char id_backup,unsigned short instance,unsigned short length,unsigned char *data)
{
    if(length>0&&length<=JMESH_BLE_MTU_SIZE)
    {
        send_cache_t send_cache;
        send_cache.send_type=send_type;
        send_cache.id=id;
        send_cache.backup_id=id_backup;
        send_cache.instance=instance;
        send_cache.length=length;
        memcpy(send_cache.data,data,length);
        if(0==os_buffer_queue_push(jmesh_gatt_queue,SEND_CACHE_DATA_OFFSET+length,(unsigned char *)&send_cache))
        {
            JMESH_LOGW(1,"routing send","send queue full\n");
        }
    }
    if(ble_adapter.gatt.is_busy==0){
        ble_adapter.gatt.is_busy=1;
        jmesh_gatt_queue_send(1);
    }
}

//#define APP_DIS_ADV_DATA                ("JBLE-J0101P")
#define GAP_LE_GEN_DISCOVERABLE_FLG             0x02
#define GAP_BR_EDR_NOT_SUPPORTED                0x04

void jmesh_ble_send_ota_adv(void)
{
    struct gapm_start_advertise_cmd_patch *cmd = AHI_MSG_ALLOC(GAPM_START_ADVERTISE_CMD,TASK_ID_GAPM, gapm_start_advertise_cmd_patch);
    cmd->op.addr_src    = GAPM_STATIC_ADDR;
    cmd->channel_map    = ADV_ALL_CHNLS_EN;
    cmd->intv_min = 320;//*0.625ms
    cmd->intv_max = 320;//*0.625ms
    cmd->op.code        = GAPM_ADV_UNDIRECT;
    cmd->info.host.mode = GAP_GEN_DISCOVERABLE;
    cmd->info.host.adv_data[0] = 0x2,
    cmd->info.host.adv_data[1] = GAP_AD_TYPE_FLAGS,
    cmd->info.host.adv_data[2] = GAP_LE_GEN_DISCOVERABLE_FLG|GAP_BR_EDR_NOT_SUPPORTED,
    cmd->info.host.adv_data_len = ADV_DATA_PACK(cmd->info.host.adv_data + 3,1,GAP_AD_TYPE_COMPLETE_NAME,\
            APP_DIS_ADV_DATA,sizeof(APP_DIS_ADV_DATA));
            // Flag value is set by the GAP
    cmd->info.host.adv_data_len       = ADV_DATA_LEN;
    cmd->info.host.scan_rsp_data_len  = SCAN_RSP_DATA_LEN;
//    nvds_get(NVDS_TAG_APP_BLE_ADV_DATA, &cmd->info.host.adv_data_len,&cmd->info.host.adv_data[0]) ;
//    nvds_get(NVDS_TAG_APP_BLE_SCAN_RESP_DATA, &cmd->info.host.scan_rsp_data_len,&cmd->info.host.scan_rsp_data[0]);
    osapp_ahi_msg_send(cmd, sizeof(struct gapm_start_advertise_cmd_patch),portMAX_DELAY);
}



int jmesh_ble_set_dbm(unsigned char dbm)
{
    unsigned char dbm_val=dbm;
    if(0!=dbm_val&&8!=dbm_val){
        return -1;
    }
    jmesh_save_write(JMESH_SAVE_DBM,1,&dbm_val);
    return 0;
}

unsigned char jmesh_ble_get_dbm(void)
{
    unsigned char dbm_val;
    if(1!=jmesh_save_read(JMESH_SAVE_DBM,1,&dbm_val)){
        dbm_val=0;
        jmesh_save_write(JMESH_SAVE_DBM,1,&dbm_val);
    }
    return dbm_val;
}




#endif
