//---------------------------------------------------
// file name : mx188_pro.h
//--------------------------------------------------- 
#ifndef MX188_PRO_H
#define MX188_PRO_H
#include <stdint.h>

#define MAX_RELAY	4
#define UNSUPPORTED_PROTOCAL	MAX_RELAY+1
#define PACKET_START_ID 0x2F
#define METER_ADDR_SIZE 5
#define PACKET_FRAME_START 0x68
#define PACKET_FRAME_END   0x16
#define NO_DATA_ID_LEN 2
#define WITH_DATA_ID_LEN 4
#define NO_ROUTE_HEADER_LEN 11
#define MAX_PACKETBUF_SIZE 240 // TODO

enum
{
	FROM_HOST_NODE = 0,
	FROME_SLAVE_NODE = 1,
};

enum
{
	READ_DATA = 1,
	WRITE_METER_DATA = 3,
	WRITE_DATA = 4,
};

enum
{
	FRAMER_FAILED = 0,
	FRAMER_SUCCESS = 1,
};


typedef union 
{
	unsigned char u8[5];
} addr_t;

typedef struct
{
    addr_t route[MAX_RELAY];
	//addr_t route_first;
	//addr_t route_second;
}addr_ext;

typedef struct
{
	uint8_t reserve:2;//reserved value:2
	uint8_t repeat_index:3;
	uint8_t repeat_level:3;
}ctr20_t;

typedef struct
{
	uint8_t function:6;
	uint8_t respond_flag:1;
	uint8_t direction:1;
}ctr10_t;

typedef struct _buffer_t
{
    struct      _buffer_t *next;                ///< Next pointer
    uint8_t          alloc;                          ///< Alloc flag
    uint8_t          *dptr;                          ///< Data pointer - points to current position in buffer array
    uint8_t          len;                            ///< Len of the data
    uint8_t          rssi;                            ///< Link quality indicator
    uint8_t          index;                          ///< Index used for buffer tracking and debugging
    uint8_t          buf[256];       ///< Main data storage //aMaxPHYPacketSize 
} buffer_t;

typedef struct 
{ 
	uint8_t meter_type;
	addr_t addr;  
	ctr20_t CTR_20;
	ctr10_t CTR_10;
	addr_ext route;
	uint8_t seq;
	uint8_t payload_len;
	
	uint8_t DI0;
	uint8_t DI1;
	uint8_t DY_RSSI;
	uint8_t DOWN_RSSI;
	uint8_t UP_RSSI;
	uint8_t protocal;
	uint8_t valid_data_offset;   //有效数据在数组的偏移量 
	uint8_t valid_data_len;      //有效数据长度  
	buffer_t *buffer;     
} mx188_frame_t;

extern uint8_t mx188_seq;

uint8_t protocol_mx188_parse(buffer_t *buf, mx188_frame_t *frame);
uint8_t protocol_mx188_parse2(uint8_t *data, uint8_t len, mx188_frame_t *frame);
uint8_t protocol_mx188_pack(mx188_frame_t *frame, uint8_t payload_len, uint8_t *payload, uint8_t *buffer, uint8_t buffer_size);
uint8_t get_new_mx188_seq(void);
#endif	//MX188_PRO_H
