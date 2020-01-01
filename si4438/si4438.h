//---------------------------------------------------
// file name : si4438.h
//--------------------------------------------------- 
#ifndef SI4438_H
#define SI4438_H
#include"si446x_defs.h"
#include"stdarg.h"

#define RADIO_CTS_TIMEOUT 5000

#define LSB 0
#define MSB 1

enum
{
    SI446X_SUCCESS,
    SI446X_NO_PATCH,
    SI446X_CTS_TIMEOUT,
    SI446X_PATCH_FAIL,
    SI446X_COMMAND_ERROR
};

extern  union si446x_cmd_reply_union Si446xCmd;
extern U8 Pro2Cmd[16];
extern U8* pPositionInPayload;
extern U8 bPositionInPayload;

extern U8 CURRENT_CHANNEL;
extern uchar Timer400ms;//qian dao ma shi xiao time
extern uchar change_channal_flag;
extern U8 ctsWentHigh ;

#if 0
const U8 driect_config[]={
        0x07, 0x02, 0x81, 0x00, 0x01, 0xC9, 0xC3, 0x80,//RF_POWER_UP,
        0x08, 0x13, 0x14, 0x00, 0x00, 0x00, 0x11, 0x0B,0x00, // RF_GPIO_PIN_CFG,
        0x05, 0x11, 0x00, 0x01, 0x00, 0x40, //RF_GLOBAL_XO_TUNE_1,
        0x05, 0x11, 0x00, 0x01, 0x03, 0x60,//RF_GLOBAL_CONFIG_1,
        0x05, 0x11, 0x01, 0x01, 0x00, 0x00,//RF_INT_CTL_ENABLE_1,
        0x08, 0x11, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,//RF_FRR_CTL_A_MODE_4,
        0x05, 0x11, 0x10, 0x01, 0x01, 0x14,//RF_PREAMBLE_CONFIG_STD_1_1, 
        0x05, 0x11, 0x12, 0x01, 0x06, 0x40,//RF_PKT_CONFIG1_1,
        0x10, 0x11, 0x20, 0x0C, 0x00, 0x2B, 0x00, 0x07, 0x06, 0x1A, 0x80, 0x05, 0xC9, 0xC3, 0x80, 0x00, 0x06,//RF_MODEM_MOD_TYPE_12, 
        0x05, 0x11, 0x20, 0x01, 0x0C, 0xD4,//RF_MODEM_FREQ_DEV_0_1,
        0x0C, 0x11, 0x20, 0x08, 0x18, 0x01, 0x80, 0x08, 0x03, 0x80, 0x00, 0x20, 0x20,//RF_MODEM_TX_RAMP_DELAY_8, 
        0x0D, 0x11, 0x20, 0x09, 0x22, 0x01, 0x77, 0x01, 0x5D, 0x86, 0x00, 0xAF, 0x00, 0xC2,//RF_MODEM_BCR_OSR_1_9, 
        0x0B, 0x11, 0x20, 0x07, 0x2C, 0x54, 0x36, 0x81, 0x55, 0x01, 0x86, 0x80,//RF_MODEM_AFC_GEAR_7, 
        0x05, 0x11, 0x20, 0x01, 0x35, 0xE2,//RF_MODEM_AGC_CONTROL_1, 
        0x07, 0x11, 0x20, 0x03, 0x38, 0x11, 0x52, 0x52,//RF_MODEM_AGC_WINDOW_SIZE_3, 
        0x05, 0x11, 0x20, 0x01, 0x40, 0x2A,//RF_MODEM_OOK_PDTC_1, 
        0x0F, 0x11, 0x20, 0x0B, 0x42, 0x84, 0x03, 0xD6, 0x8F, 0x00, 0xD8, 0x01, 0x80, 0xFF, 0x0C, 0x00,//RF_MODEM_OOK_CNT1_11, 
        0x05, 0x11, 0x20, 0x01, 0x4E, 0x40,//RF_MODEM_RSSI_COMP_1, 
        0x05, 0x11, 0x20, 0x01, 0x51, 0x0A,//RF_MODEM_CLKGEN_BAND_1, 
        0x10, 0x11, 0x21, 0x0C, 0x00, 0xCC, 0xA1, 0x30, 0xA0, 0x21, 0xD1, 0xB9, 0xC9, 0xEA, 0x05, 0x12, 0x11,//RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12, 
        0x10, 0x11, 0x21, 0x0C, 0x0C, 0x0A, 0x04, 0x15, 0xFC, 0x03, 0x00, 0xCC, 0xA1, 0x30, 0xA0, 0x21, 0xD1,//RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12, 
        0x10, 0x11, 0x21, 0x0C, 0x18, 0xB9, 0xC9, 0xEA, 0x05, 0x12, 0x11, 0x0A, 0x04, 0x15, 0xFC, 0x03, 0x00,//RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12, 
        0x0B, 0x11, 0x21, 0x0C, 0x18, 0xB9, 0xC9, 0xEA, 0x05, 0x12, 0x11, 0x0A, 0x04, 0x15, 0xFC, 0x03, 0x00,//RF_SYNTH_PFDCP_CPFF_7, 
        0x0C, 0x11, 0x40, 0x08, 0x00, 0x3E, 0x0B, 0x25, 0x8B, 0x36, 0x9D, 0x20, 0xFE,//RF_FREQ_CONTROL_INTE_8, //475.45MHz 
        0x00 
 };
#endif

/*****************************************************************************/

void vRadio_PowerUp(void);
void si446x_part_info(void);
void radio_comm_SendCmd(U8 byteCount, U8* pData);
U8 radio_comm_PollCTS(void);
U8 radio_comm_GetResp(U8 byteCount, U8* pData);
void radio_hal_SpiWriteData(U8 biDataInLength, U8 *pabiDataIn);
void radio_hal_SpiReadData(U8 biDataOutLength, U8 *paboDataOut);
U8 radio_comm_SendCmdGetResp(U8 cmdByteCount, U8* pCmdData, U8 respByteCount, U8* pRespData);
void si446x_reset(void);
//void si446x_power_up(U8 BOOT_OPTIONS, U8 XTAL_OPTIONS, U32 XO_FREQ);
U8 si446x_configuration_init(const U8* pSetPropCmd);
U8 si446x_apply_patch(void);
void si446x_part_info(void);
void si446x_start_tx(U8 CHANNEL, U8 CONDITION, U16 TX_LEN);
void si446x_start_rx(U8 CHANNEL, U8 CONDITION, U16 RX_LEN, U8 NEXT_STATE1, U8 NEXT_STATE2, U8 NEXT_STATE3);
void si446x_nop(void);

extern void si446x_set_property( U8 GROUP, U8 NUM_PROPS, U8 START_PROP, ... );//she zhi shu xin
extern void si446x_get_property(U8 GROUP, U8 NUM_PROPS, U8 START_PROP);//cha xun shu xin
extern void si446x_get_int_status(U8 PH_CLR_PEND, U8 MODEM_CLR_PEND, U8 CHIP_CLR_PEND);//qu zhong duan
extern void si446x_gpio_pin_cfg(U8 G_GPIO0, U8 G_GPIO1, U8 G_GPIO2, U8 G_GPIO3, U8 G_NIRQ, U8 G_SDO, U8 G_GEN_CONFIG);
extern void si446x_change_state(U8 NEXT_STATE1);//gai bian zhuan tai
extern void  vRadio_StartTx(U8 channel, U8 *pioFixRadioPacket,U8 TX_LEN);

extern  void vRadio_StartRX(U8 channel);
extern void vRadio_Init(void);//RF chu shi hua
extern void si446x_get_modem_status( U8 MODEM_CLR_PEND );//she bei zhuang tai
extern U8 si446x_request_device_state(void);//huo qu dang qian zhuang tai he xingdao zhi
extern void PA_power_set(uchar power_lvevl);// she zhi fa she gong lv
extern U8 vRadio_si4438Patch(void);

extern uchar PA_power_get(void);
extern void crystal_frequency_tune(uchar xo_value);//jin zheng she zhi
extern uchar crystal_frequency_get(void);
extern uchar curr_rssi_get(void);
extern uchar ByteRead(void);
extern void ByteSend(uchar i);

extern BIT gRadio_CheckReceived(uchar* SYNC_RSSI);
extern BIT gRadio_CheckTransmitted(U8 *pioFixRadioPacket);
//extern U8 RF_FIFO_RX(void);
extern U8  RF_FIFO_TX(uchar TX_ChannelNumber,uchar *Txbuff,uchar TX_Len);

void radio_comm_WriteData(U8 cmd, BIT pollCts, U8 byteCount, U8* pData);
void si446x_fifo_info(U8 FIFO);
void si446x_write_tx_fifo( U8 numBytes, U8* pData );
void si446x_read_rx_fifo( U8 numBytes, U8* pRxData );
void si446x_get_packet_info(U8 FIELD_NUMBER_MASK, U16 LEN, S16 DIFF_LEN );
//void si446x_get_ph_status(U8 PH_CLR_PEND);


//void si446x_func_info(void);
void radio_comm_ReadData(U8 cmd, BIT pollCts, U8 byteCount, U8* pData);
void si446x_frr_a_read(U8 respByteCount);
void si446x_frr_b_read(U8 respByteCount);
void si446x_frr_c_read(U8 respByteCount);
void si446x_frr_d_read(U8 respByteCount);

#endif	//SI4438_H
//---------------------------------------------------
// end of file
//---------------------------------------------------
