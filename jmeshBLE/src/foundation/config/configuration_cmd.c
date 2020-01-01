#include"configuration_cmd.h"
#include"../config/config_client_app.h"
#include"config_server.h"
#include"config_client.h"
#include"../../jmesh/jmesh_print.h"
#include"../state/jmesh_state.h"
#include"../../app/app_include.h"
#include"../../network/jmesh_netkey.h"
#include"../../network/jmesh_seq.h"
#include"../../jmesh/jmesh_addr.h"
#include"../../access/jmesh_access.h"
#include"../../driver/jmesh_uart.h"
#include"../../driver/jmesh_driver_config.h"
#include"../../driver/jmesh_ble.h"
#include"../../bearer/routing/jmesh_route.h"
#include"../../bearer/routing/jmesh_routing_neighbor.h"
#include"rf_reg_typedef.h"
#include"../../jmesh/jmesh_config.h"
#include"../../app/expand/app_expand2/app_expand2_realcomm_client.h"
#include"../../driver/jmesh_uart_callback.h"
#include"../../app/expand/app_expand2/light/light_client.h"
#include"../../bearer/gatt/jmesh_gatt.h"
#include"../../jmesh/jmesh_features.h"
#include"../../driver/jmesh_pwm.h"
#include"../../driver/jmesh_gpio.h"
#include"../../app/expand/app_expand2/water/water.h"
#include"../../app/expand/app_expand2/water/water_client.h"
#include"../../app/expand/app_expand2/water/water_gate.h"
#include"jmesh_provisioning.h"
#include"io_ctrl.h"
#include"../../upper/lowpower/jmesh_lowpower.h"
#include"../../app/expand/app_expand2/test_app/test_app_client.h"

typedef struct st_jmesh_cmd{
    void(*handler)(unsigned char interface,const char** argv);
    char* cmd;
    char argc;
    char* desc;
}jmesh_cmd_t;

unsigned char cur_cmd_interface=JMESH_CMD_INTERFACE_UART;

static void at_cmd_at_test(unsigned char interface,const char** argv);
static void at_cmd_get_version(unsigned char interface,const char** argv);
static void at_cmd_reset(unsigned char interface,const char** argv);
static void at_cmd_restore_default(unsigned char interface,const char** argv);
static void at_cmd_set_baud(unsigned char interface,const char** argv);
static void at_cmd_set_mac(unsigned char interface,const char** argv);
static void at_cmd_get_mac(unsigned char interface,const char** argv);
static void at_cmd_onoff_log(unsigned char interface,const char** argv);
static void at_cmd_set_addr(unsigned char interface,const char** argv);
static void at_cmd_get_addr(unsigned char interface,const char** argv);
static void at_cmd_add_netkey(unsigned char interface,const char** argv);
//static void at_cmd_del_netkey(unsigned char interface,const char** argv);
static void at_cmd_get_netkey(unsigned char interface,const char** argv);
static void at_cmd_add_appkey(unsigned char interface,const char** argv);
//static void at_cmd_del_appkey(unsigned char interface,const char** argv);
static void at_cmd_get_appkey(unsigned char interface,const char** argv);
static void at_cmd_bind(unsigned char interface,const char** argv);
static void at_cmd_unbind(unsigned char interface,const char** argv);
static void at_cmd_get_bind(unsigned char interface,const char** argv);
static void at_cmd_get_para(unsigned char interface,const char** argv);
static void at_cmd_add_subscribe(unsigned char interface,const char** argv);
static void at_cmd_del_subscribe(unsigned char interface,const char** argv);
static void at_cmd_get_subscribe(unsigned char interface,const char** argv);
static void at_cmd_send_hex(unsigned char interface,const char** argv);
static void at_cmd_send_meter_cmd(unsigned char interface,const char** argv);
static void at_cmd_send_trans(unsigned char interface,const char** argv);
static void at_cmd_get_neighbor(unsigned char interface,const char** argv);
static void at_cmd_get_connected(unsigned char interface,const char** argv);
static void at_cmd_get_route(unsigned char interface,const char** argv);
static void at_cmd_set_iv_index(unsigned char interface,const char** argv);
static void at_cmd_get_iv_index(unsigned char interface,const char** argv);
static void at_cmd_update_iv_index(unsigned char interface,const char** argv);
static void at_cmd_set_light_mode(unsigned char interface,const char** argv);
static void at_cmd_light_mode(unsigned char interface,const char** argv);
static void at_cmd_set_feature(unsigned char interface,const char** argv);
static void at_cmd_get_feature(unsigned char interface,const char** argv);
static void at_cmd_set_dBm(unsigned char interface,const char** argv);
static void at_cmd_get_dBm(unsigned char interface,const char** argv);
static void at_cmd_start_ota(unsigned char interface,const char** argv);
static void at_cmd_exit_ota(unsigned char interface,const char** argv);
static void at_cmd_start_pwm(unsigned char interface,const char** argv);
static void at_cmd_end_pwm(unsigned char interface,const char** argv);
static void at_cmd_set_gpio(unsigned char interface,const char** argv);
static void at_cmd_echo(unsigned char interface,const char** argv);
static void at_cmd_iowrite(unsigned char interface,const char** argv);
static void at_cmd_ioread(unsigned char interface,const char** argv);
static void at_cmd_iopull(unsigned char interface,const char** argv);
//static void at_cmd_voltage(unsigned char interface,const char** argv);
//static void at_cmd_adc(unsigned char interface,const char** argv);
static void at_cmd_get_pwm(unsigned char interface,const char** argv);
static void at_cmd_send_ascii(unsigned char interface,const char** argv);

static void at_cmd_water_set_type(unsigned char interface,const char** argv);
static void at_cmd_water_get_type(unsigned char interface,const char** argv);
static void at_cmd_water_set_val_pin(unsigned char interface,const char** argv);
static void at_cmd_water_get_val_pin(unsigned char interface,const char** argv);
static void at_cmd_water_set_unit(unsigned char interface,const char** argv);
static void at_cmd_water_get_unit(unsigned char interface,const char** argv);
static void at_cmd_water_set_pul_pin(unsigned char interface,const char** argv);
static void at_cmd_water_get_pul_pin(unsigned char interface,const char** argv);
static void at_cmd_water_get_value(unsigned char interface,const char** argv);
static void at_cmd_water_set_value(unsigned char interface,const char** argv);
static void at_cmd_water_set_valve(unsigned char interface,const char** argv);
static void at_cmd_water_get_valve(unsigned char interface,const char** argv);
static void at_cmd_water_set_param(unsigned char interface,const char** argv);
static void at_cmd_water_get_param(unsigned char interface,const char** argv);
static void at_cmd_water_set_mac(unsigned char interface,const char** argv);
static void at_cmd_water_get_mac(unsigned char interface,const char** argv);
static void at_cmd_water_set_report(unsigned char interface,const char** argv);
static void at_cmd_water_get_report(unsigned char interface,const char** argv);
static void at_cmd_gate_set_param(unsigned char interface,const char** argv);
static void at_cmd_gate_get_param(unsigned char interface,const char** argv);
static void at_cmd_gate_list_node(unsigned char interface,const char** argv);
static void at_cmd_gate_val_by_addr(unsigned char interface,const char** argv);
static void at_cmd_gate_val_by_mac(unsigned char interface,const char** argv);
static void at_cmd_gate_get_history(unsigned char interface,const char** argv);
static void at_cmd_gate_list_history(unsigned char interface,const char** argv);
static void at_cmd_set_role(unsigned char interface,const char** argv);
static void at_cmd_get_role(unsigned char interface,const char** argv);
static void at_cmd_set_lowpower_para(unsigned char interface,const char** argv);
static void at_cmd_get_lowpower_para(unsigned char interface,const char** argv);
static void at_cmd_pdu_log(unsigned char interface,const char** argv);//test
static void at_cmd_get_timer(unsigned char interface,const char** argv);//test
static void at_cmd_get_con_rd(unsigned char interface,const char** argv);//test
static void at_cmd_test_app_start(unsigned char interface,const char** argv);//test
static void at_cmd_test_app_stop(unsigned char interface,const char** argv);//test
static void at_cmd_test_app_result(unsigned char interface,const char** argv);//test
static void at_cmd_test_app_init(unsigned char interface,const char** argv);//test
static void at_cmd_light_switch(unsigned char interface,const char** argv);
const jmesh_cmd_t jmesh_cmd_table[]={
    {at_cmd_send_trans,       "AT+SENDTRANS",    2,"AT+SENDTRANS=1,123456789"},//**send ascii data
    {at_cmd_at_test,          "AT",              0,"AT"},//**test
    {at_cmd_get_version,      "AT+VERSION?",     0,"AT+VERSION?"},//**get software version
    {at_cmd_reset,            "AT+RESET",        0,"AT+RESET"},//**reset
    {at_cmd_restore_default,  "AT+DEFAULT",      0,"AT+DEFAULT"},//**default
    {at_cmd_set_baud,         "AT+BAUD",         2,"AT+BAUD=115200,2"},//**set baud
    {at_cmd_set_mac,          "AT+MAC",          1,"AT+MAC=20190410aabb"},//**set mac
    {at_cmd_get_mac,          "AT+MAC?",         0,"AT+MAC?"},//**get mac
    {at_cmd_onoff_log,        "AT+LOG",          1,"AT+LOG=0"},//on or off information report
    {at_cmd_set_addr,         "AT+ADDR",         1,"AT+ADDR=1"},//**set mesh address
    {at_cmd_get_addr,         "AT+ADDR?",        0,"AT+ADDR?"},//**get mesh address
    {at_cmd_add_netkey,       "AT+NETKEY",       1,"AT+NETKEY=01020304050607080910111213141516"},//**set net key
    //{at_cmd_del_netkey,       "AT+DNKEY",        1,"AT+DNKEY:0"},//delete net key
    {at_cmd_get_netkey,       "AT+NETKEY?",      0,"AT+NETKEY?"},//**get net key
    {at_cmd_add_appkey,       "AT+APPKEY",       2,"AT+APPKEY=1,01020304050607080910111213141516"},//**set application key
    //{at_cmd_del_appkey,       "AT+DAKEY",        2,"AT+DAKEY:0,1"},//delete application key
    {at_cmd_get_appkey,       "AT+LISTAPP",      0,"AT+LISTAPP"},//**get application index
    {at_cmd_bind,             "AT+BIND",         2,"AT+BIND=1,bcbd0001"},//**bind application
    {at_cmd_unbind,           "AT+UNBIND",       2,"AT+UNBIND=1,bcbd0001"},//unbind application
    {at_cmd_get_bind,         "AT+LISTBIND",     1,"AT+LISTBIND=bcbd0001"},//get current bind
    {at_cmd_get_para,         "AT+PARAM?",       0,"AT+PARAM?"},//**get current parameter
    {at_cmd_add_subscribe,    "AT+ASUBS",        2,"AT+ASUBS=bcbd0001,0xfff1"},
    {at_cmd_del_subscribe,    "AT+DSUBS",        2,"AT+DSUBS=bcbd0001,0xfff1"},
    {at_cmd_get_subscribe,    "AT+RSUBS",        1,"AT+RSUBS=0xbcbd0001"},
    {at_cmd_send_hex,         "AT+SENDHEX",      2,"AT+SENDHEX=1,112233aabbcc"},//**send hex data
    {at_cmd_send_meter_cmd,   "AT+SENDMETER",    2,"AT+SENDMETER=1,112233aabbcc"},//**send hex data
    {at_cmd_get_neighbor,     "AT+NEIG?",        0,"AT+NEIG?"},//get my neighbor device
    {at_cmd_get_connected,    "AT+CONNECTED?",   0,"AT+CONNECTED?"},//get my current connected device
    {at_cmd_get_route,        "AT+ROUT?",        0,"AT+ROUT?"},//get my route
    {at_cmd_set_iv_index,     "AT+SIV",          2,"AT+SIV=0,123"},
    {at_cmd_get_iv_index,     "AT+GIV",          1,"AT+GIV=0"},
    {at_cmd_update_iv_index,  "AT+UIV",          2,"AT+UIV=0,1"},
    {at_cmd_set_light_mode,   "AT+SLTM",         5,"AT+SLTM=0,150,3,8,0"},
    {at_cmd_light_mode,       "AT+LTM",          2,"AT+LTM=3,0"},
    {at_cmd_set_feature,      "AT+SFET",         4,"AT+SFET=1,1,0,0"},
    {at_cmd_get_feature,      "AT+FET?",         0,"AT+FET?"},
	{at_cmd_set_dBm,          "AT+TXPOWER",      1,"AT+TXPOWER=8"},//set txpower
    {at_cmd_get_dBm,          "AT+TXPOWER?",     0,"AT+TXPOWER?"},//get txpower
    {at_cmd_start_ota,        "AT+ENOTA",        0,"AT+ENOTA"},//start(enable) OTA
    {at_cmd_exit_ota,         "AT+DISOTA",       0,"AT+DISOTA"},//exit(disable)OTA
    {at_cmd_start_pwm,        "AT+PWM",          4,"AT+PWM=4,0,1000,50%"},//**
    {at_cmd_end_pwm,          "AT+PWMCLOSE",     1,"AT+PWMCLOSE=0"},//**
    {at_cmd_set_gpio,         "AT+IOSET",        2,"AT+IOSET=4,1"},//**
	{at_cmd_echo,             "AT+ECHO",         1,"AT+ECHO=ON"},//echo on or echo off
    {at_cmd_iowrite,          "AT+IOWRITE",      2,"AT+IOWRITE=4,1"},
    {at_cmd_ioread ,          "AT+IOREAD",       1,"AT+IOREAD=4"},
    {at_cmd_iopull,           "AT+IOPULL",       2,"AT+IOPULL=1,1"},
    {at_cmd_get_pwm,          "AT+PWM?",         0,"AT+PWM?"},
	{at_cmd_send_ascii,       "AT+SENDASCII",    2,"AT+SENDASCII=1,123456789"},

    {at_cmd_water_set_type,   "AT+WTTYPE",       2,"AT+WTTYPE=5,1"},
    {at_cmd_water_get_type,   "AT+WTRTYPE",      1,"AT+WTTYPE=5"},
    {at_cmd_water_set_val_pin,"AT+WTVALVEPIN",   2,"AT+WTVALVEPIN=5,21"},
    {at_cmd_water_get_val_pin,"AT+WTRVALVEPIN",  1,"AT+WTRVALVEPIN=5"},
    {at_cmd_water_set_unit,   "AT+WTUNIT",       2,"AT+WTUNIT=5,10"},
    {at_cmd_water_get_unit,   "AT+WTRUNIT",      1,"AT+WTUNIT=5"},
    {at_cmd_water_set_pul_pin,"AT+WTPULSEPIN",   4,"AT+WTPULSEPIN=5,16,22,0"},
    {at_cmd_water_get_pul_pin,"AT+WTRPULSEPIN",  1,"AT+WTRPULSEPIN=5"},
    {at_cmd_water_get_value,  "AT+WTRVALU",      1,"AT+WTRVALU=5"},
    {at_cmd_water_set_value,  "AT+WTVALU",       3,"AT+WTVALU=5,999,10"},
    {at_cmd_water_set_valve,  "AT+WTVALV",       2,"AT+WTVALV=5,1"},
    {at_cmd_water_get_valve,  "AT+WTRVALV",      1,"AT+WTRVALV=5"},
    {at_cmd_water_set_param,  "AT+WTPARA",       9,"AT+WTPARA=5,1,21,2000,3,10,16,22,0"},
    {at_cmd_water_get_param,  "AT+WTRPARA",      1,"AT+WTRPARA=5"},
    {at_cmd_water_set_mac,    "AT+WTSETMAC",     2,"AT+WTSETMAC=3,11223344aabbccdd"},
    {at_cmd_water_get_mac,    "AT+WTGETMAC",     1,"AT+WTGETMAC=3"},
    {at_cmd_water_set_report, "AT+WTREPTPARA",   3,"AT+WTREPTPARA=5,2000,3"},
    {at_cmd_water_get_report, "AT+WTRREPTPARA",  1,"AT+WTRREPTPARA=5"},
    {at_cmd_gate_set_param,   "AT+WTGPARA",      3,"AT+WTGPARA=5,1,3"},
    {at_cmd_gate_get_param,   "AT+WTGPARA?",     0,"AT+WTGPARA?"},
    {at_cmd_gate_list_node,   "AT+WTGLISTNODE?", 0,"AT+WTGLISTNODE?"},
    {at_cmd_gate_val_by_addr, "AT+WTGVALBYADDR", 1,"AT+WTGVALBYADDR=5"},
    {at_cmd_gate_val_by_mac,  "AT+WTGVALBYMAC",  1,"AT+WTGVALBYMAC=112233445566"},
    {at_cmd_gate_get_history, "AT+WTGHIS",       2,"AT+WTGHIS=5,30"},
    {at_cmd_gate_list_history,"AT+WTGLISTHIS",   2,"AT+WTGLISTHIS=5,0"},
    {at_cmd_set_role,         "AT+ROLE",         2,"AT+ROLE:1,0"},//water,gate
    {at_cmd_get_role,         "AT+ROLE?",        0,"AT+ROLE?"},
    {at_cmd_set_lowpower_para,"AT+LOWPARA",      4,"AT+LOWPARA:10,10,10,10"},
    {at_cmd_get_lowpower_para,"AT+LOWPARA?",     0,"AT+LOWPARA?"},
    {at_cmd_pdu_log,          "AT+PDULOG",       0,"AT+PDULOG"},//test
    {at_cmd_get_timer,        "AT+TIMER?",       0,"AT+TIMER?"},//test
    {at_cmd_get_con_rd,       "AT+CONRD?",       0,"AT+CONRD?"},//test
    {at_cmd_test_app_start,   "AT+APPSTART",     4,"AT+APPSTART=3,R,5,30"},//test
    {at_cmd_test_app_stop,    "AT+APPSTOP",      2,"AT+APPSTOP=3,R"},//test
    {at_cmd_test_app_result,  "AT+APPRESULT",    2,"AT+APPRESULT=3,R"},//test
    {at_cmd_test_app_init,    "AT+APPCLEAR",     2,"AT+APPCLEAR=3,R"},//test
    {at_cmd_light_switch,     "AT+LIGHT",        2,"AT+LIGHT=4,ON"},
};

#define JMESH_CMD_TABLE_SIZE (sizeof(jmesh_cmd_table)/sizeof(jmesh_cmd_t))
#if (JMESH_CMD_OUTPUT_CHOOSE>JMESH_CMD_OUTPUT_NONE)

static unsigned char echo_flag;
void configuration_cmd_process(unsigned char interface,unsigned char length,unsigned char* data)
{
  static int cmd_interface=JMESH_CMD_OUTPUT_CHOOSE;
  int i,j;
	int argc=0,arg_num=0;
	char *p_str;
	char* argv[15];
	if( 1==echo_flag){
	    jmesh_uart_send(JMESH_UART_0,length,data);
	    JMESH_CMD_RES(interface,"\n");
	}
	if((JMESH_CMD_OUTPUT_UART==cmd_interface)&&(JMESH_CMD_INTERFACE_UART!=interface))
  {
      return;
  }
  else if((JMESH_CMD_OUTPUT_GATT==cmd_interface)&&(JMESH_CMD_INTERFACE_UART==interface))
  {
      return;
  }
  if(length<2)
  {
      return;
  }
  if('\n'==data[length-1])
  {
      data[length-1]=0x00;
      if('\r'==data[length-2])
      {
          data[length-2]=0x00;
      }
  }
  else if('\r'==data[length-1])
  {
      data[length-1]=0x00;
  }
  else
  {
      data[length]=0x00;
  }
  cur_cmd_interface=interface;
  if(NULL!=(p_str=strstr((const char *)data,"="))){
      *p_str='\0';
      p_str++;
      if(0x00==*p_str){
          //JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARSE_FAILURE);
          return;
      }
      else{
          argv[0]=p_str;
          arg_num++;
      }
  }
	for(i=0;i<JMESH_CMD_TABLE_SIZE;i++){
        if(0==strcmp((const char *)data,jmesh_cmd_table[i].cmd)){
            argc=i?15:jmesh_cmd_table[i].argc;
            for(j=1;j<argc;j++){
                if(NULL==p_str){
                    break;
                }
                else if(NULL!=(p_str=strstr(p_str,","))){
                    *p_str=0x00;
                    p_str++;
                    if(*p_str){
                        argv[j]=p_str;
                        arg_num++;
                    }
                    else{
                        //JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARSE_FAILURE);
                        return;
                    }
                }
            }
            if(arg_num!=jmesh_cmd_table[i].argc)
            {
                //JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARSE_FAILURE);
                return;
            }
            jmesh_cmd_table[i].handler(interface,(const char**)argv);
            return;
        }
	}
  if(JMESH_CMD_TABLE_SIZE==i)
  {
      //JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARSE_FAILURE);
  }
}
#endif

int jmesh_config_cmd_recv_handler(jmesh_pdu_t *pdu)
{
    configuration_cmd_process(pdu->network.itf,pdu->length,pdu->pdu+2);
    jmesh_pdu_free(pdu);
    return 0;
}

int str_to_hex(const char* str,unsigned char hex_size,unsigned char* hex)
{
    int i,j;
    for(i=0,j=0;str[i]!='\0'&&j<hex_size;i+=2,j++){
        if(str[i]>='0'&&str[i]<='9')
            hex[j]=str[i]-'0';
        else if(str[i]>='A'&&str[i]<='F')
            hex[j]=str[i]-'A'+10;
        else if(str[i]>='a'&&str[i]<='f')
            hex[j]=str[i]-'a'+10;
        else{
            break;
        }
        if(str[i+1]=='\0'){
            j++;
            break;
        }
        hex[j]<<=4;
        if(str[i+1]>='0'&&str[i+1]<='9')
            hex[j]|=str[i+1]-'0';
        else if(str[i+1]>='A'&&str[i+1]<='F')
            hex[j]|=str[i+1]-'A'+10;
        else if(str[i+1]>='a'&&str[i+1]<='f')
            hex[j]|=str[i+1]-'a'+10;
        else{
            break;
        }
    }
    return j;
}

static void at_cmd_at_test(unsigned char interface,const char** argv)
{
//    unsigned short a=100;
//    unsigned short b=200;
//    unsigned short c;
//    unsigned long d=0x112233;
//    c=d;
//    if(a-b<50){
//      JMESH_CMD_RES(interface,"(unsigned short)(a-b):%d,(a-b):%d,c:%d,(unsigned short)c:%d\n",(unsigned short)(a-b),(a-b),c,(unsigned short)c);
//    }
//    if((unsigned short)(a-b)<50){
//      JMESH_CMD_RES(interface,"yes\n");
//    }
//    else{
//      JMESH_CMD_RES(interface,"no\n");
//    }
    JMESH_CMD_RES(interface,"OK\n");
}

#define STR(s) #s
#define CUR_VERSION(a,b,c) ""STR(a)"."STR(b)"."STR(c)"\n"
static void at_cmd_get_version(unsigned char interface,const char** argv)
{
    JMESH_CMD_RES(interface,CUR_VERSION(MAINVER,SUBVER1,SUBVER2));
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_reset(unsigned char interface,const char** argv)
{
    JMESH_CMD_RES(interface,"OK\n");
    if(JMESH_CMD_INTERFACE_UART==interface)
    {
        wait_uart_send_over();
    }
    jmesh_ble_platform_reset();
}

typedef struct st_default_param{
    JMESH_UART_BAUD_T uart_baud;
    unsigned char uart_parity;
    unsigned char mac[6];
    unsigned short addr;
    unsigned char net_key[16];
    unsigned char app_key[16];
}default_param_t;

static void at_cmd_restore_default(unsigned char interface,const char** argv)
{
  /*
    default_param_t default_param={
        .uart_baud=JMESH_UART_BAUD_115200,
        .uart_parity=JMESH_UART_PARITY_EVEN,
        .mac=BX_DEV_ADDR,
        .addr=JMESH_ADDR_UNASSIGNED,
        .net_key=JMESH_NETKEY,
        .app_key=JMESH_APPKEY
    };

    unsigned char  key[16] = {0};
	jmesh_mac_t mac;
	unsigned short jmesh_primary_addr;
	jmesh_appkey_t jmesh_appkey;
	jmesh_appkey_t *p_appkey;
	jmesh_uart_baudrate_set(default_param.uart_baud,default_param.uart_parity);
    jmesh_mac_set(default_param.mac);
	jmesh_mac_get(mac);
	jmesh_primary_addr=JMESH_ADDR_UNASSIGNED;
    jmesh_save_write(JMESH_SAVE_ADDR,2,(unsigned char*)&jmesh_primary_addr);
    jmesh_netkey_state_set(0,default_param.net_key);
	JMESH_CMD_RES(interface,"baudrate:%d\n",enum_to_baudrate( default_param.uart_baud));
	JMESH_CMD_RES(interface,"addr:%d\n",default_param.addr);
	JMESH_CMD_RES_BUFFER(interface,6,mac,"mac:");
    if(JMESH_STATUS_SUCCESS==jmesh_netkey_state_get(0,key)){
        JMESH_CMD_RES_BUFFER(interface,16,key,"net key:");
    }
	for(int i=0;i<JMESH_APPKEY_MAX_NUM;i++){
	    if(0!=jmesh_save_buffer_read(JMESH_SAVE_APPKEY,i,sizeof(jmesh_appkey_t),(unsigned char*)&jmesh_appkey))
		{
		    jmesh_appkey_delete(0,jmesh_appkey.appkey_index );
		}
	}
	jmesh_appkey_set(0,1,default_param.app_key);
    p_appkey=jmesh_appkey_get_by_key_index(0,1);
    if(NULL!=p_appkey){
		JMESH_CMD_RES_BUFFER(interface,16,p_appkey->key,"appkey index:%d\napp key:",p_appkey->appkey_index);
    }*/
    jmesh_save_init();
	  JMESH_CMD_RES(interface,"OK\n");
    if(JMESH_CMD_INTERFACE_UART==interface)
    {
        wait_uart_send_over();
    }
    jmesh_ble_platform_reset();
}

static void at_cmd_set_baud(unsigned char interface,const char** argv)
{
    signed char baud_enum;
    unsigned long baudrate=strtol(argv[0],NULL,0);
    unsigned char pari_flag=strtol(argv[1],NULL,0);
    baud_enum=baudrate_to_enum(baudrate);
    if(0>baud_enum)
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA1_INVALID);
        return;
    }
    JMESH_CMD_RES(interface,"OK\n");
	wait_uart_send_over();
    jmesh_uart_baudrate_set(baud_enum,pari_flag);
}

static void at_cmd_set_mac(unsigned char interface,const char** argv)
{
    if(12!=strlen(argv[0]))
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
    }
    else
    {
        unsigned char mac[6]={0x00};
        if(6!=str_to_hex(argv[0],6,mac))
        {
            JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
            return;
        }
        jmesh_mac_set(mac);
        JMESH_CMD_RES(interface,"OK\n");
        if(JMESH_CMD_INTERFACE_UART==interface)
        {
            wait_uart_send_over();
        }
        jmesh_ble_platform_reset();
    }
}

static void at_cmd_get_mac(unsigned char interface,const char** argv)
{
    jmesh_mac_t mac;
    jmesh_mac_get(mac);
    JMESH_CMD_RES_BUFFER(interface,6,mac,"");
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_onoff_log(unsigned char interface,const char** argv)
{
    int onoff=strtol(argv[0],NULL,0);
    if(0==onoff)
    {
        print_log_flag=0;
    }
    else if(1==onoff)
    {
        print_log_flag=1;
    }
    else
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
        return;
    }
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_set_addr(unsigned char interface,const char** argv)
{
    unsigned short mesh_addr = 0;
    mesh_addr = strtol(argv[0],NULL,0);
    if(0==mesh_addr%2){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
        return;
    }
	else if(mesh_addr<3){
	    JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
        return;
	}
    else if(0==jmesh_set_primary_addr(mesh_addr)){
        JMESH_CMD_RES(interface,"OK\n");
		if(JMESH_CMD_INTERFACE_UART==interface)
        {
            wait_uart_send_over();
        }
        jmesh_ble_platform_reset();
    }
    else{
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
}

static void at_cmd_get_addr(unsigned char interface,const char** argv)
{
    JMESH_CMD_RES(interface,"%d\n",jmesh_get_primary_addr());
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_add_netkey(unsigned char interface,const char** argv)
{
    unsigned char key[16]={0x00};
    if(32!=strlen(argv[0])){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
        return;
    }
    if(16!=str_to_hex(argv[0],16,key)){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
        return;
    }
    jmesh_netkey_state_del(0);
    if(JMESH_STATUS_SUCCESS==jmesh_netkey_state_set(0,key))
    {
        JMESH_CMD_RES(interface,"OK\n");
    }
    else
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
}

/*static void at_cmd_del_netkey(unsigned char interface,const char** argv)
{
    unsigned short netkey_index=0;
    netkey_index = strtol(argv[0],NULL,0);
    if(JMESH_STATUS_SUCCESS==jmesh_netkey_state_del(netkey_index))
    {
        JMESH_CMD_RES(interface,"OK\n");
    }
    else
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
}*/

static void at_cmd_get_netkey(unsigned char interface,const char** argv)
{
    unsigned char key[16]={0x00};
    if(JMESH_STATUS_SUCCESS==jmesh_netkey_state_get(0,key))
    {
        JMESH_CMD_RES_BUFFER(interface,16,key,"");
    }
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_add_appkey(unsigned char interface,const char** argv)
{
    unsigned char key[16]={0x00};
    unsigned short appkey_index=0;
    appkey_index = strtol(argv[0],NULL,0);
    if(32!=strlen(argv[1])){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
        return;
    }
    if(16!=str_to_hex(argv[1],16,key)){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
        return;
    }
    //jmesh_appkey_delete(0,1);
    if(JMESH_STATUS_SUCCESS==jmesh_appkey_set(0,appkey_index,key))
    {
        JMESH_CMD_RES(interface,"OK\n");
    }
    else
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
}

/*static void at_cmd_del_appkey(unsigned char interface,const char** argv)
{
    unsigned short netkey_index=0,appkey_index=0;
    netkey_index = strtol(argv[0],NULL,0);
    appkey_index = strtol(argv[1],NULL,0);
    if(JMESH_STATUS_SUCCESS==jmesh_appkey_delete(netkey_index,appkey_index))
    {
        JMESH_CMD_RES(interface,"OK\n");
    }
    else
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
}*/

static void at_cmd_get_appkey(unsigned char interface,const char** argv)
{
    //jmesh_appkey_t* appkey;
    int pdu_index;
    unsigned short appkey_index=0;
    unsigned char pdu[9]={0};
    /*unsigned char key[16]={0x00};
    appkey=jmesh_appkey_get_by_key_index(0,1);
    if(NULL!=appkey)
    {
        JMESH_CMD_RES_BUFFER(interface,16,appkey->key,"");
    }*/
	pdu_index=jmesh_appkey_list(0,9,pdu);
    for(int j=0;j<pdu_index;j+=3)
    {
        if(pdu_index-j<3)
        {
            appkey_index = pdu[j] + ((pdu[j+1]&0x0f)<<8);
            JMESH_CMD_RES(interface,"appkey index:%d\n", appkey_index);
            break;
        }
            appkey_index = pdu[j] + ((pdu[j+1]&0x0f)<<8);
            JMESH_CMD_RES(interface,"appkey index:%d\n", appkey_index);
            appkey_index = (pdu[j+1]>>4) + (pdu[j+2]<<4);
            JMESH_CMD_RES(interface,"appkey index:%d\n", appkey_index);
    }
    JMESH_CMD_RES(interface,"OK\n");
}


static void at_cmd_bind(unsigned char interface,const char** argv)
{
    unsigned long app_index=strtol(argv[0],NULL,0);
	jmesh_element_t* element;
    jmesh_model_id_t model_id;
    jmesh_model_t* model;
    jmesh_appkey_t* appkey;
    unsigned long model_id_int=0;
    unsigned char str[9];
    int str_len;
    int i,j;
    unsigned char id[4]={0x00};
    unsigned long id_val;
    element=jmesh_element_get_by_index(1);
    if(NULL==element){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
        return;
    }
    memset(str,'0',sizeof(str));
    str_len=strlen(argv[1]);
    if(str_len>8){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
        return;
    }
    for(i=8-str_len,j=0;i<8;i++,j++){
        str[i]=argv[1][j];
    }
    if(4!=str_to_hex((const char *)str,4,id)){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
        return;
    }
    id_val=id[0];
    model_id_int+=id_val<<24;
    id_val=id[1];
    model_id_int+=id_val<<16;
    id_val=id[2];
    model_id_int+=id_val<<8;
    id_val=id[3];
    model_id_int+=id_val;
    model_id.company_id = model_id_int>>16;
    if(model_id.company_id)
    {
        model_id.model_id = model_id_int&0xffff;
	    model = jmesh_model_get_by_vendor_id(element,model_id.company_id,model_id.model_id);
    }
    else
    {
        model_id.sig_id = model_id_int&0xffff;
	    model_id.model_id = JMESH_MODEL_ID_INVALID;
	    model = jmesh_model_get_by_sig_id(element,model_id.sig_id);
    }
    if(NULL==model){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
    else if(NULL==(appkey=jmesh_appkey_get_by_key_index(0,app_index))){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
    else if(JMESH_STATUS_SUCCESS==jmesh_model_bind_app(model,appkey))
    {
        JMESH_CMD_RES(interface,"OK\n");
    }
    else
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
}

static void at_cmd_unbind(unsigned char interface,const char** argv)
{
    unsigned long app_index=strtol(argv[0],NULL,0);
	jmesh_element_t* element;
    unsigned long model_id_int=0;
    jmesh_model_id_t model_id;
    jmesh_model_t* model;
    jmesh_appkey_t* appkey;
    unsigned char str[9];
    int str_len;
    int i,j;
    unsigned char id[4]={0x00};
    unsigned long id_val;
    element=jmesh_element_get_by_index(1);
    if(NULL==element){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
        return;
    }
    memset(str,'0',sizeof(str));
    str_len=strlen(argv[1]);
    if(str_len>8){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
        return;
    }
    for(i=8-str_len,j=0;i<8;i++,j++){
        str[i]=argv[1][j];
    }
    if(4!=str_to_hex((const char *)str,4,id)){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
        return;
    }
    id_val=id[0];
    model_id_int+=id_val<<24;
    id_val=id[1];
    model_id_int+=id_val<<16;
    id_val=id[2];
    model_id_int+=id_val<<8;
    id_val=id[3];
    model_id_int+=id_val;
    model_id.company_id = model_id_int>>16;
    if(model_id.company_id)
    {
        model_id.model_id = model_id_int&0xffff;
			  model = jmesh_model_get_by_vendor_id(element,model_id.company_id,model_id.model_id);
    }
    else
    {
        model_id.sig_id = model_id_int&0xffff;
			  model_id.model_id = JMESH_MODEL_ID_INVALID;
			  model = jmesh_model_get_by_sig_id(element,model_id.sig_id);
    }
    if(NULL==model){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
    else if(NULL==(appkey=jmesh_appkey_get_by_key_index(0,app_index))){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
    else if(JMESH_STATUS_SUCCESS==jmesh_model_unbind_app(model,appkey))
    {
        JMESH_CMD_RES(interface,"OK\n");
    }
    else
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
}

static void at_cmd_get_bind(unsigned char interface,const char** argv)
{
    jmesh_element_t* element;
    unsigned long model_id_int=0;
    jmesh_model_id_t  model_id;
    jmesh_model_t* model;
//  jmesh_appkey_t* appkey;
    unsigned char buffer[9] = {0};
    int length;
    unsigned short appkey_index=0;
    unsigned char str[9];
    int str_len;
    int i,j;
    unsigned char id[4]={0x00};
    unsigned long id_val;
    element=jmesh_element_get_by_index(1);
    if(NULL==element){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
        return;
    }
    memset(str,'0',sizeof(str));
    str_len=strlen(argv[0]);
    if(str_len>8){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
        return;
    }
    for(i=8-str_len,j=0;i<8;i++,j++){
        str[i]=argv[0][j];
    }
    if(4!=str_to_hex((const char *)str,4,id)){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
        return;
    }
    id_val=id[0];
    model_id_int+=id_val<<24;
    id_val=id[1];
    model_id_int+=id_val<<16;
    id_val=id[2];
    model_id_int+=id_val<<8;
    id_val=id[3];
    model_id_int+=id_val;
    model_id.company_id = model_id_int>>16;
    if(model_id.company_id)
    {
        model_id.model_id = model_id_int&0xffff;
		model = jmesh_model_get_by_vendor_id(element,model_id.company_id,model_id.model_id);
    }
    else
    {
        model_id.sig_id = model_id_int&0xffff;
	    model_id.model_id = JMESH_MODEL_ID_INVALID;
	    model = jmesh_model_get_by_sig_id(element,model_id.sig_id);
    }
    if(NULL==model){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
    else
    {
        length = jmesh_model_bind_list(model,9, buffer);
        if(length==0){
		JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
		}
		else{
		    for(int j=0;j<length;j+=3)
            {
                if(length-j<3)
                {
                    appkey_index = buffer[j] + ((buffer[j+1]&0x0f)<<8);
                    JMESH_CMD_RES(interface,"model id:0x%08x,appkey index:%d\n",model_id_int, appkey_index);
                    break;
                }
                appkey_index = buffer[j] + ((buffer[j+1]&0x0f)<<8);
                JMESH_CMD_RES(interface,"model id:0x%08x,appkey index:%d\n",model_id_int, appkey_index);
                appkey_index = (buffer[j+1]>>4) + (buffer[j+2]<<4);
                JMESH_CMD_RES(interface,"model id:0x%08x,appkey index:%d\n",model_id_int, appkey_index);
            }
		    JMESH_CMD_RES(interface,"OK\n");
        }
	}
}

static void at_cmd_get_para(unsigned char interface,const char** argv)
{
    jmesh_mac_t mac;
    int pdu_index;
    unsigned short appkey_index=0;
    unsigned char pdu[9]={0};
    unsigned char  key[16] = {0};
    jmesh_appkey_t* jmesh_appkey;
    JMESH_CMD_RES(interface,"addr:%d\n",jmesh_get_primary_addr());
    jmesh_mac_get(mac);
    JMESH_CMD_RES_BUFFER(interface,6,mac,"mac:");
    if(JMESH_STATUS_SUCCESS==jmesh_netkey_state_get(0,key)){
        JMESH_CMD_RES_BUFFER(interface,16,key,"net key:");
    }
    pdu_index=jmesh_appkey_list(0,9,pdu);
    for(int j=0;j<pdu_index;j+=3)
    {
        if(pdu_index-j<3)
        {
            appkey_index = pdu[j] + ((pdu[j+1]&0x0f)<<8);
			jmesh_appkey=jmesh_appkey_get_by_key_index(0,appkey_index);

		    JMESH_CMD_RES_BUFFER(interface,16,jmesh_appkey->key,"appkey index:%d\napp key:",jmesh_appkey->appkey_index);
            break;

        }
            appkey_index = pdu[j] + ((pdu[j+1]&0x0f)<<8);
		    jmesh_appkey=jmesh_appkey_get_by_key_index(0,appkey_index);

            JMESH_CMD_RES_BUFFER(interface,16,jmesh_appkey->key,"appkey index:%d\napp key:",jmesh_appkey->appkey_index);

		    appkey_index = (pdu[j+1]>>4) + (pdu[j+2]<<4);
		    jmesh_appkey=jmesh_appkey_get_by_key_index(0,appkey_index);

            JMESH_CMD_RES_BUFFER(interface,16,jmesh_appkey->key,"appkey index:%d\napp key:",jmesh_appkey->appkey_index);

	}

    //jmesh_appkey=jmesh_appkey_get_by_key_index(0,1);
    /*if(NULL!=jmesh_appkey){
        JMESH_CMD_RES_BUFFER(interface,16,jmesh_appkey->key,"app key:");
		JMESH_CMD_RES(interface,"appkey index:",jmesh_appkey->appkey_index);
    }*/
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_add_subscribe(unsigned char interface,const char** argv)
{
    jmesh_element_t* element;
    unsigned long model_id_int=0;
    jmesh_model_id_t model_id;
    jmesh_model_t*  model;
    unsigned short group_addr=0;
    unsigned char str[8];
    int str_len;
    int i,j;
    unsigned char id[4]={0x00};
    unsigned long id_val;
    element=jmesh_element_get_by_index(1);
    if(NULL==element){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
        return;
    }
    memset(str,'0',8);
    str_len=strlen(argv[0]);
    if(str_len>8){
        str_len=8;
    }
    for(i=8-str_len,j=0;i<8;i++,j++){
        str[i]=argv[0][j];
    }
    str_to_hex((const char *)str,4,id);
    id_val=id[0];
    model_id_int+=id_val<<24;
    id_val=id[1];
    model_id_int+=id_val<<16;
    id_val=id[2];
    model_id_int+=id_val<<8;
    id_val=id[3];
    model_id_int+=id_val;
    model_id.company_id = model_id_int>>16;
    if(model_id.company_id)
    {
        model_id.model_id = model_id_int&0xffff;
		    model = jmesh_model_get_by_vendor_id(element,model_id.company_id,model_id.model_id);
    }
    else
    {
        model_id.sig_id  = model_id_int&0xffff;
		    model_id.model_id = JMESH_MODEL_ID_INVALID;
		    model = jmesh_model_get_by_sig_id(element,model_id.sig_id);
    }
    if(NULL==model){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
    else if(JMESH_ADDR_UNICAST_MAX>=(group_addr=strtol(argv[1],NULL,0))){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
    }
    else if(JMESH_STATUS_SUCCESS==jmesh_model_subscription_list_add(model,group_addr))
    {
        jmesh_subscription_list_add_filter(&model->para.subscription_list);
        JMESH_CMD_RES(interface,"OK\n");
    }
    else
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
}

static void at_cmd_del_subscribe(unsigned char interface,const char** argv)
{
    jmesh_element_t* element;
    unsigned long model_id_int=0;
    jmesh_model_id_t model_id;
    jmesh_model_t*  model;
    unsigned short group_addr=0;
    unsigned char str[8];
    int str_len;
    int i,j;
    unsigned char id[4]={0x00};
    unsigned long id_val;
    element=jmesh_element_get_by_index(1);
    if(NULL==element){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
        return;
    }
    memset(str,'0',8);
    str_len=strlen(argv[0]);
    if(str_len>8){
        str_len=8;
    }
    for(i=8-str_len,j=0;i<8;i++,j++){
        str[i]=argv[0][j];
    }
    str_to_hex((const char *)str,4,id);
    id_val=id[0];
    model_id_int+=id_val<<24;
    id_val=id[1];
    model_id_int+=id_val<<16;
    id_val=id[2];
    model_id_int+=id_val<<8;
    id_val=id[3];
    model_id_int+=id_val;
    model_id.company_id = model_id_int>>16;
    if(model_id.company_id)
    {
        model_id.model_id = model_id_int&0xffff;
		    model = jmesh_model_get_by_vendor_id(element,model_id.company_id,model_id.model_id);
    }
    else
    {
        model_id.sig_id  = model_id_int&0xffff;
		    model_id.model_id = JMESH_MODEL_ID_INVALID;
		    model = jmesh_model_get_by_sig_id(element,model_id.sig_id);
    }
    if(NULL==model){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
    else if(JMESH_ADDR_UNICAST_MAX>=(group_addr=strtol(argv[1],NULL,0))){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
    }
    else if(JMESH_STATUS_SUCCESS==jmesh_model_subscription_list_remove(model,group_addr))
    {
        jmesh_subscription_list_add_filter(&model->para.subscription_list);
        JMESH_CMD_RES(interface,"OK\n");
    }
    else
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
}

static void at_cmd_get_subscribe(unsigned char interface,const char** argv)
{
    jmesh_element_t* element;
    unsigned long model_id_int=0;
    jmesh_model_id_t model_id;
    jmesh_model_t*  model;
    unsigned char buffer[JMESH_SUBSCRIPTION_LIST_SIZE*2] = {0};
    int index;
    unsigned short addr;
    unsigned char str[8];
    int str_len;
    int i,j;
    unsigned char id[4]={0x00};
    unsigned long id_val;
    element=jmesh_element_get_by_index(1);
    if(NULL==element){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
        return;
    }
    memset(str,'0',8);
    str_len=strlen(argv[0]);
    if(str_len>8){
        str_len=8;
    }
    for(i=8-str_len,j=0;i<8;i++,j++){
        str[i]=argv[0][j];
    }
    str_to_hex((const char *)str,4,id);
    id_val=id[0];
    model_id_int+=id_val<<24;
    id_val=id[1];
    model_id_int+=id_val<<16;
    id_val=id[2];
    model_id_int+=id_val<<8;
    id_val=id[3];
    model_id_int+=id_val;
    model_id.company_id = model_id_int>>16;
    if(model_id.company_id)
    {
        model_id.model_id = model_id_int&0xffff;
		    model = jmesh_model_get_by_vendor_id(element,model_id.company_id,model_id.model_id);
    }
    else
    {
        model_id.sig_id  = model_id_int&0xffff;
		    model_id.model_id = JMESH_MODEL_ID_INVALID;
		    model = jmesh_model_get_by_sig_id(element,model_id.sig_id);
    }
    if(NULL==model){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
        return;
    }
    index=jmesh_model_subscription_list(model,buffer);
    for(int i=0;i<index;i+=2){
        JMESH_LITTLE_ENDIAN_PICK2(addr,buffer+i);
        JMESH_CMD_RES(interface,"addr:0x%x\n",addr);
    }
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_send_hex(unsigned char interface,const char** argv)
{
    unsigned char buff[256]={0x00};
    unsigned short addr;
    unsigned short length;
    addr = strtol(argv[0],NULL,0);
    length=strlen(argv[1]);
    length=length/2+length%2;
    if(length>sizeof(buff))
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
    }
		if(length!=str_to_hex(argv[1],length,buff)){
		    JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
		}
    jmesh_app_expand2_transparent_client_trans(addr+1,0,length,buff);
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_send_meter_cmd(unsigned char interface,const char** argv)
{
    unsigned char buff[40]={0x00};
    unsigned short addr;
    unsigned short length;
    unsigned short offset;
    addr = strtol(argv[0],NULL,0);
    length=strlen(argv[1]);
    length=length/2+length%2;
    if(length>sizeof(buff))
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
    }
		if(length!=str_to_hex(argv[1],length,buff)){
		    JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
		}
		if(protocol_64507_pick(length,buff,&offset)){
		    meter_client_cmd_send(addr,0,1,length,buff);
        JMESH_CMD_RES(interface,"OK\n");
		}
		else{
		    JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
		}


}

static void at_cmd_send_trans(unsigned char interface,const char** argv)
{
    unsigned short addr=strtol(argv[0],NULL,0);
    unsigned short length=strlen(argv[1]);

    if(length>256)
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
    }
    jmesh_app_expand2_transparent_client_trans(addr+1,0,length,(unsigned char *)argv[1]);
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_get_neighbor(unsigned char interface,const char** argv)
{
    int link_num;
    for(int i=0;i<JMESH_ROUTING_NEIGHBOR_SIZE;i++)
    {
        if(routing_neighbor_table_p[i].addr)
        {
            link_num=jmesh_routing_get_neighbor_link_num(routing_neighbor_table_p+i);
            JMESH_CMD_RES(interface,"%d,addr:%d,lifetime:%d,rssi:%d,link num:%d,r:%d,p:%d,f:%d,l:%d\n",
                i,routing_neighbor_table_p[i].addr,routing_neighbor_table_p[i].lifetime,routing_neighbor_table_p[i].rssi-256,link_num,
                ((jmesh_features_t*)&routing_neighbor_table_p[i].features)->relay,((jmesh_features_t*)&routing_neighbor_table_p[i].features)->proxy,((jmesh_features_t*)&routing_neighbor_table_p[i].features)->friend,((jmesh_features_t*)&routing_neighbor_table_p[i].features)->lowpower);
        }
    }
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_get_connected(unsigned char interface,const char** argv)
{
    jmesh_gatt_t* gatt;
    for(int i=0;i<JMESH_GATT_SIZE;i++)
    {
        gatt=jmesh_gatt_get_link_by_interface(i);
        if(NULL!=gatt)
        {
            JMESH_CMD_RES(interface,"index:%d,addr:%d,interface:%d,r:%d,p:%d,f:%d,l:%d\n",i,gatt->addr,gatt->id,
                          ((jmesh_features_t*)&gatt->features)->relay,((jmesh_features_t*)&gatt->features)->proxy,((jmesh_features_t*)&gatt->features)->friend,((jmesh_features_t*)&gatt->features)->lowpower);
        }
    }
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_get_route(unsigned char interface,const char** argv)
{
    for(int i=0;i<JMESH_ROUTE_TABLE_SIZE;i++)
    {
        if(p_route_table[i].lifetime)
        {
            JMESH_CMD_RES(interface,"addr:%d,lifetime:%d,id:%d,distance:%d\n",(i/JMESH_ROUTE_NODE_RECORD_NUM)*JMESH_ROUTE_NODE_ELEMENT_NUM+3,p_route_table[i].lifetime,p_route_table[i].id,p_route_table[i].distance);
        }
    }
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_set_iv_index(unsigned char interface,const char** argv)
{
    unsigned short netkey_idx;
    unsigned long iv_idx;
    netkey_idx = strtol(argv[0],NULL,0);
    iv_idx = strtol(argv[1],NULL,0);
    if(0==jmesh_netkey_test_set_iv_index(netkey_idx,iv_idx))
    {
        JMESH_CMD_RES(interface,"OK\n");
    }
    else
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
}

static void at_cmd_get_iv_index(unsigned char interface,const char** argv)
{
    unsigned short netkey_idx;
    unsigned long iv_idx;
    netkey_idx = strtol(argv[0],NULL,0);
    iv_idx=jmesh_netkey_get_iv_index(netkey_idx);
    if(JMESH_INVALID_IV_INDEX!=iv_idx)
    {
        JMESH_CMD_RES(interface,"iv index:0x%x\n",iv_idx);
        JMESH_CMD_RES(interface,"OK\n");
    }
    else
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
}

static void at_cmd_update_iv_index(unsigned char interface,const char** argv)
{
    unsigned short netkey_idx;
    unsigned int flag;
    int ret;
    netkey_idx = strtol(argv[0],NULL,0);
    flag = strtol(argv[1],NULL,0);
    if(1==flag)
    {
        ret=jmesh_netkey_test_start_update_iv_index(netkey_idx);
    }
    else if(0==flag)
    {
        ret=jmesh_netkey_test_stop_update_iv_index(netkey_idx);
    }
    else
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
    }
    if(0==ret)
    {
        JMESH_CMD_RES(interface,"OK\n");
    }
    else
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_EXC_FAILURE);
    }
}

static void at_cmd_set_light_mode(unsigned char interface,const char** argv)
{
    unsigned char id,direction;
    unsigned short speed_ms,on_len,cycle_len;
    unsigned short on_ticks,off_ticks,speed_ticks;
    id=strtol(argv[0],NULL,0);
    speed_ms=strtol(argv[1],NULL,0);
    on_len=strtol(argv[2],NULL,0);
    cycle_len=strtol(argv[3],NULL,0);
    direction=strtol(argv[4],NULL,0);
    speed_ticks=os_time_ms_to_ticks(speed_ms);
    on_ticks=2*speed_ticks*on_len;
    off_ticks=2*speed_ticks*(cycle_len-on_len);
    light_client_mode_set(0xffff,id,speed_ticks,on_ticks,off_ticks,0,direction);
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_light_mode(unsigned char interface,const char** argv)
{
    unsigned short addr;
    unsigned char id;
    addr=strtol(argv[0],NULL,0);
    id=strtol(argv[1],NULL,0);
    light_client_mode(addr+1,id);
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_set_feature(unsigned char interface,const char** argv)
{
    unsigned char r_feature=0;
    unsigned char p_feature=0;
    unsigned char f_feature=0;
    unsigned char l_feature=0;
    r_feature=strtol(argv[0],NULL,0);
    if(r_feature>1){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA1_INVALID);
        return;
    }
    p_feature=strtol(argv[1],NULL,0);
    if(p_feature>1){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
    }
    f_feature=strtol(argv[2],NULL,0);
    if(f_feature>1){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA3_INVALID);
        return;
    }
    l_feature=strtol(argv[3],NULL,0);
    if(l_feature>1||(f_feature&l_feature)){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA4_INVALID);
        return;
    }
    jmesh_set_relay_feature(r_feature);
    jmesh_set_proxy_feature(p_feature);
    jmesh_set_friend_feature(f_feature);
    jmesh_interface_init();
    if(JMESH_FEATURE_ENABLE==l_feature){
        //jmesh_routing_neighbor_uninit();
        //jmesh_routing_beacon_stop();
        //jmesh_secure_beacon_stop();
        jmesh_set_lowpower_feature(l_feature);
        jmesh_ble_scan_disable();
        jmesh_lowpower_init();
        jmesh_gatt_disconnect_all();
    }
    else{
        if(JMESH_FEATURE_ENABLE==jmesh_get_lowpower_feature()){
            jmesh_set_lowpower_feature(l_feature);
            jmesh_gatt_init();
            jmesh_ble_scan_enable();
            jmesh_routing_neighbor_init();
            jmesh_lowpower_uninit();
            jmesh_routing_beacon_start();
            jmesh_route_init();
        }
        //jmesh_secure_beacon_start();

    }
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_get_feature(unsigned char interface,const char** argv)
{
    unsigned char relay_f;
    unsigned char proxy_f;
    unsigned char friend_f;
    unsigned char lowpower_f;
    relay_f=jmesh_get_relay_feature();
    proxy_f=jmesh_get_proxy_feature();
    friend_f=jmesh_get_friend_feature();
    lowpower_f=jmesh_get_lowpower_feature();
    JMESH_CMD_RES(interface,"relay:%d,proxy:%d,friend:%d,low power:%d\n",relay_f,proxy_f,friend_f,lowpower_f);
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_set_dBm(unsigned char interface,const char** argv)
{
    unsigned char dbm;
    dbm=strtol(argv[0],NULL,0);
    if(0==jmesh_ble_set_dbm(dbm)){
        JMESH_CMD_RES(interface,"OK\n");
        if(JMESH_CMD_INTERFACE_UART==interface)
        {
            wait_uart_send_over();
        }
        jmesh_ble_platform_reset();
    }
    else{
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
    }
}

static void at_cmd_get_dBm(unsigned char interface,const char** argv)
{
    unsigned char dbm;
    dbm=jmesh_ble_get_dbm();
    JMESH_CMD_RES(interface,"%d\n",dbm);
    JMESH_CMD_RES(interface,"OK\n");
}

static os_timer_event_t ota_adv_timer;
static unsigned int ota_adv_times=0;
#define JMESH_OTA_ADV_INTERVAL_MIN_MS 2000
#define JMESH_OTA_ADV_INTERVAL_MAX_MS 4000
#define JMESH_OTA_ADV_MAX_TIMES 60
void jmesh_ota_adv_schedule(void){
    if(ota_adv_times>=JMESH_OTA_ADV_MAX_TIMES){
        os_timer_event_remove(&ota_adv_timer);
		    jmesh_secure_beacon_start();
    }
    else{
        ota_adv_times++;
        jmesh_adv_send(JMESH_ADV_TYPE_OTA,1,NULL,JMESH_ADV_REPEAT_NUM);
        os_timer_event_set(&ota_adv_timer,jmesh_random_get(JMESH_OTA_ADV_INTERVAL_MIN_MS,JMESH_OTA_ADV_INTERVAL_MAX_MS),(os_timer_event_caller_t)jmesh_ota_adv_schedule,NULL);
    }
}

static void at_cmd_start_ota(unsigned char interface,const char** argv)
{
	  jmesh_secure_beacon_stop();
    ota_adv_times=0;
    os_timer_event_set(&ota_adv_timer,jmesh_random_get(JMESH_OTA_ADV_INTERVAL_MIN_MS,JMESH_OTA_ADV_INTERVAL_MAX_MS),(os_timer_event_caller_t)jmesh_ota_adv_schedule,NULL);
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_exit_ota(unsigned char interface,const char** argv)
{
	  jmesh_secure_beacon_start();
    ota_adv_times=JMESH_OTA_ADV_MAX_TIMES;
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_start_pwm(unsigned char interface,const char** argv)
{
    unsigned char pin_num;
    unsigned char channel;
    unsigned short high_ms;
    unsigned short low_ms;
	unsigned short duty_ratio;
	unsigned short period_time;
	char *duty_ratio_str;
    pin_num=strtol(argv[0],NULL,0);
    channel=strtol(argv[1],NULL,0);
    //high_ms=strtol(argv[2],NULL,0);
    //low_ms=strtol(argv[3],NULL,0);
	period_time=strtol(argv[2],NULL,0);
	if(pin_num>5){
	    JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA1_INVALID);
		return ;
	}
	if(NULL==(duty_ratio_str=strstr(argv[3],"%"))){
	    JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA3_INVALID);
		return ;
	}
	*duty_ratio_str=0;
	duty_ratio=strtol(argv[3],NULL,0);
	if(duty_ratio>100){
	    JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA3_INVALID);
		return ;
    }
	high_ms=period_time*duty_ratio*0.01;
	low_ms=period_time-high_ms;
    if(channel>4){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
    }
    jmesh_pwm_start((jmesh_pwm_pin_num_t)pin_num,(jmesh_pwm_channel_t)channel,high_ms,low_ms);
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_end_pwm(unsigned char interface,const char** argv)
{
    unsigned char channel;
    channel=strtol(argv[0],NULL,0);
    if(channel>4){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
        return;
    }
    jmesh_pwm_stop((jmesh_pwm_channel_t)channel);
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_set_gpio(unsigned char interface,const char** argv)
{
    unsigned char pin_num;
    unsigned char value;
    JMESH_GPIO_NAME_T gpio;
    pin_num=strtol(argv[0],NULL,0);
    switch(pin_num)
    {
        case 4:
            gpio=JMESH_LED1;
            break;
        case 1:
            gpio=JMESH_LED2;
            break;
        case 2:
            gpio=JMESH_LED_GREEN;
            break;
        case 3:
            gpio=JMESH_LED_RED;
            break;
        case 5:
            gpio=JMESH_OUTPUT_5;
            break;
        case 6:
            gpio=JMESH_INPUT_6;
            break;
        case 7:
            gpio=POWER_LIGHT;
            break;
        default:
            JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA1_INVALID);
            return;
    }
    value=strtol(argv[1],NULL,0);
    if(value>1){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
    }
    jmesh_gpio_set(gpio,value);
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_echo(unsigned char interface,const char** argv)
{
    if (0==strcmp ("ON",argv[0]))
    {    JMESH_CMD_RES (interface,"ECHO ON\nOK\n");
		 echo_flag=1;
	}
	else if(0==strcmp ("OFF",argv[0]))
    {    JMESH_CMD_RES (interface,"ECHO OFF\nOK\n");
	     echo_flag=0;
    }
	else {
	     JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
	}
}
static void at_cmd_iowrite(unsigned char interface,const char** argv)
{
    unsigned char pin_num;
    unsigned char value;
    JMESH_GPIO_NAME_T gpio;
    pin_num=strtol(argv[0],NULL,0);
    switch(pin_num)
    {
        case 4:
            gpio=JMESH_LED1;
            break;
        case 1:
            gpio=JMESH_LED2;
            break;
        case 2:
            gpio=JMESH_LED_GREEN;
            break;
        case 3:
            gpio=JMESH_LED_RED;
            break;
        case 5:
            gpio=JMESH_OUTPUT_5;
            break;
        case 6:
            gpio=JMESH_INPUT_6;
            break;
        case 7:
            gpio=POWER_LIGHT;
            break;
        default:
            JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA1_INVALID);
            return;
    }
    value=strtol(argv[1],NULL,0);
    if(value>1){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
    }
    jmesh_gpio_set(gpio,value);
    JMESH_CMD_RES(interface,"OK\n");
}
static void at_cmd_ioread(unsigned char interface,const char** argv)
{
    unsigned char pin_num;
    int io_get_value;
    JMESH_GPIO_NAME_T gpio;
    pin_num=strtol(argv[0],NULL,0);
    switch(pin_num)
    {
        case 4:
            gpio=JMESH_LED1;
            break;
        case 1:
            gpio=JMESH_LED2;
            break;
        case 2:
            gpio=JMESH_LED_GREEN;
            break;
        case 3:
            gpio=JMESH_LED_RED;
            break;
        case 5:
            gpio=JMESH_OUTPUT_5;
            break;
        case 6:
            gpio=JMESH_INPUT_6;
            break;
        case 7:
            gpio=POWER_LIGHT;
            break;
        default:
            JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA1_INVALID);
            return;
    }
    io_get_value=jmesh_gpio_get(gpio);
    JMESH_CMD_RES(interface,"%d\nOK\n",io_get_value);
}
static void at_cmd_iopull(unsigned char interface,const char** argv)
{
    unsigned char pin_num;
	unsigned char pull_num;
    io_pull_type_t pull;
    JMESH_GPIO_NAME_T gpio;
    pin_num=strtol(argv[0],NULL,0);
    switch(pin_num)
    {
        case 4:
            gpio=JMESH_LED1;
            break;
        case 1:
            gpio=JMESH_LED2;
            break;
        case 2:
            gpio=JMESH_LED_GREEN;
            break;
        case 3:
            gpio=JMESH_LED_RED;
            break;
        case 5:
            gpio=JMESH_OUTPUT_5;
            break;
        case 6:
            gpio=JMESH_INPUT_6;
            break;
        case 7:
            gpio=POWER_LIGHT;
            break;
        default:
            JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA1_INVALID);
            return;
    }
    pull_num=strtol(argv[1],NULL,0);
    switch(pull_num)
	{
	    case 0:
            pull=IO_PULL_DISABLE;
		    break;
        case 1:
            pull=IO_PULL_UP;
		    break;
        case 2:
            pull=IO_PULL_DOWN;
		    break;
        default:
			JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
            return;
	}
    jmesh_gpio_pull_write(gpio,pull);
    JMESH_CMD_RES(interface,"OK\n");
}
static void at_cmd_get_pwm(unsigned char interface,const char** argv)
{
    unsigned char pin_num;
    unsigned char j;
    unsigned short high_ms;
    unsigned short low_ms;
    unsigned short duty_ratio;
	unsigned short period_time;
	unsigned short i=0;
	unsigned short channel;
	for(j=0;j<5;j++){
        if(1==jmesh_pwm_list(&pin_num,(jmesh_pwm_channel_t)j,&high_ms,&low_ms,&channel)){
			period_time=high_ms+low_ms;
			duty_ratio=(100*high_ms)/period_time;
		    JMESH_CMD_RES(interface,"pin num:%d,channel:%d,period time:%dms,duty ratio:%d%%\n",pin_num,channel,period_time,duty_ratio);
            i++;
		}

    }
    if(i!=0){
	    JMESH_CMD_RES(interface,"OK\n");
	}
	else{
        JMESH_CMD_RES(interface,"pwm not set\n");
	}
}
static void at_cmd_send_ascii(unsigned char interface,const char** argv)
{
	unsigned short addr=strtol(argv[0],NULL,0);
    unsigned short length=strlen(argv[1]);
    if(length>256)
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
    }
    jmesh_app_expand2_realcomm_client_trans(addr+1,0,length,(unsigned char *)argv[1]);
    JMESH_CMD_RES(interface,"OK\n");



}

static void at_cmd_water_set_type(unsigned char interface,const char** argv)
{
    unsigned short addr=strtol(argv[0],NULL,0);
    unsigned char type=strtol(argv[1],NULL,0);
    if(type>5){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
    }
    JMESH_CMD_RES(interface,"OK\n");
    water_client_set_type(addr+1,type);
}

static void at_cmd_water_get_type(unsigned char interface,const char** argv)
{
    JMESH_CMD_RES(interface,"OK\n");
    water_client_get_type(strtol(argv[0],NULL,0)+1);
}

static void at_cmd_water_set_val_pin(unsigned char interface,const char** argv)
{
    JMESH_CMD_RES(interface,"OK\n");
    water_client_set_valve_pin(strtol(argv[0],NULL,0)+1,strtol(argv[1],NULL,0));
}
static void at_cmd_water_get_val_pin(unsigned char interface,const char** argv)
{
    JMESH_CMD_RES(interface,"OK\n");
    water_client_get_valve_pin(strtol(argv[0],NULL,0)+1);
}

static void at_cmd_water_set_unit(unsigned char interface,const char** argv)
{
    JMESH_CMD_RES(interface,"OK\n");
    water_client_set_value_unit(strtol(argv[0],NULL,0)+1,strtol(argv[1],NULL,0));
}

static void at_cmd_water_get_unit(unsigned char interface,const char** argv)
{
    JMESH_CMD_RES(interface,"OK\n");
    water_client_get_value_unit(strtol(argv[0],NULL,0)+1);
}

static void at_cmd_water_set_pul_pin(unsigned char interface,const char** argv)
{
    JMESH_CMD_RES(interface,"OK\n");
    water_client_set_pulse_pin(strtol(argv[0],NULL,0)+1,strtol(argv[1],NULL,0),strtol(argv[2],NULL,0),strtol(argv[3],NULL,0));
}

static void at_cmd_water_get_pul_pin(unsigned char interface,const char** argv)
{
    JMESH_CMD_RES(interface,"OK\n");
    water_client_get_pulse_pin(strtol(argv[0],NULL,0)+1);
}

static void at_cmd_water_get_value(unsigned char interface,const char** argv)
{
    unsigned short addr;
    addr=strtol(argv[0],NULL,0);
    JMESH_CMD_RES(interface,"OK\n");
    water_client_read_value(addr+1);
}

static void at_cmd_water_set_value(unsigned char interface,const char** argv)
{
    unsigned short addr;
    unsigned long value=0;
    unsigned char unit;
    addr=strtol(argv[0],NULL,0);
    value=strtol(argv[1],NULL,0);
    unit=strtol(argv[2],NULL,0);
    JMESH_CMD_RES(interface,"OK\n");
    water_client_write_value(addr+1,value,unit);
}

static void at_cmd_water_set_valve(unsigned char interface,const char** argv)
{
    unsigned short addr;
    unsigned char status;
    addr=strtol(argv[0],NULL,0);
    status=strtol(argv[1],NULL,0);
    JMESH_CMD_RES(interface,"OK\n");
    water_client_set_valve(addr+1,status);
}

static void at_cmd_water_get_valve(unsigned char interface,const char** argv)
{
    unsigned short addr;
    addr=strtol(argv[0],NULL,0);
    JMESH_CMD_RES(interface,"OK\n");
    water_client_get_valve(addr+1);
}

static void at_cmd_water_set_param(unsigned char interface,const char** argv)
{
    unsigned short addr;
    water_param_t param;
    addr=strtol(argv[0],NULL,0);
    param.type=strtol(argv[1],NULL,0);
    param.valve_pin=strtol(argv[2],NULL,0);
    param.report_intv_s=strtol(argv[3],NULL,0);
    param.report_dst=strtol(argv[4],NULL,0);
    param.unit=strtol(argv[5],NULL,0);
    param.pulse_a=strtol(argv[6],NULL,0);
    param.pulse_b=strtol(argv[7],NULL,0);
    param.pulse_c=strtol(argv[8],NULL,0);
    /*if(16!=strlen(argv[9])){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
        return;
    }
    str_to_hex(argv[9],8,param.mac);*/
    JMESH_CMD_RES(interface,"OK\n");
    water_client_set_param(addr+1,&param);
}

static void at_cmd_water_get_param(unsigned char interface,const char** argv)
{
    unsigned short addr;
    addr=strtol(argv[0],NULL,0);
    JMESH_CMD_RES(interface,"OK\n");
    water_client_read_param(addr+1);
}

static void at_cmd_water_set_mac(unsigned char interface,const char** argv)
{
    unsigned short addr;
    unsigned char mac[8];
    addr=strtol(argv[0],NULL,0);
    if(16!=strlen(argv[1])){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
    }
    str_to_hex(argv[1],8,mac);
    water_client_set_mac(addr+1,mac);
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_water_get_mac(unsigned char interface,const char** argv)
{
    unsigned short addr;
    addr=strtol(argv[0],NULL,0);
    water_client_read_mac(addr+1);
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_water_set_report(unsigned char interface,const char** argv)
{
    JMESH_CMD_RES(interface,"OK\n");
    water_client_set_report_para(strtol(argv[0],NULL,0)+1,strtol(argv[1],NULL,0),strtol(argv[2],NULL,0));
}

static void at_cmd_water_get_report(unsigned char interface,const char** argv)
{
    JMESH_CMD_RES(interface,"OK\n");
    water_client_get_report_para(strtol(argv[0],NULL,0)+1);
}

static void at_cmd_gate_set_param(unsigned char interface,const char** argv)
{
    water_gate_param_t param;
    param.record_interval_min=strtol(argv[0],NULL,0);
    param.recv_interval_min=strtol(argv[1],NULL,0);
    param.recv_window_sec=strtol(argv[2],NULL,0);
    water_gate_set_param(&param);
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_gate_get_param(unsigned char interface,const char** argv)
{
    water_gate_param_t param;
    water_gate_get_param(&param);
    JMESH_CMD_RES(interface,"OK\n");
    JMESH_CMD_RES(interface,"%d,%d,%d\n",param.record_interval_min,param.recv_interval_min,param.recv_window_sec);
}

static void at_cmd_gate_list_node(unsigned char interface,const char** argv)
{
    unsigned char buffer[9*WATER_NODE_SIZE];
    int len;
    len=water_gate_list_node(sizeof(buffer),buffer);
    JMESH_CMD_RES(interface,"OK\n");
    JMESH_CMD_RES_BUFFER(interface,len,buffer,"");
}

static void at_cmd_gate_val_by_addr(unsigned char interface,const char** argv)
{
    water_record_t *water_record;
    water_record=water_gate_get_by_addr(strtol(argv[0],NULL,0));
    if(NULL!=water_record){
        JMESH_CMD_RES(interface,"OK\n");
        JMESH_CMD_RES(interface,"%d,%d,0x%02x\n",water_record->value,water_record->unit,water_record->status);
    }
    else{
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARSE_FAILURE);
    }
}

static void at_cmd_gate_val_by_mac(unsigned char interface,const char** argv)
{
    if(14!=strlen(argv[0]))
    {
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA_INVALID);
    }
    else
    {
        unsigned char mac[7]={0x00};
        water_record_t *water_record;
        str_to_hex(argv[0],7,mac);
        water_record=water_gate_get_by_mac(mac);
        if(NULL!=water_record){
            JMESH_CMD_RES(interface,"OK\n");
            JMESH_CMD_RES(interface,"%d,%d,0x%02x\n",water_record->value,water_record->unit,water_record->status);
        }
        else{
            JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARSE_FAILURE);
        }
    }
}

static void at_cmd_gate_get_history(unsigned char interface,const char** argv)
{
    water_record_t *water_record;
    water_record=water_gate_get_history(strtol(argv[0],NULL,0),strtol(argv[1],NULL,0));
    if(NULL!=water_record){
        JMESH_CMD_RES(interface,"OK\n");
        JMESH_CMD_RES(interface,"%d,%d,0x%02x\n",water_record->value,water_record->unit,water_record->status);
    }
    else{
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARSE_FAILURE);
    }
}

static void at_cmd_gate_list_history(unsigned char interface,const char** argv)
{
    unsigned long value;
    unsigned char buffer[100];
    value=water_gate_list_history(strtol(argv[0],NULL,0),strtol(argv[1],NULL,0),sizeof(buffer),buffer);
    //JMESH_CMD_RES(interface,"value:%d\n",value);
    JMESH_CMD_RES(interface,"OK\n");
    JMESH_CMD_RES_BUFFER(interface,sizeof(buffer),buffer,"");
}

static void at_cmd_set_role(unsigned char interface,const char** argv)
{
    jmesh_device_role_t role;
    role.water=strtol(argv[0],NULL,0);
    role.gate=strtol(argv[1],NULL,0);
    /*if(role.water){
        water_init();
        water_server_init();
        water_client_init();
    }
    if(role.gate){
        jmesh_device_role_t last_role;
        last_role.roles=0;
        jmesh_save_read(JMESH_SAVE_ROLE,sizeof(jmesh_device_role_t),(unsigned char *)&last_role);
        if(last_role.water){
            water_uninit();
        }
        water_gate_init();
    }
    jmesh_save_write(JMESH_SAVE_ROLE,sizeof(jmesh_device_role_t),(unsigned char *)&role);*/
    jmesh_set_device_roles(role);
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_get_role(unsigned char interface,const char** argv)
{
    jmesh_device_role_t role;
    if(sizeof(jmesh_device_role_t)!=jmesh_save_read(JMESH_SAVE_ROLE,sizeof(jmesh_device_role_t),(unsigned char *)&role)){
        role.roles=0;
    }
    JMESH_CMD_RES(interface,"water:%d,gate:%d\n",role.water,role.gate);
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_set_lowpower_para(unsigned char interface,const char** argv)
{
    lowpower_param_t param;
    param.adv_repeats=strtol(argv[0],NULL,0);
    if(0==param.adv_repeats){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA1_INVALID);
        return;
    }
    param.poll_intval_s=strtol(argv[1],NULL,0);
    if(param.poll_intval_s<10){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
    }
    param.poll_retry_times=strtol(argv[2],NULL,0);
    param.reqst_intval_s=strtol(argv[3],NULL,0);
    if(0==param.reqst_intval_s){
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA4_INVALID);
        return;
    }
    jmesh_save_write(JMESH_SAVE_LOWPOWER_PARAM,sizeof(lowpower_param_t),(unsigned char *)&param);
    JMESH_CMD_RES(interface,"OK\n");
}

static void at_cmd_get_lowpower_para(unsigned char interface,const char** argv)
{
    JMESH_CMD_RES(interface,"adv repeats:%d,poll interval:%ds,poll retry times:%d,request interval:%ds\n",
                  lowpower_param.adv_repeats,lowpower_param.poll_intval_s,lowpower_param.poll_retry_times,lowpower_param.reqst_intval_s);
    JMESH_CMD_RES(interface,"OK\n");

}

extern void jmesh_pdu_log(void);
static void at_cmd_pdu_log(unsigned char interface,const char** argv)
{

	jmesh_pdu_log();
    JMESH_CMD_RES (interface,"LOG ON\nOK\n");

}
extern void jmesh_get_timer_list(void); //for test
static void at_cmd_get_timer(unsigned char interface,const char** argv)
{
    jmesh_get_timer_list();
    JMESH_CMD_RES (interface,"OK\n");
}
extern void jmesh_gatt_rd_table_print(void);
static void at_cmd_get_con_rd(unsigned char interface,const char** argv)
{
    jmesh_gatt_rd_table_print();
    JMESH_CMD_RES (interface,"OK\n");
}

static void at_cmd_test_app_start(unsigned char interface,const char** argv)//test
{
    unsigned short addr,dst,intv_s;
    unsigned char op_type;
    addr=strtol(argv[0],NULL,0);
    if(0==strcmp(argv[1],"R")){
        op_type=0;
    }
    else if(0==strcmp(argv[1],"W")){
        op_type=1;
    }
    else{
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
    }
    dst=strtol(argv[2],NULL,0);
    intv_s=strtol(argv[3],NULL,0);
    JMESH_CMD_RES (interface,"OK\n");
    test_client_start(addr,op_type,dst,intv_s);


}

static void at_cmd_test_app_stop(unsigned char interface,const char** argv)//test
{
    unsigned char op_type;
    unsigned short addr;
    addr=strtol(argv[0],NULL,0);
    if(0==strcmp(argv[1],"R")){
        op_type=0;
    }
    else if(0==strcmp(argv[1],"W")){
        op_type=1;
    }
    else{
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
    }
    JMESH_CMD_RES (interface,"OK\n");
    test_client_stop(addr,op_type);

}

static void at_cmd_test_app_result(unsigned char interface,const char** argv)//test
{
    unsigned char op_type;
    unsigned short addr;
    addr=strtol(argv[0],NULL,0);
    if(0==strcmp(argv[1],"R")){
        op_type=0;
    }
    else if(0==strcmp(argv[1],"W")){
        op_type=1;
    }
    else{
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
    }
    JMESH_CMD_RES (interface,"OK\n");
    test_client_get_result(addr,op_type);

}

static void at_cmd_test_app_init(unsigned char interface,const char** argv)
{
    unsigned char op_type;
    unsigned short addr;
    addr=strtol(argv[0],NULL,0);
    if(0==strcmp(argv[1],"R")){
        op_type=0;
    }
    else if(0==strcmp(argv[1],"W")){
        op_type=1;
    }
    else{
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
    }
    JMESH_CMD_RES (interface,"OK\n");
    test_client_result_init(addr,op_type);

}

static void at_cmd_light_switch(unsigned char interface,const char** argv)
{
    unsigned char data[1];
    unsigned short addr;
    addr=strtol(argv[0],NULL,0);
    if(0==strcmp(argv[1],"OFF")){
        data[0]=0;
    }
    else if(0==strcmp(argv[1],"ON")){
        data[0]=1;
    }
    else{
        JMESH_CMD_RES(interface,"ERROR:0x%02x\n",ERROR_PARA2_INVALID);
        return;
    }
    JMESH_CMD_RES (interface,"OK\n");
    light_client_point_set(addr,1,data);

}







