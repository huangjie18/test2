#include "APP_config.h"


wirless_tech_t wirless_tech;

os_timer_event_t ota_req_permit_timer;
unsigned char ota_req_permit_state = 0;

void ota_permit_timer_handler(void)
{
	ota_req_permit_state = 0;
}

/***********************************************************************************************
��    �ƣ�void ota_adv_timer_start(void)
��    �ܣ����յ���������֮������OTA������10����֮��ر�OTA����
��    �룺��
��    �أ���
��    �ڣ�20191029
************************************************************************************************/
void ota_permit_timer_start(void)
{	
	ota_req_permit_state = 1;
	os_timer_event_set(&ota_req_permit_timer,10*60*1000, (os_timer_event_caller_t)ota_permit_timer_handler,NULL);
}

void OTA_Switch(void)
{
	ota_permit_timer_start();
}
