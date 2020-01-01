#include "jmesh_scene.h"
#include"../jmesh_gpio.h"
#include"../jmesh/jmesh_save.h"
#include"../../jmesh/jmesh_save_define.h"
#include"../../jmesh/jmesh_save.h"

//情景面板
#if(Scene_Control_Enable==1)

//unsigned char scene_control_handler(unsigned char* data, unsigned short len)
//{
//	unsigned short offset;
//  int ret;
//	unsigned char ctr_code,buff[10],DI[4] = {0x81,0xff,0xcc,0xbc};
//	unsigned char mode1[7] = {0x00,0xff,0x00,0x00,0x00,0x00,0x00};	//全关
//	unsigned char mode2[7] = {0x01,0xff,0x00,0x00,0x00,0x00,0x00};	//情景1
//	unsigned char mode3[7] = {0x02,0xff,0x00,0x00,0x00,0x00,0x00};	//情景2
//	unsigned char mode4[7] = {0x0f,0xff,0x00,0x00,0x00,0x00,0x00};	//全开
//	
//  if(0<(ret=protocol_64507_pick(len,data,&offset)))
//	{
//		ctr_code = *(data+offset+8);
//		switch(ctr_code)
//		{	
//			case 0x91:                                      
//				buff[0] = *(data+offset+10) - 0x33;
//				buff[1] = *(data+offset+11) - 0x33;
//				buff[2] = *(data+offset+12) - 0x33;
//				buff[3] = *(data+offset+13) - 0x33;
//				buff[4] = *(data+offset+14) - 0x33;
//				buff[5] = *(data+offset+15) - 0x33;
//				buff[6] = *(data+offset+16) - 0x33;
//				
//				if(memcmp(buff,DI,4) == 0)
//				{
//					if(buff[5] == 0)
//						send_app_broadcast_data(6,mode1);
//					else if(buff[5] == 1)
//						send_app_broadcast_data(6,mode2);
//					else if(buff[5] == 2)
//						send_app_broadcast_data(6,mode3);
//					else if(buff[5] == 4)
//						send_app_broadcast_data(6,mode4);
//					
//					return 1;
//				}
//				break; 			
//			default:
//	
//		}
//	}
//	return 0;



//}

unsigned char scene_control_handler(unsigned char* data, unsigned short len)
{
	unsigned short offset;
	int ret;
	DL_APP_BD_FRAME_T *p;
	unsigned char report_buff[50],report_len;
	unsigned char ctr_code,buff[10],DI[4] = {0x81,0xff,0xcc,0xbc};
	
  if(0<(ret=protocol_64507_pick(len,data,&offset)))
	{
		ctr_code = *(data+offset+8);
		switch(ctr_code)
		{	
			case 0x91:                                      
				buff[0] = *(data+offset+10) - 0x33;
				buff[1] = *(data+offset+11) - 0x33;
				buff[2] = *(data+offset+12) - 0x33;
				buff[3] = *(data+offset+13) - 0x33;
				buff[4] = *(data+offset+14) - 0x33;
				buff[5] = *(data+offset+15) - 0x33;
				buff[6] = *(data+offset+16) - 0x33;
				
				if(memcmp(buff,DI,4) == 0)
				{
					
					p = (DL_APP_BD_FRAME_T *)report_buff;
					p->start = 0xBD;
					p->type = 0x0F;
					memcpy(p->addr,meter_addr,6);
					p->ctr = 0x04;
					p->load_len = 7+2;
					p->DT[0] = 0x02;
					p->DT[1] = 0xCF;
					p->data[0] = buff[5];
					p->data[1] = 0x64;
					p->data[2] = 0x00;
					p->data[3] = 0x00;
					p->data[4] = 0x00;
					p->data[5] = 0x00;
					p->data[6] = 0x00;
					p->data[7] = check_sum(7+12,report_buff+1);
					p->data[8] = 0x16;
					report_len = 7 + 15;
					
					report_meter_repeat_num = 2;
					memset(report_meter_data,0x00,200);
					JMESH_BIG_ENDIAN_FILL2(expand_opcode_realcomm_meter_reprot,report_meter_data);
					memcpy(report_meter_data+2,report_buff,report_len);
					report_meter_data_len = report_len+2;
					jmesh_access_send(realcomm_send_save.dst,realcomm_send_save.nid,realcomm_send_save.aid,&app_expand,message_app_expand2,report_meter_data_len,report_meter_data,JMESH_SEND_ALL_CHANNEL);
					os_timer_event_set(&report_meter_timer, 8000, (os_timer_event_caller_t)report_meter_overtime_handler, NULL);
					return 1;
				}
				break; 			
			default:
	
		}
	}
	return 0;



}




#endif
