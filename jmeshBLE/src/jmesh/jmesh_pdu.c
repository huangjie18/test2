#include"jmesh_pdu.h"
#include"../onchip_system/os_memb.h"
#include"../driver/jmesh_system.h"


OS_MEMB_CREATE(jmesh_long_pdu_memb,JMESH_LONG_PDU_SIZE,JMESH_LONG_PDU_CACHE_NUM);
OS_MEMB_CREATE(jmesh_short_pdu_memb,JMESH_SHORT_PDU_SIZE,JMESH_SHORT_PDU_CACHE_NUM);



void jmesh_pdu_init(void)
{
    os_memb_init(jmesh_long_pdu_memb);
    os_memb_init(jmesh_short_pdu_memb);
}
jmesh_pdu_t* jmesh_pdu_new(int length)
{
    jmesh_pdu_t* pdu=NULL;
    if(length<=JMESH_SHORT_PDU_SIZE){
        pdu=os_memb_new(jmesh_short_pdu_memb);
    }
    else if(length<=JMESH_LONG_PDU_SIZE){
        pdu=os_memb_new(jmesh_long_pdu_memb);
    }
    return pdu;
}
void jmesh_pdu_free(void* pdu)
{
    os_memb_free(jmesh_short_pdu_memb,(jmesh_pdu_t*)pdu);
    os_memb_free(jmesh_long_pdu_memb,(jmesh_pdu_t*)pdu);
}

//test
#include"jmesh_print.h"
void jmesh_pdu_log(void)
{
	int i;
	jmesh_pdu_t* pdu;
	for(i=0;i<JMESH_SHORT_PDU_CACHE_NUM;i++){
		pdu=os_memb_get_by_index(jmesh_short_pdu_memb,i);
		JMESH_LOGI_BUFFER("log",JMESH_SHORT_PDU_CACHE_NUM,pdu->pdu,"pdu:");
	}
}

