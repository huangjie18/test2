#include"jmesh_control.h"
#include"../../lower/jmesh_lower.h"
#include"../jmesh_upper.h"
#include"../../jmesh/jmesh_print.h"
#include"../../network/jmesh_seq.h"
#include"../../bearer/gatt/jmesh_gatt.h"
#include"../../bearer/routing/jmesh_routing_func.h"
#include"../../jmesh/jmesh_task.h"
#include"../../access/jmesh_access.h"
#include"../lowpower/jmesh_friend.h"
#include"../lowpower/jmesh_lowpower.h"

int jmesh_control_recv(unsigned short netkey_index,unsigned short length,jmesh_pdu_t* pdu){
	unsigned short src,dst;
	unsigned char ttl = pdu->network.TTL;
	unsigned char opcode = pdu->network.control.OPCODE;
    JMESH_BIG_ENDIAN_PICK2(src,pdu->network.SRC);
    JMESH_BIG_ENDIAN_PICK2(dst,pdu->network.DST);

    switch(opcode){
        case(JMESH_CONTROL_LOWER_SEGMENT_ACKNOWLEDGE):
            jmesh_lower_segment_recv_ack(pdu);
            break;
		default :
			pdu->control.src = src;
			pdu->control.dst = dst;
			pdu->control.ttl = ttl;
			if(OS_ERR_EVENT_FULL == os_event_post(&jmesh_task,JMESH_EVENT_CONTROL_RECV,pdu))
			{
        JMESH_LOGE(1,"control recv","post error\n");
				jmesh_pdu_free(pdu);
			}
			break;
	}
    return 0;
}
int jmesh_upper_control_recv_handler(jmesh_pdu_t *pdu)
{
    //JMESH_LOGI("contl recv","src:%d\n",pdu->control.src);
    jmesh_friend_recv(pdu->control.netkey_index,pdu->control.length,pdu);
    jmesh_lowpower_recv(pdu->control.netkey_index,pdu->control.length,pdu);
    if(NULL!=jmesh_gatt_get_ready_by_addr(pdu->control.src))
    {
        jmesh_routing_recv(pdu->control.src,pdu->control.dst,pdu->control.ttl,pdu->control.OPCODE,pdu->control.itf,pdu->control.data);
    }
	  jmesh_pdu_free(pdu);
	  return 0;
}
int jmesh_upper_control_send_handler(jmesh_pdu_t *pdu)
{
	unsigned short netkey_index = pdu->control.netkey_index;
	unsigned char ttl = pdu->control.ttl ;
	unsigned short  src = pdu->control.src;
	unsigned short  dst = pdu->control.dst;
	pdu->network.TTL = ttl;
	pdu->network.CTL = 1;
	JMESH_BIG_ENDIAN_FILL2(src,pdu->network.SRC);
	JMESH_BIG_ENDIAN_FILL2(dst,pdu->network.DST);
    return jmesh_lower_send(netkey_index,pdu);
}
int jmesh_control_send(unsigned short src,unsigned short dst,unsigned short netkey_index,unsigned char opcode,unsigned char ttl,unsigned short length,unsigned char* data)
{

    jmesh_pdu_t *pdu;
	  if(NULL==(pdu=jmesh_pdu_new(length+JMESH_PDU_ACCESS_UNSEGMENT_PACKAGE_SIZE)))
	  {
		    return -1;
	  }
	  if(length<=JMESH_PDU_ACCESS_UNSEGMENT_DATA_SIZE)
    {
		    pdu->control.SEG = 0;
    }else
	  {
		    pdu->control.SEG = 1;
	  }

    pdu->control.netkey_index = netkey_index;
    pdu->control.ttl = ttl;

    pdu->control.length = length;
    pdu->control.itf   = JMESH_SEND_ALL_CHANNEL;
    pdu->control.OPCODE = opcode;
    pdu->control.src = src;
    pdu->control.dst = dst;
    memcpy(pdu->control.data,data,length);

    if(OS_ERR_EVENT_FULL == os_event_post(&jmesh_task,JMESH_EVENT_CONTROL_SEND,pdu))
    {
        JMESH_LOGE(1,"control send","post error\n");
        jmesh_pdu_free(pdu);
    }
    return 0;
}
