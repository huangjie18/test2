#include"jmesh_lower.h"
#include"jmesh_segment.h"
#include"../onchip_system/os_timer_event.h"
#include"../onchip_system/os_memb.h"
#include"../upper/jmesh_upper.h"
#include"../upper/control/jmesh_control.h"
#include"../network/jmesh_network.h"
#include"../network/jmesh_netkey.h"
#include"../jmesh/jmesh_addr.h"
#include"../jmesh/jmesh_print.h"
#include"../foundation/state/jmesh_TTL.h"
#include"../access/jmesh_access.h"


typedef struct st_lower_segment_cache{
    jmesh_pdu_t* pdu;
    unsigned short seqZero;
    unsigned short src;
    unsigned long ack_block;
    unsigned short length:12;
    unsigned short send_flag:1;
    unsigned short retransmit_count:3;
    unsigned short netkey_index;
    os_timer_event_t timer;//1s
}lower_segment_cache_t;

OS_MEMB_CREATE(lower_segment_cache_memb,sizeof(lower_segment_cache_t),JMESH_SEGMENT_CACHE_SIZE);
void jmesh_segment_init(void)
{
    os_memb_init(lower_segment_cache_memb);
}

static int lower_segment_send_ack(unsigned short netkey_index,jmesh_pdu_t* pdu,unsigned long block_ack);
static int lower_segment_send_deny_ack(unsigned short netkey_index,jmesh_pdu_t* pdu);
static void lower_segment_cache_free(lower_segment_cache_t* cache);
static void lower_segment_retransmit(lower_segment_cache_t* cache);
static int lower_segment_transmit(jmesh_pdu_t* pdu,unsigned short netkey_index,unsigned short length,unsigned long ack_block);

void lower_segment_cache_free(lower_segment_cache_t* cache)
{
    jmesh_pdu_free(cache->pdu);
    os_timer_event_remove(&cache->timer);
    os_memb_free(lower_segment_cache_memb,cache);
}
void lower_segment_retransmit(lower_segment_cache_t* cache)
{
    if(cache->retransmit_count){
        cache->retransmit_count--;
        lower_segment_transmit(cache->pdu,cache->netkey_index,cache->length,cache->ack_block);
        os_timer_event_restart(&cache->timer);
    }
    else{
        lower_segment_cache_free(cache);
    }
}
static unsigned long ack_block_mask_table[32]={
    0x00000001,0x00000002,0x00000004,0x00000008,
    0x00000010,0x00000020,0x00000040,0x00000080,
    0x00000100,0x00000200,0x00000400,0x00000800,
    0x00001000,0x00002000,0x00004000,0x00008000,
    0x00010000,0x00020000,0x00040000,0x00080000,
    0x00100000,0x00200000,0x00400000,0x00800000,
    0x01000000,0x02000000,0x04000000,0x08000000,
    0x10000000,0x20000000,0x40000000,0x80000000,
};
static unsigned long segment_block_table[32]={
    0x00000001,0x00000003,0x00000007,0x0000000f,
    0x0000001f,0x0000003f,0x0000007f,0x000000ff,
    0x000001ff,0x000003ff,0x000007ff,0x00000fff,
    0x00001fff,0x00003fff,0x00007fff,0x0000ffff,
    0x0001ffff,0x0003ffff,0x0007ffff,0x000fffff,
    0x001fffff,0x003fffff,0x007fffff,0x00ffffff,
    0x01ffffff,0x03ffffff,0x07ffffff,0x0fffffff,
    0x1fffffff,0x3fffffff,0x7fffffff,0xffffffff,
};
int lower_segment_transmit(jmesh_pdu_t* pdu,unsigned short netkey_index,unsigned short length,unsigned long ack_block)
{
    jmesh_pdu_t* seg_pdu;
    unsigned char segN;
    int i;

    segN=jmesh_lower_get_segment_segN(pdu->network.access.segment.head);
    for( i=0;i<=segN;i++){
        if(((ack_block&ack_block_mask_table[i])==0)){//send a not acknowledged segment
            if(NULL==(seg_pdu=JMESH_PDU_NEW_SHORT())){
                JMESH_LOGW(1,"lower","new short pdu failed\n");
                break;
            }
            memcpy(seg_pdu->pdu,pdu->pdu,JMESH_LOWER_SEGMENT_PDU_OFFSET);
            if(i==segN){
                seg_pdu->network.length=length%JMESH_SEGMENT_MESSAGE_SIZE(pdu->network.CTL);//get tail length
                if(seg_pdu->network.length==0){
                    seg_pdu->network.length=JMESH_SEGMENT_MESSAGE_SIZE(pdu->network.CTL);
                }
            }
            else{
                seg_pdu->network.length=JMESH_SEGMENT_MESSAGE_SIZE(pdu->network.CTL);
            }
            jmesh_lower_set_segment_segO(seg_pdu->network.access.segment.head,i);
            memcpy(seg_pdu->network.access.segment.pdu,pdu->network.access.segment.pdu+i*JMESH_SEGMENT_MESSAGE_SIZE(pdu->network.CTL),seg_pdu->network.length);
            JMESH_LOGD("seg send","segO:%d,len:%d\n",i,seg_pdu->network.length);
            jmesh_network_send(netkey_index,seg_pdu);
        }
    }
    return 0;
}

int jmesh_segment_send(unsigned short netkey_index,unsigned short length,jmesh_pdu_t* pdu)
{
    unsigned segN;
    unsigned short seqZero;
    lower_segment_cache_t* cache;

    //if(JMESH_SEND_ADV_CHANNEL==pdu->network.itf){
    //    segN=(length-1)/31;
    //}
    //else{
        segN=(length-1)/JMESH_SEGMENT_MESSAGE_SIZE(pdu->network.CTL);
    //}

    seqZero=pdu->network.SEQ[1]&0x1f;
    seqZero<<=8;
    seqZero|=pdu->network.SEQ[2];

    jmesh_lower_init_segment_head(pdu->network.access.segment.head,segN,seqZero);

    if(NULL==(cache=os_memb_new(lower_segment_cache_memb))){
        lower_segment_transmit(pdu,netkey_index,length,0);
        jmesh_pdu_free(pdu);
        return 0;
    }
    cache->pdu=pdu;
    cache->ack_block=0;
    cache->length=length;
    cache->seqZero=seqZero;
    JMESH_BIG_ENDIAN_PICK2(cache->src,pdu->network.SRC);
    cache->netkey_index=netkey_index;
    cache->retransmit_count=JMESH_SEGMENT_RETRANSMIT_COUNT;
    cache->send_flag=1;

    os_timer_event_set(&cache->timer,500+pdu->network.TTL*50,(os_timer_event_caller_t)lower_segment_retransmit,(os_data_t)cache);
    lower_segment_transmit(pdu,netkey_index,length,0);
    return 0;
}

int lower_segment_cache_send_finder(lower_segment_cache_t* cache,jmesh_pdu_t* pdu)
{
    unsigned short seqZero_new,src;
    JMESH_BIG_ENDIAN_PICK2(src,pdu->network.SRC);
    if(cache->send_flag==1&&cache->src==src){
        seqZero_new=jmesh_lower_get_segment_seqZero(pdu->network.access.segment.head);
        if(seqZero_new==cache->seqZero){
            return 0;
        }
        if(seqZero_new>cache->seqZero){//discard old
            JMESH_LOGW(1,"lower segment","recv new pdu seq=0x%x, discard unfinished pdu seq=0x%x",cache->seqZero,seqZero_new);
            lower_segment_cache_free(cache);
        }
    }
    return 1;
}

int lower_segment_cache_read_finder(lower_segment_cache_t* cache,jmesh_pdu_t* pdu)
{
    unsigned short seqZero_new,src;
    JMESH_BIG_ENDIAN_PICK2(src,pdu->network.SRC);
    if(cache->send_flag==0&&src==cache->src){
        seqZero_new=jmesh_lower_get_segment_seqZero(pdu->network.access.segment.head);
        if(seqZero_new==cache->seqZero){
            return 0;
        }
        if(seqZero_new>cache->seqZero){//discard old
            JMESH_LOGW(1,"lower segment","recv new pdu seq=0x%x, discard unfinished pdu seq=0x%x",cache->seqZero,seqZero_new);
            lower_segment_cache_free(cache);
        }
    }
    return 1;
}

int jmesh_lower_segment_recv_ack(jmesh_pdu_t* pdu)
{
    lower_segment_cache_t* cache;
    unsigned long ack_block;

    if(NULL!=(cache=os_memb_find(lower_segment_cache_memb,pdu,(os_memb_finder_t)lower_segment_cache_send_finder))){//the  seqZero format is the same in ack and segment
        ack_block=pdu->network.control.ack.ack_block[0];
        ack_block<<=8;
        ack_block|=pdu->network.control.ack.ack_block[1];
        ack_block<<=8;
        ack_block|=pdu->network.control.ack.ack_block[2];
        ack_block<<=8;
        ack_block|=pdu->network.control.ack.ack_block[3];
        if(ack_block==0){//deny_ack,cancel!
            JMESH_LOGW(1,"lower segment","recv deny ack,discart src=0x%x%x\n",pdu->network.SRC[0],pdu->network.SRC[1]);
            lower_segment_cache_free(cache);
        }
        if((~cache->ack_block)&ack_block){//new ack
            JMESH_LOGD("lower segment","recv a valid ack=0x%4x,src=0x%x%x\n",ack_block,pdu->network.SRC[0],pdu->network.SRC[1]);
            cache->ack_block|=ack_block;
            os_timer_event_restart(&cache->timer);
        }
    }
    jmesh_pdu_free(pdu);
    return 0;
}
int lower_segment_send_ack(unsigned short netkey_index,jmesh_pdu_t* pdu,unsigned long ack_block)
{
    jmesh_pdu_t* ack_pdu;//TODO: change src and dst
    unsigned short dst,src;

    JMESH_BIG_ENDIAN_PICK2(dst,pdu->network.DST);
    JMESH_BIG_ENDIAN_PICK2(src,pdu->network.SRC);

    if(JMESH_ADDR_UNICAST!=jmesh_addr_get_type(src)||JMESH_ADDR_UNICAST!=jmesh_addr_get_type(dst)){
        JMESH_LOGW(1,"lower segment","not unicast addr,send ack from 0x%x to 0x%x\n",src,dst);
        return -1;
    }

    if(NULL==(ack_pdu=JMESH_PDU_NEW_SHORT())){
        JMESH_LOGW(1,"lower segment","new short pdu failed\n");
        return -1;
    }
    memcpy(ack_pdu->pdu,pdu->pdu,JMESH_LOWER_UNSEGMENT_PDU_OFFSET);

    ack_pdu->network.IVI=(1&jmesh_netkey_get_iv_index(netkey_index));
    ack_pdu->network.TTL=jmesh_TTL_get_default();

    JMESH_BIG_ENDIAN_FILL2(dst,ack_pdu->network.SRC);
    JMESH_BIG_ENDIAN_FILL2(src,ack_pdu->network.DST);

    ack_pdu->network.CTL=1;
    ack_pdu->network.control.SEG=0;
    ack_pdu->network.control.OPCODE=JMESH_CONTROL_LOWER_SEGMENT_ACKNOWLEDGE;
    ack_pdu->network.length=sizeof(jmesh_lower_segment_acknowledgment_t);
    ack_pdu->network.control.ack.ack_block[0]=(unsigned char)(ack_block>>24);
    ack_pdu->network.control.ack.ack_block[1]=(unsigned char)(ack_block>>16);
    ack_pdu->network.control.ack.ack_block[2]=(unsigned char)(ack_block>>8);
    ack_pdu->network.control.ack.ack_block[3]=(unsigned char)ack_block;
    JMESH_LOGD("lower segment","send ack=0x%4x to dst=0x%x",ack_block,dst);
    return jmesh_network_send(netkey_index,ack_pdu);

}
int lower_segment_send_deny_ack(unsigned short netkey_index,jmesh_pdu_t* pdu)
{
    return lower_segment_send_ack(netkey_index,pdu,0);
}
int lower_resegment_timer_handler(lower_segment_cache_t* cache)
{
    lower_segment_send_ack(cache->netkey_index,cache->pdu,cache->ack_block);
    os_timer_event_set(&cache->timer,JMESH_RESEGMENT_INCOMPLETE_S*1000,(os_timer_event_caller_t)lower_segment_cache_free,cache);
    return 0;
}
int jmesh_resegment_recv(unsigned short netkey_index,jmesh_pdu_t* pdu)
{
    lower_segment_cache_t* cache;
    jmesh_pdu_t* long_pdu;
    unsigned char segO,segN;
    if(NULL==(cache=os_memb_find(lower_segment_cache_memb,pdu,(os_memb_finder_t)lower_segment_cache_read_finder))){

        JMESH_LOGD("lower segment","recv new pdu from 0x%x",pdu->network.SRC[0],pdu->network.SRC[1]);
        if(NULL==(cache=os_memb_new(lower_segment_cache_memb))){
            lower_segment_send_deny_ack(netkey_index,pdu);
            JMESH_LOGW(1,"lower segment","deny as new cache failed\n");
            jmesh_pdu_free(pdu);
            return -1;
        }
        if(NULL==(long_pdu=jmesh_pdu_new(JMESH_LOWER_SEGMENT_PDU_OFFSET+(1+jmesh_lower_get_segment_segN(pdu->network.access.segment.head))*JMESH_SEGMENT_MESSAGE_SIZE(pdu->network.CTL)))){

            JMESH_LOGW(1,"lower segment","deny as new pdu len=%d failed\n",JMESH_LOWER_SEGMENT_PDU_OFFSET+(1+jmesh_lower_get_segment_segN(pdu->network.access.segment.head))*JMESH_SEGMENT_MESSAGE_SIZE(pdu->network.CTL));
            os_memb_free(lower_segment_cache_memb,cache);
            lower_segment_send_deny_ack(netkey_index,pdu);
            jmesh_pdu_free(pdu);
            return -1;
        }
        cache->pdu=long_pdu;
        cache->ack_block=0;
        cache->send_flag=0;
        JMESH_BIG_ENDIAN_PICK2(cache->src,pdu->network.SRC);
        cache->seqZero=jmesh_lower_get_segment_seqZero(pdu->network.access.segment.head);
        memcpy(long_pdu->pdu,pdu->pdu,JMESH_LOWER_SEGMENT_PDU_OFFSET);
        os_timer_event_set(&cache->timer,350+50*cache->pdu->network.TTL,(os_timer_event_caller_t)lower_resegment_timer_handler,cache);
    }
    segO=jmesh_lower_get_segment_segO(pdu->network.access.segment.head);
    segN=jmesh_lower_get_segment_segN(pdu->network.access.segment.head);
	JMESH_LOGD("lower segment","recv segO=%d segN=%d\n",segO,segN);
    if(cache->ack_block&ack_block_mask_table[segO]){
        jmesh_pdu_free(pdu);
        JMESH_LOGD("lower segment","discart a repeat segment pdu\n");
        return 0;
    }
    if(segN!=jmesh_lower_get_segment_segN(cache->pdu->network.access.segment.head)){
        jmesh_pdu_free(pdu);
        JMESH_LOGW(1,"lower segment","segN=%d expect %d,discart\n",segN,jmesh_lower_get_segment_segN(cache->pdu->network.access.segment.head));return 0;
    }
    cache->ack_block|=ack_block_mask_table[segO];

    if(segO==segN){
        cache->length=segN*JMESH_SEGMENT_MESSAGE_SIZE(pdu->network.CTL)+pdu->network.length;
    }
    memcpy(cache->pdu->network.access.segment.pdu+segO*JMESH_SEGMENT_MESSAGE_SIZE(pdu->network.CTL),pdu->network.access.segment.pdu,pdu->network.length);
    jmesh_pdu_free(pdu);
    if(cache->ack_block==segment_block_table[segN]){
        lower_segment_send_ack(netkey_index,cache->pdu,cache->ack_block);
        os_timer_event_remove(&cache->timer);
        JMESH_LOGD_BUFFER("lower",cache->length,cache->pdu->pdu,"resegment pdu:");
        jmesh_upper_recv(netkey_index,cache->length,cache->pdu);
        os_timer_event_set(&cache->timer,0,(os_timer_event_caller_t)lower_segment_cache_free,cache);
    }
    else{
        if(cache->timer.caller!=(os_timer_event_caller_t)lower_resegment_timer_handler){

            os_timer_event_set(&cache->timer,350+50*cache->pdu->network.TTL,(os_timer_event_caller_t)lower_resegment_timer_handler,cache);
        }
    }
    return 0;
}
