#ifndef JMESH_PDU_H
#define JMESH_PDU_H


#include"jmesh_config.h"
#include"../bearer/beacon/jmesh_beacon.h"



typedef struct st_jmesh_adv_pdu{
    unsigned char length;
    unsigned char ad_type;
    union{
      unsigned char para[29];
      jmesh_beacon_t beacon;
    };
    unsigned char mac[6];
    unsigned char rssi;
}jmesh_adv_pdu_t;
typedef struct st_jmesh_proxy_pdu{
    unsigned char length;
    union{
        unsigned char head;
        struct{
            unsigned char type:6;
            unsigned char SAR:2;
        };
    };
    unsigned char para[1];
}jmesh_proxy_pdu_t;


typedef struct st_jmesh_lower_segment_control_pdu{
    unsigned char head[3];
    unsigned char pdu[8];
}jmesh_lower_segment_control_pdu_t;

typedef struct st_jmesh_lower_segment_acknowledgment{
    unsigned char OBO_and_seqZero[2];//1bit OBO,13 bit seqZero, 2bit rfu
    unsigned char ack_block[4];
}jmesh_lower_segment_acknowledgment_t;

typedef struct st_jmesh_lower_control_pdu{
    unsigned char OPCODE:7;
    unsigned char SEG:1;
    union{
        jmesh_lower_segment_acknowledgment_t ack;
        jmesh_lower_segment_control_pdu_t segment;
        unsigned char pdu[11];
    };
}jmesh_lower_control_pdu_t;

typedef struct st_jmesh_lower_segment_access_pdu{
    unsigned char head[3];
    unsigned char pdu[12];
}jmesh_lower_segment_access_pdu_t;

typedef struct st_jmesh_lower_access_pdu{
    unsigned char AID:6;
    unsigned char AKF:1;
    unsigned char SEG:1;
    union{
        jmesh_lower_segment_access_pdu_t segment;
        unsigned char pdu[15];
    };
}jmesh_lower_access_pdu_t;

typedef struct st_jmesh_network_pdu{
    unsigned char length;
    unsigned char itf;

    unsigned char NID:7;
    unsigned char IVI:1;
    unsigned char TTL:7;
    unsigned char CTL:1;
    unsigned char SEQ[3];
    unsigned char SRC[2];
    unsigned char DST[2];
    union{
        jmesh_lower_control_pdu_t control;
        jmesh_lower_access_pdu_t access;
        unsigned char pdu[1];//1+15m+4mac
    };
}jmesh_network_pdu_t;

#define JMESH_SEGMENT_ACCESS_MESSAGE_SIZE JMESH_PDU_ACCESS_UNSEGMENT_DATA_SIZE

#define JMESH_LOWER_SEGMENT_MESSAGE_SIZE 8

#define JMESH_SEGMENT_MESSAGE_SIZE(ctl)  (ctl ? JMESH_LOWER_SEGMENT_MESSAGE_SIZE : JMESH_SEGMENT_ACCESS_MESSAGE_SIZE)


#define JMESH_LOWER_UNSEGMENT_PDU_OFFSET    ((int)(&((jmesh_pdu_t*)0)->network.access.pdu))
#define JMESH_LOWER_SEGMENT_PDU_OFFSET      ((int)(&((jmesh_pdu_t*)0)->network.access.segment.pdu))

#define JMESH_LOWER_UNSEGMENT_PDU_OFFSET    ((int)(&((jmesh_pdu_t*)0)->network.access.pdu))
#define JMESH_LOWER_SEGMENT_PDU_OFFSET      ((int)(&((jmesh_pdu_t*)0)->network.access.segment.pdu))

#define JMESH_PDU_UNSEGMENT_HEAD_SIZE ((int)(&((jmesh_pdu_t*)0)->network.access.pdu))/**< 12 */
#define JMESH_PDU_SEGMENT_HEAD_SIZE ((int)(&((jmesh_pdu_t*)0)->network.access.segment.pdu))/**< 15 */

#define JMESH_PDU_ACCESS_UNSEGMENT_PACKAGE_SIZE \
    (JMESH_PDU_UNSEGMENT_HEAD_SIZE + JMESH_PDU_NETWORK_ACCESS_MIC_SIZE + JMESH_PDU_UPPER_ACCESS_MIC_SIZE)/**< 12+4+4=20 */
#define JMESH_PDU_CONTROL_UNSEGMENT_PACKAGE_SIZE \
    (JMESH_PDU_UNSEGMENT_HEAD_SIZE + JMESH_PDU_NETWORK_CONTROL_MIC_SIZE)/**< 12+8=20 */

#define JMESH_PDU_ACCESS_SEGMENT_PACKAGE_SIZE \
    (JMESH_PDU_SEGMENT_HEAD_SIZE + JMESH_PDU_NETWORK_ACCESS_MIC_SIZE + JMESH_PDU_UPPER_ACCESS_MIC_SIZE)/**< 15+4+4=23 */
#define JMESH_PDU_CONTROL_SEGMENT_PACKAGE_SIZE \
    (JMESH_PDU_SEGMENT_SEGMENT_HEAD_SIZE + JMESH_PDU_NETWORK_CONTROL_MIC_SIZE)/**< 15+8=23 */

#define JMESH_PDU_ACCESS_UNSEGMENT_DATA_SIZE \
    (JMESH_PDU_UNSEGMENT_SIZE - JMESH_PDU_ACCESS_UNSEGMENT_PACKAGE_SIZE)/**< 256-20=236 */

//#define JMESH_PDU_ACCESS_UNSEGMENT_DATA_SIZE \
//    (31 - JMESH_PDU_ACCESS_UNSEGMENT_PACKAGE_SIZE)/**< 256-20=236 */

#define JMESH_PDU_CONTROL_UNSEGMENT_DATA_SIZE \
    (JMESH_PDU_UNSEGMENT_SIZE - JMESH_PDU_CONTROL_UNSEGMENT_PACKAGE_SIZE)/**< 256-20=236 */

#define JMESH_PDU_ACCESS_SEGMENT_DATA_SIZE \
    (JMESH_PDU_UNSEGMENT_SIZE - JMESH_PDU_ACCESS_SEGMENT_PACKAGE_SIZE)/**< 256-23=233 */

#define JMESH_PDU_CONTROL_SEGMENT_DATA_SIZE \
    (JMESH_PDU_UNSEGMENT_SIZE - JMESH_PDU_CONTROL_SEGMENT_PACKAGE_SIZE)/**< 256-23=233 */

#define JMESH_PDU_ADV_MAX_SIZE 31
#define JMESH_PDU_NETWORK_MIN_LEN JMESH_PDU_CONTROL_UNSEGMENT_PACKAGE_SIZE/**< ack pdu:12+4+4=20 */
#define JMESH_PDU_NETWORK_MAX_LEN JMESH_PDU_UNSEGMENT_SIZE

#define JMESH_PDU_INTERFACE_HEAD_SIZE 2/**< length + type */

#define JMESH_PDU_NETWORK_DST_OFFSET        ((int)(&((jmesh_pdu_t*)0)->network.DST))

#define JMESH_PDU_NETWORK_ENCRYPTION_START_OFFSET JMESH_NETWORK_DST_PDU_OFFSET

#define JMESH_PDU_NETWORK_HEAD_SIZE (JMESH_PDU_NETWORK_DST_OFFSET-JMESH_PDU_INTERFACE_HEAD_SIZE)


typedef struct st_jmesh_provisioning_pdu{
    unsigned char length;
    unsigned char pdu[1];
}jmesh_provisioning_pdu_t;

#include"../foundation/state/jmesh_appkey.h"
typedef struct st_jmesh_access_pdu{
    union{
        struct{
            unsigned short src;
            unsigned short dst;
            unsigned char length;
            unsigned char channel;
            union{
                unsigned short netkey_index;
                jmesh_appkey_t* appkey;
            };
        }head;
        struct{
            unsigned char rfu[12];
            unsigned char data[1];
        }unsegment;
        struct{
            unsigned char rfu[15];
            unsigned char data[20];
        }segment;
    };
}jmesh_access_pdu_t;
typedef struct st_jmesh_control_pdu{
    unsigned char length;
    unsigned char itf;
    unsigned short netkey_index;
    unsigned short src;
    unsigned short dst;
    unsigned char ttl;
    unsigned char rfu[2];
    unsigned char OPCODE:7;
    unsigned char SEG:1;
    unsigned char data[1];
}jmesh_control_pdu_t;
typedef struct st_jmesh_ble_pdu{
	unsigned char length;
	union{
		struct{
			unsigned char id;
			unsigned char is_server;
			unsigned char reason;
			unsigned short con_intv;
			unsigned char mac[1];
		}connect;
	};
}jmesh_ble_pdu_t;


struct st_jmesh_pdu{
    union{
        unsigned char length;
        unsigned char pdu[1];
        jmesh_adv_pdu_t adv;
        jmesh_proxy_pdu_t proxy;
        jmesh_network_pdu_t network;
        jmesh_provisioning_pdu_t provisioning;
        jmesh_access_pdu_t access;
        jmesh_control_pdu_t	control;
        jmesh_ble_pdu_t ble;
    };
};

typedef struct st_jmesh_pdu jmesh_pdu_t;

void jmesh_pdu_init(void);
jmesh_pdu_t* jmesh_pdu_new(int length);
void jmesh_pdu_free(void* pdu);

#define JMESH_PDU_NEW_SHORT() jmesh_pdu_new(JMESH_SHORT_PDU_SIZE)
#define JMESH_PDU_NEW_LONG() jmesh_pdu_new(JMESH_LONG_PDU_SIZE)


#endif // JMESH_PDU_H
