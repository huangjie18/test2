/*! @file _470_app.c
 * @brief 
 *
 */
#include "_470_phy.h"
#include "_470_app.h"
#include "protocol_mx188.h"
#include "jmesh_app_pro.h"
#include "APP_protocol.h"
#include "os_timer_event.h"

uint8_t mx188_seq = 0;

uint8_t get_new_mx188_seq(void)
{
	mx188_seq++;
	if(mx188_seq == 0)
	{
		mx188_seq = 1;
	}
	return mx188_seq;
}


static uint8_t check_cs(uint8_t *p, uint8_t len)
{
	uint8_t i;
	uint8_t sum = 0;
	for(i = 0; i < len-1; i++)
	{
		sum += *(p + i);
	}
	if(sum == *(p + len - 1))
		return 1;
	else 
		return 0;
}

static uint8_t creat_cs(uint8_t *p, uint8_t len)
{
	uint8_t sum = 0;
	uint8_t i;
	for(i = 0; i < len; i++)  //TBD
		sum += *(p + i);
	return sum;
}

static uint8_t check_meter_type(uint8_t type)
{
	return (type == 0x10 || type == 0x20 || type == 0x40 || type == 0x80 || type ==0x04);
}

static addr_t get_addr(uint8_t *p)
{
	addr_t addr;
	char i;
	for(i = 0;i < 5; i++)
		addr.u8[i] = *p++;
	return addr;
}

/***************************************************************************
* Function Name: fetch_routes()
* Arguments    : void *p - point to data buffer, frame_t *frm - reference to frm
* Return Value : return the parse status
* Description  : Creat the frame, add the header to the data buffer.
***************************************************************************/
static uint8_t fetch_routers(void *p, mx188_frame_t *frm)
{
	uint8_t i,rl;
	uint8_t *dptr;
	dptr = p;
	rl = frm->CTR_20.repeat_level;
	memset(&frm->route,0,sizeof(frm->route));
	
	if(rl <= MAX_RELAY)// 0~4
	{
		for(i = 0;i < rl;i++)
		{
			frm->route.route[i] = get_addr(dptr);
			dptr = dptr + METER_ADDR_SIZE;                  
		}
		return rl;
	}else{
		return UNSUPPORTED_PROTOCAL;
	}
}

// 将buffer_t类型的数据解析为结构体 mx188_frame_t ,
// 原LoRa工程的代码，参数buf->buf[0]存放的是长度，接着才是数据，buf->dptr指向buf[0]
uint8_t protocol_mx188_parse(buffer_t *buf, mx188_frame_t *frame)
{
	uint8_t *p, *tmp;
	uint8_t len;
	uint8_t frame_start, frame_end;
	uint8_t start_id[2];
	uint8_t rn;
	p = buf->dptr;
	len = buf->buf[0];
	p++;	//TBD; consider the radio receive implementation;
	
	frame_start = *p;
	frame_end = *(p + len - 1);	
	frame->meter_type = *(p + 1);
	
	if((frame_start == PACKET_FRAME_START) && (frame_end == PACKET_FRAME_END) 
	   && (check_meter_type(frame->meter_type) == 1) && (check_cs(p, len-1) == 1))
	{
		p = p + 2;
		frame->addr = get_addr(p);
		p += METER_ADDR_SIZE;
		
		tmp = p++;
		frame->CTR_20 = *((ctr20_t *)tmp);
		tmp = p++;
		frame->CTR_10 = *((ctr10_t *)tmp);

		frame->seq = *p++;
		rn = fetch_routers(p,frame);
		if(UNSUPPORTED_PROTOCAL == rn)
		{
			return FRAMER_FAILED;	
		}
		
		p = p + METER_ADDR_SIZE * rn;
		
		frame->payload_len = *p++;
		start_id[0] = *p++;
		start_id[1] = *p++;
		
		if((start_id[0] != PACKET_START_ID)||(start_id[1] != PACKET_START_ID))
		{
			return FRAMER_FAILED;
		}
		
		if(frame->CTR_10.respond_flag == 0)
		{
			frame->DI0 = *p++;
			frame->DI1 = *p++;
			buf->len = frame->payload_len - 4;// up , last 4B ,unused
		}
		else
		{
			buf->len = frame->payload_len - 2;// up , last 4B ,unused
		}
		
		buf->dptr = p;
		
		p+= buf->len;
		
		frame->DY_RSSI = *p++;
		frame->DOWN_RSSI = *p++;
		frame->UP_RSSI = *p++;
		frame->protocal = *p++;
		
		frame->buffer = buf;		
		
		return FRAMER_SUCCESS;
	}
	else
	{
		return FRAMER_FAILED;
	}
}


// 将buffer_t类型的数据解析为结构体 mx188_frame_t
// 注意本函数中未对frame->buffer赋值。
//
uint8_t protocol_mx188_parse2(uint8_t *data, uint8_t len, mx188_frame_t *frame)
{
	uint8_t *p, *tmp;
	uint8_t frame_start, frame_end;
	uint8_t start_id[2];
	uint8_t rn;
	
	p = data;	
	frame_start = *p;
	frame_end = *(p + len - 1);	
	frame->meter_type = *(p + 1);
	
	if((frame_start == PACKET_FRAME_START) && (frame_end == PACKET_FRAME_END) 
	   && (check_meter_type(frame->meter_type) == 1) && (check_cs(p, len-1) == 1))
	{
		p = p + 2;
		frame->addr = get_addr(p);
		p += METER_ADDR_SIZE;
		
		tmp = p++;
		frame->CTR_20 = *((ctr20_t *)tmp);
		tmp = p++;
		frame->CTR_10 = *((ctr10_t *)tmp);

		frame->seq = *p++;
		rn = fetch_routers(p,frame);
		if(UNSUPPORTED_PROTOCAL == rn)
		{
			return FRAMER_FAILED;	
		}
		
		p = p + METER_ADDR_SIZE * rn;
		
		frame->payload_len = *p++;
		start_id[0] = *p++;
		start_id[1] = *p++;
		
		if((start_id[0] != PACKET_START_ID)||(start_id[1] != PACKET_START_ID))
		{
			return FRAMER_FAILED;
		}
		
		if(frame->CTR_10.respond_flag == 0)
		{
			frame->DI0 = *p++;
			frame->DI1 = *p++;
			frame->valid_data_offset = p - data;
			frame->valid_data_len = frame->payload_len - 4;			
		}
		else
		{
			frame->valid_data_offset = p - data;
			frame->valid_data_len = frame->payload_len - 2;// up , last 4B ,unused
		}
		p += frame->valid_data_len;	
		
		frame->DY_RSSI = *p++;
		frame->DOWN_RSSI = *p++;
		frame->UP_RSSI = *p++;
		frame->protocal = *p++;
						
		return FRAMER_SUCCESS;
	}
	else
	{
		return FRAMER_FAILED;
	}
}

// frame：包含地址，控制字，路由信息等信息
// payload:构成188帧的有效数据指针，
// payload_len：有效数据长度 
uint8_t protocol_mx188_pack(mx188_frame_t *frame, uint8_t payload_len, uint8_t *payload, uint8_t *buffer, uint8_t buffer_size)
{
	int len = 0, i;
	uint8_t *p;
	uint8_t *temp;
	uint8_t j;
	
	p = buffer;
	len = payload_len;
	
	
	if(frame->CTR_10.respond_flag)
	{
		len += NO_DATA_ID_LEN;		
	}
	else 
	{
		len += WITH_DATA_ID_LEN;
	}
	if(frame->CTR_20.repeat_level <= 0x04)
	{
		len += (NO_ROUTE_HEADER_LEN + METER_ADDR_SIZE*frame->CTR_20.repeat_level);
	}
	else
	{
		return FRAMER_FAILED;		
	}
	len += 6;
	if(len > buffer_size) // 到这里len为总帧长
		return FRAMER_FAILED;
	
	temp = p;
	
	*p++ = PACKET_FRAME_START;
	*p++ = frame->meter_type;
	
	for(i = 0;i < 5; i++)
	{
		*p++ = frame->addr.u8[i];
	}
	//initialize the repeat index
	frame->CTR_20.repeat_index = frame->CTR_20.repeat_level;
	*p++ = *((uint8_t *)(&frame->CTR_20));
	*p++ = *((uint8_t *)(&frame->CTR_10));
	
	*p++ = frame->seq;

	for(i = 0;i < frame->CTR_20.repeat_level; i++)
	{
		for(j = 0; j < METER_ADDR_SIZE; j++)
			*p++ = frame->route.route[i].u8[j];	
	}
	
	if(frame->CTR_10.respond_flag == 0)
	{
		*p++ = payload_len + 4;
	}
	else
	{
		*p++ = payload_len + 2;
	}
	
	
	*p++ = PACKET_START_ID;
	*p++ = PACKET_START_ID;
	if(frame->CTR_10.respond_flag == 0)
	{
		*p++ = frame->DI0;
		*p++ = frame->DI1;
	}
	
	if (payload_len > 0)
	{
		memcpy(p, payload, payload_len);
		p += payload_len;
	}
	
	p = temp;/*rollback*/
	
	*(p + len - 6) = frame->DY_RSSI;
	*(p + len - 5) = frame->DOWN_RSSI;
	*(p + len - 4) = frame->UP_RSSI;
	*(p + len - 3) = frame->protocal;
	
	//p = shift_buf_left(p, len, 2);
	*(p + len - 2) = creat_cs(p, len - 2);
	*(p + len - 1) = PACKET_FRAME_END;
	
	return len;
}



