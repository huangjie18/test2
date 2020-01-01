#include"jmesh_lower.h"
#include"jmesh_segment.h"
#include"../network/jmesh_network.h"
#include"../upper/jmesh_upper.h"
#include"../jmesh/jmesh_pdu.h"
#include"../jmesh/jmesh_print.h"


int jmesh_lower_recv(unsigned short netkey_index,jmesh_pdu_t* pdu)
{
    if(pdu->network.access.SEG==1){
        JMESH_LOGD("lower","lower segment recv\n");
        jmesh_resegment_recv(netkey_index,pdu);
    }
    else{
        JMESH_LOGD("lower","lower unsegment recv\n");
        jmesh_upper_recv(netkey_index,pdu->network.length,pdu);
    }
    return 0;
}

int jmesh_lower_send(unsigned short netkey_index,jmesh_pdu_t* pdu)
{
	if(pdu->network.access.SEG)
	{
	    JMESH_LOGD("lower","lower segment send\n");
		  return jmesh_segment_send(netkey_index,pdu->network.length,pdu);
	}
	else
	{
	    JMESH_LOGD("lower","lower unsegment send\n");
		  return jmesh_network_send(netkey_index,pdu);
	}
}
