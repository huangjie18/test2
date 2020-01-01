#include"jmesh_ble_callback.h"
#include"../bearer/adv/jmesh_adv.h"
#include"../bearer/gatt/jmesh_gatt.h"
#include"../jmesh/jmesh_print.h"
#include"../onchip_system/os_timer_event.h"
#include"../jmesh/jmesh_task.h"
#include"../bearer/routing/jmesh_routing_neighbor.h"
#include"../bearer/routing/jmesh_route.h"
#include"limits.h"

unsigned char phone_connect_id ;
unsigned char phone_mac[6] = {0};

int jmesh_ble_connected_handler(jmesh_pdu_t* pdu)
{
    return 0;

//	jmesh_gatt_connected(pdu->ble.connect.mac,pdu->ble.connect.id,pdu->ble.connect.con_intv,pdu->ble.connect.is_server);
//	connected_param[pdu->ble.connect.id].is_server = pdu->ble.connect.is_server;
//	memcpy(connected_param[pdu->ble.connect.id].mac,pdu->ble.connect.mac,6);
//	connected_param[pdu->ble.connect.id].connect_time = os_time_get_seconds();
////	os_timer_event_set(&connect_delay_timer,6000,jmesh_routing_connected,pdu);
//	jmesh_routing_connected(pdu->ble.connect.mac,pdu->ble.connect.id,pdu->ble.connect.is_server);
//	jmesh_pdu_free(pdu);
}
int jmesh_ble_disconnected_handler(jmesh_pdu_t* pdu)
{
    return 0;
// 	jmesh_routing_disconnected(pdu->ble.connect.id);
//	jmesh_gatt_disconnected(pdu->ble.connect.id);

//	connected_param[pdu->ble.connect.id].connect_time = INT_MAX;
////	jmesh_routing_connect(NULL);
////	print_route("disconnected reason: %d\n",pdu->ble.connect.reason);
//	jmesh_pdu_free(pdu);
}
int jmesh_beacon_recv_handler(jmesh_pdu_t* pdu)
{
    jmesh_beacon_handler(pdu->adv.mac,pdu->adv.rssi,&pdu->adv.beacon);
    jmesh_pdu_free(pdu);
    return 0;
}
void jmesh_ble_recv_adv_callback(unsigned char *mac, unsigned char rssi,unsigned char ad_type, unsigned char length,unsigned char *data){
    jmesh_adv_recv(mac,rssi,length,ad_type,data);
}

void jmesh_ble_gatt_connected_callback(unsigned char *mac,unsigned char connect_id,unsigned short con_intv,unsigned char is_server){
    JMESH_LOGI_BUFFER("connected",6,mac,"id:%d,mac:",connect_id);
    if(!jmesh_gatt_is_mesh_device(mac))
    {
        jmesh_gatt_phone_connected(mac,connect_id);
        return;
    }
    jmesh_gatt_connected(mac,connect_id,con_intv,is_server);
}

void jmesh_ble_gatt_disconnected_callback(unsigned char connect_id,unsigned char reason){

    jmesh_gatt_disconnected(connect_id,reason);
    jmesh_gatt_phone_disconnected(connect_id,reason);
}

void jmesh_ble_exchange_mtu_callback(unsigned char connect_id,unsigned short mtu){
    jmesh_gatt_set_mtu(connect_id,mtu);
    jmesh_gatt_phone_set_mtu(connect_id,mtu);
}
void jmesh_ble_disc_svc_callback(unsigned short connect_id,unsigned char *uuid,unsigned short handler){
    jmesh_gatt_set_svc_instance(connect_id,handler);
}
void jmesh_ble_gatt_writed_callback(unsigned char connect_id, unsigned short char_handler, unsigned short length,unsigned char *data){
    jmesh_gatt_recv(connect_id,char_handler,length,data);
}
void jmesh_ble_gatt_notified_callback(unsigned char connect_id, unsigned short char_handler, unsigned short length,unsigned char *data){
    jmesh_gatt_recv(connect_id,char_handler,length,data);
}
