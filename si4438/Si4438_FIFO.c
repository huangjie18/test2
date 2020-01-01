/*! @file radio.c
 * @brief This file contains functions to interface with the radio chip.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */
#include "si446x_defs.h"
#include "radio_config.h"
#include "si4438.h"
#include "_470_phy.h"
#include "si4438_hal.h"
#include <stdarg.h>
#include "arch.h"
#include "jmesh_app_pro.h"
#include "task.h"
#include "os_core.h"

#define RADIO_MAX_PACKET_LENGTH     64u
//#define RADIO_MAX_LONG_PACKET_LENGTH (4u * RADIO_MAX_PACKET_LENGTH-6)//250

#define RADIO_TX_ALMOST_EMPTY_THRESHOLD 48 
#define RADIO_RX_ALMOST_FULL_THRESHOLD  48

const unsigned char u8szSi4438BPatch[][8]={      
	{0x04,0x21,0x35,0xAB,0x00,0x00,0xC4,0x3B},
	{0x05,0x2A,0x20,0xD3,0x45,0x85,0xD2,0x2D},
	{0xEF,0xEA,0x6F,0xA0,0xBB,0x0C,0x47,0x93},
	{0xEF,0x36,0xBC,0x92,0x22,0xDD,0x84,0x38},
	{0xE7,0x68,0x56,0xEA,0x60,0x31,0xBA,0xAA},
	{0xE2,0xB2,0x90,0x13,0xC0,0x56,0xD9,0x68},
	{0x05,0x60,0x1E,0x85,0x41,0xF8,0x8D,0xE7}};


const unsigned char u8szSi4438CPatch[][8]={      
{0x04,0x21,0x71,0x4B,0x00,0x00,0xBA,0x9E}, \
{0x05,0x48,0x23,0x2E,0x2B,0x90,0xB1,0x4E}, \
{0xEA,0x3F,0xB9,0xE8,0x8B,0xA9,0xCA,0xD6}, \
{0x05,0xD2,0xE5,0xBE,0xD1,0x27,0x55,0x82}, \
{0xE5,0x56,0x2A,0x3B,0x76,0x76,0x96,0x48}, \
{0x05,0x8E,0x26,0xD8,0x5D,0x01,0xA7,0x88}, \
{0xE2,0x89,0xCC,0x63,0x79,0x95,0x00,0x4B}, \
{0x05,0xE0,0x75,0xCD,0xA4,0xB9,0x46,0xBC}, \
{0xEA,0xD3,0x37,0xD2,0x9A,0x89,0x82,0xEA}, \
{0x05,0x0C,0xAE,0x4C,0xF5,0xF6,0x3C,0xB3}, \
{0xE9,0xA7,0x70,0xDF,0xF1,0x14,0x4F,0x04}, \
{0x05,0xFE,0x5B,0xDF,0x47,0x0A,0x7C,0x5B}, \
{0xE2,0xFB,0x3E,0x21,0xA2,0x1B,0xAA,0x93}, \
{0x05,0xBF,0xFD,0xAB,0x69,0x6C,0xA8,0x5A}, \
{0xE2,0x66,0xB7,0x2E,0x2C,0x45,0x2D,0xFB}, \
{0x05,0x0D,0x55,0xBD,0xC2,0x37,0x00,0x72}, \
{0xE2,0xFF,0x57,0x4D,0x7C,0x6C,0x00,0x2C}, \
{0x05,0x9E,0xF2,0x46,0xFD,0xD3,0x16,0x1B}, \
{0xEA,0x16,0x7F,0x67,0x4D,0xE5,0xE2,0xC8}, \
{0x05,0x37,0x33,0x1C,0xFA,0xBB,0xEE,0xEF}, \
{0xEA,0x00,0x5F,0xBE,0xA4,0xFC,0xBF,0xC1}, \
{0x05,0x95,0x12,0x2F,0x0A,0xCF,0x55,0x8C}, \
{0xE7,0x70,0xC2,0xD4,0xF0,0x81,0x95,0xC8}, \
{0xE7,0x72,0x00,0xF9,0x8D,0x15,0x01,0xA3}, \
{0xE7,0x18,0xE5,0x6C,0x51,0x1F,0x86,0x9F}, \
{0xE7,0xDD,0x37,0x59,0x4B,0xAD,0xB0,0x9C}, \
{0xE7,0xC8,0xE8,0x84,0xCD,0x55,0x41,0x83}, \
{0xEF,0x4F,0x8E,0x38,0xCB,0x37,0x02,0x87}, \
{0xE7,0xF5,0x00,0x88,0x4C,0x09,0x65,0xCE}, \
{0xEF,0xDD,0xBC,0x65,0x62,0xAC,0x75,0x62}, \
{0xE7,0xC0,0xF1,0x5D,0x98,0xB0,0xDD,0x43}, \
{0xE7,0x19,0xB4,0xF8,0x9F,0x6D,0x8C,0xCB}, \
{0xE1,0xDE,0x63,0xC2,0x32,0xC6,0xE4,0x2F}, \
{0x05,0xF4,0x33,0xB7,0x2E,0x72,0x9A,0xF9}, \
{0xE7,0x65,0xD9,0x38,0xB8,0xFE,0x31,0x16}, \
{0xE7,0xF3,0x06,0x2D,0xF5,0xFE,0x0C,0x38}, \
{0xE7,0x70,0x4F,0xE7,0x49,0xB4,0x58,0x39}, \
{0xEF,0xF1,0x46,0xA9,0x23,0x38,0x64,0xC0}, \
{0xE7,0x09,0x4E,0x04,0xD3,0x46,0x15,0x02}, \
{0xEF,0x8D,0xC7,0x20,0xC3,0x90,0x87,0x4D}, \
{0xEF,0x00,0xAB,0x7F,0x27,0x02,0xC6,0xA0}, \
{0xE7,0x23,0xA6,0xA6,0xA4,0x27,0x11,0x7D}, \
{0xEF,0xB3,0xF1,0x9E,0x6A,0xB3,0x19,0xAF}, \
{0xE7,0xAB,0xF5,0x15,0x78,0x5E,0x48,0xF8}, \
{0xEF,0x5B,0xB1,0x2E,0xAF,0x2A,0xFF,0x16}, \
{0xE7,0x30,0x62,0x5C,0x82,0x7A,0x3F,0x83}, \
{0xEF,0x91,0xA7,0xD3,0x1B,0x64,0x85,0xBE}, \
{0xE7,0x4D,0x81,0x94,0xE4,0xAA,0xE8,0xDB}, \
{0xEF,0xA0,0xCC,0x4A,0x23,0xA5,0x7E,0x36}, \
{0xEF,0x0C,0x72,0x4C,0xFB,0x26,0x5A,0xEC}, \
{0xEF,0x0E,0x42,0xFA,0xAF,0x49,0xA0,0xA8}, \
{0xE7,0x6D,0x12,0xDF,0x2B,0x0C,0x61,0x58}, \
{0xEA,0xB6,0x9B,0xDE,0x81,0xB9,0xFF,0xFF}, \
{0x05,0x04,0xEB,0xD8,0x12,0xD6,0x8D,0xE0}, \
{0xEC,0x29,0x66,0x4B,0xDE,0xB7,0xDE,0x36}, \
{0x05,0x0D,0x28,0xB9,0x0A,0x89,0x31,0x1A}};

/*****************************************************************************
 *  Global Variables
 *****************************************************************************/
const U8 Radio_Configuration_Data_Array[] = RADIO_CONFIGURATION_DATA_ARRAY;

/*****************************************************************************
 *  Local Macros & Definitions
 *****************************************************************************/

uchar change_channal_flag=0x00;
uchar Timer400ms=0;//qian dao ma shi xiao time

U8 ctsWentHigh = 0;
U8 CURRENT_CHANNEL;
union si446x_cmd_reply_union Si446xCmd;
U8 Pro2Cmd[16];
U8* pPositionInPayload =  &phy_rx_packet.dat[0u];//chu shi hua zhi zhen
U8 bPositionInPayload  = 0u;

//U8  RF_RX_LEN;


void SpiDelay(u8 cnt)
{
	u8 i = 0;	
	for(;i<cnt;i++)
	{
		;
	}
}

void ByteSend(uchar i)
{
	uchar n=8;
	Clr_SI4438_SCLK();
	while(n--)
	{		
		if(i&0x80)             
			Set_SI4438_SDI();
		else
			Clr_SI4438_SDI();
		Set_SI4438_SCLK();
		i=i<<1;
		SpiDelay(4);
		Clr_SI4438_SCLK();
	}
	Clr_SI4438_SCLK();
}

uchar ByteRead(void)
{
	uchar Result = 0,i = 0;
	
	Clr_SI4438_SCLK();
	for(; i < 8; i++)
	{	
		Result <<= 1;			
		if(Read_SI4438_SDO())
		{
			Result |=1 ;
		}
		Set_SI4438_SCLK();		
		SpiDelay(6);		
		Clr_SI4438_SCLK();			
	}
	return Result;
}


/*****************************************************************************
 *  Local Function Declarations
 *****************************************************************************/
/*! This function sends the PART_INFO command to the radio and receives the answer
  *  into @Si446xCmd union.
  */
 void si446x_part_info(void)
 {

	 Pro2Cmd[0] = SI446X_CMD_ID_PART_INFO;
 
	 radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_PART_INFO,
							   Pro2Cmd,
							   SI446X_CMD_REPLY_COUNT_PART_INFO,
							   Pro2Cmd );
	 
#if 0
    Si446xCmd.PART_INFO.CHIPREV         = Pro2Cmd[0];
    Si446xCmd.PART_INFO.PART   			= (((U16)Pro2Cmd[1]<<8)&(0xFF00))&Pro2Cmd[2];
    Si446xCmd.PART_INFO.PBUILD          = Pro2Cmd[3];
    Si446xCmd.PART_INFO.ID     			= (((U16)Pro2Cmd[4]<<8)&(0xFF00))&Pro2Cmd[5];
    Si446xCmd.PART_INFO.CUSTOMER        = Pro2Cmd[6];
    Si446xCmd.PART_INFO.ROMID           = Pro2Cmd[7];
#endif

 }




/*!
 * This functions is used to reset the si446x radio by applying shutdown and
 * releasing it.  After this function @ref si446x_boot should be called.  You
 * can check if POR has completed by waiting 4 ms or by polling GPIO 0, 2, or 3.
 * When these GPIOs are high, it is safe to call @ref si446x_boot.
 */
void si446x_reset(void)
{
    U16 loopCount;
    Set_SI4438_SDN();
	//! @todo this needs to be a better delay function.
	loopCount = 8000;
	while(loopCount--);
	Clr_SI4438_SDN();
	loopCount = 12000;
	while(loopCount--);
	Clr_SI4438_nSEL();
}


/*!
 *  Power up the Radio.
 *
 *  @note
 *
 */
void vRadio_PowerUp(void)
{
  //U16  wDelay = pRadioConfiguration->Radio_Delay_Cnt_After_Reset;
  U16  wDelay = RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET;
  /* Hardware reset the chip */
  si446x_reset();
  /* Wait until reset timeout or Reset IT signal */
  while(wDelay--);

}
U8 vRadio_si4438Patch(void)
{
	unsigned short line,patch_line_len;
	unsigned char row;
	const unsigned char* prt;


	si446x_part_info();
	if((Pro2Cmd[7] == 0x03 || Pro2Cmd[7] == 0x06) && (Pro2Cmd[4] < 0x8d))
	{
		if(Pro2Cmd[7] == 0x06)
        {
        	patch_line_len = sizeof(u8szSi4438CPatch) / 8u;
            prt = &u8szSi4438CPatch[0][0];
		}
		else
		{
            patch_line_len = sizeof(u8szSi4438BPatch) / 8u;
			prt = &u8szSi4438BPatch[0][0];
		}
			
		for (line = 0; line < patch_line_len; line++)
		{
			for(row=0; row<8; row++)
			{
				Pro2Cmd[row]=*prt;//Pro2Cmd[row] = u8szSi4438Patch[line][row];
				prt++;
			}
			if (radio_comm_SendCmdGetResp(8, Pro2Cmd, 0, 0) != 0xFF)
	        {
	          // Timeout occured
	          return SI446X_CTS_TIMEOUT;
	        }
			if (Read_SI4438_nIRQ() == 0)
	        {
	          /* Get and clear all interrupts.  An error has occured... */
	          si446x_get_int_status(0, 0, 0);
	          return SI446X_PATCH_FAIL;
	        }
		}
		return SI446X_SUCCESS;
	}	
	return SI446X_NO_PATCH;
}
/*!
  * This function is used to load all properties and commands with a list of NULL terminated commands.
  * Before this function @si446x_reset should be called.
  */
U8 si446x_configuration_init(const U8* pSetPropCmd)
{
	U8 col;
	U8 numOfBytes;
	while (*pSetPropCmd != 0x00)
	{
		numOfBytes = *pSetPropCmd++;
		if (numOfBytes > 16u)
		{
	   /* Number of command bytes exceeds maximal allowable length */
			return SI446X_COMMAND_ERROR;
		}
		for (col = 0u; col < numOfBytes; col++)
		{
			Pro2Cmd[col] = *pSetPropCmd;
			pSetPropCmd++;
		}
		if (radio_comm_SendCmdGetResp(numOfBytes, Pro2Cmd, 0, 0) != 0xFF)
		{
	   /* Timeout occured */
			return SI446X_CTS_TIMEOUT;
		} 
		if (Read_SI4438_nIRQ() == 0)
		{
	   /* Get and clear all interrupts.  An error has occured... */
			si446x_get_int_status(0, 0, 0);
			if (Si446xCmd.GET_INT_STATUS.CHIP_PEND & SI446X_CMD_GET_CHIP_STATUS_REP_CMD_ERROR_PEND_MASK)
			{
				return SI446X_COMMAND_ERROR;
			}
		}
	}
  return SI446X_SUCCESS;
}
 

/*!
 *  Radio Initialization.
 *
 *  @author Sz. Papp
 *
 *  @note
 *
 */
void vRadio_Init(void)
{
	U16 wDelay;
	volatile uint8 cnt = 0;
  
	/* Power Up the radio chip */
	vRadio_PowerUp();
	 
	while ((SI446X_SUCCESS !=vRadio_si4438Patch()) && (cnt < 50))
	{
		cnt ++;
		wDelay = 0x7FFF;
		while(wDelay--);
		vRadio_PowerUp();
	}	
  /* Load radio configuration */
	// 
  //while (SI446X_SUCCESS != si446x_configuration_init(pRadioConfiguration->Radio_ConfigurationArray))
	cnt = 0;
	while ((SI446X_SUCCESS != si446x_configuration_init(Radio_Configuration_Data_Array)) && (cnt < 50))
	{
    /* Error hook */
		cnt ++;
		wDelay = 0x7FFF;
		while(wDelay--);
    		/* Power Up the radio chip */
		vRadio_PowerUp();
	}

  // Read ITs, clear pending ones
	si446x_get_int_status(0u, 0u, 0u);
	si446x_part_info();
}
 /*!
  * Sends a command to the radio chip and gets a response
  *
  * @param cmdByteCount  Number of bytes in the command to send to the radio device
  * @param pCmdData 	 Pointer to the command data
  * @param respByteCount Number of bytes in the response to fetch
  * @param pRespData	 Pointer to where to put the response data
  *
  * @return CTS value
  */
 U8 radio_comm_SendCmdGetResp(U8 cmdByteCount, U8* pCmdData, U8 respByteCount, U8* pRespData)
 {
	 radio_comm_SendCmd(cmdByteCount, pCmdData);
	 return radio_comm_GetResp(respByteCount, pRespData);
 }
/*!
* Get the Interrupt status/pending flags form the radio and clear flags if requested.
*
* @param PH_CLR_PEND 	Packet Handler pending flags clear.
* @param MODEM_CLR_PEND	Modem Status pending flags clear.
* @param CHIP_CLR_PEND	Chip State pending flags clear.
*/
void si446x_get_int_status(U8 PH_CLR_PEND, U8 MODEM_CLR_PEND, U8 CHIP_CLR_PEND)
{
	Pro2Cmd[0] = SI446X_CMD_ID_GET_INT_STATUS;
	Pro2Cmd[1] = PH_CLR_PEND;
	Pro2Cmd[2] = MODEM_CLR_PEND;
	Pro2Cmd[3] = CHIP_CLR_PEND;

	radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GET_INT_STATUS,
						Pro2Cmd,
						SI446X_CMD_REPLY_COUNT_GET_INT_STATUS,
						Pro2Cmd );

	Si446xCmd.GET_INT_STATUS.INT_PEND 	  = Pro2Cmd[0];
	Si446xCmd.GET_INT_STATUS.INT_STATUS	  = Pro2Cmd[1];
	Si446xCmd.GET_INT_STATUS.PH_PEND		  = Pro2Cmd[2];
	Si446xCmd.GET_INT_STATUS.PH_STATUS	  = Pro2Cmd[3];
	Si446xCmd.GET_INT_STATUS.MODEM_PEND	  = Pro2Cmd[4];
	Si446xCmd.GET_INT_STATUS.MODEM_STATUS   = Pro2Cmd[5];
	Si446xCmd.GET_INT_STATUS.CHIP_PEND	  = Pro2Cmd[6];
	Si446xCmd.GET_INT_STATUS.CHIP_STATUS	  = Pro2Cmd[7];
}

 /*!
   * Sends a command to the radio chip
   *
   * @param byteCount	  Number of bytes in the command to send to the radio device
   * @param pData		  Pointer to the command to send.
   */
void radio_comm_SendCmd(U8 byteCount, U8* pData)
{
  /* There was a bug in A1 hardware that will not handle 1 byte commands. 
	 It was supposedly fixed in B0 but the fix didn't make it at the last minute, so here we go again */
	U8 bCnt;

	if (byteCount == 1)byteCount++;

	while (!ctsWentHigh)
	{
		radio_comm_GetResp(0, 0);
	}
	Clr_SI4438_nSEL();

	for (bCnt=0; bCnt<byteCount; bCnt++)
	{
		ByteSend(pData[bCnt]); 
	}
	Set_SI4438_nSEL();
	ctsWentHigh = 0;
}
  
 
/*!
* Gets a command response from the radio chip
*
* @param byteCount	 Number of bytes to get from the radio chip
* @param pData		 Pointer to where to put the data
*
* @return CTS value
*/
U8 radio_comm_GetResp(U8 byteCount, U8* pData)
{
	U8 ctsVal = 0u ;
	U8 bCnt;
	U16 errCnt = RADIO_CTS_TIMEOUT;
	while (errCnt != 0)		//wait until radio IC is ready with the data
	{
		Clr_SI4438_nSEL();
         
		ByteSend(0x44); 
		ctsVal = ByteRead();
		if (ctsVal == 0xFF)
		{
			if (byteCount)
			{
				for (bCnt=0; bCnt<byteCount; bCnt++)
				{
					pData[bCnt] = ByteRead();
				}
			}
			Set_SI4438_nSEL();
			break;
		}
		Set_SI4438_nSEL();
		errCnt--;
	}

	if (errCnt == 0)
	{
		ctsWentHigh = 1;
		return 0;
		//  vRadio_Init();
	}

	if (ctsVal == 0xFF)
	{
		ctsWentHigh = 1;
	}

return ctsVal;
}


/*!
 *  Set Radio to RX mode, fixed packet length.
 *
 *  @param channel Freq. Channel
 *
 *  @note
 *
 */
 void vRadio_StartRX(U8 channel)
{
  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);

  /* Reset FIFO */
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);

  /* Start Receiving packet, channel 0, START immediately, Packet length according to PH */
#if 0
  si446x_start_rx(channel, 0u, 0x00,
                  SI446X_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_NOCHANGE,
                  SI446X_CMD_START_RX_ARG_RXVALID_STATE_ENUM_READY,
                  SI446X_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX );
#endif
    si446x_start_rx(channel, 0u, 0x00,
                  SI446X_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX,
                  SI446X_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX,
                  SI446X_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX );
}


 
/*!
*  Set Radio to TX mode, fixed packet length.
*
*  @param channel Freq. Channel, Packet to be sent
*
*  @note
*
*/
void  vRadio_StartTx(U8 channel, U8 *pioFixRadioPacket,U8 TX_LEN)
{
/* Reset TX FIFO */
	si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_TX_BIT);
	si446x_get_int_status(0u, 0u, 0u);
	 
/* Position to the very beginning of the custom long payload */
	pPositionInPayload = pioFixRadioPacket;
	bPositionInPayload = TX_LEN; 	 
/* Fill the TX fifo with datas */
	if( RADIO_MAX_PACKET_LENGTH <bPositionInPayload)
	{
 /* Data to be sent is more than the size of TX FIFO */
 		si446x_write_tx_fifo(RADIO_MAX_PACKET_LENGTH, pPositionInPayload);
	 
 /* Calculate how many bytes are sent to TX FIFO */
 		bPositionInPayload -= RADIO_MAX_PACKET_LENGTH;
	 
 /* Position to the next first byte that can be sent to TX FIFO in next round */
 		pPositionInPayload += RADIO_MAX_PACKET_LENGTH;
	}
	else
	{
 // Data to be sent is less or equal than the size of TX FIFO
 		si446x_write_tx_fifo(bPositionInPayload, pioFixRadioPacket);
 		bPositionInPayload -= bPositionInPayload;

 		pPositionInPayload += bPositionInPayload;
	}
	 
	 
/* Start sending packet, channel 0, START immediately, Packet length according to PH, go READY when done */
	si446x_start_tx(channel, 0x30, TX_LEN);
}
 /*!
  * This function is used to initialize after power-up the radio chip.
  * Before this function @si446x_reset should be called.
  */
 #if 0
 void si446x_power_up(U8 BOOT_OPTIONS, U8 XTAL_OPTIONS, U32 XO_FREQ)
 {
	 Pro2Cmd[0] = SI446X_CMD_ID_POWER_UP;
	 Pro2Cmd[1] = BOOT_OPTIONS;
	 Pro2Cmd[2] = XTAL_OPTIONS;
	 Pro2Cmd[3] = (U8)(XO_FREQ >> 24);
	 Pro2Cmd[4] = (U8)(XO_FREQ >> 16);
	 Pro2Cmd[5] = (U8)(XO_FREQ >> 8);
	 Pro2Cmd[6] = (U8)(XO_FREQ);
 
	 radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_POWER_UP, Pro2Cmd );
 }
#endif
 /*! Sends START_TX command to the radio.
  *
  * @param CHANNEL	 Channel number.
  * @param CONDITION Start TX condition.
  * @param TX_LEN	 Payload length (exclude the PH generated CRC).
  */
 void si446x_start_tx(U8 CHANNEL, U8 CONDITION, U16 TX_LEN)
 {
	 Pro2Cmd[0] = SI446X_CMD_ID_START_TX;
	 Pro2Cmd[1] = CHANNEL;
	 Pro2Cmd[2] = CONDITION;
	 Pro2Cmd[3] = (U8)(TX_LEN >> 8);
	 Pro2Cmd[4] = (U8)(TX_LEN);
	 Pro2Cmd[5] = 0x00;
 
	 radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_START_TX, Pro2Cmd );
 }
 
 /*!
  * Sends START_RX command to the radio.
  *
  * @param CHANNEL	   Channel number.
  * @param CONDITION   Start RX condition.
  * @param RX_LEN	   Payload length (exclude the PH generated CRC).
  * @param NEXT_STATE1 Next state when Preamble Timeout occurs.
  * @param NEXT_STATE2 Next state when a valid packet received.
  * @param NEXT_STATE3 Next state when invalid packet received (e.g. CRC error).
  */
 void si446x_start_rx(U8 CHANNEL, U8 CONDITION, U16 RX_LEN, U8 NEXT_STATE1, U8 NEXT_STATE2, U8 NEXT_STATE3)
 {
	 Pro2Cmd[0] = SI446X_CMD_ID_START_RX;
	 Pro2Cmd[1] = CHANNEL;
	 Pro2Cmd[2] = CONDITION;
	 Pro2Cmd[3] = (U8)(RX_LEN >> 8);
	 Pro2Cmd[4] = (U8)(RX_LEN);
	 Pro2Cmd[5] = NEXT_STATE1;
	 Pro2Cmd[6] = NEXT_STATE2;
	 Pro2Cmd[7] = NEXT_STATE3;
 
	 radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_START_RX, Pro2Cmd );
 }
 

 /*!
  * Send GPIO pin config command to the radio and reads the answer into
  * @Si446xCmd union.
  *
  * @param GPIO0	   GPIO0 configuration.
  * @param GPIO1	   GPIO1 configuration.
  * @param GPIO2	   GPIO2 configuration.
  * @param GPIO3	   GPIO3 configuration.
  * @param NIRQ 	   NIRQ configuration.
  * @param SDO		   SDO configuration.
  * @param GEN_CONFIG  General pin configuration.
  */
 void si446x_gpio_pin_cfg(U8 G_GPIO0, U8 G_GPIO1, U8 G_GPIO2, U8 G_GPIO3, U8 G_NIRQ, U8 G_SDO, U8 G_GEN_CONFIG)
{
    Pro2Cmd[0] = SI446X_CMD_ID_GPIO_PIN_CFG;
    Pro2Cmd[1] = G_GPIO0;
    Pro2Cmd[2] = G_GPIO1;
    Pro2Cmd[3] = G_GPIO2;
    Pro2Cmd[4] = G_GPIO3;
    Pro2Cmd[5] = G_NIRQ;
    Pro2Cmd[6] = G_SDO;
    Pro2Cmd[7] = G_GEN_CONFIG;

    radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GPIO_PIN_CFG,
                              Pro2Cmd,
                              SI446X_CMD_REPLY_COUNT_GPIO_PIN_CFG,
                              Pro2Cmd );

//    Si446xCmd.GPIO_PIN_CFG.GPIO0        = Pro2Cmd[0];
//    Si446xCmd.GPIO_PIN_CFG.GPIO1        = Pro2Cmd[1];
//    Si446xCmd.GPIO_PIN_CFG.GPIO2        = Pro2Cmd[2];
//    Si446xCmd.GPIO_PIN_CFG.GPIO3        = Pro2Cmd[3];
//    Si446xCmd.GPIO_PIN_CFG.GPIO_NIRQ    = Pro2Cmd[4];
//    Si446xCmd.GPIO_PIN_CFG.GPIO_SDO     = Pro2Cmd[5];
//    Si446xCmd.GPIO_PIN_CFG.GEN_CONFIG   = Pro2Cmd[6];
}
 
 /*!
  * Send SET_PROPERTY command to the radio.
  *
  * @param GROUP	   Property group.
  * @param NUM_PROPS   Number of property to be set. The properties must be in ascending order
  * 				   in their sub-property aspect. Max. 12 properties can be set in one command.
  * @param START_PROP  Start sub-property address.
  */
void si446x_set_property( U8 GROUP, U8 NUM_PROPS, U8 START_PROP, ... )
{
	va_list argList;
	U8 cmdIndex;
    Pro2Cmd[0] = SI446X_CMD_ID_SET_PROPERTY;
	Pro2Cmd[1] = GROUP;
	Pro2Cmd[2] = NUM_PROPS;
	Pro2Cmd[3] = START_PROP;

	va_start (argList, START_PROP);
	cmdIndex = 4;
	while(NUM_PROPS--)
	{
		Pro2Cmd[cmdIndex] = va_arg(argList, int);
		cmdIndex++;
	}
	va_end(argList);

	radio_comm_SendCmd( cmdIndex, Pro2Cmd );
}
 
 /*!
  * Issue a change state command to the radio.
  *
  * @param NEXT_STATE1 Next state.
  */
 void si446x_change_state(U8 NEXT_STATE1)
 {
	 Pro2Cmd[0] = SI446X_CMD_ID_CHANGE_STATE;
	 Pro2Cmd[1] = NEXT_STATE1;
 
	 radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_CHANGE_STATE, Pro2Cmd );
 }
 /*!
  * Gets a command response from the radio chip
  *
  * @param cmd			 Command ID
  * @param pollCts		 Set to poll CTS
  * @param byteCount	 Number of bytes to get from the radio chip.
  * @param pData		 Pointer to where to put the data.
  */
 void radio_comm_ReadData(U8 cmd, BIT pollCts, U8 byteCount, U8* pData)
 { 
	U8 bCnt;
    if(pollCts)
	{
		while(!ctsWentHigh)
		{ 
			radio_comm_GetResp(0, 0);
		}
	}
	Clr_SI4438_nSEL();
	ByteSend(cmd); 
	for (bCnt=0; bCnt<byteCount; bCnt++)
	{
		pData[bCnt] = ByteRead();
	}
	Set_SI4438_nSEL();
	ctsWentHigh = 0;
 }

/*!
* Gets a command response from the radio chip
*
* @param cmd			 Command ID
* @param pollCts		 Set to poll CTS
* @param byteCount	 Number of bytes to get from the radio chip
* @param pData		 Pointer to where to put the data
*/
void radio_comm_WriteData(U8 cmd, BIT pollCts, U8 byteCount, U8* pData)
{
	U8 bCnt;
    if(pollCts)
	{
		while(!ctsWentHigh)
		{
			radio_comm_GetResp(0, 0);
		}
	}
	Clr_SI4438_nSEL(); 
	ByteSend(cmd); 
	for (bCnt=0; bCnt<byteCount; bCnt++)
	{
		ByteSend(pData[bCnt]); 
	}
	Set_SI4438_nSEL();
	ctsWentHigh = 0;
}

 /*!
  * Send the FIFO_INFO command to the radio. Optionally resets the TX/RX FIFO. Reads the radio response back
  * into @Si446xCmd.
  *
  * @param FIFO  RX/TX FIFO reset flags.
  */
 void si446x_fifo_info(U8 FIFO)
 {
	 Pro2Cmd[0] = SI446X_CMD_ID_FIFO_INFO;
	 Pro2Cmd[1] = FIFO;
 
	 radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_FIFO_INFO,
							   Pro2Cmd,
							   SI446X_CMD_REPLY_COUNT_FIFO_INFO,
							   Pro2Cmd );
 
	 Si446xCmd.FIFO_INFO.RX_FIFO_COUNT	 = Pro2Cmd[0];
	 Si446xCmd.FIFO_INFO.TX_FIFO_SPACE	 = Pro2Cmd[1];
 }
 
 /*!
  * The function can be used to load data into TX FIFO.
  *
  * @param numBytes  Data length to be load.
  * @param pTxData	 Pointer to the data (U8*).
  */
 void si446x_write_tx_fifo(U8 numBytes, U8* pTxData)
 {
   radio_comm_WriteData( SI446X_CMD_ID_WRITE_TX_FIFO, 0, numBytes, pTxData );
 }
 
 /*!
  * Reads the RX FIFO content from the radio.
  *
  * @param numBytes  Data length to be read.
  * @param pRxData	 Pointer to the buffer location.
  */
 void si446x_read_rx_fifo(U8 numBytes, U8* pRxData)
 {
   radio_comm_ReadData( SI446X_CMD_ID_READ_RX_FIFO, 0, numBytes, pRxData );
 }
 /*!
  * Gets the Packet Handler status flags. Optionally clears them.
  *
  * @param PH_CLR_PEND Flags to clear.
  */
  #if 0
 void si446x_get_ph_status(U8 PH_CLR_PEND)
 {
	 Pro2Cmd[0] = SI446X_CMD_ID_GET_PH_STATUS;
	 Pro2Cmd[1] = PH_CLR_PEND;
 
	 radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GET_PH_STATUS,
							   Pro2Cmd,
							   SI446X_CMD_REPLY_COUNT_GET_PH_STATUS,
							   Pro2Cmd );
 
	 Si446xCmd.GET_PH_STATUS.PH_PEND		= Pro2Cmd[0];
	 Si446xCmd.GET_PH_STATUS.PH_STATUS		= Pro2Cmd[1];
 }
  #endif
 /*!
  * Get property values from the radio. Reads them into Si446xCmd union.
  *
  * @param GROUP	   Property group number.
  * @param NUM_PROPS   Number of properties to be read.
  * @param START_PROP  Starting sub-property number.
  */
 void si446x_get_property(U8 GROUP, U8 NUM_PROPS, U8 START_PROP)
 {
	 Pro2Cmd[0] = SI446X_CMD_ID_GET_PROPERTY;
	 Pro2Cmd[1] = GROUP;
	 Pro2Cmd[2] = NUM_PROPS;
	 Pro2Cmd[3] = START_PROP;
 
	 radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GET_PROPERTY,
							   Pro2Cmd,
							   Pro2Cmd[2],
							   Pro2Cmd );
 
	 Si446xCmd.GET_PROPERTY.DATA0	 = Pro2Cmd[0];
	 Si446xCmd.GET_PROPERTY.DATA1	 = Pro2Cmd[1];
	 Si446xCmd.GET_PROPERTY.DATA2	 = Pro2Cmd[2];
	 Si446xCmd.GET_PROPERTY.DATA3	 = Pro2Cmd[3];
	 Si446xCmd.GET_PROPERTY.DATA4	 = Pro2Cmd[4];
	 Si446xCmd.GET_PROPERTY.DATA5	 = Pro2Cmd[5];
	 Si446xCmd.GET_PROPERTY.DATA6	 = Pro2Cmd[6];
	 Si446xCmd.GET_PROPERTY.DATA7	 = Pro2Cmd[7];
	 Si446xCmd.GET_PROPERTY.DATA8	 = Pro2Cmd[8];
	 Si446xCmd.GET_PROPERTY.DATA9	 = Pro2Cmd[9];
	 Si446xCmd.GET_PROPERTY.DATA10	 = Pro2Cmd[10];
	 Si446xCmd.GET_PROPERTY.DATA11	 = Pro2Cmd[11];
	 Si446xCmd.GET_PROPERTY.DATA12	 = Pro2Cmd[12];
	 Si446xCmd.GET_PROPERTY.DATA13	 = Pro2Cmd[13];
	 Si446xCmd.GET_PROPERTY.DATA14	 = Pro2Cmd[14];
	 Si446xCmd.GET_PROPERTY.DATA15	 = Pro2Cmd[15];
 }
 
 /* Full driver support functions */
 
 /*!
  * Sends the FUNC_INFO command to the radio, then reads the resonse into @Si446xCmd union.
  */
  #if 0
 void si446x_func_info(void)
 {
	 Pro2Cmd[0] = SI446X_CMD_ID_FUNC_INFO;
 
	 radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_FUNC_INFO,
							   Pro2Cmd,
							   SI446X_CMD_REPLY_COUNT_FUNC_INFO,
							   Pro2Cmd );
 
//	 Si446xCmd.FUNC_INFO.REVEXT 		 = Pro2Cmd[0];
//	 Si446xCmd.FUNC_INFO.REVBRANCH		 = Pro2Cmd[1];
//	 Si446xCmd.FUNC_INFO.REVINT 		 = Pro2Cmd[2];
//	 Si446xCmd.FUNC_INFO.PATCH.U8[MSB]	 = Pro2Cmd[3];
//	 Si446xCmd.FUNC_INFO.PATCH.U8[LSB]	 = Pro2Cmd[4];
//	 Si446xCmd.FUNC_INFO.FUNC			 = Pro2Cmd[5];
 }
 #endif
 /*!
  * Reads the Fast Response Registers starting with A register into @Si446xCmd union.
  *
  * @param respByteCount Number of Fast Response Registers to be read.
  */
 void si446x_frr_a_read(U8 respByteCount)
 {
	 radio_comm_ReadData(SI446X_CMD_ID_FRR_A_READ,
							 0,
						 respByteCount,
						 Pro2Cmd);
 
	 Si446xCmd.FRR_A_READ.FRR_A_VALUE = Pro2Cmd[0];
//	 Si446xCmd.FRR_A_READ.FRR_B_VALUE = Pro2Cmd[1];
//	 Si446xCmd.FRR_A_READ.FRR_C_VALUE = Pro2Cmd[2];
//	 Si446xCmd.FRR_A_READ.FRR_D_VALUE = Pro2Cmd[3];
 }
#if 0
 
 /*!
  * Reads the Fast Response Registers starting with B register into @Si446xCmd union.
  *
  * @param respByteCount Number of Fast Response Registers to be read.
  */
 void si446x_frr_b_read(U8 respByteCount)
 {
	 radio_comm_ReadData(SI446X_CMD_ID_FRR_B_READ,
							 0,
						 respByteCount,
						 Pro2Cmd);
 
	 Si446xCmd.FRR_B_READ.FRR_B_VALUE = Pro2Cmd[0];
	 Si446xCmd.FRR_B_READ.FRR_C_VALUE = Pro2Cmd[1];
	 Si446xCmd.FRR_B_READ.FRR_D_VALUE = Pro2Cmd[2];
	 Si446xCmd.FRR_B_READ.FRR_A_VALUE = Pro2Cmd[3];
 }
 
 /*!
  * Reads the Fast Response Registers starting with C register into @Si446xCmd union.
  *
  * @param respByteCount Number of Fast Response Registers to be read.
  */
 void si446x_frr_c_read(U8 respByteCount)
 {
	 radio_comm_ReadData(SI446X_CMD_ID_FRR_C_READ,
							 0,
						 respByteCount,
						 Pro2Cmd);
 
	 Si446xCmd.FRR_C_READ.FRR_C_VALUE = Pro2Cmd[0];
	 Si446xCmd.FRR_C_READ.FRR_D_VALUE = Pro2Cmd[1];
	 Si446xCmd.FRR_C_READ.FRR_A_VALUE = Pro2Cmd[2];
	 Si446xCmd.FRR_C_READ.FRR_B_VALUE = Pro2Cmd[3];
 }
 

 /*!
  * Reads the Fast Response Registers starting with D register into @Si446xCmd union.
  *
  * @param respByteCount Number of Fast Response Registers to be read.
  */
 void si446x_frr_d_read(U8 respByteCount)
 {
	 radio_comm_ReadData(SI446X_CMD_ID_FRR_D_READ,
							 0,
						 respByteCount,
						 Pro2Cmd);
 
	 Si446xCmd.FRR_D_READ.FRR_D_VALUE = Pro2Cmd[0];
	 Si446xCmd.FRR_D_READ.FRR_A_VALUE = Pro2Cmd[1];
	 Si446xCmd.FRR_D_READ.FRR_B_VALUE = Pro2Cmd[2];
	 Si446xCmd.FRR_D_READ.FRR_C_VALUE = Pro2Cmd[3];
 }
 #endif
 /*!
  * Receives information from the radio of the current packet. Optionally can be used to modify
  * the Packet Handler properties during packet reception.
  *
  * @param FIELD_NUMBER_MASK Packet Field number mask value.
  * @param LEN				 Length value.
  * @param DIFF_LEN 		 Difference length.
  */
 void si446x_get_packet_info(U8 FIELD_NUMBER_MASK, U16 LEN, S16 DIFF_LEN )
 {
	 Pro2Cmd[0] = SI446X_CMD_ID_PACKET_INFO;
	 Pro2Cmd[1] = FIELD_NUMBER_MASK;
	 Pro2Cmd[2] = (U8)(LEN >> 8);
	 Pro2Cmd[3] = (U8)(LEN);
	 // the different of the byte, althrough it is signed, but to command hander
	 // it can treat it as unsigned
	 Pro2Cmd[4] = (U8)((U16)DIFF_LEN >> 8);
	 Pro2Cmd[5] = (U8)(DIFF_LEN);
 
	 radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_PACKET_INFO,
							   Pro2Cmd,
							   SI446X_CMD_REPLY_COUNT_PACKET_INFO,
							   Pro2Cmd );

//	 radio_comm_SendCmdGetResp( 0x01,
//							   Pro2Cmd,
//							   SI446X_CMD_REPLY_COUNT_PACKET_INFO,
//							   Pro2Cmd );

	 Si446xCmd.PACKET_INFO.LENGTH_15_8	 = Pro2Cmd[0];
	 Si446xCmd.PACKET_INFO.LENGTH_7_0	 = Pro2Cmd[1];
 }
 /*!
  * Gets the Modem status flags. Optionally clears them.
  *
  * @param MODEM_CLR_PEND Flags to clear.
  */
 void si446x_get_modem_status( U8 MODEM_CLR_PEND )
 {
	 Pro2Cmd[0] = SI446X_CMD_ID_GET_MODEM_STATUS;
	 Pro2Cmd[1] = MODEM_CLR_PEND;
 
	 radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GET_MODEM_STATUS,
							   Pro2Cmd,
							   SI446X_CMD_REPLY_COUNT_GET_MODEM_STATUS,
							   Pro2Cmd );
 
	 Si446xCmd.GET_MODEM_STATUS.MODEM_PEND	 = Pro2Cmd[0];
	 Si446xCmd.GET_MODEM_STATUS.MODEM_STATUS = Pro2Cmd[1];
	 Si446xCmd.GET_MODEM_STATUS.CURR_RSSI	 = Pro2Cmd[2];
	 Si446xCmd.GET_MODEM_STATUS.LATCH_RSSI	 = Pro2Cmd[3];
	 Si446xCmd.GET_MODEM_STATUS.ANT1_RSSI	 = Pro2Cmd[4];
	 Si446xCmd.GET_MODEM_STATUS.ANT2_RSSI	 = Pro2Cmd[5];
	// Si446xCmd.GET_MODEM_STATUS.AFC_FREQ_OFFSET.U8[MSB]  = Pro2Cmd[6];
	 //Si446xCmd.GET_MODEM_STATUS.AFC_FREQ_OFFSET.U8[LSB]  = Pro2Cmd[7];
 }
 
 /*!
  * Gets the Chip status flags. Optionally clears them.
  *
  * @param CHIP_CLR_PEND Flags to clear.
  */
  #if 0
 void si446x_get_chip_status( U8 CHIP_CLR_PEND )
 {
	 Pro2Cmd[0] = SI446X_CMD_ID_GET_CHIP_STATUS;
	 Pro2Cmd[1] = CHIP_CLR_PEND;
 
	 radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_GET_CHIP_STATUS,
							   Pro2Cmd,
							   SI446X_CMD_REPLY_COUNT_GET_CHIP_STATUS,
							   Pro2Cmd );
 
	 Si446xCmd.GET_CHIP_STATUS.CHIP_PEND		 = Pro2Cmd[0];
	 Si446xCmd.GET_CHIP_STATUS.CHIP_STATUS		 = Pro2Cmd[1];
	 Si446xCmd.GET_CHIP_STATUS.CMD_ERR_STATUS	 = Pro2Cmd[2];
 }
 #endif

 
 /*!
  * Sets the chip up for specified protocol.
  *
  * @param PROTOCOL    0 = Packet format is generic, no dynamic reprogramming of packet handler properties.
					   1 = Packet format is IEEE802.15.4g compliant. The following properties are overriden:
					   PKT_CRC_CONFIG, CRC_ENDIAN/BIT_ORDER in PKT_CONFG1 for TX and RX, PKT_FIELD_1_CRC_CONFIG for RX.
					   Other applicable properties in the packet handler group still need to be programmed. Field 1 should
					   have the length of 16 bits to contain the PHR with PKT_LEN_FIELD_SOURCE set to 1 for RX. PSDU field
					   shall use Field 2 with variable length. Field 2 length should be set to the maximum allowed including
					   the anticipated FCS length. It is anticipated that the FCS will be calculated by the host and transmitted
					   over the air. Si4440 will receive PHR and put FCS in the FIFO for the host to retrieve and check.
					   Therefore, CRC shouldn't be enabled on Si4440.
  */
  #if 0
 void si446x_protocol_cfg(U8 PROTOCOL)
 {
	 Pro2Cmd[0] = SI446X_CMD_ID_PROTOCOL_CFG;
	 Pro2Cmd[1] = PROTOCOL;
 
	 radio_comm_SendCmd( SI446X_CMD_ARG_COUNT_PROTOCOL_CFG, Pro2Cmd );
 }
 #endif
 /*!
  * Requests the current state of the device and lists pending TX and RX requests
  */
U8 si446x_request_device_state(void)
 {
	 Pro2Cmd[0] = SI446X_CMD_ID_REQUEST_DEVICE_STATE;
 
	 radio_comm_SendCmdGetResp( SI446X_CMD_ARG_COUNT_REQUEST_DEVICE_STATE,
							   Pro2Cmd,
							   SI446X_CMD_REPLY_COUNT_REQUEST_DEVICE_STATE,
							   Pro2Cmd );
 
	 Si446xCmd.REQUEST_DEVICE_STATE.CURR_STATE		 = Pro2Cmd[0];
	 Si446xCmd.REQUEST_DEVICE_STATE.CURRENT_CHANNEL  = Pro2Cmd[1];
	 return Si446xCmd.REQUEST_DEVICE_STATE.CURRENT_CHANNEL;
 }
 
/*!
 * This function is used to send fix payload length packet.
 *
 * @return  TRUE - Started a packet TX / FALSE - No packet sent.
 */
BIT gRadio_CheckReceived(uchar* SYNC_RSSI)
{
	U16 wDelay;
	U8  RF_RX_LEN = 0;
	TickType_t xTime0;

	if(Read_SI4438_nIRQ() == 0)
	{
		Timer400ms=0;
		change_channal_flag =0xaa;
		si446x_get_int_status(0u, 0u, 0u);
		if (Si446xCmd.GET_INT_STATUS.MODEM_PEND & SI446X_CMD_GET_INT_STATUS_REP_SYNC_DETECT_BIT)
		{
			pPositionInPayload =  &phy_rx_packet.dat[0u];														//chu shi hua zhi zhen
			delay_ms(20);
		
			si446x_get_packet_info(0,64,0 );
			RF_RX_LEN = Si446xCmd.PACKET_INFO.LENGTH_7_0;																							// 获取帧长
			phy_rx_packet.dat[0]=RF_RX_LEN ;//PSDU.len+2  
			RF_RX_LEN ^= 0xFF;																																				// 解白化
			if(RF_RX_LEN > 252) RF_RX_LEN = 252;
			RF_RX_LEN+=2;																																							// +CRC.len				 
			bPositionInPayload = RF_RX_LEN;																														// base Position
			phy_rx_packet.len=RF_RX_LEN;
			phy_rx_packet.len=RF_RX_LEN+1;                                                   // one byte of phy.len domain
			pPositionInPayload += 1;																																	// 指针指到信道索引

			if(bPositionInPayload<RADIO_RX_ALMOST_FULL_THRESHOLD)																			// 设置 FIFO 接收 THRESHOLD
				si446x_set_property(0x12, 0x01, 0x0C, bPositionInPayload);															// 
			else 
				si446x_set_property(0x12, 0x01, 0x0C, RADIO_RX_ALMOST_FULL_THRESHOLD);

			xTime0 = xTaskGetTickCount();
			while(bPositionInPayload&&(change_channal_flag ==0xaa) && ((xTaskGetTickCount() - xTime0) <= (400/(1000/OS_TICKS_FREQUENCY))))																		// 当数据还未接收完																	// 当数据还未接收完
			{
				if(Read_SI4438_nIRQ() == 0)																																				// 如果无线芯片有中断
				{
					si446x_get_int_status(0u, 0u, 0u);																										// 读取中断状态

					if(Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_RX_FIFO_ALMOST_FULL_BIT)//
					{
						if (bPositionInPayload >= RADIO_RX_ALMOST_FULL_THRESHOLD)														// 如果待读取的数据长度大于RX_ALMOST_FULL_THRESHOLD
						{ 
							si446x_read_rx_fifo(RADIO_RX_ALMOST_FULL_THRESHOLD, pPositionInPayload);					// 读出RX_ALMOST_FULL_THRESHOLD个字节
							bPositionInPayload -= RADIO_RX_ALMOST_FULL_THRESHOLD;															// 
							pPositionInPayload += RADIO_RX_ALMOST_FULL_THRESHOLD;															//
							if((bPositionInPayload>0)&&(bPositionInPayload<RADIO_RX_ALMOST_FULL_THRESHOLD))		// 如果数据未读完并且剩余长度小于RX_ALMOST_FULL_THRESHOLD
								si446x_set_property(0x12, 0x01, 0x0C, bPositionInPayload);											// 设置 FIFO 接收 THRESHOLD为剩余未接收数据长度
						}
						else //if((bPositionInPayload>0)&&(bPositionInPayload<RADIO_RX_ALMOST_FULL_THRESHOLD))//否则（待读取的数据长度小于RX_ALMOST_FULL_THRESHOLD）
						{
							si446x_read_rx_fifo(bPositionInPayload, pPositionInPayload);											// 读出数据
							bPositionInPayload -= bPositionInPayload;
							pPositionInPayload += bPositionInPayload;	
#if 0		
							if(bPositionInPayload<=0)																													// 如果数据已读完
							{
/***********************************************************/				 
								si446x_frr_a_read(0x01);																												// 读快速寄存器A（读RSSI）
								SYNC_RSSI=Si446xCmd.FRR_A_READ.FRR_A_VALUE;																			// 获取RSSI
								CURRENT_CHANNEL=si446x_request_device_state();																	// 获取接收信道
/***********************************************************/
								si446x_set_property(0x12, 0x01, 0x0C, RADIO_RX_ALMOST_FULL_THRESHOLD);
								bPositionInPayload = 0u;
								pPositionInPayload =  &phy_rx_packet.data_u.phy_rx_buffer[0u];//chu shi hua zhi zhen
								//si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_TX);
								//vRadio_StartRX(CURRENT_CHANNEL);
								change_channal_flag =0x00;
								return 1;																																		
							}

							else																																							// 否则（失败，数据没能接收完）
							{
								bPositionInPayload = 0u;																												// 
								RF_RX_LEN= 0u;
/***********************************************************/				
								CURRENT_CHANNEL=si446x_request_device_state();
/***********************************************************/
								si446x_set_property(0x12, 0x01, 0x0C, RADIO_RX_ALMOST_FULL_THRESHOLD);
								wDelay = 0x7FFF;
								while(wDelay--);
								vRadio_StartRX(CURRENT_CHANNEL);
								change_channal_flag =0x00;
								return 0;
							}
#endif
						}
						if(bPositionInPayload<=0)																														// 如果数据已读完了
						{
/***********************************************************/				 
							si446x_frr_a_read(0x01);
							*SYNC_RSSI=Si446xCmd.FRR_A_READ.FRR_A_VALUE;
							rx_channel = si446x_request_device_state();
/***********************************************************/
							si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
							si446x_set_property(0x12, 0x01, 0x0C, RADIO_RX_ALMOST_FULL_THRESHOLD);
							bPositionInPayload = 0u;
							pPositionInPayload =  &phy_rx_packet.dat[0u];//chu shi hua zhi zhen
							//vRadio_StartRX(CURRENT_CHANNEL);
							change_channal_flag =0x00;
							return 1;
						}
					}
				}
			}
	
			bPositionInPayload = 0u;
			RF_RX_LEN= 0u;
/***********************************************************/				
			rx_channel=si446x_request_device_state();
/***********************************************************/		
			si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);
			si446x_set_property(0x12, 0x01, 0x0C, RADIO_RX_ALMOST_FULL_THRESHOLD);			
			wDelay = 0x7FFF;
			while(wDelay--);
			vRadio_StartRX(CURRENT_CHANNEL);
			change_channal_flag =0x00;
			return 0;
		}
		change_channal_flag =0x00;
  } 
 return 0;
}

/*!
*  Set Radio to RX mode, fixed packet length.
*
*  @param channel Freq. Channel
*
*  @note
*
*/
/*!
 *	Check if Packet sent IT flag is pending.
 *
 *	@return   TRUE / FALSE
 *
 *	@note
 *
 */
	

BIT gRadio_CheckTransmitted(U8 *pioFixRadioPacket)
{
  if (Read_SI4438_nIRQ() == 0)
  {
		/* Read ITs, clear pending ones */
		si446x_get_int_status(0u, 0u, 0u);

		/* check the reason for the IT */
		if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PACKET_SENT_PEND_BIT)
		{
			/* Nothing is sent to TX FIFO */
			bPositionInPayload = 0u;

			/* Position to the very beginning of the custom long payload */
			pPositionInPayload = pioFixRadioPacket;

			return 1;
		}

		if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_TX_FIFO_ALMOST_EMPTY_PEND_BIT)
		{
			/* Calculate the number of remaining bytes has to be sent to TX FIFO */
			if(bPositionInPayload > RADIO_TX_ALMOST_EMPTY_THRESHOLD)
			{ // remaining byte more than threshold

				/* Fill TX FIFO with the number of THRESHOLD bytes */
				si446x_write_tx_fifo(RADIO_TX_ALMOST_EMPTY_THRESHOLD, pPositionInPayload);

				/* Calculate how many bytes are sent to TX FIFO */
				bPositionInPayload -= RADIO_TX_ALMOST_EMPTY_THRESHOLD;

				/* Position to the next first byte that can be sent to TX FIFO in next round */
				pPositionInPayload += RADIO_TX_ALMOST_EMPTY_THRESHOLD;
			}
			else if(bPositionInPayload!=0)
			{ // remaining byte less or equal than threshold

				/* Fill TX FIFO with the number of rest bytes */
				si446x_write_tx_fifo(bPositionInPayload, pPositionInPayload);

				/* Calculate how many bytes are sent to TX FIFO */
				bPositionInPayload -= bPositionInPayload;

				/* Position to the next first byte that can be sent to TX FIFO in next round */
				pPositionInPayload += bPositionInPayload;
			}
		}
	}
  return 0;
}

#if 0
U8  RF_FIFO_RX(void)
{
  if (TRUE == gRadio_CheckReceived())
  {
		return TRUE;
  }
  else return FALSE;
}
#endif
U8  RF_FIFO_TX(uchar TX_ChannelNumber,uchar *Txbuff,uchar TX_Len)
{
	TickType_t xTime0;
	change_channal_flag =0xaa;
	Timer400ms=0;
	si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY); 
	vRadio_StartTx( TX_ChannelNumber, Txbuff,TX_Len);

	xTime0 = xTaskGetTickCount();
	while((change_channal_flag == 0xaa) && ((xTaskGetTickCount() - xTime0) <= os_time_ms_to_ticks(400)))	
	{
		if(1 == gRadio_CheckTransmitted(Txbuff))
		{
			vRadio_StartRX(TX_ChannelNumber);
			change_channal_flag = 0x00;
			return 1;
		}	
	}
	vRadio_StartRX(TX_ChannelNumber);
	change_channal_flag = 0x00;
	return 0;
}

#if 1
/*----------------------------------------------------------------------
----------------------------------------------------------------------*/
void PA_power_set(uchar power_lvevl)
{
	si446x_set_property(0x22,0x01,0x01,power_lvevl);

}
uchar  PA_power_get(void)
{
	si446x_get_property(0x22,0x01,0x01);
	return Si446xCmd.GET_PROPERTY.DATA0;
}

void crystal_frequency_tune(uchar xo_value)
{
	Rf_CREL = xo_value;
	xo_value =xo_value - crystal_XO_TUNE;	
	si446x_set_property(0x00,0x01,0x00,xo_value);

}
uchar crystal_frequency_get(void)
{
	si446x_get_property(0x00,0x01,0x00);
	return Si446xCmd.GET_PROPERTY.DATA0 + crystal_XO_TUNE;

}

#endif
uchar curr_rssi_get(void)
{
 si446x_get_modem_status(0);
 return Si446xCmd.GET_MODEM_STATUS.CURR_RSSI;
}


	
	
