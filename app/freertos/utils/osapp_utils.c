#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include "osapp_utils.h"
#include "co_utils.h"
#include "gattm_task.h"
#include "osapp_task.h"
#include "osapp_config.h"
#include "gattc_task.h"
/*
 * MACROS
 ****************************************************************************************
 */
//#define UTILS_BD_ADDR_VALIAD_MASK       0xC0//core v5.0   1.3.2.1
//#define UTILS_BD_ADDR_STATIC_MASK       0xC0//core v5.0   1.3.2.1
//#define UTILS_BD_ADDR_NPA_MASK          0x00//core v5.0   1.3.2.2
//#define UTILS_BD_ADDR_RPA_MASK          0x40//core v5.0   1.3.2.2

#define UTILS_SET_BD_ADDR_STATIC(addr)  \
                                        do{\
                                              addr[GAP_BD_ADDR_LEN-1] = addr[GAP_BD_ADDR_LEN-1]|GAP_STATIC_ADDR;\
                                          }while(0)
//===========================handler help
/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
typedef struct
{
    enum gap_role role;
    bd_addr_t static_addr;
    /// Address type of the device 0=public/1=private random
    uint8_t addr_type;
    struct gap_sec_key irk;
}osapp_utils_database_t;

/* LOCAL FUNCTIONS DEFINITIONS
****************************************************************************************
*/
static void osapp_gapc_conn_req_ind_handler(ke_msg_id_t const msgid, struct gapc_connection_req_ind const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static void osapp_gapc_peer_version_ind_handler(ke_msg_id_t const msgid,struct gapc_peer_version_ind const *param,ke_task_id_t const dest_id,ke_task_id_t src_id);
static void osapp_gapc_peer_feature_ind_handler(ke_msg_id_t const msgid,struct gapc_peer_features_ind const *param,ke_task_id_t const dest_id,ke_task_id_t src_id);
static void osapp_device_ready_ind_handler(ke_msg_id_t const msgid, void const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static void osapp_gapm_cmp_evt_handler(ke_msg_id_t const msgid, struct gapm_cmp_evt const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static void osapp_set_dev_config(void);
/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
static osapp_utils_database_t l_utils={
        .role = GAP_ROLE_NONE,
        .addr_type = GAPM_CFG_ADDR_PUBLIC,
        .static_addr = {0},
        .irk = {0},
};
static osapp_msg_handler_table_t const handler_table[]=
{
    {GAPM_DEVICE_READY_IND,(osapp_msg_handler_t)osapp_device_ready_ind_handler},
    {GAPM_CMP_EVT,(osapp_msg_handler_t)osapp_gapm_cmp_evt_handler},
    {GAPC_CONNECTION_REQ_IND,(osapp_msg_handler_t)osapp_gapc_conn_req_ind_handler},
    {GAPC_PEER_VERSION_IND, (osapp_msg_handler_t)osapp_gapc_peer_version_ind_handler},
    {GAPC_PEER_FEATURES_IND,(osapp_msg_handler_t)osapp_gapc_peer_feature_ind_handler},
};
static osapp_msg_handler_info_t  handler_info = HANDLER_ARRAY_INFO(handler_table);


/*
 * FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */
uint8_t * adv_data_pack(uint8_t *buf,uint8_t field_nums,...)
{
    va_list list;
    uint8_t i;
    va_start(list,field_nums);
    for(i=0;i<field_nums;++i)
    {
        uint8_t ad_type = va_arg(list,uint32_t);
        uint8_t *ad_data = va_arg(list,uint8_t *);
        uint8_t ad_data_length = va_arg(list,uint32_t);
        buf[0] = ad_data_length + 1;
        buf[1] = ad_type;
        memcpy(&buf[2],ad_data,ad_data_length);
        buf += buf[0] + 1;
    }
    va_end(list);
    return buf;
}
/**
 ****************************************************************************************
 * @brief  tools for random generate.
 *
 * @param[out] p_out      Pointer to the random data.
 * @param[in]  len        generate random length in byte.
 *
 ****************************************************************************************
 */
void osapp_utils_random_generate(uint8_t *p_out,uint8_t len)
{
    uint32_t tmp=0;
    uint8_t random_idx = 0;
    uint8_t random_remain = 0;


    while(random_idx <len)
    {
        tmp = rand();
        random_remain = len - random_idx;

        if(random_remain>=4)
        {
            memcpy((p_out+random_idx),(uint8_t *)&tmp,4);
            random_idx += 4;
        }
        else
        {
            memcpy((p_out+random_idx),(uint8_t *)&tmp,random_remain);
            random_idx += random_remain;
        }
    }
}
/**
 ****************************************************************************************
 * @brief   Is all of the array elements set to a value.
 *
 * @param[in]   buf - buffer to check
 * @param[in]   val - value to check each array element for
 * @param[in]   len - length to check
 *
 * @return  TRUE if all "val", FALSE otherwise
 ****************************************************************************************
 */
bool osapp_utils_is_buffer_set( uint8_t *buf, uint8_t val, uint8_t len )
{
  uint8_t x;

  if ( buf == NULL )
  {
    return ( false );
  }

  for ( x = 0; x < len; x++ )
  {
    // Check for non-initialized value
    if ( buf[x] != val )
    {
      return ( false );
    }
  }
  return ( true );
}
/**
 ****************************************************************************************
 * @brief   Func osapp_utils_set_dev_init
 *
 * @param[in] role           The role of the device.
 * @param[in] addr_type      Address type of the device 0=public/1=private random
 *
 *                           /// Device Address is a Public Static address
 *                           GAPM_CFG_ADDR_PUBLIC        = 0,
 *                           /// Device Address is a Private Static address
 *                           GAPM_CFG_ADDR_PRIVATE       = 1,
 *                           /// Device Address generated using host-based Privacy feature
 *                           GAPM_CFG_ADDR_HOST_PRIVACY  = 2,
 *                           /// Device Address generated using controller-based Privacy feature
 *                           GAPM_CFG_ADDR_CTNL_PRIVACY  = 4,
 *
 ****************************************************************************************
 */
void osapp_utils_set_dev_init(enum gap_role role,uint8_t addr_type)
{
    LOG(LOG_LVL_INFO , "osapp_utils_set_dev_init,role=%d\n",role);

    l_utils.role = role;
    l_utils.addr_type = addr_type;
    uint8_t addr_len = GAP_BD_ADDR_LEN;
    if(nvds_get(NVDS_TAG_STATIC_DEV_ADDR,&addr_len,l_utils.static_addr.addr)!=NVDS_OK)
    {
        osapp_utils_random_generate(l_utils.static_addr.addr,GAP_BD_ADDR_LEN);//random addr;
        nvds_put(NVDS_TAG_STATIC_DEV_ADDR,addr_len,l_utils.static_addr.addr);
    }
    UTILS_SET_BD_ADDR_STATIC(l_utils.static_addr.addr);//set mask
    osapp_utils_random_generate(l_utils.irk.key,GAP_KEY_LEN);//random irk
    //register handle table
    ahi_handler_register(&handler_info);
}
/**
 ****************************************************************************************
 * @brief   Func osapp_utils_get_params
 *
 * @param[in]  type           The type osapp utils params type.
 * @param[out] p_param        The point of the params.
 *
 ****************************************************************************************
 */
void osapp_utils_get_params(osapp_utils_param_type_t type,osapp_utils_param_t *p_param)
{
    if(p_param)
    {
        switch(type)
        {
            case OSAPP_UTILS_TYPE_GET_BD_ADDR :
                p_param->addr.addr_type = l_utils.addr_type;
                p_param->addr.addr = l_utils.static_addr;
                break;
            case OSAPP_UTILS_TYPE_GET_IRK:
                p_param->irk = l_utils.irk;
                break;
            default:break;
        }
    }
}
/**
 ****************************************************************************************
 * @brief  tools for printf to segger rtt log data.(temp buff use malloc)
 *
 * @param[in]  data      Pointer to the data to be printf to segger rtt log.
 * @param[in]  length    The length of data to be printf.
 *
 ****************************************************************************************
 */
void osapp_utils_log_hex_data(const uint8_t * data , uint16_t length)
{
    static const uint8_t l_osapp_utils_log_hex_tab[] = "0123456789ABCDEF";

    uint8_t tmp_h,tmp_l;
    uint8_t *mesh_log_format_buff=0;
    uint8_t *bufptr=0;
    uint32_t total_length;
    //init
    total_length = length * 2 + 1;
    mesh_log_format_buff = pvPortMalloc(total_length);
    bufptr = mesh_log_format_buff;
    //content
    for(uint16_t i=0;i<length;i++)
    {
        tmp_h = data[i] >> 4;
        tmp_l = data[i] & 0x0F;
        *bufptr = l_osapp_utils_log_hex_tab[tmp_h];  bufptr++;
        *bufptr = l_osapp_utils_log_hex_tab[tmp_l];  bufptr++;
    }
    //end
    *bufptr = '\n'; bufptr ++;
    //print
    SEGGER_RTT_Write(0,mesh_log_format_buff,total_length);
    #if (USE_INTERNAL_LOG == 1)
    internal_log_write(mesh_log_format_buff,total_length);
    #endif
    //free
    vPortFree(mesh_log_format_buff);
}





//static function

static void send_get_version(uint16_t conn_idx)
{
    struct gapc_get_info_cmd* cmd = AHI_MSG_ALLOC(GAPC_GET_INFO_CMD,KE_BUILD_ID(TASK_ID_GAPC, conn_idx),gapc_get_info_cmd);
    cmd->operation = GAPC_GET_PEER_VERSION;
    osapp_ahi_msg_send(cmd, sizeof(struct gapc_get_info_cmd),portMAX_DELAY);
}

static void send_get_feature(uint16_t conn_idx)
{
    struct gapc_get_info_cmd* cmd = AHI_MSG_ALLOC(GAPC_GET_INFO_CMD,KE_BUILD_ID(TASK_ID_GAPC, conn_idx),gapc_get_info_cmd);
    cmd->operation = GAPC_GET_PEER_FEATURES;
    osapp_ahi_msg_send(cmd, sizeof(struct gapc_get_info_cmd),portMAX_DELAY);
}


static void osapp_gapc_peer_version_ind_handler(ke_msg_id_t const msgid,struct gapc_peer_version_ind const *param,ke_task_id_t const dest_id,ke_task_id_t src_id)
{
    LOG(3,"osapp_gapc_peer_version_ind_handler\r\n");
}

static void osapp_gapc_peer_feature_ind_handler(ke_msg_id_t const msgid,struct gapc_peer_features_ind const *param,ke_task_id_t const dest_id,ke_task_id_t src_id)
{
    LOG(3,"osapp_gapc_peer_feature_ind_handler\r\n");
    send_get_version(KE_IDX_GET(src_id));
}

static void osapp_gapc_conn_req_ind_handler(ke_msg_id_t const msgid, struct gapc_connection_req_ind const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    LOG(3,"osapp_gapc_conn_req_ind_handler\r\n");
    send_get_feature(KE_IDX_GET(src_id));
}

static void osapp_device_ready_ind_handler(ke_msg_id_t const msgid, void const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gapm_reset_cmd *cmd = AHI_MSG_ALLOC(GAPM_RESET_CMD,TASK_ID_GAPM,gapm_reset_cmd);
    cmd->operation = GAPM_RESET;
    osapp_ahi_msg_send(cmd, sizeof(struct gapm_reset_cmd),portMAX_DELAY);
}
static void osapp_gapm_cmp_evt_handler(ke_msg_id_t const msgid, struct gapm_cmp_evt const * param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gapm_cmp_evt const *cmp_evt = param;

    switch(cmp_evt->operation)
    {
    case GAPM_RESET:
        BX_ASSERT(cmp_evt->status==GAP_ERR_NO_ERROR);
        osapp_set_dev_config();
        break;
    default:
        break;
    }
}
static void osapp_set_dev_config(void)
{
    // Set Device configuration
    struct gapm_set_dev_config_cmd* cmd = AHI_MSG_ALLOC(GAPM_SET_DEV_CONFIG_CMD,TASK_ID_GAPM,gapm_set_dev_config_cmd);
    cmd->operation = GAPM_SET_DEV_CONFIG;
    cmd->role      = l_utils.role;
    //privacy configuration
    cmd->renew_dur = GAP_TMR_PRIV_ADDR_INT;
    cmd->addr = l_utils.static_addr;
    cmd->irk = l_utils.irk;
    cmd->addr_type = l_utils.addr_type;
    //security configuration
    cmd->pairing_mode = GAPM_PAIRING_LEGACY|GAPM_PAIRING_SEC_CON ;
    //attribute database configuration
    cmd->gap_start_hdl = 0;
    cmd->gatt_start_hdl = 0;
    cmd->att_cfg = GAPM_MASK_ATT_SVC_CHG_EN | GAPM_MASK_ATT_SLV_PREF_CON_PAR_EN;
    //LE Data Length Extension configuration
    cmd->sugg_max_tx_octets = BLE_MIN_OCTETS;
    cmd->sugg_max_tx_time   = BLE_MIN_TIME;
    //L2CAP Configuration
    cmd->max_mps = GAP_MAX_LE_MTU;
    cmd->max_mtu = GAP_MAX_LE_MTU;
    cmd->max_nb_lecb = 0;
    //LE Audio Mode Supported
    cmd->audio_cfg = 0;//GAPM_MASK_AUDIO_AM0_SUP;
    //LE PHY Management
    cmd->tx_pref_rates = GAP_RATE_LE_1MBPS;
    cmd->rx_pref_rates = GAP_RATE_LE_1MBPS;

    osapp_ahi_msg_send(cmd, sizeof(struct gapm_set_dev_config_cmd),portMAX_DELAY);
}


