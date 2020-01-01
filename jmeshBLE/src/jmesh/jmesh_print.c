#include"jmesh_print.h"
#include"../driver/jmesh_uart.h"
#include"../driver/jmesh_uart_callback.h"
#include"../driver/jmesh_ble.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include"../bearer/gatt/jmesh_proxy.h"
#include"../jmesh/jmesh_config.h"
#if (!JMESH_CMD_GATT_USE_PROXY)
#include"jmesh_prov_svc.h"
#endif
#include "APP_protocol.h"

char sprint_buf[256] = {0};
char sprint_cmd_buf[256] = {0};
char print_log_flag = 1;
int _print_cmd(const char* fmt, ...)
{
	va_list args;
	int n;
	wait_uart_send_over();
	va_start(args, fmt);
	n = vsprintf(sprint_cmd_buf, fmt,args);
	va_end(args);

	jmesh_uart_send_no_take_semphr(JMESH_UART_0, strlen(sprint_cmd_buf), (unsigned char*)sprint_cmd_buf);

	return n;
}

int _gatt_send_cmd(unsigned char id,const char* fmt, ...)
{
    va_list args;
	  int n;
	  va_start(args, fmt);
	  n = vsprintf(sprint_cmd_buf, fmt,args);
	  va_end(args);
    #if (JMESH_CMD_GATT_USE_PROXY)
    jmesh_proxy_send(JMESH_SEND_TYPE_CONNECT,id,0xff,JMESH_PROXY_TYPE_CONFIG,strlen(sprint_cmd_buf),(unsigned char*)sprint_cmd_buf);
    #else
    jmesh_adapter_gatt_send(JMESH_SEND_TYPE_CONNECT,id,0xff,jmesh_ble_get_local_svc_ins()+JMESHS_IDX_MESH_CONFIG_CMD_VAL,strlen(sprint_cmd_buf),(unsigned char*)sprint_cmd_buf);
    #endif
	  return n;
}
int _print(const char* fmt, ...)
{
	if(jmesh_log_type!=1&&jmesh_log_type!=2)
		return 0;
	if(print_log_flag == 0) return 0;
	va_list args;
	int n;
	if(JMESH_UART_DEBUG != JMESH_RTT_LOG) wait_uart_send_over();
	va_start(args, fmt);
	n = vsprintf(sprint_buf, fmt,args);
	va_end(args);

	if(jmesh_log_type == 1)
		JMESH_DEBUG_SEND(strlen(sprint_buf),(unsigned char*)sprint_buf);
	else if(jmesh_log_type == 2)
		JMESH_DEBUG_SEND_TO_CKQ(strlen(sprint_buf),(unsigned char*)sprint_buf);


	return n;
}
void StrToHex(unsigned char *pbDest, char *pszSrc, int nLen)
{
    char h1, h2;
    int i;
    unsigned char s1, s2;
    for (i = 0; i < nLen; i++)
    {
        h1 = pszSrc[2 * i];
        h2 = pszSrc[2 * i + 1];

        s1 = toupper(h1) - 0x30;
        if (s1 > 9)
            s1 -= 7;

        s2 = toupper(h2) - 0x30;
        if (s2 > 9)
            s2 -= 7;

        pbDest[i] = s1 * 16 + s2;
    }
}
void HexToStr(char *pszDest, unsigned char *pbSrc, int nLen)
{
    char    ddl, ddh;
    int i;
    for (i = 0; i < nLen; i++)
    {
        ddh = 48 + pbSrc[i] / 16;
        ddl = 48 + pbSrc[i] % 16;
        if (ddh > 57) ddh = ddh + 7;
        if (ddl > 57) ddl = ddl + 7;
        pszDest[i * 2] = ddh;
        pszDest[i * 2 + 1] = ddl;
    }

    //pszDest[nLen * 2] = '\0';
}
int _print_buffer_cmd(int len,char* data,const char* fmt, ...)
{
    int str_len;
    va_list args;
	int n;
	wait_uart_send_over();
	va_start(args, fmt);
	n = vsprintf(sprint_cmd_buf, fmt,args);
	va_end(args);
	str_len=strlen(sprint_cmd_buf);
	if(sprint_cmd_buf[str_len-1]=='\n'){
        str_len--;
	}
	if(str_len+len*2>254){
        len=(254-str_len)/2;
	}
    HexToStr(sprint_cmd_buf+str_len,(unsigned char*)data,len);
    sprint_cmd_buf[str_len+len*2]='\n';
    sprint_cmd_buf[str_len+len*2+1]='\0';
	jmesh_uart_send_no_take_semphr(JMESH_UART_0, len*2+str_len+1, (unsigned char*)sprint_cmd_buf);
	return n;
}
int _gatt_send_buffer_cmd(unsigned char id,int len,char* data,const char* fmt, ...)
{
    int str_len;
    va_list args;
    int n;
    va_start(args, fmt);
    n = vsprintf(sprint_cmd_buf, fmt,args);
    va_end(args);
    str_len=strlen(sprint_cmd_buf);
    if(sprint_cmd_buf[str_len-1]=='\n'){
        str_len--;
	  }
    if(str_len+len*2>254){
        len=(254-str_len)/2;
    }
    HexToStr(sprint_cmd_buf+str_len,(unsigned char*)data,len);
    sprint_cmd_buf[str_len+len*2]='\n';
    sprint_cmd_buf[str_len+len*2+1]='\0';
    #if (JMESH_CMD_GATT_USE_PROXY)
    jmesh_proxy_send(JMESH_SEND_TYPE_CONNECT,id,0xff,JMESH_PROXY_TYPE_CONFIG,len*2+str_len+1,(unsigned char*)sprint_cmd_buf);
    #else
    jmesh_adapter_gatt_send(JMESH_SEND_TYPE_CONNECT,id,0xff,jmesh_ble_get_local_svc_ins()+JMESHS_IDX_MESH_CONFIG_CMD_VAL,len*2+str_len+1,(unsigned char*)sprint_cmd_buf);
    #endif
    return n;
}
int _print_buffer(int len,char* data,const char* fmt, ...)
{
	if(jmesh_log_type!=1&&jmesh_log_type!=2)
		return 0;
	if(print_log_flag ==0) return 0;
    int str_len;
    va_list args;
	int n;
	if(JMESH_UART_DEBUG != JMESH_RTT_LOG) wait_uart_send_over();
	va_start(args, fmt);
	n = vsprintf(sprint_buf, fmt,args);
	va_end(args);
	str_len=strlen(sprint_buf);
	if(sprint_buf[str_len-1]=='\n'){
        str_len--;
	}
	if(str_len+len*2>254){
        len=(254-str_len)/2;
	}
    HexToStr(sprint_buf+str_len,(unsigned char*)data,len);
    sprint_buf[str_len+len*2]='\n';
    sprint_buf[str_len+len*2+1]='\0';
    if(jmesh_log_type == 1)
			JMESH_DEBUG_SEND(len*2+str_len+1,(unsigned char*)sprint_buf);
		else if(jmesh_log_type == 2)
			JMESH_DEBUG_SEND_TO_CKQ(len*2+str_len+1,(unsigned char*)sprint_buf);


	return n;
}
