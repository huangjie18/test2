#include "APP_protocol.h"
#include "_470_phy.h"
#include "kehua_gas.h"
#include "beidian_app_cfg.h"
#include "app_config.h"
#include "kehua_window.h"
#include "kehua_transfer_arm.h"
#include "jmesh_remote_control.h"
#include "sm4.h"

#define ENCRYPT_ENABLE    0

const UINT8 app_output_version[9] = {
		'X','M',                   		// 厂商代码
	 	'1','0',                 		// 芯片标识
    0x21,0x11,0x19,          // 日期

    0x01,0x04                // 版本
};

os_timer_event_t app_conn_adv_timer;		//广播发送定时器
os_timer_event_t app_transmission_uart_timer;
os_timer_event_t set_mac_callback_timer;
APP_local_read_meter_info_t app_local_read_meter;
unsigned char  APP_User_Account[12];							//add by ljj 20190410
unsigned char  app_rev_data_buff[256];
unsigned short app_rev_data_len;
unsigned char  app_send_data_buff[256];
unsigned short app_send_data_len;
unsigned char jmesh_log_type; //mesh内部log信息打印

APP_read_meter_info_t APP_read_meter_by_node_info;
os_timer_event_t app_read_meter_by_node_repeat_timer;
os_timer_event_t app_read_meter_by_node_uart_timer;
unsigned char app_read_meter_by_node_flag;	
unsigned short node_src_record;
mx188_frame_t mx188_frame_record_node = {0};

void JMESH_DEBUG_SEND_TO_CKQ(unsigned short length, unsigned char *data)
{
	gatt_phone_t * phone;
	phone = jmesh_app_get_phone();
	if(phone->id!=0xff)
		jmesh_proxy_send(JMESH_SEND_TYPE_CONNECT,phone->id,0xff,JMESH_PROXY_TYPE_CONFIG,length,data);
}

//APP与模块通讯协议判断
BOOL APP_BD_protocol_CheckFrame(uint8 *data,uint16 len)
{
		if(len == 0)
			return 0;
    if(data[0]==0xBD&&data[len-1]==0x16
       &&len==(((uint16)data[10]<<8)|data[9]+13)
       &&data[len-2]==check_sum(len-3,data+1))
		{
        return 1;
    }
    return 0;
}

extern unsigned char phone_connect_id ;

//APP与模块通讯协议组帧应答
BOOL APP_BD_protocol_handler_callback(uint8 type,uint8 *addr,uint8 ctr,uint8 DI0,uint8 DI1,uint8 *data,uint16 len,uint8 rev_type)
{
//	uint8 buff[250];
//	uint16 buff_len;
	memset(app_send_data_buff, 0, sizeof(app_send_data_buff));
	if((len + 15) > sizeof(app_send_data_buff))
		return 0;	
	DL_APP_BD_FRAME_T *p;
	p = (DL_APP_BD_FRAME_T *)app_send_data_buff;
	p->start = 0xBD;
	p->type = type;
	memcpy(p->addr,addr,6);
	p->ctr = ctr;
	p->load_len = len+2;
	p->DT[0] = DI0;
	p->DT[1] = DI1;
	memcpy(p->data,data,len);
	p->data[len] = check_sum(len+12,app_send_data_buff+1);
	p->data[len+1] = 0x16;
	app_send_data_len = len + 15;
	#if ENCRYPT_ENABLE
	{		
		uint8_t tmpdata[256];
		uint8_t netkey[16]; 
		if(JMESH_STATUS_SUCCESS != jmesh_netkey_state_get(0,netkey))
		{
			memset(netkey, 0, 16);
		}
		app_send_data_len = (((app_send_data_len - 1) - 1) / 16 + 1) * 16 + 1; 		
		memcpy(tmpdata, app_send_data_buff, app_send_data_len);
		sms4_ecb(tmpdata + 1, app_send_data_len - 1, netkey, app_send_data_buff + 1, SM_ENCRYPT);
	}
	#endif
	if(rev_type == 0)			//串口应答
	{
		jmesh_uart_send(JMESH_UART_0, app_send_data_len, app_send_data_buff);
	}
	else if(rev_type == 1)	 								//APP蓝牙通过应答
	{
		//jmesh_ble_gatt_notify(phone_connect_id,jmesh_ble_get_local_svc_ins()+JMESHS_IDX_MESH_APP_TRANMISSION_DATA_OUT_VAL,app_send_data_len,app_send_data_buff);
		gatt_phone_t * phone;
		//unsigned short mtu;
		phone = jmesh_app_get_phone();
		//mtu=jmesh_gatt_mtu_get_by_id(phone->id);
		//app_print_cmd("phone_id:%d   phone_mtu:%d",phone->id,phone->mtu);
		jmesh_proxy_send(JMESH_SEND_TYPE_CONNECT,phone->id,0xff,JMESH_PROXY_TYPE_APPDATA,app_send_data_len,app_send_data_buff);
	}
	else if(rev_type == 2)
	{
		phy_tx_pkt(app_send_data_buff, app_send_data_len, 1);
	}
	else if(rev_type == 3)
	{
		if((app_send_data_len + 2) > sizeof(app_send_data_buff))
			return 0;
		memmove(app_send_data_buff+2,app_send_data_buff,app_send_data_len);
		JMESH_BIG_ENDIAN_FILL2(expand_opcode_realcomm_app_set_meter_return,app_send_data_buff);
		//memcpy(buff+2,data,len);
		jmesh_access_send(realcomm_send_save.dst,realcomm_send_save.nid,realcomm_send_save.aid,&app_expand,message_app_expand2,app_send_data_len+2,app_send_data_buff,JMESH_SEND_ALL_CHANNEL);	
	}
	else if(rev_type == 4)
	{
		message_realcomm_server_data_return_handler(app_send_data_len,app_send_data_buff);
	}
	return 0;
}

//APP广播连接数据帧
unsigned char app_conn_adv_frame(uint8 type,uint8 *addr,uint8 *buff)
{
	DL_APP_BD_FRAME_T *p;
	p = (DL_APP_BD_FRAME_T *)buff;
	p->start = 0xBD;
	p->type = type;
	memcpy(p->addr,addr,6);
	p->ctr = 0x01;
	p->load_len = 2;
	p->DT[0] = 0xBB;
	p->DT[1] = 0xBB;
	p->data[0] = check_sum(12,buff+1);
	p->data[1] = 0x16;
	#if ENCRYPT_ENABLE
	{		
		uint8_t tmpdata[17];
		uint8_t netkey[16]; 
		if(JMESH_STATUS_SUCCESS != jmesh_netkey_state_get(0,netkey))
		{
			memset(netkey, 0, 16);
		}
		memset(tmpdata, 0, sizeof(tmpdata));		
		memcpy(tmpdata, buff, 15);
		sms4_ecb(tmpdata + 1, 16, netkey, buff + 1, SM_ENCRYPT);
	}
	return 17;
	#else
	return 15;
	#endif	
}

//发起APP连接广播
void app_conn_adv_link_require(void)
{
	uint8 buff[50];
	uint16 buff_len;
	os_timer_event_restart(&app_conn_adv_timer);
	//	if(!jmesh_adv_is_ready())
	//	{
	//		return ;
	//	}
	buff_len = app_conn_adv_frame(DeviceType,meter_addr,buff);
	//jmesh_ble_send_adv(ADV_MODE_CONN,0x2D,buff_len,buff);
	jmesh_adv_send(0xff,buff_len,buff,10);
}


void app_conn_adv_send(void){
    app_conn_adv_link_require();
    os_timer_event_set(&app_conn_adv_timer,
                       jmesh_random_get(APP_CONN_ADVINTERVAL_MIN_MS,APP_CONN_ADVINTERVAL_MAX_MS),
                       (os_timer_event_caller_t)app_conn_adv_send,
                       NULL);
}

void app_conn_adv_start(void)
{
	os_timer_event_set(&app_conn_adv_timer,APP_CONN_ADVINTERVAL_MIN_MS,(os_timer_event_caller_t)app_conn_adv_send,NULL);
}

// 手机抄表超时处理 
void app_transmission_uart_overtime_handler(void)
{
	uint8 error;
	app_local_read_meter_flag = 0;

	current_realcomm_task_id = NO_TASK;
	error = BD_CommunicationTimeout;
	app_local_read_meter.ctr = 0xC4;
	APP_BD_protocol_handler_callback(app_local_read_meter.type,
	app_local_read_meter.addr,
	app_local_read_meter.ctr,
	app_local_read_meter.DT[0],
	app_local_read_meter.DT[1],&error,1,app_local_read_meter.rev_type);
}

// 手机抄表数据返回处理 
void app_transmission_uart_callback_handler(unsigned short len,unsigned char* data)
{
	APP_BD_protocol_handler_callback(app_local_read_meter.type,
		app_local_read_meter.addr,
		app_local_read_meter.ctr,
		app_local_read_meter.DT[0],
		app_local_read_meter.DT[1],data,len,app_local_read_meter.rev_type);
	app_local_read_meter_flag = 0;
	current_realcomm_task_id = NO_TASK;
	os_timer_event_remove(&app_transmission_uart_timer);
}

//APP数据透传给串口
void APP_BD_B101_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;
	uint8 ctr;
	
	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
		ctr = 0x81;
	else if(p->ctr == 0x04)
		ctr = 0x84;
	
	#if(Lamp_Control_Enable==1)
	if(APP_BD_B101_Lamp_handler(data,len,rev_type) == 1)
		return ;
	#endif
	
	#if	(defined KEHUA_TRANSFER_ARM)||(defined KEHUA_ARM_BX2400_470)	
	if(APP_BD_B101_ARM_handler(data,len,rev_type) == 1)
		return ;
	#endif
	
	app_local_read_meter.type = p->type;
	app_local_read_meter.ctr = ctr;
	memcpy(app_local_read_meter.addr, p->addr,6);
	app_local_read_meter.DT[0] = p->DT[0];
	app_local_read_meter.DT[1] = p->DT[1];
	app_local_read_meter.rev_type = rev_type;
	
	read_meter_handler(p->load_len-2, p->data, 2);
	/*if(current_realcomm_task_id == NO_TASK)
	{
		jmesh_uart_send(JMESH_UART_0, p->load_len-2, p->data);
		current_realcomm_task_id = PHONE_DIRECT_READ_METER;
		os_timer_event_set(&app_transmission_uart_timer, METER_MAX_RESPONSE_MS,(os_timer_event_caller_t)app_transmission_uart_overtime_handler,NULL);
	}
	else
	{
		uint8 error;
		error = BD_Busy;
		APP_BD_protocol_handler_callback(app_local_read_meter.type,
		app_local_read_meter.addr,
		app_local_read_meter.ctr,
		app_local_read_meter.DT[0],
		app_local_read_meter.DT[1],&error,1,app_local_read_meter.rev_type);
	}*/			
}

//修改密钥
void APP_BD_B102_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;
	uint8 ctr,error;
	unsigned char netkey[16],appkey[16];
	
	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
		ctr = 0x81;
	else if(p->ctr == 0x04)
		ctr = 0x84;

	if(p->load_len-2 != 32)
	{
		if(p->ctr == 0x01)
			ctr = 0xC1;
		else if(p->ctr == 0x04)
			ctr = 0xC4;
		error = BD_InvalidData;
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
		return ;
	}
	
	jmesh_netkey_state_del(0);
	jmesh_appkey_delete(0,1);
	memcpy(netkey,&p->data[0],16);
	memcpy(appkey,&p->data[16],16);	
	jmesh_netkey_state_set(0,netkey);
	jmesh_appkey_set(0,1,appkey);
	jmesh_model_bind_app(&app_expand,jmesh_appkey_get_by_key_index(0,1));
	APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);
	
	//delay_ms(200);
	//system_reset();
}

void factory_data_reset(void)
{
	#if(defined Three_Remote_Switch)||(defined Ble_Remote_Switch)
	Remote_factory_reset();
	#endif
	#ifdef Scene_Panel
	Panel_factory_reset();
	#endif
	
	#ifdef LAMP_PASSTHROUGH
	Lamp_factory_reset();
	#endif

}

//恢复出厂设置
void APP_BD_B103_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;
	uint8 ctr,error;
	
	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
		ctr = 0x81;
	else if(p->ctr == 0x04)
		ctr = 0x84;

	if(p->load_len-2 != 0)
	{
		if(p->ctr == 0x01)
			ctr = 0xC1;
		else if(p->ctr == 0x04)
			ctr = 0xC4;
		error = BD_InvalidData;
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
		return ;
	}
	
	APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);
	factory_data_reset();

}

void set_mac_callback_timer_handler(void)
{
	system_reset();
}

//蓝牙MAC地址
void APP_BD_B104_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;
	uint8 ctr,error;
	unsigned char mac[6] = {0};
	unsigned char mac_old[6];
	
	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
		ctr = 0x81;
	else if(p->ctr == 0x04)
		ctr = 0x84;

	if(p->load_len-2 != 6)
	{
		if(p->ctr == 0x01)
			ctr = 0xC1;
		else if(p->ctr == 0x04)
			ctr = 0xC4;
		error = BD_InvalidData;
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
		return ;
	}
	
	memcpy(mac,p->data,6);
//	mac[0] = p->data[5];
//	mac[1] = p->data[4];
//	mac[2] = p->data[3];
//	mac[3] = p->data[2];
//	mac[4] = p->data[1];
//	mac[5] = p->data[0];
	jmesh_mac_get(mac_old);
	if(memcmp(mac_old,mac,6)==0)
	{
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);
		return ;
	}
	jmesh_mac_set(mac);
	APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);
	os_timer_event_set(&set_mac_callback_timer,500,(os_timer_event_caller_t)set_mac_callback_timer_handler,NULL);
}

//设备重启
void APP_BD_B105_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;
	uint8 ctr,error;
	
	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
		ctr = 0x81;
	else if(p->ctr == 0x04)
		ctr = 0x84;
	
	if(p->load_len-2 != 0)
	{
		if(p->ctr == 0x01)
			ctr = 0xC1;
		else if(p->ctr == 0x04)
			ctr = 0xC4;
		error = BD_InvalidData;
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
		return ;
	}
	
	APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);
	delay_ms(200);
	system_reset();
}

//设置MESH短地址
void APP_BD_B106_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;
	uint8 ctr,error;
	unsigned short mesh_addr = 0;
	
	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
		ctr = 0x81;
	else if(p->ctr == 0x04)
		ctr = 0x84;
	
	mesh_addr = p->data[0]|(((uint16)p->data[1])<<8);
	if((p->load_len-2 != 2)||(0==mesh_addr%2)||mesh_addr<3 || mesh_addr>JMESH_ROUTE_NODE_ADDR_LIMIT)
	{
		if(p->ctr == 0x01)
			ctr = 0xC1;
		else if(p->ctr == 0x04)
			ctr = 0xC4;
		error = BD_InvalidData;
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
		return ;
	}

	jmesh_set_primary_addr(mesh_addr);
	APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);

}


//设置管理开关
void APP_BD_B183_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;
	uint8 ctr,error;
	
	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
		ctr = 0x81;
	else if(p->ctr == 0x04)
		ctr = 0x84;
	
	if(p->load_len-2 != 0)
	{
		if(p->ctr == 0x01)
			ctr = 0xC1;
		else if(p->ctr == 0x04)
			ctr = 0xC4;
		error = BD_InvalidData;
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
		return ;
	}
	
	APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);
	OTA_Switch();

}

//设备信息
void APP_BD_B201_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;
	uint8 ctr,error;
	uint8 buff[50];						//uint8 buff[40]; add by ljj 20190410
	uint16 mesh_addr;
	unsigned char mac[6] = {0};
	
	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
		ctr = 0x81;
	else if(p->ctr == 0x04)
		ctr = 0x84;
	
	if(p->load_len-2 != 0)
	{
		if(p->ctr == 0x01)
			ctr = 0xC1;
		else if(p->ctr == 0x04)
			ctr = 0xC4;
		error = BD_InvalidData;
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
		return ;
	}
	
	memset(buff,0x00,50);							//memset(buff,0x00,40); add by ljj 20190410
	memcpy(buff,app_output_version,9);
	buff[9] = connect_state;
	mesh_addr = jmesh_get_primary_addr();
	memcpy(buff+10,(uint8 *)&mesh_addr,2);
	memcpy(buff+12,meter_addr,6);
	jmesh_mac_get(mac);
	memcpy(buff+18,mac,6);
	memcpy(buff+24,JZQ_communication_address,6);
	buff[30] = DeviceType;
	memcpy(buff+31,APP_User_Account,12);	//add by ljj 2019010
	
	APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],buff,43,rev_type);//APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],buff,31,rev_type);	add by ljj

}

//连接参数
void APP_BD_B202_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;					//add by ljj 20190410
	uint8 ctr,error;


	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
		ctr = 0x81;
	else if(p->ctr == 0x04)
		ctr = 0x84;
	
	if(p->load_len-2 != 18)
	{
		if(p->ctr == 0x01)
			ctr = 0xC1;
		else if(p->ctr == 0x04)
			ctr = 0xC4;
		error = BD_InvalidData;
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
		return ;
	}
	
	memcpy(JZQ_communication_address,&p->data[0],6);
	jmesh_save_write(JMESH_JZQ_ADDR_SAVE,6,JZQ_communication_address);
	memcpy(APP_User_Account,&p->data[6],12);
	jmesh_save_write(JMESH_SAVE_APP_USER_ACCOUNT,12,APP_User_Account);
		
	APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);			//end	add 
	if (wirless_tech.tech == TECH_BLE_470)
	{
		uint8_t channel_group = (JZQ_communication_address[0] / 0x10) * 10 + (JZQ_communication_address[0] % 0x10);
		channel_group = channel_group % CHANNEL_GROUP_NUM + 1;
		phy_set_channel_group(channel_group);                               		
		phy_set_channel(PRIVATE_CHANNEL, phy_pib.phyCurrentChannelIndex); 
	}
}

//双模通信参数
void APP_BD_B203_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;
	uint8_t ctr,error;
	wirless_tech_t tmp_wirless_tech;
	uint8 buff[2];

	
	p = (DL_APP_BD_FRAME_T *)data;
	ctr =  p->ctr | 0x80;
	if(p->ctr == 0x01) // read
	{
		memcpy(buff, &wirless_tech, sizeof(wirless_tech));
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],buff,sizeof(wirless_tech),rev_type);
	}
	else if(p->ctr == 0x04) // set
	{		
		memcpy(&tmp_wirless_tech,&p->data[0],1);
		if ((tmp_wirless_tech.tech >= TECH_NUM) || (p->load_len-2 != 1))
		{
			ctr |=  0x40;		
			error = BD_InvalidData;
			APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
			return ;
		}
		else
		{	 	
			wirless_tech = tmp_wirless_tech;
			jmesh_save_write(JMESH_SAVE_WIRLESS_TECH,1, (uint8_t *)&wirless_tech);
			APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);
		}
	}
}
	
	
//470通信信号测试 
void APP_BD_B204_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;
	uint8_t ctr;
	uint8 buff[1];
	
	p = (DL_APP_BD_FRAME_T *)data;
	ctr =  p->ctr | 0x80;
	 
	buff[0] = phy_rx_packet.rssi;
	APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],buff,sizeof(wirless_tech),rev_type);	
}

//发送广播命令，数据长度最大值为6
void APP_BD_B403_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;
	uint8 ctr;
//	uint8 error;
	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
		ctr = 0x81;
	else if(p->ctr == 0x04)
		ctr = 0x84;
	
//	if(p->load_len-2 != 7)
//	{
//		if(p->ctr == 0x01)
//			ctr = 0xC1;
//		else if(p->ctr == 0x04)
//			ctr = 0xC4;
//		error = BD_InvalidData;
//		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
//		return ;
//	}
	send_app_broadcast_data(6,p->data);
	APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);

}

void app_read_meter_by_node_uart_handler(unsigned short len,unsigned char* data)
{
	unsigned char buff[255];
	mx188_frame_t frame = {0};
	unsigned short data_len;
	
	os_timer_event_remove(&app_read_meter_by_node_uart_timer);
	app_read_meter_by_node_flag = 0;
	memcpy(&frame, &mx188_frame_record_node, sizeof(mx188_frame_t));	
	frame.CTR_10.direction = FROME_SLAVE_NODE;				
	data_len = protocol_mx188_pack(&frame, len, data, buff+2, sizeof(buff)-2);

	JMESH_BIG_ENDIAN_FILL2(expand_opcode_realcomm_app_read_meter_by_node_return,buff);
	jmesh_access_send(node_src_record,0,1,&app_expand,message_app_expand2,data_len+2,buff,JMESH_SEND_ALL_CHANNEL);

}

void app_read_meter_by_node_uart_timer_handler(void)
{
	app_read_meter_by_node_flag = 0;
}


//指定节点抄表处理
void app_read_meter_by_node_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
	mx188_frame_t frame = {0};
	
	if (!protocol_mx188_parse2(data, len, &frame))
		return;	
	
	node_src_record = src;
	app_read_meter_by_node_flag = 1;
	memcpy(&mx188_frame_record_node, &frame, sizeof(mx188_frame_t));
	#if(Lamp_Control_Enable==1)
	if(Lamp_Control_rev_data_by_node(data + frame.valid_data_offset,frame.valid_data_len)==1)
	{
		app_read_meter_by_node_flag = 0;
		return;
	}
	#endif
	jmesh_uart_send(JMESH_UART_0, frame.valid_data_len, data + frame.valid_data_offset);
	os_timer_event_set(&app_read_meter_by_node_uart_timer,2000,(os_timer_event_caller_t)app_read_meter_by_node_uart_timer_handler,NULL);
}

//启动指定节点抄表应答处理
void app_read_meter_by_node_return_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
	uint8 buff[250];	
	mx188_frame_t frame = {0};	
	
	memcpy(buff,(uint8 *)&APP_read_meter_by_node_info.mesh_addr,2);
	memcpy(buff+2,APP_read_meter_by_node_info.slave_addr,6);
	if (protocol_mx188_parse2(data, len, &frame))
	{							
		memcpy(buff+8,data+frame.valid_data_offset, frame.valid_data_len);
		APP_BD_protocol_handler_callback(APP_read_meter_by_node_info.type,APP_read_meter_by_node_info.addr,APP_read_meter_by_node_info.ctr,\
																		APP_read_meter_by_node_info.DT[0],APP_read_meter_by_node_info.DT[1],buff,frame.valid_data_len+8,\
																		APP_read_meter_by_node_info.rev_type);
	}
	else
	{
		memcpy(buff+8,data,len);
		APP_BD_protocol_handler_callback(APP_read_meter_by_node_info.type,APP_read_meter_by_node_info.addr,APP_read_meter_by_node_info.ctr,\
																		APP_read_meter_by_node_info.DT[0],APP_read_meter_by_node_info.DT[1],buff,len+8,\
																		APP_read_meter_by_node_info.rev_type);
	}
	os_timer_event_remove(&app_read_meter_by_node_repeat_timer);
}

//启动指定节点抄表重发处理
void app_read_meter_by_node_repeat_timer_handler(void* opcode)
{
	uint8 buff[250],error;	
	if(APP_read_meter_by_node_info.repeat_num)
	{
		JMESH_BIG_ENDIAN_FILL2(expand_opcode_realcomm_app_read_meter_by_node,buff);	
		memcpy(buff+2,&APP_read_meter_by_node_info.data,APP_read_meter_by_node_info.data_len);		
		jmesh_access_send(APP_read_meter_by_node_info.mesh_addr+1,0,1,&app_expand,message_app_expand2,APP_read_meter_by_node_info.data_len+2,\
											buff,JMESH_SEND_ALL_CHANNEL);
		APP_read_meter_by_node_info.repeat_num--;
		os_timer_event_restart(&app_read_meter_by_node_repeat_timer);
	}
	else
	{
		error = BD_CommunicationTimeout;
		APP_BD_protocol_handler_callback(APP_read_meter_by_node_info.type,APP_read_meter_by_node_info.addr,0xC1,\
																		APP_read_meter_by_node_info.DT[0],APP_read_meter_by_node_info.DT[1],&error,1,\
																		APP_read_meter_by_node_info.rev_type);
		os_timer_event_remove(&app_read_meter_by_node_repeat_timer);
	}
}

//启动指定节点抄表，通过本节点抄指定节点
void APP_BD_B406_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;
	uint8 i,ctr,addr[6],error;
	uint8 buff[250];
	uint16 mesh_addr;
	
	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
		ctr = 0x81;
	else if(p->ctr == 0x04)
		ctr = 0x84;

	
	memset(&APP_read_meter_by_node_info,0x00,sizeof(APP_read_meter_by_node_info));
	mesh_addr = p->data[0]|(((uint16)p->data[1])<<8);
	memcpy(addr,p->data+2,6);
	
	APP_read_meter_by_node_info.repeat_num = 2;
	APP_read_meter_by_node_info.type = p->type;
	APP_read_meter_by_node_info.ctr = ctr;
	memcpy(APP_read_meter_by_node_info.addr,p->addr,6);
	memcpy(APP_read_meter_by_node_info.slave_addr,addr,6);
	APP_read_meter_by_node_info.DT[0] = p->DT[0];
	APP_read_meter_by_node_info.DT[1] = p->DT[1];
	APP_read_meter_by_node_info.rev_type = rev_type;
	APP_read_meter_by_node_info.mesh_addr = mesh_addr;
	
	
	// 添加188协议 
	{
		mx188_frame_t frame = {0};
				
		frame.meter_type = 0x80; 
		memcpy(frame.addr.u8, &p->data[0], METER_ADDR_SIZE);		
		frame.CTR_20.repeat_index = 0;
		frame.CTR_20.repeat_level = 0;
		frame.CTR_10.function = 0x01;
		frame.CTR_10.respond_flag = 0;
		frame.CTR_10.direction = FROM_HOST_NODE;			
		frame.seq = get_new_mx188_seq();	
		frame.DI0 = 0x90;
		frame.DI1 = 0x25;
		
		APP_read_meter_by_node_info.data_len = protocol_mx188_pack(&frame, p->load_len-10, &p->data[8], APP_read_meter_by_node_info.data,sizeof(APP_read_meter_by_node_info.data));
	}
		
	JMESH_BIG_ENDIAN_FILL2(expand_opcode_realcomm_app_read_meter_by_node,buff);	
	memcpy(buff+2,&APP_read_meter_by_node_info.data,APP_read_meter_by_node_info.data_len);		
	jmesh_access_send(APP_read_meter_by_node_info.mesh_addr+1,0,1,&app_expand,message_app_expand2,APP_read_meter_by_node_info.data_len+2,buff,JMESH_SEND_ALL_CHANNEL);
	os_timer_event_set(&app_read_meter_by_node_repeat_timer,6000,(os_timer_event_caller_t)app_read_meter_by_node_repeat_timer_handler,NULL);
}


#ifdef  KEHUA_GAS 
//采集，上报周期 
void APP_BD_C201_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;				
	uint8 ctr,error;
	uint8 buff[2];
	UINT16 i;

	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
		ctr = 0x81;
	else if(p->ctr == 0x04)
		ctr = 0x84;
	
	if(p->ctr == 0x01)
	{
		buff[0] = collect_interval_s;
		buff[1] = report_interval_m;
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],buff,2,rev_type);
	}
	else
	{
		if(p->load_len-2 != 2)
		{
			if(p->ctr == 0x01)
				ctr = 0xC1;
			else if(p->ctr == 0x04)
				ctr = 0xC4;
			error = BD_InvalidData;
			APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
			return ;
		}
		
		if(p->data[0] > 1)
		{
			collect_interval_s = p->data[0];
			jmesh_save_write(JMESH_SAVE_COLLECT_INTERVAL,1,(unsigned char*)&collect_interval_s);
		}
		if(p->data[1] > 0)
		{
			report_interval_m = p->data[1];
			jmesh_save_write(JMESH_SAVE_REPORT_INTERVAL,1,(unsigned char*)&report_interval_m);
		}
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);
		kehua_gas_start();
	}
	
}
#endif

#ifdef  KEHUA_WINDOW 
//开窗器上报周期、次数
void APP_BD_D001_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;				
	uint8 ctr,error;
	uint8 buff[2];
	UINT16 i;

	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
		ctr = 0x81;
	else if(p->ctr == 0x04)
		ctr = 0x84;
	
	if(p->ctr == 0x01)
	{
		buff[0] = window_report_period;
		buff[1] = window_report_count;
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],buff,2,rev_type);
	}
	else
	{
		if(p->load_len-2 != 2)
		{
			if(p->ctr == 0x01)
				ctr = 0xC1;
			else if(p->ctr == 0x04)
				ctr = 0xC4;
			error = BD_InvalidData;
			APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
			return ;
		}
		
		if(p->data[0] > 1)
		{
			window_report_period = p->data[0];
			jmesh_save_write(JMESH_SAVE_REPORT_PERIOD,1,(unsigned char*)&window_report_period);
		}
		else
		{
			if(p->ctr == 0x01)
				ctr = 0xC1;
			else if(p->ctr == 0x04)
				ctr = 0xC4;
			error = BD_InvalidData;
			APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
			return ;
		}
		
		if(p->data[1] > 0)
		{
			window_report_count = p->data[1];
			jmesh_save_write(JMESH_SAVE_REPORT_COUNT,1,(unsigned char*)&window_report_count);
		}
		else
		{
			if(p->ctr == 0x01)
				ctr = 0xC1;
			else if(p->ctr == 0x04)
				ctr = 0xC4;
			error = BD_InvalidData;
			APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
			return ;
		}
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);
	}
	
}
#endif


// 复位次数
void APP_BD_BF01_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;
	uint8 ctr;
	unsigned char reset_cnt[2];
	
	p = (DL_APP_BD_FRAME_T *)data;
	ctr =  p->ctr | 0x80;		
	jmesh_save_read(JMESH_SAVE_RESET_COUNT, 2, reset_cnt);
	APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],reset_cnt,2,rev_type);
}

#if((Lamp_Control_Enable==1)||(defined LAMP_PASSTHROUGH))
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
#endif


// 470射频测试 
void APP_BD_BF02_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;
	uint8 ctr, error;
	uint8 send_or_recv, test_channel, test_time_s;
	
	p = (DL_APP_BD_FRAME_T *)data;
	ctr =  p->ctr | 0x80;		
	if(p->load_len-2 < 3)
	{
		ctr |= 0x40;
		error = BD_InvalidData;
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
		return ;
	}		
	send_or_recv = p->data[0];
	test_channel = p->data[1];
	test_time_s = p->data[2];	
	APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);	
	delay_ms(100);		
	if(send_or_recv == 1)
	{
		test_stream_data(4, test_channel, test_time_s, 0);
	}
	else
	{
		test_RF_ber(test_channel, test_time_s, 0);
	}
}

//mesh内部log信息打印
//jmesh_log_type:0 不打印
//jmesh_log_type:1 串口打印
//jmesh_log_type:2 抄控器打印
void APP_BD_BF03_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;
	uint8 ctr;
	//unsigned short mesh_addr = 0;
	
	p = (DL_APP_BD_FRAME_T *)data;
	if(p->ctr == 0x01)
	{
		ctr = 0x81;
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&jmesh_log_type,1,rev_type);
	}
	else if(p->ctr == 0x04)
	{
		ctr = 0x84;
		jmesh_log_type = p->data[0];
		jmesh_save_write(JMESH_LOG_TYPE_COUNT, 1, &jmesh_log_type);
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],NULL,0,rev_type);
	}
}

//APP与模块通讯协议处理
BOOL APP_BD_protocol_handler(uint8 *data,uint16 len,uint8 rev_type)
{
	DL_APP_BD_FRAME_T *p;
	uint8  ctr,type,error;
	uint16 DTn;
	
	#if ENCRYPT_ENABLE	
	uint8_t tmpdata[len];
	uint8_t netkey[16]; 
	if(JMESH_STATUS_SUCCESS != jmesh_netkey_state_get(0,netkey))
	{
		memset(netkey, 0, 16);
	}
	memcpy(tmpdata, data, len);
	if ((len - 1) % 16 == 0) {
		sms4_ecb(tmpdata + 1, len - 1, netkey, data + 1, SM_DECRYPT);
	}
	else {
		return 0;
	}
	for(; len > 1; len--) {
		if(data[len - 1] != 0) {
			break;
		}
	}
	#endif
	
	if(APP_BD_protocol_CheckFrame(data,len) == 0)
		return 2;
	p = (DL_APP_BD_FRAME_T *)data;
	type = p->type;
	ctr = p->ctr;
	DTn = p->DT[0]|(((uint16)p->DT[1])<<8);
	
	if(rev_type == 0x02) // 来自470,要判断地址，因为是广播的 
	{
		if(memcmp(p->addr, meter_addr, 6) != 0)
		{
			return 0;
		}
	}	
	
	if(ctr!=0x01&&ctr!=0x04)
	{
		error = BD_CtrError;
		//APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
		return 0;
	}
	
	if(type != DeviceType)
	{
		if(p->ctr == 0x01)
			ctr = 0xC1;
		else if(p->ctr == 0x04)
			ctr = 0xC4;
		error = BD_DevTypeError;
		APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
		return 0;
	}
	
	switch(DTn)
	{
		case 0xB101:
			APP_BD_B101_handler(data,len,rev_type);
			break;
		
		case 0xB102:
			APP_BD_B102_handler(data,len,rev_type);
			break;
		
		case 0xB103:
			APP_BD_B103_handler(data,len,rev_type);
			break;
		
		case 0xB104:
			APP_BD_B104_handler(data,len,rev_type);
			break;
		
		case 0xB105:
			APP_BD_B105_handler(data,len,rev_type);
			break;
		
		case 0xB106:
			APP_BD_B106_handler(data,len,rev_type);
			break;
		
		case 0xB183:
			APP_BD_B183_handler(data,len,rev_type);
			break;
				
		case 0xB201:
			APP_BD_B201_handler(data,len,rev_type);
			break;
		
		case 0xB202:
			APP_BD_B202_handler(data,len,rev_type);
			break;
		
		case 0xB203:
			APP_BD_B203_handler(data,len,rev_type);
			break;
		
		case 0xB204:
			APP_BD_B204_handler(data,len,rev_type);
			break;
		
		case 0xB403: 
			APP_BD_B403_handler(data,len,rev_type);
			break;
		case 0xB406:
			APP_BD_B406_handler(data,len,rev_type);
			break;
		
		case 0xBF01:
			APP_BD_BF01_handler(data,len,rev_type);
			break;
		
		case 0xBF02: 
			APP_BD_BF02_handler(data,len,rev_type);
			break;
		
		case 0xBF03: 
			APP_BD_BF03_handler(data,len,rev_type);
			break;
		
		#ifdef  KEHUA_GAS 
		case 0xC201:
			APP_BD_C201_handler(data,len,rev_type);
			break;
		#endif
		#if((Lamp_Control_Enable==1)||(defined LAMP_PASSTHROUGH))
		case 0xCA01: 
			APP_BD_CA01_handler(data,len,rev_type);
			break;
		#endif
		#ifdef  KEHUA_WINDOW 
		case 0xD001:
			APP_BD_D001_handler(data,len,rev_type);
			break;
		#endif
		
		#if(defined Three_Remote_Switch)||(defined Ble_Remote_Switch)
		case 0xD300:
			APP_BD_D300_handler(data,len,rev_type);
		    break;
		case 0xD302:
			APP_BD_D302_handler(data,len,rev_type);
		    break;
		case 0xD303:
			APP_BD_D303_handler(data,len,rev_type);
			break;
		#endif
		
		#ifdef Scene_Panel
		case 0xCF00:
			APP_BD_CF00_handler(data,len,rev_type);
		    break;
		case 0xCF02:
			APP_BD_CF02_handler(data,len,rev_type);
		    break;
		case 0xCF03:
			APP_BD_CF03_handler(data,len,rev_type);
			break;
		#endif
		
		default :
		{
			if(p->ctr == 0x01)
				ctr = 0xC1;
			else if(p->ctr == 0x04)
				ctr = 0xC4;
			error = BD_InfInexistence;
			APP_BD_protocol_handler_callback(p->type,p->addr,ctr,p->DT[0],p->DT[1],&error,1,rev_type);
			return 0;
		}
	
	}
	return 1;

	
}

//接收APP数据处理
unsigned char jmesh_app_recv_handler(unsigned char id,unsigned short instance,unsigned short length,unsigned char* data)
{
	if(instance-jmesh_ble_get_local_svc_ins() == JMESHS_IDX_MESH_APP_TRANMISSION_DATA_IN_VAL)
	{
//		memcpy(app_rev_data_buff,data,length);
//		app_rev_data_len = length;
//		phone_connect_id = id;
//		//jmesh_uart_send(0, app_rev_data_len, data);	
//		//char a[5] = {0x11,0x22,0x33,0x44,0x55};
//		//jmesh_ble_gatt_notify_no_wait(phone_connect_id,jmesh_ble_get_local_svc_ins()+JMESHS_IDX_MESH_APP_TRANMISSION_DATA_OUT_VAL,5,a);
//		APP_BD_protocol_handler(app_rev_data_buff,app_rev_data_len,1);
		jmesh_proxy_recv(id,instance,length,data);
		return 1;
	}
	if(instance-jmesh_ble_get_local_svc_ins() == JMESHS_IDX_MESH_TEST_VAL)
	{
		unsigned char buff[10] = {0xaa,0xbb,0xcc};
		jmesh_ble_gatt_notify(id,jmesh_ble_get_local_svc_ins()+JMESHS_IDX_MESH_TEST_VAL,3,buff);
		//app_print_buffer_cmd(length,data,"DATA2:");
		return 1;
	}
	return 0;
}

int jmesh_app_data_recv_handler(unsigned char *data)
{
	if(data[0] > sizeof(app_rev_data_buff))
	{
		jmesh_pdu_free(data);
		return 0;
	}
	memcpy(app_rev_data_buff,data+2,data[0]);
	app_rev_data_len = data[0];
	APP_BD_protocol_handler(app_rev_data_buff,app_rev_data_len,1);
	jmesh_pdu_free(data);
	return 1;
}

