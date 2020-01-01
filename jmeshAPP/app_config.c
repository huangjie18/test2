#include "APP_config.h"


wirless_tech_t wirless_tech;

os_timer_event_t ota_req_permit_timer;
unsigned char ota_req_permit_state = 0;

void ota_permit_timer_handler(void)
{
	ota_req_permit_state = 0;
}

/***********************************************************************************************
名    称：void ota_adv_timer_start(void)
功    能：接收到设置命令之后，允许OTA请求，在10分钟之后关闭OTA请求
输    入：无
返    回：无
日    期：20191029
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
