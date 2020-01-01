#include"water_gate.h"
#include"pro188.h"
//#include"pro645.h"
#include"stdio.h"
#include"../../../../onchip_system/os_timer_event.h"
#include"../../../../driver/jmesh_ble.h"
#include"../app_expand.h"
#include"../../../../driver/jmesh_uart.h"
#include"../../../../jmesh/jmesh_save.h"

//#define WATER_NODE_SIZE 20
//#define WATER_NODE_RECORD_SIZE 100

#define WATER_RECORD_INTERVAL_MIN_DEFAULT 5
#define WATER_RECV_INTERVAL_MIN_DEFAULT 1
#define WATER_RECV_WINDOW_SEC_DEFAULT 3


static water_gate_param_t gate_param;
static water_node_t water_node[WATER_NODE_SIZE];
static water_record_t water_record[WATER_NODE_SIZE][WATER_NODE_RECORD_SIZE];
static unsigned char water_record_index;

static os_timer_event_t recv_timer,record_timer;

void water_gate_stop_recv(void);
void water_gate_start_recv(void);
void water_gate_stop_recv(void)
{
    os_timer_event_set(&recv_timer,gate_param.recv_interval_min*60*1000UL,(os_timer_event_caller_t)water_gate_start_recv,NULL);
    jmesh_ble_scan_disable();
}
void water_gate_start_recv(void)
{
    os_timer_event_set(&recv_timer,gate_param.recv_window_sec*1000,(os_timer_event_caller_t)water_gate_stop_recv,NULL);
    jmesh_ble_scan_enable();
}
void water_gate_record(void)
{
    int i;
    jmesh_device_role_t device_roles;
    device_roles=jmesh_get_device_roles();
    if(!device_roles.gate){
        return;
    }
    water_record_index++;
    if(water_record_index>=WATER_NODE_SIZE){
        water_record_index=0;
    }
    for(i=0;i<WATER_NODE_SIZE;i++){
        water_record[i][water_record_index].value=0xffff;
    }
    os_timer_event_restart(&record_timer);
}
void water_report_handler(unsigned short src,unsigned short dst,unsigned short len,unsigned char* data)
{
    unsigned long value;
    unsigned char mac[7]={0x00};
    if(12==len){
        memcpy(&value,data,4);
        //memcpy(mac,data+(19-5),6);//adv recv
        memcpy(mac,&data[6],6);
        JMESH_LOGI_BUFFER("rev report",7,mac,"src:%d,value:%d,mac:\n",src-1,value);
        water_gate_report_recv(mac,src-1,value,data[4],data[5]);
    }
}
void water_gate_init(void)
{
    static app_expand_message_t report;
    jmesh_app_expand2_message_register(&report,JMESH_APP_EXPAND2_WATER_REPORT,water_report_handler);
    memset(water_node,0,sizeof(water_node_t));
    memset(water_record,0xff,sizeof(water_record));

    water_record_index=0;

    if(sizeof(water_gate_param_t)!=jmesh_save_read(WATER_GATE_PARAM,sizeof(water_gate_param_t),(unsigned char *)&gate_param)){
         gate_param.record_interval_min=WATER_RECORD_INTERVAL_MIN_DEFAULT;
         gate_param.recv_interval_min=WATER_RECV_INTERVAL_MIN_DEFAULT;
         gate_param.recv_window_sec=WATER_RECV_WINDOW_SEC_DEFAULT;
    }

    //water_gate_start_recv();
    os_timer_event_set(&record_timer,gate_param.record_interval_min*60*1000UL,(os_timer_event_caller_t)water_gate_record,NULL);
}
int water_gate_list_node(unsigned char length,unsigned char* buffer)
{
    int i,j;
    for(i=0,j=0;i<WATER_NODE_SIZE&&j+9<=length;i++){
        if(water_node[i].addr!=0){
            buffer[j++]=water_node[i].addr;
            buffer[j++]=water_node[i].addr>>8;
            memcpy(buffer+j,water_node[i].mac,7);
            j+=7;
        }
    }
    return j;
}

water_record_t *water_gate_get_by_mac(unsigned char* mac)
{
    int i,record_index;;
    for(i=0;i<WATER_NODE_SIZE;i++){
        if(0==memcmp(water_node[i].mac,mac,7)){
            if(water_record[i][water_record_index].value!=0xffff){
                return &water_record[i][water_record_index];
            }
            record_index=(water_record_index+WATER_NODE_RECORD_SIZE-1)%WATER_NODE_RECORD_SIZE;
            return &water_record[i][record_index];
        }
    }
    return NULL;
}
water_record_t *water_gate_get_by_addr(unsigned short addr)
{
    int i,record_index;;
    for(i=0;i<WATER_NODE_SIZE;i++){
        if(water_node[i].addr==addr){
            if(water_record[i][water_record_index].value!=0xffff){
                //JMESH_LOGI("get1","r_idx:%d,val:%d\n",water_record_index,water_record[i][water_record_index].value);
                return &water_record[i][water_record_index];
            }
            record_index=(water_record_index+WATER_NODE_RECORD_SIZE-1)%WATER_NODE_RECORD_SIZE;
            //JMESH_LOGI("get2","idx:%d,r_idx:%d,val:%d\n",record_index,water_record_index,water_record[i][record_index].value);
            return &water_record[i][record_index];
        }
    }
    return NULL;
}
void HexToBCD(unsigned long hex,unsigned char* BCD,unsigned char len)
{
    unsigned int temp=0;
	int i=0;


     for(i=len-1; i>=0; i--)
      {
          temp = hex%100;
          BCD[i] = ((temp/10)<<4) + ((temp%10) & 0x0F);
          hex /= 100;
      }
}
int water_gate_188(unsigned char in_len,unsigned char out_len,unsigned char* in,unsigned char* out)
{
    pro188_t *pro188_in;
    pro188_t *pro188_out;
    unsigned long value;
    int i;
    unsigned char cs;
    water_record_t *water_record;
    if(out_len<35){
        return 0;
    }
    pro188_in=(pro188_t *)in;
    pro188_out=(pro188_t *)out;


    pro188_out->head=0x68;
    pro188_out->type=pro188_in->type;
    memcpy(pro188_out->addr,pro188_in->addr,7);
    pro188_out->c=pro188_in->c|0x80;
    pro188_out->len=0x16;

    if(pro188_in->di0==0x90&&pro188_in->di1==0x1f){
        pro188_out->di0=pro188_in->di0;
        pro188_out->di1=pro188_in->di1;
        pro188_out->ser=pro188_in->ser;

        water_record=water_gate_get_by_mac(pro188_in->addr);
        if(NULL!=water_record){
            value=water_record->value*water_record->unit/1000;
            pro188_out->data.cold_water.status[0]=water_record->status;
        }
        else{
            value=0xffff;
            pro188_out->data.cold_water.status[0]=0x00;
        }
        HexToBCD(value,pro188_out->data.cold_water.real_value,4);
        pro188_out->data.cold_water.real_value_unit=PRO188_UNIT_m3;
        memset(pro188_out->data.cold_water.date_value,0,4);
        pro188_out->data.cold_water.date_value_unit=PRO188_UNIT_m3;
        memset(pro188_out->data.cold_water.timestamp,0,7);
        pro188_out->data.cold_water.status[1]=0x00;
        for(i=0,cs=0;i<33;i++){
            cs+=out[i];
        }
        *(pro188_out->data.data+19)=cs;
        *(pro188_out->data.data+20)=0x16;
        return 35;
    }
    return 0;
}
int water_gate_645(unsigned char in_len,unsigned char out_len,unsigned char* in,unsigned char* out)
{
    return 0;
}

void water_gate_set_param(water_gate_param_t* param)
{
    memcpy(&gate_param,param,sizeof(water_gate_param_t));
    jmesh_save_write(WATER_GATE_PARAM,sizeof(water_gate_param_t),(unsigned char *)&gate_param);
}
void water_gate_get_param(water_gate_param_t* param)
{
    memcpy(param,&gate_param,sizeof(water_gate_param_t));
}
void water_gate_set_time(unsigned long second)
{
    ;
}
unsigned long water_gate_get_time(void)
{
    return os_time_get_seconds();
}
int water_gate_get_node_index_by_addr(unsigned short addr)
{
    int i;
    for(i=0;i<WATER_NODE_SIZE;i++){
        if(water_node[i].addr==addr){
            return i;
        }
    }
    return -1;
}
water_record_t *water_gate_get_history(unsigned short addr,unsigned long time)
{
    unsigned long now=water_gate_get_time();
    int node,index;
    if(0>(node=water_gate_get_node_index_by_addr(addr))){
        return NULL;
    }
    if(time>now){
        return NULL;
    }
    if(now-time>WATER_NODE_RECORD_SIZE*gate_param.record_interval_min*60){
        return NULL;
    }
    index=(now-time)/(gate_param.record_interval_min*60);
    index=(water_record_index+WATER_NODE_RECORD_SIZE-index)%WATER_NODE_RECORD_SIZE;
    //JMESH_LOGI("his","idx:%d,r_idx:%d,val:%d\n",index,water_record_index,water_record[node][index].value);
    return &water_record[node][index];

}
int water_gate_list_history(unsigned short addr,unsigned long start_time,unsigned char length,unsigned char* buffer)
{
    unsigned long now=water_gate_get_time();
    //JMESH_LOGI("now","%d\n",now);
    int node,index,i;
    if(0>(node=water_gate_get_node_index_by_addr(addr))){
        return -1;
    }
    if(start_time>=now){
        return -1;
    }
    memset(buffer,0xff,length);
    for(i=0;i<length;i+=6){

        index=(now-start_time)/(gate_param.record_interval_min*60);
        if(index<WATER_NODE_RECORD_SIZE && now >= start_time){

            index=(water_record_index+WATER_NODE_RECORD_SIZE-index)%WATER_NODE_RECORD_SIZE;
            buffer[i]=water_record[node][index].value;
            buffer[i+1]=water_record[node][index].value>>8;
            buffer[i+2]=water_record[node][index].value>>16;
            buffer[i+3]=water_record[node][index].value>>24;
            buffer[i+4]=water_record[node][index].unit;
            buffer[i+5]=water_record[node][index].status;
        }
        start_time+=gate_param.record_interval_min*60;

    }
    if(now-start_time>WATER_NODE_RECORD_SIZE*gate_param.record_interval_min*60){
        return 0xffff;
    }
    index=(now-start_time)/(gate_param.record_interval_min*60);
    index=(water_record_index+WATER_NODE_RECORD_SIZE-index)%WATER_NODE_RECORD_SIZE;
    return water_record[node][index].value;
}

int water_gate_insert_node(unsigned char* mac,unsigned short src)
{
    int i,index=-1;
    for(i=0;i<WATER_NODE_SIZE;i++){
        if(water_node[i].addr==src){
            if(0!=memcmp(water_node[i].mac,mac,7)){
                memcpy(water_node[i].mac,mac,7);
            }
            return i;
        }
        if(index==-1){
            if(water_node[i].addr==0){
                index=i;
            }
        }
    }
    if(index>=0){
        water_node[index].addr=src;
        memcpy(water_node[index].mac,mac,7);
        return index;
    }
    return -1;
}
void water_gate_report_recv(unsigned char* mac,unsigned short src,unsigned long value,unsigned char unit,unsigned char status)
{
    int node_index;
    if(0<=(node_index=water_gate_insert_node(mac,src))){
        water_record[node_index][water_record_index].value=value;
        water_record[node_index][water_record_index].unit=unit;
        water_record[node_index][water_record_index].status=status;
    }
}

int water_gate_188_handler(unsigned char interface,unsigned short len,unsigned char *data)
{
    unsigned char offset;
    unsigned char in_len,out_len;
    static unsigned char buffer[40];
    in_len=pro188_pick(len,(const unsigned char*)data,&offset);
    if(in_len){
        out_len=water_gate_188(in_len,sizeof(buffer),&data[offset],buffer);
        if(out_len){
            jmesh_uart_send(JMESH_UART_0,out_len,buffer);
            return 0;
        }
    }
    return -1;
}





