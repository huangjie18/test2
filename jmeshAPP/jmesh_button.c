/********************************************************************************************
��    �ƣ��������
��    �ܣ�����һ��50ms �Ķ�ʱ��ɨ�谴�����Լ��İ���״̬�����д���
��    �ߣ�LJJ
����ʱ�䣺20190717
�޸�ʱ�䣺
��    ע��
**************************************************************************************************/
#include "jmesh_button.h"

#if (defined Three_Remote_Switch)||(defined Ble_Remote_Switch)||(defined Scene_Panel)

unsigned char trg,cont;
uint8_t button_mode_state;


os_timer_event_t button_scan_timer;
Button_Config button[4]={{22,7},{21,9},{3,11},{20,10}};				//�ֳ���װ�İ���˳��

//Button_Config button[4]={{3,11},{20,10},{21,9},{22,7}};			//PCB�������� K1{3,11}   K2{20,10}   K3{21,9}   K4{22,7} 

void led_on(unsigned char led)
{
	io_pin_set(led);
}

void led_off(unsigned char led)
{	
	io_pin_clear(led);
}

void buttom_init(void)
{
	unsigned char i;
	for(i=0;i<4;i++)
	{
		jmesh_gpio_mode_in_pu(button[i].pin);
		io_pin_set(button[i].led);
		jmesh_gpio_mode_out_pp(button[i].led);
	}
	
	#ifdef Ble_Remote_Switch
	button_led_state(remote_mode);
	#endif
	
	#ifdef Scene_Panel
	button_led_init_start();
	#endif
	
}

void button_led_state(unsigned char led_state)
{
	unsigned char button_mode;
	button_mode = ~led_state;
	io_pin_write(button[0].led,button_mode&0x1);
	io_pin_write(button[1].led,(button_mode>>1)&0x1);
	io_pin_write(button[2].led,(button_mode>>2)&0x1);
	io_pin_write(button[3].led,(button_mode>>3)&0x1);
	
}

unsigned char button_state_read(void)
{
	unsigned char state = 0xff;
	state = (state<<1)|io_pin_read(button[3].pin);
	state = (state<<1)|io_pin_read(button[2].pin);
	state = (state<<1)|io_pin_read(button[1].pin);
	state = (state<<1)|io_pin_read(button[0].pin);
	return state;
	
}

#define TIMES  5
#define SACN_PERIOD 15

void button_scan_handle(void)
{
	unsigned char tmp =0x00;
	static uchar ii;						//ȥ����������
	static uint Curr = 0, Last;				//��ǰ���ϴμ�ֵ�ݴ�
	
	os_timer_event_restart(&button_scan_timer);	
	
	if (ii == 0)
		Last = Curr;						//���ϴμ�ֵ
	Curr = button_state_read();				//ȡ��ǰ��ֵ
	if (Curr^Last) 							//ǰ�����μ�ֵ��һ��,��������ȥ���ж�
	{ 
		ii = 0; 
		return;
	}		
	else 									//ǰ�����μ�ֵ��ͬ
	{
		if ((++ii) < TIMES) 			//ȥ�������ۼ�δ���趨ֵ,�����ٴ�ȥ���ж�
			return;
	}
	ii = 0;									//�趨��ȥ��������,��ֵ�ȶ�,���ۼ�����,������״̬����
	tmp = Curr^0xff;
	trg = tmp&(tmp^cont);
	cont = tmp;	
}	

//void button_scan_handle(void)
//{
//	unsigned char tmp;
//	unsigned char state = 0xff;

//	state = (state<<1)|io_pin_read(button[3].pin);
//	state = (state<<1)|io_pin_read(button[2].pin);
//	state = (state<<1)|io_pin_read(button[1].pin);
//	state = (state<<1)|io_pin_read(button[0].pin);
//				
//	tmp = state^0xff;
//	trg = tmp&(tmp^cont);
//	cont = tmp;

//	os_timer_event_restart(&button_scan_timer);
//}

void jmesh_button_scan_task_start(void)
{
	os_timer_event_set(&button_scan_timer,SACN_PERIOD, (os_timer_event_caller_t)button_scan_handle,NULL);
}

#endif

