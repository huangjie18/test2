//int
#include "_470_phy.h"
#include "si4438.h"
#include "radio_config.h"
#include "si4438_hal.h"
#include "jmesh_gpio.h"
#include "os_queue.h"
#include "jmesh_app_pro.h"
#include "jmesh_uart.h"
#include "task.h"
#include "jmesh_system.h"
extern int wdt_regist_id;

phy_tx_packet_t phy_tx_packet;
phy_rx_packet_t phy_rx_packet;
phy_pib_t phy_pib;
static uchar SWICTCH_LIST[EACH_CHANNEL_GROUP_NUM * 2] = {0};
state_flag_t StateFlag = {0};
uint8_t Rf_CREL;    // 用于通过调试寻找si4438最佳的晶振校准值
static uint8_t FixChannel; // 固定信道号 
uint8_t rx_channel; 


static const uchar phyChannelGroupSupported[CHANNEL_GROUP_NUM][EACH_CHANNEL_GROUP_NUM]={
											{35,75},{ 0,39},{ 1,40},{ 2,41},{ 3,42},{ 4,43},
											{ 5,44},{ 6,45},{ 7,46},{ 8,47},{ 9,48},{10,49},
											{11,50},{12,51},{13,52},{14,53},{15,54},{16,55},
											{17,56},{18,57},{19,58},{20,59},{21,60},{22,61},
											{23,62},{25,63},{26,64},{27,65},{28,67},{29,68},
											{30,69},{31,70},{32,71}
										};



const U8 driect_config[]={
        0x07, 0x02, 0x81, 0x00, 0x01, 0xC9, 0xC3, 0x80,//RF_POWER_UP,
        0x08, 0x13, 0x14, 0x00, 0x00, 0x00, 0x11, 0x0B,0x00, // RF_GPIO_PIN_CFG,
        0x05, 0x11, 0x00, 0x01, 0x00, XO_TUNE, //RF_GLOBAL_XO_TUNE_1,
        0x05, 0x11, 0x00, 0x01, 0x03, 0x60,//RF_GLOBAL_CONFIG_1,
        0x05, 0x11, 0x01, 0x01, 0x00, 0x00,//RF_INT_CTL_ENABLE_1,
        0x08, 0x11, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,//RF_FRR_CTL_A_MODE_4,
        0x05, 0x11, 0x10, 0x01, 0x01, 0x88,//RF_PREAMBLE_CONFIG_STD_1_1, 
        0x05, 0x11, 0x12, 0x01, 0x06, 0x40,//RF_PKT_CONFIG1_1,
        0x10, 0x11, 0x20, 0x0C, 0x00, 0x2B, 0x00, 0x07, 0x06, 0x1A, 0x80, 0x05, 0xC9, 0xC3, 0x80, 0x00, 0x06,//RF_MODEM_MOD_TYPE_12, 
        0x05, 0x11, 0x20, 0x01, 0x0C, 0xD4,//RF_MODEM_FREQ_DEV_0_1,
        0x0C, 0x11, 0x20, 0x08, 0x18, 0x01, 0x80, 0x08, 0x03, 0x80, 0x00, 0x20, 0x20,//RF_MODEM_TX_RAMP_DELAY_8, 
        0x0D, 0x11, 0x20, 0x09, 0x22, 0x01, 0x77, 0x01, 0x5D, 0x86, 0x00, 0xAF, 0x02, 0xC2,//RF_MODEM_BCR_OSR_1_9, 
        0x0B, 0x11, 0x20, 0x07, 0x2C, 0x04, 0x36, 0x00, 0x1D, 0x11, 0xEC, 0x80,//RF_MODEM_AFC_GEAR_7, 
        0x05, 0x11, 0x20, 0x01, 0x35, 0xE2,//RF_MODEM_AGC_CONTROL_1, 
        0x07, 0x11, 0x20, 0x03, 0x38, 0x11, 0x52, 0x52,//RF_MODEM_AGC_WINDOW_SIZE_3, 
        0x05, 0x11, 0x20, 0x01, 0x40, 0x2A,//RF_MODEM_OOK_PDTC_1, 
        0x0F, 0x11, 0x20, 0x0B, 0x42, 0xA4, 0x02, 0xD6, 0x83, 0x00, 0xD8, 0x01, 0x80, 0xFF, 0x0C, 0x00,//RF_MODEM_OOK_CNT1_11,     
        0x05, 0x11, 0x20, 0x01, 0x4E, 0x40,//RF_MODEM_RSSI_COMP_1, 
        0x05, 0x11, 0x20, 0x01, 0x51, 0x0A,//RF_MODEM_CLKGEN_BAND_1, 
        0x10, 0x11, 0x21, 0x0C, 0x00, 0xCC, 0xA1, 0x30, 0xA0, 0x21, 0xD1, 0xB9, 0xC9, 0xEA, 0x05, 0x12, 0x11,//RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12, 
        0x10, 0x11, 0x21, 0x0C, 0x0C, 0x0A, 0x04, 0x15, 0xFC, 0x03, 0x00, 0xCC, 0xA1, 0x30, 0xA0, 0x21, 0xD1,//RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12, 
        0x10, 0x11, 0x21, 0x0C, 0x18, 0xB9, 0xC9, 0xEA, 0x05, 0x12, 0x11, 0x0A, 0x04, 0x15, 0xFC, 0x03, 0x00,//RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12, 
		0x08, 0x11, 0x22, 0x04, 0x00, 0x08, 0x7F, 0x00, 0x3D,//RF_PA_MODE_4
		0x0B, 0x11, 0x23, 0x07, 0x00, 0x2C, 0x0E, 0x0B, 0x04, 0x0C, 0x73, 0x03,//RF_SYNTH_PFDCP_CPFF_7, 
        0x0C, 0x11, 0x40, 0x08, 0x00, 0x3E, 0x0B, 0x25, 0x8B, 0x36, 0x9D, 0x20, 0xFE,//RF_FREQ_CONTROL_INTE_8, //475.45MHz 
	//	0x0C, 0x11, 0x40, 0x08, 0x00, 0x3D, 0x0E, 0xEE, 0xEE, 0x36, 0x9D, 0x20, 0xFE,//RF_FREQ_CONTROL_INTE_8, //471.5MHz
        0x00 
 };



/* return check result*/
#define LOW_RSSI	48	
#define HIGH_RSSI 	160
#define RSSI_DBM	130
#define LOW_DBM		50
#define HIGH_DBM	120
#define IDLE_DBM	96

void rx_hop_control()
{
	if((1 == StateFlag.fChangeChannel)&&(!StateFlag.fFixChannel))
		si446x_set_property(0x50,0x06,0x00,0x34,0x04,SWICTCH_LIST[0],SWICTCH_LIST[1],SWICTCH_LIST[2],SWICTCH_LIST[3]);
	else
		si446x_set_property(0x50,0x03,0x00,0x04,0x01);
}


unsigned short Whiting_Data(unsigned char *data, unsigned short Whiting_PN9)
{ 
	unsigned char i, c; 
	*data ^= (uint8)(Whiting_PN9&0xFF);  
	for(i=0; i<8; i++)
	{ 
		c =(uint8) (Whiting_PN9 & 0x21); 
		Whiting_PN9 >>= 1;  
		if ((c==0x21)||(c==0))
		{ 
			Whiting_PN9 &= 0x0FF; 
		}
		
		else
		{ 
			Whiting_PN9 |= 0x100; 
		} 
	} 
	return Whiting_PN9; 
} 

void Whiting_Buff(unsigned char *Buff,  unsigned int len)
{  
	unsigned short Whiting_PN9; 
	Whiting_PN9 = 0x1FF;  
	while (len-- )
	{ 
		Whiting_PN9 = Whiting_Data(Buff++, Whiting_PN9);  
	}
}


static uchar tran_rssi_to_db(ushort rssi)
{ 
	uchar dbm;
    /* rssi -> dbm */
    if(rssi > HIGH_RSSI)
    {
    	dbm = LOW_DBM;			
    }
	else if (rssi > LOW_RSSI)
	{
		//dbm = HIGH_DBM - (uchar)((rssi- LOW_RSSI)*(HIGH_DBM-LOW_DBM )/(HIGH_RSSI - LOW_RSSI));
		dbm = RSSI_DBM - (uchar)rssi/2;
	}
	else
	{
		dbm = HIGH_DBM;
	}
	return dbm;
}


void phy_init()
{
	/* protocal stack int*/
	memset(&phy_tx_packet, 0, sizeof(phy_tx_packet_t));
	memset(&phy_rx_packet, 0, sizeof(phy_rx_packet_t));
	phy_pib.phyCurrentChannelIndex = 0;
	phy_pib.phyCurrentChannelGroup = 0;
	StateFlag.fChangeChannel = 1;

	/* rtx init*/
	{
		uint8_t channel_group = (JZQ_communication_address[0] / 0x10) * 10 + (JZQ_communication_address[0] % 0x10);
		channel_group = channel_group % CHANNEL_GROUP_NUM + 1;
		phy_set_channel_group(channel_group);                             // 初始化私有信道组  		
		phy_set_channel(PRIVATE_CHANNEL, phy_pib.phyCurrentChannelIndex); // 初始化当前发射信道号为私有信道  
	}
	
	vRadio_Init();
	rx_hop_control();
	vRadio_StartRX(CURRENT_CHANNEL);       	
}

void phy_set_channel_group(uchar channel_group)
{
	phy_pib.phyCurrentChannelGroup = channel_group;
	
	SWICTCH_LIST[0] = phyChannelGroupSupported[0][0];
	SWICTCH_LIST[1] = phyChannelGroupSupported[0][1];
	SWICTCH_LIST[2] = phyChannelGroupSupported[phy_pib.phyCurrentChannelGroup][0];
	SWICTCH_LIST[3] = phyChannelGroupSupported[phy_pib.phyCurrentChannelGroup][1];
	rx_hop_control();
	/*SET SCAN RX CHANNEL LIST*/
}

void phy_set_channel(channelType type, uchar index )
{
	uchar which;
	which = index % EACH_CHANNEL_GROUP_NUM;

	phy_pib.phyCurrentChannelIndex = which;
	switch(type)
	{	
		case PUBLIC_CHANNEL:
			CURRENT_CHANNEL = phyChannelGroupSupported[0][which];
			break;
		case PRIVATE_CHANNEL:
			CURRENT_CHANNEL = phyChannelGroupSupported[phy_pib.phyCurrentChannelGroup][which];
			break;
		default:
			break;
	}
}

uchar phy_get_channel()
{
	return phy_pib.phyCurrentChannelIndex;
}

void phy_lock_channel()
{
	StateFlag.fChangeChannel = 0;
	rx_hop_control();
}

void phy_unlock_channel()
{
	StateFlag.fChangeChannel = 1;
	rx_hop_control();
}

void phy_fix_channel(uint8 channel)
{
	FixChannel = channel;
	StateFlag.fFixChannel = 1;
	rx_hop_control();	
	vRadio_StartRX(FixChannel);	
}

void phy_unfix_channel(void)
{		
	StateFlag.fFixChannel=0;
	rx_hop_control();
	CURRENT_CHANNEL	= phyChannelGroupSupported[phy_pib.phyCurrentChannelGroup][phy_pib.phyCurrentChannelIndex];
	vRadio_StartRX(CURRENT_CHANNEL);	
}

uint8_t waiting_for_channel_idle(uint8_t txchannel)
{
	uint8_t i;
	vRadio_StartRX(txchannel);
	vTaskDelay(os_time_ms_to_ticks(jmesh_random_get(40, 80)));
	for (i=0;i<30;i++)
	{
		if(Read_SI4438_nIRQ() == 0)
		{
			si446x_get_int_status(0u, 0u, 0u);
			if((Si446xCmd.GET_INT_STATUS.MODEM_PEND & SI446X_CMD_GET_INT_STATUS_REP_SYNC_DETECT_BIT) ||
				(Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_RX_FIFO_ALMOST_FULL_BIT))
			{
				vRadio_StartRX(txchannel);
				vTaskDelay(os_time_ms_to_ticks(jmesh_random_get(40, 80)));
				continue;
			}
			else
			{
				return 1;			
			}
		}
		else
		{
			return 1;			
		}
	}
	return 0;
}

tx_status phy_tx_pkt(const uint8_t *data, uint8_t len, uint8_t response_flag)
{
	uchar return_valu=0;
	uchar txchannel=0;
		
	phy_tx_packet.dat[0] = len - 2;
	phy_tx_packet.len = len + 1;
	if((len + 1) > sizeof(phy_tx_packet.dat))
		return INVALID_PARAMETER;
	memcpy(phy_tx_packet.dat+1,data,len);
	Whiting_Buff((uint8_t *)phy_tx_packet.dat,phy_tx_packet.len);		
	if(!StateFlag.fFixChannel)
		txchannel = response_flag ? rx_channel : CURRENT_CHANNEL;
	else
		txchannel =  FixChannel;
	if(waiting_for_channel_idle(txchannel))
	{
		return_valu = RF_FIFO_TX(txchannel,(uint8_t *)phy_tx_packet.dat,phy_tx_packet.len);
		if(return_valu==1)return TX_SUCCESS;			 																								// 返回发送结果
		else return TRANSACTION_EXPIRED;
	}
	else
	{
		return CHANNEL_ACCESS_FAILURE;
	}		
}

void phy_tx_test(void)
{
	uchar test_data[] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A};
	phy_tx_pkt(test_data, sizeof(test_data),0);
}


uchar* phy_rx_pkt(uchar *rcv_len, uchar *rcv_rssi)
{
	uchar SYNC_RSSI = 0;
	
	if(1 != gRadio_CheckReceived(&SYNC_RSSI))
	{
		return NULL;
	}
	phy_rx_packet.rssi = tran_rssi_to_db(SYNC_RSSI);
	Whiting_Buff(phy_rx_packet.dat,phy_rx_packet.len);

	vRadio_StartRX(CURRENT_CHANNEL);
	 			
	*rcv_len = phy_rx_packet.len - 1;
	*rcv_rssi =  phy_rx_packet.rssi;	
	
	return phy_rx_packet.dat + 1;
}



void Write_TX_Power_Ceshi(uchar llc_TXPower)
{
	uchar TXPower_levels[]={0x7f,45,20,10};//20dbm 17.5dbm  10.5dbm  4.5dbm
	//uchar TXPower_levels[]={35,18,9,5};//17.5dbm  10.5dbm  4.5dbm -2dbm
	//uchar TXPower_levels[]={35,35,35,35};//17.5dbm  10.5dbm  4.5dbm -2dbm
	if(llc_TXPower<4)  llc_TXPower = TXPower_levels[llc_TXPower];
	else               llc_TXPower = TXPower_levels[0];			
	PA_power_set(llc_TXPower);			
}

void test_stream_data(uchar stream,uchar channel, uchar time_s, uint8 tx_power)
{
	uchar i;
	uint32_t ticks;
	TickType_t xTime0;
	const uchar M_data[]={0x00,0xFF,0xa5,0xa5,0x55,0xd5};//M0-0,M1-1,M2-PN9,M3-A5,M4-0X55,M5-D5
	
	if(stream > 5) 
		stream = 4;

	Write_TX_Power_Ceshi(tx_power);	
	
	si446x_gpio_pin_cfg(0x13,0x00,0x00,0x00,0x00,0x0B,0x00); //CE SHI GPIO0 BPS
	si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_TX_BIT);
	si446x_set_property(0x12, 0x08, 0x0d, 0x1f, 0xff, 0x04, 0x80, 0x1f, 0xff, 0x04, 0x80);// ptk=5bank
	si446x_set_property(0x12, 0x0C, 0x15, 0x1f, 0xff, 0x04, 0x80, 0x1f, 0xff, 0x04, 0x80,0x1f, 0xff, 0x04, 0x80);

	si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
	si446x_get_int_status(0u, 0u, 0u);
	if(0x04 != stream)
	{
		si446x_set_property(0x10, 0x01, 0x00, 0x00); //preamble= 0
		si446x_set_property(0x11, 0x01, 0x00, 0x80);//syncword= 0
		if(stream == 1)
		{
			si446x_set_property(0x10, 0x01, 0x04, 0x31);
		}
	}

	xTime0 = xTaskGetTickCount();
	ticks = (time_s * 1000UL) / (1000UL / OS_TICKS_FREQUENCY);
	
	Clr_SI4438_nSEL(); 
	ByteSend(SI446X_CMD_ID_WRITE_TX_FIFO); 
	for (i=0; i<50; i++)
	{
		ByteSend(M_data[stream]); 
	}
	Set_SI4438_nSEL();
	si446x_start_tx(channel, 0x30,0x00);		
	while((xTaskGetTickCount() - xTime0) <= ticks)																		// 当数据还未接收完																	// 当数据还未接收完
	{
		#ifdef JMESH_WDT_ENABLE
		jmesh_wdt_sub_reset(wdt_regist_id);
		#endif
		si446x_get_int_status(0u, 0u, 0u);
		if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_TX_FIFO_ALMOST_EMPTY_PEND_BIT)
		{		
			Clr_SI4438_nSEL(); 
			ByteSend(SI446X_CMD_ID_WRITE_TX_FIFO); 
			for (i=0; i<48; i++)
			{
				ByteSend(M_data[stream]); 
			}
			Set_SI4438_nSEL();
		}
	}
	phy_init();
}
#define	Rf_CREL_Const	0x67
void test_RF_ber(uchar test_channel, uchar time_s, uchar test_power)
{
	uint16_t wDelay = 0;
	uint32_t ticks;
	TickType_t xTime0;


	vRadio_PowerUp();
	while (SI446X_SUCCESS !=vRadio_si4438Patch())
	{		
		wDelay = 0x7FFF;
		while(wDelay--);
		vRadio_PowerUp();
	}

	while (SI446X_SUCCESS != si446x_configuration_init(driect_config))
	{
		wDelay = 0x7FFF; 
		while(wDelay--); 
		vRadio_PowerUp();
	}
	
	si446x_get_int_status(0u, 0u, 0u);
	Write_TX_Power_Ceshi(test_power);
	crystal_frequency_tune(Rf_CREL_Const); 	
	si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
	si446x_get_int_status(0u, 0u, 0u);
	si446x_start_rx(test_channel,0x00,0x00,0x08,0x08,0x08);
	si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
	xTime0 = xTaskGetTickCount();

	ticks = (time_s * 1000UL) / (1000UL / OS_TICKS_FREQUENCY);
	while((xTaskGetTickCount() - xTime0) <= ticks)
	{ 
		#ifdef JMESH_WDT_ENABLE
		jmesh_wdt_sub_reset(wdt_regist_id);
		#endif
	}

	phy_init();
}

/*

*/
