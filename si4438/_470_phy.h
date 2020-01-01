#ifndef _470_PHY_H
#define _470_PHY_H

#include "typedef.h"
#include <stdint.h>

/*******inter const*******/

#define CHANNEL_GROUP_NUM           33
#define EACH_CHANNEL_GROUP_NUM		2    /*CHANNEL*/

/**** phy data struct**** ****/

typedef struct phy_pib_s{
	uchar phyCurrentChannelIndex;
	uchar phyCurrentChannelGroup;
}phy_pib_t;
	
typedef  struct phy_tx_packet_s{	
	uchar len;
	uchar dat[256];
	uchar used_flag;
}phy_tx_packet_t;

typedef __packed struct phy_rx_packet_s{
		uchar len;
		uchar dat[256];
		uchar rssi;
}phy_rx_packet_t;

typedef enum _channelTpye
{
	PRIVATE_CHANNEL,	
	PUBLIC_CHANNEL,	
}channelType;


typedef enum tx_status_tag{
	TX_SUCCESS,
	TRANSACTION_OVERFLOW,
	TRANSACTION_EXPIRED,
	CHANNEL_ACCESS_FAILURE,
	INVALID_ADDRESS,
	COUNTER_ERROR,
	UNAVAILABLE_KEY,
	UNSUPPORTED_SECURITY,
	INVALID_PARAMETER,
	ACK_SUCCESS,
}tx_status;


typedef struct state_flag_s
{
	uint16_t fFixChannel:1;        //:0 normal    1 have fixed channel
	uint16_t fChangeChannel:1;	//0: disable change channnel  1:enable change channel
	uint16_t sModuleRun:1;		//0: normal; 1:aging test
	uint16_t fAgingStep:1;		//0:set mac addr ;1:Aging passthrough. 
	uint16_t Reserve1:12;
}state_flag_t;

extern phy_tx_packet_t phy_tx_packet;
extern phy_rx_packet_t phy_rx_packet;
extern phy_pib_t phy_pib;


extern uint8_t Rf_CREL;
extern uint8_t rx_channel;
/**************************************************** interface ********************************************************/
/* 
* 功能：设置信道组  
* 参数： 
*		 
*/ 
void phy_init(void); 

/* 
* 功能：设置信道组  
* 参数： 
*		group_num:私有信道组号 
*/
void phy_set_channel_group(uchar group_num);

/* 
* 功能：设置发射信道  
* 参数： 
*		type:信道组类型，PRIVATE_CHANNEL或 PUBLIC_CHANNEL
*		index:信道在信道组中的索引，0或者1 
*/
void phy_set_channel(channelType type, uchar index );

/* 
* 功能：禁止跳频接收   
* 参数： 
*				
*/
void phy_lock_channel(void);

/* 
* 功能：恢复跳频接收   
* 参数： 
*				
*/
void phy_unlock_channel(void);

/* 
* 功能：固定信道    
* 参数：
*		channel：固定的信道 			
*/
void phy_fix_channel(uint8 channel);

/* 
* 功能：解除固定信道    
* 参数：
*				
*/
void phy_unfix_channel(void);

/* 
* 功能：发送数据     
* 参数：
*		*data： 发送数据的指针
*       len ：发送数据的长度 
*/
tx_status phy_tx_pkt(const uint8_t *data, uint8_t len, uint8_t response_flag);

/* 
* 功能：发送数据     
* 参数：
*		*rcv_len： 接收长度 
*       *rcv_rssi：接收场强  
* 返回：
* 		接收数据的指针 
*/
uchar* phy_rx_pkt(uchar *rcv_len, uchar *rcv_rssi);
void phy_tx_test(void);

void Write_TX_Power_Ceshi(uchar llc_TXPower);
void test_stream_data(uchar stream,uchar channel, uchar time_s, uint8 tx_power);
void test_RF_ber(uchar test_channel,uchar test_power, uchar time_s);
#endif


