#include"../../jmesh/jmesh_pdu.h"
#include"jmesh_gatt.h"
#include"jmesh_gatt_callback.h"
#include"../../driver/jmesh_ble.h"
#include"string.h"
#include"jmesh_proxy.h"
#include"../../onchip_system/os_timer_event.h"
#include"../../BLE/jmesh_prov_svc.h"
#include"../../jmesh/jmesh_print.h"
#include"../../driver/jmesh_system.h"
#include"../../foundation/state/jmesh_state.h"
#include"../../network/jmesh_network.h"
#include"../../foundation/config/configuration_cmd.h"
#include"../routing/jmesh_route.h"
#include"../../upper/lowpower/jmesh_friend.h"
#include"../../upper/lowpower/jmesh_lowpower.h"

static os_timer_event_t link_timer;
static jmesh_gatt_t gatt_table[JMESH_GATT_SIZE];
/*
static unsigned short con_intv_table[]={307,317,331,347,359,373,383,397,409,419,
                                        431,443,457,467,479,491};*/
/*
const static unsigned short con_intv_table[]={53,63,73,83,93,103,113,123,133,143,
                                              153,163,173,183,193,203,213,223,233,243,
                                              253,263,273,283};*/
/*
const static unsigned short con_intv_table[]={103,113,123,133,143,153,163,173,183,193,
                                              203,213,223,233,243,253,263,273,283,293,
                                              303,313,323,333};*/
/*
const static unsigned short con_intv_table[]={153,163,173,183,193,203,213,223,233,243,
                                              253,263,273,283,293,303,313,323,333,343,
                                              353,363,373,383};*/

const static unsigned short con_intv_table[]={203,216,229,242,255,268,281,294,307,320,
                                              333,346,359,372,385,398,411,424,437,450,
                                              463,476,489,502};

static gatt_phone_t gatt_phone={0xff,{0x00},NULL};

static void jmesh_gatt_link_timer_handler(void);

void jmesh_gatt_init(void)
{
    int i;
    for(i=0;i<JMESH_GATT_SIZE;i++){
        gatt_table[i].flag=0;
    }
    if(JMESH_FEATURE_ENABLE!=jmesh_get_lowpower_feature()){
        os_timer_event_set(&link_timer,1000,(os_timer_event_caller_t)jmesh_gatt_link_timer_handler,NULL);
    }
    jmesh_gatt_record_init();
}
void jmesh_gatt_disconnect(jmesh_gatt_t* gatt){
    if(NULL==gatt){
        return;
    }
    else if(gatt->is_linked){
        jmesh_ble_gatt_disconnect(gatt->id);
        //jmesh_gatt_disconnected(gatt->id);
    }
}

void jmesh_gatt_disconnect_all(void)
{
    for(int i=0;i<JMESH_GATT_SIZE;i++){
        jmesh_gatt_disconnect(&gatt_table[i]);
    }
}



int jmesh_gatt_non_lowp_link_num(void)
{
    int i,count;
    for(i=0,count=0;i<JMESH_GATT_SIZE;i++){
        if(gatt_table[i].is_linked&&(JMESH_FEATURE_ENABLE!=((jmesh_features_t*)&gatt_table[i].features)->lowpower)){
            count++;
        }
    }
    return count;
}
int jmesh_gatt_total_link_num(void)
{
    int i,count;
    for(i=0,count=0;i<JMESH_GATT_SIZE;i++){
        if(gatt_table[i].is_linked){
            count++;
        }
    }
    return count;
}
int jmesh_gatt_ready_num(void)
{
    int i,count;
    for(i=0,count=0;i<JMESH_GATT_SIZE;i++){
        if(gatt_table[i].is_ready){
            count++;
        }
    }
    return count;
}

jmesh_gatt_t* jmesh_gatt_get_link_by_mac(unsigned char* mac){
    int i;
    for(i=0;i<JMESH_GATT_SIZE;i++){
        if(gatt_table[i].is_linked){
            if(0==memcmp(mac,gatt_table[i].mac,6)){
                return &gatt_table[i];
            }
        }
    }
    return NULL;
}

jmesh_gatt_t* jmesh_gatt_get_ready_by_mac(unsigned char* mac){
    int i;
    for(i=0;i<JMESH_GATT_SIZE;i++){
        if(gatt_table[i].is_ready){
            if(0==memcmp(mac,gatt_table[i].mac,6)){
                return &gatt_table[i];
            }
        }
    }
    return NULL;
}


jmesh_gatt_t* jmesh_gatt_get_link_by_id(unsigned char id)
{
    int i;
    for(i=0;i<JMESH_GATT_SIZE;i++){
        if(gatt_table[i].is_linked){
            if(id==gatt_table[i].id){
                return &gatt_table[i];
            }
        }
    }
    return NULL;
}

jmesh_gatt_t* jmesh_gatt_get_ready_by_id(unsigned char id)
{
    int i;
    for(i=0;i<JMESH_GATT_SIZE;i++){
        if(gatt_table[i].is_ready){
            if(id==gatt_table[i].id){
                return &gatt_table[i];
            }
        }
    }
    return NULL;
}


jmesh_gatt_t* jmesh_gatt_get_ready_by_interface(unsigned char interface){
    if(interface<JMESH_GATT_SIZE){
        if(gatt_table[interface].is_ready){
            return &gatt_table[interface];
        }
    }
    return NULL;
}

jmesh_gatt_t* jmesh_gatt_get_link_by_interface(unsigned char interface){
    if(interface<JMESH_GATT_SIZE){
        if(gatt_table[interface].is_linked){
            return &gatt_table[interface];
        }
    }
    return NULL;
}

jmesh_gatt_t* jmesh_gatt_get_ready_by_addr(unsigned short addr)
{
    int i;
    for(i=0;i<JMESH_GATT_SIZE;i++){
        if(gatt_table[i].is_ready){
            if(addr==gatt_table[i].addr){
                return &gatt_table[i];
            }
        }
    }
    return NULL;
}

jmesh_gatt_t* jmesh_gatt_get_link_by_addr(unsigned short addr)
{
    int i;
    for(i=0;i<JMESH_GATT_SIZE;i++){
        if(gatt_table[i].is_linked){
            if(addr==gatt_table[i].addr){
                return &gatt_table[i];
            }
        }
    }
    return NULL;
}

unsigned char jmesh_gatt_get_interface(jmesh_gatt_t* gatt){
    if(gatt!=NULL){
      return gatt - gatt_table;
    }
    return JMESH_GATT_SIZE;
}
jmesh_gatt_t* jmesh_gatt_link_next(jmesh_gatt_t* gatt){
    int i;
    if(gatt==NULL){
        i=-1;
    }
    else{
        i=gatt-gatt_table;
    }
    for(i++;i<JMESH_GATT_SIZE;i++){
        if(gatt_table[i].is_linked){
            return &gatt_table[i];
        }
    }
    return NULL;
}
int jmesh_gatt_is_ready(jmesh_gatt_t* gatt){
    if(gatt!=NULL){
        return gatt->is_ready;
    }
    return 0;
}

int jmesh_gatt_is_link(jmesh_gatt_t* gatt){
    if(gatt!=NULL){
        return gatt->is_linked;
    }
    return 0;
}

static void jmesh_gatt_link_timer_handler(void)
{
    int i;
    if(JMESH_FEATURE_ENABLE==jmesh_get_lowpower_feature()){
        return;
    }
    for(i=0;i<JMESH_GATT_SIZE;i++)
    {
        if(gatt_table[i].is_ready){
            continue;
        }
        else if(gatt_table[i].is_linked)
        {
            gatt_table[i].timer++;
            /*if(JMESH_FEATURE_ENABLE==((jmesh_features_t*)&gatt_table[i].features)->lowpower){
                if(gatt_table[i].timer>=JMESH_FRIEND_LOWPOWER_LINK_TIMEOUT_S){
                    gatt_table[i].timer=0;
                    JMESH_LOGW(1,"gatt","low power link timeout:%d\n",gatt_table[i].id);
                    jmesh_ble_gatt_disconnect(gatt_table[i].id);
                }
            }*/
            if(gatt_table[i].timer>=JMESH_GATT_LINK_IS_READY_S)
            {
                gatt_table[i].is_ready=1;
                jmesh_route_update(gatt_table[i].addr,gatt_table[i].id,0);
            }
        }
    }
    os_timer_event_restart(&link_timer);
}

static void jmesh_gatt_multilink_disconnect_old(unsigned char new_id)
{
    int count=0;
    unsigned char id[JMESH_GATT_SIZE];
    int i,j,random;
    for(i=0,j=0;i<JMESH_GATT_SIZE;i++)
    {
        if(gatt_table[i].is_linked&&(JMESH_FEATURE_ENABLE!=((jmesh_features_t*)&gatt_table[i].features)->lowpower))
        {
            count++;
            if(new_id!=gatt_table[i].id)
            {
                id[j++]=gatt_table[i].id;
            }
        }
    }
    if(count>JMESH_GATT_CONNECT_LIMIT)
    {
        random=jmesh_random_get(0,j);
        JMESH_LOGW(1,"gatt multilink","will disconnect id:%d\n",id[random]);
        jmesh_ble_gatt_disconnect(id[random]);
    }
}

void jmesh_gatt_features_update(unsigned char *mac,unsigned char features)
{
    jmesh_gatt_t* gatt=jmesh_gatt_get_link_by_mac(mac);
    if(gatt){
        gatt->features=features;
    }
}
void jmesh_gatt_connected(unsigned char* mac,unsigned char id,unsigned short con_intv,unsigned char is_server){
    int i,null_index;
    jmesh_routing_neighbor_t* neighbor;
    //static unsigned char mesh_uuid[16]=JMESH_SVC_UUID;
    //jmesh_gatt_connected_callback(mac,id);

    for(i=0,null_index=-1;i<JMESH_GATT_SIZE;i++){
        if(!gatt_table[i].is_linked){
            if(null_index==-1){
                null_index=i;
            }
        }
    }
    if(null_index>=0){
        memcpy(gatt_table[null_index].mac,mac,6);
        gatt_table[null_index].is_linked=1;
        gatt_table[null_index].id=id;
        gatt_table[null_index].addr=0;
        gatt_table[null_index].intv_pos=jmesh_gatt_get_intv_pos(con_intv);
        gatt_table[null_index].is_server=is_server;
        gatt_table[null_index].mtu=GATT_DEFAULT_MTU;
        gatt_table[null_index].svc_instance=jmesh_ble_get_local_svc_ins();//disc svc if a client,else get local svc instance
        gatt_table[null_index].timer=0;

        if(NULL!=(neighbor=jmesh_routing_neighbor_get_by_mac(mac))){
            gatt_table[null_index].addr=neighbor->addr;
            gatt_table[null_index].features=neighbor->features;
            if(JMESH_FEATURE_ENABLE==((jmesh_features_t*)(&neighbor->features))->lowpower){//neighbor is low power
                jmesh_lowpower_node_route_update(gatt_table[null_index].addr,gatt_table[null_index].id);
                jmesh_routing_neighbor_flush(neighbor);
            }
            else if(JMESH_FEATURE_ENABLE==((jmesh_features_t*)(&neighbor->features))->friend&&JMESH_FEATURE_ENABLE==jmesh_get_lowpower_feature()){//neighbor is friend and me is low power
                jmesh_route_update(gatt_table[null_index].addr,gatt_table[null_index].id,0);
            }
            neighbor->try_con_count=0;
            jmesh_gatt_record_add(gatt_table[null_index].addr,0x00,os_time_get_seconds());
        }
        else
        {
            JMESH_LOGW(1,"gatt connected","no neighbor\n");
        }
        if(is_server)
        {
            jmesh_ble_exchange_mtu(id);//exchange MTU
        }
        jmesh_gatt_multilink_disconnect_old(id);
    }
    else{
        JMESH_LOGW(1,"gatt connected","gatt table full,will disconnect id:%d\n",id);
        jmesh_ble_gatt_disconnect(id);
    }
}
void jmesh_gatt_disconnected(unsigned char connect_id,unsigned char reason){
    jmesh_gatt_t* gatt;
    //jmesh_gatt_disconnected_callback(connect_id,reason);
    if(NULL!=(gatt=jmesh_gatt_get_link_by_id(connect_id))){
        gatt->flag=0;
        if(JMESH_FEATURE_ENABLE!=((jmesh_features_t*)&gatt->features)->lowpower){
            jmesh_route_clear_id(connect_id);
        }
        else{
            jmesh_lowpower_node_route_update(gatt->addr,JMESH_INTERFACE_VIRTUAL);
        }
        jmesh_gatt_record_add(gatt->addr,reason,os_time_get_seconds());
        JMESH_LOGI_BUFFER("disconnected",6,gatt->mac,"reason:0x%02x,id:%d,mac:",reason,connect_id);
    }
}
void jmesh_gatt_set_mtu(unsigned char id, unsigned short mtu){
    jmesh_gatt_t* gatt;
    if(NULL!=(gatt=jmesh_gatt_get_link_by_id(id))){
        gatt->mtu=mtu-3;
        jmesh_lowpower_connected(gatt->addr);
    }
}
void jmesh_gatt_set_svc_instance(unsigned char id,unsigned short instance)
{
    jmesh_gatt_t* gatt;
    if(NULL!=(gatt=jmesh_gatt_get_link_by_id(id))){
        gatt->svc_instance=instance;
    }
}

unsigned char jmesh_app_recv_handler(unsigned char id,unsigned short instance,unsigned short length,unsigned char* data);

void jmesh_gatt_recv(unsigned char id,unsigned short instance,unsigned short length,unsigned char* data)
{
    JMESH_LOGW(1,"gatt recv","id:%d,len:%d\n",id,length);
    //jmesh_gatt_recv_callback(id,instance,length,data);
		if(1==jmesh_app_recv_handler(id,instance,length,data))
			return;
    if(NULL!=jmesh_gatt_get_phone(id))
    {
        if(jmesh_ble_get_local_svc_ins()+JMESHS_IDX_MESH_CONFIG_CMD_VAL==instance)
        {
            #if (JMESH_CMD_GATT_USE_PROXY)
            jmesh_proxy_recv(id,instance,length,data);
            #else
            configuration_cmd_process(id,length,data);
            #endif
        }
        else{
            jmesh_proxy_recv(id,instance,length,data);
        }
        return;
    }
    if(network_cipher_cache(data)) return;
    jmesh_proxy_recv(id,instance,length,data);
}


unsigned short jmesh_gatt_get_intv_mask(void)
{
    unsigned short intv_mask=0;
    jmesh_gatt_t* gatt=NULL;
    do
    {
        gatt=jmesh_gatt_link_next(gatt);
        if(NULL!=gatt)
        {
            intv_mask|=(1<<gatt->intv_pos);
        }

    }while(NULL!=gatt);
    return intv_mask;
}

unsigned short jmesh_gatt_get_null_intv(unsigned char *mac)
{
    int i;
    jmesh_routing_neighbor_t* neighbor;
    unsigned short intv_mask=0;
    neighbor=jmesh_routing_neighbor_get_by_mac(mac);
    if(NULL==neighbor)
    {
        return con_intv_table[sizeof(con_intv_table)/sizeof(con_intv_table[0])-1];
    }
    intv_mask=neighbor->intv_mask|jmesh_gatt_get_intv_mask();
    for(i=0;i<sizeof(con_intv_table)/sizeof(con_intv_table[0]);i++)
    {
        if(0==((1<<i)&intv_mask))
        {
            return con_intv_table[i];
        }
    }
    return con_intv_table[sizeof(con_intv_table)/sizeof(con_intv_table[0])-1];
}

unsigned char jmesh_gatt_get_intv_pos(unsigned short intv_param)
{
    unsigned char i;
    for(i=0;i<sizeof(con_intv_table)/sizeof(con_intv_table[0]);i++)
    {
        if(con_intv_table[i]==intv_param)
        {
            break;
        }
    }
    return i;
}


int jmesh_gatt_trigger_send(unsigned char id,unsigned short instance,unsigned short length,unsigned char *data)
{
    jmesh_gatt_t* gatt;
    if(length>JMESH_BLE_MTU_SIZE||0==length||0==instance){
        JMESH_LOGE(1,"gatt send","len:%d,ins:%d\n",length,instance);
        JMESH_ASSERT(0);
        return 0;
    }
    gatt=jmesh_gatt_get_link_by_id(id);
    if(NULL!=gatt){
        if(!gatt->is_server){
            jmesh_ble_gatt_notify(gatt->id,instance,length,data);
        }
        else{
            jmesh_ble_gatt_write(gatt->id,instance,length,data);
        }
        return 1;
    }
    else if(NULL!=jmesh_gatt_get_phone(id))
    {
        jmesh_ble_gatt_notify(id,instance,length,data);
        return 1;
    }
    else
    {
        JMESH_LOGN("gatt send","id %d cannot send\n",id);
        return 0;
    }
}

int jmesh_gatt_is_mesh_device(unsigned char *mac)
{
    if(NULL!=jmesh_routing_neighbor_get_by_mac(mac))
    {
        return 1;
    }
    return 0;
}


void jmesh_gatt_phone_connected(unsigned char *mac,unsigned char id)
{
    unsigned char con_id;
    con_id=gatt_phone.id;
    gatt_phone.id=id;
    memcpy(gatt_phone.mac,mac,6);
    gatt_phone.mtu=GATT_DEFAULT_MTU;
    //jmesh_ble_gatt_param_update(id,173);
    if(0xff!=con_id)
    {
        JMESH_LOGW(1,"new connected","old connection not disconnect,will disconnect it\n");
        jmesh_ble_gatt_disconnect(con_id);
    }
}

void jmesh_gatt_phone_disconnected(unsigned char id,unsigned char reason)
{
    if(id==gatt_phone.id)
    {
        gatt_phone.id=0xff;
        JMESH_LOGI_BUFFER("disconnected",6,gatt_phone.mac,"reason:0x%02x,id:%d,mac:",reason,id);
    }
}

gatt_phone_t *jmesh_gatt_get_phone(unsigned char id)
{
    if(id==gatt_phone.id)
    {
        return &gatt_phone;
    }
    return NULL;
}

void jmesh_gatt_phone_set_mtu(unsigned char id, unsigned short mtu){
    gatt_phone_t* gatt;
    if(NULL!=(gatt=jmesh_gatt_get_phone(id))){
        gatt->mtu=mtu-3;
    }
}


unsigned short jmesh_gatt_mtu_get_by_id(unsigned char id)
{
    void* gatt=NULL;
    gatt=jmesh_gatt_get_link_by_id(id);
    if(NULL!=gatt)
    {
        return ((jmesh_gatt_t*)gatt)->mtu;
    }
    gatt=jmesh_gatt_get_phone(id);
    if(NULL!=gatt)
    {
        return ((gatt_phone_t*)gatt)->mtu;
    }
    return 0;
}

gatt_phone_t *jmesh_app_get_phone(void)
{
		return &gatt_phone;
}

#if 1
#define JMESH_GATT_RD_TABLE_SIZE 80

typedef struct st_jmesh_gatt_rd
{
    unsigned char is_valid;
    unsigned short addr;
    unsigned char reason;
    unsigned long time_s;
}jmesh_gatt_rd_t;

static jmesh_gatt_rd_t gatt_rd_table[JMESH_GATT_RD_TABLE_SIZE];

void jmesh_gatt_record_init(void)
{
    memset(gatt_rd_table,0x00,sizeof(gatt_rd_table));

}

jmesh_gatt_rd_t* jmesh_gatt_record_add(unsigned short addr,unsigned char reason,unsigned long time_s)
{
    int i;
    for(i=0;i<JMESH_GATT_RD_TABLE_SIZE;i++)
    {
        if(!gatt_rd_table[i].is_valid)
        {
            gatt_rd_table[i].is_valid=1;
            gatt_rd_table[i].addr=addr;
            gatt_rd_table[i].reason=reason;
            gatt_rd_table[i].time_s=time_s;
            return &gatt_rd_table[i];
        }

    }
    return NULL;

}


void jmesh_gatt_rd_table_print(void)
{
    JMESH_CMD_RES(0xff,"t:%ds\n",os_time_get_seconds());
    for(int i=0;i<JMESH_GATT_RD_TABLE_SIZE;i++)
    {
        if(!gatt_rd_table[i].is_valid)
        {
            continue;
        }

        JMESH_CMD_RES(0xff,"%d,r:0x%02x,a:%d,t:%ds\n",i,gatt_rd_table[i].reason,gatt_rd_table[i].addr,gatt_rd_table[i].time_s);

    }

}

#endif


