/*! @file _470_app.c
 * @brief 
 *
 */
#include "_470_phy.h"
#include "_470_app.h"
#include "jmesh_uart.h"
#include "jmesh_app_pro.h"
#include "APP_protocol.h"
#include "os_timer_event.h"
#include "protocol_mx188.h"
#include "kehua_gas.h"		
#include "app_config.h"
#include "kehua_window.h"
#include "kehua_transfer_arm.h"
#include "app_protocol.h"

os_timer_event_t _470_report_timer;
static uint8_t report_cnt = 0;

void _470_report_return_handler(unsigned short len,unsigned char* data);

// 接收并处理470数据 
void _470_app_recv(void)
{
	uint8_t *rcv_data = NULL; 
	uint8_t rcv_len, rcv_rssi;
	
	// 调用物理层接口接收数据 
	rcv_data = phy_rx_pkt(&rcv_len, &rcv_rssi);
	if(rcv_data != NULL)
	{
		mx188_frame_t frame = {0};
						
		if(protocol_mx188_parse2(rcv_data, rcv_len, &frame))
		{
			if (0 != memcmp(frame.addr.u8, meter_addr, METER_ADDR_SIZE))
				return;
			if (frame.CTR_10.direction == FROM_HOST_NODE)
			{			
				if(current_realcomm_task_id != GATEWAY_READ_METER)
				{
					blueToothOr470 = FROM_470;
					if (current_realcomm_task_id == NO_TASK)
					{
						memcpy(&mx188_frame_record, &frame, sizeof(mx188_frame_t));	
						current_realcomm_task_id = GATEWAY_READ_METER;
						#ifdef  KEHUA_WINDOW
						window_rev_data_check(rcv_data + frame.valid_data_offset,frame.valid_data_len);
						#endif
						#if	(defined KEHUA_TRANSFER_ARM)||(defined KEHUA_ARM_BX2400_470)
						current_realcomm_task_id = GATEWAY_READ_METER;
						if(transfer_arm_rev_data_check(rcv_data + frame.valid_data_offset,frame.valid_data_len) == 1)
							return;
						#endif
						if(APP_BD_protocol_CheckFrame(rcv_data + frame.valid_data_offset,frame.valid_data_len) == 1)		//网关设置模块参数
						{
							current_realcomm_task_id = GATEWAY_READ_METER;
							APP_BD_protocol_handler(rcv_data + frame.valid_data_offset,frame.valid_data_len,4);
							current_realcomm_task_id = NO_TASK;	
							return ;	
						}
						jmesh_uart_send(JMESH_UART_0, frame.valid_data_len, rcv_data + frame.valid_data_offset);	
						read_meter_delay_para[0] = 0;
						os_timer_event_set(&read_meter_timer,METER_MAX_RESPONSE_MS,(os_timer_event_caller_t)read_meter_overtime_handler, &read_meter_delay_para);
						
					}
					else 
					{
						// 当前串口正在处理其他任务，延时一下再处理本任务,先把数据(原始数据)存起来,  
						memcpy(read_meter_data_cache, rcv_data, rcv_len);
						read_meter_data_cache_len = rcv_len;
						read_meter_delay_para[0] = 1;
						read_meter_delay_para[1] = GATEWAY_READ_METER;
						os_timer_event_set(&read_meter_timer, 300, (os_timer_event_caller_t)read_meter_overtime_handler, &read_meter_delay_para);
					}
				}
			}
			else // 来自主节点的应答 
			{
				if (((report_state & 0x02) == 0x02) && (mx188_seq == frame.seq))
				{
					_470_report_return_handler(rcv_len, rcv_data);
				}
			}
		}
		else if (1 == APP_BD_protocol_handler(rcv_data, rcv_len, 2))
		{
			;
		}
		else
		{
			;
		}	
	}
}


void _470_report_return_handler(unsigned short len,unsigned char* data)
{
	if (report_state != 0)
	{		
		#ifdef  KEHUA_GAS 
		if (report_content == 1)
			gas_con_report_flag = false;
		#endif
	}
	report_state = 0; // 上报成功，清除所有状态位
	os_timer_event_remove(&_470_report_timer);
}

void _470_report_overtime_handler(void *arg)
{
	if ((report_cnt < 2) && (report_state != 0))
	{		
		report_cnt++;
		phy_set_channel(PRIVATE_CHANNEL, ++phy_pib.phyCurrentChannelIndex);
		phy_tx_pkt(report_meter_data + 2, report_meter_data_len - 2, 0); 
		os_timer_event_restart(&_470_report_timer);
	}
	else
	{
		report_cnt = 0;
		report_state &= ~0x02; 
	}
}

// 通过470上报数据，发送缓冲区report_meter_data与蓝牙上报共用
void report_by_470(void)
{
	os_timer_event_set(&_470_report_timer, 2000, (os_timer_event_caller_t)_470_report_overtime_handler, NULL);
	report_state |= 0x02;
	phy_tx_pkt(report_meter_data + 2, report_meter_data_len - 2, 0);
	report_cnt++;	
}


