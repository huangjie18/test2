/********************************************************************************************
名    称：场景面板
功    能：定义一个50ms 的定时器扫描按键，对检测的按键状态进行判处理
作    者：LJJ
创建时间：20190717
修改时间：
备    注：
**************************************************************************************************/
#include "jmesh_button.h"

#if (defined Three_Remote_Switch)||(defined Ble_Remote_Switch)||(defined Scene_Panel)

unsigned char trg,cont;
uint8_t button_mode_state;


os_timer_event_t button_scan_timer;
Button_Config button[4]={{22,7},{21,9},{3,11},{20,10}};				//现场安装的按键顺序

//Button_Config button[4]={{3,11},{20,10},{21,9},{22,7}};			//PCB按键布局 K1{3,11}   K2{20,10}   K3{21,9}   K4{22,7} 

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
	static uchar ii;						//去抖次数计数
	static uint Curr = 0, Last;				//当前及上次键值暂存
	
	os_timer_event_restart(&button_scan_timer);	
	
	if (ii == 0)
		Last = Curr;						//存上次键值
	Curr = button_state_read();				//取当前键值
	if (Curr^Last) 							//前后两次键值不一致,返回重新去抖判断
	{ 
		ii = 0; 
		return;
	}		
	else 									//前后两次键值相同
	{
		if ((++ii) < TIMES) 			//去抖次数累加未到设定值,返回再次去抖判断
			return;
	}
	ii = 0;									//设定的去抖次数内,键值稳定,则累加清零,并更新状态参数
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

