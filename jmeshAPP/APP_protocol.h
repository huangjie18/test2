#ifndef APP_PROTOCOL_H
#define APP_PROTOCOL_H


#include "mx_datatype.h"
#include"jmesh_app_pro.h"
#include"protocol_645.h"
#include "arch.h"  
#include"../jmesh_uart.h"
#include"jmesh_route.h"
#include"jmesh_state.h"
#include"jmesh_addr.h"
#include"../../onchip_system/os_timer_event.h"
#include"../../access/jmesh_access.h"
#include"../../driver/jmesh_system.h"
//#include"realcomm_client_callback.h"
//add 20190601
#include"app_expand2.h"
#include"app_expand2_realcomm_client.h"
#include"app_expand2_realcomm_server.h"

#include"app_expand.h"
#include"jmesh_netkey.h"
#include"../jmesh_ble.h"
#include"../../jmesh/jmesh_save_define.h"				    //add by ljj 20190410
#include"../../jmesh/jmesh_save.h"							//add by ljj 20190410
#include "jmesh_app_svc.h"
#include"jmesh_gatt.h"
#include"jmesh_proxy.h"
#include "beidian_app_cfg.h"

#include "jmesh_lamp.h"
#include "jmesh_scene.h"

/*设备类型：DeviceType 
终端模块--0x01   	燃气报警器--0x02    	    开窗器--0x03          机械手--0x04    
电表--0x05		 	墙挂式插座表--0x06   	    移动式插座表--0x07	  分体式空调控制器--0x08	
导轨表--0x09		单灯控制器--0x0A		    水表--0x0B			  中央空调器--0x0C
四路灯控--0x0D		三路灯控--0x0E			    情景面板--0x0F		  智能窗--0x10
*/


#ifdef Central_Air_Ctrl
	#define DeviceType 0x0C															//设备类型
	#define SET_JMESH_UART_PARITY_EVEN												//偶校验
	#define BLE_TX_Power_8dbm 														//蓝牙发射功率
	#define APP_DIS_ADV_DATA                ("Central_Air")			                //广播名称
	#define GET_ADDR_FROM_METER														//自动读地址
#endif

#ifdef Split_Air_Ctrl
	#define DeviceType 0x08															//设备类型
	#define SET_JMESH_UART_PARITY_EVEN												//偶校验
	#define BLE_TX_Power_0dbm 														//蓝牙发射功率
	#define APP_DIS_ADV_DATA                ("Split_Air")			                //广播名称
	#define GET_ADDR_FROM_METER														//自动读地址
#endif

#ifdef Wall_Socket_Gauge
	#define DeviceType 0x06			
	#define SET_JMESH_UART_PARITY_EVEN
	#define BLE_TX_Power_8dbm 
	#define APP_DIS_ADV_DATA                ("Wall_Socket")
	#define GET_ADDR_FROM_METER	
#endif

#ifdef Four_Lamp_Ctrl
	#define DeviceType 0x0D	
	#define SET_JMESH_UART_PARITY_EVEN 
	#define BLE_TX_Power_8dbm 
	#define APP_DIS_ADV_DATA                ("Four_Lamp")
	#define GET_ADDR_FROM_METER
#endif

#ifdef Three_Lamp_Ctrl
	#define DeviceType 0x0E	
	#define SET_JMESH_UART_PARITY_EVEN 
	#define BLE_TX_Power_8dbm 
	#define APP_DIS_ADV_DATA                ("Three_Lamp")
	#define GET_ADDR_FROM_METER
#endif

#ifdef Scene_Ctrl
	#define DeviceType 0x0F
	#define SET_JMESH_UART_PARITY_EVEN
	#define BLE_TX_Power_8dbm 
	#define APP_DIS_ADV_DATA                ("Scene_Ctrl")
	#define GET_ADDR_FROM_METER
	#define Scene_Control_Enable 1				                                   //情景面板控制功能	
#endif

#ifdef Scene_Panel
	#define DeviceType 0x0F
	#define SET_JMESH_UART_PARITY_EVEN
	#define BLE_TX_Power_0dbm 
	#define APP_DIS_ADV_DATA                ("Scene_Panel")
#endif

#ifdef Single_Lamp_Ctrl
	#define DeviceType 0x0A
	#define SET_JMESH_UART_PARITY_EVEN
	#define BLE_TX_Power_0dbm 	
	#define APP_DIS_ADV_DATA                ("Single_Lamp")
	#define GET_ADDR_FROM_METER
	#define Lamp_Control_Enable 1					                               //单灯灯控需要通过IO口来控制开关灯，0表示非单灯灯控设备，不需要此功能，1表示单灯灯控开启此功能
#endif

#ifdef LAMP_PASSTHROUGH
	#define DeviceType 0x0A
	#define SET_JMESH_UART_PARITY_EVEN
	#define BLE_TX_Power_0dbm 	
	#define APP_DIS_ADV_DATA                ("Lamp_Passthrough")
	#define GET_ADDR_FROM_METER
#endif

#ifdef Meter_Test
	#define DeviceType 0x05	
	#define SET_JMESH_UART_PARITY_EVEN
	#define BLE_TX_Power_0dbm 
	#define APP_DIS_ADV_DATA                ("Meter_BX2400")
	#define GET_ADDR_FROM_METER
#endif


#ifdef KEHUA_GAS
	#define DeviceType 0x02	
	#define SET_JMESH_UART_PARITY_NONE
	#define BLE_TX_Power_0dbm 
	#define APP_DIS_ADV_DATA                ("Gas_RF04")
#endif

#ifdef KEHUA_WINDOW
	#define DeviceType 0x10	
	#define SET_JMESH_UART_PARITY_NONE
	#define BLE_TX_Power_0dbm 
	#define APP_DIS_ADV_DATA                ("Window_RF04")
#endif

#ifdef KEHUA_TRANSFER_ARM
		#define DeviceType 0x04	
		#define SET_JMESH_UART_PARITY_NONE
		#define BLE_TX_Power_8dbm 
		#define APP_DIS_ADV_DATA            ("ARM_BX2400")
#endif

#ifdef KEHUA_ARM_BX2400_470
	#define DeviceType 0x04	
	#define SET_JMESH_UART_PARITY_NONE
	#define BLE_TX_Power_8dbm 
	#define APP_DIS_ADV_DATA                ("ARM_BX2400_470")
#endif

#ifdef KEHUA_ARM_RF04_470
	#define DeviceType 0x04	
	#define SET_JMESH_UART_PARITY_NONE
	#define BLE_TX_Power_8dbm 
	#define APP_DIS_ADV_DATA                ("ARM_RF04_470")
#endif


//测试使用设备类型为0x0F，正式版本使用0x13
#ifdef Three_Remote_Switch
	#define DeviceType 0x13
	//#define DeviceType 0x0F
	#define SET_JMESH_UART_PARITY_EVEN
	#define BLE_TX_Power_8dbm 
	#define APP_DIS_ADV_DATA                ("Three_Remote_Switch")
	#define GET_ADDR_FROM_METER
#endif

#ifdef Four_Remote_Switch
	#define DeviceType 0x13
	#define SET_JMESH_UART_PARITY_EVEN
	#define BLE_TX_Power_8dbm 
	#define APP_DIS_ADV_DATA                ("Four_Remote_Switch")
	#define GET_ADDR_FROM_METER
#endif
#ifdef Ble_Remote_Switch
	#define DeviceType 0x13
	#define SET_JMESH_UART_PARITY_EVEN
	#define BLE_TX_Power_0dbm 
	#define APP_DIS_ADV_DATA                "Ble_Remote"
//	#define GET_ADDR_FROM_METER
#endif

#ifdef Control_Switch
	#define DeviceType 0x15
	#define SET_JMESH_UART_PARITY_EVEN
	#define BLE_TX_Power_8dbm 	
	#define APP_DIS_ADV_DATA                ("Control_Switch")
	#define GET_ADDR_FROM_METER
	#define Lamp_Control_Enable 1					                               //单灯灯控需要通过IO口来控制开关灯，0表示非单灯灯控设备，不需要此功能，1表示单灯灯控开启此功能
#endif


#define APP_CONN_ADVINTERVAL_MIN_MS 5000
#define APP_CONN_ADVINTERVAL_MAX_MS 8000

#define BD_CommunicationTimeout                 0    // 子节点应答超时 
#define BD_SlaveAddrError                       1    // 子节点不存在 
#define BD_SlaveNotOnNet                        2    // 子节点没入网 
#define BD_InvalidData                          3    // 无效数据单元 
#define BD_InfInexistence                       4    // 信息类不存在 
#define BD_DevTypeError                         5    // 设备类型不一致 
#define BD_CtrError                             6    // 控制码不存在 
#define BD_Busy                                 7    // 控制码不存在 

typedef __packed struct DL_APP_BD_FRAME
{
	UINT8 start;					//0xBD
	UINT8 type;						//设备类型
	UINT8 addr[6];				//地址域
	UINT8 ctr;						//控制码
	UINT16 load_len;				//长度
	UINT8 DT[2];					//标识符
	UINT8 data[1];				//数据域
}DL_APP_BD_FRAME_T;


typedef struct 
{
	UINT8 type;						//设备类型
	UINT8 addr[6];				//地址域
	UINT8 ctr; 
	UINT8 DT[2];
	UINT8 rev_type;			
}APP_local_read_meter_info_t;

typedef struct 
{
	UINT8 repeat_num;
	UINT8 type;						//设备类型
	UINT8 addr[6];				//地址域
	UINT8 ctr; 
	UINT8 DT[2];
	UINT8 rev_type;
	UINT16 mesh_addr;
	UINT8 slave_addr[6];				
	UINT8 data_len;
	UINT8 data[200];
}APP_read_meter_info_t;

extern unsigned char jmesh_log_type; //mesh内部log信息打印
extern unsigned char  APP_User_Account[12];				//add by ljj 20190410
extern unsigned char app_rev_data_buff[256];
extern unsigned char app_send_data_buff[256];
extern APP_local_read_meter_info_t app_local_read_meter;
extern os_timer_event_t set_mac_callback_timer;
extern unsigned char app_read_meter_by_node_flag;
extern os_timer_event_t app_read_meter_by_node_uart_timer;

void set_mac_callback_timer_handler(void);
void JMESH_DEBUG_SEND_TO_CKQ(unsigned short length, unsigned char *data);
BOOL APP_BD_protocol_handler(uint8 *data,uint16 len,uint8 rev_type);
BOOL APP_BD_protocol_CheckFrame(uint8 *data,uint16 len);
unsigned char jmesh_app_recv_handler(unsigned char id,unsigned short instance,unsigned short length,unsigned char* data);
void app_conn_adv_start(void);
void app_transmission_uart_callback_handler(unsigned short len,unsigned char* data);
void app_transmission_uart_overtime_handler(void);
int jmesh_app_data_recv_handler(unsigned char *data);
BOOL APP_BD_protocol_handler_callback(uint8 type,uint8 *addr,uint8 ctr,uint8 DI0,uint8 DI1,uint8 *data,uint16 len,uint8 rev_type);
void app_read_meter_by_node_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void app_read_meter_by_node_return_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data);
void app_read_meter_by_node_uart_handler(unsigned short len,unsigned char* data);

#endif
