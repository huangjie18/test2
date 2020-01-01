#include"jmesh_gatt.h"
#include"jmesh_proxy.h"
#include"jmesh_proxy_config.h"
#include"../../jmesh/jmesh_pdu.h"
#include"../beacon/jmesh_beacon.h"
#include"../../network/jmesh_interface.h"
#include"string.h"
#include"../../jmesh/jmesh_print.h"
#include"../provisioning/jmesh_provisioning_client.h"
#include"../provisioning/jmesh_provisioning_server.h"
#include"../../BLE/jmesh_prov_svc.h"
#include"../../jmesh/jmesh_task.h"
#include"../../driver/jmesh_ble.h"


void jmesh_proxy_send(unsigned char send_type,unsigned char id,unsigned char id_backup,unsigned char proxy_type,unsigned short length,unsigned char *data)
{
    int i,seg_num,seg_len,tail_len;
    unsigned short svc_instance,local_ins=jmesh_ble_get_local_svc_ins();
    unsigned char buff[JMESH_BLE_MTU_SIZE];
    jmesh_proxy_pdu_t *proxy=(jmesh_proxy_pdu_t *)buff;
    unsigned short mtu=jmesh_gatt_mtu_get_by_id(id);
    switch(proxy_type)
    {
         case JMESH_PROXY_TYPE_BEACON:
//            svc_instance = local_ins+JMESHS_IDX_MESH_BEACON_TRANSMIT_VAL;
            break;
        case JMESH_PROXY_TYPE_ROUTING:
        case JMESH_PROXY_TYPE_NETWORK:
            svc_instance = local_ins+JMESHS_IDX_MESH_NETWORK_TRANSMIT_OUT_VAL;
            break;
        case JMESH_PROXY_TYPE_CONFIG:
            svc_instance = local_ins+JMESHS_IDX_MESH_CONFIG_CMD_VAL;//config use ? svc instance
            break;
        case JMESH_PROXY_TYPE_PROVISION:
            svc_instance = local_ins+JMESHS_IDX_MESH_PROVISIONING_DATA_OUT_VAL;
            break;
				case JMESH_PROXY_TYPE_APPDATA:
						svc_instance = local_ins+JMESHS_IDX_MESH_APP_TRANMISSION_DATA_OUT_VAL;
						break;

        default:
            return;
    }
    proxy->type=proxy_type;
    //print_cmd("prx s:%d\n",length);
    if((mtu>=length+2)&&(JMESH_BLE_MTU_SIZE>=length+2)){//proxy format add two byte ahead data: length,head,data.once send mtu-2
        proxy->SAR=JMESH_PROXY_SAR_COMPLETE;
        proxy->length=length+1;
        memcpy(proxy->para,data,length);
        //print_cmd("prx s com:%d\n",proxy->length+1);
        jmesh_adapter_gatt_send(send_type,id,id_backup,svc_instance,proxy->length+1,buff);
        return;
    }
    else if(mtu>2&&mtu<=JMESH_BLE_MTU_SIZE){
        seg_len=mtu-2;
        seg_num=(length + (seg_len-1))/seg_len;
        tail_len=length%seg_len;
        if(tail_len==0){
            tail_len=seg_len;
        }
        proxy->SAR=JMESH_PROXY_SAR_FIRST_SEGMENT;
        proxy->length=seg_len+1;
        memcpy(proxy->para,data,seg_len);
        //print_cmd("prx s fir:%d\n",proxy->length+1);
        jmesh_adapter_gatt_send(send_type,id,id_backup,svc_instance,proxy->length+1,buff);
        proxy->SAR=JMESH_PROXY_SAR_CONTINUE_SEGMENT;
        for(i=1;i<seg_num-1;i++){
            memcpy(proxy->para,data+i*seg_len,seg_len);
            //print_cmd("prx s con:%d\n",proxy->length+1);
            jmesh_adapter_gatt_send(send_type,id,id_backup,svc_instance,proxy->length+1,buff);
        }
        proxy->SAR=JMESH_PROXY_SAR_LAST_SEGMENT;
        proxy->length=tail_len+1;
        memcpy(proxy->para,data+i*seg_len,tail_len);
        //print_cmd("prx s lat:%d\n",proxy->length+1);
        jmesh_adapter_gatt_send(send_type,id,id_backup,svc_instance,proxy->length+1,buff);
    }
}

void jmesh_proxy_recv(unsigned char id,unsigned short instance,unsigned char length,unsigned char* data)
{
    unsigned char type;
    unsigned short local_ins;
    jmesh_pdu_t* pdu=(jmesh_pdu_t*)data;
    static jmesh_pdu_t* recv_cache[10]={NULL};
    if(pdu->proxy.length+1!=length){
        //print_cmd("len error:%d,%d\n",length,pdu->proxy.length);
        return;
    }
    local_ins=jmesh_ble_get_local_svc_ins();
    switch(instance-local_ins){
        case(JMESHS_IDX_MESH_BEACON_IN_VAL):
        case(JMESHS_IDX_MESH_BEACON_OUT_VAL):
            type=JMESH_PROXY_TYPE_BEACON;
            break;
        case(JMESHS_IDX_MESH_NETWORK_TRANSMIT_IN_VAL):
        case(JMESHS_IDX_MESH_NETWORK_TRANSMIT_OUT_VAL):
            type=JMESH_PROXY_TYPE_NETWORK;
            break;
        case(JMESHS_IDX_MESH_PROVISIONING_DATA_IN_VAL):
            type=JMESH_PROXY_TYPE_PROVISION;
            break;
        case(JMESHS_IDX_MESH_CONFIG_CMD_VAL):
            type=JMESH_PROXY_TYPE_CONFIG;
            break;
				case(JMESHS_IDX_MESH_APP_TRANMISSION_DATA_IN_VAL):
            type=JMESH_PROXY_TYPE_APPDATA;
            break;

        default:
            return;
    }
    if(pdu->proxy.type!=type){
        return;
    }
    switch(pdu->proxy.SAR){
        case(JMESH_PROXY_SAR_COMPLETE):
            if(recv_cache[id]!=NULL){
                jmesh_pdu_free(recv_cache[id]);
            }
            if(NULL!=(recv_cache[id]=jmesh_pdu_new(length))){
                memcpy(recv_cache[id]->pdu,data,length);
            }
            else{
                return;
            }
            break;
        case(JMESH_PROXY_SAR_FIRST_SEGMENT):
            if(recv_cache[id]!=NULL){
                jmesh_pdu_free(recv_cache[id]);
            }
            if(NULL!=(recv_cache[id]=jmesh_pdu_new(0xff))){
                memcpy(recv_cache[id]->pdu,data,length);
            }
            return;
        case(JMESH_PROXY_SAR_CONTINUE_SEGMENT):
            if(recv_cache[id]!=NULL){
                if(recv_cache[id]->proxy.type==pdu->proxy.type && recv_cache[id]->length+pdu->proxy.length<=0xff){
                    memcpy(recv_cache[id]->pdu + recv_cache[id]->length+1, data+2, length-2);//
                    recv_cache[id]->length+=pdu->proxy.length-1;
                }
                else{//incomplete continue segment pdu or too long, just delete
                    jmesh_pdu_free(recv_cache[id]);
                    recv_cache[id]=NULL;
                }
            }
            return;
        case(JMESH_PROXY_SAR_LAST_SEGMENT):
            if(recv_cache[id]!=NULL){
                if(recv_cache[id]->proxy.type==pdu->proxy.type && recv_cache[id]->length+pdu->proxy.length<=0xff){
                    memcpy(recv_cache[id]->pdu + recv_cache[id]->length+1, data+2, length-2);//
                    recv_cache[id]->length+=pdu->proxy.length-1;
                    break;
                }
                else{
                    jmesh_pdu_free(recv_cache[id]);
                    recv_cache[id]=NULL;
                }
            }
            return;
        default:
            return;
    }

    switch(recv_cache[id]->proxy.type){
        case JMESH_PROXY_TYPE_ROUTING:
        case JMESH_PROXY_TYPE_NETWORK:
			      recv_cache[id]->length--;
            //print_cmd("intf recv:%d,%d\n",id,recv_cache[id]->length);
            jmesh_interface_recv(id,recv_cache[id]);
            break;
        case JMESH_PROXY_TYPE_BEACON:
			      memcpy(recv_cache[id]->adv.para,data,length);
			      //memcpy(recv_cache[id]->adv.beacon.mac,gatt->mac,6);
			      recv_cache[id]->adv.rssi=0;
			      if(OS_ERR_EVENT_FULL == os_event_post(&jmesh_task,JMESH_EVENT_BEACON_RECV,recv_cache[id]->adv.para))
			      {
                JMESH_LOGE(1,"proxy type beacon","post error\n");
				        jmesh_pdu_free(recv_cache[id]);
			      }
            break;
        case JMESH_PROXY_TYPE_CONFIG:
            recv_cache[id]->length--;
            recv_cache[id]->network.itf=id;
            if(OS_ERR_EVENT_FULL==os_event_post(&jmesh_task,JMESH_EVENT_CONFIG_CMD_RECV,recv_cache[id]))
						{
                JMESH_LOGE(1,"proxy type config","post error\n");
                jmesh_pdu_free(recv_cache[id]);
						}
            break;
        case JMESH_PROXY_TYPE_PROVISION:
						recv_cache[id]->proxy.head = id;
						os_event_post(&jmesh_task,JMESH_EVENT_PROVISION_RECV,recv_cache[id]);
						if(OS_ERR_EVENT_FULL == os_event_post(&jmesh_task,JMESH_EVENT_BEACON_RECV,recv_cache[id]))
						{
                JMESH_LOGE(1,"proxy type provision","post error\n");
							  jmesh_pdu_free(recv_cache[id]);
						}
            break;
				case JMESH_PROXY_TYPE_APPDATA:
						recv_cache[id]->length--;
						recv_cache[id]->network.itf=id;
						if(OS_ERR_EVENT_FULL==os_event_post(&jmesh_task,JMESH_EVENT_APP_DATA_RECV,recv_cache[id]))
						{
								print_error("os_event is full! \n");
								jmesh_pdu_free(recv_cache[id]);
						}
						break;

    }
    recv_cache[id]=NULL;
}


