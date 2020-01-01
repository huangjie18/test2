#include "jmesh_lamp.h"
#include"../jmesh_gpio.h"
#include"../jmesh/jmesh_save.h"
#include"../../jmesh/jmesh_save_define.h"
#include"../../jmesh/jmesh_save.h"
#include "io_ctrl.h"
#include "app_gpio.h"
#include"../jmesh_pwm.h"

uint8_t read_data[31] = {0};
unsigned char protocol_645_data[15] = {0};
unsigned char uart_return_data[29];
/****************************************************************************************
函数名称：read_meter_when_control(unsigned char value,unsigned char state)
功    能：控制灯的时候发一帧抄表数据，其中包括PWM值,开关控制状态，0f表示关，1f表示开
输    入：PWM值
返    回： return 0或者1
日    期：20190815
****************************************************************************************/
void read_meter_when_control(unsigned char value,unsigned char state)
{
	unsigned int temp;

	temp =(int)value * 6000/100;
	protocol_645_data[0] = state;
	protocol_645_data[1] = temp;
	protocol_645_data[2] = temp>>8;
	protocol_645_data[3] = 0x00;
	protocol_645_data[4] = 0x00;
	protocol_645_data[5] = 0x00;
	protocol_645_data[6] = 0x00;
	protocol_645_data[7] = 0x00;
	protocol_645_data[8] = 0x00;
		
	memcpy(protocol_645_data+9, meter_addr, 6);		
	protocol_64507_di_t di = {0x01, 0x03, 0xdc, 0xbc};
	protocol_64507_pack(meter_addr, di, 0x34, sizeof(protocol_645_data), protocol_645_data, 31 , read_data);
	jmesh_uart_send(JMESH_UART_0,sizeof(read_data),read_data);
}

/****************************************************************************************
函数名称：lamp_return_handler(unsigned char* data, unsigned short len)
功    能：处理计量底板串口上报的数据
输    入：data：串口接收的数据，数据长度：len
返    回： return 0或者1
日    期：20190815
****************************************************************************************/
unsigned char lamp_return_handler(unsigned char* data, unsigned short len)
{
	unsigned short offset;
	unsigned char ctr_code;
//	memcpy(uart_return_data,data,len);
    if(0<protocol_64507_pick(len,data,&offset))
	{
		ctr_code = *(data+offset+8);
		switch(ctr_code)
		{	
			case 0x36:
			    report_meter_handler(len, data, 0);
			    return 1;
			case 0xB4:
			    return 1;						
			default:
				break;
		}
	}
	return 0;
}

//单灯灯控
#if(Lamp_Control_Enable==1)
unsigned char lamp_mode[16];		//灯控模式

//初始化IO
void lamp_control_init(void)
{	
	io_pin_dir_set(22,JMESH_GPIO_MODE_OUTPUT);
}

//开灯
void lamp_control_on(void)
{
		io_pin_set(22);
}

//关灯
void lamp_control_off(void)
{
		io_pin_clear(22);
}

void lamp_mode_init(void)
{
	memset(lamp_mode,0xff,16);
	lamp_mode[0] = 0;
	
	//lamp_mode[1] = 0x64;
	//lamp_mode[2] = 0;
	
	lamp_mode[1] = 0;
	lamp_mode[2] = 20;
	lamp_mode[3] = 50;
	lamp_mode[4] = 100;
	lamp_mode[15] = 0x64;
	lamp_mode_info_save();
}

void lamp_mode_info_save(void)
{
	jmesh_save_write(JMESH_SAVE_LAMP_MODE_INFO,16,lamp_mode);
}

void lamp_mode_info_read(void)
{
	unsigned char pwm_value;
	
	if(0==jmesh_save_read(JMESH_SAVE_LAMP_MODE_INFO,16,lamp_mode))
	{
		lamp_mode_init();
	}
	
	if(0==jmesh_save_read(JMESH_SAVE_LAMP_PWM,1,&pwm_value))
	{
		pwm_value = 0;
	}
	if(pwm_value == 0)
	{
		lamp_control_off();
		read_meter_when_control(pwm_value,0x0f);
		jmesh_pwm_stop(JMESH_PWM_CHANNEL_0);
	}
	else
	{		
		if(pwm_value >= 100)
			pwm_value = 100;
		lamp_control_on();
		read_meter_when_control(pwm_value,0x1f);
		jmesh_pwm_start(21,JMESH_PWM_CHANNEL_0,pwm_value,100-pwm_value);
	}
}

void lamp_mode_control_handler(unsigned char mode,unsigned char value,unsigned char device_type)
{
	unsigned char pwm_value;
	
	if(device_type!=0xff)
		return ;
	
	if(mode > 15)
	return ;
	
//	if(value == 0xff)
//	{
		pwm_value = lamp_mode[mode];
//	}
//	else
//	{
//		pwm_value = value;
//	}
	if(pwm_value == 0xff)		//场景无效
		return ;
	
	if(pwm_value == 0)
	{
		lamp_control_off();
		read_meter_when_control(pwm_value,0x0f);
		jmesh_pwm_stop(JMESH_PWM_CHANNEL_0);
	}
	else
	{		
		if(pwm_value >= 100)
			pwm_value = 100;
		lamp_control_on();
		read_meter_when_control(pwm_value,0x1f);
		jmesh_pwm_start(21,JMESH_PWM_CHANNEL_0,pwm_value,100-pwm_value);
	}
	
	jmesh_save_write(JMESH_SAVE_LAMP_PWM,1,&pwm_value);
}

unsigned char Lamp_Control_rev_data_check(unsigned char *data,unsigned short len)
{
	unsigned short offset;
	unsigned char ctr_code,buff[20];
	//unsigned char buff_1[50];
	unsigned char DI[4] = {0xDD,0xDD,0xDD,0xDD};
	unsigned char pwm_value;
		
	if(0<protocol_64507_pick(len,data,&offset))
	{
		ctr_code = *(data+offset+8);
		switch(ctr_code)
		{	
			case 0x34:                                     
				if(memcmp(data+offset+10,DI,4) == 0)
				{	
					pwm_value = *(data+offset+14) - 0x33;
					if(pwm_value == 0)
					{
						lamp_control_off();
						read_meter_when_control(pwm_value,0x0f);
						jmesh_pwm_stop(JMESH_PWM_CHANNEL_0);
					}
					else
					{		
						if(pwm_value >= 100)
							pwm_value = 100;
						lamp_control_on();
						read_meter_when_control(pwm_value,0x1f);
						jmesh_pwm_start(21,JMESH_PWM_CHANNEL_0,pwm_value,100-pwm_value);
					}
					jmesh_save_write(JMESH_SAVE_LAMP_PWM,1,&pwm_value);
					
					connect_state = 1;					
					memcpy(buff,data+offset,8);
					buff[8] = 0xB4;
					buff[9] = 0x00;
					buff[10] = check_sum(10,buff);
					buff[11] = 0x16;

//					JMESH_BIG_ENDIAN_FILL2(expand_opcode_realcomm_app_read_meter_return,buff_1);
//					memcpy(buff_1+2,buff,12);			
//					jmesh_access_send(realcomm_send_save.dst,realcomm_send_save.nid,realcomm_send_save.aid,&app_expand,message_app_expand2,12+2,buff_1,JMESH_SEND_ALL_CHANNEL);
					message_realcomm_server_data_return_handler(12,buff);							
					return 1;
				}	
				break; 			
			default:
				break;
		}
	
	}
	return 0;
}	

unsigned char Lamp_Control_rev_data_by_node(unsigned char *data,unsigned short len)
{
	unsigned short offset;
	unsigned char ctr_code,buff[20];
	unsigned char DI[4] = {0xDD,0xDD,0xDD,0xDD};
	unsigned char pwm_value;
		
	if(0<protocol_64507_pick(len,data,&offset))
	{
		ctr_code = *(data+offset+8);
		switch(ctr_code)
		{	
			case 0x34:                                     
				if(memcmp(data+offset+10,DI,4) == 0)
				{	
					pwm_value = *(data+offset+14) - 0x33;
					if(pwm_value == 0)
					{
						lamp_control_off();
						read_meter_when_control(pwm_value,0x0f);
						jmesh_pwm_stop(JMESH_PWM_CHANNEL_0);
					}
					else
					{		
						if(pwm_value >= 100)
							pwm_value = 100;
						lamp_control_on();
						read_meter_when_control(pwm_value,0x1f);
						jmesh_pwm_start(21,JMESH_PWM_CHANNEL_0,pwm_value,100-pwm_value);
					}
					jmesh_save_write(JMESH_SAVE_LAMP_PWM,1,&pwm_value);
					
					connect_state = 1;					
					memcpy(buff,data+offset,8);
					buff[8] = 0xB4;
					buff[9] = 0x00;
					buff[10] = check_sum(10,buff);
					buff[11] = 0x16;
					app_read_meter_by_node_uart_handler(12,buff);							
					return 1;
				}	
				break; 			
			default:
				break;
		}
	
	}
	return 0;
}	

//APP直接控制处理
unsigned char APP_BD_B101_Lamp_handler(unsigned char *data,unsigned short len,unsigned char rev_type)
{
	DL_APP_BD_FRAME_T *p;
	uint8 ctr;
	unsigned short offset;
	unsigned char ctr_code,buff[20];
	unsigned char DI[4] = {0xDD,0xDD,0xDD,0xDD};
	unsigned char pwm_value;
	
	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
		ctr = 0x81;
	else if(p->ctr == 0x04)
		ctr = 0x84;
	if(0<protocol_64507_pick(p->load_len-2,p->data,&offset))
	{
		ctr_code = *(p->data+offset+8);
		switch(ctr_code)
		{	
			case 0x34:                                     
				if(memcmp(p->data+offset+10,DI,4) == 0)
				{	
					pwm_value = *(p->data+offset+14) - 0x33;
					if(pwm_value == 0)
					{
						lamp_control_off();
						read_meter_when_control(pwm_value,0x0f);
						jmesh_pwm_stop(JMESH_PWM_CHANNEL_0);
					}
					else
					{		
						if(pwm_value >= 100)
							pwm_value = 100;
						lamp_control_on();
						read_meter_when_control(pwm_value,0x1f);
						jmesh_pwm_start(21,JMESH_PWM_CHANNEL_0,pwm_value,100-pwm_value);
						
					}
					jmesh_save_write(JMESH_SAVE_LAMP_PWM,1,&pwm_value);
					memcpy(buff,p->data+offset,8);
					buff[8] = 0xB4;
					buff[9] = 0x00;
					buff[10] = check_sum(10,buff);
					buff[11] = 0x16;
					
					app_local_read_meter.type = p->type;
					app_local_read_meter.ctr = ctr;
					memcpy(app_local_read_meter.addr, p->addr,6);
					app_local_read_meter.DT[0] = p->DT[0];
					app_local_read_meter.DT[1] = p->DT[1];
					app_local_read_meter.rev_type = rev_type;
					APP_BD_protocol_handler_callback(app_local_read_meter.type,
													app_local_read_meter.addr,
													app_local_read_meter.ctr,
													app_local_read_meter.DT[0],
													app_local_read_meter.DT[1],buff,12,app_local_read_meter.rev_type);
					return 1;
				}	
				break; 			
			default:
				break;
		}
	
	}
	return 0;
}


#endif
