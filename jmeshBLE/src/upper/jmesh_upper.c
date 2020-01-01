#include"jmesh_upper.h"
#include"../jmesh/jmesh_types.h"
#include"../jmesh/jmesh_addr.h"
#include"../jmesh/jmesh_pdu.h"
#include"../jmesh/jmesh_print.h"
#include"../jmesh/jmesh_device.h"
#include"../jmesh/jmesh_task.h"
//#include"log.h"
#include"../network/jmesh_seq.h"
#include"../network/jmesh_netkey.h"
#include"../lower/jmesh_lower.h"
#include"../lower/jmesh_segment.h"
#include"../network/jmesh_interface.h"
#include"../secure/jmesh_security.h"
#include"../foundation/state/jmesh_TTL.h"
#include"../access/jmesh_access.h"
#include"../upper/control/jmesh_control.h"
#include"../bearer/routing/jmesh_routing_neighbor.h"
#include"../jmesh/jmesh_print.h"
int jmesh_upper_access_send(jmesh_pdu_t* pdu){
    unsigned short src,dst,length;
    jmesh_appkey_t* appkey;
    unsigned long seq,iv_index;
    int ret;
    jmesh_nonce_t nonce;
    unsigned char channel;
    int unseg_size;

    src=pdu->access.head.src;
    dst=pdu->access.head.dst;
    appkey=pdu->access.head.appkey;
    length=pdu->access.head.length;
    channel=pdu->access.head.channel;


    seq=jmesh_seq_get();
    iv_index=jmesh_netkey_get_iv_index(appkey->netkey_index);

    if(jmesh_netkey_get_iv_update_flag(appkey->netkey_index)){//when iv index updating,use iv_index-1 to send data
        iv_index-=1;
    }

    jmesh_generate_application_nonce(nonce,0,seq,src,dst,iv_index);
    unseg_size=(JMESH_SEND_ADV_CHANNEL==channel)?(31-(JMESH_PDU_ACCESS_UNSEGMENT_PACKAGE_SIZE)):(JMESH_PDU_ACCESS_UNSEGMENT_DATA_SIZE);
    if(length<=unseg_size)
    {
        pdu->network.access.SEG=0;
        ret=jmesh_security_encrypt(length,pdu->network.access.pdu,nonce,appkey->key,JMESH_PDU_UPPER_ACCESS_MIC_SIZE);
    }else
	  {
        pdu->network.access.SEG=1;
        ret=jmesh_security_encrypt(length,pdu->network.access.segment.pdu,nonce,appkey->key,JMESH_PDU_UPPER_ACCESS_MIC_SIZE);
	  }
    if(ret!=length+JMESH_PDU_UPPER_ACCESS_MIC_SIZE){
        JMESH_LOGE(1,"upper","access encrypt fail ret %d\n",ret);
        jmesh_pdu_free(pdu);
        return -1;
    }
    JMESH_LOGD_BUFFER("upper",ret,pdu->network.access.pdu,"send access cipher pdu:");
	  pdu->network.length = ret;
    pdu->network.TTL=jmesh_TTL_get_default();
    pdu->network.CTL=0;

    JMESH_BIG_ENDIAN_FILL3(seq,pdu->network.SEQ);
    JMESH_BIG_ENDIAN_FILL2(src,pdu->network.SRC);
    JMESH_BIG_ENDIAN_FILL2(dst,pdu->network.DST);

    pdu->network.access.AKF=1;
    pdu->network.access.AID=appkey->aid;
    pdu->network.itf=channel;
    return jmesh_lower_send(appkey->netkey_index,pdu);
}
int jmesh_upper_config_send(jmesh_pdu_t* pdu){
    unsigned short src,dst,length,netkey_index;
    unsigned char* devkey;
    unsigned long seq,iv_index;
    int ret;
    jmesh_nonce_t nonce;

    netkey_index=pdu->access.head.netkey_index;
    src=pdu->access.head.src;
    dst=pdu->access.head.dst;
    length=pdu->access.head.length;

    if(NULL==(devkey=jmesh_device_get_devkey(src))){
        JMESH_LOGW(1,"upper","dst=0x%x no devkey find\n",dst);
        jmesh_pdu_free(pdu);
        return -1;
    }
    seq=jmesh_seq_get();
    iv_index=jmesh_netkey_get_iv_index(netkey_index);

    if(jmesh_netkey_get_iv_update_flag(netkey_index)){//when iv index updating,use iv_index-1 to send data
        iv_index-=1;
    }

    jmesh_generate_device_nonce(nonce,0,seq,src,dst,iv_index);

    if(length<=JMESH_PDU_ACCESS_UNSEGMENT_DATA_SIZE){
        pdu->network.access.SEG=0;
        ret=jmesh_security_encrypt(length,pdu->network.access.pdu,nonce,devkey,JMESH_PDU_UPPER_ACCESS_MIC_SIZE);
    }
    else{
        pdu->network.access.SEG=1;
        ret=jmesh_security_encrypt(length,pdu->network.access.segment.pdu,nonce,devkey,JMESH_PDU_UPPER_ACCESS_MIC_SIZE);
	}

    if(ret!=length+JMESH_PDU_UPPER_ACCESS_MIC_SIZE){
        JMESH_LOGE(1,"upper","config encrypt fail ret %d\n",ret);
        jmesh_pdu_free(pdu);
        return -1;
    }

    JMESH_LOGD_BUFFER("upper",ret,pdu->network.access.pdu,"send config cipher pdu:");
	  pdu->network.length = ret;
    pdu->network.TTL=jmesh_TTL_get_default();
    pdu->network.CTL=0;

    JMESH_BIG_ENDIAN_FILL3(seq,pdu->network.SEQ);
    JMESH_BIG_ENDIAN_FILL2(src,pdu->network.SRC);
    JMESH_BIG_ENDIAN_FILL2(dst,pdu->network.DST);

    pdu->network.access.AKF=0;
    pdu->network.access.AID=0;
    pdu->network.itf=JMESH_INTERFACE_ADV;
    return jmesh_lower_send(netkey_index,pdu);
}

int jmesh_upper_control_send(unsigned short src,unsigned short dst,unsigned short netkey_index,unsigned short length,unsigned char* data);
int jmesh_upper_recv(unsigned short netkey_index,unsigned short length,jmesh_pdu_t* pdu)
{
    unsigned short src,dst;
    unsigned long seq,seqZero,iv_index;
    int ret;
    unsigned char* key;
    jmesh_appkey_t* appkey;
    jmesh_nonce_t nonce;
    JMESH_LOGD_BUFFER("upper",length,pdu->network.access.pdu,"recv cipher:");
    if(pdu->network.CTL){
        return jmesh_control_recv(netkey_index,length,pdu);//return jmesh_control_process(netkey_index,length,pdu);
    }

    JMESH_BIG_ENDIAN_PICK3(seq,pdu->network.SEQ);
    if(pdu->network.access.SEG){//reset seq,seq cannot > seqZero+0x2000
        seqZero=jmesh_lower_get_segment_seqZero(pdu->network.access.segment.head);
        seqZero|=(seq&0xffffe000);
        if(seqZero>seq)
            seqZero-=0x2000;
        seq=seqZero;
    }
    JMESH_BIG_ENDIAN_PICK2(src,pdu->network.SRC);
    JMESH_BIG_ENDIAN_PICK2(dst,pdu->network.DST);

    iv_index=jmesh_netkey_get_iv_index(netkey_index);
    if(pdu->network.IVI!=(iv_index&1)){
        iv_index-=1;
    }
    ret=-1;
    if(pdu->network.access.AKF){
        jmesh_generate_application_nonce(nonce,0,seq,src,dst,iv_index);

        appkey=NULL;
        while(NULL!=(appkey=jmesh_appkey_get_by_aid(netkey_index,pdu->network.access.AID,appkey))){
            if(pdu->network.access.SEG){
                if(0<(ret=jmesh_security_decrypt(length,pdu->network.access.segment.pdu,nonce,appkey->key,JMESH_PDU_UPPER_ACCESS_MIC_SIZE))){
                    JMESH_LOGD_BUFFER("upper",ret,pdu->network.access.segment.pdu,"recv access plain:");
                    pdu->access.head.src=src;
                    pdu->access.head.dst=dst;
                    pdu->access.head.appkey=appkey;
                    pdu->access.head.length=ret;
                    if(OS_ERR_EVENT_FULL == os_event_post(&jmesh_task,JMESH_EVENT_ACCESS_RECV,pdu))
					{
						JMESH_LOGW(1,"upper","os_event_post failed\n");
						jmesh_pdu_free(pdu);
					}
                    return 0;
                }
            }
            else{
                if(0<(ret=jmesh_security_decrypt(length,pdu->network.access.pdu,nonce,appkey->key,JMESH_PDU_UPPER_ACCESS_MIC_SIZE))){
                    JMESH_LOGD_BUFFER("upper",ret,pdu->network.access.pdu,"recv access plain:");
                    pdu->access.head.src=src;
                    pdu->access.head.dst=dst;
                    pdu->access.head.appkey=appkey;
                    pdu->access.head.length=ret;
                    //JMESH_LOGI("upper recv","%d\n",pdu->length);
                    if(OS_ERR_EVENT_FULL == os_event_post(&jmesh_task,JMESH_EVENT_ACCESS_RECV,pdu))
					{
						JMESH_LOGW(1,"upper","os_event_post failed\n");
						jmesh_pdu_free(pdu);
					}
                    return 0;
                }
            }
        }
    }
    else{
        if((dst == jmesh_get_primary_addr()) && (NULL != (key = jmesh_device_get_devkey(dst)))){
            jmesh_generate_device_nonce(nonce,0,seq,src,dst,iv_index);
            if(pdu->network.access.SEG){
                if(0<(ret=jmesh_security_decrypt(length,pdu->network.access.segment.pdu,nonce,key,JMESH_PDU_UPPER_ACCESS_MIC_SIZE))){
                    JMESH_LOGD_BUFFER("upper",length,pdu->network.access.segment.pdu,"config recv segment plain:");
                    pdu->access.head.src=src;
                    pdu->access.head.dst=dst;
                    pdu->access.head.netkey_index=netkey_index;
                    pdu->access.head.length=ret;
                    if(OS_ERR_EVENT_FULL == os_event_post(&jmesh_task,JMESH_EVENT_CONFIG_RECV,pdu))
					{
						JMESH_LOGW(1,"upper","os_event_post failed\n");
						jmesh_pdu_free(pdu);
					}
                    return 0;
                }
            }
            else{
                if(0<(ret=jmesh_security_decrypt(length,pdu->network.access.pdu,nonce,key,JMESH_PDU_UPPER_ACCESS_MIC_SIZE))){
                    JMESH_LOGD_BUFFER("upper",length,pdu->network.access.pdu,"config recv unsegment plain:");
                    pdu->access.head.src=src;
                    pdu->access.head.dst=dst;
                    pdu->access.head.netkey_index=netkey_index;
                    pdu->access.head.length=ret;
                    if(OS_ERR_EVENT_FULL == os_event_post(&jmesh_task,JMESH_EVENT_CONFIG_RECV,pdu))
					{
						JMESH_LOGW(1,"upper","os_event_post failed\n");
						jmesh_pdu_free(pdu);
					}
                    return 0;
                }
            }
        }
    }
    jmesh_pdu_free(pdu);
    return 0;
}

