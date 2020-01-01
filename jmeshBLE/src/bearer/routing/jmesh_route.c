
#include"stdio.h"
#include"string.h"
#include"jmesh_route.h"
#include"../../onchip_system/os_timer_event.h"
#include"../../upper/control/jmesh_control.h"
#include"../../jmesh/jmesh_addr.h"
#include"../../bearer/gatt/jmesh_proxy.h"
#include"../../network/jmesh_interface.h"
#include"../../access/jmesh_access.h"
#include"../../jmesh/jmesh_print.h"

struct st_jmesh_route_exchange_cache{
    unsigned short start_index;
    unsigned char distance[1];
};

static os_timer_event_t route_exc_timer;

static jmesh_route_t route_table[JMESH_ROUTE_TABLE_SIZE];

jmesh_route_t* p_route_table = route_table;

void jmesh_route_init(void)
{
    int index;
    unsigned short addr;
    memset(route_table,0x00,sizeof(route_table));
    if(JMESH_FEATURE_ENABLE!=jmesh_get_lowpower_feature()){
        os_timer_event_set(&route_exc_timer,1000*JMESH_ROUTE_EXCHANGE_MIN_PERIOD_S,(os_timer_event_caller_t)jmesh_route_exchange,NULL);
    }
    addr=jmesh_get_primary_addr();
    if(addr<3){
        return;
    }
    index=JMESH_ROUTE_NODE_INDEX(addr);
    for(int i=0;i<JMESH_ROUTE_NODE_RECORD_NUM;i++)
    {
        route_table[index+i].distance=0;
        route_table[index+i].lifetime=JMESH_ROUTE_MAX_DISTANCE+1;
        route_table[index+i].id=JMESH_INTERFACE_LOCAL;
    }
}
/*
void jmesh_route_uninit(void)
{
    if(JMESH_FEATURE_ENABLE==jmesh_get_lowpower_feature()){
        os_timer_event_remove(os_timer_event_t* event)

    }

}*/


void jmesh_lowpower_node_route_update(unsigned short src,unsigned char id){
    int index;
    if(src>=3&&src<=JMESH_ROUTE_NODE_ADDR_LIMIT){
        index=JMESH_ROUTE_NODE_INDEX(src);
        route_table[index].id=id;
        route_table[index].distance=1;
        route_table[index].lifetime=JMESH_ROUTE_MAX_DISTANCE;
    }
}

void jmesh_lowpower_node_route_delete(unsigned short src){
    int index;
    if(src>=3&&src<=JMESH_ROUTE_NODE_ADDR_LIMIT){
        index=JMESH_ROUTE_NODE_INDEX(src);
        route_table[index].lifetime=0;
    }
}

void jmesh_route_update(unsigned short src,unsigned char id,unsigned char distance)
{
    int index,i,null_index=-1,max_index=-1;

    if(src<=JMESH_ROUTE_NODE_ADDR_LIMIT){
        index=JMESH_ROUTE_NODE_INDEX(src);
        for(i=0;i<JMESH_ROUTE_NODE_RECORD_NUM;i++){//update
            if(route_table[index+i].lifetime){
                if(route_table[index+i].id == id){
                    route_table[index+i].distance=distance;
                    if(distance<JMESH_ROUTE_MAX_DISTANCE){
                        route_table[index+i].distance+=1;
                        route_table[index+i].lifetime=JMESH_ROUTE_MAX_DISTANCE-route_table[index+i].distance+1;
                        //JMESH_LOGI("up1","src:%d,id:%d,dis:%d,i:%d\n",src,id,route_table[index+i].distance,i);
                    }
                    else{
                        route_table[index+i].lifetime=0;
                        //JMESH_LOGI("invalid","src:%d,id:%d,dis:%d,i:%d\n",src,id,route_table[index+i].distance,i);
                    }
                    return;
                }
            }
        }
        if(distance>=JMESH_ROUTE_MAX_DISTANCE){
            return;
        }
        distance++;
        for(i=0;i<JMESH_ROUTE_NODE_RECORD_NUM;i++){
            if(route_table[index+i].lifetime){
                if(route_table[index+i].distance +1 < distance){//discard
                    return;
                }
                else if(route_table[index+i].distance > distance + 1){//replace
                    route_table[index+i].distance=distance;
                    route_table[index+i].id=id;
                    route_table[index+i].lifetime=JMESH_ROUTE_MAX_DISTANCE-distance+1;
                    //JMESH_LOGI("up2","src:%d,id:%d,dis:%d,i:%d\n",src,id,route_table[index+i].distance,i);
                    return;
                }
                if(max_index==-1){
                    max_index=index+i;
                }
                else if(route_table[max_index].distance < route_table[index+i].distance){
                    max_index=index+i;
                }
            }
            else{
                null_index=index+i;
            }
        }
        if(null_index==-1){
            if(route_table[max_index].distance>distance){
                null_index=max_index;
            }
        }
        if(null_index!=-1){
            route_table[null_index].id=id;//insert
            route_table[null_index].distance = distance;
            route_table[null_index].lifetime = JMESH_ROUTE_MAX_DISTANCE-distance+1;
            //JMESH_LOGI("insert","src:%d,id:%d,dis:%d\n",src,id,route_table[null_index].distance);
        }
    }
}


void jmesh_route_exchange(void)
{
    int i,index;
    unsigned char buffer[JMESH_ROUTE_NODE_SIZE];
    unsigned short addr;
    //jmesh_gatt_t* gatt;
    if(JMESH_FEATURE_ENABLE==jmesh_get_lowpower_feature()){
        return;
    }
    os_timer_event_set(&route_exc_timer,1000*JMESH_ROUTE_EXCHANGE_MIN_PERIOD_S,(os_timer_event_caller_t)jmesh_route_exchange,NULL);
    if((addr=jmesh_get_primary_addr())<3){
        return;
    }
    memset(buffer,0xff,sizeof(buffer));
    for(i=0;i<JMESH_ROUTE_TABLE_SIZE;i++){
        if(route_table[i].lifetime > 0){
            if(route_table[i].distance>1){
                route_table[i].lifetime--;
            }
            /*addr=(i/JMESH_ROUTE_NODE_RECORD_NUM)*JMESH_ROUTE_NODE_ELEMENT_NUM+3;
            if(NULL!=(gatt=jmesh_gatt_get_link_by_addr(addr))){
                if(JMESH_FEATURE_ENABLE!=((jmesh_features_t*)&gatt->features)->relay){//if relay feature disable
                    continue;
                }
            }*/
            if(route_table[i].lifetime+route_table[i].distance >= JMESH_ROUTE_MAX_DISTANCE){//add: lifetime +distance = JMESH_ROUTE_MAX_DISTANCE
                index=i/JMESH_ROUTE_NODE_RECORD_NUM;
                if(buffer[index]>route_table[i].distance){
                    buffer[index]=route_table[i].distance;
                }
            }
        }
    }
    //JMESH_LOGI_BUFFER("exc",30,buffer,":");
    jmesh_control_send(addr,
                       JMESH_ADDR_ALL_ROUTING,
                       0,
                       JMESH_CONTROL_ROUTING_SHARE,
                       1,
					             JMESH_ROUTE_NODE_SIZE,
                       buffer);
}
void jmesh_route_exchange_recv(unsigned char id,unsigned char* buffer)
{
    int i;
    for(i=0;i<JMESH_ROUTE_NODE_SIZE;i++){
        jmesh_route_update(i*JMESH_ROUTE_NODE_ELEMENT_NUM+3,id,buffer[i]);
    }
}
void jmesh_route_clear_id(unsigned char id)
{
    int i;
    for(i=0;i<JMESH_ROUTE_TABLE_SIZE;i++){
        if(route_table[i].id==id){
            route_table[i].lifetime=0;
        }
    }
    //JMESH_LOGI("clr","id:%d\n",id);
}

jmesh_route_t* jmesh_route_get(unsigned short addr,unsigned char ttl)
{
    unsigned short index;
    int i;
    jmesh_route_t* route=NULL;

    if(addr>JMESH_ROUTE_NODE_ADDR_LIMIT||addr<3){
        return NULL;
    }
    index=JMESH_ROUTE_NODE_INDEX(addr);
    for(i=index;i<index+JMESH_ROUTE_NODE_RECORD_NUM;i++){
        if(route_table[i].lifetime && route_table[i].distance<=ttl){
            if(route==NULL){
                route=&route_table[i];
            }
            else if(route->distance>route_table[i].distance){
                route=&route_table[i];
            }
        }
    }
    return route;
}

void jmesh_route_get2(unsigned short addr,unsigned char ttl,jmesh_route_t** r1,jmesh_route_t** r2)
{
    unsigned short index;
    int i;
    *r1=*r2=NULL;
    if(addr>JMESH_ROUTE_NODE_ADDR_LIMIT||addr<3){
        return;
    }
    index=JMESH_ROUTE_NODE_INDEX(addr);
    for(i=index;i<index+JMESH_ROUTE_NODE_RECORD_NUM;i++){
        if(route_table[i].lifetime&&route_table[i].distance<=ttl){
            if(*r1==NULL){
                *r1=&route_table[i];
            }
            else if(((jmesh_route_t *)*r1)->distance>route_table[i].distance){
                if(((jmesh_route_t *)*r1)->id!=route_table[i].id){
                    *r2=*r1;
                }
                *r1=&route_table[i];
            }
            else if(*r2==NULL||((jmesh_route_t *)*r2)->distance>route_table[i].distance){
                if(((jmesh_route_t *)*r1)->id!=route_table[i].id){
                    *r2=&route_table[i];
                }
            }
        }
    }
}

jmesh_route_t* jmesh_route_get_by_addr(unsigned short addr)
{
    unsigned short index;
    int i;
    jmesh_route_t* route=NULL;

    if(addr>JMESH_ROUTE_NODE_ADDR_LIMIT||addr<3){
        return NULL;
    }
    index=JMESH_ROUTE_NODE_INDEX(addr);
    for(i=index;i<index+JMESH_ROUTE_NODE_RECORD_NUM;i++){
        if(route_table[i].lifetime){
            if(route==NULL){
                route=&route_table[i];
            }
            else if(route->distance>route_table[i].distance){
                route=&route_table[i];
            }
        }
    }
    return route;
}


void jmesh_route_send(unsigned char send_type,unsigned short dst,unsigned char ttl,jmesh_pdu_t* pdu)
{
    jmesh_route_t *routing1=NULL,*routing2=NULL;
    unsigned char id_backup=0xff;

    JMESH_LOGD_BUFFER("routing",pdu->length,pdu->pdu,"send to 0x%x data:",dst);
    //if(JMESH_ADDR_UNICAST==jmesh_addr_get_type(dst))
    //{
        jmesh_route_get2(dst,ttl,&routing1,&routing2);
        if(NULL!=routing2)
        {
            if(pdu->network.itf!=routing2->id)
            {
                id_backup=routing2->id;
            }
        }
        if(NULL!=routing1)
        {
            if(pdu->network.itf!=routing1->id){
                jmesh_proxy_send(send_type,routing1->id,id_backup,JMESH_PROXY_TYPE_NETWORK,pdu->length,pdu->pdu+2);
            }
            else if(JMESH_SEND_TYPE_ALL==send_type){
                jmesh_adv_send_message(pdu);
            }
            else{
                JMESH_LOGW(1,"routing","send fail interface=%d\n",routing1->id);
            }
        }
        else if(JMESH_SEND_TYPE_ALL==send_type){
            jmesh_adv_send_message(pdu);
        }
        else if(JMESH_ADDR_UNICAST==jmesh_addr_get_type(dst)){
		        JMESH_LOGW(1,"routing","send fail dst=%d no routing\n",dst);
        }
        else{
            jmesh_routing_neighbor_broadcast_all(pdu);
        }
    //}
    //else
    //{
    //    jmesh_routing_neighbor_broadcast_all(pdu);
    //}
}

void jmesh_gatt_adv_send(unsigned short dst,unsigned char ttl,jmesh_pdu_t* pdu){
    unsigned char send_type;
    if(pdu->network.itf==JMESH_SEND_ALL_CHANNEL){
        if(pdu->adv.length+2<=31){
            send_type=JMESH_SEND_TYPE_ALL;
        }
        else{
            send_type=JMESH_SEND_TYPE_ROUTE;
        }
        jmesh_route_send(send_type,dst,ttl,pdu);
    }
    else if(pdu->network.itf==JMESH_SEND_GATT_CHANNEL||pdu->network.itf<JMESH_SEND_ADV_CHANNEL){
        jmesh_route_send(JMESH_SEND_TYPE_ROUTE,dst,ttl,pdu);
    }
    else if(pdu->network.itf==JMESH_SEND_ADV_CHANNEL){
        if(pdu->adv.length+2<=31){
            jmesh_adv_send_message(pdu);
        }
    }
}
