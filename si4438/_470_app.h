//---------------------------------------------------
// file name : _470_app.h
//--------------------------------------------------- 
#ifndef _470_APP_H
#define _470_APP_H

#include <stdint.h>


typedef enum
{
	_470appTaskID_NULL             = 0xff,
	_470appTaskID_CtrlMeter        = 1,	
}_470appTaskID_T;


typedef struct _470AppTaskStatusInf
{
	_470appTaskID_T 	TaskId; 		// 任务固定ID
}_470AppTaskStatusInf_T;

/* 
* 功能：470应用层接收数据并处理  
*/
void _470_app_recv(void);

/* 
* 功能：470应用层打包并发送抄表数据（使用北电手机通信接口协议透传） 
* 参数：
* 	dest_addr:
* 		目标节点地址，6字节
* 	send_data：
* 		发送给目标节点的数据的指针 
* 	len：
* 		发送给目标节点的数据的长度 
* 返回：
* 	发送正常返回1，否则返回0 
*/
uint8_t _470_app_read_meter(uint8_t* dest_addr,const uint8_t *data, uint8_t len);
void report_by_470(void);
#endif	//_470_APP_H
