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
* ���ܣ������ŵ���  
* ������ 
*		 
*/ 
void phy_init(void); 

/* 
* ���ܣ������ŵ���  
* ������ 
*		group_num:˽���ŵ���� 
*/
void phy_set_channel_group(uchar group_num);

/* 
* ���ܣ����÷����ŵ�  
* ������ 
*		type:�ŵ������ͣ�PRIVATE_CHANNEL�� PUBLIC_CHANNEL
*		index:�ŵ����ŵ����е�������0����1 
*/
void phy_set_channel(channelType type, uchar index );

/* 
* ���ܣ���ֹ��Ƶ����   
* ������ 
*				
*/
void phy_lock_channel(void);

/* 
* ���ܣ��ָ���Ƶ����   
* ������ 
*				
*/
void phy_unlock_channel(void);

/* 
* ���ܣ��̶��ŵ�    
* ������
*		channel���̶����ŵ� 			
*/
void phy_fix_channel(uint8 channel);

/* 
* ���ܣ�����̶��ŵ�    
* ������
*				
*/
void phy_unfix_channel(void);

/* 
* ���ܣ���������     
* ������
*		*data�� �������ݵ�ָ��
*       len ���������ݵĳ��� 
*/
tx_status phy_tx_pkt(const uint8_t *data, uint8_t len, uint8_t response_flag);

/* 
* ���ܣ���������     
* ������
*		*rcv_len�� ���ճ��� 
*       *rcv_rssi�����ճ�ǿ  
* ���أ�
* 		�������ݵ�ָ�� 
*/
uchar* phy_rx_pkt(uchar *rcv_len, uchar *rcv_rssi);
void phy_tx_test(void);

void Write_TX_Power_Ceshi(uchar llc_TXPower);
void test_stream_data(uchar stream,uchar channel, uchar time_s, uint8 tx_power);
void test_RF_ber(uchar test_channel,uchar test_power, uchar time_s);
#endif


