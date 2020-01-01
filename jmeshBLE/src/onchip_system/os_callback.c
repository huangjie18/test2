#include"os_callback.h"
#include"../driver/jmesh_ble.h"
#include"../foundation/config/configuration_cmd.h"
#include"../app/expand/app_expand2/app_expand2_realcomm_client.h"
#include"../app/expand/app_expand1/app_expand1_device_client.h"
#include"../app/expand/app_expand2/light/light_client.h"
#include"../app/expand/app_expand2/light/light_server.h"
#include"../app/expand/app_expand2/test_app/test_app_client.h"
#include"../app/expand/app_expand2/test_app/test_app_server.h"
#include"../app/expand/app_expand2/meter/meter_client.h"
#include"../app/expand/app_expand2/meter/meter_server.h"
#include"../bearer/routing/jmesh_route.h"
#include"../onchip_system/os_timer_event.h"
#include"jmesh_friend.h"
#include"jmesh_lowpower.h"
#if (JMESH_DRIVER_BX2400)
#include"FreeRTOS.h"
#include"task.h"
#elif (JMESH_DRIVER_X86)
#include"windows.h"
#endif
#include"jmesh_app_pro.h"
#include"gap.h"
#include "si4438_hal.h"
#include "_470_phy.h"
#include "_470_app.h"
#include "jmesh_app_pro.h"
#include "jmesh_route.h"
#include "app_config.h"
#include "app_protocol.h"
#include"../jmesh_pwm.h"

void os_start_callback(void){
    jmesh_ble_init();

    //jmesh_wdt_init();
	  #ifdef SET_JMESH_UART_PARITY_NONE
		jmesh_uart_init(JMESH_UART_0, JMESH_UART_BAUD_9600, JMESH_UART_PARITY_NONE,JMESH_UART_STOP_BITS_1);
		#endif
		#ifdef SET_JMESH_UART_PARITY_EVEN
		jmesh_uart_init(JMESH_UART_0, JMESH_UART_BAUD_9600, JMESH_UART_PARITY_EVEN,JMESH_UART_STOP_BITS_1);
		#endif
    //JMESH_LOGI("init","gpio");
    //jmesh_random_init(os_time_ticks());
    jmesh_mac_init();
    jmesh_pdu_init();
    jmesh_addr_init();
    jmesh_TTL_init();
    jmesh_features_init();
//    if(JMESH_FEATURE_ENABLE!=jmesh_get_lowpower_feature()){
//        jmesh_gpio_init(JMESH_LED1,JMESH_GPIO_MODE_OUTPUT);
//        jmesh_gpio_set(JMESH_LED1, GPIO_VALUE_HIGH);
//    }

    jmesh_friendship_init();

    jmesh_lowpower_init();

    jmesh_network_init();
    jmesh_segment_init();
    jmesh_netkey_init();
    jmesh_appkey_init();
    jmesh_device_init();
    jmesh_interface_init();
    jmesh_access_init();
    jmesh_gatt_init();
    jmesh_routing_neighbor_init();
    jmesh_seq_list_init();
    jmesh_TTL_set_default(5);
//		network_light_indicate_init();
    jmesh_rev_adv_timer_start();
		ApsInit();
		//jmesh_pwm_start(4,JMESH_PWM_CHANNEL_0,9,1);
}
int wdt_regist_id;
static os_timer_event_t uart_stop_timer;
extern void io_pin_toggle(uint8_t pin_num);
void uart_stop(void){
    io_pin_toggle(4);
    //jmesh_uart_stop(JMESH_UART_0);
    os_timer_event_set(&uart_stop_timer,5000,(os_timer_event_caller_t)uart_stop,NULL);//test
}
void os_inited_callback(void){
    //unsigned short addr;
    //unsigned char net_key[16]=JMESH_NETKEY;
    //unsigned char app_key[16]=JMESH_APPKEY;
		jmesh_wdt_init();
    wdt_regist_id=jmesh_wdt_sub_regist();

    if(JMESH_FEATURE_ENABLE==jmesh_get_lowpower_feature()){
        //os_timer_event_set(&uart_stop_timer,40000,(os_timer_event_caller_t)uart_stop,NULL);
    }
//    os_timer_event_set(&uart_stop_timer,5000,(os_timer_event_caller_t)uart_stop,NULL);//test
//    if(JMESH_STATUS_SUCCESS!=jmesh_netkey_state_get(0,net_key))
//    {
//        jmesh_netkey_state_set(0,net_key);
//    }
//    if(NULL==jmesh_appkey_get_by_key_index(0,1))
//    {
//        jmesh_appkey_set(0,1,app_key);
//    }
    jmesh_model_bind_app(&app_expand,jmesh_appkey_get_by_key_index(0,1));
    jmesh_model_bind_app(&test_server,jmesh_appkey_get_by_key_index(0,1));
    jmesh_model_bind_app(&test_client,jmesh_appkey_get_by_key_index(0,1));
    jmesh_model_bind_app(&meter_server,jmesh_appkey_get_by_key_index(0,1));
    jmesh_model_bind_app(&meter_client,jmesh_appkey_get_by_key_index(0,1));


    jmesh_routing_beacon_start();
    jmesh_secure_beacon_start();

    jmesh_route_init();



    //os_timer_event_set(&trans_test_timer,jmesh_random_get(3000,5000),(os_timer_event_caller_t)jmesh_trans_test_schedule,NULL);

    //#define info "this is a very long message to send  for test if the code works well for trans long pdu,it's really very very long!!!\n"
    //jmesh_app_expand2_realcomm_client_trans(0xffff,0,sizeof(info),info);
    //addr=jmesh_get_primary_addr();
    //jmesh_app_expand2_realcomm_client_set_baudrate(addr+1,1,2,1);
    //jmesh_app_expand1_device_client_bind_report(8,1,3);
    //jmesh_app_expand1_device_client_bind_report(8,1,4);
    //jmesh_app_expand1_device_client_bind_report(8,2,4);

    //jmesh_app_expand1_device_client_unbind_report(8,1,4);
    //jmesh_app_expand1_device_client_get_info(addr+1);
    jmesh_ble_status_start();
    //_print("ready\n");
}

void os_cycle_callback(void){
    #if (JMESH_DRIVER_BX2400)
    jmesh_wdt_sub_reset(wdt_regist_id);

    int d=0;

    if(os_event_is_null()&&1<=(d=os_time_get_expired())){
        //JMESH_LOGI("s","delay:%d\n",d);
        jmesh_task_sleep(d*(configTICK_RATE_HZ/OS_TICKS_FREQUENCY));
    }
    //JMESH_LOGI("wake up","\n");
    os_time_ticks_trigger();

    #elif (JMESH_DRIVER_X86)
    Sleep(1);

    #endif
}
