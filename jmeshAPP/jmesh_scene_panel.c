#include "jmesh_scene_panel.h"

#ifdef Scene_Panel

#define BUTTON_ENABLE 0xFF
#define BUTTON_DISABLE 0xEE


unsigned char scene_mode;
unsigned char button_state_set_buff[25];
unsigned char button_ctr_state,button_ctr_num,button_ctr_mode;
unsigned char ButtonInfo[20];
unsigned char button_num,switch_mode = 0;

static const unsigned char ButtonInfoInit[20] = {0xFF,0x01,0x00,0x00,0xFF,0x02,0x00,0x00,0xFF,0x03,0x00,0x00,0xFF,0x04,0x00,0x00,0xFF,0x0F,0x00,0x00};	
os_timer_event_t button_state_set_timer;
os_timer_event_t button_mode_set_timer;
os_timer_event_t button_led_init_timer;

	
void button_led_init_handle(void)
{
	button_led_state(scene_mode);
}	
	
void button_led_init_start(void)
{
	button_led_state(0x00);
	os_timer_event_set(&button_led_init_timer,1000,(os_timer_event_caller_t)button_led_init_handle,NULL);
}


	
/***********************************************************************************************
��    �ƣ�jmesh_scene_panel_BD_protocol_pakage_send(unsigned char* data, unsigned short len)
��    �ܣ�������Ϣͨ��BDЭ���ϱ������ڵ�
��    �룺num��������ţ� mode������ģʽ�������߹�
��    �أ� return 0����1
��    �ڣ�20190725
************************************************************************************************/
void jmesh_scene_panel_BD_protocol_pakage_send(unsigned char num,unsigned char mode)
{
	DL_APP_BD_FRAME_T *p;
	unsigned char report_buff[50],report_len;
	
	unsigned char key_num,switch_mode;
	key_num = num;
	switch_mode = mode;
	
	if((num==0)&&(mode==0))
	{
		return ;
	}
	
	p = (DL_APP_BD_FRAME_T *)report_buff;
	p->start = 0xBD;
	p->type = 0x0F;
	memcpy(p->addr,meter_addr,6);
	p->ctr = 0x04;
	p->load_len = 7+2;//6+2;
	p->DT[0] = 0x01;
	p->DT[1] = 0xCF;
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

/***********************************************************************************************
��    �ƣ�Panel_Memeory_Init(void)
��    �ܣ���ʼ������ң�ص����ò�����Ĭ��ֵ������0x0F���أ�0x00
��    �룺��
��    �أ���
��    �ڣ�20190823
************************************************************************************************/	
void Panel_Memeory_Init(void)
{	
	if(0 == jmesh_save_read(JMESH_SCENE_PANEL_INFO,20,ButtonInfo))
	{
		memcpy(ButtonInfo,ButtonInfoInit,20);	
	}
}

/***********************************************************************************************
��    �ƣ�Panel_factory_reset(void)
��    �ܣ��ָ����������ã�ɾ��flash����洢�İ���������Ϣ��Ȼ��ϵͳ��λ�����¶�ȡĬ�ϲ���
��    �룺��
��    �أ���
��    �ڣ�20190823
************************************************************************************************/
void Panel_factory_reset(void)
{
	jmesh_save_delete(JMESH_SCENE_PANEL_INFO);
	delay_ms(200);
	system_reset();
}	

/***********************************************************************************************
��    �ƣ�void APP_BD_CF00_handler(uint8 *data,uint16 len,uint8 rev_type)
��    �ܣ�����ң��ģʽ����
��    �룺data�����л��ߴ������ݣ�rev_type�����յ����� ��������470
��    �أ���
��    �ڣ�20190823
************************************************************************************************/
void APP_BD_CF00_handler(uint8 *data,uint16 len,uint8 rev_type)
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
		jmesh_save_write(JMESH_SCENE_PANEL_INFO,20,ButtonInfo);
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);
	}

}

/***********************************************************************************************
��    �ƣ�void APP_BD_CF02_handler(uint8 *data,uint16 len,uint8 rev_type)
��    �ܣ���ȡң�ز��������õĿ���ģʽ��������ǰ״̬
��    �룺data�����л��ߴ������ݣ�rev_type�����յ����� ��������470
��    �أ���
��    �ڣ�20190823
************************************************************************************************/
void APP_BD_CF02_handler(uint8 *data,uint16 len,uint8 rev_type)
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
��    �ƣ�void APP_BD_CF03_handler(uint8 *data,uint16 len,uint8 rev_type)
��    �ܣ���̨���߳��������Ƶƣ�λģʽ����modeģʽ
��    �룺data�����л��ߴ������ݣ�rev_type�����յ����� ��������470
��    �أ���
��    �ڣ�20191101
************************************************************************************************/
void APP_BD_CF03_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;				
	uint8 ctr,error;
	UINT16 i,j;
	
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
			button_ctr_num = p->data[0];          //���Ƶİ���
			button_ctr_mode = p->data[1];         //����ģʽ  1Ϊλģʽ��2Ϊmodeģʽ
			button_ctr_state = p->data[2];        //���ư����ǿ����ǹ�
						
			for(i = 0;i < 4;i++)
			{
				if(((button_ctr_num>>i)&0x01) == 0x01)
				{
					ButtonInfo[4*i] = 0xFF;
					ButtonInfo[4*i+3] = 0xFF;
					if(((button_ctr_state>>i)&0x01) == 0x01)
					{
						ButtonInfo[4*i+3] = ButtonInfo[4*i+1];
						scene_mode = ~(0x01<<i);
						button_led_state(scene_mode);
					}					
				}
				else ButtonInfo[4*i] = 0xEE;								
			}															
		}
		else
		{
			for(j = 0;j < 4;j++)			
			ButtonInfo[4*j] = 0xEE;	
		}			
		jmesh_save_write(JMESH_SAVE_SCENE_MODE_INFO,1,(unsigned char*)&scene_mode);
		jmesh_save_write(JMESH_SCENE_PANEL_INFO,20,ButtonInfo);
		
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);
	}
}

static unsigned char button_flag,button_num_buff,button_mode_buff;

os_timer_event_t button_state_buff_send_timer;
os_timer_event_t button_flag_clear_timer;


void button_state_buff_send_handle(void)
{
	jmesh_scene_panel_BD_protocol_pakage_send(button_num_buff,button_mode_buff);
}

/***********************************************************************************************
��    �ƣ�void button_state_buff_send_timer_statr(void)
��    �ܣ���ʱ1.5s���Ͱ���״̬��������
��    �룺
��    �أ���
��    �ڣ�20190911
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
��    �ƣ�void button_flag_clear_timer_statr(void)
��    �ܣ���ʱ2s���������־λ
��    �룺
��    �أ���
��    �ڣ�20190911
************************************************************************************************/
void button_flag_clear_timer_statr(void)
{
	os_timer_event_set(&button_flag_clear_timer,2000,(os_timer_event_caller_t)button_flag_clear_timer_handle,NULL);
}

/***********************************************************************************************
��    �ƣ�void button_data_send_handle(unsigned char num,unsigned char mode)
��    �ܣ�2s����������ͬһ������ʱ��������һ�ΰ���״̬�͵�һ����ͬ��ֻ���͵�һ��״̬��
          �����͵�һ�κ����һ�ε�״̬��2s�ڰ��²�ͬ�İ���ʱ��������
��    �룺
��    �أ���
��    �ڣ�20190911
************************************************************************************************/
void button_data_send_handle(unsigned char num,unsigned char mode)
{	
	static unsigned char last_mode;
	static unsigned char last_num;
	
	if(mode != last_mode)
	{
		button_flag = 1;
		last_num = num;	
		last_mode = mode;
		jmesh_scene_panel_BD_protocol_pakage_send(last_num,last_mode);
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
			jmesh_scene_panel_BD_protocol_pakage_send(num,mode);
			os_timer_event_restart(&button_flag_clear_timer);
		}
	}	
}


void scene_panel_button_handle(uint8_t button_state,uint8_t times)
{
	unsigned char i = 5,j = 0;
	static unsigned char new_button_state,old_button_state;

	new_button_state = button_state;
	
	if(old_button_state != new_button_state)
	{	
		switch(new_button_state)
		{
			case 0x01:  i = 0;	break;				
			case 0x02:	i = 1;	break;				
			case 0x04:	i = 2;	break;				
			case 0x08:	i = 3;	break;				
		}
		if(i != 5)
		{
			button_num = ButtonInfo[4*i];
			if(button_num != BUTTON_DISABLE)
			{
				for(j = 0;j<4;j++)
				{
					ButtonInfo[4*j+3] = 0xFF;
				}
				switch_mode = ButtonInfo[4*i+1];
				ButtonInfo[4*i+3] = switch_mode;
				scene_mode = ~(0x01<<i);
				button_data_send_handle(button_num,switch_mode);
				button_led_state(scene_mode);
			}
			else
				;
		}
		else
			;
		jmesh_save_write(JMESH_SAVE_SCENE_MODE_INFO,1,(unsigned char*)&scene_mode);
		jmesh_save_write(JMESH_SCENE_PANEL_INFO,20,ButtonInfo);
		old_button_state = new_button_state;
	}
	else
		;
}

#endif
















