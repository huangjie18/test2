#include "jmesh_lamp_passthrough.h"

#ifdef LAMP_PASSTHROUGH

unsigned char lamp_mode[16];

unsigned char protocol_64507_valid_data[11] = {0};
unsigned char protocol_64507_data_no_di[23] = {0};
unsigned char lamp_switch_state[1];
unsigned char read_meter_data_buff[16];
int read_meter_delay_time = 0;


//灯控模式参数
void APP_BD_CA01_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;				
	uint8 num,ctr,error;
	uint8 buff[50];
	UINT16 i,j;

	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
	{
		ctr = 0x81;
		buff[0] = 1;
		buff[1] = 14;
		j = 2;
		for(i=1;i<=14;i++)
		{
			buff[j++] = i;
			buff[j++] = lamp_mode[i];
		}
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],buff,30,rev_type);
	}
	else if(p->ctr == 0x04)
	{
		ctr = 0x84;
		num = p->data[1];
		
		if(p->load_len-2 != num*2+2)
		{
			if(p->ctr == 0x01)
				ctr = 0xC1;
			else if(p->ctr == 0x04)
				ctr = 0xC4;
			error = BD_InvalidData;
			APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
			return ;
		}
		
		for(i=1;i<=num;i++)
		{
			j = p->data[2*i];
			if(j>0||j<15)
			{
				lamp_mode[j] = p->data[2*i+1];
			}	
		}
		lamp_mode_info_save();
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);
	}
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

void Lamp_factory_reset(void)
{
	jmesh_save_delete(JMESH_SAVE_LAMP_MODE_INFO);
	delay_ms(200);
	system_reset();
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
}



void read_meter_when_control(void)
{
	uint8_t DI[4] ={0x10,0xFF,0xCC,0xBC};
	unsigned char protocol_645_data[1]={0};	
	protocol_64507_pack(meter_addr, DI, 0x11, 0, protocol_645_data, 16 , read_meter_data_buff);
	jmesh_uart_send(JMESH_UART_0,sizeof(read_meter_data_buff),read_meter_data_buff);
}

unsigned char lamp_ctrl_pwm_value,lamp_state;
os_timer_event_t lamp_read_meter_timer;

void lamp_read_meter_timer_handler(void)
{
	read_meter_when_control();
}

void lamp_read_meter_timer_start(void)
{	
	unsigned char meter_addr_buff[6];
	memcpy(meter_addr_buff,meter_addr,6);
	read_meter_delay_time = (int)meter_addr_buff[0]*500+1000;
	LOG(LOG_LVL_INFO,"read_meter_delay_time = %d\n",read_meter_delay_time);
	os_timer_event_set(&lamp_read_meter_timer,read_meter_delay_time, (os_timer_event_caller_t)lamp_read_meter_timer_handler,NULL);
}

uint8_t hex2BCD(uint8_t hex)
{
	uint8_t	i;

	hex %= 100;
	i = hex / 10;
	hex %= 10;
	return ((i<<4)|hex);
}

void lamp_mode_control_handler(unsigned char mode,unsigned char value,unsigned char device_type)
{		
	uint16_t pwm_value;
	uint8_t pwm_bcd[2];	
		
	uint8_t password[4] ={0x02,0x00,0x00,0x00};
	uint8_t ctrl_code[4] ={0x78,0x56,0x34,0x12};
	
	memcpy(protocol_64507_valid_data,password,4);
	memcpy(protocol_64507_valid_data+4,ctrl_code,4);
	
	if(device_type!=0xff)
		return ;
	
	if(mode > 15)
	return ;
	
	pwm_value = (char)lamp_mode[mode];
	
	if(pwm_value == 0xff)		
		return ;
	
//	jmesh_save_write(JMESH_SAVE_LAMP_PWM,1,&pwm_value);
//	
//	jmesh_save_write(JMESH_SAVE_LAMP_MODE_INFO,16,lamp_mode);
	
	if(pwm_value != 0)
	{
		lamp_switch_state[0] = 0x1B;
	}
	else 
	{
		lamp_switch_state[0] = 0x1A;
	}
	
	if (((lamp_switch_state[0] == 0x1A) && (pwm_value<= 100)) || (lamp_switch_state[0] == 0x1B)) 
	{
		pwm_value = (int)pwm_value*10;
		pwm_bcd[0] = hex2BCD(pwm_value % 100);
		pwm_bcd[1] = hex2BCD(pwm_value / 100);
				
		memcpy(protocol_64507_valid_data+8,lamp_switch_state,1);
		memcpy(protocol_64507_valid_data+9,pwm_bcd,2);		
		protocol_64507_no_di_pack(meter_addr, 0x1C, 0x0B,  protocol_64507_valid_data, 23, protocol_64507_data_no_di);
		jmesh_uart_send(JMESH_UART_0,sizeof(protocol_64507_data_no_di),protocol_64507_data_no_di);
		lamp_read_meter_timer_start();
	}	
}

unsigned char lamp_return_handler(unsigned char* data, unsigned short len)
{
	unsigned short offset;
	int ret;
	unsigned char ctr_code;
	unsigned char return_buff[50];
	memcpy(return_buff,data,len);
//	memcpy(uart_return_data,data,len);
    if(0<(ret=protocol_64507_pick(len,data,&offset)))
	{
		ctr_code = *(data+offset+8);
		switch(ctr_code)
		{	
			case 0x9C:
			    return 1;						
			default:
				break;
		}
	}
	return 0;
}


//unsigned char protocol_64507_valid_data[11] = {0};
//unsigned char protocol_64507_data_no_di[23] = {0};
//void single_lamp_broadcast_control(uint8 *data,uint16 len,uint8 rev_type)
//{		

//	uint8 ctr;
//	int i,ctrl_mode[1];
//	uint16_t lamp_pwm_hex;
//	uint8_t pwm_bcd[2];	
//	unsigned char lamp_switch_state[1];
//	
//	
//	uint8_t password[4] ={0x00,0x00,0x00,0x00};
//	uint8_t ctrl_code[4] ={0x78,0x56,0x34,0x12};
//	
//	memcpy(protocol_64507_valid_data,password,4);
//	memcpy(protocol_64507_valid_data+4,ctrl_code,4);
//	
//	DL_APP_BD_FRAME_T *p;
//	p = (DL_APP_BD_FRAME_T *)data;
//	
//	ctrl_mode[0] = (int)p->data[0];
//	
//	if((p->DT[0] == 0x03)&&(p->DT[1] == 0xB4))
//	{
//		for(i = 0;i<16;i++)
//		{
//			if(ctrl_mode[0] == i)
//			{
//				
//				
//				lamp_pwm_hex = (char)lamp_mode[i];
//				LOG(LOG_LVL_ERROR,"lamp_pwm_hex = %d\n",lamp_pwm_hex);
//				if(lamp_pwm_hex == 0xff)
//				{
//					return ;
//				}
//				if(lamp_pwm_hex != 0)
//				{
//					lamp_switch_state[0] = 0x1B;
//				}
//				else 
//				{
//					lamp_switch_state[0] = 0x1A;
//				}
//					
//			}
//		}
//		if (((lamp_switch_state[0] == 0x1A) && (lamp_pwm_hex<= 100)) || (lamp_switch_state[0] == 0x1B)) 
//		{
//			lamp_pwm_hex = (int)lamp_pwm_hex*10;
//			pwm_bcd[0] = hex2BCD(lamp_pwm_hex % 100);
//			pwm_bcd[1] = hex2BCD(lamp_pwm_hex / 100);
//			
//			memcpy(protocol_64507_valid_data+8,lamp_switch_state,1);
//			memcpy(protocol_64507_valid_data+9,pwm_bcd,2);			
//			protocol_64507_no_di_pack(meter_addr, 0x1C, 0x0B,  protocol_64507_valid_data, 23, protocol_64507_data_no_di);
//			jmesh_uart_send(JMESH_UART_0,sizeof(protocol_64507_data_no_di),protocol_64507_data_no_di);
//			APP_BD_protocol_handler_callback(p->type,p->addr,p->ctr,p->DT[0],p->DT[1],NULL,0,rev_type);
//		}	
//	}
//	else
//		return;
//}

#endif


