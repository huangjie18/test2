#include"jmesh_network.h"
#include"jmesh_netkey.h"
#include"jmesh_interface.h"
#include"jmesh_seq.h"
#include"jmesh_filter.h"
//#include"log.h"

#include"../lower/jmesh_lower.h"

#include"../jmesh/jmesh_addr.h"
#include"../jmesh/jmesh_print.h"
#include"../jmesh/jmesh_features.h"
#include"../onchip_system/os_queue.h"
#include"../secure/jmesh_security.h"

#include"../secure/AES_CMAC/toolbox/security_kn.h"
#include"../secure/AES_CMAC/base/aes128.h"

#include"../bearer/routing/jmesh_route.h"
#include"../access/jmesh_access.h"
#include"../upper/lowpower/jmesh_lowpower.h"
#include"../upper/lowpower/jmesh_friend.h"
OS_QUEUE_CREATE(cipher_pdu_queue, JMESH_NETWORK_CIPHER_PDU_CACHE_SIZE, JMESH_NETWORK_CIPHER_PDU_CACHE_NUM);
OS_QUEUE_CREATE(plain_pdu_queue, JMESH_NETWORK_PLAIN_PDU_CACHE_SIZE, JMESH_NETWORK_PLAIN_PDU_CACHE_NUM);


void jmesh_network_init(void)
{
    os_queue_init(cipher_pdu_queue);
    os_queue_init(plain_pdu_queue);
}

int network_cipher_cache(unsigned char* pdu)
{
    //cipher queue cache use first 2 byte because is is obfuscated
    if(!os_queue_is_in(cipher_pdu_queue,pdu+JMESH_NETWORK_CIPHER_CACHE_OFFSET)){
        os_queue_push(cipher_pdu_queue,pdu+JMESH_NETWORK_CIPHER_CACHE_OFFSET);
        return 0;
    }
    return 1;
}
static int network_plain_cache(jmesh_pdu_t* pdu)
{
    //plain queue cache use  2 byte seq and src because each new pdu has a difference seq for a src
    if(!os_queue_is_in(plain_pdu_queue,pdu->pdu+JMESH_NETWORK_PLAIN_CACHE_OFFSET)){
        os_queue_push(plain_pdu_queue,pdu->pdu+JMESH_NETWORK_PLAIN_CACHE_OFFSET);
        return 0;
    }
    return 1;
}

/*
Privacy Random = (EncDST || EncTransportPDU || NetMIC)[0-6]
PECB = e (PrivacyKey, 0x0000000000 || IV Index || Privacy Random)
ObfuscatedData = (CTL || TTL || SEQ || SRC)^  PECB[0-5]

Privacy Random = (EncDST || EncTransportPDU || NetMIC)[0-6]
PECB = e (PrivacyKey, 0x0000000000 || IV Index || Privacy Random)
(CTL || TTL || SEQ || SRC) = ObfuscatedData ^ PECB[0-5]
*/
static void network_obfuscation(jmesh_pdu_t* pdu,unsigned char* privacy_key,unsigned long iv_index)
{
    int i;
    jmesh_key_t random;
    jmesh_key_t PECB;

    random[0]=0;
    random[1]=0;
    random[2]=0;
    random[3]=0;
    random[4]=0;

    random[5]=(unsigned char)(iv_index>>24);
    random[6]=(unsigned char)(iv_index>>16);
    random[7]=(unsigned char)(iv_index>>8);
    random[8]=(unsigned char)iv_index;

    memcpy(random+9,pdu->network.DST,7);

    aes128_encryption(privacy_key,random,PECB);

    for(i=0;i<6;i++){
        pdu->pdu[i+3] = pdu->pdu[i+3] ^ PECB[i];
	}
}

int jmesh_network_send(unsigned short netkey_index,jmesh_pdu_t* pdu)
{
    jmesh_nonce_t nonce;
    unsigned short src,dst;
    unsigned long iv_index,seq;
    jmesh_netkey_t* netkey;
    jmesh_pdu_t* p;
    unsigned char ttl = pdu->network.TTL;
    int cache_flag=0;
    unsigned char ctl=0;
    JMESH_BIG_ENDIAN_PICK2(dst,pdu->network.DST);
    iv_index=jmesh_netkey_get_iv_index(netkey_index);
    if(jmesh_netkey_get_iv_update_flag(netkey_index)){//when iv index updating,use iv_index-1 to send data
        iv_index-=1;
    }
    pdu->network.IVI=iv_index&1;

    if(jmesh_interface_filtering(JMESH_INTERFACE_LOCAL,dst)){
        JMESH_LOGD("network","local recv dst=0x%x\n",dst);
        if(dst<0x8000){
            return jmesh_lower_recv(netkey_index,pdu);
        }
        else{
            if(NULL!=(p=jmesh_pdu_new(pdu->length+JMESH_LOWER_UNSEGMENT_PDU_OFFSET))){
                memcpy(p->pdu,pdu->pdu,pdu->length+JMESH_LOWER_UNSEGMENT_PDU_OFFSET);
                jmesh_lower_recv(netkey_index,p);
            }
            else
			      {
			          JMESH_LOGW(1,"network","new pdu len=%d fail\n",pdu->length+pdu->length+JMESH_LOWER_UNSEGMENT_PDU_OFFSET);
                jmesh_pdu_free(pdu);
				        return -1;
			      }
        }
    }

    if(NULL==(netkey=jmesh_netkey_get_by_index(netkey_index))){
        JMESH_LOGE(1,"network","no find netkey index=%x\n",netkey_index);
        jmesh_pdu_free(pdu);
        return -1;
    }

    seq=jmesh_seq_get();
    jmesh_seq_add();
    JMESH_BIG_ENDIAN_FILL3(seq,pdu->network.SEQ);

    JMESH_BIG_ENDIAN_PICK2(src,pdu->network.SRC);


    pdu->network.NID=JMESH_NETKEY_GET_NID(netkey);
    jmesh_generate_network_nonce(nonce,pdu->pdu[3],seq,src,iv_index);

    if(pdu->network.access.SEG){
        pdu->network.length+=JMESH_PDU_SEGMENT_HEAD_SIZE - JMESH_PDU_NETWORK_DST_OFFSET;
    }
    else{
        pdu->network.length+=JMESH_PDU_UNSEGMENT_HEAD_SIZE - JMESH_PDU_NETWORK_DST_OFFSET;
    }

    JMESH_LOGD_BUFFER("network",pdu->length+JMESH_PDU_INTERFACE_HEAD_SIZE,pdu->pdu,"send plain:");
    ctl=pdu->network.CTL;
    if(pdu->network.CTL==1){
        jmesh_security_encrypt(pdu->network.length, pdu->network.DST, nonce, JMESH_NETKEY_GET_ENCRYPTION_KEY(netkey), JMESH_PDU_NETWORK_CONTROL_MIC_SIZE);
        pdu->network.length+=(JMESH_PDU_NETWORK_CONTROL_MIC_SIZE+JMESH_PDU_NETWORK_HEAD_SIZE);
    }
    else{
        jmesh_security_encrypt(pdu->network.length, pdu->network.DST, nonce, JMESH_NETKEY_GET_ENCRYPTION_KEY(netkey), JMESH_PDU_NETWORK_ACCESS_MIC_SIZE);
        pdu->network.length+=(JMESH_PDU_NETWORK_ACCESS_MIC_SIZE+JMESH_PDU_NETWORK_HEAD_SIZE);
    }
    if(pdu->network.TTL>1)
    {
        network_plain_cache(pdu);
    }
    network_obfuscation(pdu,JMESH_NETKEY_GET_PRIVACY_KEY(netkey),iv_index);
    JMESH_LOGD_BUFFER("network",pdu->length+2,pdu->pdu,"send cipher:");
    if(0==ctl){
        if(JMESH_FEATURE_ENABLE==jmesh_get_lowpower_feature()){
            if(0==jmesh_lowpower_cache_push(pdu)){
                cache_flag=1;
            }
            else{
                jmesh_pdu_free(pdu);
                return -1;
            }
        }
        else if(JMESH_FEATURE_ENABLE==jmesh_get_friend_feature()){
            if(0==jmesh_friend_pdu_cache(dst,pdu)){
                cache_flag=1;
            }
        }
    }
    if(0==cache_flag){
        jmesh_gatt_adv_send(dst,ttl,pdu);
        jmesh_pdu_free(pdu);
    }
    return 0;
}

int jmesh_network_recv(jmesh_pdu_t* pdu)
{
    jmesh_netkey_t* netkey=NULL;
    jmesh_nonce_t nonce;
    unsigned short src,dst,netkey_index;
    unsigned long iv_index,seq;
    unsigned char ivi;
    unsigned char ttl;
    int cache_ret=-1;
    JMESH_LOGD_BUFFER("network",pdu->length+2,pdu->pdu,"recv cipher:");

    while(NULL!=(netkey=jmesh_netkey_get_by_nid(pdu->network.NID,netkey))){

        iv_index=JMESH_NETKEY_GET_IV_INDEX(netkey);
        ivi=(iv_index&1);
        if(ivi!=pdu->network.IVI){//iv index-1
            iv_index-=1;
        }
        network_obfuscation(pdu,JMESH_NETKEY_GET_PRIVACY_KEY(netkey),iv_index);
        if(0!=network_plain_cache(pdu)){
            jmesh_pdu_free(pdu);
            JMESH_LOGD("network","recv discart repeat\n");
            return -1;
        }

        JMESH_BIG_ENDIAN_PICK3(seq,pdu->network.SEQ);
        JMESH_BIG_ENDIAN_PICK2(src,pdu->network.SRC);

        if(0==jmesh_seq_is_valid(src,pdu->network.IVI,ivi,seq)){
            JMESH_LOGI("network","recv old seq=%0x4x,src:%d\n",seq,src);
            break;
        }
        jmesh_generate_network_nonce(nonce,pdu->pdu[3],seq,src,iv_index);
        pdu->network.length-=JMESH_PDU_NETWORK_HEAD_SIZE;

        if(pdu->network.CTL){
            if(0>=jmesh_security_decrypt(pdu->network.length, pdu->network.DST, nonce, JMESH_NETKEY_GET_ENCRYPTION_KEY(netkey), JMESH_PDU_NETWORK_CONTROL_MIC_SIZE)){
                JMESH_LOGI("network","decrypt failed,src:%d\n",src);
                break;
            }
            pdu->network.length-=JMESH_PDU_NETWORK_CONTROL_MIC_SIZE;
        }
        else{
            if(0>=jmesh_security_decrypt(pdu->network.length, pdu->network.DST, nonce, JMESH_NETKEY_GET_ENCRYPTION_KEY(netkey), JMESH_PDU_NETWORK_ACCESS_MIC_SIZE)){
                JMESH_LOGI("network","decrypt failed,src:%d\n",src);
                break;
            }
            pdu->network.length-=JMESH_PDU_NETWORK_ACCESS_MIC_SIZE;
        }

        JMESH_LOGD_BUFFER("network",pdu->length+2,pdu->pdu,"recv plain:");
        jmesh_seq_set(src,pdu->network.IVI,seq);

        netkey_index=JMESH_NETKEY_GET_INDEX(netkey);
        JMESH_BIG_ENDIAN_PICK2(dst,pdu->network.DST);
        if(pdu->network.TTL>0){
            pdu->network.TTL-=1;
        }
        if((JMESH_FEATURE_ENABLE==jmesh_get_relay_feature()||JMESH_FEATURE_ENABLE==jmesh_get_friend_feature()) && 0==jmesh_is_my_addr(dst) && pdu->network.TTL>0 && !pdu->network.CTL){
            jmesh_pdu_t* relay_pdu;
            JMESH_LOGI("network","relay dst=0x%x,ttl=%d\n",dst,pdu->network.TTL);
			      if(NULL==(relay_pdu=jmesh_pdu_new(pdu->network.length+JMESH_PDU_NETWORK_ACCESS_MIC_SIZE+JMESH_PDU_NETWORK_HEAD_SIZE+JMESH_PDU_INTERFACE_HEAD_SIZE))){
                JMESH_LOGW(1,"network","new pdu len=%d failed\n",pdu->network.length+JMESH_PDU_NETWORK_ACCESS_MIC_SIZE+JMESH_PDU_NETWORK_HEAD_SIZE+JMESH_PDU_INTERFACE_HEAD_SIZE);
				        jmesh_pdu_free(pdu);
					      return -1;
				    }
				    memcpy(relay_pdu->pdu,pdu->pdu,pdu->network.length+JMESH_PDU_NETWORK_ACCESS_MIC_SIZE+JMESH_PDU_NETWORK_HEAD_SIZE+JMESH_PDU_INTERFACE_HEAD_SIZE);
            nonce[1]-=1;             //attention: nonce[1] is ctl&ttl,here ttl dec 1
            ttl = relay_pdu->network.TTL;
            jmesh_security_encrypt(relay_pdu->network.length, relay_pdu->network.DST, nonce, JMESH_NETKEY_GET_ENCRYPTION_KEY(netkey), JMESH_PDU_NETWORK_ACCESS_MIC_SIZE);
            relay_pdu->network.length+=(JMESH_PDU_NETWORK_ACCESS_MIC_SIZE+JMESH_PDU_NETWORK_HEAD_SIZE);
            network_obfuscation(relay_pdu,JMESH_NETKEY_GET_PRIVACY_KEY(netkey),iv_index);

            JMESH_LOGD_BUFFER("network",pdu->length+2,pdu->pdu,"relay cipher:");
            if(JMESH_FEATURE_ENABLE==jmesh_get_friend_feature()){
                cache_ret=jmesh_friend_pdu_cache(dst,relay_pdu);
            }
            if(-1==cache_ret){
                if(JMESH_FEATURE_ENABLE==jmesh_get_relay_feature()){
                    jmesh_gatt_adv_send(dst,ttl,relay_pdu);
                }
                jmesh_pdu_free(relay_pdu);
            }
        }
        pdu->network.length-=pdu->network.access.SEG ? (JMESH_PDU_SEGMENT_HEAD_SIZE-JMESH_PDU_NETWORK_DST_OFFSET) : (JMESH_PDU_UNSEGMENT_HEAD_SIZE-JMESH_PDU_NETWORK_DST_OFFSET);//dec seg head
        if(jmesh_interface_filtering(JMESH_INTERFACE_LOCAL,dst)){
            //JMESH_LOGI("low recv","%d\n",pdu->length);
            return jmesh_lower_recv(netkey_index,pdu);
        }
        break;
    }
    jmesh_pdu_free(pdu);
    return -1;

}
void jmesh_network_reset_pdu_ttl(unsigned char ttl,jmesh_pdu_t* pdu)
{
    jmesh_netkey_t* netkey;
    unsigned long iv_index;
    unsigned char ivi;

    netkey=jmesh_netkey_get_by_index(0);

    iv_index=JMESH_NETKEY_GET_IV_INDEX(netkey);
    ivi=(iv_index&1);
    if(ivi!=pdu->network.IVI){
        iv_index-=1;
    }
    network_obfuscation(pdu,JMESH_NETKEY_GET_PRIVACY_KEY(netkey),iv_index);
    if(pdu->network.TTL>=ttl)
        pdu->network.TTL-=ttl;
    else{
        pdu->network.TTL=0;
    }
    network_obfuscation(pdu,JMESH_NETKEY_GET_PRIVACY_KEY(netkey),iv_index);
}
