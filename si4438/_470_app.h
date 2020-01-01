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
	_470appTaskID_T 	TaskId; 		// ����̶�ID
}_470AppTaskStatusInf_T;

/* 
* ���ܣ�470Ӧ�ò�������ݲ�����  
*/
void _470_app_recv(void);

/* 
* ���ܣ�470Ӧ�ò��������ͳ������ݣ�ʹ�ñ����ֻ�ͨ�Žӿ�Э��͸���� 
* ������
* 	dest_addr:
* 		Ŀ��ڵ��ַ��6�ֽ�
* 	send_data��
* 		���͸�Ŀ��ڵ�����ݵ�ָ�� 
* 	len��
* 		���͸�Ŀ��ڵ�����ݵĳ��� 
* ���أ�
* 	������������1�����򷵻�0 
*/
uint8_t _470_app_read_meter(uint8_t* dest_addr,const uint8_t *data, uint8_t len);
void report_by_470(void);
#endif	//_470_APP_H
