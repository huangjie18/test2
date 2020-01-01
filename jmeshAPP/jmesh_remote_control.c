/********************************************************************************************
名    称：遥控开关
功    能：检测串口的数据，对数据进行数据分析，并变换为三个独立的按键进行数据上报，开：0x0F,关：0x00，对应的按键分别是AA,BB,CC
作    者：LJJ
创建时间：20190724
修改时间：
备    注：
**************************************************************************************************/

#include "jmesh_remote_control.h"

#if (defined Three_Remote_Switch)||(defined Four_Remote_Switch)||(defined Ble_Remote_Switch)

#define SW_ON 0x0F
#define SW_OFF 0x00

unsigned char remote_mode;
unsigned char button_state_set_buff[25];
unsigned char button_ctr_state,button_ctr_num,button_ctr_mode,button_on,button_off,button_state_mode[4];
unsigned char ButtonInfo[20];
unsigned char button_num,switch_mode = 0;

static unsigned char key_state[4];
static const unsigned char ButtonInfoInit[20] = {0xAA,0x0F,0x00,0x00,0xBB,0x0F,0x00,0x00,0xCC,0x0F,0x00,0x00,0xDD,0x0F,0x00,0x00,0xEE,0x0F,0x00,0x00};	
os_timer_event_t button_state_set_timer;
os_timer_event_t button_mode_set_timer;
	
/***********************************************************************************************
名    称：jmesh_remote_control_BD_protocol_pakage_send(unsigned char* data, unsigned short len)
功    能：按键信息通过BD协议上报给主节点
输    入：num：按键编号； mode：按键模式，开或者关
返    回： return 0或者1
日    期：20190725
************************************************************************************************/
void jmesh_remote_control_BD_protocol_pakage_send(unsigned char num,unsigned char mode)
{
	DL_APP_BD_FRAME_T *p;
	unsigned char report_buff[50],report_len;
	
	unsigned char key_num,switch_mode;
	key_num = num;
	switch_mode = mode;
	
	p = (DL_APP_BD_FRAME_T *)report_buff;
	p->start = 0xBD;
	p->type = 0x13;
	memcpy(p->addr,meter_addr,6);
	p->ctr = 0x04;
	p->load_len = 7+2;//6+2;
	p->DT[0] = 0x01;
	p->DT[1] = 0xD3;
	p->data[0] = key_num;//buff[5];
	p->data[1] = switch_mode;//0x64;
	p->data[2] = 0x00;
	p->data[3] = 0x00;
	p->data[4] = 0x00;
	p->data[5] = 0x00;
	p->data[6] = 0x00;
	p->data[7] = check_sum(7+12,report_buff+1);
	p->data[8] = 0x16;
	report_len = 7 + 15;		
	
	report_meter_handler( report_len, report_buff, 0);
	
//	memset(report_meter_data,0x00,200);
//	JMESH_BIG_ENDIAN_FILL2(expand_opcode_realcomm_meter_reprot,report_meter_data);
//	memcpy(report_meter_data+2,report_buff,report_len);
//	report_meter_data_len = report_len+2;
//	jmesh_access_send(realcomm_send_save.dst,realcomm_send_save.nid,realcomm_send_save.aid,&app_expand,message_app_expand2,report_meter_data_len,report_meter_data,JMESH_SEND_ALL_CHANNEL);
}

#if (defined Three_Remote_Switch)||(defined Ble_Remote_Switch)
/***********************************************************************************************
名    称：Remote_Memeory_Init(void)
功    能：初始化蓝牙遥控的设置参数，默认值：开：0x0F，关：0x00
输    入：空
返    回：无
日    期：20190823
************************************************************************************************/	
void Remote_Memeory_Init(void)
{	
	if(0 == jmesh_save_read(JMESH_REMOTE_CONTROL_INFO,20,ButtonInfo))
	{
		memcpy(ButtonInfo,ButtonInfoInit,20);	
	}
}

/***********************************************************************************************
名    称：Remote_factory_reset(void)
功    能：恢复出厂化设置，删除flash里面存储的按键参数信息，然后系统复位，重新读取默认参数
输    入：空
返    回：无
日    期：20190823
************************************************************************************************/
void Remote_factory_reset(void)
{
	jmesh_save_delete(JMESH_REMOTE_CONTROL_INFO);
	delay_ms(200);
	system_reset();
}	


/***********************************************************************************************
名    称：void button_state_set_data_pakage_send(unsigned char Button_Num,unsigned char SW_State)
功    能：将按键控制信息发送给底板，底板控制继电器保证按键与后台状态一致
输    入：Button_Num：需要控制的按键编号   SW_State：控制状态为是开、关
返    回：无
日    期：20190823
************************************************************************************************/
void button_state_set_data_pakage_send(unsigned char Button_Num,unsigned char SW_State)
{
	unsigned char protocol_64507_data[9];
	unsigned char DI[4];
	protocol_64507_di_t D1[4] = {0x01,0x01,0x20,0x97};			//off
	protocol_64507_di_t D2[4] = {0x02,0x01,0x20,0x97};			//on
	unsigned char protocol_key[4] ={0x00,0x00,0x00,0x00};
	unsigned char ctr_code[4] ={0x78,0x56,0x34,0x12};
	if(SW_State == 0x00)
	{
		memcpy(DI,D1,4);
	}
    else if(SW_State == 0x01)
    {
		memcpy(DI,D2,4);
	}
    memcpy(protocol_64507_data,protocol_key,4);
	memcpy(protocol_64507_data+4,ctr_code,4);
	memcpy(protocol_64507_data+8,&Button_Num,1);
	
	protocol_64507_pack(meter_addr,DI,0x14,sizeof(protocol_64507_data),protocol_64507_data,25,button_state_set_buff);
	jmesh_uart_send(JMESH_UART_0,25,button_state_set_buff);	
}

//位模式定时器处理函数
void button_state_set_timer_handler(void)
{
	button_state_set_data_pakage_send(button_off,0x00);
}


/***********************************************************************************************
名    称：void button_state_set_start(void)
功    能：位模式启动跳合闸状态，防止两条数据发送时间间隔短，被误认为是以同一条数据
输    入：无
返    回：无
日    期：20190823
************************************************************************************************/
void button_state_set_start(void)
{	
	os_timer_event_set(&button_state_set_timer,300, (os_timer_event_caller_t)button_state_set_timer_handler,NULL);
}

/***********************************************************************************************
名    称：void APP_BD_D300_handler(uint8 *data,uint16 len,uint8 rev_type)
功    能：设置遥控模式参数
输    入：data：空中或者串口数据，rev_type：接收的类型 蓝牙或者470
返    回：无
日    期：20190823
************************************************************************************************/
void APP_BD_D300_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;				
	uint8 ctr,error;
	uint8 buff[15];
	UINT16 i,j;

	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
	{
		ctr = 0x81;
		j = 0;
		for(i=0;i<4;i++)
		{
			buff[j++] = ButtonInfo[4*i];
			buff[j++] = ButtonInfo[4*i+1];
			buff[j++] = ButtonInfo[4*i+2];
		}
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],buff,12,rev_type);
	}
	else if(p->ctr == 0x04)
	{
		ctr = 0x84;		
		if(p->load_len-2 != 12)
		{
			if(p->ctr == 0x01)
				ctr = 0xC1;
			else if(p->ctr == 0x04)
				ctr = 0xC4;
			error = BD_InvalidData;
			APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
			return ;
		}
		
		for(i=0;i<4;i++)
		{
			ButtonInfo[4*i] = p->data[3*i];
			ButtonInfo[4*i+1] = p->data[3*i+1];
			ButtonInfo[4*i+2] = p->data[3*i+2];

		}
		jmesh_save_write(JMESH_REMOTE_CONTROL_INFO,20,ButtonInfo);
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);
	}

}

/***********************************************************************************************
名    称：void APP_BD_D302_handler(uint8 *data,uint16 len,uint8 rev_type)
功    能：读取遥控参数：设置的开关模式，按键当前状态
输    入：data：空中或者串口数据，rev_type：接收的类型 蓝牙或者470
返    回：无
日    期：20190823
************************************************************************************************/
void APP_BD_D302_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;				
	uint8 ctr;
	uint8 buff[20] ={0};
	UINT16 i,j;
		
	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
	{
		ctr = 0x81;
		j = 0;
		for(i=0;i<4;i++)
		{
			buff[j++] = ButtonInfo[4*i];
			buff[j++] = ButtonInfo[4*i+1];
			buff[j++] = ButtonInfo[4*i+2];
			buff[j++] = ButtonInfo[4*i+3];
		}
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],buff,20,rev_type);
	}
}

/***********************************************************************************************
名    称：void APP_BD_D303_handler(uint8 *data,uint16 len,uint8 rev_type)
功    能：后台或者抄控器控制灯，位模式或者mode模式
输    入：data：空中或者串口数据，rev_type：接收的类型 蓝牙或者470
返    回：无
日    期：20190823
************************************************************************************************/
void APP_BD_D303_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;				
	uint8 ctr,error;
	UINT16 i,j;
	button_on = 0x00;
	button_off = 0x00;
	memcpy(button_state_mode,0x00,4);
	
	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x04)
	{
		ctr = 0x84;		
		if(p->load_len-2 != 11)
		{
			if(p->ctr == 0x01)
				ctr = 0xC1;
			else if(p->ctr == 0x04)
				ctr = 0xC4;
			error = BD_InvalidData;
			APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
			return ;
		}		
		if(p->data[0]!=0)
		{

			button_ctr_num = p->data[0];          //控制的按键
			button_ctr_mode = p->data[1];         //控制模式  1为位模式，2为mode模式
			button_ctr_state = p->data[2];        //控制按键是开还是关
			
			if(button_ctr_mode == 0x01)           //设置按键位模式
			{
				button_off = (button_ctr_num^button_ctr_state)&button_ctr_num;
				button_on = (~(button_ctr_num^button_ctr_state))&button_ctr_num;				
				if(button_on != 0)
				{
					button_state_set_data_pakage_send(button_on,0x01);
				}
				if(button_off != 0)
				{
					button_state_set_start();
				}
			}
			
			if(button_ctr_mode == 0x02)			//按键状态mode模式
			{
				memcpy(button_state_mode,&p->data[3],4);
				for(i=0;i<4;i++)
				{
					if(button_state_mode[i] == ButtonInfo[4*i+1])
					{
						button_on = button_on|(0x01<<i);
						
					}
					else if(button_state_mode[i] == ButtonInfo[4*i+2])
					{
						button_off = button_off|(0x01<<i);
					}
				}
				button_on = button_on & button_ctr_num;
				button_off = button_off & button_ctr_num;
				if(button_on != 0)
				{
					button_state_set_data_pakage_send(button_on,0x01);
				}
				if(button_off != 0)
				{
					button_state_set_start();
				}
			}
			
			for(j=0;j<4;j++)
			{
				if(((button_on>>j)&0x01) ==0x01)		//
				{
					button_num = ButtonInfo[4*j];
					switch_mode = ButtonInfo[4*j+1];
					led_on(button[j].led);
					ButtonInfo[4*j+3] = switch_mode; 
					remote_mode = remote_mode|(0x01<<j);
				}
				if(((button_off>>j)&0x01) ==0x01)
				{
					button_num = ButtonInfo[4*j];
					switch_mode = ButtonInfo[4*j+2];
					led_off(button[j].led);
					ButtonInfo[4*j+3] = switch_mode;
					remote_mode = remote_mode&(~(0x01<<j));
				}							
			}
			
			jmesh_save_write(JMESH_SAVE_REMOTE_MODE_INFO,1,(unsigned char*)&remote_mode);
			jmesh_save_write(JMESH_REMOTE_CONTROL_INFO,20,ButtonInfo);
			
		}
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);
	}
}
#endif

#ifdef Ble_Remote_Switch
static unsigned char button_flag,button_num_buff,button_mode_buff;

os_timer_event_t button_state_buff_send_timer;
os_timer_event_t button_flag_clear_timer;


void button_state_buff_send_handle(void)
{
	jmesh_remote_control_BD_protocol_pakage_send(button_num_buff,button_mode_buff);
}

/***********************************************************************************************
名    称：void button_state_buff_send_timer_statr(void)
功    能：延时1.5s发送按键状态缓存数据
输    入：
返    回：无
日    期：20190911
************************************************************************************************/
void button_state_buff_send_timer_statr(void)
{
	os_timer_event_set(&button_state_buff_send_timer,1500,(os_timer_event_caller_t)button_state_buff_send_handle,NULL);
}

void button_flag_clear_timer_handle(void)
{
	button_flag = 0;
}

/***********************************************************************************************
名    称：void button_flag_clear_timer_statr(void)
功    能：延时2s清除按键标志位
输    入：
返    回：无
日    期：20190911
************************************************************************************************/
void button_flag_clear_timer_statr(void)
{
	os_timer_event_set(&button_flag_clear_timer,2000,(os_timer_event_caller_t)button_flag_clear_timer_handle,NULL);
}

/***********************************************************************************************
名    称：void button_data_send_handle(unsigned char num,unsigned char mode)
功    能：2s内连续按下同一个按键时，如果最后一次按键状态和第一次相同，只发送第一个状态，
          否则发送第一次和最后一次的状态，2s内按下不同的按键时立即发送
输    入：
返    回：无
日    期：20190911
************************************************************************************************/
void button_data_send_handle(unsigned char num,unsigned char mode)
{	
	static unsigned char last_num,last_mode;
	
	if(num != last_num)
	{
		button_flag = 1;
		last_num = num;	
		last_mode = mode;
		jmesh_remote_control_BD_protocol_pakage_send(num,mode);
		os_timer_event_restart(&button_flag_clear_timer);
	}
	else 
	{
		os_timer_event_restart(&button_flag_clear_timer);
		if(button_flag == 1)
		{
			button_num_buff = num;			
			button_mode_buff = mode;
			if(last_mode != button_mode_buff)
			{
				os_timer_event_restart(&button_state_buff_send_timer);	
			}
			else
				os_timer_event_remove(&button_state_buff_send_timer);
		}
		else 
		{
			button_flag = 1;
			last_mode = mode;
			jmesh_remote_control_BD_protocol_pakage_send(num,mode);
			os_timer_event_restart(&button_flag_clear_timer);
		}
	}	
}


void jmesh_remote_button_handle(uint8_t button_state,uint8_t times)
{
	unsigned char i;
	static unsigned char new_button_state,old_button_state;

	new_button_state = button_state;
	
	if(old_button_state != new_button_state)
	{	
		for(i=0;i<4;i++)
		{
			if(new_button_state ==(0x01<<i))
			{
				button_num = ButtonInfo[4*i];
				key_state[i] = key_state[i]^0x01;
				if(key_state[i])
				{
					switch_mode = ButtonInfo[4*i+1];
					led_on(button[i].led);
				}
				else
				{
					switch_mode = ButtonInfo[4*i+2];
					led_off(button[i].led);
				}
//				jmesh_remote_control_BD_protocol_pakage_send(button_num,switch_mode);
				button_data_send_handle(button_num,switch_mode);
				
				ButtonInfo[4*i+3] = switch_mode; 
				remote_mode = remote_mode^(key_state[i]<<i);					
			}			
		}
		jmesh_save_write(JMESH_SAVE_REMOTE_MODE_INFO,1,(unsigned char*)&remote_mode);
		jmesh_save_write(JMESH_REMOTE_CONTROL_INFO,20,ButtonInfo);
		old_button_state = new_button_state;
	}
	else
		;
}

#endif

#ifdef Three_Remote_Switch
/*****************************************************************************************
函数名称：three_remote_control_handler(unsigned char* data, unsigned short len)
功    能：三路遥控开关处理函数
输    入：data：串口接收的数据，数据长度：len
返    回： return 0或者1
日    期：20190822
*******************************************************************************************/
unsigned char three_remote_control_handler(unsigned char* data, unsigned short len)
{
	unsigned char i = 0;
	unsigned short offset;
	int ret;
	unsigned char ctr_code,buff[10],DI[4] = {0x81,0xff,0xcc,0xbc};
	unsigned char new_key_state;
	static unsigned char old_key_state;	
	
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
			
				new_key_state = buff[5];			
				if(memcmp(buff,DI,4) == 0)
				{

					for(i=0;i<4;i++)
					{
						if(((new_key_state>>i)&0x01) != (((old_key_state>>i))&0x01))
						{
							button_num = ButtonInfo[4*i];
							key_state[i] = key_state[i]^0x01;
							if(((new_key_state>>i)&0x01) == 0x01)
							{
								switch_mode = ButtonInfo[4*i+1];
							}
							else
							{
								switch_mode = ButtonInfo[4*i+2];
							}
							jmesh_remote_control_BD_protocol_pakage_send(button_num,switch_mode);
							ButtonInfo[4*i+3] = switch_mode; 				
						}			
					}
				}
				jmesh_save_write(JMESH_REMOTE_CONTROL_INFO,20,ButtonInfo);
				old_key_state = new_key_state;
				return 1;
            case 0x94:
					return 1;
			default: ;
		}
	}
	return 0;
}
#endif 

#ifdef Four_Remote_Switch
/****************************************************************************************
函数名称：four_remote_control_handler(unsigned char* data, unsigned short len)
功    能：四路遥控开关处理函数(主控CPU烧写场景面板程序，根据场景面板上报的按键数据修改，只用于外带CPU的双模硬件)
输    入：data：串口接收的数据，数据长度：len
返    回： return 0或者1
日    期：20190806
****************************************************************************************/
unsigned char four_remote_control_handler(unsigned char* data, unsigned short len)
{
	unsigned short offset;
	int ret;
	unsigned char ctr_code,buff[10],DI[3] = {0x02,0xdc,0xbc};
	unsigned char key_state,key_num,key_num_send,switch_mode = 0;
	
    if(0<(ret=protocol_64507_pick(len,data,&offset)))
	{
		ctr_code = *(data+offset+8);
		switch(ctr_code)
		{	
			case 0x36:

				buff[0] = *(data+offset+10) - 0x33;
				buff[1] = *(data+offset+11) - 0x33;
				buff[2] = *(data+offset+12) - 0x33;
				buff[3] = *(data+offset+13) - 0x33;
				buff[4] = *(data+offset+14) - 0x33;
				buff[5] = *(data+offset+15) - 0x33;
				buff[6] = *(data+offset+16) - 0x33;
			
				key_num = buff[0];
				key_state = buff[4];
			    
				if(memcmp(buff+1,DI,3) == 0)
				{
					switch(key_num)
					{
					    case 0x01:
							key_num_send = 0xAA;
							if(key_state != 0)
							{
								switch_mode = SW_ON;
							}
							else
							{
								switch_mode = SW_OFF;
							}
							jmesh_remote_control_BD_protocol_pakage_send(key_num_send,switch_mode);
							break;
					    case 0x02:
							key_num_send = 0xBB;
						    if(key_state != 0)
							{
								switch_mode = SW_ON;
							}
							else
							{
								switch_mode = SW_OFF;
							}
							jmesh_remote_control_BD_protocol_pakage_send(key_num_send,switch_mode);
							break;
						case 0x04:
							key_num_send = 0xCC;
						    if(key_state != 0)
							{
								switch_mode = SW_ON;
							}
							else
							{
								switch_mode = SW_OFF;
							}
							jmesh_remote_control_BD_protocol_pakage_send(key_num_send,switch_mode);
							break;
						case 0x03:
							key_num_send = 0xDD;
						    if(key_state != 0)
							{
								switch_mode = SW_ON;
							}
							else
							{
								switch_mode = SW_OFF;
							}
							jmesh_remote_control_BD_protocol_pakage_send(key_num_send,switch_mode);
							break;								
					}
                    return 1;					
				}	
				break; 			
			default:
					;
		}
	}
	return 0;
}
#endif	//Four_Remote_Switch

#endif


