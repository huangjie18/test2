#include"jmesh_route.h"
#include"jmesh_routing_neighbor.h"
#include"../../jmesh/jmesh_types.h"
#include"../../jmesh/jmesh_addr.h"
#include"../../access/jmesh_access.h"
#include"../../onchip_system/os_timer.h"
#include"../../onchip_system/os_timer_event.h"
#include"../../driver/jmesh_ble.h"
#include"../gatt/jmesh_proxy.h"
#include"../../onchip_system/os_list.h"
#include"../../driver/jmesh_system.h"
#include"../../jmesh/jmesh_print.h"
#include"../../network/jmesh_interface.h"
#include"../gatt/jmesh_gatt.h"
#include"jmesh_routing_heartbeat.h"
#include"jmesh_routing_remove.h"
#include"jmesh_routing_disconnect_request.h"
#include"../../jmesh/jmesh_features.h"
static os_timer_t neighbor_connect_enable_timer;
static os_timer_event_t neighbor_maintain_timer;
static jmesh_routing_neighbor_t routing_neighbor_table[JMESH_ROUTING_NEIGHBOR_SIZE];
//static unsigned short jmesh_disconnect_try_timer;
//jmesh_routing_neighbor_t* connected_neighbor_table[JMESH_ROUTING_CONNECT_MAX_NUM];

//static unsigned char routing_tag;
jmesh_routing_neighbor_t* routing_neighbor_table_p = routing_neighbor_table;


//rewrite this function please

/** \brief  rssi
 * \param
 * \param
 * \return
 */

static int routing_neighbor_compare(jmesh_routing_neighbor_t* na,jmesh_routing_neighbor_t* nb){
    jmesh_route_t *ra;
    jmesh_route_t *rb;
    int random;
    if(NULL==na){
        return 0;
    }
    else if(na->rssi<JMESH_ROUTING_NEIGHBOR_MIN_RSSI){
        return 0;
    }
    else if(jmesh_routing_get_neighbor_link_num(na)>=JMESH_ROUTING_CONNECT_MAX_NUM){
        return 0;
    }
    else if(NULL==nb){
        return 1;
    }
    else if(nb->rssi<JMESH_ROUTING_NEIGHBOR_MIN_RSSI){
        return 1;
    }
    else if(jmesh_routing_get_neighbor_link_num(nb)>=JMESH_ROUTING_CONNECT_MAX_NUM){
        return 1;
    }
    ra=jmesh_route_get_by_addr(na->addr);
    rb=jmesh_route_get_by_addr(nb->addr);
    if(ra!=NULL){
        if(rb!=NULL){
            return ra->distance>rb->distance;
        }
        return 0;//ra!=null,ra==null
    }
    else if(rb!=NULL){
        return 1;//ra==null,rb!=null
    }
    random=jmesh_random_get(1,261);
    if(random<(na->try_con_count+1)*260/(na->try_con_count+nb->try_con_count+2)){
        return 0;
    }
    else{
        return 1;
    }
}
//static int routing_neighbor_compare(jmesh_routing_neighbor_t* na,jmesh_routing_neighbor_t* nb){

//    jmesh_routing_t* ra;
//    jmesh_routing_t* rb;
//    /**< 1 rssi must be larger than connect enable limit */
//	int connect_numa = 0,connect_numb = 0;
//
//	for(int j=0;j<16;j++)
//	{
//		if(na->intv_mask & 0x01<<j) connect_numa++;
//		if(nb->intv_mask & 0x01<<j) connect_numb++;
//	}

//	if(connect_numa >= JMESH_ROUTING_CONNECT_MAX_NUM) return 0;
//	if(connect_numb >= JMESH_ROUTING_CONNECT_MAX_NUM) return 1;
//
//	// avoid to connect a no Power device
//	if(na->lifetime - nb->lifetime >200)
//	{
//		return 1;
//	}
//	if(na->lifetime - nb->lifetime <200)
//	{
//		return 0;
//	}
//    if(na->rssi > JMESH_ROUTING_NEIGHBOR_MIN_RSSI
//       && nb->rssi < JMESH_ROUTING_NEIGHBOR_MIN_RSSI){
//        return 1;
//    }
//    if(na->rssi < JMESH_ROUTING_NEIGHBOR_MIN_RSSI
//       && nb->rssi > JMESH_ROUTING_NEIGHBOR_MIN_RSSI){
//        return 0;
//    }
//    if(NULL==(ra=jmesh_routing_get(na->addr))){
//        return 1;
//    }
//    if(NULL==(rb=jmesh_routing_get(nb->addr))){
//        return 0;
//    }

//#ifdef JMESH_ROUTING_USE_SUBNET
//    /**< 2 in the same subnet but no route */
//    if(na->subnet==jmesh_routing_local_subnet.subnet){
//        if(NULL==(ra=jmesh_routing_get(na->addr))){
//            return 1;
//        }
//    }
//    if(nb->subnet==jmesh_routing_local_subnet.subnet){
//        if(NULL==(rb=jmesh_routing_get(nb->addr))){
//            return 0;
//        }
//    }
//    /**< 3 a unknown neighbor subnet */
//    if(NULL==jmesh_routing_subnet_get_by_addr(na->subnet)){
//        return 1;
//    }
//    if(NULL==jmesh_routing_subnet_get_by_addr(nb->subnet)){
//        return 0;
//    }
//#endif
//    /**< 4 make routing closer */
//    if(ra->distance >= JMESH_ROUTING_ADJUST_ENABLE_DISTANCE){
//        return 1;
//    }
//    if(rb->distance >= JMESH_ROUTING_ADJUST_ENABLE_DISTANCE){
//        return 0;
//    }
//    /**< 5 random choose one */
//	return connect_numa<connect_numb ? 1: 0 ;
//    return jmesh_random_get(0,2);
//}

int jmesh_routing_get_neighbor_link_num(jmesh_routing_neighbor_t *neighbor)
{
    int i,num;
    for(i=0,num=0;i<16;i++){
         if(neighbor->intv_mask&(1<<i)){
             num++;
         }
    }
    return num;
}

static void routing_neighbor_maintain(void)
{
    int i;
    unsigned short time_now;
    jmesh_gatt_t* gatt;
    jmesh_routing_neighbor_t* best=NULL;
    //JMESH_LOGI("nei main","\n");
    if(JMESH_FEATURE_ENABLE==jmesh_get_lowpower_feature()){
        return;
    }
    time_now=os_time_get_seconds();
    for(i=0;i<JMESH_ROUTING_NEIGHBOR_SIZE;i++){
        if(routing_neighbor_table[i].addr){
            if((unsigned short)(time_now-routing_neighbor_table[i].lifetime) <= JMESH_ROUTING_NEIGHBOR_LIFETIME_S ){
                if(NULL==jmesh_gatt_get_link_by_mac(routing_neighbor_table[i].mac))
                {
                    if(1==routing_neighbor_compare(&routing_neighbor_table[i],best))
                    {
                        best=&routing_neighbor_table[i];
                    }
                }
            }
            else{
                gatt=jmesh_gatt_get_link_by_addr(routing_neighbor_table[i].addr);
                if(NULL!=gatt){
                    JMESH_LOGI("routing neighbor","addr=0x%x id=%d timeout disconnect",routing_neighbor_table[i].addr,gatt->id);
                    jmesh_ble_gatt_disconnect(gatt->id);
                }
            }
        }
    }
    if(jmesh_gatt_non_lowp_link_num()<JMESH_ROUTING_CONNECT_MAX_NUM){
        if(os_timer_is_flow(&neighbor_connect_enable_timer)&& best!=NULL ){
            if(JMESH_FEATURE_ENABLE!=((jmesh_features_t*)(&best->features))->lowpower){//don't connect low power node
                os_timer_restart(&neighbor_connect_enable_timer);
                JMESH_LOGI("routing beacon","request addr %d to connect\n",best->addr);
                if(0==jmesh_routing_beacon_link_require(best->addr,NULL)){
                    best->try_con_count++;
                }
            }
        }
    }
    os_timer_event_set(&neighbor_maintain_timer,jmesh_random_get(JMESH_ROUTING_MAINTAIN_INTERVAL_MIN_MS,JMESH_ROUTING_MAINTAIN_INTERVAL_MAX_MS),(os_timer_event_caller_t)routing_neighbor_maintain,NULL);
}

void jmesh_routing_neighbor_init(void)
{
    int i;
    for(i=0;i<JMESH_ROUTING_NEIGHBOR_SIZE;i++){
        routing_neighbor_table[i].addr=0;
        routing_neighbor_table[i].try_con_count=0;
    }
    //jmesh_routing_share_timer=JMESH_ROUTING_NEIGHBOR_SHARE_INTERVAL;
    //jmesh_disconnect_try_timer=JMESH_ROUTING_DISCONNECT_TRY_INTERVAL;
    if(JMESH_FEATURE_ENABLE!=jmesh_get_lowpower_feature()){
        os_timer_set(&neighbor_connect_enable_timer,jmesh_random_get(JMESH_ROUTING_NEIGHBOR_CONNECT_INTERVAL_MIN_MS,JMESH_ROUTING_NEIGHBOR_CONNECT_INTERVAL_MAX_MS));
        os_timer_event_set(&neighbor_maintain_timer,jmesh_random_get(JMESH_ROUTING_MAINTAIN_INTERVAL_MIN_MS,JMESH_ROUTING_MAINTAIN_INTERVAL_MAX_MS),(os_timer_event_caller_t)routing_neighbor_maintain,NULL);
    }
}
/*void jmesh_routing_neighbor_uninit(void)
{
    if(JMESH_FEATURE_ENABLE!=jmesh_get_lowpower_feature()){
        os_timer_event_remove(&neighbor_maintain_timer);
    }
}*/
int jmesh_routing_neighbor_add(unsigned short addr,unsigned char* mac,unsigned char rssi,unsigned char features)
{
  int i,null=-1;
	for(i=0;i<JMESH_ROUTING_NEIGHBOR_SIZE;i++)
	{
	  if(addr==routing_neighbor_table[i].addr){
	      unsigned char temp_features=features;
	      routing_neighbor_table[i].rssi = rssi;
			  routing_neighbor_table[i].lifetime = os_time_get_seconds();
			  ((jmesh_features_t*)&routing_neighbor_table[i].features)->lowpower=((jmesh_features_t*)&temp_features)->lowpower;
        ((jmesh_features_t*)&routing_neighbor_table[i].features)->friend=((jmesh_features_t*)&temp_features)->friend;
        return 0;
	  }
		if(routing_neighbor_table[i].addr == 0)
		{
		  if(null<0){
		      null=i;
		  }
		}
	}
	if(null>=0){
	    routing_neighbor_table[null].addr = addr;
			memcpy(routing_neighbor_table[null].mac,mac,6);
			routing_neighbor_table[null].rssi = rssi;
			routing_neighbor_table[null].lifetime = os_time_get_seconds();
			routing_neighbor_table[null].intv_mask = 0;
			routing_neighbor_table[null].features=features;
      return 0;
	}
	return -1;
}
void jmesh_routing_neig_lifetime_refresh(unsigned short addr)
{
    int i;
    for(i=0;i<JMESH_ROUTING_NEIGHBOR_SIZE;i++){
        if(routing_neighbor_table[i].addr==addr){
            //JMESH_LOGI("lifetime refresh","addr:%d\n",addr);
            routing_neighbor_table[i].lifetime=os_time_get_seconds();
            return;
        }
    }
}
jmesh_routing_neighbor_t* jmesh_routing_neighbor_update(unsigned char* mac,unsigned char rssi,jmesh_routing_beacon_t* beacon)
{
    int find_index,null_index,timeflow_index;
    unsigned short addr,time_now;
    JMESH_BIG_ENDIAN_PICK2(addr,beacon->addr);
    time_now=os_time_get_seconds();
    for(find_index=0,null_index=-1,timeflow_index=-1;find_index<JMESH_ROUTING_NEIGHBOR_SIZE;find_index++){
        if(routing_neighbor_table[find_index].addr==addr){
            break;
        }
        if(routing_neighbor_table[find_index].addr==0){
            if(null_index==-1){
                null_index=find_index;
            }
        }
        else if(timeflow_index==-1){
            if(time_now - routing_neighbor_table[find_index].lifetime > JMESH_ROUTING_NEIGHBOR_LIFETIME_S && NULL==jmesh_gatt_get_link_by_addr(routing_neighbor_table[find_index].addr)){
                timeflow_index=find_index;
            }
        }
    }
    if(find_index>=JMESH_ROUTING_NEIGHBOR_SIZE){
        if(null_index!=-1){
            find_index=null_index;
        }
        else if(timeflow_index!=-1){
            find_index=timeflow_index;
        }
        else{
            return NULL;
        }
    }
    routing_neighbor_table[find_index].addr=addr;
    memcpy(routing_neighbor_table[find_index].mac,mac,6);
    routing_neighbor_table[find_index].rssi=rssi;
//    routing_neighbor_table[find_index].element_num=beacon->element_num;
//    routing_neighbor_table[find_index].net_distance=beacon->net_distance;
//    routing_neighbor_table[find_index].net_size=beacon->net_size;

    JMESH_BIG_ENDIAN_PICK2(routing_neighbor_table[find_index].intv_mask,beacon->intv_mask);
//    JMESH_BIG_ENDIAN_PICK2(routing_neighbor_table[find_index].subnet,beacon->subnet);
    //JMESH_BIG_ENDIAN_PICK2(routing_neighbor_table[find_index].link_node[0],beacon->link_node[0]);
    //JMESH_BIG_ENDIAN_PICK2(routing_neighbor_table[find_index].link_node[1],beacon->link_node[1]);
    //JMESH_BIG_ENDIAN_PICK2(routing_neighbor_table[find_index].link_node[2],beacon->link_node[2]);
    //JMESH_BIG_ENDIAN_PICK2(routing_neighbor_table[find_index].link_node[3],beacon->link_node[3]);
    //JMESH_BIG_ENDIAN_PICK2(routing_neighbor_table[find_index].link_node[4],beacon->link_node[4]);
    //JMESH_BIG_ENDIAN_PICK2(routing_neighbor_table[find_index].link_node[5],beacon->link_node[5]);
    routing_neighbor_table[find_index].lifetime=time_now;
    routing_neighbor_table[find_index].features=beacon->features;
    jmesh_gatt_features_update(mac,beacon->features);
    return &routing_neighbor_table[find_index];
}
jmesh_routing_neighbor_t* jmesh_routing_neighbor_get_by_addr(unsigned short addr)
{
    int i;
    for(i=0;i<JMESH_ROUTING_NEIGHBOR_SIZE;i++){
        if(routing_neighbor_table[i].addr<=addr&&(routing_neighbor_table[i].addr+2) > addr){
            return &routing_neighbor_table[i];
        }
    }
    return NULL;
}
jmesh_routing_neighbor_t* jmesh_routing_neighbor_get_by_mac(unsigned char* mac)
{
    int i;
    for(i=0;i<JMESH_ROUTING_NEIGHBOR_SIZE;i++){
        if(routing_neighbor_table[i].addr!=0 && 0 == memcmp(routing_neighbor_table[i].mac,mac,6)){
            return &routing_neighbor_table[i];
        }
    }
    return NULL;
}


void jmesh_routing_neighbor_flush(jmesh_routing_neighbor_t* neighbor)
{
    if(neighbor!=NULL){
        neighbor->lifetime=os_time_get_seconds();
    }
}

int jmesh_routing_neighbor_is_multilinked(jmesh_routing_neighbor_t* neighbor)
{
    /*int i,j;
    for(i=0;i<JMESH_ROUTING_NEIGHBOR_SIZE;i++){
        if(routing_neighbor_table[i].addr!=0 && &routing_neighbor_table[i]!=neighbor){
            for(j=0;j<6;j++){
                if(routing_neighbor_table[i].link_node[j]==neighbor->addr){
                    return 1;
                }
            }
        }
    }*/
    return 0;
}

void jmesh_routing_connected(unsigned char* mac,unsigned char interface)
{
}

void jmesh_routing_disconnected(unsigned char interface){
}
#if 0
unsigned short jmesh_routing_get_local_gate(void){
    int i;
    for(i=0;i<JMESH_ROUTING_CONNECT_MAX_NUM;i++){
        if(connected_neighbor_table[i]!=NULL){
            return connected_neighbor_table[i]->addr;
        }
    }
    return 0;
}
#endif
void jmesh_routing_fill_beacon(jmesh_routing_beacon_t* beacon){
    unsigned short primary_addr;
    unsigned short intv_mask;
    //unsigned long iv_index;
    unsigned short net_ticks;
    jmesh_features_t features;
    primary_addr=jmesh_get_primary_addr();
    JMESH_BIG_ENDIAN_FILL2(primary_addr,beacon->addr);
    //beacon->iv_update_flag=jmesh_netkey_get_iv_update_flag(0);
    //iv_index=jmesh_netkey_get_iv_index(0);
    //beacon->iv_index[0]=(unsigned char)(iv_index>>24);
    //beacon->iv_index[1]=(unsigned char)(iv_index>>16);
    //beacon->iv_index[2]=(unsigned char)(iv_index>>8);
    //beacon->iv_index[3]=(unsigned char)(iv_index);
    //beacon->element_num=jmesh_get_element_num();
    net_ticks=jmesh_net_ticks_get();
    JMESH_BIG_ENDIAN_FILL2(net_ticks,beacon->net_ticks);

    /*for(i=0;i<JMESH_GATT_SIZE;i++){
        gatt=jmesh_gatt_get_link_by_interface(i);
        if(gatt!=NULL){
            JMESH_BIG_ENDIAN_FILL2(gatt->addr,beacon->link_node[i]);
        }
        else{
            JMESH_BIG_ENDIAN_FILL2(0,beacon->link_node[i]);
        }
    }*/
    features=jmesh_get_features();
    beacon->features=features.features;
    intv_mask=jmesh_gatt_get_intv_mask();
    JMESH_BIG_ENDIAN_FILL2(intv_mask,beacon->intv_mask);
}
void jmesh_routing_link_require(jmesh_routing_neighbor_t* neighbor)
{
    //if(JMESH_ROUTING_NEIGHBOR_CONNECT_INTERVAL_MS > os_timer_remain(&neighbor_connect_enable_timer)){
    //    os_timer_set(&neighbor_connect_enable_timer,jmesh_random_get(JMESH_ROUTING_NEIGHBOR_CONNECT_INTERVAL_MIN_MS,JMESH_ROUTING_NEIGHBOR_CONNECT_INTERVAL_MAX_MS));
        if(NULL==jmesh_gatt_get_link_by_mac(neighbor->mac)){
            if(jmesh_gatt_non_lowp_link_num()<JMESH_GATT_CONNECT_LIMIT){
                JMESH_LOGI("connect","%d\n",neighbor->addr);
                jmesh_connect_start(neighbor->mac);
            }
            else if(jmesh_gatt_total_link_num()<JMESH_GATT_SIZE){
                if(JMESH_FEATURE_ENABLE==jmesh_get_friend_feature()&&(JMESH_FEATURE_ENABLE==((jmesh_features_t*)&neighbor->features)->lowpower)){
                    JMESH_LOGI("connect","%d\n",neighbor->addr);
                    jmesh_connect_start(neighbor->mac);
                }
            }
        }
    //}
}

void jmesh_routing_neighbor_broadcast_all(jmesh_pdu_t* pdu)
{
    unsigned char i;
    jmesh_gatt_t *gatt;
    for(i=0;i<JMESH_GATT_SIZE;i++){
        gatt=jmesh_gatt_get_ready_by_interface(i);
        if(gatt!=NULL){
            if(pdu->network.itf!=gatt->id){
                jmesh_proxy_send(JMESH_SEND_TYPE_CONNECT,gatt->id,0xff,JMESH_PROXY_TYPE_NETWORK,pdu->length,pdu->pdu+2);
            }
        }
    }
}


